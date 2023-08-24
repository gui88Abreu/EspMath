#ifndef _custom_dsps_converter_H_
#define _custom_dsps_converter_H_

#include "dsp_err.h"

#ifdef __cplusplus
extern "C"
{
#endif

esp_err_t dsps_f32_s1615_esp(const float *x, int16_t *y, int len, int step_x = 1, int step_y = 1);
esp_err_t dsps_f32_s1614_esp(const float *x, int16_t *y, int len, int step_x = 1, int step_y = 1);
esp_err_t dsps_f32_s1613_esp(const float *x, int16_t *y, int len, int step_x = 1, int step_y = 1);
esp_err_t dsps_f32_s1612_esp(const float *x, int16_t *y, int len, int step_x = 1, int step_y = 1);
esp_err_t dsps_f32_s1611_esp(const float *x, int16_t *y, int len, int step_x = 1, int step_y = 1);
esp_err_t dsps_f32_s1610_esp(const float *x, int16_t *y, int len, int step_x = 1, int step_y = 1);
esp_err_t dsps_f32_s169_esp(const float *x, int16_t *y, int len, int step_x = 1, int step_y = 1);
esp_err_t dsps_f32_s168_esp(const float *x, int16_t *y, int len, int step_x = 1, int step_y = 1);
esp_err_t dsps_f32_s167_esp(const float *x, int16_t *y, int len, int step_x = 1, int step_y = 1);
esp_err_t dsps_f32_s166_esp(const float *x, int16_t *y, int len, int step_x = 1, int step_y = 1);
esp_err_t dsps_f32_s165_esp(const float *x, int16_t *y, int len, int step_x = 1, int step_y = 1);
esp_err_t dsps_f32_s164_esp(const float *x, int16_t *y, int len, int step_x = 1, int step_y = 1);
esp_err_t dsps_f32_s163_esp(const float *x, int16_t *y, int len, int step_x = 1, int step_y = 1);
esp_err_t dsps_f32_s162_esp(const float *x, int16_t *y, int len, int step_x = 1, int step_y = 1);
esp_err_t dsps_f32_s161_esp(const float *x, int16_t *y, int len, int step_x = 1, int step_y = 1);


esp_err_t dsps_s1615_f32_esp(const int16_t *x, float *y, int len, int step_x = 1, int step_y = 1);
esp_err_t dsps_s1614_f32_esp(const int16_t *x, float *y, int len, int step_x = 1, int step_y = 1);
esp_err_t dsps_s1613_f32_esp(const int16_t *x, float *y, int len, int step_x = 1, int step_y = 1);
esp_err_t dsps_s1612_f32_esp(const int16_t *x, float *y, int len, int step_x = 1, int step_y = 1);
esp_err_t dsps_s1611_f32_esp(const int16_t *x, float *y, int len, int step_x = 1, int step_y = 1);
esp_err_t dsps_s1610_f32_esp(const int16_t *x, float *y, int len, int step_x = 1, int step_y = 1);
esp_err_t dsps_s169_f32_esp(const int16_t *x, float *y, int len, int step_x = 1, int step_y = 1);
esp_err_t dsps_s168_f32_esp(const int16_t *x, float *y, int len, int step_x = 1, int step_y = 1);
esp_err_t dsps_s167_f32_esp(const int16_t *x, float *y, int len, int step_x = 1, int step_y = 1);
esp_err_t dsps_s166_f32_esp(const int16_t *x, float *y, int len, int step_x = 1, int step_y = 1);
esp_err_t dsps_s165_f32_esp(const int16_t *x, float *y, int len, int step_x = 1, int step_y = 1);
esp_err_t dsps_s164_f32_esp(const int16_t *x, float *y, int len, int step_x = 1, int step_y = 1);
esp_err_t dsps_s163_f32_esp(const int16_t *x, float *y, int len, int step_x = 1, int step_y = 1);
esp_err_t dsps_s162_f32_esp(const int16_t *x, float *y, int len, int step_x = 1, int step_y = 1);
esp_err_t dsps_s161_f32_esp(const int16_t *x, float *y, int len, int step_x = 1, int step_y = 1);

/**@}*/

#ifdef __cplusplus
}
#endif

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
                          int frac = 15,\
                          int len = 1,\
                          int step_x = 1,\
                          int step_y = 1)
{
  switch (frac)
  {
    case 1: return dsps_s161_f32_esp(x, y, len, step_x, step_y);
    case 2: return dsps_s162_f32_esp(x, y, len, step_x, step_y);
    case 3: return dsps_s163_f32_esp(x, y, len, step_x, step_y);
    case 4: return dsps_s164_f32_esp(x, y, len, step_x, step_y);
    case 5: return dsps_s165_f32_esp(x, y, len, step_x, step_y);
    case 6: return dsps_s166_f32_esp(x, y, len, step_x, step_y);
    case 7: return dsps_s167_f32_esp(x, y, len, step_x, step_y);
    case 8: return dsps_s168_f32_esp(x, y, len, step_x, step_y);
    case 9: return dsps_s169_f32_esp(x, y, len, step_x, step_y);
    case 10: return dsps_s1610_f32_esp(x, y, len, step_x, step_y);
    case 11: return dsps_s1611_f32_esp(x, y, len, step_x, step_y);
    case 12: return dsps_s1612_f32_esp(x, y, len, step_x, step_y);
    case 13: return dsps_s1613_f32_esp(x, y, len, step_x, step_y);
    case 14: return dsps_s1614_f32_esp(x, y, len, step_x, step_y);
    case 15: return dsps_s1615_f32_esp(x, y, len, step_x, step_y);
  }
}

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
                          int frac = 15,\
                          int len = 1,\
                          int step_x = 1,\
                          int step_y = 1)
{
  switch (frac)
  {
    case 1: return dsps_f32_s161_esp(x, y, len, step_x, step_y);
    case 2: return dsps_f32_s162_esp(x, y, len, step_x, step_y);
    case 3: return dsps_f32_s163_esp(x, y, len, step_x, step_y);
    case 4: return dsps_f32_s164_esp(x, y, len, step_x, step_y);
    case 5: return dsps_f32_s165_esp(x, y, len, step_x, step_y);
    case 6: return dsps_f32_s166_esp(x, y, len, step_x, step_y);
    case 7: return dsps_f32_s167_esp(x, y, len, step_x, step_y);
    case 8: return dsps_f32_s168_esp(x, y, len, step_x, step_y);
    case 9: return dsps_f32_s169_esp(x, y, len, step_x, step_y);
    case 10: return dsps_f32_s1610_esp(x, y, len, step_x, step_y);
    case 11: return dsps_f32_s1611_esp(x, y, len, step_x, step_y);
    case 12: return dsps_f32_s1612_esp(x, y, len, step_x, step_y);
    case 13: return dsps_f32_s1613_esp(x, y, len, step_x, step_y);
    case 14: return dsps_f32_s1614_esp(x, y, len, step_x, step_y);
    case 15: return dsps_f32_s1615_esp(x, y, len, step_x, step_y);
  }
}

#endif // _dsps_converter_H_