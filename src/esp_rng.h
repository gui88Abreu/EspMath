#ifndef _ESP_RANDOM_H_
#define _ESP_RANDOM_H_

#include <Arduino.h>

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
  rn = (T)(esp_random() % _MAX_NUM_) + 1;
  return rn;
}

template<>
inline const float nonZeroRandomNumber(size_t _MAX_NUM_)
{
  float rn;
  rn = (float)(esp_random() % _MAX_NUM_) + 0.1;
  rn += ((float)1.0 / (float)(esp_random() + 1.0));
  return rn;
}

#endif