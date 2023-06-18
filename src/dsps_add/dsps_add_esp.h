#ifndef _custom_dsps_add_H_
#define _custom_dsps_add_H_
#include "dsp_err.h"

#include "dsps_add_platform.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief   add arrays
 *
 * The function multiplies input array to the constant value
 * y[i] = x1[i]+x2[i]; i=[0..len)
 * The implementation target ESP32 devices and it's optmized using DSP instructions.
 * 
 * @note Caution. If FAST_MODE is enabled, only 16 bytes aligned data can be used with it.
 * If you are using espmath::Array, you don't have to worry about it.
 *
 * @param x1: input array
 * @param x2: input array
 * @param output: output array
 * @param len: amount of operations for arrays
 *
 * @return
 *      - ESP_OK on success
 *      - One of the error codes from DSP library
 */
esp_err_t dsps_add_u8_esp(const uint8_t *x1, const uint8_t *x2, uint8_t *y, int len);

/**
 * @brief   add arrays
 *
 * The function multiplies input array to the constant value
 * y[i] = x1[i]+x2[i]; i=[0..len)
 * The implementation target ESP32 devices and it's optmized using DSP instructions.
 * 
 * @note Caution. If FAST_MODE is enabled, only 16 bytes aligned data can be used with it.
 * If you are using espmath::Array, you don't have to worry about it.
 *
 * @param x1: input array
 * @param x2: input array
 * @param output: output array
 * @param len: amount of operations for arrays
 * @param frac: Fractional part. For instance, if Q15, then frac = 15
 *
 * @return
 *      - ESP_OK on success
 *      - One of the error codes from DSP library
 */
esp_err_t dsps_add_s16_esp(const int16_t *x1, const int16_t *x2, int16_t *y, int len, int frac = 0);

/**
 * @brief   add arrays
 *
 * The function multiplies input array to the constant value
 * y[i] = x1[i]+x2[i]; i=[0..len)
 * The implementation target ESP32 devices and it's optmized using DSP instructions.
 * 
 * @note Caution. If FAST_MODE is enabled, only 16 bytes aligned data can be used with it.
 * If you are using espmath::Array, you don't have to worry about it.
 *
 * @param x1: input array
 * @param x2: input array
 * @param output: output array
 * @param len: amount of operations for arrays
 *
 * @return
 *      - ESP_OK on success
 *      - One of the error codes from DSP library
 */
esp_err_t dsps_add_s32_esp(const int32_t *x1, const int32_t *x2, int32_t *y, int len);

/**
 * @brief   add arrays
 *
 * The function multiplies input array to the constant value
 * y[i] = x1[i]+x2[i]; i=[0..len)
 * The implementation target ESP32 devices and it's optmized using DSP instructions.
 * 
 * @note Caution. If FAST_MODE is enabled, only 16 bytes aligned data can be used with it.
 * If you are using espmath::Array, you don't have to worry about it.
 *
 * @param x1: input array
 * @param x2: input array
 * @param output: output array
 * @param len: amount of operations for arrays
 *
 * @return
 *      - ESP_OK on success
 *      - One of the error codes from DSP library
 */
esp_err_t dsps_add_f32_esp(const float *x1, const float *x2, float *y, int len);

/**@}*/

#ifdef __cplusplus
}
#endif

#endif // _dsps_add_H_