/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) 2019, The Linux Foundation. All rights reserved.
 * Copyright (c) 2022 Qualcomm Innovation Center, Inc. All rights reserved.
 */

#ifndef _CAM_SOC_BUS_H_
#define _CAM_SOC_BUS_H_

#include <linux/of.h>
#include <linux/slab.h>
#include <linux/platform_device.h>
#include "cam_debug_util.h"
#include "cam_cpas.h"

#define CAM_SOC_BUS_MAX_NUM_USECASES 9

enum cam_soc_bus_path_data {
	CAM_SOC_BUS_PATH_DATA_HLOS,
	CAM_SOC_BUS_PATH_DATA_DRV_HIGH,
	CAM_SOC_BUS_PATH_DATA_DRV_LOW,
	CAM_SOC_BUS_PATH_DATA_MAX,
};

/**
 * struct cam_soc_bus_client_ab_ib : Bandwidth values for selected usecase
 *
 * @ab: Arbitrated Bandwidth
 * @ib: Instantaneous Bandwidth
 */
struct cam_soc_bus_client_ab_ib {
	uint64_t ab;
	uint64_t ib;
};

/**
 * struct cam_soc_bus_client_common_data : Common data fields for bus client
 *
 * @name: Name of bus client
 * @src_id: Bus master/src id
 * @dst_id: Bus slave/dst id
 * @is_drv_port: If DRV bus client
 * @num_usecases: Number of use cases for this client
 * @bw_pair: Bandwidth values for applicable usecases
 */
struct cam_soc_bus_client_common_data {
	const char *name;
	uint32_t src_id;
	uint32_t dst_id;
	bool is_drv_port;
	int num_usecases;
	struct cam_soc_bus_client_ab_ib bw_pair[CAM_SOC_BUS_MAX_NUM_USECASES];
};

/**
 * struct cam_soc_bus_client : Bus client information
 *
 * @client_data: Bus client data
 * @common_data: Common data fields for bus client
 */
struct cam_soc_bus_client {
	void *client_data;
	struct cam_soc_bus_client_common_data *common_data;
};


#if IS_REACHABLE(CONFIG_QCOM_BUS_SCALING) || \
	IS_REACHABLE(CONFIG_INTERCONNECT_QCOM)

const char *cam_soc_bus_path_data_to_str(enum cam_soc_bus_path_data bus_path_data);

int cam_soc_bus_client_update_request(void *client, unsigned int idx);

int cam_soc_bus_client_update_bw(void *client, uint64_t ab, uint64_t ib,
	enum cam_soc_bus_path_data bus_path_data);

int cam_soc_bus_client_register(struct platform_device *pdev,
	struct device_node *dev_node, void **client,
	struct cam_soc_bus_client_common_data *common_data);

void cam_soc_bus_client_unregister(void **client);

#else

static const char *cam_soc_bus_path_data_to_str(enum cam_soc_bus_path_data bus_path_data)
{
	return NULL;
}

static inline int cam_soc_bus_client_update_request(void *client,
	unsigned int idx)
{
	return 0;
}

int cam_soc_bus_client_update_bw(void *client, uint64_t ab, uint64_t ib,
	enum cam_soc_bus_path_data bus_path_data)
{
	return 0;
}

static inline int cam_soc_bus_client_register(
	struct platform_device *pdev, struct device_node *dev_node,
	void **client, struct cam_soc_bus_client_common_data *common_data)
{
	return 0;
}

static inline void cam_soc_bus_client_unregister(void **client)
{
}

#endif

#endif /* _CAM_SOC_BUS_H_ */
