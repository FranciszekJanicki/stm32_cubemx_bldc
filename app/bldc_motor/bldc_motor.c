#include "bldc_motor.h"
#include <assert.h>
#include <string.h>

static bldc_motor_err_t bldc_motor_hall_init(bldc_motor_t const* bldc_motor)
{
    assert(bldc_motor);

    if (bldc_motor->interface.hall_init) {
        return bldc_motor->interface.hall_init(bldc_motor->interface.hall_user);
    }

    return BLDC_MOTOR_ERR_NULL;
}

static bldc_motor_err_t bldc_motor_hall_deinit(bldc_motor_t const* bldc_motor)
{
    assert(bldc_motor);

    if (bldc_motor->interface.hall_deinit) {
        return bldc_motor->interface.hall_deinit(bldc_motor->interface.hall_user);
    }

    return BLDC_MOTOR_ERR_NULL;
}

static bldc_motor_err_t bldc_motor_hall_get_position(bldc_motor_t const* bldc_motor,
                                                     bldc_motor_position_t* position)
{
    assert(bldc_motor);

    if (bldc_motor->interface.hall_get_position) {
        return bldc_motor->interface.hall_get_position(bldc_motor->interface.hall_user, position);
    }

    return BLDC_MOTOR_ERR_NULL;
}

static bldc_motor_err_t bldc_motor_pwm_init(bldc_motor_t const* bldc_motor,
                                            bldc_motor_channel_t channel)
{
    assert(bldc_motor);

    if (bldc_motor->interface.pwm_init) {
        return bldc_motor->interface.pwm_init(bldc_motor->interface.pwm_user, channel);
    }

    return BLDC_MOTOR_ERR_NULL;
}

static bldc_motor_err_t bldc_motor_pwm_deinit(bldc_motor_t const* bldc_motor,
                                              bldc_motor_channel_t channel)
{
    assert(bldc_motor);

    if (bldc_motor->interface.pwm_deinit) {
        return bldc_motor->interface.pwm_deinit(bldc_motor->interface.pwm_user, channel);
    }

    return BLDC_MOTOR_ERR_NULL;
}

static bldc_motor_err_t bldc_motor_pwm_start(bldc_motor_t const* bldc_motor,
                                             bldc_motor_channel_t channel)
{
    assert(bldc_motor);

    if (bldc_motor->interface.pwm_start) {
        return bldc_motor->interface.pwm_start(bldc_motor->interface.pwm_user, channel);
    }

    return BLDC_MOTOR_ERR_NULL;
}

static bldc_motor_err_t bldc_motor_pwm_stop(bldc_motor_t const* bldc_motor,
                                            bldc_motor_channel_t channel)
{
    assert(bldc_motor);

    if (bldc_motor->interface.pwm_stop) {
        return bldc_motor->interface.pwm_stop(bldc_motor->interface.pwm_user, channel);
    }

    return BLDC_MOTOR_ERR_NULL;
}

static bldc_motor_err_t bldc_motor_pwm_stop_all(bldc_motor_t const* bldc_motor)
{
    assert(bldc_motor);

    bldc_motor_err_t err = BLDC_MOTOR_ERR_OK;
    for (bldc_motor_channel_t channel = 0; channel < 6; ++channel) {
        err |= bldc_motor_pwm_stop(bldc_motor, channel);
    }

    return err;
}

static bldc_motor_err_t bldc_motor_pwm_set_compare(bldc_motor_t const* bldc_motor,
                                                   bldc_motor_channel_t channel,
                                                   uint32_t compare)
{
    assert(bldc_motor);

    if (bldc_motor->interface.pwm_set_compare) {
        return bldc_motor->interface.pwm_set_compare(bldc_motor->interface.pwm_user,
                                                     channel,
                                                     compare);
    }

    return BLDC_MOTOR_ERR_NULL;
}

static bldc_motor_err_t bldc_motor_commutation_forward_0(bldc_motor_t const* bldc_motor,
                                                         uint32_t compare)
{
    assert(bldc_motor);

    bldc_motor_err_t err = bldc_motor_pwm_start(bldc_motor, BLDC_MOTOR_CHANNEL_1H);
    err |= bldc_motor_pwm_start(bldc_motor, BLDC_MOTOR_CHANNEL_3L);
    err |= bldc_motor_pwm_set_compare(bldc_motor, BLDC_MOTOR_CHANNEL_1H, compare);
    err |= bldc_motor_pwm_set_compare(bldc_motor, BLDC_MOTOR_CHANNEL_3L, compare);

    return err;
}

static bldc_motor_err_t bldc_motor_commutation_forward_60(bldc_motor_t const* bldc_motor,
                                                          uint32_t compare)
{
    assert(bldc_motor);

    bldc_motor_err_t err = bldc_motor_pwm_start(bldc_motor, BLDC_MOTOR_CHANNEL_2H);
    err |= bldc_motor_pwm_start(bldc_motor, BLDC_MOTOR_CHANNEL_1L);
    err |= bldc_motor_pwm_set_compare(bldc_motor, BLDC_MOTOR_CHANNEL_2H, compare);
    err |= bldc_motor_pwm_set_compare(bldc_motor, BLDC_MOTOR_CHANNEL_1L, compare);

    return err;
}

static bldc_motor_err_t bldc_motor_commutation_forward_120(bldc_motor_t const* bldc_motor,
                                                           uint32_t compare)
{
    assert(bldc_motor);

    bldc_motor_err_t err = bldc_motor_pwm_start(bldc_motor, BLDC_MOTOR_CHANNEL_3H);
    err |= bldc_motor_pwm_start(bldc_motor, BLDC_MOTOR_CHANNEL_1L);
    err |= bldc_motor_pwm_set_compare(bldc_motor, BLDC_MOTOR_CHANNEL_3H, compare);
    err |= bldc_motor_pwm_set_compare(bldc_motor, BLDC_MOTOR_CHANNEL_1L, compare);

    return err;
}

static bldc_motor_err_t bldc_motor_commutation_forward_180(bldc_motor_t const* bldc_motor,
                                                           uint32_t compare)
{
    assert(bldc_motor);

    bldc_motor_err_t err = bldc_motor_pwm_start(bldc_motor, BLDC_MOTOR_CHANNEL_3H);
    err |= bldc_motor_pwm_start(bldc_motor, BLDC_MOTOR_CHANNEL_2L);
    err |= bldc_motor_pwm_set_compare(bldc_motor, BLDC_MOTOR_CHANNEL_3H, compare);
    err |= bldc_motor_pwm_set_compare(bldc_motor, BLDC_MOTOR_CHANNEL_2L, compare);

    return err;
}

static bldc_motor_err_t bldc_motor_commutation_forward_240(bldc_motor_t const* bldc_motor,
                                                           uint32_t compare)
{
    assert(bldc_motor);

    bldc_motor_err_t err = bldc_motor_pwm_start(bldc_motor, BLDC_MOTOR_CHANNEL_1H);
    err |= bldc_motor_pwm_start(bldc_motor, BLDC_MOTOR_CHANNEL_2L);
    err |= bldc_motor_pwm_set_compare(bldc_motor, BLDC_MOTOR_CHANNEL_1H, compare);
    err |= bldc_motor_pwm_set_compare(bldc_motor, BLDC_MOTOR_CHANNEL_2L, compare);

    return err;
}

static bldc_motor_err_t bldc_motor_commutation_forward_300(bldc_motor_t const* bldc_motor,
                                                           uint32_t compare)
{
    assert(bldc_motor);

    bldc_motor_err_t err = bldc_motor_pwm_start(bldc_motor, BLDC_MOTOR_CHANNEL_1H);
    err |= bldc_motor_pwm_start(bldc_motor, BLDC_MOTOR_CHANNEL_3L);
    err |= bldc_motor_pwm_set_compare(bldc_motor, BLDC_MOTOR_CHANNEL_1H, compare);
    err |= bldc_motor_pwm_set_compare(bldc_motor, BLDC_MOTOR_CHANNEL_3L, compare);

    return err;
}

static bldc_motor_err_t bldc_motor_commutation_backward_0(bldc_motor_t const* bldc_motor,
                                                          uint32_t compare)
{
    assert(bldc_motor);

    bldc_motor_err_t err = bldc_motor_pwm_start(bldc_motor, BLDC_MOTOR_CHANNEL_3H);
    err |= bldc_motor_pwm_start(bldc_motor, BLDC_MOTOR_CHANNEL_2L);
    err |= bldc_motor_pwm_set_compare(bldc_motor, BLDC_MOTOR_CHANNEL_3H, compare);
    err |= bldc_motor_pwm_set_compare(bldc_motor, BLDC_MOTOR_CHANNEL_2L, compare);

    return err;
}

static bldc_motor_err_t bldc_motor_commutation_backward_60(bldc_motor_t const* bldc_motor,
                                                           uint32_t compare)
{
    assert(bldc_motor);

    bldc_motor_err_t err = bldc_motor_pwm_start(bldc_motor, BLDC_MOTOR_CHANNEL_1H);
    err |= bldc_motor_pwm_start(bldc_motor, BLDC_MOTOR_CHANNEL_2L);
    err |= bldc_motor_pwm_set_compare(bldc_motor, BLDC_MOTOR_CHANNEL_1H, compare);
    err |= bldc_motor_pwm_set_compare(bldc_motor, BLDC_MOTOR_CHANNEL_2L, compare);

    return err;
}

static bldc_motor_err_t bldc_motor_commutation_backward_120(bldc_motor_t const* bldc_motor,
                                                            uint32_t compare)
{
    assert(bldc_motor);

    bldc_motor_err_t err = bldc_motor_pwm_start(bldc_motor, BLDC_MOTOR_CHANNEL_3H);
    err |= bldc_motor_pwm_start(bldc_motor, BLDC_MOTOR_CHANNEL_1L);
    err |= bldc_motor_pwm_set_compare(bldc_motor, BLDC_MOTOR_CHANNEL_3H, compare);
    err |= bldc_motor_pwm_set_compare(bldc_motor, BLDC_MOTOR_CHANNEL_1L, compare);

    return err;
}

bldc_motor_err_t bldc_motor_commutation_backward_180(bldc_motor_t const* bldc_motor,
                                                     uint32_t compare)
{
    assert(bldc_motor);

    bldc_motor_err_t err = bldc_motor_pwm_start(bldc_motor, BLDC_MOTOR_CHANNEL_2H);
    err |= bldc_motor_pwm_start(bldc_motor, BLDC_MOTOR_CHANNEL_3L);
    err |= bldc_motor_pwm_set_compare(bldc_motor, BLDC_MOTOR_CHANNEL_2H, compare);
    err |= bldc_motor_pwm_set_compare(bldc_motor, BLDC_MOTOR_CHANNEL_3L, compare);

    return err;
}

bldc_motor_err_t bldc_motor_commutation_backward_240(bldc_motor_t const* bldc_motor,
                                                     uint32_t compare)
{
    assert(bldc_motor);

    bldc_motor_err_t err = bldc_motor_pwm_start(bldc_motor, BLDC_MOTOR_CHANNEL_1H);
    err |= bldc_motor_pwm_start(bldc_motor, BLDC_MOTOR_CHANNEL_2L);
    err |= bldc_motor_pwm_set_compare(bldc_motor, BLDC_MOTOR_CHANNEL_1H, compare);
    err |= bldc_motor_pwm_set_compare(bldc_motor, BLDC_MOTOR_CHANNEL_2L, compare);

    return err;
}

static bldc_motor_err_t bldc_motor_commutation_backward_300(bldc_motor_t const* bldc_motor,
                                                            uint32_t compare)
{
    assert(bldc_motor);

    bldc_motor_err_t err = bldc_motor_pwm_start(bldc_motor, BLDC_MOTOR_CHANNEL_3H);
    err |= bldc_motor_pwm_start(bldc_motor, BLDC_MOTOR_CHANNEL_1L);
    err |= bldc_motor_pwm_set_compare(bldc_motor, BLDC_MOTOR_CHANNEL_3H, compare);
    err |= bldc_motor_pwm_set_compare(bldc_motor, BLDC_MOTOR_CHANNEL_1L, compare);

    return err;
}

bldc_motor_err_t bldc_motor_initialize(bldc_motor_t* bldc_motor,
                                       bldc_motor_config_t const* config,
                                       bldc_motor_interface_t const* interface)
{
    assert(bldc_motor && config && interface);

    memset(bldc_motor, 0, sizeof(*bldc_motor));
    memcpy(&bldc_motor->config, config, sizeof(*config));
    memcpy(&bldc_motor->interface, interface, sizeof(*interface));

    bldc_motor_err_t err = bldc_motor_hall_init(bldc_motor);
    for (bldc_motor_channel_t channel = 0; channel < 6; ++channel) {
        err |= bldc_motor_pwm_init(bldc_motor, channel);
    }

    return err;
}

bldc_motor_err_t bldc_motor_deinitialize(bldc_motor_t* bldc_motor)
{
    assert(bldc_motor);

    bldc_motor_err_t err = bldc_motor_hall_deinit(bldc_motor);
    for (bldc_motor_channel_t channel = 0; channel < 6; ++channel) {
        err |= bldc_motor_pwm_deinit(bldc_motor, channel);
    }

    memset(bldc_motor, 0, sizeof(*bldc_motor));

    return err;
}

bldc_motor_err_t bldc_motor_commutation(bldc_motor_t* bldc_motor,
                                        bldc_motor_direction_t direction,
                                        uint32_t compare)
{
    assert(bldc_motor);

    switch (direction) {
        case BLDC_MOTOR_DIRECTION_FORWARD:
            return bldc_motor_commutation_forward(bldc_motor, compare);
        case BLDC_MOTOR_DIRECTION_BACKWARD:
            return bldc_motor_commutation_backward(bldc_motor, compare);
        default:
            return BLDC_MOTOR_ERR_FAIL;
    }
}

static bldc_motor_err_t (*forward_commutations[])(bldc_motor_t const*, uint32_t) = {
    [BLDC_MOTOR_POSITION_0] = bldc_motor_commutation_forward_0,
    [BLDC_MOTOR_POSITION_60] = bldc_motor_commutation_forward_60,
    [BLDC_MOTOR_POSITION_120] = bldc_motor_commutation_forward_120,
    [BLDC_MOTOR_POSITION_180] = bldc_motor_commutation_forward_180,
    [BLDC_MOTOR_POSITION_240] = bldc_motor_commutation_forward_240,
    [BLDC_MOTOR_POSITION_300] = bldc_motor_commutation_forward_300};

bldc_motor_err_t bldc_motor_commutation_forward(bldc_motor_t* bldc_motor, uint32_t compare)
{
    assert(bldc_motor);

    bldc_motor_position_t position = {};
    bldc_motor_err_t err = bldc_motor_hall_get_position(bldc_motor, &position);

    if (position != bldc_motor->position && position > 0b000 && position < 0b111) {
        bldc_motor->position = position;

        err |= bldc_motor_pwm_stop_all(bldc_motor);
        err |= forward_commutations[bldc_motor->position](bldc_motor, compare);
    }

    return err;
}

static bldc_motor_err_t (*backward_commutations[])(bldc_motor_t const*, uint32_t) = {
    [BLDC_MOTOR_POSITION_0] = bldc_motor_commutation_backward_0,
    [BLDC_MOTOR_POSITION_60] = bldc_motor_commutation_backward_60,
    [BLDC_MOTOR_POSITION_120] = bldc_motor_commutation_backward_120,
    [BLDC_MOTOR_POSITION_180] = bldc_motor_commutation_backward_180,
    [BLDC_MOTOR_POSITION_240] = bldc_motor_commutation_backward_240,
    [BLDC_MOTOR_POSITION_300] = bldc_motor_commutation_backward_300};

bldc_motor_err_t bldc_motor_commutation_backward(bldc_motor_t* bldc_motor, uint32_t compare)
{
    assert(bldc_motor);

    bldc_motor_position_t position = {};
    bldc_motor_err_t err = bldc_motor_hall_get_position(bldc_motor, &position);

    if (position != bldc_motor->position && position > 0b000 && position < 0b111) {
        bldc_motor->position = position;

        err |= bldc_motor_pwm_stop_all(bldc_motor);
        err |= backward_commutations[bldc_motor->position](bldc_motor, compare);
    }

    return err;
}
