/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "stm32l0xx_hal.h"

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
#define BAT_FB_Pin GPIO_PIN_0
#define BAT_FB_GPIO_Port GPIOA
#define BAT_FB_nEN_Pin GPIO_PIN_1
#define BAT_FB_nEN_GPIO_Port GPIOA
#define RADIO_EN_Pin GPIO_PIN_2
#define RADIO_EN_GPIO_Port GPIOA
#define RADIO_IRQ_Pin GPIO_PIN_3
#define RADIO_IRQ_GPIO_Port GPIOA
#define RADIO_IRQ_EXTI_IRQn EXTI2_3_IRQn
#define RADIO_nCS_Pin GPIO_PIN_4
#define RADIO_nCS_GPIO_Port GPIOA
#define SD_nCS_Pin GPIO_PIN_1
#define SD_nCS_GPIO_Port GPIOB
#define SD_nEN_Pin GPIO_PIN_8
#define SD_nEN_GPIO_Port GPIOA
#define SD_DET_Pin GPIO_PIN_9
#define SD_DET_GPIO_Port GPIOA
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define PWR_HOLD_Pin GPIO_PIN_15
#define PWR_HOLD_GPIO_Port GPIOA
#define PWR_nBTN_Pin GPIO_PIN_4
#define PWR_nBTN_GPIO_Port GPIOB
#define PWR_nBTN_EXTI_IRQn EXTI4_15_IRQn
#define LED_G_Pin GPIO_PIN_5
#define LED_G_GPIO_Port GPIOB
#define LED_Y_Pin GPIO_PIN_6
#define LED_Y_GPIO_Port GPIOB
#define LED_R_Pin GPIO_PIN_7
#define LED_R_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
