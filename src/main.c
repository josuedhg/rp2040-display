#include <stdint.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/display.h>
#include <zephyr/input/input.h>
#include <zephyr/drivers/fuel_gauge.h>
#include <lvgl.h>

#include "events.h"

LV_IMAGE_DECLARE(my_bg);

ZBUS_SUBSCRIBER_DEFINE(display_sub, 4);

ZBUS_CHAN_DEFINE(
        input_chan,
        struct button_action,
        NULL, NULL,
        ZBUS_OBSERVERS(display_sub),
        ZBUS_MSG_INIT(0)
);

const struct device *gauge_dev = DEVICE_DT_GET_ANY(maxim_max17048);

int main(void)
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
    lv_obj_t *battery_label = lv_label_create(lv_screen_active());
    lv_obj_set_style_text_align(battery_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_color(battery_label, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_align(battery_label, LV_ALIGN_TOP_RIGHT, -2, 2);
    lv_label_set_text(battery_label, "Bat: --%");

    if (!device_is_ready(gauge_dev)) {
        lv_label_set_text(battery_label, "Bat: N/A");
    }

    // Button label
    lv_obj_t *label = lv_label_create(lv_screen_active());
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

    lv_obj_add_flag(label, LV_OBJ_FLAG_HIDDEN);

    struct button_action current_action;

    int check_battyr_counter = 0;

    while (1) {

        fuel_gauge_prop_t props[] = {
            FUEL_GAUGE_RELATIVE_STATE_OF_CHARGE_PCT,
            FUEL_GAUGE_VOLTAGE_UV
        };

        const struct zbus_channel *chan;
        if (zbus_sub_wait(&display_sub, &chan, K_NO_WAIT) == 0) {
            if (chan == &input_chan) {
                zbus_chan_read(&input_chan, &current_action, K_NO_WAIT);
                switch (current_action.key_code) {
                    case INPUT_KEY_A:
                        if (current_action.is_pressed) {
                            lv_label_set_text(label, "INPUT A");
                            lv_obj_remove_flag(label, LV_OBJ_FLAG_HIDDEN);
                        } else {
                            lv_obj_add_flag(label, LV_OBJ_FLAG_HIDDEN);
                        }
                        break;
                    case INPUT_KEY_B:
                        if (current_action.is_pressed) {
                            lv_label_set_text(label, "INPUT B");
                            lv_obj_remove_flag(label, LV_OBJ_FLAG_HIDDEN);
                        } else {
                            lv_obj_add_flag(label, LV_OBJ_FLAG_HIDDEN);
                        }
                        break;
                    case INPUT_KEY_C:
                        if (current_action.is_pressed) {
                            lv_label_set_text(label, "INPUT C");
                            lv_obj_remove_flag(label, LV_OBJ_FLAG_HIDDEN);
                        } else {
                            lv_obj_add_flag(label, LV_OBJ_FLAG_HIDDEN);
                        }
                        break;
                    case INPUT_KEY_R:
                        if (current_action.is_pressed) {
                            lv_label_set_text(label, "INPUT R");
                            lv_obj_remove_flag(label, LV_OBJ_FLAG_HIDDEN);
                        } else {
                            lv_obj_add_flag(label, LV_OBJ_FLAG_HIDDEN);
                        }
                        break;
                }
            }
        }

        if ((check_battyr_counter++) % 500 == 0) {
            if (device_is_ready(gauge_dev)) {
                union fuel_gauge_prop_val vals[ARRAY_SIZE(props)];
                int ret = fuel_gauge_get_props(gauge_dev, props, vals, ARRAY_SIZE(props));
                if (ret < 0) {
                    lv_label_set_text(battery_label, "Bat: ERR");
                } else {
                    char batt_str[16];
                    snprintf(batt_str, sizeof(batt_str), "Bat: %d%%", vals[0].relative_state_of_charge_pct);
                    lv_label_set_text(battery_label, batt_str);
                }
            } else {
                lv_label_set_text(battery_label, "Bat: N/A");
            }
        }

        lv_task_handler();
        k_msleep(10);
    }
}
