#include "dsps_addc_platform.h"
#include "esp_opt.h"

#define in_addr   a2
#define out_addr  a3
#define len       a4
#define C         a5
#define step_in   a6
#define step_out  a7

  .text
  .align  ALIGNMENT
  .global dsps_cdiv_s8_esp
  .type   dsps_cdiv_s8_esp,@function

dsps_cdiv_s8_esp: 
// input    - a2
// output   - a3
// len      - a4
// C        - a5
// step_in  - a6
// step_out - a7

  entry	a1, 16
  loopgtz len, return_success
    l8ui a8, in_addr, 0              // Load next data
    sext a8, a8, 7                   // sign extend
    quos a9, C, a8                   // Store the multiplication
    s8i a9, out_addr, 0              // Store result

    add out_addr, out_addr, step_out // next output;
    add in_addr, in_addr, step_in    // next input;
return_success:
  movi.n	in_addr, 0  //
  retw.n              // return status ESP_OK