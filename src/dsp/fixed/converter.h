#ifndef _custom_dsps_converter_H_
#define _custom_dsps_converter_H_

#include "dsp_err.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Convert float array to fixed point array
 *
 * 
 * @note Caution. If MEMORY_ALIGN is enabled, only 16 bytes aligned data can be used with it.
 * If you are using espmath::Array, you don't have to worry about it.
 *
 * @param x: input array
 * @param y: output result
 * @param len: amount of operations for arrays
 * @param step_x: step for input x
 * @param step_y: step for input y
 * @param frac: Fractional part. For instance, if Q15, then frac = 15
 *
 * @return
 *      - ESP_OK on success
 *      - One of the error codes from DSP library
 */
inline esp_err_t dsps_f32_s16_esp(const float *x,\
                                  int16_t *y,\
                                  int len,\
                                  int step_x = 1,\
                                  int step_y = 1);

/**
 * @brief Convert fixed point array to float array
 *
 * 
 * @note Caution. If MEMORY_ALIGN is enabled, only 16 bytes aligned data can be used with it.
 * If you are using espmath::Array, you don't have to worry about it.
 *
 * @param x: input array
 * @param y: output result
 * @param len: amount of operations for arrays
 * @param step_x: step for input x
 * @param step_y: step for input y
 * @param frac: Fractional part. For instance, if Q15, then frac = 15
 *
 * @return
 *      - ESP_OK on success
 *      - One of the error codes from DSP library
 */
inline esp_err_t dsps_s16_f32_esp(const int16_t *x,\
                                  float *y,\
                                  int len,\
                                  int step_x = 1,\
                                  int step_y = 1);

/**@}*/

#ifdef __cplusplus
}
#endif

#endif // _dsps_converter_H_