#include "TimerDriver.h"

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM6) {
        // Handle TIM6 interrupt
        GPIO_BUZZER_TOGGLE(); // Toggle the buzzer
    }
}
