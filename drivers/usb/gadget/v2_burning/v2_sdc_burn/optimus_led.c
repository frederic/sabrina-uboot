/*
* Copyright (C) 2017 Amlogic, Inc. All rights reserved.
* *
This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
* *
This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
* more details.
* *
You should have received a copy of the GNU General Public License along
* with this program; if not, write to the Free Software Foundation, Inc.,
* 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
* *
Description:
*/

#include "../v2_burning_i.h"
#include "optimus_led.h"

#define OPTIMUS_LED_SRC_IS_PWM      1//pwm type led

typedef enum{
    OPTIMUS_LED_STATE_INVALID   = 0XF0  ,
    OPTIMUS_LED_STATE_RED               ,//Burning failed if stay on this state
    OPTIMUS_LED_STATE_GREEN             ,
    OPTIMUS_LED_STATE_SLOW_FLICKERING   ,
    OPTIMUS_LED_STATE_FAST_FLICKERING   ,//fast flickering to catch the eye that it's burning successful

}OptimusLedStates_e;

#if OPTIMUS_LED_SRC_IS_PWM
#define PWM_CHANNEL_INDEX   5

static int optimus_pwm_led_show_state(OptimusLedStates_e ledState)
{
    int rc = 0;
    int PwmHighLevelPeriod = 0;
    int PwmLowLevelPeriod  = 0;
    char cmdStr[64];

    switch (ledState)
    {
        case OPTIMUS_LED_STATE_SLOW_FLICKERING:
            {
                PwmHighLevelPeriod = 0xfffe;
                PwmLowLevelPeriod  = PwmHighLevelPeriod;

                sprintf(cmdStr, "pwm config %d %d %d", PWM_CHANNEL_INDEX, PwmHighLevelPeriod, PwmLowLevelPeriod);
            }
            break;

        case OPTIMUS_LED_STATE_FAST_FLICKERING:
            {
                PwmHighLevelPeriod = 0x2000;
                PwmLowLevelPeriod  = PwmHighLevelPeriod;

                sprintf(cmdStr, "pwm config %d %d %d", PWM_CHANNEL_INDEX, PwmHighLevelPeriod, PwmLowLevelPeriod);
            }
            break;

        case OPTIMUS_LED_STATE_GREEN:
            {
                PwmHighLevelPeriod = 0;
                PwmLowLevelPeriod  = 0xffffu;//always low level

                sprintf(cmdStr, "pwm config %d %d %d", PWM_CHANNEL_INDEX, PwmHighLevelPeriod, PwmLowLevelPeriod);
            }
            break;

        case OPTIMUS_LED_STATE_RED:
            {
                PwmHighLevelPeriod = 0xffffu;//always high level
                PwmLowLevelPeriod  = 0;

                sprintf(cmdStr, "pwm config %d %d %d", PWM_CHANNEL_INDEX, PwmHighLevelPeriod, PwmLowLevelPeriod);
            }
            break;
        default:
            DWN_ERR("invlaid pwm state %d\n", ledState);
            return __LINE__;
    }

    rc = run_command(cmdStr, 0);
    if (rc) {
        DWN_ERR("Fail in run_cmd[%s], ret=%d\n", cmdStr, rc);
        return __LINE__;
    }

    return 0;
}

int optimus_led_open(int ledType)
{
    const int clkSel    = 0;
    const int clkDiv    = 0x7f;
    const int pinIndex  = 0;
    char cmdStr[64];
    int rc = 0;

    sprintf(cmdStr, "pwm enable %d %d %d %d", PWM_CHANNEL_INDEX, pinIndex, clkSel, clkDiv);
    rc = run_command(cmdStr, 0);
    if (rc) {
        DWN_ERR("Fail in run_cmd[%s], ret=%d\n", cmdStr, rc);
        return __LINE__;
    }

    return 0;
}

int optimus_led_close(void)
{
    int rc = 0;
    char cmdStr[64];

    sprintf(cmdStr, "pwm disable %d", PWM_CHANNEL_INDEX);
    rc = run_command(cmdStr, 0);

    return rc;
}

int optimus_led_show_in_process_of_burning(void)
{
    return optimus_pwm_led_show_state(OPTIMUS_LED_STATE_SLOW_FLICKERING);
}

int optimus_led_show_burning_success(void)
{
    optimus_pwm_led_show_state(OPTIMUS_LED_STATE_FAST_FLICKERING);

    return 0;
}

int optimus_led_show_burning_failure(void)
{
    return optimus_pwm_led_show_state(OPTIMUS_LED_STATE_RED);
}

#else
int optimus_led_open(int ledType)
{
    return 0;
}

int optimus_led_close(void)
{
    return 0;
}

int optimus_led_show_in_process_of_burning(void)
{
    return 0;
}

int optimus_led_show_burning_success(void)
{
    return 0;
}
#endif// #if OPTIMUS_LED_SRC_IS_PWM


