#include "dsps_addc_platform.h"
#include "esp_opt.h"

#define x_addr   a2
#define y_addr  a3
#define len       a4
#define C         a5
#define step_x    a6
#define step_y    a7

#define c_r       f0
#define x_r       f1
#define y_r       f2

#define unligned  a6
#define bit_msk   a7

  .text
  .align  ALIGNMENT
  .global dsps_addc_f32_esp
  .type   dsps_addc_f32_esp,@function

dsps_addc_f32_esp:
// x        - a2
// y        - a3
// len      - a4
// Constant - a5
// step_x   - a6
// step_y   - a7

  entry	sp, 16
    
  wfr		c_r, C		// a5 - load to the f0
  beqi step_x, 1, no_step_mode
step_mode:
  slli step_x,  step_x, 2
  slli step_y,  step_y, 2
  loopgtz len, return_success
    lsi x_r, x_addr, 0
    add.s y_r, x_r, c_r
    ssi y_r, y_addr, 0

    add  y_addr, y_addr, step_y  // next output;
    add  x_addr, x_addr, step_x  // next input;
no_step_mode:
  bgei  step_y, 2, step_mode
  movi.n bit_msk, 0b11
  and    unligned, bit_msk, len
  srli   len, len, 2
loop_begin:
  loopgtz len, loopgtz_unligned
    ee.ldf.128.ip f4, f3, f2, f1, x_addr, 16          // load input
    add.s f4, f4, c_r
    add.s f3, f3, c_r
    add.s f2, f2, c_r
    add.s f1, f1, c_r
    ee.stf.128.ip f4, f3, f2, f1, y_addr, 16         // store result
loopgtz_unligned:
  loopgtz unligned, return_success
    lsi x_r, x_addr, 0
    add.s y_r, x_r, c_r
    ssi y_r, y_addr, 0

    addi  y_addr, y_addr, 4  // next output;
    addi  x_addr, x_addr, 4  // next input;
return_success:
  movi.n	x_addr, 0  //
  retw.n              // return status ESP_OK