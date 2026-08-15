#include <stdint.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/display.h>
#include <zephyr/input/input.h>
#include <lvgl.h>

LV_IMAGE_DECLARE(my_bg);

struct button_action {
    uint16_t key_code;
    bool is_pressed;
};

K_MSGQ_DEFINE_TYPE(btn_msgq, struct button_action, 10);

static void input_cb(struct input_event *e, void *user_input)
{
    if (e->type == INPUT_EV_KEY) {
        struct button_action action = {
            .key_code = e->code,
            .is_pressed = (e->value == 1),
        };

        k_msgq_put(&btn_msgq, &action, K_NO_WAIT);
    }
}

INPUT_CALLBACK_DEFINE(NULL, input_cb, NULL);

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

    lv_obj_t *label = lv_label_create(bg_img);
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

    lv_obj_add_flag(label, LV_OBJ_FLAG_HIDDEN);

    struct button_action current_action;

    while (1) {
        while (k_msgq_get(&btn_msgq, &current_action, K_NO_WAIT) == 0) {
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
        lv_task_handler();
        k_msleep(10);
    }
}
