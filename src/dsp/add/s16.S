#include "dsps_add_platform.h"
#include "esp_opt.h"

#define x1_addr   a2
#define x2_addr   a3
#define y_addr    a4
#define len       a5
#define frac      a6
#define addr_msk  a7

#define x1_r      a8
#define x2_r      a9
#define y_r       a10

#define bit_msk   a11

#define x1_v      q0
#define x2_v      q1
#define y_v       q2

  .text
  .align  ALIGNMENT
  .global dsps_add_s16_esp
  .type   dsps_add_s16_esp,@function

dsps_add_s16_esp: 
// x1       - a2
// x2       - a3
// output   - a4
// len      - a5
// frac     - a6

  entry	a1, 16
  ssr     frac                 // sar = frac
#if (!FAST_MODE)
  loopgtz len, return_success
    l16si x1_r, x1_addr, 0       // load next data
    l16si x2_r, x2_addr, 0       // load next data
    add.n y_r, x1_r, x2_r        // Store the multiplication at the acc
    srl y_r, y_r                 // Shift right (sar), and store in x1_r
    s16i y_r, y_addr, 0          // Store result in the output memory

    addi.n y_addr, y_addr, 2     // next output;
    addi.n x1_addr, x1_addr, 2   // next input;
    addi.n x2_addr, x2_addr, 2   // next input;

#else
  movi.n bit_msk, 0b111
  and    bit_msk, bit_msk, len
  srli   len, len, 3
  beqz   bit_msk, loop_begin
  ee.vld.128.ip x1_v, x1_addr, 16    // load input
  ee.vld.128.ip x2_v, x2_addr, 16    // load input
  ee.vadds.s16   y_v, x1_v, x2_v     // multiply the input by C
  ee.vst.128.ip y_v, y_addr, 16      // store results
loop_begin:
  loopgtz len, return_success
    ee.vld.128.ip x1_v, x1_addr, 16    // load input
    ee.vld.128.ip x2_v, x2_addr, 16    // load input
    ee.vadds.s16   y_v, x1_v, x2_v     // multiply the input by C
    ee.vst.128.ip y_v, y_addr, 16      // store results
#endif
return_success:
  movi.n	x1_addr, 0  //
  retw.n              // return status ESP_OK