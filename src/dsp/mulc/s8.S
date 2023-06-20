#include "dsps_mulc_platform.h"
#include "esp_opt.h"

#define x_addr    a2
#define y_addr    a3
#define len       a4
#define C         a5
#define step_x    a6
#define step_y    a7

#define x_r       a8
#define y_r       a9

#if MEMORY_ALIGN
#define shift     a7
#define bit_msk   a6
#define x_v       q0
#define c_v       q1
#define y_v       q2
#endif

  .text
  .align  ALIGNMENT
  .global dsps_mulc_s8_esp
  .type   dsps_mulc_s8_esp,@function

dsps_mulc_s8_esp: 
// x        - a2
// y        - a3
// len      - a4
// Constant - a5
// step_x1  - a6
// step_y   - a7

  entry	a1, 16
  
#if (MEMORY_ALIGN)
  beqi step_x, 1, no_step_mode
#endif
step_mode:
  l32i C, C, 0             // Load const
  loopgtz len, return_success
    l8ui x_r, x_addr, 0         // Load next data
    mul.aa.ll x_r, C            // Store the multiplication in the acc
    rsr y_r, acclo              // Read the 32 low bits from the acc
    s8i y_r, y_addr, 0          // Store result

    add  y_addr, y_addr, step_y  // next output;
    add  x_addr, x_addr, step_x  // next input;
#if (MEMORY_ALIGN)
no_step_mode:
  bgei  step_y, 2, step_mode
  ee.vldbc.8 c_v, C          // c_v = C
  movi shift, 0
  ssr    shift               // sar = frac
  movi   bit_msk, 0b1111
  and    bit_msk, bit_msk, len
  srli   len, len, 4
  beqz   bit_msk, loop_begin
  ee.vld.128.ip x_v, x_addr, 16          // load input
  ee.vmul.s8 y_v, x_v, c_v              // multiply, shift (sar), and store result
  ee.vst.128.ip y_v, y_addr, 16
loop_begin:
  loopgtz len, return_success
    ee.vld.128.ip x_v, x_addr, 16        // load input
    ee.vmul.s8 y_v, x_v, c_v            // multiply, shift (sar), and store result
    ee.vst.128.ip y_v, y_addr, 16
#endif
return_success:
  movi.n	x_addr, 0  //
  retw.n              // return status ESP_OK