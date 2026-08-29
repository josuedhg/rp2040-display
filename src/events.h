#ifndef EVENTS_H
#define EVENTS_H

#include <stdint.h>
#include <stdbool.h>
#include <zephyr/zbus/zbus.h>

struct button_action {
    uint16_t key_code;
    bool is_pressed;
};

struct battery_action {
    uint16_t percentage;
    bool has_error;
};

ZBUS_CHAN_DECLARE(input_chan);
ZBUS_CHAN_DECLARE(battery_chan);

#endif
