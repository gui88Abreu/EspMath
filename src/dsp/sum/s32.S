#include "esp_opt.h"

#define x_addr    a2
#define y_addr    a3
#define len       a4
#define step_x    a5

#define x_r       a6
#define y_r       a7

  .text
  .align  ALIGNMENT
  .global dsps_vsum_s32_esp
  .type   dsps_vsum_s32_esp,@function

dsps_vsum_s32_esp:
// x        - a2
// y        - a3
// len      - a4
// step_x   - a5

  entry	a1, 16

  movi.n y_r, 0
  slli step_x, step_x, 2
  loopgtz len, return_success
    l32i x_r, x_addr, 0            // Load next data
    add  y_r, y_r, x_r             // add
    add x_addr, x_addr, step_x     // next input;
return_success:
  s32i y_r, y_addr, 0 // Store result
  movi.n	 x_addr, 0  //
  retw.n              // return status ESP_OK