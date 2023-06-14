#include "dsps_mulc_platform.h"
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
#define sel_8     0b1000
#define lsb3_msk  0b0111
#define lsb4_msk  0b1111
#define lsb16_msk 0xFFFF

  .text
  .align  ALIGNMENT
  .global dsps_mulc_s16_esp
  .type   dsps_mulc_s16_esp,@function

dsps_mulc_s16_esp: 
// input    - a2
// output   - a3
// len      - a4
// C        - a5
// frac     - a6

  entry	a1, 16

  ee.movi.32.q C_v, C, sel_p // C_v = C
#if (!FAST_MODE)
  ssr     frac                 // sar = frac
  movi  bit_msk, lsb16_msk
  movi  addr_msk, lsb4_msk
decider:
  blti len, 1, return_success
  and   a8, in_addr, addr_msk
  beqz  a8, mulc_128_bits
  beqi  a8, sel_8, mulc_64_bits
  j mulc_32_bits

mulc_16_bits:
  l16si a8, in_addr, 0         // load next data
  mul.aa.ll a8, C              // Store the multiplication at the acc
  rsr a8, acclo                // Read the 32 low bits from the acc
  srl a8, a8                   // Shift right (sar), and store in a8
  s16i a8, out_addr, 0         // Store result in the output memory
  addi.n out_addr, out_addr, 2 // next output;
  addi.n in_addr, in_addr, 2   // next input;

  addi.n len, len, -1          // decrement length;
  j decider

mulc_32_bits:
  blti len, 2, mulc_16_bits
  l32i a8, in_addr, 0          // Load next data
  mul.aa.ll a8, C              // Store the multiplication in the acc
  rsr a9, acclo                // Read the 32 low bits from the acc
  srl a9, a9                   // Shift right (sar), and store in a9

  mul.aa.hl a8, C              // Store the multiplication in the acc
  rsr a10, acclo               // Read the 32 low bits from the acc
  srl a10, a10                 // Shift right (sar), and store in a10

  slli a10, a10, 16            // Shift left to give space to the 1st mulc
  and   a9,  a9, bit_msk       // Select the first 16 bits
  or   a10, a10, a9            // Combine results in the same register
  s32i a10, out_addr, 0        // Store result in the output memory

  addi.n out_addr, out_addr, 4 // next output;
  addi.n in_addr, in_addr, 4   // next input;
  addi.n len, len, -2          // decrement length;
  j decider

mulc_64_bits:
  blti len, 4, mulc_32_bits
  ee.zero.qacc                         // clear accumulator
  ee.vld.h.64.ip in_v, in_addr, 8      // load input
  ee.vsmulas.s16.qacc in_v, C_v, sel_p // multiply the input by C
  ee.srcmb.s16.qacc out_v, frac, 0       // store results in the output register
  ee.vst.h.64.ip out_v, out_addr, 8    // store results
  addi.n len, len, -4                  // decrement length
  j decider

mulc_128_bits:
  blti len, 8, mulc_64_bits

  ee.zero.qacc                         // clear accumulator
  ee.vld.128.ip in_v, in_addr, 16      // load input
  ee.vsmulas.s16.qacc in_v, C_v, sel_p // multiply the input by C
  ee.srcmb.s16.qacc out_v, frac, 0       // store results in the output register
  ee.vst.128.ip out_v, out_addr, 16    // store results
  addi.n len, len, -8                  // decrement length
  j decider
#else
  movi.n bit_msk, lsb3_msk
  and    bit_msk, bit_msk, len
  srli   len, len, 3
  beqz   bit_msk, loop_begin
  ee.zero.qacc                         // clear accumulator
  ee.vld.128.ip in_v, in_addr, 16      // load input
  ee.vsmulas.s16.qacc in_v, C_v, sel_p // multiply the input by C
  ee.srcmb.s16.qacc out_v, frac, 0     // store results in the output register
  ee.vst.128.ip out_v, out_addr, 16    // store results
loop_begin:
  loopgtz len, return_success
    ee.zero.qacc                         // clear accumulator
    ee.vld.128.ip in_v, in_addr, 16      // load input
    ee.vsmulas.s16.qacc in_v, C_v, sel_p // multiply the input by C
    ee.srcmb.s16.qacc out_v, frac, 0     // store results in the output register
    ee.vst.128.ip out_v, out_addr, 16    // store results
#endif
return_success:
  movi.n	in_addr, 0  //
  retw.n              // return status ESP_OK