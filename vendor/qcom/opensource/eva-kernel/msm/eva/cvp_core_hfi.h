/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) 2018-2021, The Linux Foundation. All rights reserved.
 * Copyright (c) 2023 Qualcomm Innovation Center, Inc. All rights reserved.
 */

#ifndef __H_CVP_CORE_HFI_H__
#define __H_CVP_CORE_HFI_H__

#include <linux/clk.h>
#include <linux/clk-provider.h>
#include <linux/mutex.h>
#include <linux/platform_device.h>
#include <linux/pm_qos.h>
#include <linux/spinlock.h>
#include <linux/interrupt.h>
#include <linux/soc/qcom/msm_mmrm.h>
#include "cvp_hfi_api.h"
#include "cvp_hfi_helper.h"
#include "cvp_hfi_api.h"
#include "cvp_hfi.h"
#include "msm_cvp_resources.h"
#include "hfi_packetization.h"

#define HFI_MASK_QHDR_TX_TYPE			0xFF000000
#define HFI_MASK_QHDR_RX_TYPE			0x00FF0000
#define HFI_MASK_QHDR_PRI_TYPE			0x0000FF00
#define HFI_MASK_QHDR_Q_ID_TYPE			0x000000FF
#define HFI_Q_ID_HOST_TO_CTRL_CMD_Q		0x00
#define HFI_Q_ID_CTRL_TO_HOST_MSG_Q		0x01
#define HFI_Q_ID_CTRL_TO_HOST_DEBUG_Q	0x02
#define HFI_MASK_QHDR_STATUS			0x000000FF

#define CVP_IFACEQ_NUMQ					3
#define CVP_IFACEQ_CMDQ_IDX				0
#define CVP_IFACEQ_MSGQ_IDX				1
#define CVP_IFACEQ_DBGQ_IDX				2
#define CVP_IFACEQ_MAX_BUF_COUNT			50
#define CVP_IFACE_MAX_PARALLEL_CLNTS		16
#define CVP_IFACEQ_DFLT_QHDR				0x01010000

#define CVP_MAX_NAME_LENGTH 64
#define CVP_MAX_PC_SKIP_COUNT 10
#define CVP_MAX_SUBCACHES 4
#define CVP_MAX_SUBCACHE_SIZE 52

struct cvp_hfi_queue_table_header {
	u32 qtbl_version;
	u32 qtbl_size;
	u32 qtbl_qhdr0_offset;
	u32 qtbl_qhdr_size;
	u32 qtbl_num_q;
	u32 qtbl_num_active_q;
	void *device_addr;
	char name[256];
};

struct cvp_hfi_queue_header {
	u32 qhdr_status;
	u32 qhdr_start_addr;
	u32 qhdr_type;
	u32 qhdr_q_size;
	u32 qhdr_pkt_size;
	u32 qhdr_pkt_drop_cnt;
	u32 qhdr_rx_wm;
	u32 qhdr_tx_wm;
	u32 qhdr_rx_req;
	u32 qhdr_tx_req;
	u32 qhdr_rx_irq_status;
	u32 qhdr_tx_irq_status;
	u32 qhdr_read_idx;
	u32 qhdr_write_idx;
};

struct cvp_hfi_mem_map_table {
	u32 mem_map_num_entries;
	u32 mem_map_table_base_addr;
};

struct cvp_hfi_mem_map {
	u32 virtual_addr;
	u32 physical_addr;
	u32 size;
	u32 attr;
};

#define CVP_IFACEQ_TABLE_SIZE (sizeof(struct cvp_hfi_queue_table_header) \
	+ sizeof(struct cvp_hfi_queue_header) * CVP_IFACEQ_NUMQ)

#define CVP_IFACEQ_QUEUE_SIZE	(CVP_IFACEQ_MAX_PKT_SIZE *  \
	CVP_IFACEQ_MAX_BUF_COUNT * CVP_IFACE_MAX_PARALLEL_CLNTS)

#define CVP_IFACEQ_GET_QHDR_START_ADDR(ptr, i)     \
	(void *)((ptr + sizeof(struct cvp_hfi_queue_table_header)) + \
		(i * sizeof(struct cvp_hfi_queue_header)))

#define QDSS_SIZE 4096
#define SFR_SIZE 1048576

#define QUEUE_SIZE (CVP_IFACEQ_TABLE_SIZE + \
	(CVP_IFACEQ_QUEUE_SIZE * CVP_IFACEQ_NUMQ))

#define ALIGNED_QDSS_SIZE ALIGN(QDSS_SIZE, SZ_4K)
#define ALIGNED_SFR_SIZE ALIGN(SFR_SIZE, SZ_4K)
#define ALIGNED_QUEUE_SIZE ALIGN(QUEUE_SIZE, SZ_4K)
#define SHARED_QSIZE ALIGN(ALIGNED_SFR_SIZE + ALIGNED_QUEUE_SIZE + \
			ALIGNED_QDSS_SIZE, SZ_1M)

struct cvp_mem_addr {
	u32 align_device_addr;
	u8 *align_virtual_addr;
	u32 mem_size;
	struct msm_cvp_smem mem_data;
};

struct cvp_iface_q_info {
	spinlock_t hfi_lock;
	void *q_hdr;
	struct cvp_mem_addr q_array;
};

/*
 * These are helper macros to iterate over various lists within
 * iris_hfi_device->res.  The intention is to cut down on a lot of boiler-plate
 * code
 */

/* Read as "for each 'thing' in a set of 'thingies'" */
#define iris_hfi_for_each_thing(__device, __thing, __thingy) \
	iris_hfi_for_each_thing_continue(__device, __thing, __thingy, 0)

#define iris_hfi_for_each_thing_reverse(__device, __thing, __thingy) \
	iris_hfi_for_each_thing_reverse_continue(__device, __thing, __thingy, \
			(__device)->res->__thingy##_set.count - 1)

/* TODO: the __from parameter technically not required since we can figure it
 * out with some pointer magic (i.e. __thing - __thing##_tbl[0]).  If this macro
 * sees extensive use, probably worth cleaning it up but for now omitting it
 * since it introduces unnecessary complexity.
 */
#define iris_hfi_for_each_thing_continue(__device, __thing, __thingy, __from) \
	for (__thing = &(__device)->res->\
			__thingy##_set.__thingy##_tbl[__from]; \
		__thing < &(__device)->res->__thingy##_set.__thingy##_tbl[0] + \
			((__device)->res->__thingy##_set.count - __from); \
		++__thing)

#define iris_hfi_for_each_thing_reverse_continue(__device, __thing, __thingy, \
		__from) \
	for (__thing = &(__device)->res->\
			__thingy##_set.__thingy##_tbl[__from]; \
		__thing >= &(__device)->res->__thingy##_set.__thingy##_tbl[0]; \
		--__thing)

/* Regular set helpers */
#define iris_hfi_for_each_regulator(__device, __rinfo) \
	iris_hfi_for_each_thing(__device, __rinfo, regulator)

#define iris_hfi_for_each_regulator_reverse(__device, __rinfo) \
	iris_hfi_for_each_thing_reverse(__device, __rinfo, regulator)

#define iris_hfi_for_each_regulator_reverse_continue(__device, __rinfo, \
		__from) \
	iris_hfi_for_each_thing_reverse_continue(__device, __rinfo, \
			regulator, __from)

/* Clock set helpers */
#define iris_hfi_for_each_clock(__device, __cinfo) \
	iris_hfi_for_each_thing(__device, __cinfo, clock)

#define iris_hfi_for_each_clock_reverse(__device, __cinfo) \
	iris_hfi_for_each_thing_reverse(__device, __cinfo, clock)

#define iris_hfi_for_each_clock_reverse_continue(__device, __rinfo, \
		__from) \
	iris_hfi_for_each_thing_reverse_continue(__device, __rinfo, \
			clock, __from)

/* reset set helpers */
#define iris_hfi_for_each_reset_clock(__device, __resetinfo) \
	iris_hfi_for_each_thing(__device, __resetinfo, reset)

#define iris_hfi_for_each_reset_clock_reverse(__device, __resetinfo) \
	iris_hfi_for_each_thing_reverse(__device, __resetinfo, reset)

/* Bus set helpers */
#define iris_hfi_for_each_bus(__device, __binfo) \
	iris_hfi_for_each_thing(__device, __binfo, bus)
#define iris_hfi_for_each_bus_reverse(__device, __binfo) \
	iris_hfi_for_each_thing_reverse(__device, __binfo, bus)

/* Subcache set helpers */
#define iris_hfi_for_each_subcache(__device, __sinfo) \
	iris_hfi_for_each_thing(__device, __sinfo, subcache)
#define iris_hfi_for_each_subcache_reverse(__device, __sinfo) \
	iris_hfi_for_each_thing_reverse(__device, __sinfo, subcache)

#define call_iris_op(d, op, args...)			\
	(((d) && (d)->hal_ops && (d)->hal_ops->op) ? \
	((d)->hal_ops->op(args)):0)

struct cvp_hal_data {
	u32 irq;
	u32 irq_wd;
	phys_addr_t firmware_base;
	u8 __iomem *register_base;
	u8 __iomem *gcc_reg_base;
	u32 register_size;
	u32 gcc_reg_size;
};

struct iris_resources {
	struct msm_cvp_fw fw;
};

enum iris_hfi_state {
	IRIS_STATE_DEINIT = 1,
	IRIS_STATE_INIT,
};

enum reset_state {
	INIT = 1,
	ASSERT,
	DEASSERT,
};

/* Indices of hfi queues in hfi queue arrays (iface_queues & dsp_iface_queues) */
enum hfi_queue_idx {
	CMD_Q, /* Command queue */
	MSG_Q, /* Message queue */
	DEBUG_Q, /* Debug queue */
	MAX_Q
};

struct iris_hfi_device;

struct cvp_hal_ops {
	void (*interrupt_init)(struct iris_hfi_device *ptr);
	void (*setup_dsp_uc_memmap)(struct iris_hfi_device *device);
	void (*clock_config_on_enable)(struct iris_hfi_device *device);
	void (*power_off)(struct iris_hfi_device *device);
	void (*noc_error_info)(struct iris_hfi_device *device);
	int (*reset_control_assert_name)(struct iris_hfi_device *device, const char *name);
	int (*reset_control_deassert_name)(struct iris_hfi_device *device, const char *name);
	int (*reset_control_acquire_name)(struct iris_hfi_device *device, const char *name);
	int (*reset_control_release_name)(struct iris_hfi_device *device, const char *name);
};

struct iris_hfi_device {
	struct list_head sess_head;
	u32 version;
	u32 intr_status;
	u32 clk_freq;
	u32 last_packet_type;
	u32 error;
	unsigned long clk_bitrate;
	unsigned long scaled_rate;
	struct msm_cvp_gov_data bus_vote;
	bool power_enabled;
	bool reg_dumped;
	struct mutex lock;
	msm_cvp_callback callback;
	struct cvp_mem_addr iface_q_table;
	struct cvp_mem_addr dsp_iface_q_table;
	struct cvp_mem_addr qdss;
	struct cvp_mem_addr sfr;
	struct cvp_mem_addr mem_addr;
	struct cvp_iface_q_info iface_queues[CVP_IFACEQ_NUMQ];
	struct cvp_iface_q_info dsp_iface_queues[CVP_IFACEQ_NUMQ];
	struct cvp_hal_data *cvp_hal_data;
	struct workqueue_struct *cvp_workq;
	struct workqueue_struct *iris_pm_workq;
	int spur_count;
	int reg_count;
	struct iris_resources resources;
	struct msm_cvp_platform_resources *res;
	struct mmrm_client_desc mmrm_desc;
	struct mmrm_client *mmrm_cvp;
	enum iris_hfi_state state;
	struct cvp_hfi_packetization_ops *pkt_ops;
	enum hfi_packetization_type packetization_type;
	struct msm_cvp_cb_info *response_pkt;
	u8 *raw_packet;
	struct pm_qos_request qos;
	unsigned int skip_pc_count;
	struct msm_cvp_capability *sys_init_capabilities;
	struct cvp_hal_ops *hal_ops;
};

irqreturn_t cvp_hfi_isr(int irq, void *dev);
irqreturn_t iris_hfi_core_work_handler(int irq, void *data);
irqreturn_t iris_hfi_isr_wd(int irq, void *dev);
void cvp_iris_hfi_delete_device(void *device);

int cvp_iris_hfi_initialize(struct cvp_hfi_ops *hdev,
		struct msm_cvp_platform_resources *res,
		hfi_cmd_response_callback callback);

int load_cvp_fw_impl(struct iris_hfi_device *device);
int unload_cvp_fw_impl(struct iris_hfi_device *device);
void cvp_dump_csr(struct iris_hfi_device *dev);
#endif
