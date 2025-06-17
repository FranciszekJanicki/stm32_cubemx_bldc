#ifndef BLDC_MOTOR_BLDC_MOTOR_H
#define BLDC_MOTOR_BLDC_MOTOR_H

#include "bldc_motor_config.h"

typedef struct {
    bldc_motor_config_t config;
    bldc_motor_interface_t interface;

    uint32_t compare;
    bldc_motor_direction_t direction;
    bldc_motor_position_t position;
} bldc_motor_t;

bldc_motor_err_t bldc_motor_initialize(bldc_motor_t* bldc_motor,
                                       bldc_motor_config_t const* config,
                                       bldc_motor_interface_t const* interface);
bldc_motor_err_t bldc_motor_deinitialize(bldc_motor_t* bldc_motor);

bldc_motor_err_t bldc_motor_commutation(bldc_motor_t* bldc_motor,
                                        bldc_motor_direction_t direction,
                                        uint32_t compare);
bldc_motor_err_t bldc_motor_commutation_forward(bldc_motor_t* bldc_motor, uint32_t compare);
bldc_motor_err_t bldc_motor_commutation_backward(bldc_motor_t* bldc_motor, uint32_t compare);

#endif // BLDC_MOTOR_BLDC_MOTOR_H