#ifndef _ESP_FIXED_POINT_H_
#define _ESP_FIXED_POINT_H_

#include <Arduino.h>
#include "esp_ansi.h"

#define DEFAULT_FRAC 8
#define FP_MUL(x, y, f) ((((long long)x) * (y)) >> (f))
#define FP_DIV(x, y, f) ((((long long)x) << (f)) / (y))

namespace espmath{
  /**
   * @brief Fixed Point Representation
   * 
   */
  typedef class FixedPoint
  {
  public:
    int16_t data = 0;
    uint8_t frac = DEFAULT_FRAC;

    // Constructors
    FixedPoint():data(0),frac(DEFAULT_FRAC){}
    FixedPoint(float value, uint8_t fracBits = DEFAULT_FRAC):frac(fracBits){data = float2fixed(value, fracBits);}
    FixedPoint(const FixedPoint& other):FixedPoint(other.data, other.frac){}
    FixedPoint(const FixedPoint&& other):FixedPoint(other.data, other.frac){}

    // Cast Operator
    operator float() const {return fixed2float(data, frac);} // float

    // Assign Operators
    void operator=(const FixedPoint& other){data = other.data, frac = other.frac;}
    void operator=(const FixedPoint&& other){data = other.data, frac = other.frac;}
    void operator=(float value){data = float2fixed(value, frac);}
    template<typename T>
    void operator=(T value){data = float2fixed((float)value, frac);}

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