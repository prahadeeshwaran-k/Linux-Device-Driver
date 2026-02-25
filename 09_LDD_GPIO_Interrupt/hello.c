#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/gpio/consumer.h> /* The modern gpiod API */
#include <linux/interrupt.h>
#include <linux/mod_devicetable.h>

/* The Interrupt Service Routine (ISR) */
static irqreturn_t button_isr(int irq, void *dev_id) {
    pr_info("gpio_irq_gpiod: Interrupt triggered! Button pressed.\n");
    return IRQ_HANDLED;
}

static int gpio_irq_probe(struct platform_device *pdev) {
    struct device *dev = &pdev->dev;
    struct gpio_desc *button_desc;
    int irq_number;
    int ret;

    pr_info("gpio_irq_gpiod: Probing modern GPIO device...\n");

    /* * 1. Get the GPIO descriptor. 
     * We look for "button", which matches "button-gpios" in the Device Tree.
     * GPIOD_IN configures the pin as an input automatically.
     */
    button_desc = devm_gpiod_get(dev, "button", GPIOD_IN);
    if (IS_ERR(button_desc)) {
        pr_err("gpio_irq_gpiod: Failed to get GPIO descriptor\n");
        return PTR_ERR(button_desc);
    }

    /* 2. Get the IRQ number dynamically mapped to this GPIO */
    irq_number = gpiod_to_irq(button_desc);
    if (irq_number < 0) {
        pr_err("gpio_irq_gpiod: Failed to get IRQ number\n");
        return irq_number;
    }
    pr_info("gpio_irq_gpiod: Successfully mapped to IRQ %d\n", irq_number);

    /* 3. Request the Interrupt 
     * Using devm_request_irq means the kernel auto-frees it on module removal.
     */
    ret = devm_request_irq(dev, 
                           irq_number, 
                           button_isr, 
                           IRQF_TRIGGER_RISING, 
                           "custom_button_irq", 
                           NULL);
    if (ret) {
        pr_err("gpio_irq_gpiod: Failed to request IRQ\n");
        return ret;
    }

    pr_info("gpio_irq_gpiod: Probe successful. Listening for interrupts!\n");
    return 0;
}

static int gpio_irq_remove(struct platform_device *pdev) {
    pr_info("gpio_irq_gpiod: Device removed. Managed resources auto-freed.\n");
    return 0;
}

/* Device Tree match table */
static const struct of_device_id gpio_irq_dt_ids[] = {
    { .compatible = "custom,gpio-irq" },
    { }
};
MODULE_DEVICE_TABLE(of, gpio_irq_dt_ids);

/* Register as a Platform Driver */
static struct platform_driver gpio_irq_driver = {
    .driver = {
        .name = "gpio_irq_modern",
        .of_match_table = gpio_irq_dt_ids,
    },
    .probe = gpio_irq_probe,
    .remove = gpio_irq_remove,
};

module_platform_driver(gpio_irq_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Modern gpiod Interrupt Driver for Pi 5");
