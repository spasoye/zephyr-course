#include "zephyr/logging/log_core.h"
#define DT_DRV_COMPAT spas_driver

#include <zephyr/logging/log.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/gpio.h>

LOG_MODULE_REGISTER(spas_driver, LOG_LEVEL_INF );

struct spas_config {
    struct gpio_dt_spec led;
};

static int spas_channel_get(const struct device *dev,
				            enum sensor_channel chan,
				            struct sensor_value *val) {
    const struct spas_config *cfg = dev->config;

    LOG_INF("Hello from Channel get, channel %d", chan);
    LOG_INF("LED off.");

    gpio_pin_set_dt(&cfg->led, 0);
    
    return 0;
}

static int spas_sample_fetch(const struct device *dev,
                             enum sensor_channel chan){
    const struct spas_config *cfg = dev -> config;
    LOG_INF("Hello from sample fetch, channel %d", chan);
    LOG_INF("LED on.");

    gpio_pin_set_dt(&cfg->led, 1);
    
    return 0;
}

static DEVICE_API(sensor, api_iomico_lecture) = {

    .channel_get = spas_channel_get,
    .sample_fetch = spas_sample_fetch,
};

static const struct spas_config spas_cfg = {
    .led = GPIO_DT_SPEC_INST_GET(0, gpios),
};

static int init(const struct device *dev) {
    const struct spas_config *cfg = dev->config;

    if (!gpio_is_ready_dt(&cfg->led)) {
        LOG_ERR("LED GPIO not ready");
        return -ENODEV;
    }

    int ret = gpio_pin_configure_dt(&cfg->led, GPIO_OUTPUT_INACTIVE);
    if (ret != 0){
        LOG_ERR("Failed to configure LED GPIO, error %d", ret);
        return ret;
    }

    LOG_INF("Device driver initialized.");
    return 0;
}


DEVICE_DT_INST_DEFINE(0, init, NULL, NULL, &spas_cfg, POST_KERNEL, 80, 
                       &api_iomico_lecture);