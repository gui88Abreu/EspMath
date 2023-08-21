#ifndef _ESP_ARRAY_TEST_BENCH_H_
#define _ESP_ARRAY_TEST_BENCH_H_

#include <Arduino.h>

#include "esp_math.h"
#include "hard_debug.h"
#include "dsps_dotprod.h"
#include "dsps_math.h"

using namespace espmath;

template<typename T>
inline const size_t max_random()
{
  return (size_t)(100);
}

template<>
inline const size_t max_random<float>()
{
  return (size_t)(100);
}

template<>
inline const size_t max_random<int8_t>()
{
  return (size_t)(10);
}

template<>
inline const size_t max_random<int16_t>()
{
  return (size_t)(100);
}

template<>
inline const size_t max_random<int32_t>()
{
  return (size_t)(1000);
}

/**
 * @brief Test array arithmetic operations
 * 
 * @tparam T Array type
 * @tparam _ARRAY_LENGTH_ Length of the array
 * @param _suspend If true, it will suspend the main task on failure.
 */
template<typename T>
inline void test_ari(const size_t _ARRAY_LENGTH_ = 5, bool _suspend = true)
{
  T data1[_ARRAY_LENGTH_];
  T data2[_ARRAY_LENGTH_];
  T output[_ARRAY_LENGTH_];
  shape2D shape = shape2D(1, _ARRAY_LENGTH_);

  const T randomConstant = nonZeroRandomNumber<T>(max_random<T>());
  for(size_t i = 0; i < _ARRAY_LENGTH_; i++)
  {
    data1[i] = nonZeroRandomNumber<T>(max_random<T>());
    data2[i] = nonZeroRandomNumber<T>(max_random<T>());
    output[i] = data1[i] + data2[i];
  }

  Array<T> array1(data1, shape);
  Array<T> array2(data2, shape);
  Array<T> result;
  
  debug.print("Testing arrays addition...");
  result = array1 + array2;
  if(!(result == output))
  {
    debug.print(array1.flatten, _ARRAY_LENGTH_);
    debug.print(array2.flatten, _ARRAY_LENGTH_);
    debug.print(result.flatten, _ARRAY_LENGTH_);
    debug.print(output, _ARRAY_LENGTH_);
    if (_suspend) vTaskSuspend(NULL);
  }
  else
    debug.print("Succeeded!");

  for(size_t i = 0; i < _ARRAY_LENGTH_; i++)
  {
    output[i] = data1[i] - data2[i];
  }

  debug.print("Testing arrays subtration...");
  result = array1 - array2;
  if(!(result == output))
  {
    debug.print(array1.flatten, _ARRAY_LENGTH_);
    debug.print(array2.flatten, _ARRAY_LENGTH_);
    debug.print(result.flatten, _ARRAY_LENGTH_);
    debug.print(output, _ARRAY_LENGTH_);
    if (_suspend) vTaskSuspend(NULL);
  }
  else
    debug.print("Succeeded!");

  for(size_t i = 0; i < _ARRAY_LENGTH_; i++)
  {
    output[i] = data1[i] * data2[i];
  }

  debug.print("Testing arrays multiplication...");
  result = array1 * array2;
  if(!(result == output))
  {
    debug.print(array1.flatten, _ARRAY_LENGTH_);
    debug.print(array2.flatten, _ARRAY_LENGTH_);
    debug.print(result.flatten, _ARRAY_LENGTH_);
    debug.print(output, _ARRAY_LENGTH_);
    if (_suspend) vTaskSuspend(NULL);
  }
  else
    debug.print("Succeeded!");

  for(size_t i = 0; i < _ARRAY_LENGTH_; i++)
  {
    output[i] = data1[i] / data2[i];
  }

  debug.print("Testing arrays division...");
  result = array1 / array2;
  if(!(result == output))
  {
    debug.print(array1.flatten, _ARRAY_LENGTH_);
    debug.print(array2.flatten, _ARRAY_LENGTH_);
    debug.print(result.flatten, _ARRAY_LENGTH_);
    debug.print(output, _ARRAY_LENGTH_);
    if (_suspend) vTaskSuspend(NULL);
  }
  else
    debug.print("Succeeded!");

  for(size_t i = 0; i < _ARRAY_LENGTH_; i++)
  {
    output[i] = data1[i] + randomConstant;
  }

  debug.print("Testing array + constant...");
  result = array1 + randomConstant;
  if(!(result == output))
  {
    debug.print(array1.flatten, _ARRAY_LENGTH_);
    debug.print(randomConstant);
    debug.print(result.flatten, _ARRAY_LENGTH_);
    debug.print(output, _ARRAY_LENGTH_);
    if (_suspend) vTaskSuspend(NULL);
  }
  else
    debug.print("Succeeded!");

  debug.print("Testing constant + array...");
  result = randomConstant + array1;
  if(!(result == output))
  {
    debug.print(array1.flatten, _ARRAY_LENGTH_);
    debug.print(randomConstant);
    debug.print(result.flatten, _ARRAY_LENGTH_);
    debug.print(output, _ARRAY_LENGTH_);
    if (_suspend) vTaskSuspend(NULL);
  }
  else
    debug.print("Succeeded!");

  for(size_t i = 0; i < _ARRAY_LENGTH_; i++)
  {
    output[i] = data1[i] - randomConstant;
  }

  debug.print("Testing array - constant...");
  result = array1 - randomConstant;
  if(!(result == output))
  {
    debug.print(array1.flatten, _ARRAY_LENGTH_);
    debug.print(randomConstant);
    debug.print(result.flatten, _ARRAY_LENGTH_);
    debug.print(output, _ARRAY_LENGTH_);
    if (_suspend) vTaskSuspend(NULL);
  }
  else
    debug.print("Succeeded!");

  for(size_t i = 0; i < _ARRAY_LENGTH_; i++)
  {
    output[i] = randomConstant - data1[i];
  }

  debug.print("Testing constant - array...");
  result = randomConstant - array1;
  if(!(result == output))
  {
    debug.print(array1.flatten, _ARRAY_LENGTH_);
    debug.print(randomConstant);
    debug.print(result.flatten, _ARRAY_LENGTH_);
    debug.print(output, _ARRAY_LENGTH_);
    if (_suspend) vTaskSuspend(NULL);
  }
  else
    debug.print("Succeeded!");

  for(size_t i = 0; i < _ARRAY_LENGTH_; i++)
  {
    output[i] = data1[i] * randomConstant;
  }

  debug.print("Testing array * constant...");
  result = array1 * randomConstant;
  if(!(result == output))
  {
    debug.print(array1.flatten, _ARRAY_LENGTH_);
    debug.print(randomConstant);
    debug.print(result.flatten, _ARRAY_LENGTH_);
    debug.print(output, _ARRAY_LENGTH_);
    if (_suspend) vTaskSuspend(NULL);
  }
  else
    debug.print("Succeeded!");

  debug.print("Testing constant * array...");
  result = randomConstant * array1;
  if(!(result == output))
  {
    debug.print(array1.flatten, _ARRAY_LENGTH_);
    debug.print(randomConstant);
    debug.print(result.flatten, _ARRAY_LENGTH_);
    debug.print(output, _ARRAY_LENGTH_);
    if (_suspend) vTaskSuspend(NULL);
  }
  else
    debug.print("Succeeded!");

  for(size_t i = 0; i < _ARRAY_LENGTH_; i++)
  {
    output[i] = data1[i] / randomConstant;
  }
  
  debug.print("Testing array / constant...");
  result = array1 / randomConstant;
  if(!(result == output))
  {
    debug.print(array1.flatten, _ARRAY_LENGTH_);
    debug.print(randomConstant);
    debug.print(result.flatten, _ARRAY_LENGTH_);
    debug.print(output, _ARRAY_LENGTH_);
    if (_suspend) vTaskSuspend(NULL);
  }
  else
    debug.print("Succeeded!");

  for(size_t i = 0; i < _ARRAY_LENGTH_; i++)
  {
    output[i] = randomConstant / data1[i];
  }

  debug.print("Testing constant / array...");
  result = randomConstant / array1;
  if(!(result == output))
  {
    debug.print(array1.flatten, _ARRAY_LENGTH_);
    debug.print(randomConstant);
    debug.print(result.flatten, _ARRAY_LENGTH_);
    debug.print(output, _ARRAY_LENGTH_);
    if (_suspend) vTaskSuspend(NULL);
  }
  else
    debug.print("Succeeded!");
}

inline int16_t addFixedPoint(int16_t x1, int16_t x2, uint8_t frac)
{
  int32_t acc = (x1 + x2);
  int32_t result = acc >> frac;
  return (int16_t)result;
}

inline int16_t subFixedPoint(int16_t x1, int16_t x2, uint8_t frac)
{
  int32_t acc = (x1 - x2);
  int32_t result = acc >> frac;
  return (int16_t)result;
}

inline int16_t mulFixedPoint(int16_t x1, int16_t x2, uint8_t frac)
{
  int32_t acc = (x1 * x2);
  int32_t result = acc >> frac;
  return (int16_t)result;
}

inline int16_t s16dotProduct(int16_t* x1, int16_t* x2, int len, uint8_t frac)
{
  int16_t helper[(const int) len];
  int32_t result = 0;
  dsps_mul_s16_esp(x1, x2, helper, len, 1, 1, 1, frac);
  for (int i = 0; i < len; i++)
  {
    result += helper[i];
  }
  return (int16_t)result;
}

inline void test_fixed_point(const size_t _ARRAY_LENGTH_ = 5, const uint8_t FRAC = 0, bool _suspend = true)
{
  int16_t data1[_ARRAY_LENGTH_];
  int16_t data2[_ARRAY_LENGTH_];
  float data1F[_ARRAY_LENGTH_];
  float data2F[_ARRAY_LENGTH_];
  int16_t output[_ARRAY_LENGTH_];

  fpVector f1(data1, FRAC), f2(data2, FRAC);
  shape2D shape = shape2D(1, _ARRAY_LENGTH_);

  const FixedPoint randomConstant = FixedPoint(nonZeroRandomNumber<float>(3), FRAC);
  for(size_t i = 0; i < _ARRAY_LENGTH_; i++)
  {
    data1F[i] = nonZeroRandomNumber<float>(3);
    data2F[i] = nonZeroRandomNumber<float>(3);
    data1[i] = float2fixed(data1F[i], FRAC);
    data2[i] = float2fixed(data2F[i], FRAC);
  }

  Array<int16_t> array1(&f1, shape);
  Array<int16_t> array2(&f2, shape);
  Array<int16_t> result;

  debug.print(array1.flatten, _ARRAY_LENGTH_, FRAC);
  debug.print(array2.flatten, _ARRAY_LENGTH_, FRAC);
  debug.print("Testing arrays addition...");
  dsps_add_s16_esp(array1.flatten, array2.flatten, output, _ARRAY_LENGTH_, 1, 1, 1, 0);
  result = array1 + array2;
  debug.print(result.flatten, _ARRAY_LENGTH_, FRAC);
  if(!(result == output))
  {
    debug.print(array1.flatten, _ARRAY_LENGTH_, FRAC);
    debug.print(array2.flatten, _ARRAY_LENGTH_, FRAC);
    debug.print(result.flatten, _ARRAY_LENGTH_, FRAC);
    debug.print(output, _ARRAY_LENGTH_, FRAC);
    if (_suspend) vTaskSuspend(NULL);
  }
  else
    debug.print("Succeeded!");

  debug.print("Testing arrays subtration...");
  for (size_t i = 0; i < _ARRAY_LENGTH_; i++)
  {
    output[i] = subFixedPoint(array1.flatten[i], array2.flatten[i], 0);
  }
  result = array1 - array2;
  debug.print(result.flatten, _ARRAY_LENGTH_, FRAC);
  if(!(result == output))
  {
    debug.print(array1.flatten, _ARRAY_LENGTH_, FRAC);
    debug.print(array2.flatten, _ARRAY_LENGTH_, FRAC);
    debug.print(result.flatten, _ARRAY_LENGTH_, FRAC);
    debug.print(output, _ARRAY_LENGTH_, FRAC);
    if (_suspend) vTaskSuspend(NULL);
  }
  else
    debug.print("Succeeded!");

  debug.print("Testing arrays multiplication...");
  dsps_mul_s16_ansi(array1.flatten, array2.flatten, output, _ARRAY_LENGTH_, 1, 1, 1, FRAC);
  result = array1 * array2;
  debug.print(result.flatten, _ARRAY_LENGTH_, FRAC);
  if(!(result == output))
  {
    debug.print(array1.flatten, _ARRAY_LENGTH_, FRAC);
    debug.print(array2.flatten, _ARRAY_LENGTH_, FRAC);
    debug.print(result.flatten, _ARRAY_LENGTH_, FRAC);
    debug.print(output, _ARRAY_LENGTH_, FRAC);
    if (_suspend) vTaskSuspend(NULL);
  }
  else
    debug.print("Succeeded!");

  for(size_t i = 0; i < _ARRAY_LENGTH_; i++)
  {
    output[i] = addFixedPoint(data1[i], randomConstant.data, 0);
  }

  debug.print("Testing array + constant...");
  result = array1 + randomConstant.data;
  debug.print(result.flatten, _ARRAY_LENGTH_, FRAC);
  if(!(result == output))
  {
    debug.print(array1.flatten, _ARRAY_LENGTH_, FRAC);
    debug.print(randomConstant.data, FRAC);
    debug.print(result.flatten, _ARRAY_LENGTH_, FRAC);
    debug.print(output, _ARRAY_LENGTH_, FRAC);
    if (_suspend) vTaskSuspend(NULL);
  }
  else
    debug.print("Succeeded!");

  for(size_t i = 0; i < _ARRAY_LENGTH_; i++)
  {
    output[i] =  mulFixedPoint(data1[i], randomConstant.data, FRAC);
  }

  debug.print("Testing array * constant...");
  result = array1 * randomConstant.data;
  debug.print(result.flatten, _ARRAY_LENGTH_, FRAC);
  if(!(result == output))
  {
    debug.print(array1.flatten, _ARRAY_LENGTH_, FRAC);
    debug.print(randomConstant.data, FRAC);
    debug.print(result.flatten, _ARRAY_LENGTH_, FRAC);
    debug.print(output, _ARRAY_LENGTH_, FRAC);
    if (_suspend) vTaskSuspend(NULL);
  }
  else
    debug.print("Succeeded!");

  debug.print("Testing dot product...");
  output[0] = s16dotProduct(data1, data2, _ARRAY_LENGTH_, FRAC);
  result.flatten[0] = array1 ^ array2;
  debug.print(String("DotProduct Result: ") + String(fixed2float(result.flatten[0], FRAC)));
  if(!(result.flatten[0] == output[0]))
  {
    debug.print(array1.flatten, _ARRAY_LENGTH_, FRAC);
    debug.print(array2.flatten, _ARRAY_LENGTH_, FRAC);
    debug.print(String("Result: ") + String(fixed2float(result.flatten[0], FRAC)));
    debug.print(String("Correct: ") + String(fixed2float(output[0], FRAC)));
    if (_suspend) vTaskSuspend(NULL);
  }
  else
    debug.print("Succeeded!");
}

#endif