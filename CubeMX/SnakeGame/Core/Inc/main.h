/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h5xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define RETURN_BTN_Pin GPIO_PIN_1
#define RETURN_BTN_GPIO_Port GPIOF
#define RETURN_BTN_EXTI_IRQn EXTI1_IRQn
#define OK_BTN_Pin GPIO_PIN_2
#define OK_BTN_GPIO_Port GPIOF
#define OK_BTN_EXTI_IRQn EXTI2_IRQn
#define DOWN_BTN_Pin GPIO_PIN_4
#define DOWN_BTN_GPIO_Port GPIOF
#define DOWN_BTN_EXTI_IRQn EXTI4_IRQn
#define UP_BTN_Pin GPIO_PIN_5
#define UP_BTN_GPIO_Port GPIOF
#define UP_BTN_EXTI_IRQn EXTI5_IRQn
#define LCD_BackLight_Pin GPIO_PIN_7
#define LCD_BackLight_GPIO_Port GPIOF
#define BUZZER_Pin GPIO_PIN_13
#define BUZZER_GPIO_Port GPIOF
#define LED_RED_Pin GPIO_PIN_14
#define LED_RED_GPIO_Port GPIOF
#define RIGHT_BTN_Pin GPIO_PIN_15
#define RIGHT_BTN_GPIO_Port GPIOF
#define RIGHT_BTN_EXTI_IRQn EXTI15_IRQn
#define HOME_BTN_Pin GPIO_PIN_0
#define HOME_BTN_GPIO_Port GPIOG
#define HOME_BTN_EXTI_IRQn EXTI0_IRQn
#define LEFT_BTN_Pin GPIO_PIN_13
#define LEFT_BTN_GPIO_Port GPIOD
#define LEFT_BTN_EXTI_IRQn EXTI13_IRQn
#define LCD_RES_Pin GPIO_PIN_3
#define LCD_RES_GPIO_Port GPIOD
#define LED_GREEN_Pin GPIO_PIN_12
#define LED_GREEN_GPIO_Port GPIOG
#define LED_YELLOW_Pin GPIO_PIN_13
#define LED_YELLOW_GPIO_Port GPIOG

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
