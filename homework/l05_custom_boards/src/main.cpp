#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

int main(void)
{
    printk("Hello from custom board: %s\n", CONFIG_BOARD);
    while (1) {
        k_msleep(1000);
    }
    return 0;
}