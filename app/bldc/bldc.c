#include "bldc.h"
#include <assert.h>
#include <string.h>

static bldc_err_t bldc_hall_init(bldc_t const* bldc)
{
    assert(bldc);

    if (bldc->interface.hall_init) {
        return bldc->interface.hall_init(bldc->interface.hall_user);
    }

    return BLDC_ERR_NULL;
}

static bldc_err_t bldc_hall_deinit(bldc_t const* bldc)
{
    assert(bldc);

    if (bldc->interface.hall_deinit) {
        return bldc->interface.hall_deinit(bldc->interface.hall_user);
    }

    return BLDC_ERR_NULL;
}

static bldc_err_t bldc_hall_get_position(bldc_t const* bldc, bldc_position_t* position)
{
    assert(bldc);

    if (bldc->interface.hall_get_position) {
        return bldc->interface.hall_get_position(bldc->interface.hall_user, position);
    }

    return BLDC_ERR_NULL;
}

static bldc_err_t bldc_pwm_init(bldc_t const* bldc, bldc_channel_t channel)
{
    assert(bldc);

    if (bldc->interface.pwm_init) {
        return bldc->interface.pwm_init(bldc->interface.pwm_user, channel);
    }

    return BLDC_ERR_NULL;
}

static bldc_err_t bldc_pwm_deinit(bldc_t const* bldc, bldc_channel_t channel)
{
    assert(bldc);

    if (bldc->interface.pwm_deinit) {
        return bldc->interface.pwm_deinit(bldc->interface.pwm_user, channel);
    }

    return BLDC_ERR_NULL;
}

static bldc_err_t bldc_pwm_start(bldc_t const* bldc, bldc_channel_t channel)
{
    assert(bldc);

    if (bldc->interface.pwm_start) {
        return bldc->interface.pwm_start(bldc->interface.pwm_user, channel);
    }

    return BLDC_ERR_NULL;
}

static bldc_err_t bldc_pwm_stop(bldc_t const* bldc, bldc_channel_t channel)
{
    assert(bldc);

    if (bldc->interface.pwm_stop) {
        return bldc->interface.pwm_stop(bldc->interface.pwm_user, channel);
    }

    return BLDC_ERR_NULL;
}

static bldc_err_t bldc_pwm_stop_all(bldc_t const* bldc)
{
    assert(bldc);

    bldc_err_t err = BLDC_ERR_OK;
    for (bldc_channel_t channel = 0; channel < 6; ++channel) {
        err |= bldc_pwm_stop(bldc, channel);
    }

    return err;
}

static bldc_err_t bldc_pwm_set_compare(bldc_t const* bldc, bldc_channel_t channel, uint32_t compare)
{
    assert(bldc);

    if (bldc->interface.pwm_set_compare) {
        return bldc->interface.pwm_set_compare(bldc->interface.pwm_user, channel, compare);
    }

    return BLDC_ERR_NULL;
}

static bldc_err_t bldc_commutation_forward_0(bldc_t const* bldc, uint32_t compare)
{
    assert(bldc);

    bldc_err_t err = bldc_pwm_start(bldc, BLDC_CHANNEL_1H);
    err |= bldc_pwm_start(bldc, BLDC_CHANNEL_3L);
    err |= bldc_pwm_set_compare(bldc, BLDC_CHANNEL_1H, compare);
    err |= bldc_pwm_set_compare(bldc, BLDC_CHANNEL_3L, compare);

    return err;
}

static bldc_err_t bldc_commutation_forward_60(bldc_t const* bldc, uint32_t compare)
{
    assert(bldc);

    bldc_err_t err = bldc_pwm_start(bldc, BLDC_CHANNEL_2H);
    err |= bldc_pwm_start(bldc, BLDC_CHANNEL_1L);
    err |= bldc_pwm_set_compare(bldc, BLDC_CHANNEL_2H, compare);
    err |= bldc_pwm_set_compare(bldc, BLDC_CHANNEL_1L, compare);

    return err;
}

static bldc_err_t bldc_commutation_forward_120(bldc_t const* bldc, uint32_t compare)
{
    assert(bldc);

    bldc_err_t err = bldc_pwm_start(bldc, BLDC_CHANNEL_3H);
    err |= bldc_pwm_start(bldc, BLDC_CHANNEL_1L);
    err |= bldc_pwm_set_compare(bldc, BLDC_CHANNEL_3H, compare);
    err |= bldc_pwm_set_compare(bldc, BLDC_CHANNEL_1L, compare);

    return err;
}

static bldc_err_t bldc_commutation_forward_180(bldc_t const* bldc, uint32_t compare)
{
    assert(bldc);

    bldc_err_t err = bldc_pwm_start(bldc, BLDC_CHANNEL_3H);
    err |= bldc_pwm_start(bldc, BLDC_CHANNEL_2L);
    err |= bldc_pwm_set_compare(bldc, BLDC_CHANNEL_3H, compare);
    err |= bldc_pwm_set_compare(bldc, BLDC_CHANNEL_2L, compare);

    return err;
}

static bldc_err_t bldc_commutation_forward_240(bldc_t const* bldc, uint32_t compare)
{
    assert(bldc);

    bldc_err_t err = bldc_pwm_start(bldc, BLDC_CHANNEL_1H);
    err |= bldc_pwm_start(bldc, BLDC_CHANNEL_2L);
    err |= bldc_pwm_set_compare(bldc, BLDC_CHANNEL_1H, compare);
    err |= bldc_pwm_set_compare(bldc, BLDC_CHANNEL_2L, compare);

    return err;
}

static bldc_err_t bldc_commutation_forward_300(bldc_t const* bldc, uint32_t compare)
{
    assert(bldc);

    bldc_err_t err = bldc_pwm_start(bldc, BLDC_CHANNEL_1H);
    err |= bldc_pwm_start(bldc, BLDC_CHANNEL_3L);
    err |= bldc_pwm_set_compare(bldc, BLDC_CHANNEL_1H, compare);
    err |= bldc_pwm_set_compare(bldc, BLDC_CHANNEL_3L, compare);

    return err;
}

static bldc_err_t bldc_commutation_backward_0(bldc_t const* bldc, uint32_t compare)
{
    assert(bldc);

    bldc_err_t err = bldc_pwm_start(bldc, BLDC_CHANNEL_3H);
    err |= bldc_pwm_start(bldc, BLDC_CHANNEL_2L);
    err |= bldc_pwm_set_compare(bldc, BLDC_CHANNEL_3H, compare);
    err |= bldc_pwm_set_compare(bldc, BLDC_CHANNEL_2L, compare);

    return err;
}

static bldc_err_t bldc_commutation_backward_60(bldc_t const* bldc, uint32_t compare)
{
    assert(bldc);

    bldc_err_t err = bldc_pwm_start(bldc, BLDC_CHANNEL_1H);
    err |= bldc_pwm_start(bldc, BLDC_CHANNEL_2L);
    err |= bldc_pwm_set_compare(bldc, BLDC_CHANNEL_1H, compare);
    err |= bldc_pwm_set_compare(bldc, BLDC_CHANNEL_2L, compare);

    return err;
}

static bldc_err_t bldc_commutation_backward_120(bldc_t const* bldc, uint32_t compare)
{
    assert(bldc);

    bldc_err_t err = bldc_pwm_start(bldc, BLDC_CHANNEL_3H);
    err |= bldc_pwm_start(bldc, BLDC_CHANNEL_1L);
    err |= bldc_pwm_set_compare(bldc, BLDC_CHANNEL_3H, compare);
    err |= bldc_pwm_set_compare(bldc, BLDC_CHANNEL_1L, compare);

    return err;
}

bldc_err_t bldc_commutation_backward_180(bldc_t const* bldc, uint32_t compare)
{
    assert(bldc);

    bldc_err_t err = bldc_pwm_start(bldc, BLDC_CHANNEL_2H);
    err |= bldc_pwm_start(bldc, BLDC_CHANNEL_3L);
    err |= bldc_pwm_set_compare(bldc, BLDC_CHANNEL_2H, compare);
    err |= bldc_pwm_set_compare(bldc, BLDC_CHANNEL_3L, compare);

    return err;
}

bldc_err_t bldc_commutation_backward_240(bldc_t const* bldc, uint32_t compare)
{
    assert(bldc);

    bldc_err_t err = bldc_pwm_start(bldc, BLDC_CHANNEL_1H);
    err |= bldc_pwm_start(bldc, BLDC_CHANNEL_2L);
    err |= bldc_pwm_set_compare(bldc, BLDC_CHANNEL_1H, compare);
    err |= bldc_pwm_set_compare(bldc, BLDC_CHANNEL_2L, compare);

    return err;
}

static bldc_err_t bldc_commutation_backward_300(bldc_t const* bldc, uint32_t compare)
{
    assert(bldc);

    bldc_err_t err = bldc_pwm_start(bldc, BLDC_CHANNEL_3H);
    err |= bldc_pwm_start(bldc, BLDC_CHANNEL_1L);
    err |= bldc_pwm_set_compare(bldc, BLDC_CHANNEL_3H, compare);
    err |= bldc_pwm_set_compare(bldc, BLDC_CHANNEL_1L, compare);

    return err;
}

bldc_err_t bldc_initialize(bldc_t* bldc, bldc_config_t const* config, bldc_interface_t const* interface)
{
    assert(bldc && config && interface);

    memset(bldc, 0, sizeof(*bldc));
    memcpy(&bldc->config, config, sizeof(*config));
    memcpy(&bldc->interface, interface, sizeof(*interface));

    bldc_err_t err = bldc_hall_init(bldc);
    for (bldc_channel_t channel = 0; channel < 6; ++channel) {
        err |= bldc_pwm_init(bldc, channel);
    }

    return err;
}

bldc_err_t bldc_deinitialize(bldc_t* bldc)
{
    assert(bldc);

    bldc_err_t err = bldc_hall_deinit(bldc);
    for (bldc_channel_t channel = 0; channel < 6; ++channel) {
        err |= bldc_pwm_deinit(bldc, channel);
    }

    memset(bldc, 0, sizeof(*bldc));

    return err;
}

bldc_err_t bldc_commutation(bldc_t* bldc, bldc_direction_t direction, uint32_t compare)
{
    assert(bldc);

    switch (direction) {
        case BLDC_DIRECTION_FORWARD:
            return bldc_commutation_forward(bldc, compare);
        case BLDC_DIRECTION_BACKWARD:
            return bldc_commutation_backward(bldc, compare);
        default:
            return BLDC_ERR_FAIL;
    }
}

static bldc_err_t (*forward_commutations[])(bldc_t const*,
                                            uint32_t) = {[BLDC_POSITION_0] = bldc_commutation_forward_0,
                                                         [BLDC_POSITION_60] = bldc_commutation_forward_60,
                                                         [BLDC_POSITION_120] = bldc_commutation_forward_120,
                                                         [BLDC_POSITION_180] = bldc_commutation_forward_180,
                                                         [BLDC_POSITION_240] = bldc_commutation_forward_240,
                                                         [BLDC_POSITION_300] = bldc_commutation_forward_300};

bldc_err_t bldc_commutation_forward(bldc_t* bldc, uint32_t compare)
{
    assert(bldc);

    bldc_position_t position = {};
    bldc_err_t err = bldc_hall_get_position(bldc, &position);

    if (position != bldc->position && position > 0b000 && position < 0b111) {
        bldc->position = position;

        err |= bldc_pwm_stop_all(bldc);
        err |= forward_commutations[bldc->position](bldc, compare);
    }

    return err;
}

static bldc_err_t (*backward_commutations[])(bldc_t const*,
                                             uint32_t) = {[BLDC_POSITION_0] = bldc_commutation_backward_0,
                                                          [BLDC_POSITION_60] = bldc_commutation_backward_60,
                                                          [BLDC_POSITION_120] = bldc_commutation_backward_120,
                                                          [BLDC_POSITION_180] = bldc_commutation_backward_180,
                                                          [BLDC_POSITION_240] = bldc_commutation_backward_240,
                                                          [BLDC_POSITION_300] = bldc_commutation_backward_300};

bldc_err_t bldc_commutation_backward(bldc_t* bldc, uint32_t compare)
{
    assert(bldc);

    bldc_position_t position = {};
    bldc_err_t err = bldc_hall_get_position(bldc, &position);

    if (position != bldc->position && position > 0b000 && position < 0b111) {
        bldc->position = position;

        err |= bldc_pwm_stop_all(bldc);
        err |= backward_commutations[bldc->position](bldc, compare);
    }

    return err;
}
