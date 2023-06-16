#define FAST_MODE 1
#include "esp_opt.h"
#include "test_bench.h"

void setup(){
  size_t array_length = nonZeroRandomNumber<size_t>(256);
  vTaskDelay(pdMS_TO_TICKS(5000));
  debug.print("----------------------------------------------------------------------");
  debug.print("----------------------------------------------------------------------");
  debug.print("Array Size: " + String(array_length));
  debug.print("Free size[bytes]: " + String(xPortGetFreeHeapSize()));
  debug.print("----------------------------------------------------------------------");
  debug.print("----------------------------------------------------------------------");
  debug.print("Testing floating-point arrays arithmetic...");
  test_ari<float>(array_length);
  debug.print("Succeeded!");
  debug.print("----------------------------------------------------------------------");
  debug.print("----------------------------------------------------------------------");
  debug.print("Testing integer 32 bits arrays arithmetic...");
  test_ari<int32_t>(array_length);
  debug.print("Succeeded!");
  debug.print("----------------------------------------------------------------------");
  debug.print("----------------------------------------------------------------------");
  debug.print("Testing integer 16 bits arrays arithmetic...");
  test_ari<int16_t>(array_length);
  debug.print("Succeeded!");
  debug.print("----------------------------------------------------------------------");
  debug.print("----------------------------------------------------------------------");
  debug.print("Testing integer 8 bits arrays arithmetic...");
  test_ari<uint8_t>(array_length);
  debug.print("Completed!");
  debug.print("Free size[bytes]: " + String(xPortGetFreeHeapSize()));
  debug.print("----------------------------------------------------------------------");
  debug.print("----------------------------------------------------------------------");
}

void loop(){}
