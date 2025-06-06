/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
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
#include "stm32l4xx_hal.h"

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
void SystemClock_Config(void);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define BLDC_UL_Pin GPIO_PIN_0
#define BLDC_UL_GPIO_Port GPIOA
#define BLDC_UH_Pin GPIO_PIN_1
#define BLDC_UH_GPIO_Port GPIOA
#define BLDC_VL_Pin GPIO_PIN_2
#define BLDC_VL_GPIO_Port GPIOA
#define BLDC_VH_Pin GPIO_PIN_3
#define BLDC_VH_GPIO_Port GPIOA
#define BLDC_WL_Pin GPIO_PIN_4
#define BLDC_WL_GPIO_Port GPIOA
#define BLDC_WH_Pin GPIO_PIN_5
#define BLDC_WH_GPIO_Port GPIOA
#define BLDC_PWM_U_Pin GPIO_PIN_6
#define BLDC_PWM_U_GPIO_Port GPIOC
#define BLDC_PWM_V_Pin GPIO_PIN_7
#define BLDC_PWM_V_GPIO_Port GPIOC
#define BLDC_PWM_W_Pin GPIO_PIN_8
#define BLDC_PWM_W_GPIO_Port GPIOC
#define BLDC_HALL_0_Pin GPIO_PIN_6
#define BLDC_HALL_0_GPIO_Port GPIOB
#define BLDC_HALL_1_Pin GPIO_PIN_7
#define BLDC_HALL_1_GPIO_Port GPIOB
#define BLDC_HALL_2_Pin GPIO_PIN_8
#define BLDC_HALL_2_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
