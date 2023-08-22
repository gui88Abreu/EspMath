#include "esp_fixed_point.h"
#include "esp_debug.h"

#define C0  2.5f  /*Coefficient of the temperature difference parameter*/
#define C1   .1f  /*Coefficient of the spent time parameter*/
#define C2   .8f  /*Coefficient of the target to ambient temperature difference parameter*/
#define C3 -1.6f  /*Coefficient of the heatsink parameter*/
#define C4 -1.0f  /*Coefficient of the battery energy parameter*/

#define regFUNC(X0, X1, X2, X3, X4) (X0*C0 + X1*C1 + X2*C2 + X3*C3 + X4*C4)

using namespace espmath;
  
typedef struct Parameters{
    fixed target = 8.0f;
    fixed observed = 18.0f;
    fixed ambient = 20.0f;
    fixed heatsink = 26.0f;
    fixed cooling = 0.0f;
    char energy = 60;
    const uint16_t maxmV = 8000;
    const uint32_t expectTime = 1800;
    const char energyTh = 70;
    const fixed heatsinkTh = 32.0f;
}param;

void regulator(Parameters* p)
{
  fixed X0 = (p->observed - p->target) / (abs((float)p->observed) + abs((float)p->target));
  fixed X1 = 1.0f - ((float)((p->expectTime - 360) / p->expectTime));
  fixed X2 = (p->ambient - p->target) / (abs((float)p->ambient) + abs((float)p->target));
  fixed X3 = (p->heatsink - p->ambient) / (p->heatsinkTh - p->ambient);
  fixed X4 = p->energy > p->energyTh ? 1.0f : p->energy / 100.0f;
  X4 *= X4;
  fixed X5 = p->cooling - p->target;
  
  debug.print("X0*C0: " + String(float(X0*C0)));
  debug.print("X1*C1: " + String(float(X1*C1)));
  debug.print("X2*C2: " + String(float(X2*C2)));
  debug.print("X3*C3: " + String(float(X3*C3)));
  debug.print("X4*C4: " + String(float(X4*C4)));
  
  // if ((!X0 && !X2) || X0 < 0.f || X3 >= 1.f || X5 > 0.f)
  // {
  //   debug.print("Shutdown!");
  //   return;
  // }

  float x = regFUNC(X0, X1, X2, X3, X4);
  fixed activF = tanh(x);
  
  debug.print(activF);
  debug.print(((activF + 1.f) / 2.f) * (p->maxmV / 1000.f));
}

void setup()
{
  param p;
  vTaskDelay(pdMS_TO_TICKS(5000));
  regulator(&p);
}

void loop(){}