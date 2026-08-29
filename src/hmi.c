#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/display.h>
#include <lvgl.h>

LV_IMAGE_DECLARE(my_bg);

lv_obj_t *battery_label = NULL;
lv_obj_t *buttons_label = NULL;

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
