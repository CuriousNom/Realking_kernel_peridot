// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2012-2014, 2017-2021, The Linux Foundation. All rights reserved.
 * Copyright (c) 2022-2024, Qualcomm Innovation Center, Inc. All rights reserved.
 */

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/err.h>
#include <linux/string.h>
#include <linux/delay.h>
#include <linux/platform_device.h>
#include <dsp/spf-core.h>
#include <linux/of_device.h>
#include <linux/sysfs.h>
#include <linux/workqueue.h>
#include <linux/nvmem-consumer.h>
#include <linux/slab.h>
#include <linux/remoteproc.h>
#include <linux/remoteproc/qcom_rproc.h>


#define Q6_PIL_GET_DELAY_MS 100
#define BOOT_CMD 1
#define SSR_RESET_CMD 1
#define IMAGE_UNLOAD_CMD 0
#define MAX_FW_IMAGES 4
#define ADSP_LOADER_APM_TIMEOUT_MS 10000

enum spf_subsys_state {
	SPF_SUBSYS_DOWN,
	SPF_SUBSYS_UP,
	SPF_SUBSYS_LOADED,
	SPF_SUBSYS_UNKNOWN,
};

static ssize_t adsp_boot_store(struct kobject *kobj,
	struct kobj_attribute *attr,
	const char *buf, size_t count);

static ssize_t adsp_ssr_store(struct kobject *kobj,
	struct kobj_attribute *attr,
	const char *buf, size_t count);

struct adsp_loader_private {
	void *pil_h;
	struct kobject *boot_adsp_obj;
	struct attribute_group *attr_group;
	char *adsp_fw_name;
	char *adsp_dtb_name;
};

static struct kobj_attribute adsp_boot_attribute =
	__ATTR(boot, 0220, NULL, adsp_boot_store);

static struct kobj_attribute adsp_ssr_attribute =
	__ATTR(ssr, 0220, NULL, adsp_ssr_store);

static struct attribute *attrs[] = {
	&adsp_boot_attribute.attr,
	&adsp_ssr_attribute.attr,
	NULL,
};

static struct work_struct adsp_ldr_work;
static struct platform_device *adsp_private;
static void adsp_loader_unload(struct platform_device *pdev);


static void adsp_load_fw(struct work_struct *adsp_ldr_work)
{
	struct platform_device *pdev = adsp_private;
	struct adsp_loader_private *priv = NULL;
	const char *adsp_dt = "qcom,adsp-state";
	int rc = 0;
	u32 adsp_state;
	struct property *prop;
	int size;
	phandle rproc_phandle;
	struct rproc *rproc;

	if (!pdev) {
		dev_err(&pdev->dev, "%s: Platform device null\n", __func__);
		goto fail;
	}

	if (!pdev->dev.of_node) {
		dev_err(&pdev->dev,
			"%s: Device tree information missing\n", __func__);
		goto fail;
	}

	priv = platform_get_drvdata(pdev);
	if (!priv) {
		dev_err(&pdev->dev,
		" %s: Private data get failed\n", __func__);
		goto fail;
	}

	rc = of_property_read_u32(pdev->dev.of_node, adsp_dt, &adsp_state);
	if (rc) {
		dev_err(&pdev->dev,
			"%s: ADSP state = %x\n", __func__, adsp_state);
		goto fail;
	}

	prop = of_find_property(pdev->dev.of_node, "qcom,proc-img-to-load",
					&size);
	if (!prop) {
		dev_dbg(&pdev->dev,
			"%s: loading default image ADSP\n", __func__);
		goto load_adsp;
	}

	rproc_phandle = be32_to_cpup(prop->value);
	priv->pil_h = rproc_get_by_phandle(rproc_phandle);
	if (!priv->pil_h)
		goto fail;

	rproc = priv->pil_h;
	if (!strcmp(rproc->name, "modem")) {
		if (adsp_state == SPF_SUBSYS_DOWN) {
			rc = rproc_boot(priv->pil_h);
			if (IS_ERR(priv->pil_h) || rc) {
				dev_err(&pdev->dev, "%s: pil get failed,\n",
					__func__);
				goto fail;
			}

		} else if (adsp_state == SPF_SUBSYS_LOADED) {
			dev_dbg(&pdev->dev,
			"%s: MDSP state = %x\n", __func__, adsp_state);
		}

		dev_dbg(&pdev->dev, "%s: Q6/MDSP image is loaded\n", __func__);
	}

load_adsp:
	{
		adsp_state = spf_core_is_apm_ready(ADSP_LOADER_APM_TIMEOUT_MS);
		if (adsp_state == SPF_SUBSYS_DOWN) {
			if (!priv->adsp_fw_name) {
				dev_info(&pdev->dev, "%s: Load default ADSP\n",
					__func__);
			} else {
				dev_info(&pdev->dev, "%s: Load ADSP with fw name %s\n",
					__func__, priv->adsp_fw_name);
				rc = rproc_set_firmware(priv->pil_h,
					priv->adsp_fw_name);
				if (rc) {
					dev_err(&pdev->dev, "%s: rproc set firmware failed,\n",
						__func__);
					goto fail;
				}
			}
			if (!priv->adsp_dtb_name) {
				dev_info(&pdev->dev, "%s: Load default ADSP DTB\n",
					__func__);
			} else {
				dev_info(&pdev->dev, "%s: Load ADSP DTB with fw name %s\n",
					__func__, priv->adsp_dtb_name);
				rc = qcom_rproc_set_dtb_firmware(priv->pil_h,
					priv->adsp_dtb_name);
				if (rc) {
					dev_err(&pdev->dev, "%s: rproc set dtb firmware failed,\n",
						__func__);
					goto fail;
				}
			}
			rc = rproc_boot(priv->pil_h);
			if (rc) {
				dev_err(&pdev->dev, "%s: pil get failed,\n",
					__func__);
				goto fail;
			}
		} else if (adsp_state == SPF_SUBSYS_LOADED) {
			dev_dbg(&pdev->dev,
				"%s: ADSP state = %x\n", __func__, adsp_state);
		}

		dev_dbg(&pdev->dev, "%s: Q6/ADSP image is loaded\n", __func__);
		return;
	}

fail:
	dev_err(&pdev->dev, "%s: Q6 image loading failed\n", __func__);
}

static void adsp_loader_do(struct platform_device *pdev)
{
	schedule_work(&adsp_ldr_work);
}

static ssize_t adsp_ssr_store(struct kobject *kobj,
	struct kobj_attribute *attr,
	const char *buf,
	size_t count)
{
	int ssr_command = 0;
	struct rproc *adsp_dev = NULL;
	struct platform_device *pdev = adsp_private;
	struct adsp_loader_private *priv = NULL;

	dev_dbg(&pdev->dev, "%s: going to call adsp ssr\n ", __func__);

	priv = platform_get_drvdata(pdev);
	if (!priv)
		return -EINVAL;

	if (kstrtoint(buf, 10, &ssr_command) < 0)
		return -EINVAL;

	if (ssr_command != SSR_RESET_CMD)
		return -EINVAL;

	adsp_dev = (struct rproc *)priv->pil_h;
	if (!adsp_dev)
		return -EINVAL;

	dev_err(&pdev->dev, "requesting for ADSP restart\n");

	rproc_shutdown(adsp_dev);
	adsp_loader_do(adsp_private);

	dev_dbg(&pdev->dev, "%s :: ADSP restarted\n", __func__);
	return count;
}

static ssize_t adsp_boot_store(struct kobject *kobj,
	struct kobj_attribute *attr,
	const char *buf,
	size_t count)
{
	int boot = 0;

	if (sscanf(buf, "%du", &boot) != 1) {
		pr_err("%s: failed to read boot info from string\n", __func__);
		return -EINVAL;
	}

	if (boot == BOOT_CMD) {
		pr_debug("%s: going to call adsp_loader_do\n", __func__);
		adsp_loader_do(adsp_private);
	} else if (boot == IMAGE_UNLOAD_CMD) {
		pr_debug("%s: going to call adsp_unloader\n", __func__);
		adsp_loader_unload(adsp_private);
	}
	return count;
}

static void adsp_loader_unload(struct platform_device *pdev)
{
	struct adsp_loader_private *priv = NULL;

	priv = platform_get_drvdata(pdev);

	if (!priv)
		return;

	if (priv->pil_h) {
		rproc_shutdown(priv->pil_h);
	}
}

static int adsp_loader_init_sysfs(struct platform_device *pdev)
{
	int ret = -EINVAL;
	struct adsp_loader_private *priv = NULL;

	adsp_private = NULL;
	priv = devm_kzalloc(&pdev->dev, sizeof(*priv), GFP_KERNEL);
	if (!priv) {
		ret = -ENOMEM;
		return ret;
	}

	platform_set_drvdata(pdev, priv);

	priv->pil_h = NULL;
	priv->boot_adsp_obj = NULL;
	priv->attr_group = devm_kzalloc(&pdev->dev,
				sizeof(*(priv->attr_group)),
				GFP_KERNEL);
	if (!priv->attr_group) {
		ret = -ENOMEM;
		goto error_return;
	}

	priv->attr_group->attrs = attrs;

	priv->boot_adsp_obj = kobject_create_and_add("boot_adsp", kernel_kobj);
	if (!priv->boot_adsp_obj) {
		dev_err(&pdev->dev, "%s: sysfs create and add failed\n",
						__func__);
		ret = -ENOMEM;
		goto error_return;
	}

	ret = sysfs_create_group(priv->boot_adsp_obj, priv->attr_group);
	if (ret) {
		dev_err(&pdev->dev, "%s: sysfs create group failed %d\n",
							__func__, ret);
		goto error_return;
	}

	adsp_private = pdev;

	return 0;

error_return:

	if (priv->boot_adsp_obj) {
		kobject_del(priv->boot_adsp_obj);
		priv->boot_adsp_obj = NULL;
	}

	return ret;
}

static int adsp_loader_remove(struct platform_device *pdev)
{
	struct adsp_loader_private *priv = NULL;

	priv = platform_get_drvdata(pdev);

	if (!priv)
		return 0;

	if (priv->pil_h) {
		rproc_shutdown(priv->pil_h);
		priv->pil_h = NULL;
	}

	if (priv->boot_adsp_obj) {
		sysfs_remove_group(priv->boot_adsp_obj, priv->attr_group);
		kobject_del(priv->boot_adsp_obj);
		priv->boot_adsp_obj = NULL;
	}

	return 0;
}

static int adsp_loader_probe(struct platform_device *pdev)
{
	struct adsp_loader_private *priv = NULL;
	struct nvmem_cell *cell;
	size_t len;
	u32 *buf;
	const char **adsp_fw_name_array = NULL;
	const char **adsp_dtb_fw_name_array = NULL;
	int adsp_fw_cnt;
	u32* adsp_fw_bit_values = NULL;
	int i;
	int fw_name_size;
	u32 adsp_var_idx = 0;
	int ret = 0;
	u32 adsp_fuse_not_supported = 0;
	const char *adsp_fw_name;
	const char *adsp_dtb_name;
	struct property *prop;
	int size;
	phandle rproc_phandle;
	struct rproc *adsp;

	prop = of_find_property(pdev->dev.of_node, "qcom,rproc-handle",
				&size);
	if (!prop) {
		dev_err(&pdev->dev, "Missing remotproc handle\n");
		return -ENOPARAM;
	}
	rproc_phandle = be32_to_cpup(prop->value);
	adsp = rproc_get_by_phandle(rproc_phandle);
	if (!adsp) {
		dev_err(&pdev->dev, "fail to get rproc\n");
		return -EPROBE_DEFER;
	}

	ret = adsp_loader_init_sysfs(pdev);
	if (ret != 0) {
		dev_err(&pdev->dev, "%s: Error in initing sysfs\n", __func__);
		rproc_put(adsp);
		return ret;
	}

	priv = platform_get_drvdata(pdev);
	priv->pil_h = adsp;

	/* get adsp variant idx */
	cell = nvmem_cell_get(&pdev->dev, "adsp_variant");
	if (IS_ERR_OR_NULL(cell)) {
		dev_dbg(&pdev->dev, "%s: FAILED to get nvmem cell \n",
			__func__);

		/*
		 * When ADSP variant read from fuse register is not
		 * supported, check if image with different fw image
		 * name needs to be loaded
		 */
		ret = of_property_read_u32(pdev->dev.of_node,
					  "adsp-fuse-not-supported",
					  &adsp_fuse_not_supported);
		if (ret) {
			dev_dbg(&pdev->dev,
				"%s: adsp_fuse_not_supported prop not found %d\n",
				__func__, ret);
			goto wqueue;
		}

		if (adsp_fuse_not_supported) {
			/* Read ADSP firmware image name */
			ret = of_property_read_string(pdev->dev.of_node,
						"adsp-fw-name",
						 &adsp_fw_name);
			if (ret < 0) {
				dev_dbg(&pdev->dev, "%s: unable to read fw-name\n",
					__func__);
				goto wqueue;
			}

			fw_name_size = strlen(adsp_fw_name) + 1;
			priv->adsp_fw_name = devm_kzalloc(&pdev->dev,
						fw_name_size,
						GFP_KERNEL);
			if (!priv->adsp_fw_name)
				goto wqueue;
			strlcpy(priv->adsp_fw_name, adsp_fw_name,
				fw_name_size);

			ret = of_property_read_string(pdev->dev.of_node,
						"adsp-dtb-name",
						 &adsp_dtb_name);
			if (ret < 0) {
				dev_dbg(&pdev->dev, "%s: unable to read fw-dtb-name\n",
					__func__);
				goto wqueue;
			}

			fw_name_size = strlen(adsp_dtb_name) + 1;
			priv->adsp_dtb_name = devm_kzalloc(&pdev->dev,
						fw_name_size,
						GFP_KERNEL);
			if (!priv->adsp_dtb_name)
				goto wqueue;
			strscpy(priv->adsp_dtb_name, adsp_dtb_name,
				fw_name_size);
		}
		goto wqueue;
	}
	buf = nvmem_cell_read(cell, &len);
	nvmem_cell_put(cell);
	if (IS_ERR_OR_NULL(buf)) {
		dev_dbg(&pdev->dev, "%s: FAILED to read nvmem cell \n", __func__);
		goto wqueue;
	}
	if (len <= 0 || len > sizeof(u32)) {
		dev_dbg(&pdev->dev, "%s: nvmem cell length out of range: %d\n",
			__func__, len);
		kfree(buf);
		goto wqueue;
	}
	memcpy(&adsp_var_idx, buf, len);
	dev_info(&pdev->dev, "%s: adsp variant fuse reg value: 0x%x\n",
		__func__, adsp_var_idx);
	kfree(buf);

	/* Get count of fw images */
	adsp_fw_cnt = of_property_count_strings(pdev->dev.of_node,
						"adsp-fw-names");
	if (adsp_fw_cnt <= 0 || adsp_fw_cnt > MAX_FW_IMAGES) {
		dev_dbg(&pdev->dev, "%s: Invalid number of fw images %d",
			__func__, adsp_fw_cnt);
		goto wqueue;
	}

	adsp_fw_bit_values = devm_kzalloc(&pdev->dev,
				adsp_fw_cnt * sizeof(u32), GFP_KERNEL);
	if (!adsp_fw_bit_values)
		goto wqueue;

	/* Read bit values corresponding to each firmware image entry */
	ret = of_property_read_u32_array(pdev->dev.of_node,
					 "adsp-fw-bit-values",
					 adsp_fw_bit_values,
					 adsp_fw_cnt);
	if (ret) {
		dev_dbg(&pdev->dev, "%s: unable to read fw-bit-values\n",
			__func__);
		goto wqueue;
	}

	adsp_fw_name_array = devm_kzalloc(&pdev->dev,
				adsp_fw_cnt * sizeof(char *), GFP_KERNEL);
	if (!adsp_fw_name_array)
		goto wqueue;

	/* Read ADSP firmware image names */
	ret = of_property_read_string_array(pdev->dev.of_node,
					 "adsp-fw-names",
					 adsp_fw_name_array,
					 adsp_fw_cnt);
	if (ret < 0) {
		dev_dbg(&pdev->dev, "%s: unable to read fw-names\n",
			__func__);
		goto wqueue;
	}

	adsp_dtb_fw_name_array = devm_kzalloc(&pdev->dev,
				adsp_fw_cnt * sizeof(char *), GFP_KERNEL);

	/* Read ADSP dtb firmware image names */
	ret = of_property_read_string_array(pdev->dev.of_node,
					"adsp-dtb-fw-names",
					adsp_dtb_fw_name_array,
					adsp_fw_cnt);
	if (ret < 0) {
		dev_dbg(&pdev->dev, "%s: unable to read adsp-dtb-fw-names\n",
			__func__);
		goto wqueue;
	}

	for (i = 0; i < adsp_fw_cnt; i++) {
		if (adsp_fw_bit_values[i] == adsp_var_idx) {
			fw_name_size = strlen(adsp_fw_name_array[i]) + 1;
			priv->adsp_fw_name = devm_kzalloc(&pdev->dev,
						fw_name_size,
						GFP_KERNEL);
			if (!priv->adsp_fw_name)
				goto wqueue;
			strlcpy(priv->adsp_fw_name, adsp_fw_name_array[i],
				fw_name_size);

			fw_name_size = strlen(adsp_dtb_fw_name_array[i]) + 1;
			priv->adsp_dtb_name = devm_kzalloc(&pdev->dev,
						fw_name_size,
						GFP_KERNEL);
			if (!priv->adsp_dtb_name)
				goto wqueue;
			strscpy(priv->adsp_dtb_name, adsp_dtb_fw_name_array[i],
				fw_name_size);
			break;
		}
	}
wqueue:
	INIT_WORK(&adsp_ldr_work, adsp_load_fw);
	if (adsp_fw_bit_values)
		devm_kfree(&pdev->dev, adsp_fw_bit_values);
	if (adsp_fw_name_array)
		devm_kfree(&pdev->dev, adsp_fw_name_array);

	return 0;

}

static const struct of_device_id adsp_loader_dt_match[] = {
	{ .compatible = "qcom,adsp-loader" },
	{ }
};
MODULE_DEVICE_TABLE(of, adsp_loader_dt_match);

static struct platform_driver adsp_loader_driver = {
	.driver = {
		.name = "adsp-loader",
		.owner = THIS_MODULE,
		.of_match_table = adsp_loader_dt_match,
		.suppress_bind_attrs = true,
	},
	.probe = adsp_loader_probe,
	.remove = adsp_loader_remove,
};

static int __init adsp_loader_init(void)
{
	return platform_driver_register(&adsp_loader_driver);
}
module_init(adsp_loader_init);

static void __exit adsp_loader_exit(void)
{
	platform_driver_unregister(&adsp_loader_driver);
}
module_exit(adsp_loader_exit);

MODULE_DESCRIPTION("ADSP Loader module");
MODULE_LICENSE("GPL v2");
