#include <stdint.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/input/input.h>

#include "events.h"

const struct device * const buttons = DEVICE_DT_GET(DT_NODELABEL(buttons));

static void input_cb(struct input_event *e, void *user_input)
{
    if (e->type == INPUT_EV_KEY) {
        struct button_action action = {
            .key_code = e->code,
            .is_pressed = (e->value == 1),
        };
        zbus_chan_pub(&input_chan, &action, K_NO_WAIT);
    }
}

INPUT_CALLBACK_DEFINE(buttons, input_cb, NULL);
