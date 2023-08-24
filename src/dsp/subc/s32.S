#include "dsps_addc_platform.h"
#include "esp_opt.h"

#define x_addr    a2
#define y_addr    a3
#define len       a4
#define C         a5
#define step_x    a6
#define step_y    a7

#define x_r       a8
#define y_r       a9

#define unligned  a6
#define bit_msk   a7
#define x_v       q0
#define c_v       q1
#define y_v       q2

    .text
    .align  ALIGNMENT
    .global dsps_csub_s32_esp
    .type   dsps_csub_s32_esp,@function

dsps_csub_s32_esp: 
// x        - a2
// y        - a3
// len      - a4
// C        - a5
// step_x   - a6
// step_y   - a7

  entry	sp, 16

  beqi step_x, 1, no_step_mode
step_mode:
  slli  step_x,  step_x, 2
  slli  step_y,  step_y, 2
  loopgtz len, return_success
    l32i x_r, x_addr, 0         // Load next data
    sub  y_r,      C, x_r       // Store the multiplication in the a9
    s32i y_r, y_addr, 0         // Store result in the output memory

    add  y_addr, y_addr, step_y  // next output;
    add  x_addr, x_addr, step_x  // next input;
no_step_mode:
  bgei  step_y, 2, step_mode
  movi.n bit_msk, 0b11
  and    unligned, bit_msk, len
  srli   len, len, 2
loop_begin:
  loopgtz len, loop_unligned
    ee.vld.128.ip x_v, x_addr, 16      // load input
    ee.movi.32.a x_v, a8, 0             // Move first operand
    ee.movi.32.a x_v, a9, 1             // Move second operand
    ee.movi.32.a x_v, a10, 2            // Move third operand
    ee.movi.32.a x_v, a11, 3            // Move fourth operand
    sub a8, C, a8                       // Store the multiplication in the a8
    sub a9, C, a9                       // Store the multiplication in the a9
    sub a10, C, a10                     // Store the multiplication in the a10
    sub a11, C, a11                     // Store the multiplication in the a11
    ee.movi.32.q y_v, a8, 0            // Move result of the first multiplication
    ee.movi.32.q y_v, a9, 1            // Move result of the second multiplication
    ee.movi.32.q y_v, a10, 2           // Move result of the third multiplication
    ee.movi.32.q y_v, a11, 3           // Move result of the fourth multiplication
    ee.vst.128.ip y_v, y_addr, 16    // store results
loop_unligned:
  loopgtz unligned, return_success
    l32i x_r, x_addr, 0         // Load next data
    sub  y_r,      C, x_r       // Store the multiplication in the a9
    s32i y_r, y_addr, 0         // Store result in the output memory

    addi  y_addr, y_addr, 4  // next output;
    addi  x_addr, x_addr, 4  // next input;
return_success:
  movi.n	x_addr, 0  //
  retw.n              // return status ESP_OK