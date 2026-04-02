#include <sys/types.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/led_strip.h>
#include <zephyr/sys/util.h>
#include <zephyr/logging/log.h>
#include <zephyr/autoconf.h>

// Get the device pointer for the LED strip from the devicetree alias
#define LED_NODE DT_ALIAS(led_strip)
static const struct device *led_strip = DEVICE_DT_GET(LED_NODE);

// Define a pixel buffer for one LED (GRB order)
static led_rgb pixel_struct = {.r = 0x00, .g = 0x00, .b = 0x00 }; // Blue, Red, Green

u_int32_t delay = CONFIG_BLINK_SLEEP_TIME_MS;

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);
int main(void)
{
    int ret;

    // Verify the LED strip device is ready
    if (!device_is_ready(led_strip)) {
        return 0;
    }


    while (1) {
        // Set LED to Red
        pixel_struct.r = 0x8F; // Red
        pixel_struct.g = 0x00; // Green
        pixel_struct.b = 0x00; // Blue
        
        ret = led_strip_update_rgb(led_strip, &pixel_struct, 1);
        LOG_INF("LED state: Red\n");

        
        if (ret) {
            return 0;
        }
        k_msleep(delay);

        // Set LED to Green
        pixel_struct.r = 0x00; // Red
        pixel_struct.g = 0x8F; // Green
        pixel_struct.b = 0x00; // Blue

        ret = led_strip_update_rgb(led_strip, &pixel_struct, 1);
        LOG_INF("LED state: Green\n");

        if (ret) {
            return 0;
        }
        k_msleep(delay);

        // Set LED to Blue
        pixel_struct.r = 0x00; // Red
        pixel_struct.g = 0x00; // Green
        pixel_struct.b = 0x8F; // Blue

        ret = led_strip_update_rgb(led_strip, &pixel_struct, 1);
        LOG_INF("LED state: Blue\n");
        
        if (ret) {
            return 0;
        }

        k_msleep(delay);
    }

    return 0;
}