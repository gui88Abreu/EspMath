#ifndef _ESP_ARRAY_H_
#define _ESP_ARRAY_H_

#include <Arduino.h>
#include <type_traits>

#include "dsps_add.h"
#include "dsps_sub.h"
#include "dsps_mul.h"
#include "dsps_addc.h"
#include "dsps_mulc.h"
#include "dsps_conv.h"
#include "dsps_corr.h"
#include "dsps_dotprod.h"

#include "dsps_mulc/dsps_mulc_esp.h"
#include "esp_opt.h"
#include "ansi.h"

#if BENCHMARK_TEST
#include "hard_debug.h"
#endif

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

    /**
     * @brief Return the allocated memory capabilities.
     * 
     * @return uint32_t
     */
    static const uint32_t memCaps()
    {
      return (const uint32_t)(MALLOC_CAP_DEFAULT | MALLOC_CAP_8BIT);
    }

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
     */
    Array(const size_t initialMem = 0)
    {
      _length = initialMem;
       _size = _mem2alloc(initialMem);
      _array = _size > 0 ? (T*)heap_caps_malloc(_size, this->memCaps()) : NULL;
    }
    
    /**
     * @brief Construct a new Array object with initial values
     * 
     * @param initialValues Initial values of the array.
     * @param initialMem The initial size of the array.
     */
    Array(const T* initialValues,\
          const size_t initialMem = 0):Array(initialMem)
    {
      if (!_array)
        return;
      cpyArray(initialValues, _array, _length);
    }

    /**
     * @brief Constructor
     * 
     * @param another 
     */
    Array(Array& another){copy(another);}
    Array(Array&& another){copyRef(another);}

    /**
     * @brief Assign operation
     * 
     * @param another 
     */
    void operator=(Array& another){copyRef(another);}
    void operator=(Array&& another){copyRef(another);}

    /**
     * @brief Get array element
     * 
     * @param index 
     * @return T 
     */
    T& operator[](const size_t index)
    {
      return _array[index];
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
      for (size_t i = 0; i < _length; i++)
        if(filter[i]) newArray << _array[i];
      
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
      Array<T> newArray(_length);
      for (size_t i = 0; i < _length; i++)
        _array[i] == value ? newArray[i] = 1 : 0;
      
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
      Array<T> newArray(_length);
      for (size_t i = 0; i < _length; i++)
        _array[i] != value ? newArray[i] = 1 : 0;
      
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
      Array<T> newArray(_length);
      for (size_t i = 0; i < _length; i++)
        _array[i] > value ? newArray[i] = 1 : 0;
      
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
      Array<T> newArray(_length);
      for (size_t i = 0; i < _length; i++)
        _array[i] < value ? newArray[i] = 1 : 0;
      
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
      Array<T> newArray(_length);
      for (size_t i = 0; i < _length; i++)
        _array[i] >= value ? newArray[i] = 1 : 0;
      
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
      Array<T> newArray(_length);
      for (size_t i = 0; i < _length; i++)
        _array[i] <= value ? newArray[i] = 1 : 0;
      
      return newArray;
    }

    /**
     * @brief copy[i] = !array[i], i = 0,1,2,3...
     * 
     * @return Array 
     */
    Array operator!()
    {
      Array<T> newArray(_length);
      for (size_t i = 0; i < _length; i++)
        newArray[i] = !_array[i];
      
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
      while( i < _length)
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
      addConstToArray(_array, _array, _length, value);
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
      mulConstByArray(_array, _array, _length, value);
    }

    /**
     * @brief Divide by a constant
     * 
     * @param value 
     * @note Float arrays make use of DSP instructions.
     */
    void operator/=(const float value)
    {
      divArrayByConst(_array, _array, _length, value);
    }

    /**
     * @brief Add an array
     * 
     * @param another
     * @note Float and int16_t arrays make use of DSP instructions.
     */
    void operator+=(Array& another)
    { 
      addArrayToArray((T*)another, _array, _array, _length);
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
      subArrayFromArray((T*)another, _array, _array, _length);
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
      mulArrayByArray((T*)another, _array, _array, _length);
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
      divArrayByArray(_array, another, _array, _length);
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
      for(size_t i = 0; i < another.length(); i++)
      {
        *this = *this << another[i];
      }
      return *this;
    }

    /**
     * @brief Implicit cast for array pointer
     * 
     * @return arrayPntr 
     */
    operator arrayPntr() const {return _array;}
    
    /**
     * @brief Assign a value to an existent position of the _array
     * 
     * @param value 
     * @param index 
     */
    const bool assign(const T value, const size_t index)
    {
      if (index >= _length) return false;
      (*this)[index] = value;
      return true;
    }

    /**
     * @brief Append a value to the _array
     * 
     * @param value 
     * @return true Successful appended
     * @return false Failed to append
     */
    const bool append(const T value)
    {
      if(_length < _size/sizeof(T))
      {
        (*this)[_length++] = value;
        return true;
      }

      _size = _mem2alloc(_length+1);

      if (_array)
      {
        heap_caps_free(_array);
        _array = _size > 0 ? (T*)heap_caps_malloc(_size, this->memCaps()) : NULL;
      }
      else
      {
        _array = _size > 0 ? (T*)heap_caps_malloc(_size, this->memCaps()) : NULL;
      }

      if (_array)
      {
        (*this)[_length++] = value;
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
     * @brief Get the convolution of the array by the given kernel
     * 
     * @param kernel 
     * @return Array output array with convolution result length of (siglen + Kernel -1)
     * 
     * @note Float arrays make use of DSP instructions.
     */
    Array conv(Array& kernel)
    {
      const size_t outputLength = _length + kernel.length() -1;
      Array convOutput(outputLength);

      float output[outputLength];
      float itself[_length];
      float _kernel[kernel.length()];

      cpyArray(_array, itself, _length);
      cpyArray(kernel, _kernel, kernel.length());
      dsps_conv_f32_ae32(itself, _length, _kernel, kernel.length(), output);
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

      _length = another.length();
      _size = another.memSize();
      _array = _size > 0 ? (T*)heap_caps_malloc(_size, this->memCaps()) : NULL;
      
      for(size_t i = 0; i < _length; i++)
        _array[i] = another[i];
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

      _length = another.length();
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
      Array<float> corr(_length);

      float itself[_length];
      float _pattern[pattern.length()];

      cpyArray(_array, itself, _length);
      cpyArray(pattern, _pattern, pattern.length());
      dsps_corr_f32_ae32(itself, _length, _pattern, pattern.length(), corr);

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
      while (i < _length)
      {
        if (_array[i]!=another[i])
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

    /**
     * @brief Get the Array Size
     * 
     * @return size_t 
     */
    size_t length() const
    {
      return _length;
    }

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
    size_t _length = 0; /*Length of the _array*/
    T* _array = NULL; /*Array pointer*/

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
#if (FAST_MODE)
      size_t extraBytes = extraToAlign(minBytes);
      return minBytes + extraBytes;
#else
      return minBytes;
#endif
    }

    /**
     * @brief Get the extra quantity bytes to be allocated and align array
     * 
     * @param min Minumum quantity of bytes
     * @return size_t Result
     */
    static size_t extraToAlign(const size_t min)
    {
      return (ALIGNMENT - min%ALIGNMENT)%ALIGNMENT;
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
  inline const uint32_t Array<int32_t>::memCaps()
  {
    return (const uint32_t)(MALLOC_CAP_DEFAULT | MALLOC_CAP_32BIT);
  }

  template<>
  inline void Array<float>::operator+=(const float value)
  {
    dsps_addc_f32_ae32(_array, _array, _length, value, 1, 1);
  }

  template<>
  inline void Array<float>::operator*=(const float value)
  { 
    dsps_mulc_f32_esp(_array, _array, _length, value);
  }

  template<>
  inline void Array<int32_t>::operator*=(const int32_t value)
  {
    dsps_mulc_s32_esp(_array, _array, _length, value);
  }

  template<>
  inline void Array<uint32_t>::operator*=(const uint32_t value)
  {
    dsps_mulc_s32_esp((int32_t*)_array, (int32_t*)_array, _length, (int32_t)value);
  }

  template<>
  inline void Array<uint8_t>::operator*=(const uint8_t value)
  {
    dsps_mulc_u8_esp(_array, _array, _length, &value);
  }

  template<>
  inline void Array<int16_t>::operator*=(const int16_t value)
  {
    dsps_mulc_s16_esp(_array, _array, _length, value);
  }

  template<>
  inline void Array<float>::operator+=(Array<float>& another)
  {
    dsps_add_f32_ae32(_array, another, _array, _length, 1, 1, 1);
  }

  template<>
  inline void Array<int16_t>::operator+=(Array<int16_t>& another)
  {
    dsps_add_s16_ae32(_array, another, _array, _length, 1, 1, 1, 0);
  }

  template<>
  inline void Array<float>::operator-=(Array<float>& another)
  {
    dsps_sub_f32_ae32(_array, another, _array, _length, 1, 1, 1);
  }

  template<>
  inline void Array<float>::operator*=(Array<float>& another)
  {
    dsps_mul_f32_ae32(_array, another, _array, _length, 1, 1, 1);
  }

  template<>
  inline void Array<int16_t>::operator*=(Array<int16_t>& another)
  {
    dsps_mul_s16_ansi(_array, another, _array, _length, 1, 1, 1, 0);
  }

  template<>
  inline Array<float> Array<float>::operator==(const float value)
  {
    Array<float> newArray(_length);
    for (size_t i = 0; i < _length; i++)
      eqFloats(_array[i++], value) ? newArray[i] = 1 : 0;
    return newArray;
  }

  template<>
  inline const bool Array<float>::operator==(const float* input)
  {
    size_t i = 0;
    while( i < _length)
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
    const size_t outputLength = _length + kernel.length() -1;
    Array<float> convOutput(outputLength);
    dsps_conv_f32_ae32(_array, _length, kernel, kernel.length(), convOutput);
    return convOutput;
  }

  template<>
  inline Array<float> Array<float>::correlation(Array<float>& pattern)
  {
    Array<float> corr(_length);
    dsps_corr_f32_ae32(_array, _length, pattern, pattern.length(), corr);
    return corr;
  }

  template<>
  inline const bool Array<float>::diff(Array<float>& another, const float EPSILON)
  {
    size_t i = 0;
    while(i < _length)
    {
      if (!eqFloats(_array[i], another[i], EPSILON))
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
    Array<T> newArray(onearray.length());
    addArrayToArray<T>((T*)onearray, (T*)another, (T*)newArray, onearray.length());
    return newArray;
  }

  template<>
  inline Array<float> operator+(Array<float>& onearray, Array<float> another)
  {
    Array<float> newArray(onearray.length());
    dsps_add_f32_ae32(onearray, another, newArray, onearray.length(), 1, 1, 1);
    return newArray;
  }

  template<>
  inline Array<int16_t> operator+(Array<int16_t>& onearray, Array<int16_t> another)
  {
    Array<int16_t> newArray(onearray.length());
    dsps_add_s16_ae32(onearray, another, newArray, onearray.length(), 1, 1, 1, 0);
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
    Array<T> newArray(onearray.length());
    addConstToArray<T>(onearray, newArray, newArray.length(), value);
    return newArray;
  }

  template<>
  inline Array<float> operator+(Array<float>& onearray, const float value)
  {
    Array<float> newArray(onearray.length());
    dsps_addc_f32_ae32(onearray, newArray, newArray.length(), value, 1, 1);
    return newArray;
  }

  /**
   * @brief Add an array to a constant
   * 
   * @param another 
   * @return Array
   */
  template<typename T>
  inline Array<T> operator+(const T value, Array<T> another)
  {
    Array<T> newArray(another.length());
    addConstToArray<T>(another, newArray, newArray.length(), value);
    return newArray;
  }

  template<>
  inline Array<float> operator+(const float value, Array<float> another)
  {
    Array<float> newArray(another.length());
    dsps_addc_f32_ae32(another, newArray, newArray.length(), value, 1, 1);
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
    Array<T> newArray(onearray.length());
    subArrayFromArray<T>((T*)onearray, (T*)another, (T*)newArray, onearray.length());
    return newArray;
  }

  template<>
  inline Array<float> operator-(Array<float>& onearray, Array<float> another)
  {
    Array<float> newArray(onearray.length());
    dsps_sub_f32_ae32(onearray, another, newArray, onearray.length(), 1, 1, 1);
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
    Array<T> newArray(onearray.length());
    subConstFromArray<T>(onearray, newArray, newArray.length(), value);
    return newArray;
  }

  template<>
  inline Array<float> operator-(Array<float>& onearray, const float value)
  {
    Array<float> newArray(onearray.length());
    dsps_addc_f32_ae32(onearray, newArray, newArray.length(), -1*value, 1, 1);
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
    Array<T> newArray(onearray.length());
    subConstFromArray<T>(onearray, newArray, newArray.length(), value, -1);
    return newArray;
  }

  template<>
  inline Array<float> operator-(const float value, Array<float> onearray)
  {
    Array<float> newArray(onearray.length());
    float constant = -1*value;
    dsps_addc_f32_ae32(onearray, newArray, newArray.length(), constant, 1, 1);
    dsps_mulc_f32_esp(newArray, newArray, newArray.length(), -1);
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
    Array<T> newArray(onearray.length());
    mulArrayByArray((T*)onearray, (T*)another, (T*)newArray, newArray.length());
    return newArray;
  }

  template<>
  inline Array<float> operator*(Array<float>& onearray, Array<float> another)
  {
    Array<float> newArray(onearray.length());
    dsps_mul_f32_ae32(onearray, another, newArray, onearray.length(), 1, 1, 1);
    return newArray;
  }

  template<>
  inline Array<int16_t> operator*(Array<int16_t>& onearray, Array<int16_t> another)
  {
    Array<int16_t> newArray(onearray.length());
    dsps_mul_s16_ansi(onearray, another, newArray, onearray.length(), 1, 1, 1, 0);
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
    Array<T> newArray(onearray.length());
    mulConstByArray<T>(onearray, newArray, newArray.length(), value);
    return newArray;
  }

  template<>
  inline Array<float> operator*(Array<float>& onearray, const float value)
  {
    Array<float> newArray(onearray.length());
#if BENCHMARK_TEST
    unsigned intlevel = dsp_ENTER_CRITICAL();
    dsps_mulc_f32_esp(onearray, newArray, newArray.length(), value);
    size_t x1 = xthal_get_ccount();
    dsps_mulc_f32_esp(onearray, newArray, newArray.length(), value);
    size_t x2 = xthal_get_ccount();
    dsp_EXIT_CRITICAL(intlevel);
    debug.print("It took " + String(x2-x1) + " cycles!");
#else
    unsigned intlevel = dsp_ENTER_CRITICAL();
    dsps_mulc_f32_esp(onearray, newArray, newArray.length(), value);
    dsp_EXIT_CRITICAL(intlevel);
#endif
    return newArray;
  }

  template<>
  inline Array<int32_t> operator*(Array<int32_t>& onearray, const int32_t value)
  {
    Array<int32_t> newArray(onearray.length());
#if BENCHMARK_TEST
    unsigned intlevel = dsp_ENTER_CRITICAL();
    dsps_mulc_s32_esp(onearray, newArray, newArray.length(), value);
    size_t x1 = xthal_get_ccount();
    dsps_mulc_s32_esp(onearray, newArray, newArray.length(), value);
    size_t x2 = xthal_get_ccount();
    dsp_EXIT_CRITICAL(intlevel);
    debug.print("It took " + String(x2-x1) + " cycles!");
#else
    unsigned intlevel = dsp_ENTER_CRITICAL();
    dsps_mulc_s32_esp(onearray, newArray, newArray.length(), value);
    dsp_EXIT_CRITICAL(intlevel);
#endif
    return newArray;
  }

  template<>
  inline Array<uint32_t> operator*(Array<uint32_t>& onearray, const uint32_t value)
  {
    Array<uint32_t> newArray(onearray.length());
#if BENCHMARK_TEST
    unsigned intlevel = dsp_ENTER_CRITICAL();
    dsps_mulc_s32_esp((int32_t*)onearray.getArrayPntr(), (int32_t*)newArray.getArrayPntr(), newArray.length(), value);
    size_t x1 = xthal_get_ccount();
    dsps_mulc_s32_esp((int32_t*)onearray.getArrayPntr(), (int32_t*)newArray.getArrayPntr(), newArray.length(), value);
    size_t x2 = xthal_get_ccount();
    dsp_EXIT_CRITICAL(intlevel);
    debug.print("It took " + String(x2-x1) + " cycles!");
#else
    unsigned intlevel = dsp_ENTER_CRITICAL();
    dsps_mulc_s32_esp((int32_t*)onearray.getArrayPntr(), (int32_t*)newArray.getArrayPntr(), newArray.length(), value);
    dsp_EXIT_CRITICAL(intlevel);
#endif
    return newArray;
  }

  template<>
  inline Array<int16_t> operator*(Array<int16_t>& onearray, const int16_t value)
  {
    Array<int16_t> newArray(onearray.length());
#if BENCHMARK_TEST
    unsigned intlevel = dsp_ENTER_CRITICAL();
    dsps_mulc_s16_esp(onearray, newArray, newArray.length(), value);
    size_t x1 = xthal_get_ccount();
    dsps_mulc_s16_esp(onearray, newArray, newArray.length(), value);
    size_t x2 = xthal_get_ccount();
    dsp_EXIT_CRITICAL(intlevel);
    debug.print("It took " + String(x2-x1) + " cycles!");
#else
    unsigned intlevel = dsp_ENTER_CRITICAL();
    dsps_mulc_s16_esp(onearray, newArray, newArray.length(), value);
    dsp_EXIT_CRITICAL(intlevel);
#endif
    return newArray;
  }

  template<>
  inline Array<uint8_t> operator*(Array<uint8_t>& onearray, const uint8_t value)
  {
    Array<uint8_t> newArray(onearray.length());
#if BENCHMARK_TEST
    unsigned intlevel = dsp_ENTER_CRITICAL();
    dsps_mulc_u8_esp(onearray, newArray, newArray.length(), &value);
    size_t x1 = xthal_get_ccount();
    dsps_mulc_u8_esp(onearray, newArray, newArray.length(), &value);
    size_t x2 = xthal_get_ccount();
    dsp_EXIT_CRITICAL(intlevel);
    debug.print("It took " + String(x2-x1) + " cycles!");
#else
    unsigned intlevel = dsp_ENTER_CRITICAL();
    dsps_mulc_u8_esp(onearray, newArray, newArray.length(), &value);
    dsp_EXIT_CRITICAL(intlevel);
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
    Array<T> newArray(another.length());
    mulConstByArray<T>(another, newArray, another.length(), value);
    return newArray;
  }

  template<>
  inline Array<float> operator*(const float value, Array<float> onearray)
  {
    Array<float> newArray(onearray.length());
    dsps_mulc_f32_esp(onearray, newArray, newArray.length(), value);
    return newArray;
  }

  template<>
  inline Array<int32_t> operator*(const int32_t value, Array<int32_t> onearray)
  {
    Array<int32_t> newArray(onearray.length());
    dsps_mulc_s32_esp(onearray, newArray, newArray.length(), value);
    return newArray;
  }

  template<>
  inline Array<uint32_t> operator*(const uint32_t value, Array<uint32_t> onearray)
  {
    Array<uint32_t> newArray(onearray.length());
    dsps_mulc_s32_esp((int32_t*)onearray.getArrayPntr(), (int32_t*)newArray.getArrayPntr(), newArray.length(), value);
    return newArray;
  }

  template<>
  inline Array<int16_t> operator*(const int16_t value, Array<int16_t> onearray)
  {
    Array<int16_t> newArray(onearray.length());
    dsps_mulc_s16_esp(onearray, newArray, newArray.length(), value);
    return newArray;
  }

  template<>
  inline Array<uint8_t> operator*(const uint8_t value, Array<uint8_t> onearray)
  {
    Array<uint8_t> newArray(onearray.length());
    dsps_mulc_u8_esp(onearray, newArray, newArray.length(), &value);
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
  inline Array<float> operator/(Array<T>& onearray, const float value)
  {
    Array<float> newArray(onearray.length());
    divArrayByConst((T*)onearray, newArray, onearray.length(), value);
    return newArray;
  }

  template<>
  inline Array<float> operator/(Array<float>& onearray, const float value)
  {
    Array<float> newArray(onearray.length());
    divArrayByConst((float*)onearray, newArray, newArray.length(), value);
    return newArray;
  }

  /**
   * @brief Divide a constant by an array
   * 
   * @param another 
   * @return Array 
   */
  template<typename T>
  inline Array<float> operator/(const float value, Array<T> another)
  {
    Array<float> newArray(another.length());
    divConstByArray((T*)another, newArray, newArray.length(), value);
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
  inline Array<float> operator/(Array<T>& onearray, Array<T> another)
  {
    Array<float> newArray(onearray.length());
    divArrayByArray((T*)onearray, (T*)another, newArray, onearray.length());
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
    float input1[onearray.length()];
    float input2[onearray.length()];
    float r;
    cpyArray((int32_t*)onearray, input1, onearray.length());
    cpyArray((int32_t*)another, input2, onearray.length());
    dsps_dotprod_f32_ae32(input1, input2, &r, onearray.length());
    result = (int32_t)r;
    return result;
  }

  template<>
  inline const uint8_t operator^(Array<uint8_t>& onearray, Array<uint8_t> another)
  {
    uint8_t result;
    int16_t input1[onearray.length()];
    int16_t input2[onearray.length()];
    int16_t r;
    cpyArray((uint8_t*)onearray, input1, onearray.length());
    cpyArray((uint8_t*)another, input2, onearray.length());
    dsps_dotprod_s16_ae32(input1, input2, &r, onearray.length(), 0);
    result = (uint8_t)r;
    return result;
  }

  template<>
  inline const float operator^(Array<float>& onearray, Array<float> another)
  {
    float result;
    dsps_dotprod_f32_ae32(onearray, another, &result, onearray.length());
    return result;
  }

  template<>
  inline const int16_t operator^(Array<int16_t>& onearray, Array<int16_t> another)
  {
    int16_t result;
    dsps_dotprod_s16_ae32((int16_t *)onearray, another, &result, onearray.length(), 0);
    return result;
  }
}

#endif