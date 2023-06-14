#ifndef _ESP_ARRAY_TEST_BENCH_H_
#define _ESP_ARRAY_TEST_BENCH_H_

#include <Arduino.h>
#include "esp_math.h"
#include "hard_debug.h"

using namespace espmath;

/**
 * @brief Generate a non-zero random number
 * 
 * @tparam T Output type
 * @param _MAX_NUM_ Maximum random number
 * @return const T Result
 */
template<typename T>
inline const T nonZeroRandomNumber(size_t _MAX_NUM_)
{
  T rn;
  do
  {
    rn = (T)(esp_random() % _MAX_NUM_);
  }while(rn==0);

  return rn;
}

template<>
inline const float nonZeroRandomNumber(size_t _MAX_NUM_)
{
  float rn;
  do
  {
    rn = (float)(esp_random() % _MAX_NUM_);
    rn += (float)(1 / esp_random());
  }while(rn==0);

  return rn;
}

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
inline const size_t max_random<uint8_t>()
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

  size_t x1, x2;

  const T randomConstant = nonZeroRandomNumber<T>(max_random<T>());
  for(size_t i = 0; i < _ARRAY_LENGTH_; i++)
  {
    data1[i] = nonZeroRandomNumber<T>(max_random<T>());
    data2[i] = nonZeroRandomNumber<T>(max_random<T>());
    output[i] = data1[i] + data2[i];
  }

  Array<T> array1 = Array<T>(data1, _ARRAY_LENGTH_);
  Array<T> array2 = Array<T>(data2, _ARRAY_LENGTH_);
  Array<T> result;
  Array<float> floatResult;
  
  debug.print("Testing arrays addition...");
  x1 = xthal_get_ccount();
  result = array1 + array2;
  x2 = xthal_get_ccount();
  debug.print("It took " + String(x2-x1) + " cycles!");
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
  x1 = xthal_get_ccount();
  result = array1 - array2;
  x2 = xthal_get_ccount();
  debug.print("It took " + String(x2-x1) + " cycles!");
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
  x1 = xthal_get_ccount();
  result = array1 * array2;
  x2 = xthal_get_ccount();
  debug.print("It took " + String(x2-x1) + " cycles!");
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
  x1 = xthal_get_ccount();
  floatResult = (array1 / array2);
  x2 = xthal_get_ccount();
  debug.print("It took " + String(x2-x1) + " cycles!");
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
  x1 = xthal_get_ccount();
  result = array1 + randomConstant;
  x2 = xthal_get_ccount();
  debug.print("It took " + String(x2-x1) + " cycles!");
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
  x1 = xthal_get_ccount();
  result = randomConstant + array1;
  x2 = xthal_get_ccount();
  debug.print("It took " + String(x2-x1) + " cycles!");
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
  x1 = xthal_get_ccount();
  result = array1 - randomConstant;
  x2 = xthal_get_ccount();
  debug.print("It took " + String(x2-x1) + " cycles!");
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
  x1 = xthal_get_ccount();
  result = randomConstant - array1;
  x2 = xthal_get_ccount();
  debug.print("It took " + String(x2-x1) + " cycles!");
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
  x1 = xthal_get_ccount();
  result = array1 * randomConstant;
  x2 = xthal_get_ccount();
  debug.print("It took " + String(x2-x1) + " cycles!");
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
  x1 = xthal_get_ccount();
  result = randomConstant * array1;
  x2 = xthal_get_ccount();
  debug.print("It took " + String(x2-x1) + " cycles!");
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
  x1 = xthal_get_ccount();
  floatResult = array1 / randomConstant;
  x2 = xthal_get_ccount();
  debug.print("It took " + String(x2-x1) + " cycles!");
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
  x1 = xthal_get_ccount();
  floatResult = randomConstant / array1;
  x2 = xthal_get_ccount();
  debug.print("It took " + String(x2-x1) + " cycles!");
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