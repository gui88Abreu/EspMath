#ifndef _custom_dsps_addc_H_
#define _custom_dsps_addc_H_
#include "dsp_err.h"

#include "dsps_addc_platform.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**@{*/
/**
 * @brief add constant
 *
 * The function multiplies input array to the constant value
 * x[i] = y[i] + C; i=[0..len)
 * The implementation target ESP32 devices and it's optmized using DSP instructions.
 * 
 * @note Caution. If FAST_MODE is enabled, only 16 bytes aligned data can be used with it.
 * If you are using espmath::Array, you don't have to worry about it.
 *
 * @param input: input array
 * @param output: output array
 * @param len: amount of operations for arrays
 * @param C: constant value
 *
 * @return
 *      - ESP_OK on success
 *      - One of the error codes from DSP library
 */
esp_err_t dsps_addc_s32_esp(const int32_t *input, int32_t *output, int len, int32_t C);

/**
 * @brief add constant
 *
 * The function multiplies input array to the constant value
 * x[i] = y[i] + C; i=[0..len)
 * The implementation target ESP32 devices and it's optmized using DSP instructions.
 * Also, it uses fixed-point arithmetic. Use frac parameter to define the fractional 
 * number of the input.
 * 
 * @note Caution. If FAST_MODE is enabled, only 16 bytes aligned data can be used with it.
 * If you are using espmath::Array, you don't have to worry about it.
 *
 * @param input: input array
 * @param output: output array
 * @param len: amount of operations for arrays
 * @param C: constant value
 * @param frac: Fractional number (by default 0)
 *
 * @return
 *      - ESP_OK on success
 *      - One of the error codes from DSP library
 */
esp_err_t dsps_addc_s16_esp(const int16_t *input, int16_t *output, int len, const int16_t* C, int frac = 0);

/**
 * @brief add constant
 *
 * The function multiplies input array to the constant value
 * x[i] = y[i] + C; i=[0..len)
 * The implementation target ESP32 devices and it's optmized using DSP instructions.
 * 
 * @note Caution. If FAST_MODE is enabled, only 16 bytes aligned data can be used with it.
 * If you are using espmath::Array, you don't have to worry about it.
 *
 * @param input: input array
 * @param output: output array
 * @param len: amount of operations for arrays
 * @param C: constant value
 *
 * @return
 *      - ESP_OK on success
 *      - One of the error codes from DSP library
 */
esp_err_t dsps_addc_s8_esp(const int8_t *input, int8_t *output, int len, const int8_t* C);

/**
 * @brief add constant
 *
 * The function multiplies input array to the constant value
 * x[i] = y[i] + C; i=[0..len)
 * The implementation target ESP32 devices and it's optmized using DSP instructions.
 * 
 * @note Caution. If FAST_MODE is enabled, only 16 bytes aligned data can be used with it.
 * If you are using espmath::Array, you don't have to worry about it.
 *
 * @param input: input array
 * @param output: output array
 * @param len: amount of operations for arrays
 * @param C: constant value
 *
 * @return
 *      - ESP_OK on success
 *      - One of the error codes from DSP library
 */
esp_err_t dsps_addc_f32_esp(const float *input, float *output, int len, const float C);

/**@}*/

#ifdef __cplusplus
}
#endif

#endif // _dsps_addc_H_