#include "dsps_add_platform.h"
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

#define unligned  a6
#define bit_msk   a7
#define x1_v      q0
#define x2_v      q1
#define y_v       q2

  .text
  .align  ALIGNMENT
  .global dsps_add_s8_esp
  .type   dsps_add_s8_esp,@function

dsps_add_s8_esp:
// x1       - a2
// x2       - a3
// output   - a4
// len      - a5
// step_x1  - a6
// step_x2  - a7
// step_y   - a8 (stack)

  entry	a1, 16

  l32i step_y, a1, 16
  beqi step_x1, 1, no_step_mode
step_mode:
  loopgtz len, return_success
    l8ui x1_r, x1_addr, 0          // Load next data
    l8ui x2_r, x2_addr, 0          // Load next data
    add.n  y_r, x1_r, x2_r         // Store the multiplication in the acc
    s8i  y_r, y_addr, 0            // Store result

    add  y_addr,  y_addr, step_y   // next output;
    add x1_addr, x1_addr, step_x1  // next input;
    add x2_addr, x2_addr, step_x2  // next input;
no_step_mode:
  bgei step_x2, 2, step_mode
  bgei  step_y, 2, step_mode
  movi   bit_msk, 0b1111
  and    unligned, bit_msk, len
  srli   len, len, 4
loop_begin:
  loopgtz len, loop_unligned
    ee.vld.128.ip x1_v, x1_addr, 16         // load input
    ee.vld.128.ip x2_v, x2_addr, 16         // load input
    ee.vadds.s8 y_v, x2_v, x1_v             // add
    ee.vst.128.ip y_v, y_addr, 16
loop_unligned:
  loopgtz unligned, return_success
    l8ui x1_r, x1_addr, 0          // Load next data
    l8ui x2_r, x2_addr, 0          // Load next data
    add.n  y_r, x1_r, x2_r         // Store the multiplication in the acc
    s8i  y_r, y_addr, 0            // Store result

    addi  y_addr,  y_addr, 1   // next output;
    addi x1_addr, x1_addr, 1  // next input;
    addi x2_addr, x2_addr, 1  // next input;
return_success:
  movi.n	x1_addr, 0  //
  retw.n              // return status ESP_OK