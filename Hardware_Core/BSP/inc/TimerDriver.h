#ifndef TIMER_DRIVER_H
#define TIMER_DRIVER_H

#include "main.h"

extern TIM_HandleTypeDef htim6;
extern TIM_HandleTypeDef htim7;

void Timer_Init(void);
// void DelayUsNop(uint32_t iusTimeOut);
// void DelayMsNop(uint32_t imsTimeOut);

#endif /* TIMER_DRIVER_H */