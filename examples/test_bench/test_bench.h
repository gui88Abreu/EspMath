#ifndef _ESP_ARRAY_TEST_BENCH_H_
#define _ESP_ARRAY_TEST_BENCH_H_

#include <Arduino.h>

#include "esp_math.h"
#include "hard_debug.h"

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
void test_ari(const size_t _ARRAY_LENGTH_ = 5, bool _suspend = true)
{
  T data1[_ARRAY_LENGTH_];
  T data2[_ARRAY_LENGTH_];
  T output[_ARRAY_LENGTH_];
  float floatOutput[_ARRAY_LENGTH_];

  const T randomConstant = nonZeroRandomNumber<T>(max_random<T>());
  for(size_t i = 0; i < _ARRAY_LENGTH_; i++)
  {
    data1[i] = nonZeroRandomNumber<T>(max_random<T>());
    data2[i] = nonZeroRandomNumber<T>(max_random<T>());
    output[i] = data1[i] + data2[i];
  }

  Array<T> array1(data1, _ARRAY_LENGTH_);
  Array<T> array2(data2, _ARRAY_LENGTH_);
  Array<T> result;
  Array<float> floatResult;
  
  debug.print("Testing arrays addition...");
  result = array1 + array2;
  if(!(result == output))
  {
    debug.print((T*)array1, _ARRAY_LENGTH_);
    debug.print((T*)array2, _ARRAY_LENGTH_);
    debug.print((T*)result, _ARRAY_LENGTH_);
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
    debug.print((T*)array1, _ARRAY_LENGTH_);
    debug.print((T*)array2, _ARRAY_LENGTH_);
    debug.print((T*)result, _ARRAY_LENGTH_);
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
    debug.print((T*)array1, _ARRAY_LENGTH_);
    debug.print((T*)array2, _ARRAY_LENGTH_);
    debug.print((T*)result, _ARRAY_LENGTH_);
    debug.print(output, _ARRAY_LENGTH_);
    if (_suspend) vTaskSuspend(NULL);
  }
  else
    debug.print("Succeeded!");

  for(size_t i = 0; i < _ARRAY_LENGTH_; i++)
  {
    floatOutput[i] = (float)((float)data1[i] / (float)data2[i]);
  }

  debug.print("Testing arrays division...");
  floatResult = array1 / array2;
  if(!(floatResult == floatOutput))
  {
    debug.print((T*)array1, _ARRAY_LENGTH_);
    debug.print((T*)array2, _ARRAY_LENGTH_);
    debug.print(floatResult.getArrayPntr(), _ARRAY_LENGTH_);
    debug.print(floatOutput, _ARRAY_LENGTH_);
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
    debug.print((T*)array1, _ARRAY_LENGTH_);
    debug.print(randomConstant);
    debug.print((T*)result, _ARRAY_LENGTH_);
    debug.print(output, _ARRAY_LENGTH_);
    if (_suspend) vTaskSuspend(NULL);
  }
  else
    debug.print("Succeeded!");

  debug.print("Testing constant + array...");
  result = randomConstant + array1;
  if(!(result == output))
  {
    debug.print((T*)array1, _ARRAY_LENGTH_);
    debug.print(randomConstant);
    debug.print((T*)result, _ARRAY_LENGTH_);
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
    debug.print((T*)array1, _ARRAY_LENGTH_);
    debug.print(randomConstant);
    debug.print((T*)result, _ARRAY_LENGTH_);
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
    debug.print((T*)array1, _ARRAY_LENGTH_);
    debug.print(randomConstant);
    debug.print((T*)result, _ARRAY_LENGTH_);
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
    debug.print((T*)array1, _ARRAY_LENGTH_);
    debug.print(randomConstant);
    debug.print((T*)result, _ARRAY_LENGTH_);
    debug.print(output, _ARRAY_LENGTH_);
    if (_suspend) vTaskSuspend(NULL);
  }
  else
    debug.print("Succeeded!");

  debug.print("Testing constant * array...");
  result = randomConstant * array1;
  if(!(result == output))
  {
    debug.print((T*)array1, _ARRAY_LENGTH_);
    debug.print(randomConstant);
    debug.print((T*)result, _ARRAY_LENGTH_);
    debug.print(output, _ARRAY_LENGTH_);
    if (_suspend) vTaskSuspend(NULL);
  }
  else
    debug.print("Succeeded!");

  for(size_t i = 0; i < _ARRAY_LENGTH_; i++)
  {
    floatOutput[i] = (float)((float)data1[i] / randomConstant);
  }
  
  debug.print("Testing array / constant...");
  floatResult = array1 / randomConstant;
  if(!(floatResult == floatOutput))
  {
    debug.print((T*)array1, _ARRAY_LENGTH_);
    debug.print(randomConstant);
    debug.print(floatResult.getArrayPntr(), _ARRAY_LENGTH_);
    debug.print(floatOutput, _ARRAY_LENGTH_);
    if (_suspend) vTaskSuspend(NULL);
  }
  else
    debug.print("Succeeded!");

  for(size_t i = 0; i < _ARRAY_LENGTH_; i++)
  {
    floatOutput[i] = (float)(randomConstant / (float)data1[i]);
  }

  debug.print("Testing constant / array...");
  floatResult = randomConstant / array1;
  if(!(floatResult == floatOutput))
  {
    debug.print((T*)array1, _ARRAY_LENGTH_);
    debug.print(randomConstant);
    debug.print(floatResult, _ARRAY_LENGTH_);
    debug.print(floatOutput, _ARRAY_LENGTH_);
    if (_suspend) vTaskSuspend(NULL);
  }
  else
    debug.print("Succeeded!");
}

#endif