#ifndef __TOUCH_T__
#define __TOUCH_T__

#include <Arduino.h>

#define MINPRESSURE 200
#define MAXPRESSURE 1000

uint16_t xpos, ypos;  //screen coordinates
tp = ts.getPoint();   //tp.x, tp.y are ADC values


void getPos() {
  
  if (tp.z > MINPRESSURE && tp.z < MAXPRESSURE) {
      
      xpos = map(tp.y, TS_TOP, TS_BOT, 0, 320);
      ypos = map(tp.x, TS_RT, TS_LEFT, 0, 240);
      
      }
}


#endif