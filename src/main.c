#include <zephyr/kernel.h>
#include <lvgl.h>

int main(void)
{
    while (1) {

        /*
         * lv_tast_handler is called here because it cannot be called in the thread.
         */
        lv_task_handler();
        k_msleep(10);
    }
}
