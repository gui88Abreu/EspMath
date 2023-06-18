#include "dsps_addc_platform.h"
#include "esp_opt.h"

#define in_addr   a2
#define out_addr  a3
#define len       a4
#define C         a5
#define frac      a6
#define addr_msk  a7
#define bit_msk   a11

#define C_v       q0
#define in_v      q1
#define out_v     q2

#define sel_p     0

  .text
  .align  ALIGNMENT
  .global dsps_csub_s16_esp
  .type   dsps_csub_s16_esp,@function

dsps_csub_s16_esp: 
// input    - a2
// output   - a3
// len      - a4
// C        - a5
// frac     - a6

  entry	a1, 16

  ee.vldbc.16 C_v, C           // C_v = C
#if (!FAST_MODE)
  l32i C, C, 0                 // Load const
  ssr     frac                 // sar = frac
  loopgtz len, return_success
    l16si a8, in_addr, 0         // load next data
    sub a8, C, a8                // Store the multiplication at a8
    srl a8, a8                   // Shift right (sar), and store in a8
    s16i a8, out_addr, 0         // Store result in the output memory
    addi.n out_addr, out_addr, 2 // next output;
    addi.n in_addr, in_addr, 2   // next input;

#else
  movi.n bit_msk, 0b111
  and    bit_msk, bit_msk, len
  srli   len, len, 3
  beqz   bit_msk, loop_begin
  ee.zero.qacc                         // clear accumulator
  ee.vld.128.ip in_v, in_addr, 16      // load input
  ee.vsubs.s16 out_v, C_v, in_v        // sub
  ee.vst.128.ip out_v, out_addr, 16    // store results
loop_begin:
  loopgtz len, return_success
    ee.zero.qacc                         // clear accumulator
    ee.vld.128.ip in_v, in_addr, 16      // load input
    ee.vsubs.s16 out_v, C_v, in_v        // sub
    ee.vst.128.ip out_v, out_addr, 16    // store results
#endif
return_success:
  movi.n	in_addr, 0  //
  retw.n              // return status ESP_OK