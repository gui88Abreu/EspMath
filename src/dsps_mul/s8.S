#include "dsps_mul_platform.h"
#include "esp_opt.h"

#define x1_addr   a2
#define x2_addr   a3
#define y_addr    a4
#define len       a5
#define shift     a6
#define addr_msk  a7

#define x1_r      a8
#define x2_r      a12

#define bit_msk   a11

#define x1_v      q0
#define x2_v      q1
#define y_v       q2

  .text
  .align  ALIGNMENT
  .global dsps_mul_u8_esp
  .type   dsps_mul_u8_esp,@function

dsps_mul_u8_esp:
// x1       - a2
// x2       - a3
// output   - a4
// len      - a5

  entry	a1, 16

#if (!FAST_MODE)
  loopgtz len, return_success
    l8ui x1_r, x1_addr, 0          // Load next data
    l8ui x2_r, x2_addr, 0          // Load next data
    mul.aa.ll x1_r, x2_r           // Store the multiplication in the acc
    rsr a9, acclo                  // Read the 32 low bits from the acc
    s8i a9, y_addr, 0              // Store result

    addi y_addr, y_addr, 1         // next output;
    addi x1_addr, x1_addr, 1       // next input;
    addi x2_addr, x2_addr, 1       // next input;
#else
  movi shift, 0
  ssr    shift               // sar = frac
  movi   bit_msk, 0b1111
  and    bit_msk, bit_msk, len
  srli   len, len, 4
  beqz   bit_msk, loop_begin
  ee.vld.128.ip x1_v, x1_addr, 16         // load input
  ee.vld.128.ip x2_v, x2_addr, 16         // load input
  ee.vmul.u8 y_v, x2_v, x1_v              // multiply, shift (sar), and store result
  ee.vst.128.ip y_v, y_addr, 16
loop_begin:
  loopgtz len, return_success
    ee.vld.128.ip x1_v, x1_addr, 16         // load input
    ee.vld.128.ip x2_v, x2_addr, 16         // load input
    ee.vmul.u8 y_v, x2_v, x1_v              // multiply, shift (sar), and store result
    ee.vst.128.ip y_v, y_addr, 16
#endif
return_success:
  movi.n	x1_addr, 0  //
  retw.n              // return status ESP_OK