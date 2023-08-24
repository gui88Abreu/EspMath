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
    fixed target = 8;
    fixed observed = 18;
    fixed ambient = 20;
    fixed heatsink = 26;
    fixed cooling = 0;
    char energy = 80;
    fixed maxV = 8.0;
    const uint32_t expectTime = 1800;
    const char energyTh = 70;
    const fixed heatsinkTh = 32;
}param;

void regulator(Parameters* p)
{
  fixed X0 = (p->observed - p->target) / (abs(p->observed) + abs(p->target));
  fixed X1 = 1 - ((float)((p->expectTime - 360) / p->expectTime));
  fixed X2 = (p->ambient - p->target) / (abs(p->ambient) + abs(p->target));
  fixed X3 = (p->heatsink - p->ambient) / (p->heatsinkTh - p->ambient);
  fixed X4 = p->energy > p->energyTh ? 0 : p->energy / 100;
  X4 *= X4;
  fixed X5 = p->cooling - p->target;
  
  debug.print("X0*C0: " + String(X0*C0));
  debug.print("X1*C1: " + String(X1*C1));
  debug.print("X2*C2: " + String(X2*C2));
  debug.print("X3*C3: " + String(X3*C3));
  debug.print("X4*C4: " + String(X4*C4));
  
  // Not working. Need to be reviewed
  // if ((!X0 && !X2) || X0 < 0.f || X3 >= 1.f || X5 > 0.f)
  // {
  //   debug.print("Shutdown!");
  //   return;
  // }

  float x = regFUNC(X0, X1, X2, X3, X4);
  fixed activF = tanh(x);
  
  debug.print(activF);
  debug.print(((activF + 1.) / 2.) * (p->maxV));
}

void setup()
{
  param p;
  vTaskDelay(pdMS_TO_TICKS(5000));
  regulator(&p);
}

void loop(){}