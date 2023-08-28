#include "test_bench.h"
#include "bootloader_random.h"

void setup(){
  bootloader_random_enable();
  size_t array_length = 6;//nonZeroRandomNumber<size_t>(24);
  vTaskDelay(pdMS_TO_TICKS(5000));
  debug.print("----------------------------------------------------------------------");
  debug.print("----------------------------------------------------------------------");
  debug.print("Array Size: " + String(array_length));
  debug.print("Free size[bytes]: " + String(xPortGetFreeHeapSize()));
  debug.print("----------------------------------------------------------------------");
  debug.print("----------------------------------------------------------------------");
  debug.print("Testing floating-point arrays arithmetic...");
  test_ari<float>(array_length);
  debug.print("----------------------------------------------------------------------");
  debug.print("----------------------------------------------------------------------");
  debug.print("Testing integer 32 bits arrays arithmetic...");
  test_ari<int32_t>(array_length);
  debug.print("----------------------------------------------------------------------");
  debug.print("----------------------------------------------------------------------");
  debug.print("Testing integer 16 bits arrays arithmetic...");
  test_ari<int16_t>(array_length);
  debug.print("----------------------------------------------------------------------");
  debug.print("----------------------------------------------------------------------");
  debug.print("Testing fixed point arrays arithmetic...");
  test_fixed_point(array_length, FRACTIONAL);
  debug.print("----------------------------------------------------------------------");
  debug.print("----------------------------------------------------------------------");
  debug.print("Testing integer 8 bits arrays arithmetic...");
  test_ari<int8_t>(array_length);
  debug.print("Completed!");
  debug.print("Free size[bytes]: " + String(xPortGetFreeHeapSize()));
  debug.print("----------------------------------------------------------------------");
  debug.print("----------------------------------------------------------------------");
}

void loop(){}
