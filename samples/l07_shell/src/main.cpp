#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/shell/shell.h>
#include <zephyr/version.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/drivers/sensor.h>
#include "spas_driver.h"

LOG_MODULE_REGISTER(shell, LOG_LEVEL_INF);

static int cmd_fetch(const struct shell *sh, size_t argc, char **argv) {
    const struct device *driver = DEVICE_DT_GET(DT_NODELABEL(spas_driver0));

    sensor_sample_fetch(driver);

    shell_print(sh, "Fetching data from sensor %s", driver->name);
    
    return 0;
}

static int cmd_read(const struct shell *sh, size_t argc, char **argv) {
    const struct device *driver = DEVICE_DT_GET(DT_NODELABEL(spas_driver0));
    struct sensor_value sens_val;

    sensor_channel_get(driver, SENSOR_CHAN_AMBIENT_TEMP,  &sens_val);
    shell_print(sh, "Reading data from sensor %s, value: %d.%06d", driver->name, sens_val.val1, sens_val.val2);

    return 0;
}

static int cmd_info(const struct shell *sh, size_t argc, char **argv) {
    const struct device *driver = DEVICE_DT_GET(DT_NODELABEL(spas_driver0));

    shell_print(sh, "Device name: %s", driver->name);
    shell_print(sh, "Device state: %s", device_is_ready(driver) ? "yes" : "no");

    return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(sub_sensor,
    SHELL_CMD(fetch, NULL, "Calls sensor_sample_fetch", cmd_fetch),
    SHELL_CMD(read, NULL, "Calls sensor_sample_fetch_chan", cmd_read),
    SHELL_CMD(info, NULL, "Print dev name and ready state", cmd_info),
    SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(sensor, &sub_sensor, "LED-based sensor control", NULL);

int main(void)
{
    LOG_INF("Welcome to Zephyr Shell!");

    while (1) {
        // Will never reach here
        k_sleep(K_MSEC(5));
    }

    return 0;
}