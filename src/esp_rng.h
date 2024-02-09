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
inline T nonZeroRandomNumber(size_t _MAX_NUM_)
{
  T rn;

  do
  {
    rn = (T)esp_random() % _MAX_NUM_;
  }while(!rn);

  if (rn > _MAX_NUM_ / 2) rn*=-1;
  return rn;
}

template<>
inline float nonZeroRandomNumber(size_t _MAX_NUM_)
{
  float f_rn = sqrt(esp_random());
  while ((size_t)f_rn > _MAX_NUM_) f_rn /= (float)_MAX_NUM_;
  if (((size_t)f_rn)%2) f_rn*=-1;
  return f_rn;
}

#endif