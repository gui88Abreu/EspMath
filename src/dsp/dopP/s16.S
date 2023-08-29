#include "esp_opt.h"

#define x1_addr   a2
#define x2_addr   a3
#define y_addr    a4
#define len       a5
#define step_x1   a6
#define step_x2   a7
#define frac      a8
#define acc       a9
#define aux       a10

#define x1_r      a11
#define x2_r      a12

#define x1_v      q0
#define x2_v      q1
#define y_v       q2
#define acc_v     q3


  .text
  .align  ALIGNMENT
  .global dsps_dotp_s16_esp
  .type   dsps_dotp_s16_esp,@function

dsps_dotp_s16_esp: 
// x1       - a2
// x2       - a3
// y        - a4
// len      - a5
// step_x1  - a6
// step_x2  - a7
// frac     - a8 (stack)

  entry	 sp, 16
  l32i   frac, a1, 16
  ssr    frac                         // sar = frac
  movi.n acc, 0

  bgei   step_x1, 2, .L1
  bgei   step_x2, 2, .L1
  blti       len, 8, .L1              // branch if vector accleration is not possible

  srli   aux, len, 3                  // aux = len / 8
  ee.zero.q acc_v
  loopgtz aux, .L0
    ee.vld.128.ip x1_v, x1_addr, 16   // load input
    ee.vld.128.ip x2_v, x2_addr, 16   // load input
    ee.vmul.s16   y_v, x1_v, x2_v     // multiply 
    ee.vadds.s16  acc_v, acc_v, y_v   // acc +=  x1_v * x2_v
.L0:
  ee.movi.32.a  acc_v, aux, 0
  extui         x1_r, aux, 16, 16
  sext          x1_r, x1_r, 15
  extui         x2_r, aux,  0, 16
  sext          x2_r, x2_r, 15
  add.n         acc, acc, x1_r        //
  add.n         acc, acc, x2_r        //

  ee.movi.32.a  acc_v, aux, 1
  extui         x1_r, aux, 16, 16
  sext          x1_r, x1_r, 15
  extui         x2_r, aux,  0, 16
  sext          x2_r, x2_r, 15
  add.n         acc, acc, x1_r        //
  add.n         acc, acc, x2_r        //

  ee.movi.32.a  acc_v, aux, 2
  ee.src.q      acc_v, acc_v, y_v
  extui         x1_r, aux, 16, 16
  sext          x1_r, x1_r, 15
  extui         x2_r, aux,  0, 16
  sext          x2_r, x2_r, 15
  add.n         acc, acc, x1_r        //
  add.n         acc, acc, x2_r        //

  ee.movi.32.a  acc_v, aux, 3
  ee.src.q      acc_v, acc_v, y_v
  extui         x1_r, aux, 16, 16
  sext          x1_r, x1_r, 15
  extui         x2_r, aux,  0, 16
  sext          x2_r, x2_r, 15
  add.n         acc, acc, x1_r        //
  add.n         acc, acc, x2_r        //

  extui  len, len, 0, 3               // len = len % 8
.L1:
  slli step_x1, step_x1, 1
  slli step_x2, step_x2, 1
  loopgtz len, .R0
    l16si x1_r, x1_addr, 0            // load next data
    add.n x1_addr, x1_addr, step_x1   // next input;

    l16si x2_r, x2_addr, 0            // load next data
    add.n x2_addr, x2_addr, step_x2   // next input;

    mul.aa.ll x1_r, x2_r              // Store the multiplication at the acc
    rsr       x1_r, acclo             // Read the 32 low bits from the acc
    rsr        aux, acchi             // Read the 32 low bits from the acc
    src       x1_r, aux, x1_r         // Shift right (sar), and store in x1_r
    add.n     acc, acc, x1_r          //
.R0:
  s16i acc, y_addr, 0            // Store result in the output memory
  movi.n	  a2, 0                //
  retw.n                         // return status ESP_OK