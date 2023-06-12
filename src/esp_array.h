#ifndef _ESP_ARRAY_H_
#define _ESP_ARRAY_H_

#include <Arduino.h>
#include <type_traits>
#include <esp_dsp.h>
#include "hard_debug.h"

#include "dsps_mulc.h"

/**
 * @brief Namespace for custom ESP32 MATH libraries
 * 
 */
namespace espmath{
  using namespace espmath;

#define ALIGNMENT 16

  /**
   * @brief Compare floats.
   * 
   * @param f1
   * @param f2
   * @param EPSILON tolerance
   * @return true floats are equal
   * @return false floats aren't equal
   */
  inline const bool eqFloats(const float f1, const float f2, const float EPSILON = 0.0001)
  {
   return (fabs(f1 - f2) <= EPSILON);
  }

  /**
   * @brief dest[i] = src[i]*cnst;
   * 
   * @tparam T1 Type of the source array.
   * @tparam T2 Type of the destination array.
   * @param src Source array.
   * @param dest Destination array.
   * @param len Quantity of elements to copy.
   * @param cnst Constant to multiply the output.
   */
  template<typename T1, typename T2>
  inline void cpyArray(const T1* src, T2* dest, const size_t len, const T2 cnst = 1)
  {
    for(size_t i = 0; i < len; i++)
      dest[i] = (T2)(src[i]*cnst);
  }

  /**
   * @brief dest[i] = cnst;
   * 
   * @tparam T Type of the destination array.
   * @param src Source array.
   * @param dest Destination array.
   * @param len Quantity of elements to copy.
   * @param cnst Constant to multiply the output.
   */
  template<typename T>
  inline void cpyConst(T* dest, const size_t len, const T cnst = 1)
  {
    for(size_t i = 0; i < len; i++)
      dest[i] = (T)(cnst);
  }

  /**
   * @brief dest[i] += src[i]*cnst;
   * 
   * @tparam T1 Type of the source array.
   * @tparam T2 Type of the destination array.
   * @param src Source array.
   * @param dest Destination array.
   * @param len Quantity of elements to copy.
   * @param cnst Constant to multiply the output.
   */
  template<typename T1, typename T2>
  inline void addArrayToArray(const T1* src, T2* dest, const size_t len, const T2 cnst = 1)
  {
    for(size_t i = 0; i < len; i++)
      dest[i] += (T2)(src[i]*cnst);
  }

  /**
   * @brief dest[i] += cnst;
   * 
   * @tparam T Type of the destination array.
   * @param src Source array.
   * @param dest Destination array.
   * @param len Quantity of elements to copy.
   * @param cnst Constant to multiply the output.
   */

  template<typename T>
  inline void addConstToArray(T* src, T* dest, const size_t len, const T cnst = 1)
  {
    for(size_t i = 0; i < len; i++)
      dest[i] = src[i] + cnst;
  }

  /**
   * @brief dest[i] -= src[i]*cnst;
   * 
   * @tparam T1 Type of the source array.
   * @tparam T2 Type of the destination array.
   * @param src Source array.
   * @param dest Destination array.
   * @param len Quantity of elements to copy.
   * @param cnst Constant to multiply the output.
   */
  template<typename T1, typename T2>
  inline void subArrayFromArray(const T1* src, T2* dest, const size_t len, const T2 cnst = 1)
  {
    for(size_t i = 0; i < len; i++)
      dest[i] -= (T2)(src[i]*cnst);
  }

  /**
   * @brief dest[i] -= cnst;
   * 
   * @tparam T Type of the destination array.
   * @param src Source array.
   * @param dest Destination array.
   * @param len Quantity of elements to copy.
   * @param cnst Constant to multiply the output.
   */
  template<typename T>
  inline void subConstFromArray(T* src, T* dest, const size_t len, const T cnst = 1)
  {
    for(size_t i = 0; i < len; i++)
      dest[i] = src[i] - cnst;
  }

  /**
   * @brief dest[i] *= src[i]*cnst;
   * 
   * @tparam T1 Type of the source array.
   * @tparam T2 Type of the destination array.
   * @param src Source array.
   * @param dest Destination array.
   * @param len Quantity of elements to copy.
   * @param cnst Constant to multiply the output.
   */
  template<typename T1, typename T2>
  inline void mulArrayByArray(const T1* src, T2* dest, const size_t len, const T2 cnst = 1)
  {
    for(size_t i = 0; i < len; i++)
      dest[i] *= (T2)(src[i]*cnst);
  }

  /**
   * @brief dest[i] *= cnst;
   * 
   * @tparam T Type of the destination array.
   * @param src Source array.
   * @param dest Destination array.
   * @param len Quantity of elements to copy.
   * @param cnst Constant to multiply the output.
   */
  template<typename T>
  inline void mulConstByArray(T* src, T* dest, const size_t len, const T cnst = 1)
  {
    for(size_t i = 0; i < len; i++)
      dest[i] = src[i]*cnst;
  }

  /**
   * @brief dest[i] /= src[i]*cnst;
   * 
   * @tparam T1 Type of the source array.
   * @tparam T2 Type of the destination array.
   * @param src Source array.
   * @param dest Destination array.
   * @param len Quantity of elements to copy.
   * @param cnst Constant to multiply the output.
   */
  template<typename T1, typename T2>
  inline void divArrayByArray(const T1* src, T2* dest, const size_t len, const T2 cnst = 1)
  {
    for(size_t i = 0; i < len; i++)
      dest[i] /= (T2)(src[i]*cnst);
  }

  /**
   * @brief dest[i] /= src[i]*cnst;
   * 
   * @tparam T Type of the destination array.
   * @param src Source array.
   * @param dest Destination array.
   * @param len Quantity of elements to copy.
   * @param cnst Constant to multiply the output.
   */
  template<typename T>
  inline void divArrayByConst(T* src, T* dest, const size_t len, const T cnst = 1)
  {
    for(size_t i = 0; i < len; i++)
      dest[i] = src[i] / cnst;
  }

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

    typedef T* arrayPntr;

    /**
     * @brief Supported array types for dsp instructions
     * 
     */
    enum DSPtypes{
      int16_type,
      float_type
    };

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
      heap_caps_aligned_free(_array);
    }

    /**
     * @brief Construct a new Array object
     * 
     * @param initialMem The initial size (amount of memory blocks) of the array.
     */
    Array(const size_t initialMem = 0)
    {
       _size = _mem2alloc(initialMem);
      _array = _size > 0 ? (T*)heap_caps_aligned_alloc(ALIGNMENT, _size, this->memCaps()) : NULL;
    }
    
    /**
     * @brief Construct a new Array object with initial values
     * 
     * @param initialValues Initial values of the array.
     * @param initialMem The initial size (amount of memory blocks) of the array.
     */
    Array(const T* initialValues,\
          const size_t initialMem = 0):Array(initialMem)
    {
      if (!_array)
        return;

      _length = initialMem;
      cpyArray(initialValues, _array, _length);
    }

    /**
     * @brief Constructer
     * 
     * @param another 
     */
    Array(const Array<T>& another){copy(another);}
    Array(const Array&& another):Array(another){}

    /**
     * @brief Assign operation
     * 
     * @param another 
     */
    void operator=(const Array& another){copy(another);}
    void operator=(const Array&& another){copy(another);}

    /**
     * @brief Get array element
     * 
     * @param index 
     * @return T 
     */
    const T& operator[](const size_t index)
    {
      return (*((T*)_array + index));
    }

    /**
     * @brief Verify if a value belongs to the array.
     * 
     * @param value The value to be verified.
     * @return true There is at least 1 value in the _array.
     * @return false The value is no present in the _array.
     */
    const bool operator==(const T value)
    {
      size_t i = 0;
      while(i < _length)
      {
        if(_array[i++] == value)
          return true;
      }
      return false;
    }

    /**
     * @brief Verify if a value doesn't belong to the array.
     * 
     * @param value The value to be verified.
     * @return true The value is no present in the _array.
     * @return false There is at least 1 value in the _array.
     */
    const bool operator!=(const T value) const
    {
      return !(*this == value);
    }

    /**
     * @brief Verify if all two arrays are identical.
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
     * @brief Verify if all two arrays are identical.
     * 
     * @param input The input array
     * @return true Every value of the _array is contained in input.
     * @return false Not all values of the _array are contained in input.
     */
    const bool operator==(Array& another)
    {
      bool result = *this == another.getArrayPntr();
      return result;
    }
    const bool operator==(Array&& another)
    {
      bool result = *this == another.getArrayPntr();
      return result;
    }

    /**
     * @brief Verify if all two arrays are not identical regardless the value order.
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
      (*this) *= (1.0/value);
    }

    /**
     * @brief Add an array
     * 
     * @param another
     * @note Float and int16_t arrays make use of DSP instructions.
     */
    void operator+=(const Array& another)
    { 
      addArrayToArray((T*)another, _array, _length);
    }
    void operator+=(const Array&& another)
    {
      *this+=another;
    }

    /**
     * @brief Subtract an array
     * 
     * @param another 
     * @note Float arrays make use of DSP instructions.
     */
    void operator-=(const Array& another)
    {
      subArrayFromArray((T*)another, _array, _length);
    }
    void operator-=(const Array&& another)
    {
      *this-=another;
    }

    /**
     * @brief Multiply by an array
     * 
     * @param another 
     * @note Float and int16_t arrays make use of DSP instructions.
     */
    void operator*=(const Array& another)
    {
      mulArrayByArray((T*)another, _array, _length);
    }
    void operator*=(const Array&& another)
    {
      *this*=another;
    }

    /**
     * @brief Divide by an array
     * 
     * @param another 
     * 
     */
    void operator/=(const Array& another)
    {
      (*this) = (*this) / (another);
    }
    void operator/=(const Array&& another)
    {
      *this/=another;
    }

    /**
     * @brief Concatenate a value
     * 
     * @param value 
     * @return Array& 
     */
    const Array& operator<<(const T value)
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
    const Array& operator<<(const Array& another)
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
        heap_caps_aligned_free(_array);
        (T*)heap_caps_aligned_alloc(ALIGNMENT, _size, this->memCaps());
      }
      else
        (T*)heap_caps_aligned_alloc(ALIGNMENT, _size, this->memCaps());

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
    const Array conv(const Array& kernel)
    {
      const size_t outputLength = _length + kernel.length() -1;
      Array convOutput = Array<T>(outputLength);

      float output[outputLength];
      float itself[_length];
      float _kernel[kernel.length()];

      cpyArray(_array, itself, _length);
      cpyArray(kernel, _kernel, kernel.length());
      dsps_conv_f32(itself, _length, _kernel, kernel.length(), output);
      cpyArray(output, convOutput, outputLength);
      return convOutput;
    }

    /**
     * @brief Copy another array into this one
     * 
     * @param another 
     */
    void copy(const Array& another)
    {
      if (_array)
        heap_caps_aligned_free(_array);

      _length = another.length();
      _size = another.memSize();
      _array = (T*)heap_caps_aligned_alloc(ALIGNMENT, _size, memCaps());
      
      for(size_t i = 0; i < _length; i++)
        _array[i] = another[i];
    }

    /**
     * @brief Get the correlation array with the given pattern
     * 
     * @param pattern 
     * @return Array<float>
     * 
     * @note Float arrays make use of DSP instructions.
     */
    const Array<float> correlation(const Array& pattern)
    {
      Array<float> corr = Array<float>(_length);

      float itself[_length];
      float _pattern[pattern.length()];

      cpyArray(_array, itself, _length);
      cpyArray(pattern, _pattern, pattern.length());
      dsps_corr_f32(itself, _length, _pattern, pattern.length(), corr);
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
    const bool diff(const Array& another, const float EPSILON = 0.0001)
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
    T* getArrayPntr() const
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

    /**
     * @brief Get the total bytes to be allocated
     * 
     * @param min The minimum quantity of memory blocks
     * @return size_t 
     */
    virtual size_t _mem2alloc(const size_t min){
      return min*sizeof(T);
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
    dsps_addc_f32(_array, _array, _length, value, 1, 1);
  }

  template<>
  inline void Array<float>::operator*=(const float value)
  { 
    dsps_mulc_f32(_array, _array, _length, value, 1, 1);
  }

  template<>
  inline void Array<int16_t>::operator*=(const int16_t value)
  { 
    dsps_mulc_s16_esp(_array, _array, _length, value);
  }

  template<>
  inline void Array<float>::operator+=(const Array<float>& another)
  {
    dsps_add_f32(_array, another, _array, _length, 1, 1, 1);
  }

  template<>
  inline void Array<int16_t>::operator+=(const Array<int16_t>& another)
  {
    dsps_add_s16(_array, another, _array, _length, 1, 1, 1, 0);
  }

  template<>
  inline void Array<float>::operator-=(const Array<float>& another)
  {
    dsps_sub_f32(_array, another, _array, _length, 1, 1, 1);
  }

  template<>
  inline void Array<float>::operator*=(const Array<float>& another)
  {
    dsps_mul_f32(_array, another, _array, _length, 1, 1, 1);
  }

  template<>
  inline void Array<int16_t>::operator*=(const Array<int16_t>& another)
  {
    dsps_mul_s16(_array, another, _array, _length, 1, 1, 1, 0);
  }

  template<>
  inline const bool Array<float>::operator==(const float value)
  {
    size_t i = 0;
    while(i < _length)
    {
      if(eqFloats(_array[i++], value))
        return true;
    }
    return false;
  }

  template <>
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
  inline const Array<float> Array<float>::conv(const Array<float>& kernel)
  {
    const size_t outputLength = _length + kernel.length() -1;
    Array<float> convOutput = Array<float>(outputLength);
    dsps_conv_f32(_array, _length, kernel, kernel.length(), convOutput);
    return convOutput;
  }

  template<>
  inline const Array<float> Array<float>::correlation(const Array<float>& pattern)
  {
    Array<float> corr = Array<float>(_length);
    dsps_corr_f32(_array, _length, pattern, pattern.length(), corr);
    return corr;
  }

  template<>
  inline const bool Array<float>::diff(const Array<float>& another, const float EPSILON)
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
  inline const Array<T> operator+(const Array<T>& onearray, const Array<T> another)
  {
    Array<T> newArray = Array<T>((T*)onearray, onearray.length());
    newArray += another;
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
  inline const Array<T> operator+(const Array<T>& onearray, const T value)
  {
    Array<T> newArray = Array<T>((T*)onearray, onearray.length());
    newArray += value;
    return newArray;
  }

  /**
   * @brief Add an array to a constant
   * 
   * @param another 
   * @return Array
   */
  template<typename T>
  inline const Array<T> operator+(const T value, const Array<T> another)
  {
    return another + value;
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
  inline const Array<T> operator-(const Array<T>& onearray, const Array<T> another)
  {
    Array<T> newArray = Array<T>((T*)onearray, onearray.length());
    newArray -= another;
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
  inline const Array<T> operator-(const Array<T>& onearray, const T value)
  {
    Array<T> newArray = Array<T>((T*)onearray, onearray.length());
    newArray -= value;
    return newArray;
  }

  /**
   * @brief Subtract a constant from an array
   * 
   * @param another 
   * @return Array 
   */
  template<typename T>
  inline const Array<T> operator-(const T value, const Array<T> onearray)
  {
    const T minus1 = -1;
    Array<T> newArray = onearray - value;
    return newArray*minus1;
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
  inline const Array<T> operator*(const Array<T>& onearray, const Array<T> another)
  {
    Array<T> newArray = Array<T>((T*)onearray, onearray.length());
    newArray *= another;
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
  inline const Array<T> operator*(const Array<T>& onearray, const T value)
  {
    Array<T> newArray = Array<T>((T*)onearray, onearray.length());
    newArray *= value;
    return newArray;
  }

  /**
   * @brief Multiply a constant by an array
   * 
   * @param another 
   * @return Array 
   */
  template<typename T>
  inline const Array<T> operator*(const T value, const Array<T> another)
  {
    Array<T> newArray = Array<T>(another, another.length());
    newArray *= value;
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
  inline const Array<float> operator/(const Array<T>& onearray, const float value)
  {
    Array<float> newArray;
    float input[onearray.length()];

    cpyArray((T*)onearray, input, onearray.length());  
    newArray = Array<float>(input, onearray.length());

    newArray /= value;
    return newArray;
  }

  template<>
  inline const Array<float> operator/(const Array<float>& onearray, const float value)
  {
    Array<float> newArray = Array<float>(onearray, onearray.length());
    newArray /= value;
    return newArray;
  }

  /**
   * @brief Divide a constant by an array
   * 
   * @param another 
   * @return Array 
   */
  template<typename T>
  inline const Array<float> operator/(const float value, const Array<T> another)
  {
    float output[another.length()];
    for(size_t i = 0; i < another.length(); i++)
    {
      output[i] = (float)(value / (float)another[i]);
    }

    Array<float> newArray = Array<float>(output, another.length());
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
  inline const Array<float> operator/(const Array<T>& onearray, const Array<T> another)
  {
    Array<float> newArray;
    float input[onearray.length()];

    cpyArray((T*)onearray, input, onearray.length());  
    newArray = Array<float>(input, onearray.length());

    newArray *= (1.0 / another);
    return newArray;
  }

  template<>
  inline const Array<float> operator/(const Array<float>& onearray, const Array<float> another)
  {
    Array<float> newArray = Array<float>(onearray, onearray.length());
    newArray *= (1.0 / another);
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
  inline const T operator^(const Array<T>& onearray, const Array<T> another)
  {
    return -1;
  }

  template<>
  inline const int32_t operator^(const Array<int32_t>& onearray, const Array<int32_t> another)
  {
    int32_t result;
    float input1[onearray.length()];
    float input2[onearray.length()];
    float r;
    cpyArray((int32_t*)onearray, input1, onearray.length());
    cpyArray((int32_t*)another, input2, onearray.length());
    dsps_dotprod_f32(input1, input2, &r, onearray.length());
    result = (int32_t)r;
    return result;
  }

  template<>
  inline const int8_t operator^(const Array<int8_t>& onearray, const Array<int8_t> another)
  {
    int8_t result;
    int16_t input1[onearray.length()];
    int16_t input2[onearray.length()];
    int16_t r;
    cpyArray((int8_t*)onearray, input1, onearray.length());
    cpyArray((int8_t*)another, input2, onearray.length());
    dsps_dotprod_s16(input1, input2, &r, onearray.length(), 0);
    result = (int8_t)r;
    return result;
  }

  template<>
  inline const float operator^(const Array<float>& onearray, const Array<float> another)
  {
    float result;
    dsps_dotprod_f32(onearray, another, &result, onearray.length());
    return result;
  }

  template<>
  inline const int16_t operator^(const Array<int16_t>& onearray, const Array<int16_t> another)
  {
    int16_t result;
    dsps_dotprod_s16((int16_t *)onearray, another, &result, onearray.length(), 0);
    return result;
  }
}

#endif