/*
 * platform indepent driver interface
 *
 * Coypritht (c) 2017 Goodix
 */
#include <linux/delay.h>
#include <linux/workqueue.h>
#include <linux/of_gpio.h>
#include <linux/gpio.h>
#include <linux/regulator/consumer.h>
#include <linux/timer.h>
#include <linux/err.h>
#include <drm/drm_panel.h>

#include "gf_spi.h"

#if defined(USE_SPI_BUS)
#include <linux/spi/spi.h>
#include <linux/spi/spidev.h>
#elif defined(USE_PLATFORM_BUS)
#include <linux/platform_device.h>
#endif

int check_drm_dt(struct gf_dev *gf_dev)
{
	int count,i = 0;
	struct device *dev = &gf_dev->spi->dev;
	struct device_node *np = dev->of_node;
	struct device_node *node = NULL;
	struct drm_panel *panel = NULL;

	count = of_count_phandle_with_args(np, "panel", NULL);
	if (count <= 0) {
		pr_err("find drm_panel count(%d) fail", count);
		return -ENODEV;
	}

	for (i = 0; i < count; i++) {
		node = of_parse_phandle(np, "panel", i);
		panel = of_drm_find_panel(node);
		of_node_put(node);
		if (!IS_ERR(panel)) {
			pr_info("find drm_panel successfully");
			gf_dev->active_panel = panel;
			return 0;
		}
	}
	pr_err("goodix fp can not find drm_panel");

	return -ENODEV;
}

int gf_parse_dts(struct gf_dev *gf_dev)
{
	int rc = 0;
	struct device *dev = &gf_dev->spi->dev;
	struct device_node *np = dev->of_node;

	gf_dev->reset_gpio = of_get_named_gpio(np, "fp-gpio-reset", 0);
	if (gf_dev->reset_gpio < 0) {
		pr_err("falied to get reset gpio!\n");
		return gf_dev->reset_gpio;
	}

	rc = devm_gpio_request(dev, gf_dev->reset_gpio, "goodix_reset");
	if (rc) {
		pr_err("failed to request reset gpio, rc = %d\n", rc);
		goto err_reset;
	}
	gpio_direction_output(gf_dev->reset_gpio, 0);

	gf_dev->irq_gpio = of_get_named_gpio(np, "fp-gpio-irq", 0);
	if (gf_dev->irq_gpio < 0) {
		pr_err("falied to get irq gpio!\n");
		return gf_dev->irq_gpio;
	}

	rc = devm_gpio_request(dev, gf_dev->irq_gpio, "goodix_irq");
	if (rc) {
		pr_err("failed to request irq gpio, rc = %d\n", rc);
		goto err_irq;
	}
	gpio_direction_input(gf_dev->irq_gpio);

	gf_dev->vdd =  regulator_get(dev, "vdd");
	if (IS_ERR(gf_dev->vdd)) {
		rc = PTR_ERR(gf_dev->vdd);
		pr_err("Regulator get failed vdd rc = %d\n", rc);
		goto err_pwr;
	}

	rc = of_property_read_u32(np, "vdd-max-uv", &gf_dev->vdd_max_uv);
	if(rc){
		pr_err("fail to get vdd_max_uv\n");
		goto err_reg;
	}

	rc = of_property_read_u32(np, "vdd-min-uv", &gf_dev->vdd_min_uv);
	if(rc){
		pr_err("fail to get vdd_min_uv\n");
		goto err_reg;
	}

	rc = regulator_set_voltage(gf_dev->vdd,gf_dev->vdd_min_uv, gf_dev->vdd_max_uv);
	if (rc) {
		pr_err("Regulator set voltage failed rc=%d\n", rc);
		goto err_reg;
	}

	return rc;

err_reg:
	regulator_put(gf_dev->vdd);
err_pwr:
	gpio_free(gf_dev->irq_gpio);
err_irq:
	gpio_free(gf_dev->reset_gpio);
err_reset:
	return rc;
}

void gf_cleanup(struct gf_dev *gf_dev)
{
	pr_info("[info] %s\n", __func__);

	if (gpio_is_valid(gf_dev->irq_gpio)) {
		gpio_free(gf_dev->irq_gpio);
		pr_info("remove irq_gpio success\n");
	}
	if (gpio_is_valid(gf_dev->reset_gpio)) {
		gpio_free(gf_dev->reset_gpio);
		pr_info("remove reset_gpio success\n");
	}
}

int gf_power_on(struct gf_dev *gf_dev)
{
	int rc = 0;

	if(!regulator_is_enabled(gf_dev->vdd)){
		rc = regulator_enable(gf_dev->vdd);
		if (rc) {
			pr_err("Regulator enable failed rc=%d\n", rc);
			goto err_reg;
		}
	}
	/* TODO: add your power control here */
	return rc;
err_reg:
	return rc;
}

int gf_power_off(struct gf_dev *gf_dev)
{
	int rc = 0;

	rc = regulator_disable(gf_dev->vdd);

	/* TODO: add your power control here */

	return rc;
}

int gf_hw_reset(struct gf_dev *gf_dev, unsigned int delay_ms)
{
	if (gf_dev == NULL) {
		pr_info("Input buff is NULL.\n");
		return -1;
	}
	//gpio_direction_output(gf_dev->reset_gpio, 1);
	gpio_set_value(gf_dev->reset_gpio, 0);
	usleep_range(delay_ms*1000, delay_ms*1000);
	gpio_set_value(gf_dev->reset_gpio, 1);
	return 0;
}

int gf_irq_num(struct gf_dev *gf_dev)
{
	if (gf_dev == NULL) {
		pr_info("Input buff is NULL.\n");
		return -1;
	} else {
		return gpio_to_irq(gf_dev->irq_gpio);
	}
}
