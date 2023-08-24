#include "esp_opt.h"

#define x_addr    a2
#define y_addr    a3
#define len       a4
#define step_x    a5
#define step_y    a6

#define x_r       f0
#define y_r       a8

  .text
  .align  ALIGNMENT
  .global dsps_f32_s16_esp
  .type   dsps_f32_s16_esp,@function

dsps_f32_s16_esp:
// x        - a2
// y        - a3
// len      - a4
// step_x   - a5
// step_y   - a6

  entry	sp, 16

  slli step_x, step_x, 2
  slli step_y, step_y, 1
  loopgtz len, return_success
    lsi     x_r, x_addr, 0           // load float
    round.s y_r, x_r, FRACTIONAL     // convert
    s16i    y_r, y_addr, 0           // Store result

    add.n x_addr, x_addr, step_x     // next input;
    add.n y_addr, y_addr, step_y     // next input;
return_success:
  movi.n	  a2, 0                    //
  retw.n                             // return status ESP_OK