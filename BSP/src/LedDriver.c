#include "LedDriver.h"

void LedDriver_Init(void) {
    LED_RED_OFF();
    LED_GREEN_OFF();
    LED_YELLOW_OFF();
}

void TestLedDriver(void) {
    LED_RED_TOGGLE();
    LED_GREEN_TOGGLE();
    LED_YELLOW_TOGGLE();
}