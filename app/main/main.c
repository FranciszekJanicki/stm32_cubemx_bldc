#include "main.h"
#include "gpio.h"
#include "stm32l4xx_hal.h"
#include "stm32l4xx_hal_conf.h"
#include "stm32l4xx_hal_gpio.h"
#include "stm32l4xx_hal_tim.h"
#include "tim.h"
#include <stdbool.h>
#include <stdint.h>

#define UH_GPIO BLDC_UH_GPIO_Port
#define UL_GPIO BLDC_UH_GPIO_Port
#define VH_GPIO BLDC_VH_GPIO_Port
#define VL_GPIO BLDC_VL_GPIO_Port
#define WH_GPIO BLDC_WH_GPIO_Port
#define WL_GPIO BLDC_WL_GPIO_Port

#define UH_PIN BLDC_UH_Pin
#define UL_PIN BLDC_UL_Pin
#define VH_PIN BLDC_VH_Pin
#define VL_PIN BLDC_VL_Pin
#define WH_PIN BLDC_WH_Pin
#define WL_PIN BLDC_WL_Pin

#define PWM_TIM TIM8

#define PWM_U_CHANNEL TIM_CHANNEL_1
#define PWM_W_CHANNEL TIM_CHANNEL_2
#define PWM_V_CHANNEL TIM_CHANNEL_3

#define HALL_TIM TIM4

#define PWM_U_TIM (&htim8)
#define PWM_V_TIM (&htim8)
#define PWM_W_TIM (&htim8)

#define HALL_1_GPIO BLDC_HALL_0_GPIO_Port
#define HALL_2_GPIO BLDC_HALL_1_GPIO_Port
#define HALL_3_GPIO BLDC_HALL_2_GPIO_Port

#define HALL_1_PIN BLDC_HALL_0_Pin
#define HALL_2_PIN BLDC_HALL_1_Pin
#define HALL_3_PIN BLDC_HALL_2_Pin

static inline void pwm_set_u(uint16_t value)
{
    __HAL_TIM_SET_COMPARE(PWM_U_TIM, PWM_U_CHANNEL, value & 0xFFFFU);
}

static inline void pwm_set_v(uint16_t value)
{
    __HAL_TIM_SET_COMPARE(PWM_V_TIM, PWM_V_CHANNEL, value & 0xFFFFU);
}

static inline void pwm_set_w(uint16_t value)
{
    __HAL_TIM_SET_COMPARE(PWM_W_TIM, PWM_W_CHANNEL, value & 0xFFFFU);
}

static inline void pwm_set_all(uint16_t value)
{
    pwm_set_u(value);
    pwm_set_v(value);
    pwm_set_w(value);
}

static inline void pwm_reset_UH()
{
    UH_GPIO->ODR &= ~UH_PIN;
}

static inline void pwm_set_UH()
{
    UH_GPIO->ODR |= UH_PIN;
}

static inline void pwm_reset_UL()
{
    UL_GPIO->ODR &= ~UL_PIN;
}

static inline void pwm_set_UL()
{
    UL_GPIO->ODR |= UL_PIN;
}

static inline void pwm_reset_VH()
{
    VH_GPIO->ODR &= ~VH_PIN;
}

static inline void pwm_set_VH()
{
    VH_GPIO->ODR |= VH_PIN;
}

static inline void pwm_reset_VL()
{
    VL_GPIO->ODR &= ~VL_PIN;
}

static inline void pwm_set_VL()
{
    VL_GPIO->ODR |= VL_PIN;
}

static inline void pwm_reset_WH()
{
    WH_GPIO->ODR &= ~WH_PIN;
}

static inline void pwm_set_WH()
{
    WH_GPIO->ODR |= WH_PIN;
}

static inline void pwm_reset_WL()
{
    WL_GPIO->ODR &= ~WL_PIN;
}

static inline void pwm_set_WL()
{
    WL_GPIO->ODR |= WL_PIN;
}

static inline void pwm_reset_all()
{
    pwm_reset_UH();
    pwm_reset_UL();
    pwm_reset_VH();
    pwm_reset_VL();
    pwm_reset_WH();
    pwm_reset_WL();
}

static bool hall_get_1(void)
{
    return (HALL_1_GPIO->IDR & HALL_1_PIN) ? 1 : 0;
}

static bool hall_get_2(void)
{
    return (HALL_2_GPIO->IDR & HALL_2_PIN) ? 1 : 0;
}

static bool hall_get_3(void)
{
    return (HALL_3_GPIO->IDR & HALL_3_PIN) ? 1 : 0;
}

typedef enum {
    BLDC_DIRECTION_FORWARD,
    BLDC_DIRECTION_BACKWARD,
} bldc_direction_t;

typedef enum {
    BLDC_POSITION_0 = 0b110,
    BLDC_POSITION_60 = 0b010,
    BLDC_POSITION_120 = 0b011,
    BLDC_POSITION_180 = 0b001,
    BLDC_POSITION_240 = 0b101,
    BLDC_POSITION_300 = 0b100,
} bldc_position_t;

#define BIPOLAR

static inline void bldc_commutation_forward_0(void)
{
#ifdef BIPOLAR
    pwm_set_UH();
    pwm_set_VL();
#else
    pwm_set_UH();
#endif
}

static inline void bldc_commutation_forward_60(void)
{
#ifdef BIPOLAR
    pwm_set_UH();
    pwm_set_WL();
#else
    pwm_set_UH();
#endif
}

static inline void bldc_commutation_forward_120(void)
{
#ifdef BIPOLAR
    pwm_set_VH();
    pwm_set_WL();
#else
    pwm_set_VH();
#endif
}

static inline void bldc_commutation_forward_180(void)
{
#ifdef BIPOLAR
    pwm_set_VH();
    pwm_set_UL();
#else
    pwm_set_VH();
#endif
}

static inline void bldc_commutation_forward_240(void)
{
#ifdef BIPOLAR
    pwm_set_WH();
    pwm_set_UL();
#else
    pwm_set_WH();
#endif
}

static inline void bldc_commutation_forward_300(void)
{
#ifdef BIPOLAR
    pwm_set_WH();
    pwm_set_VL();
#else
    pwm_set_WH();
#endif
}

static inline void bldc_commutation_backward_0(void)
{
#ifdef BIPOLAR
    pwm_set_VH();
    pwm_set_UL();
#else
    pwm_set_VH();
#endif
}

static inline void bldc_commutation_backward_60(void)
{
#ifdef BIPOLAR
    pwm_set_WH();
    pwm_set_UL();
#else
    pwm_set_WH();
#endif
}

static inline void bldc_commutation_backward_120(void)
{
#ifdef BIPOLAR
    pwm_set_WH();
    pwm_set_VL();
#else
    pwm_set_WH();
#endif
}

static inline void bldc_commutation_backward_180(void)
{
#ifdef BIPOLAR
    pwm_set_UH();
    pwm_set_VL();
#else
    pwm_set_UH();
#endif
}

static inline void bldc_commutation_backward_240(void)
{
#ifdef BIPOLAR
    pwm_set_UH();
    pwm_set_WL();
#else
    pwm_set_UH();
#endif
}

static inline void bldc_commutation_backward_300(void)
{
#ifdef BIPOLAR
    pwm_set_VH();
    pwm_set_WL();
#else
    pwm_set_VH();
#endif
}

typedef void (*bldc_commutation_t)(void);

static void bldc_commutation_forward(bldc_position_t position)
{
    static bldc_commutation_t commutations[] = {[BLDC_POSITION_0] = &bldc_commutation_forward_0,
                                                [BLDC_POSITION_60] = &bldc_commutation_forward_60,
                                                [BLDC_POSITION_120] = &bldc_commutation_forward_120,
                                                [BLDC_POSITION_180] = &bldc_commutation_forward_180,
                                                [BLDC_POSITION_240] = &bldc_commutation_forward_240,
                                                [BLDC_POSITION_300] = &bldc_commutation_forward_300};

    if ((position < 0b111) && (position > 0b000)) {
        (*commutations[position])();
    }
}

static void bldc_commutation_backward(bldc_position_t position)
{
    static bldc_commutation_t commutations[] = {[BLDC_POSITION_0] = &bldc_commutation_backward_0,
                                                [BLDC_POSITION_60] = &bldc_commutation_backward_60,
                                                [BLDC_POSITION_120] = &bldc_commutation_backward_120,
                                                [BLDC_POSITION_180] = &bldc_commutation_backward_180,
                                                [BLDC_POSITION_240] = &bldc_commutation_backward_240,
                                                [BLDC_POSITION_300] = &bldc_commutation_backward_300};

    if ((position < 0b111) && (position > 0b000)) {
        (*commutations[position])();
    }
}

static void bldc_commutation(bldc_direction_t direction)
{
    static uint8_t prev_hall = 0xFFU;

    // faza U -> hall 3
    uint8_t hall_u = hall_get_3();
    // faza V -> hall 1
    uint8_t hall_v = hall_get_1();
    // faza W -> hall 2
    uint8_t hall_w = hall_get_2();

    // bitwise or aby zebrac do 3 bitowej unsigned wartosci
    uint8_t hall = (hall_u << 2U) | (hall_v << 1U) | hall_w;

    if (hall != prev_hall) {
        pwm_reset_all();

        switch (direction) {
            case BLDC_DIRECTION_FORWARD:
                bldc_commutation_forward(hall);
                break;
            case BLDC_DIRECTION_BACKWARD:
                bldc_commutation_backward(hall);
                break;
            default:
                break;
        }
    }

    prev_hall = hall;
}

void HAL_TIMEx_HallSensor_Callback(TIM_HandleTypeDef* htim)
{
    if (htim->Instance == HALL_TIM) {
        bldc_commutation(BLDC_DIRECTION_FORWARD);
        HAL_TIMEx_HallSensor_Start_IT(htim);
    }
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef* htim)
{
    if (htim->Instance == PWM_TIM) {
        HAL_TIM_PWM_Start_IT(htim, PWM_U_CHANNEL | PWM_V_CHANNEL | PWM_W_CHANNEL);
    }
}

int main()
{
    HAL_Init();
    SystemClock_Config();

    MX_GPIO_Init();
    MX_TIM4_Init();
    MX_TIM8_Init();

    HAL_TIM_PWM_Start_IT(&htim8, TIM_CHANNEL_1 | TIM_CHANNEL_2 | TIM_CHANNEL_3);
    HAL_TIMEx_HallSensor_Start_IT(&htim4);

    while (1) {
    }
}
