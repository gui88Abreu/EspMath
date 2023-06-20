#ifndef _custom_dsps_addc_H_
#define _custom_dsps_addc_H_
#include "dsp_err.h"

#include "dsps_addc_platform.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief   add constant
 *
 * The function multiplies input array to the constant value
 * y[i] = x[i]+C; i=[0..len)
 * The implementation target ESP32 devices and it's optmized using DSP instructions.
 * 
 * @note Caution. If MEMORY_ALIGN is enabled, only 16 bytes aligned data can be used with it.
 * If you are using espmath::Array, you don't have to worry about it.
 *
 * @param x: input array
 * @param y: output array
 * @param len: amount of operations for arrays
 *
 * @return
 *      - ESP_OK on success
 *      - One of the error codes from DSP library
 */
esp_err_t dsps_addc_s8_esp(const int8_t *x,\
                          int8_t *y,\
                          int len,\
                          const int8_t *C,\
                          int step_x = 1,\
                          int step_y = 1);

/**
 * @brief   add constant
 *
 * The function multiplies input array to the constant value
 * y[i] = x[i]+C; i=[0..len)
 * The implementation target ESP32 devices and it's optmized using DSP instructions.
 * 
 * @note Caution. If MEMORY_ALIGN is enabled, only 16 bytes aligned data can be used with it.
 * If you are using espmath::Array, you don't have to worry about it.
 *
 * @param x: input array
 * @param y: output array
 * @param len: amount of operations for arrays
 * @param frac: Fractional part. For instance, if Q15, then frac = 15
 *
 * @return
 *      - ESP_OK on success
 *      - One of the error codes from DSP library
 */
esp_err_t dsps_addc_s16_esp(const int16_t *x,\
                          int16_t *y,\
                          int len,\
                          const int16_t *C,\
                          int step_x = 1,\
                          int step_y  = 1,\
                          int frac    = 0);

/**
 * @brief   add constant
 *
 * The function multiplies input array to the constant value
 * y[i] = x[i]+C; i=[0..len)
 * The implementation target ESP32 devices and it's optmized using DSP instructions.
 * 
 * @note Caution. If MEMORY_ALIGN is enabled, only 16 bytes aligned data can be used with it.
 * If you are using espmath::Array, you don't have to worry about it.
 *
 * @param x: input array
 * @param y: output array
 * @param len: amount of operations for arrays
 *
 * @return
 *      - ESP_OK on success
 *      - One of the error codes from DSP library
 */
esp_err_t dsps_addc_s32_esp(const int32_t *x,\
                          int32_t *y,\
                          int len,\
                          const int32_t C,\
                          int step_x = 1,\
                          int step_y = 1);

/**
 * @brief   add constant
 *
 * The function multiplies input array to the constant value
 * y[i] = x[i]+C; i=[0..len)
 * The implementation target ESP32 devices and it's optmized using DSP instructions.
 * 
 * @note Caution. If MEMORY_ALIGN is enabled, only 16 bytes aligned data can be used with it.
 * If you are using espmath::Array, you don't have to worry about it.
 *
 * @param x: input array
 * @param y: output array
 * @param len: amount of operations for arrays
 *
 * @return
 *      - ESP_OK on success
 *      - One of the error codes from DSP library
 */
esp_err_t dsps_addc_f32_esp(const float *x,\
                          float *y,\
                          int len,\
                          const float C,\
                          int step_x = 1,\
                          int step_y = 1);

/**@}*/

#ifdef __cplusplus
}
#endif

#endif // _dsps_addc_H_