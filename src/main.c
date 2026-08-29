#include <stdint.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/display.h>
#include <lvgl.h>

#include "events.h"

ZBUS_SUBSCRIBER_DEFINE(display_sub, 4);

ZBUS_CHAN_DEFINE(
        input_chan,
        struct button_action,
        NULL, NULL,
        ZBUS_OBSERVERS(display_sub),
        ZBUS_MSG_INIT(0)
);

ZBUS_CHAN_DEFINE(
        battery_chan,
        struct battery_action,
        NULL, NULL,
        ZBUS_OBSERVERS(display_sub),
        ZBUS_MSG_INIT(0)
);

extern lv_obj_t *battery_label;
extern lv_obj_t *buttons_label;

int main(void)
{
    struct button_action current_action;

    while (1) {
        if (battery_label == NULL || buttons_label == NULL)
            continue;

        const struct zbus_channel *chan;
        if (zbus_sub_wait(&display_sub, &chan, K_NO_WAIT) == 0) {
            if (chan == &input_chan) {
                zbus_chan_read(&input_chan, &current_action, K_NO_WAIT);
                switch (current_action.key_code) {
                    case BUTTON_A:
                        if (current_action.is_pressed) {
                            lv_label_set_text(buttons_label, "INPUT A");
                            lv_obj_remove_flag(buttons_label, LV_OBJ_FLAG_HIDDEN);
                        } else {
                            lv_obj_add_flag(buttons_label, LV_OBJ_FLAG_HIDDEN);
                        }
                        break;
                    case BUTTON_B:
                        if (current_action.is_pressed) {
                            lv_label_set_text(buttons_label, "INPUT B");
                            lv_obj_remove_flag(buttons_label, LV_OBJ_FLAG_HIDDEN);
                        } else {
                            lv_obj_add_flag(buttons_label, LV_OBJ_FLAG_HIDDEN);
                        }
                        break;
                    case BUTTON_C:
                        if (current_action.is_pressed) {
                            lv_label_set_text(buttons_label, "INPUT C");
                            lv_obj_remove_flag(buttons_label, LV_OBJ_FLAG_HIDDEN);
                        } else {
                            lv_obj_add_flag(buttons_label, LV_OBJ_FLAG_HIDDEN);
                        }
                        break;
                    case BUTTON_RESET:
                        if (current_action.is_pressed) {
                            lv_label_set_text(buttons_label, "INPUT R");
                            lv_obj_remove_flag(buttons_label, LV_OBJ_FLAG_HIDDEN);
                        } else {
                            lv_obj_add_flag(buttons_label, LV_OBJ_FLAG_HIDDEN);
                        }
                        break;
                }
            } else if (chan == &battery_chan) {
                struct battery_action action;
                zbus_chan_read(&battery_chan, &action, K_NO_WAIT);
                if (action.has_error)
                    lv_label_set_text(battery_label, "Bat: ERR");
                else
                    lv_label_set_text_fmt(battery_label, "Bat: %d%%", action.percentage);
            }
        }

        lv_task_handler();
        k_msleep(10);
    }
}
