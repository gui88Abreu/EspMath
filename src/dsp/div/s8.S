#include "dsps_mul_platform.h"
#include "esp_opt.h"

#define x1_addr   a2
#define x2_addr   a3
#define y_addr    a4
#define len       a5
#define step_x1   a6
#define step_x2   a7
#define step_y    a8

#define x1_r      a9
#define x2_r      a10
#define y_r       a11

  .text
  .align  ALIGNMENT
  .global dsps_div_s8_esp
  .type   dsps_div_s8_esp,@function

dsps_div_s8_esp:
// x1       - a2
// x2       - a3
// output   - a4
// len      - a5
// step_x1  - a6
// step_x2  - a7
// step_y   - a8 (stack)

  entry	a1, 16

  l32i a8, a1, 16

  loopgtz len, return_success
    l8ui x1_r, x1_addr, 0          // Load next data
    l8ui x2_r, x2_addr, 0          // Load next data
    quos y_r, x1_r, x2_r           // Store the multiplication in the acc
    s8i  y_r, y_addr, 0            // Store result

    add y_addr, y_addr, step_y     // next output;
    add x1_addr, x1_addr, step_x1  // next input;
    add x2_addr, x2_addr, step_x2  // next input;
return_success:
  movi.n	x1_addr, 0  //
  retw.n              // return status ESP_OK