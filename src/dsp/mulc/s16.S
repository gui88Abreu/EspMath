#include "dsps_mulc_platform.h"
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
#define x_v       q0
#define c_v       q1
#define y_v       q2
#define sel_p     0
#endif

  .text
  .align  ALIGNMENT
  .global dsps_mulc_s16_esp
  .type   dsps_mulc_s16_esp,@function

dsps_mulc_s16_esp: 
// x        - a2
// y        - a3
// len      - a4
// Constant - a5
// step_x1  - a6
// step_y   - a7
// frac     - a8 (stack)

  entry	a1, 16

  l32i frac, a1, 16
#if (MEMORY_ALIGN)
  beqi step_x, 1, no_step_mode
#endif
step_mode:
  slli step_x,  step_x, 1
  slli step_y,  step_y, 1
  ssr     frac                 // sar = frac
  loopgtz len, return_success
    l16si x_r, x_addr, 0         // load next data
    mul.aa.ll x_r, C              // Store the multiplication at the acc
    rsr y_r, acclo                // Read the 32 low bits from the acc
    srl y_r, y_r                   // Shift right (sar), and store in a8
    s16i y_r, y_addr, 0         // Store result in the output memory

    add  y_addr, y_addr, step_y  // next output;
    add  x_addr, x_addr, step_x  // next input;
#if (MEMORY_ALIGN)
no_step_mode:
  bgei  step_y, 2, step_mode
  ee.movi.32.q c_v, C, sel_p // c_v = C
  movi.n bit_msk, 0b111
  and    bit_msk, bit_msk, len
  srli   len, len, 3
  beqz   bit_msk, loop_begin
  ee.zero.qacc                        // clear accumulator
  ee.vld.128.ip x_v, x_addr, 16       // load input
  ee.vsmulas.s16.qacc x_v, c_v, sel_p // multiply the input by C
  ee.srcmb.s16.qacc y_v, frac, 0      // store results in the output register
  ee.vst.128.ip y_v, y_addr, 16       // store results
loop_begin:
  loopgtz len, return_success
    ee.zero.qacc                         // clear accumulator
    ee.vld.128.ip x_v, x_addr, 16      // load input
    ee.vsmulas.s16.qacc x_v, c_v, sel_p // multiply the input by C
    ee.srcmb.s16.qacc y_v, frac, 0     // store results in the output register
    ee.vst.128.ip y_v, y_addr, 16    // store results
#endif
return_success:
  movi.n	x_addr, 0  //
  retw.n              // return status ESP_OK