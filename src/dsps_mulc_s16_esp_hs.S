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

#include "dsps_mulc_platform.h"
#if (dsps_mulc_s16_ae32_enabled == 1)

    .text
    .align  16
    .global dsps_mulc_s16_esp
    .type   dsps_mulc_s16_esp,@function
// The function implements the following C code:
// esp_err_t dsps_mulc_s16_ansi_custom(const int16_t *input, int16_t *output, int len, int16_t C, int step_in, int step_out, int shift)
// {
//   if (NULL == input) return ESP_ERR_DSP_PARAM_OUTOFRANGE;
//   if (NULL == output) return ESP_ERR_DSP_PARAM_OUTOFRANGE;
// 
//   for (int i = 0 ; i < len ; i++) {
//       int32_t acc = (int32_t)input[i * step_in] * (int32_t)C;
//       output[i * step_out] = (int16_t)(acc>>shift);
//   }
//   return ESP_OK;
// }
dsps_mulc_s16_esp: 
// input    - a2
// output   - a3
// len      - a4
// C        - a5
// frac     - a6

    entry	a1, 0
    ssr     a6             // sar = a6

    ee.movi.32.q q0, a5, 0 // q0 = a5
decider:
    bltz a4, return_success
    beqz a4, return_success
    movi.n a8, 0b1111
    and a8, a2, a8
    beqz a8, _128_bits
    beqi a8, 0b1000, _64_bits
    j _32_bits

_16_bits:
    l16si a10, a2, 0    // load next data
    mul.aa.ll a10, a5  // Store the multiplication at the acc
    rsr a8, acchi      // Read the 8 high bits from the acc
    rsr a9, acclo      // Read the 32 low bits from the acc
    src a8, a8, a9     // Combine a8 and a9, shift right, and store in a8
    s16i a8, a3, 0     // Store result in the output memory
    addi.n a3, a3, 2   // next output;
    addi.n a2, a2, 2   // next input;

    addi.n a4, a4, -1  // decrement length;
    j decider

_32_bits:
    blti a4, 2, _16_bits
    l32i a10, a2, 0    // load next data
    mul.aa.ll a10, a5  // Store the multiplication at the acc
    rsr a8, acchi      // Read the 8 high bits from the acc
    rsr a9, acclo      // Read the 32 low bits from the acc
    src a8, a8, a9     // Combine a8 and a9, shift right, and store in a8
    s16i a8, a3, 0     // Store result in the output memory
    addi.n a3, a3, 2   // next output;

    mul.aa.hl a10, a5  // Store the multiplication at the acc
    rsr a8, acchi      // Read the 8 high bits from the acc
    rsr a9, acclo      // Read the 32 low bits from the acc
    src a10, a8, a9    // Combine a8 and a9, shift right, and store in a10
    s16i a10, a3, 0    // Store result in the output memory
    addi.n a3, a3, 2   // next output;
    addi.n a2, a2, 4   // next input;

    addi.n a4, a4, -2  // decrement length;
    j decider

_64_bits:
    blti a4, 4, _32_bits
    ee.zero.qacc
    ee.vld.h.64.ip q1, a2, 8
    ee.vsmulas.s16.qacc q1, q0, 0
    ee.srcmb.s16.qacc q2, a6, 0
    ee.vst.h.64.ip q2, a3, 8
    addi.n a4, a4, -4  // decrement length;
    j decider

_128_bits:
    blti a4, 8, _64_bits
    ee.zero.qacc
    ee.vld.128.ip q1, a2, 16
    ee.vsmulas.s16.qacc q1, q0, 0
    ee.srcmb.s16.qacc q2, a6, 0
    ee.vst.128.ip q2, a3, 16
    addi.n a4, a4, -8  // decrement length;
    j decider

return_success:
    movi.n	a2, 0  //
    retw.n         // return status ESP_OK

#endif // dsps_mulc_s16_ae32_enabled