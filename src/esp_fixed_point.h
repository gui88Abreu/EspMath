#ifndef _ESP_FIXED_POINT_H_
#define _ESP_FIXED_POINT_H_

#include <Arduino.h>
#include "esp_ansi.h"
#include "esp_opt.h"
#include "dsp/fixed/converter.h"
#include "dsp/mulc/dsps_mulc_esp.h"
#include "dsp/divc/dsps_divc_esp.h"

#define FP_MUL(x, y, f) ((((long)x) * (y)) >> (f))
#define FP_DIV(x, y, f) ((((long)x) << (f)) / (y))

namespace espmath{
  /**
   * @brief Fixed Point Representation
   * 
   */
  typedef class FixedPoint
  {
  public:
    int16_t data = 0;
    uint8_t frac = FRACTIONAL;

    // Constructors
    FixedPoint(){}
    FixedPoint(float value, uint8_t fracBits = FRACTIONAL){frac = fracBits, data = toFixed(value, frac);}
    FixedPoint(const FixedPoint& other):data(other.data),frac(other.frac){}
    FixedPoint(const FixedPoint&& other):data(other.data),frac(other.frac){}

    // Cast Operator
    operator float() const {return toFloat(data, frac);}

    // Assign Operators
    void operator=(const FixedPoint& other){data = other.data, frac = other.frac;}
    void operator=(const FixedPoint&& other){data = other.data, frac = other.frac;}
    void operator=(float value){data = toFixed(value, frac);}

    // Comparison Operators
    bool operator==(FixedPoint& other){return data == other.data && frac == other.frac;}
    bool operator!=(FixedPoint& other){return data != other.data && frac != other.frac;}
    bool operator!(){return !data;}
    
    // Arithmetic Operators
    void operator+=(FixedPoint& other){assert(frac == other.frac); data += other.data;}
    void operator-=(FixedPoint& other){assert(frac == other.frac); data -= other.data;}
    void operator*=(FixedPoint& other){assert(frac == other.frac); data = FP_MUL(data, other.data, frac);}
    void operator/=(FixedPoint& other){assert(frac == other.frac); data = FP_DIV(data, other.data, frac);}

    static float toFloat(int16_t value, int frac)
    {
      float f;
      dsps_s16_f32_esp(&value, &f, frac);
      return f;
    }

    static int16_t toFixed(float value, int frac)
    {
      int16_t f;
      dsps_f32_s16_esp(&value, &f, frac);
      return f;
    }
  }fixed;

  /* Extended Comparison Operators*/
  bool operator>(FixedPoint& one, FixedPoint& another);
  bool operator>(FixedPoint& one, float another);
  bool operator>(float another, FixedPoint& one);
  bool operator>=(FixedPoint& one, FixedPoint& another);
  bool operator>=(FixedPoint& one, float another);
  bool operator>=(float another, FixedPoint& one);
  bool operator<(FixedPoint& one, FixedPoint& another);
  bool operator<(FixedPoint& one, float another);
  bool operator<(float another, FixedPoint& one);
  bool operator<=(FixedPoint& one, FixedPoint& another);
  bool operator<=(FixedPoint& one, float another);
  bool operator<=(float another, FixedPoint& one);

  /* Extended Arithmetic Operators*/
  fixed operator+(fixed f1, fixed f2);
  fixed operator-(fixed f1, fixed f2);
  fixed operator*(fixed f1, fixed f2);
  fixed operator/(fixed f1, fixed f2);
  fixed operator+(fixed f1, float f);
  fixed operator-(fixed f1, float f);
  fixed operator*(fixed f1, float f);
  fixed operator/(fixed f1, float f);
  fixed operator+(float f, fixed f1);
  fixed operator-(float f, fixed f1);
  fixed operator*(float f, fixed f1);
  fixed operator/(float f, fixed f1);
  template<typename T>
  fixed operator+(fixed f1, T t){return f1 + (float)t;}
  template<typename T>
  fixed operator-(fixed f1, T t){return f1 - (float)t;}
  template<typename T>
  fixed operator*(fixed f1, T t){return f1 * (float)t;}
  template<typename T>
  fixed operator/(fixed f1, T t){return f1 / (float)t;}
  template<typename T>
  fixed operator+(T t, fixed f1){return f1 + (float)t;}
  template<typename T>
  fixed operator-(T t, fixed f1){return (float)t - f1;}
  template<typename T>
  fixed operator*(T t, fixed f1){return (float)t * f1;}
  template<typename T>
  fixed operator/(T t, fixed f1){return (float)t / f1;}
}

#endif