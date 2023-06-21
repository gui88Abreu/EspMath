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
  .global dsps_csub_s8_esp
  .type   dsps_csub_s8_esp,@function

dsps_csub_s8_esp: 
// x        - a2
// y        - a3
// len      - a4
// C        - a5
// step_x   - a6
// step_y   - a7

  entry	a1, 16

  beqi step_x, 1, no_step_mode
step_mode:
  l32i C, C, 0                   // Load const
  loopgtz len, return_success
    l8ui x_r, x_addr, 0          // Load next data
    sub y_r, C, x_r              // Store the multiplication
    s8i y_r, y_addr, 0           // Store result

    add  y_addr, y_addr, step_y  // next output;
    add  x_addr, x_addr, step_x  // next input;
no_step_mode:
  bgei  step_y, 2, step_mode
  ee.vldbc.8 c_v, C          // c_v = C
  movi   bit_msk, 0b1111
  and    unligned, bit_msk, len
  srli   len, len, 4
loop_begin:
  loopgtz len, loop_unligned
    ee.vld.128.ip x_v, x_addr, 16         // load input
    ee.vsubs.s8 y_v, c_v, x_v           // sub
    ee.vst.128.ip y_v, y_addr, 16
loop_unligned:
  l32i C, C, 0                // Load const
  loopgtz unligned, return_success
    l8ui x_r, x_addr, 0          // Load next data
    sub y_r, C, x_r              // Store the multiplication
    s8i y_r, y_addr, 0           // Store result

    addi  y_addr, y_addr, 1  // next output;
    addi  x_addr, x_addr, 1  // next input;
return_success:
  movi.n	x_addr, 0  //
  retw.n              // return status ESP_OK