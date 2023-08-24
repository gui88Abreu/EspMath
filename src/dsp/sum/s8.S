#include "esp_opt.h"

#define x_addr    a2
#define y_addr    a3
#define len       a4
#define step_x    a5

#define x_r       a6
#define y_r       a7

  .text
  .align  ALIGNMENT
  .global dsps_vsum_s8_esp
  .type   dsps_vsum_s8_esp,@function

dsps_vsum_s8_esp:
// x        - a2
// y        - a3
// len      - a4
// step_x   - a5

  entry	sp, 16

  movi.n y_r, 0
  loopgtz len, return_success
    l8ui x_r, x_addr, 0           // Load next data
    sext x_r, x_r, 7              // sign extend
    add  y_r, y_r, x_r            // add
    add x_addr, x_addr, step_x     // next input;
return_success:
  s8i y_r, y_addr, 0  // Store result
  movi.n	 x_addr, 0  //
  retw.n              // return status ESP_OK