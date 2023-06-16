#ifndef _ESP_MATH_OPT_H_
#define _ESP_MATH_OPT_H_  

#include "esp_idf_version.h"

/**
 * @brief Memory Alignment
 * 
 * Memory Alignment is more useful when FAST_MODE is enabled as well.
 * 
 */
#define ALIGNMENT 16

/**
 * @brief Fast mode consumes more RAM memory, but speed up operations
 * 
 * When fast mode is enabled, all read and write operations are done in 
 * chuncks of 128 bits. When it is disabled, 64, 32, 16, and 8 bits chunks 
 * can be used as well.
 * 
 * @note define FAST_MODE as 0 to disable this and consume less memory.
 */
#ifndef FAST_MODE
#define FAST_MODE 1
#endif

/**
 * @brief If enabled, benchmark test willbe done and result will be printed via serial port
 * 
 */
#ifndef BENCHMARK_TEST
#define BENCHMARK_TEST 1
#endif

#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 0, 0)
#define dsp_ENTER_CRITICAL      portSET_INTERRUPT_MASK_FROM_ISR
#define dsp_EXIT_CRITICAL       portCLEAR_INTERRUPT_MASK_FROM_ISR
#else
#define dsp_ENTER_CRITICAL      portENTER_CRITICAL_NESTED
#define dsp_EXIT_CRITICAL       portEXIT_CRITICAL_NESTED
#endif

#endif