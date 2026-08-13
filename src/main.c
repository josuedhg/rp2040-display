#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/display.h>
#include <lvgl.h>

LV_IMAGE_DECLARE(my_bg);

#define FIX_COLOR(h) lv_color_hex( \
    ( ((((h) >> 8) & 0x1C) << 3) | ((((h) >> 16) & 0xC0) >> 3) ) << 16 | \
    ( ((((h) >> 16) & 0x38) << 2) | (((h) & 0xE0) >> 3) ) << 8 | \
    ( (((h) & 0x18) << 3) | ((((h) >> 8) & 0xE0) >> 2) ) \
)

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
    lv_task_handler();

    while (1) {
        lv_task_handler();
        k_msleep(10);
    }
}
