#include "dsps_addc_platform.h"
#include "esp_opt.h"

#define x_addr    a2
#define y_addr    a3
#define len       a4
#define C         a5
#define step_x    a6
#define step_y    a7
#define frac      a8

#define x_r       a9
#define y_r       a10

#if MEMORY_ALIGN
#define bit_msk   a6
#define aone      a7
#define x_v       q0
#define c_v       q1
#define y_v       q2
#define v1        q3
#endif

  .text
  .align  ALIGNMENT
  .global dsps_addc_s16_esp
  .type   dsps_addc_s16_esp,@function

dsps_addc_s16_esp: 
// x        - a2
// y        - a3
// len      - a4
// Constant - a5
// step_x   - a6
// step_y   - a7
// frac     - a8 (stack)

  entry	a1, 16

  l32i frac, a1, 16
  ssr     frac                 // sar = frac
#if (MEMORY_ALIGN)
  beqi step_x, 1, no_step_mode
#endif
step_mode:
  l16si C, C, 0                // Load const
  slli step_x,  step_x, 1
  slli step_y,  step_y, 1
  loopgtz len, return_success
    l16si x_r, x_addr, 0        // load next data
    add.n y_r, x_r, C            // Store the multiplication at x_r
    srl y_r, y_r                 // Shift right (sar), and store in x_r
    s16i y_r, y_addr, 0        // Store result in the output memory

    add  y_addr, y_addr, step_y  // next output;
    add  x_addr, x_addr, step_x  // next input;

#if (MEMORY_ALIGN)
no_step_mode:
  bgei  step_y, 2, step_mode
  movi aone, 0x00010001
  ee.movi.32.a v1, aone, 0
  ee.movi.32.a v1, aone, 1
  ee.movi.32.a v1, aone, 2
  ee.movi.32.a v1, aone, 3
  ee.vldbc.16 c_v, C           // c_v = C
  movi.n bit_msk, 0b111
  and    bit_msk, bit_msk, len
  srli   len, len, 3
  beqz   bit_msk, loop_begin
  ee.zero.qacc                         // clear accumulator
  ee.vld.128.ip x_v, x_addr, 16      // load input
  ee.vadds.s16 y_v, x_v, c_v        // Add constant
  ee.vmul.s16    y_v, y_v, v1
  ee.vst.128.ip y_v, y_addr, 16    // store results
loop_begin:
  loopgtz len, return_success
    ee.zero.qacc                         // clear accumulator
    ee.vld.128.ip x_v, x_addr, 16      // load input
    ee.vadds.s16 y_v, x_v, c_v        // Add constant
    ee.vmul.s16    y_v, y_v, v1
    ee.vst.128.ip y_v, y_addr, 16    // store results
#endif
return_success:
  movi.n	x_addr, 0  //
  retw.n              // return status ESP_OK