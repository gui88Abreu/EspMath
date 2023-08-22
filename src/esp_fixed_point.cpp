#include "esp_fixed_point.h"

namespace espmath{
  FixedPoint operator+(FixedPoint& fp1, FixedPoint& fp2)
  {
    FixedPoint result = FixedPoint(fp1);
    result+=fp2;
    return result;
  }

  FixedPoint operator-(FixedPoint& fp1, FixedPoint& fp2)
  {
    FixedPoint result = FixedPoint(fp1);
    result-=fp2;
    return result;
  }

  FixedPoint operator*(FixedPoint& fp1, FixedPoint& fp2)
  {
    FixedPoint result = FixedPoint(fp1);
    result+=fp2;
    return result;
  }

  FixedPoint operator/(FixedPoint& fp1, FixedPoint& fp2)
  {
    FixedPoint result = FixedPoint(fp1);
    result/=fp2;
    return result;
  }

  FixedPoint operator+(FixedPoint& fp1, float f)
  {
    FixedPoint result = FixedPoint(fp1);
    FixedPoint fp2 = FixedPoint(f, fp1.frac);

    result+=fp2;
    return result;
  }

  FixedPoint operator-(FixedPoint& fp1, float f)
  {
    FixedPoint result = FixedPoint(fp1);
    FixedPoint fp2 = FixedPoint(f, fp1.frac);

    result-=fp2;
    return result;
  }

  FixedPoint operator*(FixedPoint& fp1, float f)
  {
    FixedPoint result = FixedPoint(fp1);
    FixedPoint fp2 = FixedPoint(f, fp1.frac);

    result*=fp2;
    return result;
  }

  FixedPoint operator/(FixedPoint& fp1, float f)
  {
    FixedPoint result = FixedPoint(fp1);
    FixedPoint fp2 = FixedPoint(f, fp1.frac);

    result/=fp2;
    return result;
  }
}