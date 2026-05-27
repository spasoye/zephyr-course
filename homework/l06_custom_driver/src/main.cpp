#include <sys/types.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/led_strip.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/util.h>
#include <zephyr/logging/log.h>
#include <zephyr/autoconf.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/sensor.h>
#include "spas_driver.h"


LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

#define APP_LED_NODE DT_ALIAS(app_led)
static const struct gpio_dt_spec heartbeat_led = GPIO_DT_SPEC_GET(APP_LED_NODE, gpios);

/**
 * @brief Toggle the heartbeat LED at a regular interval to indicate the system 
          is alive. Iomico lecture 4 homework task.
 * 
 */
void heartbeat()
{
    if (!device_is_ready(heartbeat_led.port)) {
        return;
    }

    if (gpio_pin_configure_dt(&heartbeat_led, GPIO_OUTPUT_ACTIVE)) {
        return;
    }

    while (1) {
        gpio_pin_toggle_dt(&heartbeat_led);
        LOG_INF("Heartbeat: System is alive.\n");
        k_msleep(CONFIG_LED_HEARTBEAT_PERIOD_MS);
    }
}

namespace {
    void test() {
        const struct device *driver = DEVICE_DT_GET(DT_NODELABEL(spas_driver0));
        struct sensor_value val;

        auto ret = sensor_sample_fetch(driver);

        uint32_t sleep_time = spas_get_sleep_time(driver);

        LOG_INF("Sleeping for %d ms.", sleep_time);
        k_sleep(K_MSEC(sleep_time));

        spas_set_sleep_time(driver, 1000);
        
        sleep_time = spas_get_sleep_time(driver);
        LOG_INF("Set sleep_time to %d ms.", sleep_time);
        k_sleep(K_MSEC(sleep_time));

        
        ret = sensor_channel_get(driver, SENSOR_CHAN_AMBIENT_TEMP, &val);
        LOG_INF("Channel ret %d", ret);    
    }
}

int main(void)
{
    int ret;

    test();

    printk("Hearbeat LED configured on GPIO pin %d\n", heartbeat_led.pin);
    heartbeat();

    while (1) {
        // Will never reach here
        k_sleep(K_MSEC(5));
    }

    return 0;
}