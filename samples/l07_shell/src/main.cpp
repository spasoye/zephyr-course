#include <cstdint>
#include <cstdlib>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/shell/shell.h>
#include <zephyr/version.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>
#include "spas_driver.h"
#include "zephyr/toolchain.h"

LOG_MODULE_REGISTER(shell, LOG_LEVEL_INF);

// Heartbeat LED to indicate the system is alive and to demonstrate the sleep time of the sensor.
static const struct gpio_dt_spec hb_led = GPIO_DT_SPEC_GET(DT_ALIAS(heartbeat_led), gpios);

// static uint32_t hb_sleep_ms = 0;

static int cmd_fetch(const struct shell *sh, size_t argc, char **argv) {
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);
    

    const struct device *driver = DEVICE_DT_GET(DT_NODELABEL(spas_driver0));

    sensor_sample_fetch(driver);

    shell_print(sh, "Fetching data from sensor %s", driver->name);
    
    return 0;
}

static int cmd_read(const struct shell *sh, size_t argc, char **argv) {
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);
    
    const struct device *driver = DEVICE_DT_GET(DT_NODELABEL(spas_driver0));
    struct sensor_value sens_val;

    sensor_channel_get(driver, SENSOR_CHAN_AMBIENT_TEMP,  &sens_val);
    shell_print(sh, "Reading data from sensor %s, value: %d.%06d", driver->name, sens_val.val1, sens_val.val2);

    return 0;
}

static int cmd_info(const struct shell *sh, size_t argc, char **argv) {
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);

    const struct device *driver = DEVICE_DT_GET(DT_NODELABEL(spas_driver0));

    shell_print(sh, "Device name: %s", driver->name);
    shell_print(sh, "Device state: %s", device_is_ready(driver) ? "yes" : "no");
    shell_print(sh, "Current sleep time: %d ms", spas_get_sleep_time(driver));

    return 0;
}

static int cmd_set(const struct shell *sh, size_t argc, char **argv) {
    long sleep_time;
    char *end_ptr;
    static const struct device *driver = DEVICE_DT_GET(DT_NODELABEL(spas_driver0));

    // Check if the argument is valid.
    sleep_time = strtol(argv[1], &end_ptr, 10);
    if (*end_ptr != '\0' || sleep_time < 500 || sleep_time > 2000) {
        shell_error(sh, "Invalid argument");
        return -EINVAL;
    }

    spas_set_sleep_time(driver, sleep_time);

    return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(sub_sensor,
    SHELL_CMD(fetch, NULL, "Calls sensor_sample_fetch", cmd_fetch),
    SHELL_CMD(read, NULL, "Calls sensor_sample_fetch_chan", cmd_read),
    SHELL_CMD(info, NULL, "Print dev name and ready state", cmd_info),
    SHELL_CMD_ARG(set, NULL, "Change heartbeat LED sleep time (500 - 2000 ms) ", cmd_set, 2, 0),
    SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(sensor, &sub_sensor, "LED-based sensor control", NULL);

int main(void)
{
    const struct device *driver = DEVICE_DT_GET(DT_NODELABEL(spas_driver0));
    uint32_t sleep_time;

    if (!device_is_ready(hb_led.port)) {
        return -ENODEV;
    }

    if (gpio_pin_configure_dt(&hb_led, GPIO_OUTPUT_ACTIVE)) {
        return -ENODEV;
    }

    LOG_INF("Welcome to Zephyr Shell!");

    if (!device_is_ready(driver)) {
        LOG_ERR("Sensor device not ready");
        return -ENODEV;
    }

    sleep_time = spas_get_sleep_time(driver);
    LOG_INF("Initial sleep time: %d ms", sleep_time);
    
    while (1) {
        sleep_time = spas_get_sleep_time(driver);
        gpio_pin_set_dt(&hb_led, 1);
        k_sleep(K_MSEC(200));
        gpio_pin_set_dt(&hb_led, 0);
        k_sleep(K_MSEC(sleep_time));
    }

    return 0;
}