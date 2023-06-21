#ifndef _ESP_ARRAY_H_
#define _ESP_ARRAY_H_

#include <Arduino.h>
#include <type_traits>
#include <esp_err.h>

#include "dsps_conv.h"
#include "dsps_corr.h"
#include "dsps_dotprod.h"

#include "esp_opt.h"
#include "esp_dsp.h"
#include "esp_ansi.h"

#if BENCHMARK_TEST
#include "hard_debug.h" // https://github.com/guilhAbreu/EspDebug

#define REPORT_BENCHMARK(title, func1, ...)\
{\
  func1(__VA_ARGS__); /* warm up the cache */ \
  unsigned intlevel = dsp_ENTER_CRITICAL(); \
  uint32_t func1_start = xthal_get_ccount(); \
  func1(__VA_ARGS__); \
  uint32_t func1_end = xthal_get_ccount(); \
  dsp_EXIT_CRITICAL(intlevel); \
  debug.print(title + String(func1_end - func1_start)); \
}

#endif

#define exec_dsp(dsp_func, ...)\
{\
  unsigned intlevel = dsp_ENTER_CRITICAL();\
  dsp_func(__VA_ARGS__);\
  dsp_EXIT_CRITICAL(intlevel);\
}

/**
 * @brief Namespace for custom ESP32 MATH libraries
 * 
 */
namespace espmath{
  using namespace espmath;
  
  /**
   * @brief Custom Array implementation suitable for ESP32 devices.
   * 
   * This implements an array suitable for ESP32 boards.
   * Using this class, you can create an array of any arithmetic type.
   * 
   * @note Most of float and int16_t arithmetic operations make use of DSP instructions.
   * 
   * @tparam T Array type
   */
  template <typename T = float> class Array
  {
    static_assert(std::is_arithmetic<T>::value, "Array type must be arithmetic!");
    static_assert(sizeof(T) <= 4, "Long types are not supported!");
  public:

    typedef T* const arrayPntr;

    static const bool isDSPSupported(){return false;}

    /**
     * @brief Destroy the Array object
     * 
     */
    ~Array()
    {
      if (canBeDestroyed && _array)
        heap_caps_free(_array);
    }

    /**
     * @brief Construct a new Array object
     * 
     * @param initialMem The initial size of the array.
     * @param capabilities Memory capabilities.
     */
    Array(const size_t cols = 0, const size_t rows = 1, uint32_t capabilities = UINT32_MAX)
    {
      ESP_ERROR_CHECK(rows == 0); //"rows must be greater than 0!"
      if (capabilities != UINT32_MAX)
        _caps = capabilities;
      _rows = rows;
      _cols = cols;
       _size = _mem2alloc(_cols);
      _array = _size > 0 ? (T*)heap_caps_malloc(_size, _caps) : NULL;
      if(!_array)
        _size = 0;
    }
    
    /**
     * @brief Construct a new Array object with initial values
     * 
     * @param initialValues Initial values of the array.
     * @param initialMem The initial size of the array.
     * @param capabilities Memory capabilities
     */
    Array(const T* initialValues,\
          const size_t cols = 0,\
          const size_t rows = 1,\
          uint32_t capabilities = UINT32_MAX):Array(cols, rows, capabilities)
    {
      if (_array)
        cpyArray(initialValues, _array, _cols);
    }

    /**
     * @brief Constructor
     * 
     * @param another 
     */
    Array(Array& another){copy(another);}
    Array(Array&& another){copy(another);}

    /**
     * @brief Assign operation
     * 
     * @param another 
     */
    void operator=(Array& another){copy(another);}
    void operator=(Array&& another){copyRef(another);}

    /**
     * @brief Get array element
     * 
     * @param index 
     * @return T 
     */
    T* operator[](const size_t index)
    {
      return &_array[_cols*index];
    }

    T* operator()(const size_t i = 0)
    {
      return &_array[i];
    }

    /**
     * @brief Filter the array removing the elements where filter is false.
     * 
     * Example:
     * filter = {0, 1, 0, 1, 1};
     * array  = {1, 2, 3, 4, 5};
     * 
     * array[filter] -> {2,4,5}.
     * 
     * @param filter The array filter
     * @return Array
     */
    Array operator[](Array filter)
    {
      Array<T> newArray();
      for (size_t i = 0; i < _cols; i++)
        if(filter()[i]) newArray << _array[i];
      return newArray;
    }

    /**
     * @brief Get the boolean array indicating where the value is the same as the element.
     * 
     * @param value The value to be verified.
     * @return Array.
     */
    Array operator==(const T value)
    {
      Array<T> newArray(_cols, _rows);
      for (size_t i = 0; i < _rows*_cols; i++)
        _array[i] == value ? newArray()[i] = 1 : 0;
      
      return newArray;
    }

    /**
     * @brief Get the boolean array indicating where the value is different than the element.
     * 
     * @param value The value to be verified.
     * @return Array.
     */
    Array operator!=(const T value) const
    {
      Array<T> newArray(_cols, _rows);
      for (size_t i = 0; i < _rows*_cols; i++)
        _array[i] != value ? newArray()[i] = 1 : 0;
      
      return newArray;
    }

    /**
     * @brief Get the boolean array indicating where the value is greater than the element.
     * 
     * @param value The value to be verified.
     * @return Array.
     */
    Array operator>(const T value) const
    {
      Array<T> newArray(_cols, _rows);
      for (size_t i = 0; i < _rows*_cols; i++)
        _array[i] > value ? newArray()[i] = 1 : 0;
      
      return newArray;
    }

    /**
     * @brief Get the boolean array indicating where the value is less than the element.
     * 
     * @param value The value to be verified.
     * @return Array.
     */
    Array operator<(const T value) const
    {
      Array<T> newArray(_cols, _rows);
      for (size_t i = 0; i < _rows*_cols; i++)
        _array[i] < value ? newArray()[i] = 1 : 0;
      
      return newArray;
    }

    /**
     * @brief Get the boolean array indicating where the value is greater or equal to the element.
     * 
     * @param value The value to be verified.
     * @return Array.
     */
    Array operator>=(const T value) const
    {
      Array<T> newArray(_cols, _rows);
      for (size_t i = 0; i < _rows*_cols; i++)
        _array[i] >= value ? newArray()[i] = 1 : 0;
      
      return newArray;
    }

    /**
     * @brief Get the boolean array indicating where the value is less or equal to the element.
     * 
     * @param value The value to be verified.
     * @return Array.
     */
    Array operator<=(const T value) const
    {
      Array<T> newArray(_cols, _rows);
      for (size_t i = 0; i < _rows*_cols; i++)
        _array[i] <= value ? newArray()[i] = 1 : 0;
      
      return newArray;
    }

    /**
     * @brief copy()[i] = !array()[i], i = 0,1,2,3...
     * 
     * @return Array 
     */
    Array operator!()
    {
      Array<T> newArray(_cols, _rows);
      for (size_t i = 0; i < _rows*_cols; i++)
        newArray()[i] = !_array[i];
      
      return newArray;
    }

    /**
     * @brief Perform a binary not on every element and return the result
     * 
     * @return Array 
     */
    Array operator~()
    {
      Array<T> newArray(_cols, _rows);
      for (size_t i = 0; i < _rows*_cols; i++)
        newArray()[i] = ~_array[i];
      
      return newArray;
    }

    /**
     * @brief Verify if two arrays are identical.
     * 
     * @param input The input _array
     * @return true Every value of the _array is contained in input.
     * @return false Not all values of the _array are contained in input.
     */
    const bool operator==(const T* input)
    {
      size_t i = 0;
      while( i < _rows*_cols)
      {
        if (_array[i] != input[i])
          return false;
        i++;
      }
      return true;
    }

    /**
     * @brief Verify if two arrays are identical.
     * 
     * @param input The input array
     * @return true Every value of the _array is contained in input.
     * @return false Not all values of the _array are contained in input.
     */
    const bool operator==(Array& another)
    {
      bool result = *this == (T*)another;
      return result;
    }
    const bool operator==(Array&& another)
    {
      bool result = *this == (T*)another;
      return result;
    }

    /**
     * @brief Verify if two arrays are not identical.
     * 
     * @param input The input _array
     * @return true Not all values of the _array are contained in input.
     * @return false Every value of the _array is contained in input.
     */
    const bool operator!=(const T* input)
    {
      return !(*this == input);
    }

    /**
     * @brief Add a constant
     * 
     * @param value 
     * @note Float arrays make use of DSP instructions.
     */
    void operator+=(const T value)
    {
      addConstToArray(_array, _array, _cols, value);
    }

    /**
     * @brief Subtract a constant
     * 
     * @param value 
     * @note Float arrays make use of DSP instructions.
     */
    void operator-=(const T value)
    {
      *this += value*(-1);
    }

    /**
     * @brief Multiply by a constant
     * 
     * @param value 
     * @note Float arrays make use of DSP instructions.
     */
    void operator*=(const T value)
    { 
      mulConstByArray(_array, _array, _cols, value);
    }

    /**
     * @brief Divide by a constant
     * 
     * @param value 
     * @note Float arrays make use of DSP instructions.
     */
    void operator/=(const T value)
    {
      divArrayByConst(_array, _array, _cols, value);
    }

    /**
     * @brief Add an array
     * 
     * @param another
     * @note Float and int16_t arrays make use of DSP instructions.
     */
    void operator+=(Array& another)
    { 
      addArrayToArray((T*)another, _array, _array, _cols);
    }
    void operator+=(Array&& another)
    {
      *this+=another;
    }

    /**
     * @brief Subtract an array
     * 
     * @param another 
     * @note Float arrays make use of DSP instructions.
     */
    void operator-=(Array& another)
    {
      subArrayFromArray((T*)another, _array, _array, _cols);
    }
    void operator-=(Array&& another)
    {
      *this-=another;
    }

    /**
     * @brief Multiply by an array
     * 
     * @param another 
     * @note Float and int16_t arrays make use of DSP instructions.
     */
    void operator*=(Array& another)
    {
      mulArrayByArray((T*)another, _array, _array, _cols);
    }
    void operator*=(Array&& another)
    {
      *this*=another;
    }

    /**
     * @brief Divide by an array
     * 
     * @param another 
     * 
     */
    void operator/=(Array& another)
    {
      divArrayByArray(_array, another, _array, _cols);
    }
    void operator/=(Array&& another)
    {
      *this/=another;
    }

    /**
     * @brief Concatenate a value
     * 
     * @param value 
     * @return Array& 
     */
    Array& operator<<(const T value)
    {
      this->append(value);
      return *this;
    }

    /**
     * @brief Concatenate an array
     * 
     * @param another 
     * @return Array& 
     */
    Array& operator<<(Array& another)
    {
      for(size_t i = 0; i < another.cols(); i++)
      {
        *this = *this << another()[i];
      }
      return *this;
    }

    // /**
    //  * @brief Implicit cast for array pointer
    //  * 
    //  * @return arrayPntr 
    //  */
    // operator arrayPntr() const {return _array;}

    template<typename _type>
    operator _type*() const {return (_type*)_array;}

    /**
     * @brief Append a value to the _array
     * 
     * @param value 
     * @return true Successful appended
     * @return false Failed to append
     */
    const bool append(const T value)
    {
      assert(_rows == 1);
      if(_cols < _size/sizeof(T))
      {
        (*this)[_cols++] = value;
        return true;
      }

      _size = _mem2alloc(_cols+1);

      if (_array)
      {
        heap_caps_free(_array);
        _array = _size > 0 ? (T*)heap_caps_malloc(_size, _caps) : NULL;
      }
      else
      {
        _array = _size > 0 ? (T*)heap_caps_malloc(_size, _caps) : NULL;
      }

      if (_array)
      {
        (*this)[_cols++] = value;
        return true;
      }
      return false;
    }

    /**
     * @brief Concatenate itself to another
     * 
     * @param another 
     * @return true Successful concatenation
     * @return false Failure during concatenation
     */
    const bool append(Array& another)
    {
      *this << another;
      return _array == NULL ? false : true;
    }

    /**
     * @brief Get the memory capabilities
     * 
     * @return const uint32_t 
     */
    const uint32_t capabilities(){return _caps;}

    /**
     * @brief Get the convolution of the array by the given kernel
     * 
     * @param kernel 
     * @return Array output array with convolution result length of (siglen + Kernel -1)
     * 
     * @note Float arrays make use of DSP instructions.
     */
    Array conv(Array& kernel)
    {
      const size_t outputLength = _cols + kernel.cols() -1;
      Array convOutput(outputLength);

      float output[outputLength];
      float itself[_cols];
      float _kernel[kernel.cols()];

      cpyArray(_array, itself, _cols);
      cpyArray(kernel, _kernel, kernel.cols());
      exec_dsp(dsps_conv_f32_ae32, itself, _cols, _kernel, kernel.cols(), output);
      cpyArray(output, convOutput, outputLength);
      
      return convOutput;
    }

    /**
     * @brief Copy another array into this one
     * 
     * @param another 
     */
    void copy(Array& another)
    {
      if (_array)
        heap_caps_free(_array);

      _rows = another._rows;
      _cols = another._cols;
      _size = another.memSize();
      _array = _size > 0 ? (T*)heap_caps_malloc(_size, _caps) : NULL;
      
      for(size_t i = 0; i < _cols; i++)
        _array[i] = another()[i];
    }

    /**
     * @brief Copy array reference and prevents array from being freed by the destructor
     * 
     * @param another 
     */
    void copyRef(Array& another)
    {
      if (_array)
        heap_caps_free(_array);

      _rows = another._rows;
      _cols = another._cols;
      _size = another.memSize();
      _array = another.preserveMem();
    }

    /**
     * @brief Get the correlation array with the given pattern
     * 
     * @param pattern 
     * @return Array<float>
     * 
     * @note Float arrays make use of DSP instructions.
     */
    Array<float> correlation(Array& pattern)
    {
      Array<float> corr(_cols, _rows);

      float itself[_cols];
      float _pattern[pattern.cols()];

      cpyArray(_array, itself, _cols);
      cpyArray(pattern, _pattern, pattern.cols());
      exec_dsp(dsps_corr_f32_ae32, itself, _cols, _pattern, pattern.cols(), corr);
      return corr;
    }

    /**
     * @brief Compares to another array
     * 
     * @param another Another array
     * @param EPSILON tolerance. Only used when comparing float arrays.
     * @return true Two differents arrays
     * @return false Array are not different
     */
    const bool diff(Array& another, const float EPSILON = 0.0001)
    {
      size_t i = 0;
      while (i < _rows*_cols)
      {
        if (_array[i]!=another()[i])
          return true;
        i++;
      }
      return false;
    }

    /**
     * @brief Get the Array Pointer
     * 
     * @return void*
     */
    const T* getArrayPntr() const
    {
      return _array;
    }

    size_t cols() const{return _cols;}
    size_t rows() const{return _rows;}

    /**
     * @brief Get the allocated memory bytes
     * 
     * @return size_t 
     */
    size_t memSize() const
    {
      return _size;
    }

  protected:
    size_t _size = 0; /*Total bytes allocated*/
    size_t _rows = 1; /*Number of rows if Array is a matrix*/
    size_t _cols = 0; /*Number of columns if Array is a matrix*/
    T* _array = NULL; /*Array pointer*/
    uint32_t _caps = this->memCaps();

  private:
    bool canBeDestroyed = true;

    /**
     * @brief Get the total bytes to be allocated
     * 
     * @param min The minimum quantity of memory blocks
     * @return size_t 
     */
    size_t _mem2alloc(const size_t min)
    {
      size_t minBytes = min*sizeof(T);
      size_t extraBytes = extra2align(minBytes);
      return minBytes + extraBytes;
    }

    /**
     * @brief Get the extra quantity bytes to be allocated and align array
     * 
     * @param min Minumum quantity of bytes
     * @return size_t Result
     */
    static size_t extra2align(const size_t min)
    {
      return (ALIGNMENT - min%ALIGNMENT)%ALIGNMENT;
    }

    /**
     * @brief Return the allocated memory capabilities.
     * 
     * @return uint32_t
     */
    static const uint32_t memCaps()
    {
      return MALLOC_CAP_8BIT;
    }

    /**
     * @brief Prevents array from being freed by the destructor
     * 
     * @return T* 
     */
    T* preserveMem()
    {
      canBeDestroyed = false;
      return _array;
    }
  };

  template<>
  const uint32_t Array<int32_t>::memCaps(){return MALLOC_CAP_32BIT;}
  template<>
  const uint32_t Array<uint32_t>::memCaps(){return MALLOC_CAP_32BIT;}

  template<>
  const bool Array<int32_t>::isDSPSupported(){return true;}
  template<>
  const bool Array<int16_t>::isDSPSupported(){return true;}
  template<>
  const bool Array<int8_t>::isDSPSupported(){return true;}
  template<>
  const bool Array<float>::isDSPSupported(){return true;}

  template<>
  inline void Array<float>::operator+=(const float value)
  {
    exec_dsp(dsps_addc_f32_esp, _array, _array, _cols, value);
  }

  template<>
  inline void Array<int32_t>::operator+=(const int32_t value)
  {
    exec_dsp(dsps_addc_s32_esp, _array, _array, _cols, value);
  }

  template<>
  inline void Array<uint32_t>::operator+=(const uint32_t value)
  {
    exec_dsp(dsps_addc_s32_esp, (int32_t*)_array, (int32_t*)_array, _cols, value);
  }

  template<>
  inline void Array<int16_t>::operator+=(const int16_t value)
  {
    exec_dsp(dsps_addc_s16_esp, _array, _array, _cols, &value);
  }

  template<>
  inline void Array<int8_t>::operator+=(const int8_t value)
  {
    exec_dsp(dsps_addc_s8_esp, _array, _array, _cols, &value);
  }

  template<>
  inline void Array<float>::operator-=(const float value)
  {
    exec_dsp(dsps_subc_f32_esp, _array, _array, _cols, value);
  }

  template<>
  inline void Array<int32_t>::operator-=(const int32_t value)
  {
    exec_dsp(dsps_subc_s32_esp, _array, _array, _cols, value);
  }

  template<>
  inline void Array<uint32_t>::operator-=(const uint32_t value)
  {
    exec_dsp(dsps_subc_s32_esp, (int32_t*)_array, (int32_t*)_array, _cols, value);
  }

  template<>
  inline void Array<int16_t>::operator-=(const int16_t value)
  {
    exec_dsp(dsps_subc_s16_esp, _array, _array, _cols, &value);
  }

  template<>
  inline void Array<int8_t>::operator-=(const int8_t value)
  {
    exec_dsp(dsps_subc_s8_esp, _array, _array, _cols, &value);
  }

  template<>
  inline void Array<float>::operator*=(const float value)
  {
    exec_dsp(dsps_mulc_f32_esp, _array, _array, _cols, value);
  }

  template<>
  inline void Array<int32_t>::operator*=(const int32_t value)
  {
    exec_dsp(dsps_mulc_s32_esp, _array, _array, _cols, value);
  }

  template<>
  inline void Array<uint32_t>::operator*=(const uint32_t value)
  {
    exec_dsp(dsps_mulc_s32_esp, (int32_t*)_array, (int32_t*)_array, _cols, (int32_t)value);
  }

  template<>
  inline void Array<int8_t>::operator*=(const int8_t value)
  {
    exec_dsp(dsps_mulc_s8_esp, _array, _array, _cols, &value);
  }

  template<>
  inline void Array<int16_t>::operator*=(const int16_t value)
  {
    exec_dsp(dsps_mulc_s16_esp, _array, _array, _cols, value);
  }

  template<>
  inline void Array<float>::operator/=(const float value)
  {
    exec_dsp(dsps_divc_f32_esp, _array, _array, _cols, value);
  }

  template<>
  inline void Array<int32_t>::operator/=(const int32_t value)
  {
    exec_dsp(dsps_divc_s32_esp, _array, _array, _cols, value);
  }

  template<>
  inline void Array<uint32_t>::operator/=(const uint32_t value)
  {
    exec_dsp(dsps_divc_s32_esp, (int32_t*)_array, (int32_t*)_array, _cols, (int32_t)value);
  }

  template<>
  inline void Array<int16_t>::operator/=(const int16_t value)
  {
    exec_dsp(dsps_divc_s16_esp, _array, _array, _cols, value);
  }

  template<>
  inline void Array<int8_t>::operator/=(const int8_t value)
  {
    exec_dsp(dsps_divc_s8_esp, _array, _array, _cols, value);
  }

  template<>
  inline void Array<float>::operator+=(Array<float>& another)
  {
    exec_dsp(dsps_add_f32_esp, _array, another, _array, _cols);
  }

  template<>
  inline void Array<int32_t>::operator+=(Array<int32_t>& another)
  {
    exec_dsp(dsps_add_s32_esp, _array, another, _array, _cols);
  }

  template<>
  inline void Array<uint32_t>::operator+=(Array<uint32_t>& another)
  {
    exec_dsp(dsps_add_s32_esp, (int32_t*)_array, (int32_t*)another, (int32_t*)_array, _cols);
  }

  template<>
  inline void Array<int16_t>::operator+=(Array<int16_t>& another)
  {
    exec_dsp(dsps_add_s16_esp, _array, another, _array, _cols);
  }

  template<>
  inline void Array<int8_t>::operator+=(Array<int8_t>& another)
  {
    exec_dsp(dsps_add_s8_esp, _array, another, _array, _cols);
  }

  template<>
  inline void Array<float>::operator-=(Array<float>& another)
  {
    exec_dsp(dsps_sub_f32_esp, _array, another, _array, _cols);
  }

  template<>
  inline void Array<int32_t>::operator-=(Array<int32_t>& another)
  {
    exec_dsp(dsps_sub_s32_esp, _array, another, _array, _cols);
  }

  template<>
  inline void Array<uint32_t>::operator-=(Array<uint32_t>& another)
  {
    exec_dsp(dsps_sub_s32_esp, (int32_t*)_array, (int32_t*)another, (int32_t*)_array, _cols);
  }

  template<>
  inline void Array<int16_t>::operator-=(Array<int16_t>& another)
  {
    exec_dsp(dsps_sub_s16_esp, _array, another, _array, _cols);
  }

  template<>
  inline void Array<int8_t>::operator-=(Array<int8_t>& another)
  {
    exec_dsp(dsps_sub_s8_esp, _array, another, _array, _cols);
  }

  template<>
  inline void Array<float>::operator*=(Array<float>& another)
  {
    exec_dsp(dsps_mul_f32_esp, _array, another, _array, _cols);
  }

  template<>
  inline void Array<int32_t>::operator*=(Array<int32_t>& another)
  {
    exec_dsp(dsps_mul_s32_esp, _array, another, _array, _cols);
  }

  template<>
  inline void Array<uint32_t>::operator*=(Array<uint32_t>& another)
  {
    exec_dsp(dsps_mul_s32_esp, (int32_t*)_array, (int32_t*)another, (int32_t*)_array, _cols);
  }

  template<>
  inline void Array<int16_t>::operator*=(Array<int16_t>& another)
  {
    exec_dsp(dsps_mul_s16_esp,_array, another, _array, _cols);
  }

  template<>
  inline void Array<int8_t>::operator*=(Array<int8_t>& another)
  {
    exec_dsp(dsps_mul_s8_esp,_array, another, _array, _cols);
  }

  template<>
  inline void Array<float>::operator/=(Array<float>& another)
  {
    exec_dsp(dsps_div_f32_esp, _array, another, _array, _cols);
  }

  template<>
  inline void Array<int32_t>::operator/=(Array<int32_t>& another)
  {
    exec_dsp(dsps_div_s32_esp, _array, another, _array, _cols);
  }

  template<>
  inline void Array<int16_t>::operator/=(Array<int16_t>& another)
  {
    exec_dsp(dsps_div_s16_esp, _array, another, _array, _cols);
  }

  template<>
  inline void Array<int8_t>::operator/=(Array<int8_t>& another)
  {
    exec_dsp(dsps_div_s8_esp, _array, another, _array, _cols);
  }

  template<>
  inline Array<float> Array<float>::operator==(const float value)
  {
    Array<float> newArray(_cols, _rows);
    for (size_t i = 0; i < _rows*_cols; i++)
      eqFloats(_array[i], value) ? newArray()[i] = 1 : 0;
    return newArray;
  }

  template<>
  inline const bool Array<float>::operator==(const float* input)
  {
    size_t i = 0;
    while( i < _rows*_cols)
    {
      if (!eqFloats(_array[i], input[i]))
        return false;
      i++;
    }
    return true;
  }

  template<>
  inline Array<float> Array<float>::conv(Array<float>& kernel)
  {
    const size_t outputLength = _cols + kernel.cols() -1;
    Array<float> convOutput(outputLength);
    exec_dsp(dsps_conv_f32_ae32, _array, _cols, kernel, kernel.cols(), convOutput);
    return convOutput;
  }

  template<>
  inline Array<float> Array<float>::correlation(Array<float>& pattern)
  {
    Array<float> corr(_cols, _rows);
    exec_dsp(dsps_corr_f32_ae32, _array, _cols, pattern, pattern.cols(), corr);
    return corr;
  }

  template<>
  inline const bool Array<float>::diff(Array<float>& another, const float EPSILON)
  {
    size_t i = 0;
    while(i < _cols)
    {
      if (!eqFloats(_array[i], another()[i], EPSILON))
        return true;
      i++;
    }
    return false;
  }

  /**
   * @brief Add two arrays
   * 
   * @tparam T 
   * @param onearray 
   * @param another 
   * @return Array<T>& 
   */
  template<typename T>
  inline Array<T> operator+(Array<T>& onearray, Array<T> another)
  {
    Array<T> newArray(onearray.cols());
    addArrayToArray<T>((T*)onearray, (T*)another, (T*)newArray, onearray.cols());
    return newArray;
  }

  template<>
  inline Array<float> operator+(Array<float>& onearray, Array<float> another)
  {
    Array<float> newArray(onearray.cols());
#if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_add_f32_esp, onearray, another, newArray, onearray.cols());
#else
    exec_dsp(dsps_add_f32_esp, onearray, another, newArray, onearray.cols());
#endif
    return newArray;
  }

  template<>
  inline Array<int32_t> operator+(Array<int32_t>& onearray, Array<int32_t> another)
  {
    Array<int32_t> newArray(onearray.cols());
#if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_add_s32_esp, onearray, another, newArray, onearray.cols());
#else
    exec_dsp(dsps_add_s32_esp, onearray, another, newArray, onearray.cols());
#endif
    return newArray;
  }

  template<>
  inline Array<uint32_t> operator+(Array<uint32_t>& onearray, Array<uint32_t> another)
  {
    Array<uint32_t> newArray(onearray.cols());
#if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ",\
                    dsps_add_s32_esp,\
                    (int32_t*)onearray,\
                    (int32_t*)another,\
                    (int32_t*)newArray,\
                    onearray.cols());
#else
    exec_dsp(dsps_add_s32_esp,\
            (int32_t*)onearray,\
            (int32_t*)another,\
            (int32_t*)newArray,\
            onearray.cols());
#endif
    return newArray;
  }

  template<>
  inline Array<int16_t> operator+(Array<int16_t>& onearray, Array<int16_t> another)
  {
    Array<int16_t> newArray(onearray.cols());
#if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_add_s16_esp, onearray, another, newArray, onearray.cols());
#else
    exec_dsp(dsps_add_s16_esp, onearray, another, newArray, onearray.cols());
#endif
    return newArray;
  }

  template<>
  inline Array<int8_t> operator+(Array<int8_t>& onearray, Array<int8_t> another)
  {
    Array<int8_t> newArray(onearray.cols());
#if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_add_s8_esp, onearray, another, newArray, onearray.cols());
#else
    exec_dsp(dsps_add_s8_esp, onearray, another, newArray, onearray.cols());
#endif
    return newArray;
  }

  /**
   * @brief Add a constant to an array
   * 
   * @tparam T 
   * @param onearray 
   * @param value 
   * @return Array<T>& 
   */
  template<typename T>
  inline Array<T> operator+(Array<T>& onearray, const T value)
  {
    Array<T> newArray(onearray.cols());
    addConstToArray<T>(onearray, newArray, newArray.cols(), value);
    return newArray;
  }

  template<>
  inline Array<float> operator+(Array<float>& onearray, const float value)
  {
    Array<float> newArray(onearray.cols());
#if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_addc_f32_esp, onearray, newArray, newArray.cols(), value);
#else
    exec_dsp(dsps_addc_f32_esp, onearray, newArray, newArray.cols(), value);
#endif
    return newArray;
  }

  template<>
  inline Array<int32_t> operator+(Array<int32_t>& onearray, const int32_t value)
  {
    Array<int32_t> newArray(onearray.cols());
#if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_addc_s32_esp, onearray, newArray, newArray.cols(), value);
#else
    exec_dsp(dsps_addc_s32_esp, onearray, newArray, newArray.cols(), value);
#endif
    return newArray;
  }

  template<>
  inline Array<uint32_t> operator+(Array<uint32_t>& onearray, const uint32_t value)
  {
    Array<uint32_t> newArray(onearray.cols());
#if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_addc_s32_esp, (int32_t*)onearray, (int32_t*)newArray, newArray.cols(), value);
#else
    exec_dsp(dsps_addc_s32_esp, (int32_t*)onearray, (int32_t*)newArray, newArray.cols(), value);
#endif
    return newArray;
  }

  template<>
  inline Array<int16_t> operator+(Array<int16_t>& onearray, const int16_t value)
  {
    Array<int16_t> newArray(onearray.cols());
#if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_addc_s16_esp, onearray, newArray, newArray.cols(), &value);
#else
    exec_dsp(dsps_addc_s16_esp, onearray, newArray, newArray.cols(), &value);
#endif
    return newArray;
  }

  template<>
  inline Array<int8_t> operator+(Array<int8_t>& onearray, const int8_t value)
  {
    Array<int8_t> newArray(onearray.cols());
#if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_addc_s8_esp, onearray, newArray, newArray.cols(), &value);
#else
    exec_dsp(dsps_addc_s8_esp, onearray, newArray, newArray.cols(), &value);
#endif
    return newArray;
  }

  /**
   * @brief Add an array to a constant
   * 
   * @param onearray 
   * @return Array
   */
  template<typename T>
  inline Array<T> operator+(const T value, Array<T> onearray)
  {
    Array<T> newArray(onearray.cols());
    addConstToArray<T>(onearray, newArray, newArray.cols(), value);
    return newArray;
  }

  template<>
  inline Array<float> operator+(const float value, Array<float> onearray)
  {
    Array<float> newArray(onearray.cols());
#if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_addc_f32_esp, onearray, newArray, newArray.cols(), value);
#else
    exec_dsp(dsps_addc_f32_esp, onearray, newArray, newArray.cols(), value);
#endif
    return newArray;
  }

  template<>
  inline Array<int32_t> operator+(const int32_t value, Array<int32_t> onearray)
  {
    Array<int32_t> newArray(onearray.cols());
#if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_addc_s32_esp, onearray, newArray, newArray.cols(), value);
#else
    exec_dsp(dsps_addc_s32_esp, onearray, newArray, newArray.cols(), value);
#endif
    return newArray;
  }

  template<>
  inline Array<uint32_t> operator+(const uint32_t value, Array<uint32_t> onearray)
  {
    Array<uint32_t> newArray(onearray.cols());
#if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_addc_s32_esp, (int32_t*)onearray, (int32_t*)newArray, newArray.cols(), value);
#else
    exec_dsp(dsps_addc_s32_esp, (int32_t*)onearray, (int32_t*)newArray, newArray.cols(), value);
#endif
    return newArray;
  }

  template<>
  inline Array<int16_t> operator+(const int16_t value, Array<int16_t> onearray)
  {
    Array<int16_t> newArray(onearray.cols());
#if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_addc_s16_esp, onearray, newArray, newArray.cols(), &value);
#else
    exec_dsp(dsps_addc_s16_esp, onearray, newArray, newArray.cols(), &value);
#endif
    return newArray;
  }

  template<>
  inline Array<int8_t> operator+(const int8_t value, Array<int8_t> onearray)
  {
    Array<int8_t> newArray(onearray.cols());
#if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_addc_s8_esp, onearray, newArray, newArray.cols(), &value);
#else
    exec_dsp(dsps_addc_s8_esp, onearray, newArray, newArray.cols(), &value);
#endif
    return newArray;
  }

  /**
   * @brief Subtract an array from another
   * 
   * @tparam T 
   * @param onearray 
   * @param another 
   * @return Array<T>& 
   */
  template<typename T>
  inline Array<T> operator-(Array<T>& onearray, Array<T> another)
  {
    Array<T> newArray(onearray.cols());
    subArrayFromArray<T>((T*)onearray, (T*)another, (T*)newArray, onearray.cols());
    return newArray;
  }

  template<>
  inline Array<float> operator-(Array<float>& onearray, Array<float> another)
  {
    Array<float> newArray(onearray.cols());
#if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_sub_f32_esp, onearray, another, newArray, onearray.cols());
#else
    exec_dsp(dsps_sub_f32_esp, onearray, another, newArray, onearray.cols());
#endif
    return newArray;
  }

  template<>
  inline Array<int32_t> operator-(Array<int32_t>& onearray, Array<int32_t> another)
  {
    Array<int32_t> newArray(onearray.cols());
#if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_sub_s32_esp, onearray, another, newArray, onearray.cols());
#else
    exec_dsp(dsps_sub_s32_esp, onearray, another, newArray, onearray.cols());
#endif
    return newArray;
  }

  template<>
  inline Array<uint32_t> operator-(Array<uint32_t>& onearray, Array<uint32_t> another)
  {
    Array<uint32_t> newArray(onearray.cols());
#if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ",\
                    dsps_sub_s32_esp,\
                    (int32_t*)onearray,\
                    (int32_t*)another,\
                    (int32_t*)newArray,\
                    onearray.cols());
#else
    exec_dsp(dsps_sub_s32_esp,\
            (int32_t*)onearray,\
            (int32_t*)another,\
            (int32_t*)newArray,\
            onearray.cols());
#endif
    return newArray;
  }

  template<>
  inline Array<int16_t> operator-(Array<int16_t>& onearray, Array<int16_t> another)
  {
    Array<int16_t> newArray(onearray.cols());
#if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_sub_s16_esp, onearray, another, newArray, onearray.cols());
#else
    exec_dsp(dsps_sub_s16_esp, onearray, another, newArray, onearray.cols());
#endif
    return newArray;
  }

  template<>
  inline Array<int8_t> operator-(Array<int8_t>& onearray, Array<int8_t> another)
  {
    Array<int8_t> newArray(onearray.cols());
#if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_sub_s8_esp, onearray, another, newArray, onearray.cols());
#else
    exec_dsp(dsps_sub_s8_esp, onearray, another, newArray, onearray.cols());
#endif
    return newArray;
  }

  /**
   * @brief Substract an array from a constant
   * 
   * @tparam T 
   * @param onearray 
   * @param value 
   * @return Array<T>& 
   */
  template<typename T>
  inline Array<T> operator-(Array<T>& onearray, const T value)
  {
    Array<T> newArray(onearray.cols());
    subConstFromArray<T>(onearray, newArray, newArray.cols(), value);
    return newArray;
  }

  template<>
  inline Array<float> operator-(Array<float>& onearray, const float value)
  {
    Array<float> newArray(onearray.cols());
#if BENCHMARK_TEST
    REPORT_BENCHMARK("cycles to complete: ", dsps_subc_f32_esp, onearray, newArray, newArray.cols(), value);
#else
    exec_dsp(dsps_subc_f32_esp, onearray, newArray, newArray.cols(), value);
#endif
    return newArray;
  }

  template<>
  inline Array<int32_t> operator-(Array<int32_t>& onearray, const int32_t value)
  {
    Array<int32_t> newArray(onearray.cols());
#if BENCHMARK_TEST
    REPORT_BENCHMARK("cycles to complete: ", dsps_subc_s32_esp, onearray, newArray, newArray.cols(), value);
#else
    exec_dsp(dsps_subc_s32_esp, onearray, newArray, newArray.cols(), value);
#endif
    return newArray;
  }

  template<>
  inline Array<uint32_t> operator-(Array<uint32_t>& onearray, const uint32_t value)
  {
    Array<uint32_t> newArray(onearray.cols());
#if BENCHMARK_TEST
    REPORT_BENCHMARK("cycles to complete: ", dsps_subc_s32_esp, (int32_t*)onearray, (int32_t*)newArray, newArray.cols(), value);
#else
    exec_dsp(dsps_subc_s32_esp, (int32_t*)onearray, (int32_t*)newArray, newArray.cols(), value);
#endif
    return newArray;
  }

  template<>
  inline Array<int16_t> operator-(Array<int16_t>& onearray, const int16_t value)
  {
    Array<int16_t> newArray(onearray.cols());
#if BENCHMARK_TEST
    REPORT_BENCHMARK("cycles to complete: ", dsps_subc_s16_esp, onearray, newArray, newArray.cols(), &value);
#else
    exec_dsp(dsps_subc_s16_esp, onearray, newArray, newArray.cols(), &value);
#endif
    return newArray;
  }

  template<>
  inline Array<int8_t> operator-(Array<int8_t>& onearray, const int8_t value)
  {
    Array<int8_t> newArray(onearray.cols());
#if BENCHMARK_TEST
    REPORT_BENCHMARK("cycles to complete: ", dsps_subc_s8_esp, onearray, newArray, newArray.cols(), &value);
#else
    exec_dsp(dsps_subc_s8_esp, onearray, newArray, newArray.cols(), &value);
#endif
    return newArray;
  }

  /**
   * @brief Subtract a constant from an array
   * 
   * @param another 
   * @return Array 
   */
  template<typename T>
  inline Array<T> operator-(const T value, Array<T> onearray)
  {
    Array<T> newArray(onearray.cols());
    subConstFromArray<T>(onearray, newArray, newArray.cols(), value, -1);
    return newArray;
  }

  template<>
  inline Array<float> operator-(const float value, Array<float> onearray)
  {
    Array<float> newArray(onearray.cols());
#if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_csub_f32_esp, onearray, newArray, newArray.cols(), value);
#else
    exec_dsp(dsps_csub_f32_esp, onearray, newArray, newArray.cols(), value);
#endif
    return newArray;
  }

  template<>
  inline Array<int32_t> operator-(const int32_t value, Array<int32_t> onearray)
  {
    Array<int32_t> newArray(onearray.cols());
#if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_csub_s32_esp, onearray, newArray, newArray.cols(), value);
#else
    exec_dsp(dsps_csub_s32_esp, onearray, newArray, newArray.cols(), value);
#endif
    return newArray;
  }

  template<>
  inline Array<uint32_t> operator-(const uint32_t value, Array<uint32_t> onearray)
  {
    Array<uint32_t> newArray(onearray.cols());
#if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_csub_s32_esp, (int32_t*)onearray, (int32_t*)newArray, newArray.cols(), value);
#else
    exec_dsp(dsps_csub_s32_esp, onearray, newArray, newArray.cols(), value);
#endif
    return newArray;
  }

  template<>
  inline Array<int16_t> operator-(const int16_t value, Array<int16_t> onearray)
  {
    Array<int16_t> newArray(onearray.cols());
#if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_csub_s16_esp, onearray, newArray, newArray.cols(), &value);
#else
    exec_dsp(dsps_csub_s16_esp, onearray, newArray, newArray.cols(), &value);
#endif
    return newArray;
  }

  template<>
  inline Array<int8_t> operator-(const int8_t value, Array<int8_t> onearray)
  {
    Array<int8_t> newArray(onearray.cols());
#if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_csub_s8_esp, onearray, newArray, newArray.cols(), &value);
#else
    exec_dsp(dsps_csub_s8_esp, onearray, newArray, newArray.cols(), &value);
#endif
    return newArray;
  }
  
  /**
   * @brief Multiply an array by another
   * 
   * @tparam T 
   * @param onearray 
   * @param another 
   * @return Array<T>& 
   */
  template<typename T>
  inline Array<T> operator*(Array<T>& onearray, Array<T> another)
  {
    Array<T> newArray(onearray.cols());
    mulArrayByArray((T*)onearray, (T*)another, (T*)newArray, newArray.cols());
    return newArray;
  }

  template<>
  inline Array<float> operator*(Array<float>& onearray, Array<float> another)
  {
    Array<float> newArray(onearray.cols());
 #if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_mul_f32_esp, onearray, another, newArray, onearray.cols());
#else
    exec_dsp(dsps_mul_f32_esp, onearray, another, newArray, onearray.cols());
#endif
    return newArray;
  }

  template<>
  inline Array<int32_t> operator*(Array<int32_t>& onearray, Array<int32_t> another)
  {
    Array<int32_t> newArray(onearray.cols());
#if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_mul_s32_esp, onearray, another, newArray, onearray.cols());
#else
    exec_dsp(dsps_mul_s32_esp, onearray, another, newArray, onearray.cols());
#endif
    return newArray;
  }

  template<>
  inline Array<uint32_t> operator*(Array<uint32_t>& onearray, Array<uint32_t> another)
  {
    Array<uint32_t> newArray(onearray.cols());
#if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ",\
                      dsps_mul_s32_esp,\
                      (int32_t*)onearray,\
                      (int32_t*)another,\
                      (int32_t*)newArray,\
                      onearray.cols());
#else
    exec_dsp(dsps_mul_s32_esp,\
            (int32_t*)onearray,\
            (int32_t*)another,\
            (int32_t*)newArray,\
            onearray.cols());
#endif
    return newArray;
  }

  template<>
  inline Array<int16_t> operator*(Array<int16_t>& onearray, Array<int16_t> another)
  {
    Array<int16_t> newArray(onearray.cols());
#if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_mul_s16_esp, onearray, another, newArray, onearray.cols());
#else
    exec_dsp(dsps_mul_s16_esp, onearray, another, newArray, onearray.cols());
#endif
    return newArray;
  }

  template<>
  inline Array<int8_t> operator*(Array<int8_t>& onearray, Array<int8_t> another)
  {
    Array<int8_t> newArray(onearray.cols());
#if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_mul_s8_esp, onearray, another, newArray, onearray.cols());
#else
    exec_dsp(dsps_mul_s8_esp, onearray, another, newArray, onearray.cols());
#endif
    return newArray;
  }

  /**
   * @brief Multiply an array by a constant
   * 
   * @tparam T 
   * @param onearray 
   * @param value 
   * @return Array<T>& 
   */
  template<typename T>
  inline Array<T> operator*(Array<T>& onearray, const T value)
  {
    Array<T> newArray(onearray.cols());
    mulConstByArray<T>(onearray, newArray, newArray.cols(), value);
    return newArray;
  }

  template<>
  inline Array<float> operator*(Array<float>& onearray, const float value)
  {
    Array<float> newArray(onearray.cols());
#if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_mulc_f32_esp, onearray, newArray, newArray.cols(), value);
#else
    exec_dsp(dsps_mulc_f32_esp, onearray, newArray, newArray.cols(), value);
#endif
    return newArray;
  }

  template<>
  inline Array<int32_t> operator*(Array<int32_t>& onearray, const int32_t value)
  {
    Array<int32_t> newArray(onearray.cols());
#if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_mulc_s32_esp, onearray, newArray, newArray.cols(), value);
#else
    exec_dsp(dsps_mulc_s32_esp, onearray, newArray, newArray.cols(), value);
#endif
    return newArray;
  }

  template<>
  inline Array<uint32_t> operator*(Array<uint32_t>& onearray, const uint32_t value)
  {
    Array<uint32_t> newArray(onearray.cols());
#if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ",\
                    dsps_mulc_s32_esp,(int32_t*)onearray,\
                    (int32_t*)newArray,\
                    newArray.cols(),\
                    value);
#else
    exec_dsp(dsps_mulc_s32_esp,(int32_t*)onearray,\
                (int32_t*)newArray,\
                newArray.cols(),\
                value);
#endif
    return newArray;
  }

  template<>
  inline Array<int16_t> operator*(Array<int16_t>& onearray, const int16_t value)
  {
    Array<int16_t> newArray(onearray.cols());
#if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_mulc_s16_esp, onearray, newArray, newArray.cols(), value);
#else
    exec_dsp(dsps_mulc_s16_esp, onearray, newArray, newArray.cols(), value);
#endif
    return newArray;
  }

  template<>
  inline Array<int8_t> operator*(Array<int8_t>& onearray, const int8_t value)
  {
    Array<int8_t> newArray(onearray.cols());
#if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_mulc_s8_esp, onearray, newArray, newArray.cols(), &value);
#else
    exec_dsp(dsps_mulc_s8_esp, onearray, newArray, newArray.cols(), &value);
#endif
    return newArray;
  }

  /**
   * @brief Multiply a constant by an array
   * 
   * @param another 
   * @return Array 
   */
  template<typename T>
  inline Array<T> operator*(const T value, Array<T> another)
  {
    Array<T> newArray(another.cols());
    mulConstByArray<T>(another, newArray, another.cols(), value);
    return newArray;
  }

  template<>
  inline Array<float> operator*(const float value, Array<float> onearray)
  {
    Array<float> newArray(onearray.cols());
#if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_mulc_f32_esp, onearray, newArray, newArray.cols(), value);
#else
    exec_dsp(dsps_mulc_f32_esp, onearray, newArray, newArray.cols(), value);
#endif
    return newArray;
  }

  template<>
  inline Array<int32_t> operator*(const int32_t value, Array<int32_t> onearray)
  {
    Array<int32_t> newArray(onearray.cols());
#if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_mulc_s32_esp, onearray, newArray, newArray.cols(), value);
#else
    exec_dsp(dsps_mulc_s32_esp, onearray, newArray, newArray.cols(), value);
#endif
    return newArray;
  }

  template<>
  inline Array<uint32_t> operator*(const uint32_t value, Array<uint32_t> onearray)
  {
    Array<uint32_t> newArray(onearray.cols());
#if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ",\
                    dsps_mulc_s32_esp,(int32_t*)onearray,\
                    (int32_t*)newArray,\
                    newArray.cols(),\
                    value);
#else
    exec_dsp(dsps_mulc_s32_esp,(int32_t*)onearray,\
                (int32_t*)newArray,\
                newArray.cols(),\
                value);
#endif
    return newArray;
  }

  template<>
  inline Array<int16_t> operator*(const int16_t value, Array<int16_t> onearray)
  {
    Array<int16_t> newArray(onearray.cols());
#if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_mulc_s16_esp, onearray, newArray, newArray.cols(), value);
#else
    exec_dsp(dsps_mulc_s16_esp, onearray, newArray, newArray.cols(), value);
#endif
    return newArray;
  }

  template<>
  inline Array<int8_t> operator*(const int8_t value, Array<int8_t> onearray)
  {
    Array<int8_t> newArray(onearray.cols());
#if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_mulc_s8_esp, onearray, newArray, newArray.cols(), &value);
#else
    exec_dsp(dsps_mulc_s8_esp, onearray, newArray, newArray.cols(), &value);
#endif
    return newArray;
  }

  /**
   * @brief Divide an array by a constant
   * 
   * @tparam T 
   * @param onearray 
   * @param value 
   * @return Array<T>& 
   */
  template<typename T>
  inline Array<T> operator/(Array<T>& onearray, const T value)
  {
    Array<T> newArray(onearray.cols());
    divArrayByConst((T*)onearray, newArray, onearray.cols(), value);
    return newArray;
  }

  template<>
  inline Array<float> operator/(Array<float>& onearray, const float value)
  {
    Array<float> newArray(onearray.cols());
#if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_divc_f32_esp, onearray, newArray, onearray.cols(), value);
#else
    exec_dsp(dsps_divc_f32_esp, onearray, newArray, onearray.cols(), value);
#endif
    return newArray;
  }

  template<>
  inline Array<int32_t> operator/(Array<int32_t>& onearray, const int32_t value)
  {
    Array<int32_t> newArray(onearray.cols());
#if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_divc_s32_esp, onearray, newArray, onearray.cols(), value);
#else
    exec_dsp(dsps_divc_s32_esp, onearray, newArray, onearray.cols(), value);
#endif
    return newArray;
  }

  template<>
  inline Array<int16_t> operator/(Array<int16_t>& onearray, const int16_t value)
  {
    Array<int16_t> newArray(onearray.cols());
#if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_divc_s16_esp, onearray, newArray, onearray.cols(), value);
#else
    exec_dsp(dsps_divc_s16_esp, onearray, newArray, onearray.cols(), value);
#endif
    return newArray;
  }

  template<>
  inline Array<int8_t> operator/(Array<int8_t>& onearray, const int8_t value)
  {
    Array<int8_t> newArray(onearray.cols());
#if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_divc_s8_esp, onearray, newArray, onearray.cols(), value);
#else
    exec_dsp(dsps_divc_s8_esp, onearray, newArray, onearray.cols(), value);
#endif
    return newArray;
  }

  /**
   * @brief Divide a constant by an array
   * 
   * @param another 
   * @return Array 
   */
  template<typename T>
  inline Array<T> operator/(const T value, Array<T> another)
  {
    Array<T> newArray(another.cols());
    divConstByArray((T*)another, (T*)newArray, newArray.cols(), (T)value);
    return newArray;
  }

  template<>
  inline Array<float> operator/(const float value, Array<float> another)
  {
    Array<float> newArray(another.cols());
#if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_cdiv_f32_esp, another, newArray, newArray.cols(), value);
#else
    exec_dsp(dsps_cdiv_f32_esp, another, newArray, newArray.cols(), value);
#endif
    return newArray;
  }

  template<>
  inline Array<int32_t> operator/(const int32_t value, Array<int32_t> onearray)
  {
    Array<int32_t> newArray(onearray.cols());
#if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_cdiv_s32_esp, onearray, newArray, newArray.cols(), value);
#else
    exec_dsp(dsps_cdiv_s32_esp, onearray, newArray, newArray.cols(), value);
#endif
    return newArray;
  }

  template<>
  inline Array<int16_t> operator/(const int16_t value, Array<int16_t> onearray)
  {
    Array<int16_t> newArray(onearray.cols());
#if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_cdiv_s16_esp, onearray, newArray, newArray.cols(), value);
#else
    exec_dsp(dsps_cdiv_s16_esp, onearray, newArray, newArray.cols(), value);
#endif
    return newArray;
  }

  template<>
  inline Array<int8_t> operator/(const int8_t value, Array<int8_t> onearray)
  {
    Array<int8_t> newArray(onearray.cols());
#if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_cdiv_s8_esp, onearray, newArray, newArray.cols(), value);
#else
    exec_dsp(dsps_cdiv_s8esp, onearray, newArray, newArray.cols(), value);
#endif
    return newArray;
  }

  /**
   * @brief Divide an array by another
   * 
   * @tparam T 
   * @param onearray 
   * @param another 
   * @return Array<T>& 
   */
  template<typename T>
  inline Array<T> operator/(Array<T>& onearray, Array<T> another)
  {
    Array<T> newArray(onearray.cols());
    divArrayByArray((T*)onearray, (T*)another, (T*)newArray, onearray.cols());
    return newArray;
  }

  template<>
  inline Array<float> operator/(Array<float>& onearray, Array<float> another)
  {
    Array<float> newArray(onearray.cols());
#if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_div_f32_esp, onearray, another, newArray, onearray.cols());
#else
    exec_dsp(dsps_div_f32_esp, onearray, another, newArray, onearray.cols());
#endif
    return newArray;
  }

  template<>
  inline Array<int32_t> operator/(Array<int32_t>& onearray, Array<int32_t> another)
  {
    Array<int32_t> newArray(onearray.cols());
#if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_div_s32_esp, onearray, another, newArray, onearray.cols());
#else
    exec_dsp(dsps_div_s32_esp, onearray, another, newArray, onearray.cols());
#endif
    return newArray;
  }

  template<>
  inline Array<int16_t> operator/(Array<int16_t>& onearray, Array<int16_t> another)
  {
    Array<int16_t> newArray(onearray.cols());
#if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_div_s16_esp, onearray, another, newArray, onearray.cols());
#else
    exec_dsp(dsps_div_s16_esp, onearray, another, newArray, onearray.cols());
#endif
    return newArray;
  }

  template<>
  inline Array<int8_t> operator/(Array<int8_t>& onearray, Array<int8_t> another)
  {
    Array<int8_t> newArray(onearray.cols());
#if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_div_s8_esp, onearray, another, newArray, onearray.cols());
#else
    exec_dsp(dsps_div_s8_esp, onearray, another, newArray, onearray.cols());
#endif
    return newArray;
  }

  /**
   * @brief Dot product between 2 arrays
   * 
   * @tparam T 
   * @param onearray 
   * @param another 
   * @return const T result
   */
  template<typename T>
  inline const T operator^(Array<T>& onearray, Array<T> another)
  {
    return -1;
  }

  template<>
  inline const int32_t operator^(Array<int32_t>& onearray, Array<int32_t> another)
  {
    int32_t result;
    float input1[onearray.cols()];
    float input2[onearray.cols()];
    float r;
    cpyArray((int32_t*)onearray, input1, onearray.cols());
    cpyArray((int32_t*)another, input2, onearray.cols());
#if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_dotprod_f32_ae32, input1, input2, &r, onearray.cols());
#else
    exec_dsp(dsps_dotprod_f32_ae32, input1, input2, &r, onearray.cols());
#endif
    result = (int32_t)r;
    return result;
  }

  template<>
  inline const uint8_t operator^(Array<uint8_t>& onearray, Array<uint8_t> another)
  {
    uint8_t result;
    int16_t input1[onearray.cols()];
    int16_t input2[onearray.cols()];
    int16_t r;
    cpyArray((uint8_t*)onearray, input1, onearray.cols());
    cpyArray((uint8_t*)another, input2, onearray.cols());
#if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_dotprod_s16_ae32, input1, input2, &r, onearray.cols(), 0);
#else
    exec_dsp(dsps_dotprod_s16_ae32, input1, input2, &r, onearray.cols(), 0);
#endif
    result = (uint8_t)r;
    return result;
  }

  template<>
  inline const float operator^(Array<float>& onearray, Array<float> another)
  {
    float result;
#if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_dotprod_f32_ae32, onearray, another, &result, onearray.cols());
#else
    exec_dsp(dsps_dotprod_f32_ae32, onearray, another, &result, onearray.cols());
#endif
    return result;
  }

  template<>
  inline const int16_t operator^(Array<int16_t>& onearray, Array<int16_t> another)
  {
    int16_t result;
#if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_dotprod_s16_ae32, onearray, another, &result, onearray.cols(), 0);
#else
    exec_dsp(dsps_dotprod_s16_ae32, onearray, another, &result, onearray.cols(), 0);
#endif
    return result;
  }
}

#endif