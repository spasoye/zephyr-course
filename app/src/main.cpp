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

#if CONFIG_LED_RED
    static struct led_rgb color = {.r = (0xFF), .g = (0x00), .b = (0x00)};
#elif CONFIG_LED_GREEN
    static struct led_rgb color = {.r = (0x00), .g = (0xFF), .b = (0x00)};
#elif CONFIG_LED_BLUE
    static struct led_rgb color = {.r = (0x00), .g = (0x00), .b = (0xFF)};
#endif

static struct led_rgb scale_brightness(struct led_rgb curr, uint8_t brightness)
{
    return (struct led_rgb){
        .r = (curr.r * brightness) / 255,
        .g = (curr.g * brightness) / 255,
        .b = (curr.b * brightness) / 255
    };
}

void fade_led()
{
    int step_delay = CONFIG_LED_FADE_MS / CONFIG_LED_BRIGHTNESS;

    for (int i = 0; i < CONFIG_LED_BRIGHTNESS; i++) {
        pixel_struct = scale_brightness(color, CONFIG_LED_BRIGHTNESS - i);
        led_strip_update_rgb(led_strip, &pixel_struct, 1);
        k_msleep(step_delay);
    }

    pixel_struct = (struct led_rgb){ .r = 0x00, .g = 0x00, .b = 0x00 };
    led_strip_update_rgb(led_strip, &pixel_struct, 1);
}

int main(void)
{
    int ret;

    // Verify the LED strip device is ready
    if (!device_is_ready(led_strip)) {
        return 0;
    }


    while (1) {
        
#if CONFIG_ADV_LED

        fade_led();
#if CONFIG_LED_DEBUG
        LOG_INF("LED faded out, going to sleep.\n");
#endif
        k_msleep(delay);
#else      
        /* Circle through Red, Green, and Blue colors with the configured brightness level. */

        LOG_INF("LED brightness: %d %\n", CONFIG_LED_BRIGHTNESS);
        // Set LED to Red
        pixel_struct = scale_brightness((struct led_rgb){.r = 0x8F, .g = 0x00, .b = 0x00}, CONFIG_LED_BRIGHTNESS);
        LOG_INF("RED value: %X\n", pixel_struct.r);
        ret = led_strip_update_rgb(led_strip, &pixel_struct, 1);
        LOG_INF("LED state: Red\n");

        
        if (ret) {
            return 0;
        }
        k_msleep(delay);

        // Set LED to Green
        pixel_struct = scale_brightness((struct led_rgb){.r = 0x00, .g = 0x87, .b = 0x00}, CONFIG_LED_BRIGHTNESS); 

        ret = led_strip_update_rgb(led_strip, &pixel_struct, 1);
        LOG_INF("LED state: Green\n");

        if (ret) {
            return 0;
        }
        k_msleep(delay);

        // Set LED to Blue
        pixel_struct = scale_brightness((struct led_rgb){.r = 0x00, .g = 0x00, .b = 0x8F}, CONFIG_LED_BRIGHTNESS);

        ret = led_strip_update_rgb(led_strip, &pixel_struct, 1);
        LOG_INF("LED state: Blue\n");
        
        if (ret) {
            return 0;
        }

        k_msleep(delay);
#endif
    }

    return 0;
}