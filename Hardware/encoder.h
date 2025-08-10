#ifndef __ENCODER_H__
#define __ENCODER_H__

#include "stm32f10x.h"

void Encoder_Init(void);
int16_t Encoder_GetCount(void);

extern int16_t Encoder_Count;

#endif
