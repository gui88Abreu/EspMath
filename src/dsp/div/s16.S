#include "dsps_mul_platform.h"
#include "esp_opt.h"

#define x1_addr   a2
#define x2_addr   a3
#define y_addr    a4
#define len       a5
#define step_x1   a6
#define step_x2   a7
#define step_y    a8
#define frac      a9

#define x1_r      a9
#define x2_r      a10
#define y_r       a11

  .text
  .align  ALIGNMENT
  .global dsps_div_s16_esp
  .type   dsps_div_s16_esp,@function

dsps_div_s16_esp:
// x1       - a2
// x2       - a3
// output   - a4
// len      - a5
// step_x1  - a6
// step_x2  - a7
// step_y   - a8 (stack)
// frac     - a9 (stack)

  entry	sp, 16

  slli step_x1, step_x1, 1
  l32i step_y, a1, 16
  slli step_x2, step_x2, 1
  l32i frac, a1, 20
  ssl  frac
  slli step_y,   step_y, 1
  loopgtz len, return_success
    l16si x1_r, x1_addr, 0         // Load next data
    add x1_addr, x1_addr, step_x1  // next input;
    sll   x1_r, x1_r               // shif left
    l16si x2_r, x2_addr, 0         // Load next data
    add x2_addr, x2_addr, step_x2  // next input;
    quos y_r, x1_r, x2_r           // divide
    s16i y_r, y_addr, 0            // Store result
    add y_addr, y_addr, step_y     // next output;
return_success:
  movi.n	x1_addr, 0  //
  retw.n              // return status ESP_OK