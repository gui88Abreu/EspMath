// Copyright 2018-2019 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef _custom_dsps_mulc_H_
#define _custom_dsps_mulc_H_
#include "dsp_err.h"

#include "dsps_mulc_platform.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**@{*/
/**
 * @brief   multiply constant
 *
 * The function multiplies input array to the constant value
 * x[i*step_out] = y[i*step_in]*C; i=[0..len)
 * The implementation use ANSI C and could be compiled and run on any platform
 *
 * @param[in] input: input array
 * @param output: output array
 * @param len: amount of operations for arrays
 * @param C: constant value
 * @param step_in: step over input array (by default should be 1)
 * @param step_out: step over output array (by default should be 1)
 * @param shift: Fractional number
 *
 * @return
 *      - ESP_OK on success
 *      - One of the error codes from DSP library
 */

esp_err_t dsps_mulc_s16_esp(const int16_t *input, int16_t *output, int len, int16_t C, int step_in = 1, int step_out = 1, int shift = 0);

/**@}*/

#ifdef __cplusplus
}
#endif

#define dsps_mulc_s16 dsps_mulc_s16_esp

#endif // _dsps_mulc_H_