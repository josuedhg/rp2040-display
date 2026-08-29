#ifndef EVENTS_H
#define EVENTS_H

#include <stdint.h>
#include <stdbool.h>
#include <zephyr/zbus/zbus.h>
#include <zephyr/dt-bindings/input/input-event-codes.h>

struct button_action {
#define BUTTON_A INPUT_KEY_A
#define BUTTON_B INPUT_KEY_B
#define BUTTON_C INPUT_KEY_C
#define BUTTON_RESET INPUT_KEY_R
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
