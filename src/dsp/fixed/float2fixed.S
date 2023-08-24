#include "esp_opt.h"

#define x_addr    a2
#define y_addr    a3
#define len       a4
#define step_x    a5
#define step_y    a6

#define x_r       f0
#define y_r       a8

  .equ    FRAC15, 15
  .text
  .align  ALIGNMENT
  .global dsps_f32_s1615_esp
  .type   dsps_f32_s1615_esp,@function
dsps_f32_s1615_esp:
  entry	sp, 16
  slli step_x, step_x, 2
  slli step_y, step_y, 1
  loopgtz len, .R15
    lsi     x_r, x_addr, 0           // load float
    round.s y_r, x_r, FRAC15          // convert
    s16i    y_r, y_addr, 0           // Store result

    add.n x_addr, x_addr, step_x     // next input;
    add.n y_addr, y_addr, step_y     // next input;
.R15:
  movi.n	  a2, 0                    //
  retw.n                             // return status ESP_OK

  .equ    FRAC14, 14
  .text
  .align  ALIGNMENT
  .global dsps_f32_s1614_esp
  .type   dsps_f32_s1614_esp,@function
dsps_f32_s1614_esp:
  entry	sp, 16
  slli step_x, step_x, 2
  slli step_y, step_y, 1
  loopgtz len, .R14
    lsi     x_r, x_addr, 0           // load float
    round.s y_r, x_r, FRAC14          // convert
    s16i    y_r, y_addr, 0           // Store result

    add.n x_addr, x_addr, step_x     // next input;
    add.n y_addr, y_addr, step_y     // next input;
.R14:
  movi.n	  a2, 0                    //
  retw.n                             // return status ESP_OK

  .equ    FRAC13, 13
  .text
  .align  ALIGNMENT
  .global dsps_f32_s1613_esp
  .type   dsps_f32_s1613_esp,@function
dsps_f32_s1613_esp:
  entry	sp, 16
  slli step_x, step_x, 2
  slli step_y, step_y, 1
  loopgtz len, .R13
    lsi     x_r, x_addr, 0           // load float
    round.s y_r, x_r, FRAC13          // convert
    s16i    y_r, y_addr, 0           // Store result

    add.n x_addr, x_addr, step_x     // next input;
    add.n y_addr, y_addr, step_y     // next input;
.R13:
  movi.n	  a2, 0                    //
  retw.n                             // return status ESP_OK

  .equ    FRAC12, 12
  .text
  .align  ALIGNMENT
  .global dsps_f32_s1612_esp
  .type   dsps_f32_s1612_esp,@function
dsps_f32_s1612_esp:
  entry	sp, 16
  slli step_x, step_x, 2
  slli step_y, step_y, 1
  loopgtz len, .R12
    lsi     x_r, x_addr, 0           // load float
    round.s y_r, x_r, FRAC12          // convert
    s16i    y_r, y_addr, 0           // Store result

    add.n x_addr, x_addr, step_x     // next input;
    add.n y_addr, y_addr, step_y     // next input;
.R12:
  movi.n	  a2, 0                    //
  retw.n                             // return status ESP_OK

  .equ    FRAC11, 11
  .text
  .align  ALIGNMENT
  .global dsps_f32_s1611_esp
  .type   dsps_f32_s1611_esp,@function
dsps_f32_s1611_esp:
  entry	sp, 16
  slli step_x, step_x, 2
  slli step_y, step_y, 1
  loopgtz len, .R11
    lsi     x_r, x_addr, 0           // load float
    round.s y_r, x_r, FRAC11          // convert
    s16i    y_r, y_addr, 0           // Store result

    add.n x_addr, x_addr, step_x     // next input;
    add.n y_addr, y_addr, step_y     // next input;
.R11:
  movi.n	  a2, 0                    //
  retw.n                             // return status ESP_OK

  .equ    FRAC10, 10
  .text
  .align  ALIGNMENT
  .global dsps_f32_s1610_esp
  .type   dsps_f32_s1610_esp,@function
dsps_f32_s1610_esp:
  entry	sp, 16
  slli step_x, step_x, 2
  slli step_y, step_y, 1
  loopgtz len, .R10
    lsi     x_r, x_addr, 0           // load float
    round.s y_r, x_r, FRAC10          // convert
    s16i    y_r, y_addr, 0           // Store result

    add.n x_addr, x_addr, step_x     // next input;
    add.n y_addr, y_addr, step_y     // next input;
.R10:
  movi.n	  a2, 0                    //
  retw.n                             // return status ESP_OK

  .equ    FRAC9, 9
  .text
  .align  ALIGNMENT
  .global dsps_f32_s169_esp
  .type   dsps_f32_s169_esp,@function
dsps_f32_s169_esp:
  entry	sp, 16
  slli step_x, step_x, 2
  slli step_y, step_y, 1
  loopgtz len, .R9
    lsi     x_r, x_addr, 0           // load float
    round.s y_r, x_r, FRAC9          // convert
    s16i    y_r, y_addr, 0           // Store result

    add.n x_addr, x_addr, step_x     // next input;
    add.n y_addr, y_addr, step_y     // next input;
.R9:
  movi.n	  a2, 0                    //
  retw.n                             // return status ESP_OK

  .equ    FRAC8, 8
  .text
  .align  ALIGNMENT
  .global dsps_f32_s168_esp
  .type   dsps_f32_s168_esp,@function
dsps_f32_s168_esp:
  entry	sp, 16
  slli step_x, step_x, 2
  slli step_y, step_y, 1
  loopgtz len, .R8
    lsi     x_r, x_addr, 0           // load float
    round.s y_r, x_r, FRAC8          // convert
    s16i    y_r, y_addr, 0           // Store result

    add.n x_addr, x_addr, step_x     // next input;
    add.n y_addr, y_addr, step_y     // next input;
.R8:
  movi.n	  a2, 0                    //
  retw.n                             // return status ESP_OK

  .equ    FRAC7, 7
  .text
  .align  ALIGNMENT
  .global dsps_f32_s167_esp
  .type   dsps_f32_s167_esp,@function
dsps_f32_s167_esp:
  entry	sp, 16
  slli step_x, step_x, 2
  slli step_y, step_y, 1
  loopgtz len, .R7
    lsi     x_r, x_addr, 0           // load float
    round.s y_r, x_r, FRAC7          // convert
    s16i    y_r, y_addr, 0           // Store result

    add.n x_addr, x_addr, step_x     // next input;
    add.n y_addr, y_addr, step_y     // next input;
.R7:
  movi.n	  a2, 0                    //
  retw.n                             // return status ESP_OK

  .equ    FRAC6, 6
  .text
  .align  ALIGNMENT
  .global dsps_f32_s166_esp
  .type   dsps_f32_s166_esp,@function
dsps_f32_s166_esp:
  entry	sp, 16
  slli step_x, step_x, 2
  slli step_y, step_y, 1
  loopgtz len, .R6
    lsi     x_r, x_addr, 0           // load float
    round.s y_r, x_r, FRAC6          // convert
    s16i    y_r, y_addr, 0           // Store result

    add.n x_addr, x_addr, step_x     // next input;
    add.n y_addr, y_addr, step_y     // next input;
.R6:
  movi.n	  a2, 0                    //
  retw.n                             // return status ESP_OK

  .equ    FRAC5, 5
  .text
  .align  ALIGNMENT
  .global dsps_f32_s165_esp
  .type   dsps_f32_s165_esp,@function
dsps_f32_s165_esp:
  entry	sp, 16
  slli step_x, step_x, 2
  slli step_y, step_y, 1
  loopgtz len, .R5
    lsi     x_r, x_addr, 0           // load float
    round.s y_r, x_r, FRAC5          // convert
    s16i    y_r, y_addr, 0           // Store result

    add.n x_addr, x_addr, step_x     // next input;
    add.n y_addr, y_addr, step_y     // next input;
.R5:
  movi.n	  a2, 0                    //
  retw.n                             // return status ESP_OK

  .equ    FRAC4, 4
  .text
  .align  ALIGNMENT
  .global dsps_f32_s164_esp
  .type   dsps_f32_s164_esp,@function
dsps_f32_s164_esp:
  entry	sp, 16
  slli step_x, step_x, 2
  slli step_y, step_y, 1
  loopgtz len, .R4
    lsi     x_r, x_addr, 0           // load float
    round.s y_r, x_r, FRAC4          // convert
    s16i    y_r, y_addr, 0           // Store result

    add.n x_addr, x_addr, step_x     // next input;
    add.n y_addr, y_addr, step_y     // next input;
.R4:
  movi.n	  a2, 0                    //
  retw.n                             // return status ESP_OK

  .equ    FRAC3, 3
  .text
  .align  ALIGNMENT
  .global dsps_f32_s163_esp
  .type   dsps_f32_s163_esp,@function
dsps_f32_s163_esp:
  entry	sp, 16
  slli step_x, step_x, 2
  slli step_y, step_y, 1
  loopgtz len, .R3
    lsi     x_r, x_addr, 0           // load float
    round.s y_r, x_r, FRAC3          // convert
    s16i    y_r, y_addr, 0           // Store result

    add.n x_addr, x_addr, step_x     // next input;
    add.n y_addr, y_addr, step_y     // next input;
.R3:
  movi.n	  a2, 0                    //
  retw.n                             // return status ESP_OK

  .equ    FRAC2, 2
  .text
  .align  ALIGNMENT
  .global dsps_f32_s162_esp
  .type   dsps_f32_s162_esp,@function
dsps_f32_s162_esp:
  entry	sp, 16
  slli step_x, step_x, 2
  slli step_y, step_y, 1
  loopgtz len, .R2
    lsi     x_r, x_addr, 0           // load float
    round.s y_r, x_r, FRAC2          // convert
    s16i    y_r, y_addr, 0           // Store result

    add.n x_addr, x_addr, step_x     // next input;
    add.n y_addr, y_addr, step_y     // next input;
.R2:
  movi.n	  a2, 0                    //
  retw.n                             // return status ESP_OK

  .equ    FRAC1, 1
  .text
  .align  ALIGNMENT
  .global dsps_f32_s161_esp
  .type   dsps_f32_s161_esp,@function
dsps_f32_s161_esp:
  entry	sp, 16
  slli step_x, step_x, 2
  slli step_y, step_y, 1
  loopgtz len, .R1
    lsi     x_r, x_addr, 0           // load float
    round.s y_r, x_r, FRAC1          // convert
    s16i    y_r, y_addr, 0           // Store result

    add.n x_addr, x_addr, step_x     // next input;
    add.n y_addr, y_addr, step_y     // next input;
.R1:
  movi.n	  a2, 0                    //
  retw.n                             // return status ESP_OK