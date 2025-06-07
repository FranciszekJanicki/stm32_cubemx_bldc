#include "main.h"
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

GPIO_InitTypeDef init = {.Speed = GPIO_SPEED_FREQ_HIGH, .Alternate = GPIO_AF3_TIM8, .Pull = GPIO_NOPULL};

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

static inline void pwm_confUH(uint16_t compare)
{
    gpio_set_mode(&init, PWM_UH_PIN, GPIO_MODE_AF_PP);
    tim_set_compare(PWM_U_TIM, PWM_U_CHANNEL, compare);

    gpio_set_mode(&init, PWM_UL_PIN, GPIO_MODE_OUTPUT_PP);
    gpio_set_level(PWM_UL_PORT, PWM_UL_PIN, false);
}

static inline void pwm_confUL(uint16_t compare)
{
    gpio_set_mode(&init, PWM_UL_PIN, GPIO_MODE_AF_PP);
    tim_set_compare(PWM_U_TIM, PWM_U_CHANNEL, compare);

    gpio_set_mode(&init, PWM_UH_PIN, GPIO_MODE_OUTPUT_PP);
    gpio_set_level(PWM_UH_PORT, PWM_UH_PIN, false);
}

static inline void pwm_confVH(uint16_t compare)
{
    gpio_set_mode(&init, PWM_VH_PIN, GPIO_MODE_AF_PP);
    tim_set_compare(PWM_V_TIM, PWM_V_CHANNEL, compare);

    gpio_set_mode(&init, PWM_VL_PIN, GPIO_MODE_OUTPUT_PP);
    gpio_set_level(PWM_VL_PORT, PWM_VL_PIN, false);
}

static inline void pwm_confVL(uint16_t compare)
{
    gpio_set_mode(&init, PWM_VL_PIN, GPIO_MODE_AF_PP);
    tim_set_compare(PWM_V_TIM, PWM_V_CHANNEL, compare);

    gpio_set_mode(&init, PWM_VH_PIN, GPIO_MODE_OUTPUT_PP);
    gpio_set_level(PWM_VH_PORT, PWM_VH_PIN, false);
}

static inline void pwm_confWH(uint16_t compare)
{
    gpio_set_mode(&init, PWM_WH_PIN, GPIO_MODE_AF_PP);
    tim_set_compare(PWM_W_TIM, PWM_W_CHANNEL, compare);

    gpio_set_mode(&init, PWM_WL_PIN, GPIO_MODE_OUTPUT_PP);
    gpio_set_level(PWM_WL_PORT, PWM_WL_PIN, false);
}

static inline void pwm_confWL(uint16_t compare)
{
    gpio_set_mode(&init, PWM_WL_PIN, GPIO_MODE_AF_PP);
    tim_set_compare(PWM_W_TIM, PWM_W_CHANNEL, compare);

    gpio_set_mode(&init, PWM_WH_PIN, GPIO_MODE_OUTPUT_PP);
    gpio_set_level(PWM_WH_PORT, PWM_WH_PIN, false);
}

static inline void pwm_set_L_UH(void)
{
    gpio_set_mode(&init, PWM_UH_PIN, GPIO_MODE_OUTPUT_PP);
    gpio_set_level(PWM_UH_PORT, PWM_UH_PIN, false);
}

static inline void pwm_set_H_UH(void)
{
    gpio_set_mode(&init, PWM_UH_PIN, GPIO_MODE_OUTPUT_PP);
    gpio_set_level(PWM_UH_PORT, PWM_UH_PIN, true);
}

static inline void pwm_set_L_UL(void)
{
    gpio_set_mode(&init, PWM_UL_PIN, GPIO_MODE_OUTPUT_PP);
    gpio_set_level(PWM_UL_PORT, PWM_UL_PIN, false);
}

static inline void pwm_set_H_UL(void)
{
    gpio_set_mode(&init, PWM_UL_PIN, GPIO_MODE_OUTPUT_PP);
    gpio_set_level(PWM_UL_PORT, PWM_UL_PIN, true);
}

static inline void pwm_set_L_VH(void)
{
    gpio_set_mode(&init, PWM_VH_PIN, GPIO_MODE_OUTPUT_PP);
    gpio_set_level(PWM_VH_PORT, PWM_VH_PIN, false);
}

static inline void pwm_set_H_VH(void)
{
    gpio_set_mode(&init, PWM_VH_PIN, GPIO_MODE_OUTPUT_PP);
    gpio_set_level(PWM_VH_PORT, PWM_VH_PIN, true);
}

static inline void pwm_set_L_VL(void)
{
    gpio_set_mode(&init, PWM_VL_PIN, GPIO_MODE_OUTPUT_PP);
    gpio_set_level(PWM_VL_PORT, PWM_VL_PIN, false);
}

static inline void pwm_set_H_VL(void)
{
    gpio_set_mode(&init, PWM_VL_PIN, GPIO_MODE_OUTPUT_PP);
    gpio_set_level(PWM_VL_PORT, PWM_VL_PIN, true);
}

static inline void pwm_set_L_WH(void)
{
    gpio_set_mode(&init, PWM_WH_PIN, GPIO_MODE_OUTPUT_PP);
    gpio_set_level(PWM_WH_PORT, PWM_WH_PIN, false);
}

static inline void pwm_set_H_WH(void)
{
    gpio_set_mode(&init, PWM_WH_PIN, GPIO_MODE_OUTPUT_PP);
    gpio_set_level(PWM_WH_PORT, PWM_WH_PIN, true);
}

static inline void pwm_set_L_WL(void)
{
    gpio_set_mode(&init, PWM_WL_PIN, GPIO_MODE_OUTPUT_PP);
    gpio_set_level(PWM_WL_PORT, PWM_WL_PIN, false);
}

static inline void pwm_set_H_WL(void)
{
    gpio_set_mode(&init, PWM_WL_PIN, GPIO_MODE_OUTPUT_PP);
    gpio_set_level(PWM_WL_PORT, PWM_WL_PIN, true);
}

static inline void pwm_set_L_all(void)
{
    pwm_set_L_UH();
    pwm_set_L_UL();
    pwm_set_L_VH();
    pwm_set_L_VL();
    pwm_set_L_WH();
    pwm_set_L_WL();
}

static inline bool hall_read_H1(void)
{
    return gpio_get_level(HALL_1_PORT, HALL_1_PIN);
}

static inline bool hall_read_H2(void)
{
    return gpio_get_level(HALL_2_PORT, HALL_2_PIN);
}

static inline bool hall_read_H3(void)
{
    return gpio_get_level(HALL_3_PORT, HALL_3_PIN);
}

typedef enum {
    BLDC_DIRECTION_FORWARD,
    BLDC_DIRECTION_BACKWARD,
} bldc_direction_t;

typedef enum {
    BLDC_POSITION_0 = 0b101,
    BLDC_POSITION_60 = 0b100,
    BLDC_POSITION_120 = 0b110,
    BLDC_POSITION_180 = 0b010,
    BLDC_POSITION_240 = 0b011,
    BLDC_POSITION_300 = 0b001,
} bldc_position_t;

#ifdef BLDC_BIPOLAR_CONTROL

static inline void bldc_commutation_forward_0(uint16_t compare)
{
    pwm_confVH(compare);
    pwm_confWL(compare);
}

static inline void bldc_commutation_forward_60(uint16_t compare)
{
    pwm_confVH(compare);
    pwm_confUL(compare);
}

static inline void bldc_commutation_forward_120(uint16_t compare)
{
    pwm_confWH(compare);
    pwm_confUL(compare);
}

static inline void bldc_commutation_forward_180(uint16_t compare)
{
    pwm_confWH(compare);
    pwm_confVL(compare);
}

static inline void bldc_commutation_forward_240(uint16_t compare)
{
    pwm_confUH(compare);
    pwm_confVL(compare);
}

static inline void bldc_commutation_forward_300(uint16_t compare)
{
    pwm_confUH(compare);
    pwm_confWL(compare);
}

static inline void bldc_commutation_backward_0(uint16_t compare)
{
    pwm_confWH(compare);
    pwm_confVL(compare);
}

static inline void bldc_commutation_backward_60(uint16_t compare)
{
    pwm_confUH(compare);
    pwm_confVL(compare);
}

static inline void bldc_commutation_backward_120(uint16_t compare)
{
    pwm_confWH(compare);
    pwm_confUL(compare);
}

static inline void bldc_commutation_backward_180(uint16_t compare)
{
    pwm_confVH(compare);
    pwm_confWL(compare);
}

static inline void bldc_commutation_backward_240(uint16_t compare)
{
    pwm_confUH(compare);
    pwm_confVL(compare);
}

static inline void bldc_commutation_backward_300(uint16_t compare)
{
    pwm_confWH(compare);
    pwm_confUL(compare);
}

#else

static inline void bldc_commutation_forward_0(uint16_t compare)
{
    pwm_confVH(compare);
    pwm_set_H_WL();
}

static inline void bldc_commutation_forward_60(uint16_t compare)
{
    pwm_confVH(compare);
    pwm_set_H_UL();
}

static inline void bldc_commutation_forward_120(uint16_t compare)
{
    pwm_confWH(compare);
    pwm_set_H_UL();
}

static inline void bldc_commutation_forward_180(uint16_t compare)
{
    pwm_confWH(compare);
    pwm_set_H_VL();
}

static inline void bldc_commutation_forward_240(uint16_t compare)
{
    pwm_confUH(compare);
    pwm_set_H_VL();
}

static inline void bldc_commutation_forward_300(uint16_t compare)
{
    pwm_confUH(compare);
    pwm_set_H_WL();
}

static inline void bldc_commutation_backward_0(uint16_t compare)
{
    pwm_confWH(compare);
    pwm_set_H_VL();
}

static inline void bldc_commutation_backward_60(uint16_t compare)
{
    pwm_confUH(compare);
    pwm_set_H_VL();
}

static inline void bldc_commutation_backward_120(uint16_t compare)
{
    pwm_confWH(compare);
    pwm_set_H_UL();
}

static inline void bldc_commutation_backward_180(uint16_t compare)
{
    pwm_confVH(compare);
    pwm_set_H_WL();
}

static inline void bldc_commutation_backward_240(uint16_t compare)
{
    pwm_confUH(compare);
    pwm_set_H_VL();
}

static inline void bldc_commutation_backward_300(uint16_t compare)
{
    pwm_confWH(compare);
    pwm_set_H_UL();
}

#endif

typedef void (*bldc_commutation_t)(uint16_t);

static void bldc_commutation_forward(bldc_position_t position, uint16_t compare)
{
    static bldc_commutation_t forward_commutations[] = {
        [BLDC_POSITION_0] = &bldc_commutation_forward_0,
        [BLDC_POSITION_60] = &bldc_commutation_forward_60,
        [BLDC_POSITION_120] = &bldc_commutation_forward_120,
        [BLDC_POSITION_180] = &bldc_commutation_forward_180,
        [BLDC_POSITION_240] = &bldc_commutation_forward_240,
        [BLDC_POSITION_300] = &bldc_commutation_forward_300,
    };

    if (position > 0b000 && position < 0b111) {
        forward_commutations[position](compare);
    }
}

static void bldc_commutation_backward(bldc_position_t position, uint16_t compare)
{
    static bldc_commutation_t backward_commutations[] = {
        [BLDC_POSITION_0] = &bldc_commutation_backward_0,
        [BLDC_POSITION_60] = &bldc_commutation_backward_60,
        [BLDC_POSITION_120] = &bldc_commutation_backward_120,
        [BLDC_POSITION_180] = &bldc_commutation_backward_180,
        [BLDC_POSITION_240] = &bldc_commutation_backward_240,
        [BLDC_POSITION_300] = &bldc_commutation_backward_300,
    };

    if (position > 0b000 && position < 0b111) {
        backward_commutations[position](compare);
    }
}

static void bldc_commutation(bldc_direction_t direction, uint16_t compare)
{
    static uint8_t prev_position = 0xFFU;

    uint8_t hall_1 = hall_read_H1() & 0x01U;
    uint8_t hall_2 = hall_read_H2() & 0x01U;
    uint8_t hall_3 = hall_read_H3() & 0x01U;
    uint8_t position = (hall_3 << 2U) | (hall_2 << 1U) | hall_1;

    if (position != prev_position) {
        pwm_set_L_all();

        switch (direction) {
            case BLDC_DIRECTION_FORWARD:
                bldc_commutation_forward(position, compare);
                break;
            case BLDC_DIRECTION_BACKWARD:
                bldc_commutation_backward(position, compare);
                break;
            default:
                break;
        }
    }

    prev_position = position;
}

void HAL_TIMEx_HallSensor_Callback(TIM_HandleTypeDef* htim)
{
    bldc_commutation(BLDC_DIRECTION_FORWARD, 0x8000U);
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
