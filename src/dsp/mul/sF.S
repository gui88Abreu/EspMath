#include "dsps_mul_platform.h"
#include "esp_opt.h"

#define x1_addr   a2
#define x2_addr   a3
#define y_addr    a4
#define len       a5
#define step_x1   a6
#define step_x2   a7
#define step_y    a8

#define x1_r      f0
#define x2_r      f1
#define y_r       f2

#define unligned  a6
#define bit_msk   a7
#define x1_v      q0
#define x2_v      q1
#define y_v       q2

  .text
  .align  ALIGNMENT
  .global dsps_mul_f32_esp
  .type   dsps_mul_f32_esp,@function

dsps_mul_f32_esp:
// x1       - a2
// x2       - a3
// output   - a4
// len      - a5

  entry	a1, 16

  l32i step_y, a1, 16
  beqi step_x1, 1, no_step_mode
step_mode:
  slli step_x1, step_x1, 2
  slli step_x2, step_x2, 2
  slli  step_y,  step_y, 2
  loopgtz len, return_success
    lsi x1_r, x1_addr, 0
    lsi x2_r, x2_addr, 0
    mul.s y_r, x1_r, x2_r
    ssi y_r, y_addr, 0

    add  y_addr,  y_addr, step_y   // next output;
    add x1_addr, x1_addr, step_x1  // next input;
    add x2_addr, x2_addr, step_x2  // next input;
no_step_mode:
  bgei step_x2, 2, step_mode
  bgei  step_y, 2, step_mode
  movi.n bit_msk, 0b11
  and    unligned, bit_msk, len
  srli   len, len, 2
loop_begin:
  loopgtz len, loop_unligned
    ee.ldf.128.ip f3, f2, f1, f0, x1_addr, 16          // load input
    ee.ldf.128.ip f7, f6, f5, f4, x2_addr, 16          // load input
    mul.s f3, f3, f7
    mul.s f2, f2, f6
    mul.s f1, f1, f5
    mul.s f0, f0, f4
    ee.stf.128.ip f3, f2, f1, f0, y_addr, 16         // store result
loop_unligned:
  loopgtz unligned, return_success
    lsi x1_r, x1_addr, 0
    lsi x2_r, x2_addr, 0
    mul.s y_r, x1_r, x2_r
    ssi y_r, y_addr, 0

    addi  y_addr,  y_addr, 4  // next output;
    addi x1_addr, x1_addr, 4  // next input;
    addi x2_addr, x2_addr, 4  // next input;
return_success:
  movi.n	x1_addr, 0  //
  retw.n              // return status ESP_OK