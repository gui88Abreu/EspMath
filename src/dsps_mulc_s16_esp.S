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
    .align  4
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
// step_in  - a6
// step_out - a7
// shift    - stack (a8)

    entry	a1, 16
    
    l32i.n	a8, a1,  16     // Load shift to the a8 register
    ssr     a8              // sar = a8

    srli    a4, a4, 1   // a4 = a4>>1
    slli 	a6, a6, 2  	// a6 - step_in<<2, because we load two inputs per loop
    slli 	a7, a7, 1  	// a7 - step_out<<1

    addi    a6, a6, -4;
    addi    a2, a2, -4;

    loopnez a4, loop_twice_mulc_s16
        ldinc m0, a2       // load next data
        mul.DA.LL m0, a5   // Store the multiplication at the acc
        rsr a8, acchi      // Read the 8 high bits from the acc
        rsr a9, acclo      // Read the 32 low bits from the acc
        src a8, a8, a9     // Combine a8 and a9 and store in a8
    	s16i a8, a3, 0     // Store result in the output memory
        add.n a3, a3, a7   // output+=step_out;

        mul.DA.HL m0, a5   // Store the multiplication at the acc
        rsr a8, acchi      // Read the 8 high bits from the acc
        rsr a9, acclo      // Read the 32 low bits from the acc
        src a10, a8, a9    // Combine a8 and a9 and store in a10
    	s16i a10, a3, 0    // Store result in the output memory
        add.n a3, a3, a7   // output+=step_out;
        add.n a2, a2, a6   // input+=step_input;

loop_twice_mulc_s16:
    movi.n	a2, 0  //
    retw.n         // return status ESP_OK

#endif // dsps_mulc_s16_ae32_enabled