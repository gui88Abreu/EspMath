#ifndef _ESP_DSP_H_
#define _ESP_DSP_H_

#include <sdkconfig.h>

#ifdef CONFIG_IDF_TARGET_ESP32S3
#if CONFIG_IDF_TARGET_ESP32S3
#include "dsp/add/dsps_add_esp.h"
#include "dsp/sub/dsps_sub_esp.h"
#include "dsp/mul/dsps_mul_esp.h"
#include "dsp/div/dsps_div_esp.h"
#include "dsp/addc/dsps_addc_esp.h"
#include "dsp/subc/dsps_subc_esp.h"
#include "dsp/mulc/dsps_mulc_esp.h"
#include "dsp/divc/dsps_divc_esp.h"
#include "dsp/dopP/dot_product.h"
#endif
#endif

#define exec_dsp(dsp_func, ...)\
{\
unsigned intlevel = dsp_ENTER_CRITICAL();\
dsp_func(__VA_ARGS__);\
dsp_EXIT_CRITICAL(intlevel);\
}\

#endif