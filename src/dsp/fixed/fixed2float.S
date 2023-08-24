#include "esp_opt.h"

#define x_addr    a2
#define y_addr    a3
#define len       a4
#define step_x    a5
#define step_y    a6

#define x_r       a8
#define y_r       f0

  .equ     FRAC15, 15
  .text
  .align  ALIGNMENT
  .global dsps_s1615_f32_esp
  .type   dsps_s1615_f32_esp,@function
dsps_s1615_f32_esp:
  entry	sp, 16
  slli step_x, step_x, 1
  slli step_y, step_y, 2
  loopgtz len, .R15
    l16si   x_r, x_addr, 0           // load s16
    float.s y_r, x_r, FRAC15     // convert
    ssi     y_r, y_addr, 0           // store result
    add.n x_addr, x_addr, step_x     // next input;
    add.n y_addr, y_addr, step_y     // next input;
.R15:
  movi.n	  a2, 0                    //
  retw.n                             // return status ESP_OK

  .equ     FRAC14, 14
  .text
  .align  ALIGNMENT
  .global dsps_s1614_f32_esp
  .type   dsps_s1614_f32_esp,@function
dsps_s1614_f32_esp:
  entry	sp, 16
  slli step_x, step_x, 1
  slli step_y, step_y, 2
  loopgtz len, .R14
    l16si   x_r, x_addr, 0           // load s16
    float.s y_r, x_r, FRAC14     // convert
    ssi     y_r, y_addr, 0           // store result
    add.n x_addr, x_addr, step_x     // next input;
    add.n y_addr, y_addr, step_y     // next input;
.R14:
  movi.n	  a2, 0                    //
  retw.n                             // return status ESP_OK

  .equ     FRAC13, 13
  .text
  .align  ALIGNMENT
  .global dsps_s1613_f32_esp
  .type   dsps_s1613_f32_esp,@function
dsps_s1613_f32_esp:
  entry	sp, 16
  slli step_x, step_x, 1
  slli step_y, step_y, 2
  loopgtz len, .R13
    l16si   x_r, x_addr, 0           // load s16
    float.s y_r, x_r, FRAC13     // convert
    ssi     y_r, y_addr, 0           // store result
    add.n x_addr, x_addr, step_x     // next input;
    add.n y_addr, y_addr, step_y     // next input;
.R13:
  movi.n	  a2, 0                    //
  retw.n                             // return status ESP_OK

  .equ     FRAC12, 12
  .text
  .align  ALIGNMENT
  .global dsps_s1612_f32_esp
  .type   dsps_s1612_f32_esp,@function
dsps_s1612_f32_esp:
  entry	sp, 16
  slli step_x, step_x, 1
  slli step_y, step_y, 2
  loopgtz len, .R12
    l16si   x_r, x_addr, 0           // load s16
    float.s y_r, x_r, FRAC12     // convert
    ssi     y_r, y_addr, 0           // store result
    add.n x_addr, x_addr, step_x     // next input;
    add.n y_addr, y_addr, step_y     // next input;
.R12:
  movi.n	  a2, 0                    //
  retw.n                             // return status ESP_OK

  .equ     FRAC11, 11
  .text
  .align  ALIGNMENT
  .global dsps_s1611_f32_esp
  .type   dsps_s1611_f32_esp,@function
dsps_s1611_f32_esp:
  entry	sp, 16
  slli step_x, step_x, 1
  slli step_y, step_y, 2
  loopgtz len, .R11
    l16si   x_r, x_addr, 0           // load s16
    float.s y_r, x_r, FRAC11     // convert
    ssi     y_r, y_addr, 0           // store result
    add.n x_addr, x_addr, step_x     // next input;
    add.n y_addr, y_addr, step_y     // next input;
.R11:
  movi.n	  a2, 0                    //
  retw.n                             // return status ESP_OK

  .equ     FRAC10, 10
  .text
  .align  ALIGNMENT
  .global dsps_s1610_f32_esp
  .type   dsps_s1610_f32_esp,@function
dsps_s1610_f32_esp:
  entry	sp, 16
  slli step_x, step_x, 1
  slli step_y, step_y, 2
  loopgtz len, .R10
    l16si   x_r, x_addr, 0           // load s16
    float.s y_r, x_r, FRAC10     // convert
    ssi     y_r, y_addr, 0           // store result
    add.n x_addr, x_addr, step_x     // next input;
    add.n y_addr, y_addr, step_y     // next input;
.R10:
  movi.n	  a2, 0                    //
  retw.n                             // return status ESP_OK

  .equ     FRAC9, 9
  .text
  .align  ALIGNMENT
  .global dsps_s169_f32_esp
  .type   dsps_s169_f32_esp,@function
dsps_s169_f32_esp:
  entry	sp, 16
  slli step_x, step_x, 1
  slli step_y, step_y, 2
  loopgtz len, .R9
    l16si   x_r, x_addr, 0           // load s16
    float.s y_r, x_r, FRAC9     // convert
    ssi     y_r, y_addr, 0           // store result
    add.n x_addr, x_addr, step_x     // next input;
    add.n y_addr, y_addr, step_y     // next input;
.R9:
  movi.n	  a2, 0                    //
  retw.n                             // return status ESP_OK

  .equ     FRAC8, 8
  .text
  .align  ALIGNMENT
  .global dsps_s168_f32_esp
  .type   dsps_s168_f32_esp,@function
dsps_s168_f32_esp:
  entry	sp, 16
  slli step_x, step_x, 1
  slli step_y, step_y, 2
  loopgtz len, .R8
    l16si   x_r, x_addr, 0           // load s16
    float.s y_r, x_r, FRAC8     // convert
    ssi     y_r, y_addr, 0           // store result
    add.n x_addr, x_addr, step_x     // next input;
    add.n y_addr, y_addr, step_y     // next input;
.R8:
  movi.n	  a2, 0                    //
  retw.n                             // return status ESP_OK

  .equ     FRAC7, 7
  .text
  .align  ALIGNMENT
  .global dsps_s167_f32_esp
  .type   dsps_s167_f32_esp,@function
dsps_s167_f32_esp:
  entry	sp, 16
  slli step_x, step_x, 1
  slli step_y, step_y, 2
  loopgtz len, .R7
    l16si   x_r, x_addr, 0           // load s16
    float.s y_r, x_r, FRAC7     // convert
    ssi     y_r, y_addr, 0           // store result
    add.n x_addr, x_addr, step_x     // next input;
    add.n y_addr, y_addr, step_y     // next input;
.R7:
  movi.n	  a2, 0                    //
  retw.n                             // return status ESP_OK

  .equ     FRAC6, 6
  .text
  .align  ALIGNMENT
  .global dsps_s166_f32_esp
  .type   dsps_s166_f32_esp,@function
dsps_s166_f32_esp:
  entry	sp, 16
  slli step_x, step_x, 1
  slli step_y, step_y, 2
  loopgtz len, .R6
    l16si   x_r, x_addr, 0           // load s16
    float.s y_r, x_r, FRAC6     // convert
    ssi     y_r, y_addr, 0           // store result
    add.n x_addr, x_addr, step_x     // next input;
    add.n y_addr, y_addr, step_y     // next input;
.R6:
  movi.n	  a2, 0                    //
  retw.n                             // return status ESP_OK

  .equ     FRAC5, 5
  .text
  .align  ALIGNMENT
  .global dsps_s165_f32_esp
  .type   dsps_s165_f32_esp,@function
dsps_s165_f32_esp:
  entry	sp, 16
  slli step_x, step_x, 1
  slli step_y, step_y, 2
  loopgtz len, .R5
    l16si   x_r, x_addr, 0           // load s16
    float.s y_r, x_r, FRAC5     // convert
    ssi     y_r, y_addr, 0           // store result
    add.n x_addr, x_addr, step_x     // next input;
    add.n y_addr, y_addr, step_y     // next input;
.R5:
  movi.n	  a2, 0                    //
  retw.n                             // return status ESP_OK

  .equ     FRAC4, 4
  .text
  .align  ALIGNMENT
  .global dsps_s164_f32_esp
  .type   dsps_s164_f32_esp,@function
dsps_s164_f32_esp:
  entry	sp, 16
  slli step_x, step_x, 1
  slli step_y, step_y, 2
  loopgtz len, .R4
    l16si   x_r, x_addr, 0           // load s16
    float.s y_r, x_r, FRAC4     // convert
    ssi     y_r, y_addr, 0           // store result
    add.n x_addr, x_addr, step_x     // next input;
    add.n y_addr, y_addr, step_y     // next input;
.R4:
  movi.n	  a2, 0                    //
  retw.n                             // return status ESP_OK

  .equ     FRAC3, 3
  .text
  .align  ALIGNMENT
  .global dsps_s163_f32_esp
  .type   dsps_s163_f32_esp,@function
dsps_s163_f32_esp:
  entry	sp, 16
  slli step_x, step_x, 1
  slli step_y, step_y, 2
  loopgtz len, .R3
    l16si   x_r, x_addr, 0           // load s16
    float.s y_r, x_r, FRAC3     // convert
    ssi     y_r, y_addr, 0           // store result
    add.n x_addr, x_addr, step_x     // next input;
    add.n y_addr, y_addr, step_y     // next input;
.R3:
  movi.n	  a2, 0                    //
  retw.n                             // return status ESP_OK

  .equ     FRAC2, 2
  .text
  .align  ALIGNMENT
  .global dsps_s162_f32_esp
  .type   dsps_s162_f32_esp,@function
dsps_s162_f32_esp:
  entry	sp, 16
  slli step_x, step_x, 1
  slli step_y, step_y, 2
  loopgtz len, .R2
    l16si   x_r, x_addr, 0           // load s16
    float.s y_r, x_r, FRAC2     // convert
    ssi     y_r, y_addr, 0           // store result
    add.n x_addr, x_addr, step_x     // next input;
    add.n y_addr, y_addr, step_y     // next input;
.R2:
  movi.n	  a2, 0                    //
  retw.n                             // return status ESP_OK

  .equ     FRAC1, 1
  .text
  .align  ALIGNMENT
  .global dsps_s161_f32_esp
  .type   dsps_s161_f32_esp,@function
dsps_s161_f32_esp:
  entry	sp, 16
  slli step_x, step_x, 1
  slli step_y, step_y, 2
  loopgtz len, .R1
    l16si   x_r, x_addr, 0           // load s16
    float.s y_r, x_r, FRAC1     // convert
    ssi     y_r, y_addr, 0           // store result
    add.n x_addr, x_addr, step_x     // next input;
    add.n y_addr, y_addr, step_y     // next input;
.R1:
  movi.n	  a2, 0                    //
  retw.n                             // return status ESP_OK