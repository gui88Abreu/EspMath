#ifndef _custom_dsps_dotp_H_
#define _custom_dsps_dotp_H_
#include "dsp_err.h"
#include "../mul/dsps_mul_esp.h"
#include "../sum/dsps_vsum_esp.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Dot Produt between arrays
 *
 * 
 * @note Caution. If MEMORY_ALIGN is enabled, only 16 bytes aligned data can be used with it.
 * If you are using espmath::Array, you don't have to worry about it.
 *
 * @param x1: input array
 * @param x2: input array
 * @param y: output result
 * @param len: amount of operations for arrays
 * @param step_x1: step for input x1
 * @param step_x2: step for input x2
 * @param frac: Fractional part. For instance, if Q15, then frac = 15
 *
 * @return
 *      - ESP_OK on success
 *      - One of the error codes from DSP library
 */
esp_err_t dsps_dotp_s16_esp(const int16_t *x1,\
                            const int16_t *x2,\
                            int16_t *y,\
                            int len,\
                            int step_x1 = 1,\
                            int step_x2 = 1,\
                            int frac = 0);

/**
 * @brief   Dot Produt between arrays
 *
 * 
 * @note Caution. If MEMORY_ALIGN is enabled, only 16 bytes aligned data can be used with it.
 * If you are using espmath::Array, you don't have to worry about it.
 *
 * @param x1: input array
 * @param x2: input array
 * @param y: output result
 * @param len: amount of operations for arrays
 * @param step_x1: step for input x1
 * @param step_x2: step for input x2
 *
 * @return
 *      - ESP_OK on success
 *      - One of the error codes from DSP library
 */
inline esp_err_t dsps_dotp_s8_esp(const int8_t *x1,\
                                  const int8_t *x2,\
                                  int8_t *y,\
                                  int len,\
                                  int step_x1 = 1,\
                                  int step_x2 = 1)
{
  int8_t helper[(const int)len];
  int32_t result = 0;
  dsps_mul_s8_esp(x1, x2, helper, len, step_x1, step_x2, 1);
  dsps_vsum_s8_esp(helper, y, len, 1);
  return ESP_OK;
}

/**
 * @brief   Dot Produt between arrays
 *
 * 
 * @note Caution. If MEMORY_ALIGN is enabled, only 16 bytes aligned data can be used with it.
 * If you are using espmath::Array, you don't have to worry about it.
 *
 * @param x1: input array
 * @param x2: input array
 * @param y: output result
 * @param len: amount of operations for arrays
 * @param step_x1: step for input x1
 * @param step_x2: step for input x2
 *
 * @return
 *      - ESP_OK on success
 *      - One of the error codes from DSP library
 */
inline esp_err_t dsps_dotp_s32_esp(const int32_t *x1,\
                                  const int32_t *x2,\
                                  int32_t *y,\
                                  int len,\
                                  int step_x1 = 1,\
                                  int step_x2 = 1,\
                                  int frac = 0)
{
  int32_t helper[(const int)len];
  int64_t result = 0;
  dsps_mul_s32_esp(x1, x2, helper, len, step_x1, step_x2, 1);
  dsps_vsum_s32_esp(helper, y, len, 1);
  return ESP_OK;
}

/**
 * @brief   Dot Produt between arrays
 *
 * 
 * @note Caution. If MEMORY_ALIGN is enabled, only 16 bytes aligned data can be used with it.
 * If you are using espmath::Array, you don't have to worry about it.
 *
 * @param x1: input array
 * @param x2: input array
 * @param y: output result
 * @param len: amount of operations for arrays
 * @param step_x1: step for input x1
 * @param step_x2: step for input x2
 *
 * @return
 *      - ESP_OK on success
 *      - One of the error codes from DSP library
 */
esp_err_t dsps_dotp_f32_esp(const float *x1,\
                                  const float *x2,\
                                  float *y,\
                                  int len,\
                                  int step_x1 = 1,\
                                  int step_x2 = 1);

/**@}*/

#ifdef __cplusplus
}
#endif

#endif // _dsps_dotp_H_