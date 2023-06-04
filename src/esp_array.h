#ifndef _ESP_ARRAY_H_
#define _ESP_ARRAY_H_

#include <Arduino.h>
#include <type_traits>
#include <esp_dsp.h>

/**
 * @brief Namespace for custom ESP32 MATH libraries
 * 
 */
namespace espmath{
  using namespace espmath;

  /**
   * @brief Compare floats with different tolerances.
   * 
   * @param f1
   * @param f2
   * @param EPSILON tolerance
   * @return true floats are equal
   * @return false floats aren't equal
   */
  bool eqFloats(const float f1, const float f2, const float EPSILON = 0.0001)
  {
   return (fabs(f1 - f2) <= EPSILON);
  }

  /**
   * @brief Copy array and multiply it by a constant
   * 
   * @tparam T1 Type of the source array.
   * @tparam T2 Type of the destination array.
   * @param src Source array.
   * @param dest Destination array.
   * @param len Quantity of elements to copy.
   * @param cnst Constant to multiply the output.
   */
  template<typename T1, typename T2>
  void cpyArray(T1* src, T2* dest, size_t len, const T2 cnst = 1)
  {
    for(size_t i = 0; i < len; i++)
      dest[i] = (T2)(src[i]*cnst);
  }

  /**
   * @brief Custom Array implementation suitable for ESP32 devices.
   * 
   * This implements an array suitable for ESP32 boards.
   * Using this class, you can create an array of any arithmetic type.
   * 
   * @note Arithmetic operations make use of DSP instructions.
   * 
   * @tparam T Array type
   */
  template <typename T = float> class Array
  {
    static_assert(std::is_arithmetic<T>::value, "Arrays must support arithmetic operations!");
    static_assert(std::is_signed<T>::value, "Arrays must be signed!");
    static_assert(sizeof(T) <= 4, "Long types are not supported!");
  public:

    typedef T* arrayPntr;

    /**
     * @brief Supported array types
     * 
     */
    enum SupportedTypes{
      int8_type,
      int16_type,
      int32_type,
      float_type
    };

    /**
     * @brief Get the Array Type object
     * 
     * @param someArray 
     * @return SupportedTypes 
     */
    static const SupportedTypes getType(const int8_t* someArray)
    {
      return int8_type;
    }

    /**
     * @brief Get the Array Type object
     * 
     * @param someArray 
     * @return SupportedTypes 
     */
    static const SupportedTypes getType(const int16_t* someArray)
    {
      return int16_type;
    }

    /**
     * @brief Get the Array Type object
     * 
     * @param someArray 
     * @return SupportedTypes 
     */
    static const SupportedTypes getType(const int32_t* someArray)
    {
      return int32_type;
    }

    /**
     * @brief Get the Array Type object
     * 
     * @param someArray 
     * @return SupportedTypes 
     */
    static const SupportedTypes getType(const float* someArray)
    {
      return float_type;
    }

    /**
     * @brief Destroy the Array object
     * 
     */
    ~Array()
    {
      heap_caps_free(_array);
    }

    /**
     * @brief Construct a new Array object
     * 
     * @param initialMem The initial size (amount of memory blocks) of the array.
     * @param __persistent If true, the object won't be deleted after the next array operation.
     */
    Array(const size_t initialMem = 0, bool __persistent = true)
    {
      _persistent = __persistent;
       _size = _mem2alloc(initialMem);
      _array = _size > 0 ? (T*)heap_caps_malloc(_size, this->memCaps()) : NULL;
    }
    
    /**
     * @brief Construct a new Array object with initial values
     * 
     * @param initialValues Initial values of the array.
     * @param initialMem The initial size (amount of memory blocks) of the array.
     * @param __persistent If true, the object won't be deleted after the next array operation.
     */
    Array(const T* initialValues,\
          const size_t initialMem = 0,\
          bool __persistent = true):Array(initialMem, __persistent)
    {
      if (!_array)
        return;
        
      while(_length < initialMem)
      {
        _array[_length] = initialValues[_length];
        _length++;
      }
    }

    /**
     * @brief Construct a new Array object from another array
     * 
     * @param another 
     * @param __persistent If true, the object won't be deleted after the next array operation.
     */
    Array(Array<T>& another, bool __persistent = true)
    {
      *this = another;
      _persistent = __persistent;
    }

    /**
     * @brief Get array element
     * 
     * @param index 
     * @return T 
     */
    T& operator[](const size_t index)
    {
      return (*((T*)_array + index));
    }

    /**
     * @brief Assign another object to this object
     * 
     * @param another 
     * @return Array& 
     */
    Array& operator=(Array& another)
    {
      if (_array)
        heap_caps_free(_array);
      _length = another.length();
      _size = another.memSize();
      _array = (T*)heap_caps_malloc(_size, this->memCaps());

      for(size_t i = 0; i < _length; i++)
        _array[i] = another[i];
      
      if (!another.persistent())
        delete &another;

      return *this;
    }

    /**
     * @brief Verify if a value belongs to the array.
     * 
     * @param value The value to be verified.
     * @return true There is at least 1 value in the _array.
     * @return false The value is no present in the _array.
     */
    bool operator==(const T value)
    {
      size_t i = 0;
      
      if (getType(_array) == float_type)
      {
        while(i < _length)
        {
          if(eqFloats(_array[i++], value))
            return true;
        }
      }
      else
      {
        while(i < _length)
        {
          if(_array[i++] == value)
            return true;
        }
      }

      if(!_persistent)
        delete this;

      return false;
    }

    /**
     * @brief Verify if a value doesn't belong to the array.
     * 
     * @param value The value to be verified.
     * @return true The value is no present in the _array.
     * @return false There is at least 1 value in the _array.
     */
    bool operator!=(const T value) const
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
    bool operator==(const T* input)
    {
      size_t i = 0;
      if (getType(_array) == float_type)
      {
        while( i < _length)
        {
          if (!eqFloats(_array[i], input[i]))
            return false;
          i++;
        }
      }
      else
      {
        while( i < _length)
        {
          if (_array[i] != input[i])
            return false;
          i++;
        }
      }

      if(!_persistent)
        delete this;
      return true;
    }

    /**
     * @brief Verify if all two arrays are identical.
     * 
     * @param input The input array
     * @return true Every value of the _array is contained in input.
     * @return false Not all values of the _array are contained in input.
     */
    bool operator==(Array& another)
    {
      bool result = *this == another.getArrayPntr();
      if (!another.persistent())
        delete &another;
      return result;
    }

    /**
     * @brief Verify if all two arrays are not identical regardless the value order.
     * 
     * @param input The input _array
     * @return true Not all values of the _array are contained in input.
     * @return false Every value of the _array is contained in input.
     */
    bool operator!=(const T* input)
    {
      return !(*this == input);
    }

    /**
     * @brief Add a constant
     * 
     * @param value 
     * @return Array& 
     */
    Array& operator+=(const T value)
    {
      switch(getType(_array))
      {
        case float_type:
        {
          dsps_addc_f32_ae32((float*)_array, (float*)_array, _length, value, 1, 1);
          break;
        }
        case int8_type:
        case int16_type:
        case int32_type:
        default:
        {
          float _arrayCpy[_length];
          cpyArray(_array, _arrayCpy, _length);
          dsps_addc_f32_ae32(_arrayCpy, _arrayCpy,_length, value, 1, 1);
          cpyArray(_arrayCpy, _array, _length);
          break;
        }
      }
      return *this;
    }

    /**
     * @brief Subtract a constant
     * 
     * @param value 
     * @return Array& 
     */
    Array& operator-=(const T value)
    {
      *this += value*(-1);
      return *this;
    }

    /**
     * @brief Multiply by a constant
     * 
     * @param value 
     * @return Array& 
     */
    Array& operator*=(const T value)
    { 
      switch(getType(_array))
      {
        case float_type:
        {
          dsps_mulc_f32_ae32((float*)_array, (float*)_array, _length, value, 1, 1);
          break;
        }
        case int8_type:
        case int16_type:
        case int32_type:
        default:
        {
          float _arrayCpy[_length];
          cpyArray(_array, _arrayCpy, _length);
          dsps_mulc_f32_ae32(_arrayCpy, _arrayCpy, _length, value, 1, 1);
          cpyArray(_arrayCpy, _array, _length);
          break;
        }
      }
      return *this;
    }

    /**
     * @brief Divide by a constant
     * 
     * @param value 
     * @return Array& 
     */
    Array& operator/=(const float value)
    {
      (*this) *= (1.0/value);

      return *this;
    }

    /**
     * @brief Add an array
     * 
     * @param another 
     * @return Array& 
     */
    Array& operator+=(Array& another)
    { 
      switch(getType(_array))
      {
        case int8_type:
        {
          int16_t _arrayCpy[_length];
          int16_t _anotherCpy[_length];
          cpyArray(_array, _arrayCpy, _length);
          cpyArray(another.getArrayPntr(), _anotherCpy, _length);
          dsps_add_s16_ae32(_arrayCpy, _anotherCpy, _arrayCpy, _length, 1, 1, 1, 0);
          cpyArray(_arrayCpy, _array, _length);
          break;
        }
        case int16_type:
        {
          dsps_add_s16_ae32((int16_t*)_array, (int16_t*)another.getArrayPntr(), (int16_t*)_array, _length, 1, 1, 1, 0);
          break;
        }
        case float_type:
        {
          dsps_add_f32_ae32((float*)_array, (float*)another.getArrayPntr(), (float*)_array, _length, 1, 1, 1);
          break;
        }
        default: case int32_type:
        {
          float _arrayCpy[_length];
          float _anotherCpy[_length];
          cpyArray(_array, _arrayCpy, _length);
          cpyArray(another.getArrayPntr(), _anotherCpy, _length);
          dsps_add_f32_ae32(_arrayCpy, _anotherCpy, _arrayCpy, _length, 1, 1, 1);
          cpyArray(_arrayCpy, _array, _length);
          break;
        }
      }

      if (!another.persistent())
        delete &another;
      return *this;
    }

    /**
     * @brief Subtract an array
     * 
     * @param another 
     * @return Array& 
     */
    Array& operator-=(Array& another)
    {
      switch(getType(_array))
      {
        case int8_type:
        {
          int16_t _arrayCpy[_length];
          int16_t negativeAnother[_length];
          cpyArray(_array, _arrayCpy, _length);
          cpyArray(another.getArrayPntr(), negativeAnother, _length, (int16_t)-1);
          dsps_add_s16_ae32(_arrayCpy, negativeAnother, _arrayCpy, _length, 1, 1, 1, 0);
          cpyArray(_arrayCpy, _array, _length);
          break;
        }
        case int16_type:
        {
          int16_t negativeAnother[_length];
          cpyArray(another.getArrayPntr(), negativeAnother, _length, (int16_t)-1);
          dsps_add_s16_ae32((int16_t*)_array, negativeAnother, (int16_t*)_array, _length, 1, 1, 1, 0);
          break;
        }
        case float_type:
        {
          dsps_sub_f32_ae32((float*)_array, (float*)another.getArrayPntr(), (float*)_array, _length,1,1,1);
          break;
        }
        default: case int32_type:
        {
          float _arrayCpy[_length];
          float _anotherCpy[_length];
          cpyArray(_array, _arrayCpy, _length);
          cpyArray(another.getArrayPntr(), _anotherCpy, _length);
          dsps_sub_f32_ae32(_arrayCpy, _anotherCpy, _arrayCpy, _length, 1, 1, 1);
          cpyArray(_arrayCpy, _array, _length);
          break;
        }
      }

      if (!another.persistent())
        delete &another;

      return *this;
    }

    /**
     * @brief Multiply by an array
     * 
     * @param another 
     * @return Array& 
     */
    Array& operator*=(Array& another)
    {
      switch(getType(_array))
      {
        case int8_type:
        {
          int16_t _arrayCpy[_length];
          int16_t _anotherCpy[_length];
          cpyArray(_array, _arrayCpy, _length);
          cpyArray(another.getArrayPntr(), _anotherCpy, _length);
          dsps_mul_s16_ansi(_arrayCpy, _anotherCpy, _arrayCpy, _length, 1, 1, 1, 0);
          cpyArray(_arrayCpy, _array, _length);
          break;
        }
        case int16_type:
        {
          dsps_mul_s16_ansi((int16_t*)_array, (int16_t*)another.getArrayPntr(), (int16_t*)_array, _length, 1, 1, 1, 0);
          break;
        }
        case float_type:
        {
          dsps_mul_f32_ae32((float*)_array, (float*)another.getArrayPntr(), (float*)_array, _length,1,1,1);
          break;
        }
        default: case int32_type:
        {
          float _arrayCpy[_length];
          float _anotherCpy[_length];
          cpyArray(_array, _arrayCpy, _length);
          cpyArray(another.getArrayPntr(), _anotherCpy, _length);
          dsps_mul_f32_ae32(_arrayCpy, _anotherCpy, _arrayCpy, _length, 1, 1, 1);
          cpyArray(_arrayCpy, _array, _length);
          break;
        }
      }
      
      if (!another.persistent())
        delete &another;
      
      return *this;
    }

    /**
     * @brief Divide by an array
     * 
     * @param another 
     * @return Array& 
     */
    Array& operator/=(Array& another)
    {
      (*this) = (*this) / (another);
      if (!another.persistent())
        delete &another;
      return *this;
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

      if (!another.persistent())
        delete &another;

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
    bool assign(const T value, const size_t index)
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
    bool append(const T value)
    {
      if(_length < _size/sizeof(T))
      {
        (*this)[_length++] = value;
        return true;
      }

      _size = _mem2alloc(_length+1);
      _array = _array 
                ? (T*)heap_caps_realloc(_array, _size, this->memCaps())
                : (T*)heap_caps_malloc(_size, this->memCaps());

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
    bool append(Array& another)
    {
      *this << another;
      if (!another.persistent())
        delete &another;
      return _array == NULL ? false : true;
    }

    /**
     * @brief Compares to another array
     * 
     * @param another Another array
     * @param EPSILON tolerance. Only used when comparing float arrays.
     * @return true Two differents arrays
     * @return false Array are not different
     */
    bool diff(Array& another, const float EPSILON = 0.0001)
    {
      if (getType(_array))
      {
        for (size_t i = 0; i < _length; i++)
        {
          if (!eqFloats(_array[i], another[i], EPSILON))
            return true;
        }
      }
      else
      {
        for (size_t i = 0; i < _length; i++)
        {
          if (_array[i]!=another[i])
            return true;
        }
      }

      if(!another.persistent())
        delete another;
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

    /**
     * @brief Return the allocated memory capabilities.
     * 
     * @return uint32_t 
     */
    uint32_t memCaps() const
    {
      return (const uint32_t)(MALLOC_CAP_DEFAULT | MALLOC_CAP_8BIT);
    }

    /**
     * @brief Retrieve the array type
     * 
     * @return SupportedTypes
     */
    SupportedTypes myType() const
    {
      return getType(_array);
    }

    /**
     * @brief Get the persistent status
     * 
     * @return true The object won't be deleted after assign operation.
     * @return false 
     */
    bool persistent() const
    {
      return _persistent;
    }

    /**
     * @brief Modify the persistent status
     * 
     * @param __persistent If true, the object won't be deleted after the next array operation.
     * @return true 
     * @return false
     */
    bool persistent(bool __persistent)
    {
      _persistent = __persistent;
      return _persistent;
    }

  protected:
    bool _persistent = true; /*If true, the object won't be deleted after the next array operation.*/
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

  /**
   * @brief Add two arrays
   * 
   * @tparam T 
   * @param onearray 
   * @param another 
   * @return Array<T>& 
   */
  template<typename T>
  inline Array<T>& operator+(Array<T>& onearray, Array<T>& another)
  {
    Array<T>* newArray = new Array<T>(onearray.getArrayPntr(), onearray.length(), false);
    *newArray += another;
    if (!another.persistent())
      delete &another;
    if (!onearray.persistent())
      delete &onearray;
    return *newArray;
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
  inline Array<T>& operator+(Array<T>& onearray, const T value)
  {
    Array<T>* newArray = new Array<T>(onearray.getArrayPntr(), onearray.length(), false);
    *newArray += value;
    if (!onearray.persistent())
      delete &onearray;
    return *newArray;
  }

  /**
   * @brief Add an array to a constant
   * 
   * @param another 
   * @return Array& 
   */
  template<typename T>
  inline Array<T>& operator+(const T value, Array<T>& another)
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
  inline Array<T>& operator-(Array<T>& onearray, Array<T>& another)
  {
    Array<T>* newArray = new Array<T>(onearray.getArrayPntr(), onearray.length(), false);
    *newArray -= another;
    if (!another.persistent())
      delete &another;
    if (!onearray.persistent())
      delete &onearray;
    return *newArray;
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
  inline Array<T>& operator-(Array<T>& onearray, const T value)
  {
    Array<T>* newArray = new Array<T>(onearray.getArrayPntr(), onearray.length(), false);
    *newArray -= value;
    if (!onearray.persistent())
      delete &onearray;
    return *newArray;
  }

  /**
   * @brief Subtract a constant from an array
   * 
   * @param another 
   * @return Array& 
   */
  template<typename T>
  inline Array<T>& operator-(const T value, Array<T>& onearray)
  {
    const T minus1 = -1;
    return (onearray - value)*minus1;
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
  inline Array<T>& operator*(Array<T>& onearray, Array<T>& another)
  {
    Array<T>* newArray = new Array<T>(onearray.getArrayPntr(), onearray.length(), false);
    *newArray *= another;
    if (!another.persistent())
      delete &another;
    if (!onearray.persistent())
      delete &onearray;
    return *newArray;
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
  inline Array<T>& operator*(Array<T>& onearray, const T value)
  {
    Array<T>* newArray = new Array<T>(onearray.getArrayPntr(), onearray.length(), false);
    *newArray *= value;
    if (!onearray.persistent())
      delete &onearray;
    return *newArray;
  }

  /**
   * @brief Multiply a constant by an array
   * 
   * @param another 
   * @return Array& 
   */
  template<typename T>
  inline Array<T>& operator*(const T value, Array<T>& another)
  {
    Array<T>* newArray = new Array<T>(another.getArrayPntr(), another.length(), false);
    *newArray *= value;
    if (!another.persistent())
      delete &another;
    return *newArray;
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
  inline Array<float>& operator/(Array<T>& onearray, const float value)
  {
    Array<float>* newArray;
    float input[onearray.length()];

    if (onearray.myType() == Array<>::float_type)
    {
      newArray = new Array<float>((float*)onearray.getArrayPntr(), onearray.length());
    }
    else
    {
      cpyArray(onearray.getArrayPntr(), input, onearray.length());  
      newArray = new Array<float>(input, onearray.length());
    }

    *newArray /= value;
    
    if (!onearray.persistent())
      delete &onearray;
    return *newArray;
  }

  /**
   * @brief Divide a constant by an array
   * 
   * @param another 
   * @return Array& 
   */
  template<typename T>
  inline Array<float>& operator/(const float value, Array<T>& another)
  {
    float output[another.length()];
    for(size_t i = 0; i < another.length(); i++)
    {
      output[i] = (float)(value / (float)another[i]);
    }

    Array<float>* newArray = new Array<float>(output, another.length(), false);
    if (!another.persistent())
      delete &another;
    return *newArray;
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
  inline Array<float>& operator/(Array<T>& onearray, Array<T>& another)
  {
    Array<float>* newArray;
    float input[onearray.length()];

    if (onearray.myType() == Array<>::float_type)
    {
      newArray = new Array<float>((float*)onearray.getArrayPntr(), onearray.length());
    }
    else
    {
      cpyArray(onearray.getArrayPntr(), input, onearray.length());  
      newArray = new Array<float>(input, onearray.length());
    }

    *newArray *= (1.0 / another);
    
    if (!another.persistent())
      delete &another;
    if (!onearray.persistent())
      delete &onearray;

    newArray->persistent(false);
    return *newArray;
  }
}

#endif