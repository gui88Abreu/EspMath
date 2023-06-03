#ifndef _TEST_BENCH_H_
#define _TEST_BENCH_H_

#include <Arduino.h>
#include "esp_array.h"
#include "hard_debug.h"

using namespace espmath;

template<typename T>
const T nonZeroRandomNumber(size_t _MAX_NUM_)
{
  T rn;
  do
  {
    rn = (T)(esp_random() % _MAX_NUM_);
  }while(rn==0);

  return rn;
}

template<typename T, size_t _MAX_SIZE_ = 5>
void test_ari(bool _suspend = true)
{
  T data1[_MAX_SIZE_];
  T data2[_MAX_SIZE_];
  T output[_MAX_SIZE_];
  
  float floatOutput[_MAX_SIZE_];
  size_t max_random;

  switch (getArrayType(output))
  {
    case int8_type: max_random = (size_t)(10); break;
    case int16_type: max_random = (size_t)(200); break;
    case float_type: max_random = (size_t)(200); break;
    default: case int32_type: max_random = (size_t)(1000); break;
  }

  const T randomConstant = nonZeroRandomNumber<T>(max_random);
  for(size_t i = 0; i < _MAX_SIZE_; i++)
  {
    data1[i] = nonZeroRandomNumber<T>(max_random);
    data2[i] = nonZeroRandomNumber<T>(max_random);
    output[i] = data1[i] + data2[i];
  }

  Array<T> array1 = Array<T>(data1, _MAX_SIZE_);
  Array<T> array2 = Array<T>(data2, _MAX_SIZE_);
  Array<T> result;
  Array<float> floatResult;
  
  debug.print("Testing arrays addition...");
  result = array1 + array2;
  if(!(result == output))
  {
    debug.print(array1.getArrayPntr(), _MAX_SIZE_);
    debug.print(array2.getArrayPntr(), _MAX_SIZE_);
    debug.print(result.getArrayPntr(), _MAX_SIZE_);
    debug.print(output, _MAX_SIZE_);
    if (_suspend) vTaskSuspend(NULL);
  }
  else
    debug.print("Succeeded!");

  for(size_t i = 0; i < _MAX_SIZE_; i++)
  {
    output[i] = data1[i] - data2[i];
  }

  debug.print("Testing arrays subtration...");
  result = array1 - array2;
  if(!(result == output))
  {
    debug.print(array1.getArrayPntr(), _MAX_SIZE_);
    debug.print(array2.getArrayPntr(), _MAX_SIZE_);
    debug.print(result.getArrayPntr(), _MAX_SIZE_);
    debug.print(output, _MAX_SIZE_);
    if (_suspend) vTaskSuspend(NULL);
  }
  else
    debug.print("Succeeded!");

  for(size_t i = 0; i < _MAX_SIZE_; i++)
  {
    output[i] = data1[i] * data2[i];
  }

  debug.print("Testing arrays multiplication...");
  result = array1 * array2;
  if(!(result == output))
  {
    debug.print(array1.getArrayPntr(), _MAX_SIZE_);
    debug.print(array2.getArrayPntr(), _MAX_SIZE_);
    debug.print(result.getArrayPntr(), _MAX_SIZE_);
    debug.print(output, _MAX_SIZE_);
    if (_suspend) vTaskSuspend(NULL);
  }
  else
    debug.print("Succeeded!");

  for(size_t i = 0; i < _MAX_SIZE_; i++)
  {
    floatOutput[i] = (float)((float)data1[i] / (float)data2[i]);
  }

  debug.print("Testing arrays division...");
  floatResult = (array1 / array2);
  if(!(floatResult == floatOutput))
  {
    debug.print(array1.getArrayPntr(), _MAX_SIZE_);
    debug.print(array2.getArrayPntr(), _MAX_SIZE_);
    debug.print(floatResult.getArrayPntr(), _MAX_SIZE_);
    debug.print(floatOutput, _MAX_SIZE_);
    if (_suspend) vTaskSuspend(NULL);
  }
  else
    debug.print("Succeeded!");

  for(size_t i = 0; i < _MAX_SIZE_; i++)
  {
    output[i] = data1[i] + randomConstant;
  }

  debug.print("Testing array + constant...");
  result = array1 + randomConstant;
  if(!(result == output))
  {
    debug.print(array1.getArrayPntr(), _MAX_SIZE_);
    debug.print(randomConstant);
    debug.print(result.getArrayPntr(), _MAX_SIZE_);
    debug.print(output, _MAX_SIZE_);
    if (_suspend) vTaskSuspend(NULL);
  }
  else
    debug.print("Succeeded!");

  debug.print("Testing constant + array...");
  result = randomConstant + array1;
  if(!(result == output))
  {
    debug.print(array1.getArrayPntr(), _MAX_SIZE_);
    debug.print(randomConstant);
    debug.print(result.getArrayPntr(), _MAX_SIZE_);
    debug.print(output, _MAX_SIZE_);
    if (_suspend) vTaskSuspend(NULL);
  }
  else
    debug.print("Succeeded!");

  for(size_t i = 0; i < _MAX_SIZE_; i++)
  {
    output[i] = data1[i] - randomConstant;
  }

  debug.print("Testing array - constant...");
  result = array1 - randomConstant;
  if(!(result == output))
  {
    debug.print(array1.getArrayPntr(), _MAX_SIZE_);
    debug.print(randomConstant);
    debug.print(result.getArrayPntr(), _MAX_SIZE_);
    debug.print(output, _MAX_SIZE_);
    if (_suspend) vTaskSuspend(NULL);
  }
  else
    debug.print("Succeeded!");

  for(size_t i = 0; i < _MAX_SIZE_; i++)
  {
    output[i] = randomConstant - data1[i];
  }

  debug.print("Testing constant - array...");
  result = randomConstant - array1;
  if(!(result == output))
  {
    debug.print(array1.getArrayPntr(), _MAX_SIZE_);
    debug.print(randomConstant);
    debug.print(result.getArrayPntr(), _MAX_SIZE_);
    debug.print(output, _MAX_SIZE_);
    if (_suspend) vTaskSuspend(NULL);
  }
  else
    debug.print("Succeeded!");

  for(size_t i = 0; i < _MAX_SIZE_; i++)
  {
    output[i] = data1[i] * randomConstant;
  }

  debug.print("Testing array * constant...");
  result = array1 * randomConstant;
  if(!(result == output))
  {
    debug.print(array1.getArrayPntr(), _MAX_SIZE_);
    debug.print(randomConstant);
    debug.print(result.getArrayPntr(), _MAX_SIZE_);
    debug.print(output, _MAX_SIZE_);
    if (_suspend) vTaskSuspend(NULL);
  }
  else
    debug.print("Succeeded!");

  debug.print("Testing constant * array...");
  result = randomConstant * array1;
  if(!(result == output))
  {
    debug.print(array1.getArrayPntr(), _MAX_SIZE_);
    debug.print(randomConstant);
    debug.print(result.getArrayPntr(), _MAX_SIZE_);
    debug.print(output, _MAX_SIZE_);
    if (_suspend) vTaskSuspend(NULL);
  }
  else
    debug.print("Succeeded!");

  for(size_t i = 0; i < _MAX_SIZE_; i++)
  {
    floatOutput[i] = (float)((float)data1[i] / randomConstant);
  }

  debug.print("Testing array / constant...");
  floatResult = (array1 / randomConstant);
  if(!(floatResult == floatOutput))
  {
    debug.print(array1.getArrayPntr(), _MAX_SIZE_);
    debug.print(randomConstant);
    debug.print(floatResult.getArrayPntr(), _MAX_SIZE_);
    debug.print(floatOutput, _MAX_SIZE_);
    if (_suspend) vTaskSuspend(NULL);
  }
  else
    debug.print("Succeeded!");

  for(size_t i = 0; i < _MAX_SIZE_; i++)
  {
    floatOutput[i] = (float)(randomConstant / (float)data1[i]);
  }

  debug.print("Testing constant / array...");
  floatResult = (randomConstant / array1);
  if(!(floatResult == floatOutput))
  {
    debug.print(array1.getArrayPntr(), _MAX_SIZE_);
    debug.print(randomConstant);
    debug.print(floatResult.getArrayPntr(), _MAX_SIZE_);
    debug.print(floatOutput, _MAX_SIZE_);
    if (_suspend) vTaskSuspend(NULL);
  }
  else
    debug.print("Succeeded!");
}

#endif