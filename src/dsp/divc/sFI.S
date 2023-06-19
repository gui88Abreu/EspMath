#include "dsps_addc_platform.h"
#include "esp_opt.h"

#define in_addr   a2
#define out_addr  a3
#define len       a4
#define C         a5
#define step_in   a6
#define step_out  a7

  .text
  .align  ALIGNMENT
  .global dsps_cdiv_f32_esp
  .type   dsps_cdiv_f32_esp,@function

dsps_cdiv_f32_esp:
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
    l32i a11, in_addr, 0
    mov.n   a10, C
    call8   __divsf3
    s32i.n  a10, out_addr, 0

    add in_addr, in_addr, step_in
    add out_addr, out_addr, step_out
return_success:
  movi.n	in_addr, 0  //
  retw.n              // return status ESP_OK