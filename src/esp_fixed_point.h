#ifndef _ESP_FIXED_POINT_H_
#define _ESP_FIXED_POINT_H_

#include <Arduino.h>
#include "esp_ansi.h"

#define DEFAULT_FRAC 6
#define FP_MUL(x, y, f) (((int32_t)(x) * (y)) >> (f))
#define FP_DIV(x, y, f) (((int32_t)(x) << 2*(f)) / (y))

namespace espmath{
  /**
   * @brief Fixed Point Representation
   * 
   */
  class FixedPoint
  {
  public:
    int16_t data = 0;
    uint8_t frac = DEFAULT_FRAC;

    // Constructors
    FixedPoint():data(DEFAULT_FRAC),frac(DEFAULT_FRAC){}
    FixedPoint(int16_t value, uint8_t fracBits = DEFAULT_FRAC){data = value, frac = fracBits;}
    FixedPoint(float value, uint8_t fracBits = DEFAULT_FRAC):frac(fracBits){data = float2fixed(value, fracBits);}
    FixedPoint(FixedPoint& other):FixedPoint(other.data, other.frac){}
    FixedPoint(FixedPoint&& other):FixedPoint(other.data, other.frac){}

    // Cast Operators
    template<typename _type>
    operator _type(){return (_type)data;} // default
    operator int16_t(){return data;} // 16 bits
    operator float(){return fixed2float(data, frac);} // float
    operator double(){return (double)fixed2float(data, frac);} // double
    operator int16_t*(){return &data;} // pointer

    // Assign Operators
    void operator=(FixedPoint& other){data = other.data, frac = other.frac;}
    void operator=(FixedPoint&& other){data = other.data, frac = other.frac;}
    void operator=(uint16_t value){data = value;}
    void operator=(float value){data = float2fixed(value, frac);}

    // Comparison Operators
    bool operator==(FixedPoint& other){return data == other.data && frac == other.frac;}
    bool operator!=(FixedPoint& other){return data != other.data && frac != other.frac;}
    bool operator>(FixedPoint& other){assert(frac==other.frac); return data > other.data;}
    bool operator>=(FixedPoint& other){assert(frac==other.frac); return data >= other.data;}
    bool operator<(FixedPoint& other){assert(frac==other.frac); return data < other.data;}
    bool operator<=(FixedPoint& other){assert(frac==other.frac); return data <= other.data;}
    
    // Arithmetic Operators
    void operator+=(FixedPoint& other){assert(frac == other.frac); data += other.data;}
    void operator-=(FixedPoint& other){assert(frac == other.frac); data -= other.data;}
    void operator*=(FixedPoint& other){assert(frac == other.frac); data = FP_MUL(data, other.data, frac);}
    void operator/=(FixedPoint& other){assert(frac == other.frac); data = FP_DIV(data, other.data, frac);}

    const int16_t& ref = data;
  };

  FixedPoint operator+(FixedPoint& fp1, FixedPoint& fp2);
  FixedPoint operator-(FixedPoint& fp1, FixedPoint& fp2);
  FixedPoint operator*(FixedPoint& fp1, FixedPoint& fp2);
  FixedPoint operator/(FixedPoint& fp1, FixedPoint& fp2);

  FixedPoint operator+(FixedPoint& fp1, float f);
  FixedPoint operator-(FixedPoint& fp1, float f);
  FixedPoint operator*(FixedPoint& fp1, float f);
  FixedPoint operator/(FixedPoint& fp1, float f);
}

#endif