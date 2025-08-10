#ifndef __LIGHTSENSOR_H__
#define __LIGHTSENSOR_H__

#include "stm32f10x.h"

void LightSensor_Init(void);
uint8_t LightSensor_GetState(void);

#endif
