#include "game_control.h"

void init_control(Control *control)
{
    control->target_steering = 0;
}

void modify_steering(Control *control, float delta_steering)
{
    control->target_steering += delta_steering * 0.005;
    if (control->target_steering > 1)
    {
        control->target_steering = 1;
    }
    if (control->target_steering < -1)
    {
        control->target_steering = -1;
    }
}