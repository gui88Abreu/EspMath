#include "dsps_mulc_s16.h"

esp_err_t dsps_mulc_s16_ansi_custom(const int16_t *input, int16_t *output, int len, int16_t C, int step_in, int step_out, int shift)
{
  if (NULL == input) return ESP_ERR_DSP_PARAM_OUTOFRANGE;
  if (NULL == output) return ESP_ERR_DSP_PARAM_OUTOFRANGE;

  for (int i = 0 ; i < len ; i++) {
      int32_t acc = (int32_t)input[i * step_in] * (int32_t)C;
      output[i * step_out] = (int16_t)(acc>>shift);
  }
  return ESP_OK;
}