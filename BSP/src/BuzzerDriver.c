#include "BuzzerDriver.h"

void BuzzerDriver_On(void) {
    HAL_TIM_Base_Start_IT(&htim6);
}

void BuzzerDriver_Off(void) {
    HAL_TIM_Base_Stop_IT(&htim6);
    GPIO_BUZZER_OFF();
}

