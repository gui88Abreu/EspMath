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
    FixedPoint(float value, uint8_t fracBits = FRACTIONAL){frac = fracBits, data = toFixed(value);}
    FixedPoint(const FixedPoint& other):data(other.data),frac(other.frac){}
    FixedPoint(const FixedPoint&& other):data(other.data),frac(other.frac){}

    // Cast Operator
    operator float() const {return toFloat(data);}

    // Assign Operators
    void operator=(const FixedPoint& other){data = other.data, frac = other.frac;}
    void operator=(const FixedPoint&& other){data = other.data, frac = other.frac;}
    void operator=(float value){data = toFixed(value);}

    // Comparison Operators
    bool operator==(FixedPoint& other){return data == other.data && frac == other.frac;}
    bool operator!=(FixedPoint& other){return data != other.data && frac != other.frac;}
    bool operator!(){return !data;}
    bool operator>(FixedPoint& other){assert(frac==other.frac); return data > other.data;}
    bool operator>(float other){return *this > FixedPoint(other);}
    bool operator>=(FixedPoint& other){assert(frac==other.frac); return data >= other.data;}
    bool operator>=(float other){return *this >= FixedPoint(other);}
    bool operator<(FixedPoint& other){assert(frac==other.frac); return data < other.data;}
    bool operator<(float other){return *this < FixedPoint(other);}
    bool operator<=(FixedPoint& other){assert(frac==other.frac); return data <= other.data;}
    bool operator<=(float other){return *this <= FixedPoint(other);}
    
    // Arithmetic Operators
    void operator+=(FixedPoint& other){assert(frac == other.frac); data += other.data;}
    void operator-=(FixedPoint& other){assert(frac == other.frac); data -= other.data;}
    void operator*=(FixedPoint& other){assert(frac == other.frac); data = FP_MUL(data, other.data, frac);}
    void operator/=(FixedPoint& other){assert(frac == other.frac); data = FP_DIV(data, other.data, frac);}

    static float toFloat(int16_t value)
    {
      float f;
      dsps_s16_f32_esp(&value, &f, 1);
      return f;
    }

    static int16_t toFixed(float value)
    {
      int16_t f;
      dsps_f32_s16_esp(&value, &f, 1);
      return f;
    }
  }fixed;

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