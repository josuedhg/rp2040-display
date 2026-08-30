#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/display.h>
#include <lvgl.h>

#include "events.h"

LV_IMAGE_DECLARE(my_bg);

lv_obj_t *battery_label = NULL;
lv_obj_t *buttons_label = NULL;

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

static void _hmi_thread(void *p1, void *p2, void *p3)
{
    while (1) {
        if (battery_label == NULL || buttons_label == NULL)
            continue;
        const struct zbus_channel *chan;
        if (zbus_sub_wait(&display_sub, &chan, K_MSEC(50)) == 0) {
            if (chan == &input_chan) {
                struct button_action current_action;
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

    }
}
K_THREAD_DEFINE(hmi_thread, 2048, _hmi_thread, NULL, NULL, NULL, 101, 0, 50);

int hmi_init(void)
{
    const struct device *display_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
    if (!device_is_ready(display_dev)) {
        return -1;
    }

    display_blanking_off(display_dev);
    lv_obj_t *bg_img = lv_image_create(lv_screen_active());

    lv_image_set_src(bg_img, &my_bg);
    lv_obj_center(bg_img);

    // Battery label
    battery_label = lv_label_create(lv_screen_active());
    lv_obj_set_style_text_align(battery_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_color(battery_label, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_align(battery_label, LV_ALIGN_TOP_RIGHT, -2, 2);
    lv_label_set_text(battery_label, "Bat: N/A");

    // Button label
    buttons_label = lv_label_create(lv_screen_active());
    lv_obj_set_style_text_align(buttons_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_color(buttons_label, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_align(buttons_label, LV_ALIGN_CENTER, 0, 0);

    lv_obj_add_flag(buttons_label, LV_OBJ_FLAG_HIDDEN);

    return 0;
}

SYS_INIT(hmi_init, APPLICATION, 100);
