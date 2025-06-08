#ifndef BLDC_BLDC_CONFIG_H
#define BLDC_BLDC_CONFIG_H

#include <stdint.h>

typedef enum {
    BLDC_ERR_OK = 0,
    BLDC_ERR_FAIL = 1 << 0,
    BLDC_ERR_NULL = 1 << 1,
} bldc_err_t;

typedef enum {
    BLDC_CHANNEL_1H,
    BLDC_CHANNEL_1L,
    BLDC_CHANNEL_2H,
    BLDC_CHANNEL_2L,
    BLDC_CHANNEL_3H,
    BLDC_CHANNEL_3L,
} bldc_channel_t;

typedef enum {
    BLDC_DIRECTION_FORWARD,
    BLDC_DIRECTION_BACKWARD,
    BLDC_DIRECTION_STOP,
} bldc_direction_t;

typedef enum {
    BLDC_POSITION_0 = 0b101,
    BLDC_POSITION_60 = 0b100,
    BLDC_POSITION_120 = 0b110,
    BLDC_POSITION_180 = 0b010,
    BLDC_POSITION_240 = 0b011,
    BLDC_POSITION_300 = 0b001,
} bldc_position_t;

typedef struct {
    uint32_t max_compare;
    uint32_t min_compare;
    float min_voltage;
    float max_voltage;
} bldc_config_t;

typedef struct {
    void* hall_user;
    bldc_err_t (*hall_init)(void*);
    bldc_err_t (*hall_deinit)(void*);
    bldc_err_t (*hall_get_position)(void*, bldc_position_t*);

    void* pwm_user;
    bldc_err_t (*pwm_init)(void*, bldc_channel_t);
    bldc_err_t (*pwm_deinit)(void*, bldc_channel_t);
    bldc_err_t (*pwm_start)(void*, bldc_channel_t);
    bldc_err_t (*pwm_stop)(void*, bldc_channel_t);
    bldc_err_t (*pwm_set_compare)(void*, bldc_channel_t, uint32_t);
} bldc_interface_t;

#endif // BLDC_BLDC_CONFIG_H