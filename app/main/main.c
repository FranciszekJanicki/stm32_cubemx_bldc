#include "main.h"
#include "bldc.h"
#include "gpio.h"
#include "stm32l4xx_hal.h"
#include "stm32l4xx_hal_gpio.h"
#include "stm32l4xx_hal_tim.h"
#include "tim.h"
#include <stdint.h>

#define BLDC_BIPOLAR_CONTROL

#define PWM_UH_PORT BLDC_UH_GPIO_Port
#define PWM_UL_PORT BLDC_UL_GPIO_Port
#define PWM_VH_PORT BLDC_VH_GPIO_Port
#define PWM_VL_PORT BLDC_VL_GPIO_Port
#define PWM_WH_PORT BLDC_WH_GPIO_Port
#define PWM_WL_PORT BLDC_WL_GPIO_Port

#define PWM_UH_PIN BLDC_UH_Pin
#define PWM_UL_PIN BLDC_UL_Pin
#define PWM_VH_PIN BLDC_VH_Pin
#define PWM_VL_PIN BLDC_VL_Pin
#define PWM_WH_PIN BLDC_WH_Pin
#define PWM_WL_PIN BLDC_WL_Pin

#define PWM_U_CHANNEL TIM_CHANNEL_1
#define PWM_W_CHANNEL TIM_CHANNEL_2
#define PWM_V_CHANNEL TIM_CHANNEL_3

#define PWM_U_TIM (&htim8)
#define PWM_V_TIM (&htim8)
#define PWM_W_TIM (&htim8)

#define HALL_TIM (&htim4)

#define HALL_1_PORT BLDC_HALL_0_GPIO_Port
#define HALL_2_PORT BLDC_HALL_1_GPIO_Port
#define HALL_3_PORT BLDC_HALL_2_GPIO_Port

#define HALL_1_PIN BLDC_HALL_0_Pin
#define HALL_2_PIN BLDC_HALL_1_Pin
#define HALL_3_PIN BLDC_HALL_2_Pin

static inline void gpio_set_mode(GPIO_InitTypeDef* gpio_init, uint16_t pin_mask, uint32_t mode)
{
    gpio_init->Pin = pin_mask;
    gpio_init->Mode = mode;
    HAL_GPIO_Init(PWM_UH_PORT, gpio_init);
}

static inline void gpio_set_level(GPIO_TypeDef* gpio, uint16_t pin_mask, bool level)
{
    gpio->BSRR = (level ? (pin_mask << 16U) : pin_mask);
}

static inline bool gpio_get_level(GPIO_TypeDef* gpio, uint16_t pin_mask)
{
    return (gpio->IDR & pin_mask) ? true : false;
}

static inline void tim_set_compare(TIM_HandleTypeDef* htim, uint32_t channel_mask, uint16_t compare)
{
    __HAL_TIM_SET_COMPARE(htim, channel_mask, compare);
}

static bldc_t bldc = {};

void HAL_TIMEx_HallSensor_Callback(TIM_HandleTypeDef* htim)
{
    bldc_commutation(&bldc, BLDC_DIRECTION_FORWARD, 0x8000U);
}

int main()
{
    HAL_Init();
    SystemClock_Config();

    MX_GPIO_Init();
    MX_TIM4_Init();
    MX_TIM8_Init();

    HAL_TIM_PWM_Start_IT(PWM_U_TIM, PWM_U_CHANNEL);
    HAL_TIM_PWM_Start_IT(PWM_V_TIM, PWM_V_CHANNEL);
    HAL_TIM_PWM_Start_IT(PWM_W_TIM, PWM_W_CHANNEL);

    HAL_TIMEx_PWMN_Start_IT(PWM_U_TIM, PWM_U_CHANNEL);
    HAL_TIMEx_PWMN_Start_IT(PWM_V_TIM, PWM_V_CHANNEL);
    HAL_TIMEx_PWMN_Start_IT(PWM_W_TIM, PWM_W_CHANNEL);

    HAL_TIMEx_HallSensor_Start_IT(HALL_TIM);

    while (1) {
    }
}
