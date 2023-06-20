#include "dsps_mul_platform.h"
#include "esp_opt.h"

#define x1_addr   a2
#define x2_addr   a3
#define y_addr    a4
#define len       a5
#define step_x1   a6
#define step_x2   a7
#define step_y    a8
#define frac      a9

#define x1_r      a10
#define x2_r      a11
#define y_r       a12

#if MEMORY_ALIGN
#define bit_msk   a6
#define x1_v      q0
#define x2_v      q1
#define y_v       q2
#endif

  .text
  .align  ALIGNMENT
  .global dsps_mul_s16_esp
  .type   dsps_mul_s16_esp,@function

dsps_mul_s16_esp: 
// x1       - a2
// x2       - a3
// output   - a4
// len      - a5
// step_x1  - a6
// step_x2  - a7
// step_y   - a8 (stack)
// frac     - a9 (stack)

  entry	a1, 16
  l32i step_y, a1, 16
  l32i frac, a1, 20
  ssr  frac                 // sar = frac
#if (MEMORY_ALIGN)
  beqi step_x1, 1, no_step_mode
#endif
step_mode:
  slli step_x1, step_x1, 1
  slli step_x2, step_x2, 1
  slli  step_y,  step_y, 1
  loopgtz len, return_success
    l16si x1_r, x1_addr, 0      // load next data
    l16si x2_r, x2_addr, 0      // load next data
    mul.aa.ll x1_r, x2_r        // Store the multiplication at the acc
    rsr y_r, acclo              // Read the 32 low bits from the acc
    srl y_r, y_r                // Shift right (sar), and store in x1_r
    s16i y_r, y_addr, 0         // Store result in the output memory

    add  y_addr,  y_addr, step_y   // next output;
    add x1_addr, x1_addr, step_x1  // next input;
    add x2_addr, x2_addr, step_x2  // next input;

#if (MEMORY_ALIGN)
no_step_mode:
  bgei step_x2, 2, step_mode
  bgei  step_y, 2, step_mode
  movi.n bit_msk, 0b111
  and    bit_msk, bit_msk, len
  srli   len, len, 3
  beqz   bit_msk, loop_begin
  ee.vld.128.ip x1_v, x1_addr, 16    // load input
  ee.vld.128.ip x2_v, x2_addr, 16    // load input
  ee.vmul.s16   y_v, x1_v, x2_v      // multiply the input by C
  ee.vst.128.ip y_v, y_addr, 16      // store results
loop_begin:
  loopgtz len, return_success
    ee.vld.128.ip x1_v, x1_addr, 16    // load input
    ee.vld.128.ip x2_v, x2_addr, 16    // load input
    ee.vmul.s16   y_v, x1_v, x2_v      // multiply the input by C
    ee.vst.128.ip y_v, y_addr, 16      // store results
#endif
return_success:
  movi.n	x1_addr, 0  //
  retw.n              // return status ESP_OK