#ifndef CONTROL_H
#define CONTROL_H

typedef struct Control
{
    float target_steering;
} Control;

void init_control(Control *control);

void modify_steering(Control *control, float delta_steering);

#endif // CONTROL_H
