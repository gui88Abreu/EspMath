#ifndef _ANSI_VERSION_H
#define _ANSI_VERSION_H

#include <Arduino.h>

namespace espmath{

  inline int16_t float2fixed(float num, uint8_t fractional){return (int16_t)(round(num * (1 << fractional)));}
  inline float fixed2float(float num, uint8_t fractional){return ((float)num / (float)(1 << fractional));}

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
   * @brief dest[i] = (T2)(src[i]*cnst);
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
   * @brief dest[i] = (T)(cnst);
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
   * @brief dest[i] = (T2)(src1[i] + src2)*cnst;
   * 
   * @tparam T1 Type of the source array.
   * @tparam T2 Type of the destination array.
   * @param src Source array.
   * @param dest Destination array.
   * @param len Quantity of elements to copy.
   * @param cnst Constant to multiply the output.
   */
  template<typename T1, typename T2>
  inline void addArrayToArray(const T1* src1, const T1* src2, T2* dest, const size_t len, const T2 cnst = 1)
  {
    for(size_t i = 0; i < len; i++)
      dest[i] = (T2)(src1[i] + src2[i])*cnst;
  }

  /**
   * @brief dest[i] = src[i] + cnst
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
   * @brief dest[i] = (T2)(src1[i] - src2[i])*cnst;
   * 
   * @tparam T1 Type of the source array.
   * @tparam T2 Type of the destination array.
   * @param src Source array.
   * @param dest Destination array.
   * @param len Quantity of elements to copy.
   * @param cnst Constant to multiply the output.
   */
  template<typename T1, typename T2>
  inline void subArrayFromArray(const T1* src1, const T1* src2, T2* dest, const size_t len, const T2 cnst = 1)
  {
    for(size_t i = 0; i < len; i++)
      dest[i] = (T2)(src1[i] - src2[i])*cnst;
  }

  /**
   * @brief dest[i] = (src[i] - cnst1)*cnst2;
   * 
   * @tparam T Type of the destination array.
   * @param src Source array.
   * @param dest Destination array.
   * @param len Quantity of elements to copy.
   * @param cnst1 Constant to be subtracted.
   * @param cnst1 Constant to multiply the output.
   */
  template<typename T>
  inline void subConstFromArray(T* src, T* dest, const size_t len, const T cnst1 = 1, const T cnst2 = 1)
  {
    for(size_t i = 0; i < len; i++)
      dest[i] = (src[i] - cnst1)*cnst2;
  }

  /**
   * @brief dest[i] = (T2)(src1[i]*src2[i])*cnst;
   * 
   * @tparam T1 Type of the source array.
   * @tparam T2 Type of the destination array.
   * @param src Source array.
   * @param dest Destination array.
   * @param len Quantity of elements to copy.
   * @param cnst Constant to multiply the output.
   */
  template<typename T1, typename T2>
  inline void mulArrayByArray(const T1* src1, const T1* src2, T2* dest, const size_t len, const T2 cnst = 1)
  {
    for(size_t i = 0; i < len; i++)
      dest[i] = (T2)(src1[i]*src2[i])*cnst;
  }

  /**
   * @brief dest[i] = src[i]*cnst;
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
   * @brief dest[i] = (float)((float)src1[i]/(float)src2[i])*cnst;
   * 
   * @tparam T Type of the array.
   * @param src Source array.
   * @param dest Destination array.
   * @param len Quantity of elements to copy.
   * @param cnst Constant to multiply the output.
   */
  template<typename T>
  inline void divArrayByArray(const T* src1, const T* src2, T* const dest, const size_t len, float cnst = 1)
  {
    for(size_t i = 0; i < len; i++)
      dest[i] = (src1[i] / src2[i])*cnst;
  }

  /**
   * @brief dest[i] = src[i] / cnst;
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
   * @brief dest[i] = cnst / (float)src[i];
   * 
   * @tparam T Type of the destination array.
   * @param src Source array.
   * @param dest Destination array.
   * @param len Quantity of elements to copy.
   * @param cnst Constant to multiply the output.
   */
  template<typename T>
  inline void divConstByArray(T* src, T* dest, const size_t len, const float cnst = 1)
  {
    for(size_t i = 0; i < len; i++)
      dest[i] = cnst / src[i];
  }

  template<typename T>
  inline T power(T x, unsigned n)
  {
    // Initialize result to 1
    T acc = 1;

    // Multiply x for n times
    for (int i = 0; i < n; i++) {
      acc = acc * x;
    }

    return acc;
  }

  /**
   * @brief Round a float value with a specific number of decimals
   * 
   * @param value Float value.
   * @param dec Desired decimals. Default is 2.
   * @return float rounded float number.
   */
  inline float roundF(float value, uint8_t dec = 2)
  {
    return round((value*power(10.f, dec))/power(10.f, dec));
  }
}

#endif