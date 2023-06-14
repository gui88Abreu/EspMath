#ifndef _ESP_MATH_OPT_H_
#define _ESP_MATH_OPT_H_  

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
#define FAST_MODE 1

#endif