#include "main.h"
#include "gpio.h"
#include "stm32l4xx_hal.h"
#include "stm32l4xx_hal_conf.h"
#include "stm32l4xx_hal_gpio.h"
#include "stm32l4xx_hal_tim.h"
#include "tim.h"
#include <stdbool.h>
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

GPIO_InitTypeDef gpio_init = {};

static inline void gpio_write_uh(bool state)
{
    gpio_init.Pin = PWM_UH_PIN;
    gpio_init.Mode = MODE_OUTPUT | OUTPUT_PP;
    HAL_GPIO_Init(PWM_UH_PORT, &gpio_init);
    HAL_GPIO_WritePin(PWM_UH_PORT, PWM_UH_PIN, state);
}

static inline void gpio_write_ul(bool state)
{
    gpio_init.Pin = PWM_UL_PIN;
    gpio_init.Mode = MODE_OUTPUT | OUTPUT_PP;
    HAL_GPIO_Init(PWM_UL_PORT, &gpio_init);
    HAL_GPIO_WritePin(PWM_UL_PORT, PWM_UL_PIN, state);
}

static inline void gpio_write_vh(bool state)
{
    gpio_init.Pin = PWM_VH_PIN;
    gpio_init.Mode = MODE_OUTPUT | OUTPUT_PP;
    HAL_GPIO_Init(PWM_VH_PORT, &gpio_init);
    HAL_GPIO_WritePin(PWM_VH_PORT, PWM_VH_PIN, state);
}

static inline void gpio_write_vl(bool state)
{
    gpio_init.Pin = PWM_VL_PIN;
    gpio_init.Mode = MODE_OUTPUT | OUTPUT_PP;
    HAL_GPIO_Init(PWM_VL_PORT, &gpio_init);
    HAL_GPIO_WritePin(PWM_VL_PORT, PWM_VL_PIN, state);
}

static inline void gpio_write_wh(bool state)
{
    gpio_init.Pin = PWM_WH_PIN;
    gpio_init.Mode = MODE_OUTPUT | OUTPUT_PP;
    HAL_GPIO_Init(PWM_WH_PORT, &gpio_init);
    HAL_GPIO_WritePin(PWM_WH_PORT, PWM_WH_PIN, state);
}

static inline void gpio_write_wl(bool state)
{
    gpio_init.Pin = PWM_WL_PIN;
    gpio_init.Mode = MODE_OUTPUT | OUTPUT_PP;
    HAL_GPIO_Init(PWM_WL_PORT, &gpio_init);
    HAL_GPIO_WritePin(PWM_WL_PORT, PWM_WL_PIN, state);
}

static inline void tim_set_compare_uh(uint16_t compare)
{
    gpio_init.Pin = PWM_UH_PIN;
    gpio_init.Mode = MODE_AF | OUTPUT_PP;
    HAL_GPIO_Init(PWM_UH_PORT, &gpio_init);
    __HAL_TIM_SET_COMPARE(PWM_U_TIM, PWM_U_CHANNEL, compare);
}

static inline void tim_set_compare_ul(uint16_t compare)
{
    gpio_init.Pin = PWM_UL_PIN;
    gpio_init.Mode = MODE_AF | OUTPUT_PP;
    HAL_GPIO_Init(PWM_UL_PORT, &gpio_init);
    __HAL_TIM_SET_COMPARE(PWM_U_TIM, PWM_U_CHANNEL, compare);
}

static inline void tim_set_compare_vh(uint16_t compare)
{
    gpio_init.Pin = PWM_VH_PIN;
    gpio_init.Mode = MODE_AF | OUTPUT_PP;
    HAL_GPIO_Init(PWM_VH_PORT, &gpio_init);
    __HAL_TIM_SET_COMPARE(PWM_V_TIM, PWM_V_CHANNEL, compare);
}

static inline void tim_set_compare_vl(uint16_t compare)
{
    gpio_init.Pin = PWM_VL_PIN;
    gpio_init.Mode = MODE_AF | OUTPUT_PP;
    HAL_GPIO_Init(PWM_VL_PORT, &gpio_init);
    __HAL_TIM_SET_COMPARE(PWM_V_TIM, PWM_V_CHANNEL, compare);
}

static inline void tim_set_compare_wh(uint16_t compare)
{
    gpio_init.Pin = PWM_WH_PIN;
    gpio_init.Mode = MODE_AF | OUTPUT_PP;
    HAL_GPIO_Init(PWM_WH_PORT, &gpio_init);
    __HAL_TIM_SET_COMPARE(PWM_W_TIM, PWM_W_CHANNEL, compare);
}

static inline void tim_set_compare_wl(uint16_t compare)
{
    gpio_init.Pin = PWM_WL_PIN;
    gpio_init.Mode = MODE_AF | OUTPUT_PP;
    HAL_GPIO_Init(PWM_WL_PORT, &gpio_init);
    __HAL_TIM_SET_COMPARE(PWM_W_TIM, PWM_W_CHANNEL, compare);
}

static inline void pwm_conf_UH(uint16_t compare)
{
    tim_set_compare_uh(compare);
    gpio_write_ul(false);
}

static inline void pwm_conf_UL(uint16_t compare)
{
    tim_set_compare_ul(compare);
    gpio_write_uh(false);
}

static inline void pwm_conf_VH(uint16_t compare)
{
    tim_set_compare_vh(compare);
    gpio_write_vh(false);
}

static inline void pwm_conf_VL(uint16_t compare)
{
    tim_set_compare_ul(compare);
    gpio_write_ul(false);
}

static inline void pwm_conf_WH(uint16_t compare)
{
    tim_set_compare_wh(compare);
    gpio_write_wh(false);
}

static inline void pwm_conf_WL(uint16_t compare)
{
    tim_set_compare_wl(compare);
    gpio_write_wl(false);
}

static inline void pwm_set_UH(bool state)
{
    gpio_write_uh(state);
}

static inline void pwm_set_UL(bool state)
{
    gpio_write_ul(state);
}

static inline void pwm_set_VH(bool state)
{
    gpio_write_vh(state);
}

static inline void pwm_set_VL(bool state)
{
    gpio_write_vl(state);
}

static inline void pwm_set_WH(bool state)
{
    gpio_write_wh(state);
}

static inline void pwm_set_WL(bool state)
{
    gpio_write_wl(state);
}

static inline void pwm_set_all(bool state)
{
    pwm_set_UH(state);
    pwm_set_UL(state);
    pwm_set_VH(state);
    pwm_set_VL(state);
    pwm_set_WH(state);
    pwm_set_WL(state);
}

static inline bool gpio_read_h1(void)
{
    return HAL_GPIO_ReadPin(HALL_1_PORT, HALL_1_PIN);
}

static inline bool gpio_read_h2(void)
{
    return HAL_GPIO_ReadPin(HALL_2_PORT, HALL_2_PIN);
}

static inline bool gpio_read_h3(void)
{
    return HAL_GPIO_ReadPin(HALL_3_PORT, HALL_3_PIN);
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

#ifdef BLDC_BIPOLAR_CONTROL

static inline void bldc_commutation_forward_0(uint16_t compare)
{
    pwm_conf_WH(compare);
    pwm_conf_VL(compare);
}

static inline void bldc_commutation_forward_60(uint16_t compare)
{
    pwm_conf_UH(compare);
    pwm_conf_VL(compare);
}

static inline void bldc_commutation_forward_120(uint16_t compare)
{
    pwm_conf_UH(compare);
    pwm_conf_WL(compare);
}

static inline void bldc_commutation_forward_180(uint16_t compare)
{
    pwm_conf_VH(compare);
    pwm_conf_WL(compare);
}

static inline void bldc_commutation_forward_240(uint16_t compare)
{
    pwm_conf_VH(compare);
    pwm_conf_UL(compare);
}

static inline void bldc_commutation_forward_300(uint16_t compare)
{
    pwm_conf_WH(compare);
    pwm_conf_UL(compare);
}

static inline void bldc_commutation_backward_0(uint16_t compare)
{
    pwm_conf_VH(compare);
    pwm_conf_WL(compare);
}

static inline void bldc_commutation_backward_60(uint16_t compare)
{
    pwm_conf_VH(compare);
    pwm_conf_UL(compare);
}

static inline void bldc_commutation_backward_120(uint16_t compare)
{
    pwm_conf_WH(compare);
    pwm_conf_UL(compare);
}

static inline void bldc_commutation_backward_180(uint16_t compare)
{
    pwm_conf_WH(compare);
    pwm_conf_VL(compare);
}

static inline void bldc_commutation_backward_240(uint16_t compare)
{
    pwm_conf_UH(compare);
    pwm_conf_VL(compare);
}

static inline void bldc_commutation_backward_300(uint16_t compare)
{
    pwm_conf_UH(compare);
    pwm_conf_WL(compare);
}

#else

static inline void bldc_commutation_forward_0(uint16_t compare)
{
    pwm_conf_WH(compare);
    pwm_set_VL(true);
}

static inline void bldc_commutation_forward_60(uint16_t compare)
{
    pwm_conf_UH(compare);
    pwm_set_VL(true);
}

static inline void bldc_commutation_forward_120(uint16_t compare)
{
    pwm_conf_UH(compare);
    pwm_set_WL(true);
}

static inline void bldc_commutation_forward_180(uint16_t compare)
{
    pwm_conf_VH(compare);
    pwm_set_WL(true);
}

static inline void bldc_commutation_forward_240(uint16_t compare)
{
    pwm_conf_VH(compare);
    pwm_set_UL(true);
}

static inline void bldc_commutation_forward_300(uint16_t compare)
{
    pwm_conf_WH(compare);
    pwm_set_UL(true);
}

static inline void bldc_commutation_backward_0(uint16_t compare)
{
    pwm_conf_VH(compare);
    pwm_set_WL(true);
}

static inline void bldc_commutation_backward_60(uint16_t compare)
{
    pwm_conf_VH(compare);
    pwm_set_UL(true);
}

static inline void bldc_commutation_backward_120(uint16_t compare)
{
    pwm_conf_WH(compare);
    pwm_set_UL(true);
}

static inline void bldc_commutation_backward_180(uint16_t compare)
{
    pwm_conf_WH(compare);
    pwm_set_VL(true);
}

static inline void bldc_commutation_backward_240(uint16_t compare)
{
    pwm_conf_UH(compare);
    pwm_set_VL(true);
}

static inline void bldc_commutation_backward_300(uint16_t compare)
{
    pwm_conf_UH(compare);
    pwm_set_WL(true);
}

#endif

typedef void (*bldc_commutation_t)(uint16_t);

static void bldc_commutation_forward(bldc_position_t position, uint16_t compare)
{
    static bldc_commutation_t forward_commutations[] = {[BLDC_POSITION_0] = &bldc_commutation_forward_0,
                                                        [BLDC_POSITION_60] = &bldc_commutation_forward_60,
                                                        [BLDC_POSITION_120] = &bldc_commutation_forward_120,
                                                        [BLDC_POSITION_180] = &bldc_commutation_forward_180,
                                                        [BLDC_POSITION_240] = &bldc_commutation_forward_240,
                                                        [BLDC_POSITION_300] = &bldc_commutation_forward_300};

    if (position != 0b000 && position != 0b111) {
        forward_commutations[position](compare);
    }
}

static void bldc_commutation_backward(bldc_position_t position, uint16_t compare)
{
    static bldc_commutation_t backward_commutations[] = {[BLDC_POSITION_0] = &bldc_commutation_backward_0,
                                                         [BLDC_POSITION_60] = &bldc_commutation_backward_60,
                                                         [BLDC_POSITION_120] = &bldc_commutation_backward_120,
                                                         [BLDC_POSITION_180] = &bldc_commutation_backward_180,
                                                         [BLDC_POSITION_240] = &bldc_commutation_backward_240,
                                                         [BLDC_POSITION_300] = &bldc_commutation_backward_300};

    if (position != 0b000 && position != 0b111) {
        backward_commutations[position](compare);
    }
}

static void bldc_commutation(bldc_direction_t direction, uint16_t compare)
{
    static uint8_t prev_position = 0xFFU;

    // faza U -> hall 3
    uint8_t hall_u = gpio_read_h3();
    // faza V -> hall 1
    uint8_t hall_v = gpio_read_h1();
    // faza W -> hall 2
    uint8_t hall_w = gpio_read_h2();

    // bitwise OR aby zebrac do 3 bitowej unsigned wartosci
    uint8_t position = (hall_u << 2U) | (hall_v << 1U) | hall_w;

    if (position != prev_position) {
        pwm_set_all(false);

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
