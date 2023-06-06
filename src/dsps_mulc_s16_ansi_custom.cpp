#include "dsps_mulc_s16.h"

esp_err_t dsps_mulc_f32_ansi_custom(const float *input, float *output, int len, float C, int step_in, int step_out, const int shift)
{
  for (int i = 0 ; i < len ; i++) {
      int32_t acc = (int32_t)input[i * step_in] * (int32_t)C;
      output[i * step_out] = (int16_t)(acc>>shift);
  }
  return ESP_OK;
}