#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/fuel_gauge.h>

#include "events.h"

#define GET_VALUE_OR_FULL(val, full) \
    ((val) > (full))? (full): (val)

const struct device *gauge_dev = DEVICE_DT_GET_ANY(maxim_max17048);

static void battery_check(void)
{
    while (1) {
        if (device_is_ready(gauge_dev)) {
            union fuel_gauge_prop_val val;
            int ret = fuel_gauge_get_prop(gauge_dev, FUEL_GAUGE_RELATIVE_STATE_OF_CHARGE_PCT, &val);
            if (ret < 0) {
                struct battery_action error = {.has_error=true};
                zbus_chan_pub(&battery_chan, &error, K_NO_WAIT);
            } else {
                struct battery_action msg = {.percentage=GET_VALUE_OR_FULL(val.relative_state_of_charge_pct, 100)};
                zbus_chan_pub(&battery_chan, &msg, K_NO_WAIT);
            }
        }
        k_sleep(K_SECONDS(1));
    }
}
K_THREAD_DEFINE(battery_tid, 1024, battery_check, NULL, NULL, NULL, 7, 0, 0);
