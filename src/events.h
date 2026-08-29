#ifndef EVENTS_H
#define EVENTS_H

#include <stdint.h>
#include <stdbool.h>
#include <zephyr/zbus/zbus.h>

struct button_action {
    uint16_t key_code;
    bool is_pressed;
};

ZBUS_CHAN_DECLARE(input_chan);

#endif
