#ifndef _custom_dsps_vsum_H_
#define _custom_dsps_vsum_H_
#include "dsp_err.h"
#include "../mul/dsps_mul_esp.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief   sum of array's elements
 *
 * 
 * @note Caution. If MEMORY_ALIGN is enabled, only 16 bytes aligned data can be used with it.
 * If you are using espmath::Array, you don't have to worry about it.
 *
 * @param x: input array
 * @param y: output result
 * @param len: amount of operations for arrays
 * @param step_x: step for input x
 *
 * @return
 *      - ESP_OK on success
 *      - One of the error codes from DSP library
 */
esp_err_t dsps_vsum_s16_esp(const int16_t *x, int16_t *y, int len, int step_x = 1);

/**
 * @brief   sum of array's elements
 *
 * 
 * @note Caution. If MEMORY_ALIGN is enabled, only 16 bytes aligned data can be used with it.
 * If you are using espmath::Array, you don't have to worry about it.
 *
 * @param x: input array
 * @param y: output result
 * @param len: amount of operations for arrays
 * @param step_x: step for input x
 *
 * @return
 *      - ESP_OK on success
 *      - One of the error codes from DSP library
 */
esp_err_t dsps_vsum_s8_esp(const int8_t *x, int8_t *y, int len, int step_x = 1);

/**
 * @brief   sum of array's elements
 *
 * 
 * @note Caution. If MEMORY_ALIGN is enabled, only 16 bytes aligned data can be used with it.
 * If you are using espmath::Array, you don't have to worry about it.
 *
 * @param x: input array
 * @param x2: input array
 * @param y: output result
 * @param len: amount of operations for arrays
 * @param step_x: step for input x
 * @param step_x2: step for input x2
 *
 * @return
 *      - ESP_OK on success
 *      - One of the error codes from DSP library
 */
esp_err_t dsps_vsum_s32_esp(const int32_t *x, int32_t *y, int len, int step_x = 1);

/**@}*/

#ifdef __cplusplus
}
#endif

#endif // _dsps_vsum_H_