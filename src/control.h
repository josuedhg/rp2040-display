#ifndef CONTROL_H
#define CONTROL_H

#include <zephyr/kernel.h>

struct button_action {
    uint16_t key_code;
    bool is_pressed;
};

extern struct k_msgq btn_msgq;

#endif
