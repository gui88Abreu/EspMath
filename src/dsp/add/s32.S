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
    .global dsps_add_s32_esp
    .type   dsps_add_s32_esp,@function

dsps_add_s32_esp: 
// x1       - a2
// x2       - a3
// output   - a4
// len      - a5
// step_x1  - a6
// step_x2  - a7
// step_y   - a8 (stack)

  entry	sp, 16
  l32i step_y, a1, 16
  beqi step_x1, 1, no_step_mode
step_mode:
  slli step_x1, step_x1, 2
  slli step_x2, step_x2, 2
  slli  step_y,  step_y, 2
  loopgtz len, return_success
    l32i x1_r, x1_addr, 0        // Load next data
    l32i x2_r, x2_addr, 0        // Load next data
    add y_r, x1_r, x2_r          // Store the multiplication in the a9
    s32i y_r, y_addr, 0          // Store result in the output memory

    add y_addr, y_addr, step_y      // next output;
    add x1_addr, x1_addr, step_x1   // next input;
    add x2_addr, x2_addr, step_x2   // next input;
no_step_mode:
  bgei step_x2, 2, step_mode
  bgei  step_y, 2, step_mode
  movi.n bit_msk, 0b11
  and    unligned, bit_msk, len
  srli   len, len, 2
loop_begin:
  loopgtz len, loop_unligned
    ee.vld.128.ip x1_v, x1_addr, 16    // load input
    ee.vld.128.ip x2_v, x2_addr, 16    // load input
    ee.movi.32.a x1_v, a8, 0           // Move first operand
    ee.movi.32.a x1_v, a9, 1           // Move second operand
    ee.movi.32.a x2_v, a10, 0          // Move third operand
    ee.movi.32.a x2_v, a11, 1          // Move fourth operand
    add a8, a8, a10                   // Store the multiplication in the a8
    add a9, a9, a11                   // Store the multiplication in the a9
    ee.movi.32.q y_v, a8, 0            // Move result of the first multiplication
    ee.movi.32.q y_v, a9, 1            // Move result of the second multiplication
    ee.movi.32.a x1_v, a8, 2           // Move first operand
    ee.movi.32.a x1_v, a9, 3           // Move second operand
    ee.movi.32.a x2_v, a10, 2          // Move third operand
    ee.movi.32.a x2_v, a11, 3          // Move fourth operand
    add a8, a8, a10                   // Store the multiplication in the a8
    add a9, a9, a11                   // Store the multiplication in the a9
    ee.movi.32.q y_v, a8, 2            // Move result of the third multiplication
    ee.movi.32.q y_v, a9, 3            // Move result of the fourth multiplication
    ee.vst.128.ip y_v, y_addr, 16      // store results
loop_unligned:
  loopgtz unligned, return_success
    l32i x1_r, x1_addr, 0        // Load next data
    l32i x2_r, x2_addr, 0        // Load next data
    add y_r, x1_r, x2_r          // Store the multiplication in the a9
    s32i y_r, y_addr, 0          // Store result in the output memory

    addi y_addr, y_addr, 4      // next output;
    addi x1_addr, x1_addr, 4   // next input;
    addi x2_addr, x2_addr, 4   // next input;
return_success:
  movi.n	x1_addr, 0  //
  retw.n              // return status ESP_OK