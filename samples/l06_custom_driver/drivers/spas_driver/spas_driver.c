#define DT_DRV_COMPAT spas_driver

#include "zephyr/devicetree.h"
#include <stdint.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/gpio.h>
#include "spas_driver.h"

LOG_MODULE_REGISTER(spas_driver, LOG_LEVEL_INF );

struct spas_config {
    struct gpio_dt_spec led;
};

struct spas_data {
    int sleep_ms;
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

static struct spas_data spas_dat = {
    .sleep_ms = 0,
};

static int init(const struct device *dev) {

    struct spas_data *data = dev->data;
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

    data->sleep_ms = DT_INST_PROP(0, sleep_ms);

    LOG_INF("Device driver initialized.");
    return 0;
}

int spas_get_sleep_time(const struct device *dev) {
    struct spas_data *data = dev->data;
    LOG_INF("Getting sleep_ms value...");
    return data->sleep_ms;
}

void spas_set_sleep_time(const struct device *dev, int sleep_time) {
    struct spas_data *data = dev->data;
    LOG_INF("Setting sleep_ms value...");
    data->sleep_ms = sleep_time;
}

DEVICE_DT_INST_DEFINE(0, init, NULL, &spas_dat, &spas_cfg, POST_KERNEL, 80, 
                       &api_iomico_lecture);