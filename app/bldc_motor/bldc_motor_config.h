#ifndef BLDC_MOTOR_BLDC_MOTOR_CONFIG_H
#define BLDC_MOTOR_BLDC_MOTOR_CONFIG_H

#include <stdint.h>

typedef enum {
    BLDC_MOTOR_ERR_OK = 0,
    BLDC_MOTOR_ERR_FAIL = 1 << 0,
    BLDC_MOTOR_ERR_NULL = 1 << 1,
} bldc_motor_err_t;

typedef enum {
    BLDC_MOTOR_CHANNEL_1H,
    BLDC_MOTOR_CHANNEL_1L,
    BLDC_MOTOR_CHANNEL_2H,
    BLDC_MOTOR_CHANNEL_2L,
    BLDC_MOTOR_CHANNEL_3H,
    BLDC_MOTOR_CHANNEL_3L,
} bldc_motor_channel_t;

typedef enum {
    BLDC_MOTOR_DIRECTION_FORWARD,
    BLDC_MOTOR_DIRECTION_BACKWARD,
    BLDC_MOTOR_DIRECTION_STOP,
} bldc_motor_direction_t;

typedef enum {
    BLDC_MOTOR_POSITION_0 = 0b101,
    BLDC_MOTOR_POSITION_60 = 0b100,
    BLDC_MOTOR_POSITION_120 = 0b110,
    BLDC_MOTOR_POSITION_180 = 0b010,
    BLDC_MOTOR_POSITION_240 = 0b011,
    BLDC_MOTOR_POSITION_300 = 0b001,
} bldc_motor_position_t;

typedef struct {
    uint32_t max_compare;
    uint32_t min_compare;
    float min_voltage;
    float max_voltage;
} bldc_motor_config_t;

typedef struct {
    void* hall_user;
    bldc_motor_err_t (*hall_init)(void*);
    bldc_motor_err_t (*hall_deinit)(void*);
    bldc_motor_err_t (*hall_get_position)(void*, bldc_motor_position_t*);

    void* pwm_user;
    bldc_motor_err_t (*pwm_init)(void*, bldc_motor_channel_t);
    bldc_motor_err_t (*pwm_deinit)(void*, bldc_motor_channel_t);
    bldc_motor_err_t (*pwm_start)(void*, bldc_motor_channel_t);
    bldc_motor_err_t (*pwm_stop)(void*, bldc_motor_channel_t);
    bldc_motor_err_t (*pwm_set_compare)(void*, bldc_motor_channel_t, uint32_t);
} bldc_motor_interface_t;

#endif // BLDC_MOTOR_BLDC_MOTOR_CONFIG_H