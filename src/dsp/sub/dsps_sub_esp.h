#ifndef _custom_dsps_sub_H_
#define _custom_dsps_sub_H_
#include "dsp_err.h"

#include "dsps_sub_platform.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief   subtract arrays
 *
 * The function multiplies input array to the constant value
 * y[i] = x1[i] - x2[i]; i=[0..len)
 * The implementation target ESP32 devices and it's optmized using DSP instructions.
 * 
 * @note Caution. If MEMORY_ALIGN is enabled, only 16 bytes aligned data can be used with it.
 * If you are using espmath::Array, you don't have to worry about it.
 *
 * @param x1: input array
 * @param x2: input array
 * @param y: output array
 * @param len: amount of operations for arrays
 *
 * @return
 *      - ESP_OK on success
 *      - One of the error codes from DSP library
 */
esp_err_t dsps_sub_s8_esp(const int8_t *x1,\
                          const int8_t *x2,\
                          int8_t *y,\
                          int len,\
                          int step_x1 = 1,\
                          int step_x2 = 1,\
                          int step_y  = 1);

/**
 * @brief   subtract arrays
 *
 * The function multiplies input array to the constant value
 * y[i] = x1[i] - x2[i]; i=[0..len)
 * The implementation target ESP32 devices and it's optmized using DSP instructions.
 * 
 * @note Caution. If MEMORY_ALIGN is enabled, only 16 bytes aligned data can be used with it.
 * If you are using espmath::Array, you don't have to worry about it.
 *
 * @param x1: input array
 * @param x2: input array
 * @param y: output array
 * @param len: amount of operations for arrays
 * @param frac: Fractional part. For instance, if Q15, then frac = 15
 *
 * @return
 *      - ESP_OK on success
 *      - One of the error codes from DSP library
 */
esp_err_t dsps_sub_s16_esp(const int16_t *x1,\
                          const int16_t *x2,\
                          int16_t *y,\
                          int len,\
                          int step_x1 = 1,\
                          int step_x2 = 1,\
                          int step_y  = 1,\
                          int frac = 0);

/**
 * @brief   subtract arrays
 *
 * The function multiplies input array to the constant value
 * y[i] = x1[i] - x2[i]; i=[0..len)
 * The implementation target ESP32 devices and it's optmized using DSP instructions.
 * 
 * @note Caution. If MEMORY_ALIGN is enabled, only 16 bytes aligned data can be used with it.
 * If you are using espmath::Array, you don't have to worry about it.
 *
 * @param x1: input array
 * @param x2: input array
 * @param y: output array
 * @param len: amount of operations for arrays
 *
 * @return
 *      - ESP_OK on success
 *      - One of the error codes from DSP library
 */
esp_err_t dsps_sub_s32_esp(const int32_t *x1,\
                          const int32_t *x2,\
                          int32_t *y,\
                          int len,\
                          int step_x1 = 1,\
                          int step_x2 = 1,\
                          int step_y  = 1);

/**
 * @brief   subtract arrays
 *
 * The function multiplies input array to the constant value
 * y[i] = x1[i] - x2[i]; i=[0..len)
 * The implementation target ESP32 devices and it's optmized using DSP instructions.
 * 
 * @note Caution. If MEMORY_ALIGN is enabled, only 16 bytes aligned data can be used with it.
 * If you are using espmath::Array, you don't have to worry about it.
 *
 * @param x1: input array
 * @param x2: input array
 * @param y: output array
 * @param len: amount of operations for arrays
 *
 * @return
 *      - ESP_OK on success
 *      - One of the error codes from DSP library
 */
esp_err_t dsps_sub_f32_esp(const float *x1,\
                          const float *x2,\
                          float *y,\
                          int len,\
                          int step_x1 = 1,\
                          int step_x2 = 1,\
                          int step_y  = 1);

/**@}*/

#ifdef __cplusplus
}
#endif

#endif // _dsps_sub_H_