#include "dsps_addc_platform.h"
#include "esp_opt.h"

#define in_addr   a2
#define out_addr  a3
#define len       a4
#define C         a5
#define step_in   a6
#define step_out  a7
#define bit_msk   a8

#define in_v      q1
#define out_v     q2

    .text
    .align  ALIGNMENT
    .global dsps_cdiv_s32_esp
    .type   dsps_cdiv_s32_esp,@function

dsps_cdiv_s32_esp: 
// input    - a2
// output   - a3
// len      - a4
// C        - a5
// step_in  - a6
// step_out - a7

  entry	a1, 16

  slli step_in, step_in, 2
  slli step_out, step_out, 2
  loopgtz len, return_success
    l32i a8, in_addr, 0          // Load next data
    quos  a9, C, a8               // Store the multiplication in the a9
    s32i a9, out_addr, 0         // Store result in the output memory

    add in_addr, in_addr, step_in
    add out_addr, out_addr, step_out
return_success:
  movi.n	in_addr, 0  //
  retw.n              // return status ESP_OK