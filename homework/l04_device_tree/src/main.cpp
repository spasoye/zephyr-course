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

int main(void)
{
    int ret;

    if (device_is_ready(strip)) {
        led_strip_update_rgb(strip, &off, 1);
    }

    printk("Hearbeat LED configured on GPIO pin %d\n", heartbeat_led.pin);
    heartbeat();

    while (1) {

    }

    return 0;
}