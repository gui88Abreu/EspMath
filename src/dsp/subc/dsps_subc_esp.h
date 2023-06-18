#ifndef _custom_dsps_subc_H_
#define _custom_dsps_subc_H_
#include "dsp_err.h"

#include "../addc/dsps_addc_esp.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**@{*/
/**
 * @brief subtract constant
 *
 * The function multiplies input array to the constant value
 * x[i] = y[i] - C; i=[0..len)
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
inline esp_err_t dsps_subc_s32_esp(const int32_t *input, int32_t *output, int len, int32_t C)
{
  return dsps_addc_s32_esp(input, output, len, C*(-1));
}

/**
 * @brief subtract constant
 *
 * The function multiplies input array to the constant value
 * x[i] = y[i] - C; i=[0..len)
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
inline esp_err_t dsps_subc_s16_esp(const int16_t *input, int16_t *output, int len, const int16_t* C, int frac = 0)
{
  const int16_t constant = (*C)*(-1);
  return dsps_addc_s16_esp(input, output, len, &constant, frac);
}

/**
 * @brief subtract constant
 *
 * The function multiplies input array to the constant value
 * x[i] = y[i] - C; i=[0..len)
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
inline esp_err_t dsps_subc_s8_esp(const int8_t *input, int8_t *output, int len, const int8_t* C)
{
  const int8_t constant = (*C)*(-1);
  return dsps_addc_s8_esp(input, output, len, &constant);
}

/**
 * @brief subtract constant
 *
 * The function multiplies input array to the constant value
 * x[i] = y[i] - C; i=[0..len)
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
inline esp_err_t dsps_subc_f32_esp(const float *input, float *output, int len, const float C)
{
  return dsps_addc_f32_esp(input, output, len, C*(-1));
}

/**
 * @brief subtract constant
 *
 * The function multiplies input array to the constant value
 * x[i] = c - Y[I] ; i=[0..len)
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
inline esp_err_t dsps_csub_s32_esp(const int32_t *input, int32_t *output, int len, int32_t C);

/**
 * @brief subtract constant
 *
 * The function multiplies input array to the constant value
 * x[i] = c - Y[I] ; i=[0..len)
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
inline esp_err_t dsps_csub_s16_esp(const int16_t *input, int16_t *output, int len, const int16_t* C, int frac = 0);

/**
 * @brief subtract constant
 *
 * The function multiplies input array to the constant value
 * x[i] = c - Y[I] ; i=[0..len)
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
inline esp_err_t dsps_csub_s8_esp(const int8_t *input, int8_t *output, int len, const int8_t* C);

/**
 * @brief subtract constant
 *
 * The function multiplies input array to the constant value
 * x[i] = c - Y[I] ; i=[0..len)
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
inline esp_err_t dsps_csub_f32_esp(const float *input, float *output, int len, const float C);

/**@}*/

#ifdef __cplusplus
}
#endif

#endif // _dsps_subc_H_