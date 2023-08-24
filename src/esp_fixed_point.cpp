#include "esp_fixed_point.h"
#include <type_traits>

namespace espmath{
  bool operator>(FixedPoint& one, FixedPoint& another)
  {
    assert(one.frac==another.frac); 
    return one.data > another.data;
  }

  bool operator>(FixedPoint& one, float another)
  {
    return one.data > FixedPoint(another).data;
  }

  bool operator>(float another, FixedPoint& one)
  {
    return FixedPoint(another).data > one.data;
  }

  bool operator>=(FixedPoint& one, FixedPoint& another)
  {
    assert(one.frac==another.frac); 
    return one.data >= another.data;
  }

  bool operator>=(FixedPoint& one, float another)
  {
    return one.data >= FixedPoint(another).data;
  }

  bool operator>=(float another, FixedPoint& one)
  {
    return FixedPoint(another).data >= one.data;
  }

  bool operator<(FixedPoint& one, FixedPoint& another)
  {
    assert(one.frac==another.frac); 
    return one.data < another.data;
  }

  bool operator<(FixedPoint& one, float another)
  {
    return one.data < FixedPoint(another).data;
  }

  bool operator<(float another, FixedPoint& one)
  {
    return FixedPoint(another).data < one.data;
  }

  bool operator<=(FixedPoint& one, FixedPoint& another)
  {
    assert(one.frac==another.frac); 
    return one.data <= another.data;
  }

  bool operator<=(FixedPoint& one, float another)
  {
    return one.data <= FixedPoint(another).data;
  }

  bool operator<=(float another, FixedPoint& one)
  {
    return FixedPoint(another).data <= one.data;
  }

  fixed operator+(fixed fp1, fixed fp2)
  {
    fp1+=fp2;
    return fp1;
  }

  fixed operator-(fixed fp1, fixed fp2)
  {
    fp1-=fp2;
    return fp1;
  }

  fixed operator*(fixed fp1, fixed fp2)
  {
    fp1*=fp2;
    return fp1;
  }

  fixed operator/(fixed fp1, fixed fp2)
  {
    fp1/=fp2;
    return fp1;
  }

  fixed operator+(fixed fp1, float f)
  {
    fixed fp2 = fixed(f, fp1.frac);
    fp1+=fp2;
    return fp1;
  }

  fixed operator-(fixed fp1, float f)
  {
    fixed fp2 = fixed(f, fp1.frac);
    fp1-=fp2;
    return fp1;
  }

  fixed operator*(fixed fp1, float f)
  {
    fixed fp2 = fixed(f, fp1.frac);
    fp1*=fp2;
    return fp1;
  }

  fixed operator/(fixed fp1, float f)
  {
    fixed fp2 = fixed(f, fp1.frac);
    fp1/=fp2;
    return fp1;
  }

  fixed operator+(float f, fixed fp2)
  {
    fixed fp1 = fixed(f, fp2.frac);
    fp1+=fp2;
    return fp1;
  }

  fixed operator-(float f, fixed fp2)
  {
    fixed fp1 = fixed(f, fp2.frac);
    fp1-=fp2;
    return fp1;
  }

  fixed operator*(float f, fixed fp2)
  {
    fixed fp1 = fixed(f, fp2.frac);
    fp1*=fp2;
    return fp1;
  }

  fixed operator/(float f, fixed fp2)
  {
    fixed fp1 = fixed(f, fp2.frac);
    fp1/=fp2;
    return fp1;
  }
}