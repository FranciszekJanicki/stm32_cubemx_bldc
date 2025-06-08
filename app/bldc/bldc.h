#ifndef BLDC_BLDC_H
#define BLDC_BLDC_H

#include "bldc_config.h"

typedef struct {
    bldc_config_t config;
    bldc_interface_t interface;

    bldc_direction_t direction;
    uint32_t compare;
    bldc_position_t position;
} bldc_t;

bldc_err_t bldc_initialize(bldc_t* bldc, bldc_config_t const* config, bldc_interface_t const* interface);
bldc_err_t bldc_deinitialize(bldc_t* bldc);

bldc_err_t bldc_commutation(bldc_t* bldc, bldc_direction_t direction, uint32_t compare);
bldc_err_t bldc_commutation_forward(bldc_t* bldc, uint32_t compare);
bldc_err_t bldc_commutation_backward(bldc_t* bldc, uint32_t compare);

#endif // BLDC_BLDC_H