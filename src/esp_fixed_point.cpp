#include "esp_fixed_point.h"
#include <type_traits>

namespace espmath{

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