#include "dsps_mulc_platform.h"
#include "esp_opt.h"

#define in_addr   a2
#define out_addr  a3
#define len       a4
#define C         a5
#define shift     a6
#define addr_msk  a7
#define bit_msk   a11
#define cnst     a12

#define C_v       q0
#define in_v      q1
#define out_v     q2

#define sel_p     0
#define sel_8     0b1000
#define lsb4_msk  0x000F
#define lsb8_msk  0x00FF
#define lsb16_msk 0xFFFF

  .text
  .align  ALIGNMENT
  .global dsps_mulc_u8_esp
  .type   dsps_mulc_u8_esp,@function

dsps_mulc_u8_esp: 
// input    - a2
// output   - a3
// len      - a4
// C        - a5

  entry	a1, 16

  movi.n shift, 0
  ssr    shift               // sar = frac
  ee.vldbc.8 C_v, C         // C_v = C
#if (!FAST_MODE)
  l32i cnst, C, 0             // Load const
  movi.n  bit_msk, lsb8_msk
  movi.n  addr_msk, lsb4_msk
decider:
  blti len, 1, return_success
  and   a8, in_addr, addr_msk
  beqz  a8, mulc_128_bits
  beqi  a8, sel_8, mulc_64_bits
  j mulc_32_bits

mulc_8_bits:
  l8ui a8, in_addr, 0          // Load next data
  mul.aa.ll a8, cnst           // Store the multiplication in the acc
  rsr a9, acclo                // Read the 32 low bits from the acc
  s8i a9, out_addr, 0          // Store result

  addi out_addr, out_addr, 1 // next output;
  addi in_addr, in_addr, 1   // next input;
  addi len, len, -1          // decrement length;

  j decider

mulc_16_bits:
  blti len, 2, mulc_8_bits
  l16ui a8, in_addr, 0         // Load next data
  mul.aa.ll a8, cnst           // Store the multiplication in the acc
  rsr a9, acclo                // Read the 32 low bits from the acc
  
  slli a8, a8, 8
  mul.aa.hl a8, cnst           // Store the multiplication in the acc
  rsr a8, acclo                // Read the 32 low bits from the acc
  slli a8, a8, 8
  or a9, a8, a9                // Combine results in the same register

  s16i a9, out_addr, 0         // Store result
  addi out_addr, out_addr, 2   // next output;
  addi in_addr, in_addr, 2     // next input;
  addi len, len, -2            // decrement length;

  j decider
mulc_32_bits:
  blti len, 4, mulc_16_bits

  l32i a8, in_addr, 0          // Load next data

  mul.aa.ll a8, cnst           // Store the multiplication in the acc
  rsr a9, acclo                // Read the 32 low bits from the acc
  and a10, a9, bit_msk         // Select the least significant 8 bits

  mul.aa.hl a8, cnst           // Store the multiplication in the acc
  rsr a9, acclo                // Read the 32 low bits from the acc
  and a9, a9, bit_msk          // Select the least significant 8 bits
  slli a9, a9, 16              // Shift left to give space to the 1st mulc
  or a10, a10, a9              // Combine results in the same register

  srli a8, a8, 8
  mul.aa.hl a8, cnst           // Store the multiplication in the acc
  rsr a9, acclo                // Read the 32 low bits from the acc
  and a9, a9, bit_msk          // Select the least significant 8 bits
  slli a9, a9, 24              // Shift left to give space to the 1st mulc
  or a10, a10, a9              // Combine results in the same register

  mul.aa.ll a8, cnst           // Store the multiplication in the acc
  rsr a9, acclo                // Read the 32 low bits from the acc
  and a9, a9, bit_msk          // Select the least significant 8 bits
  slli a9, a9, 8               // Shift left to give space to the 1st mulc
  or a10, a10, a9              // Combine results in the same register

  s32i a10, out_addr, 0        // Store result in the output memory
  addi out_addr, out_addr, 4   // next output;
  addi in_addr, in_addr, 4     // next input;
  addi len, len, -4            // decrement length;
  j decider

mulc_64_bits:
  blti len, 8, mulc_32_bits
  ee.vld.h.64.ip in_v, in_addr, 8      // load input
  ee.vmul.u8 out_v, in_v, C_v          // multiply, shift (sar), and store result
  ee.vst.h.64.ip out_v, out_addr, 8    // store results
  addi.n len, len, -8                  // decrement length
  j decider

mulc_128_bits:
  blti len, 16, mulc_64_bits
  ee.vld.128.ip in_v, in_addr, 16          // load input
  ee.vmul.u8 out_v, in_v, C_v              // multiply, shift (sar), and store result
  ee.vst.128.ip out_v, out_addr, 16
  addi.n len, len, -16                                   // len-=16
  j decider
#else
  movi.n bit_msk, lsb4_msk
  and    bit_msk, bit_msk, len
  srli   len, len, 4
  beqz   bit_msk, loop_begin
  ee.vld.128.ip in_v, in_addr, 16          // load input
  ee.vmul.u8 out_v, in_v, C_v              // multiply, shift (sar), and store result
  ee.vst.128.ip out_v, out_addr, 16
loop_begin:
  loopgtz len, return_success
    ee.vld.128.ip in_v, in_addr, 16        // load input
    ee.vmul.u8 out_v, in_v, C_v            // multiply, shift (sar), and store result
    ee.vst.128.ip out_v, out_addr, 16
#endif
return_success:
  movi.n	in_addr, 0  //
  retw.n              // return status ESP_OK