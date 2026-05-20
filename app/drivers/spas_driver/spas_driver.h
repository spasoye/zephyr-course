#ifndef _SPAS_DRIVER_H_
#define _SPAS_DRIVER_H_

#include <zephyr/device.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

int spas_get_sleep_time(const struct device *dev);
void spas_set_sleep_time(const struct device *dev, int sleep_time);

#ifdef __cplusplus
}
#endif

#endif /* _SPAS_DRIVER_H_ */