/*
 * Copyright (c) 2016-2021 The Linux Foundation. All rights reserved.
 * Copyright (c) 2021-2024 Qualcomm Innovation Center, Inc. All rights reserved.
 *
 *
 * Permission to use, copy, modify, and/or distribute this software for
 * any purpose with or without fee is hereby granted, provided that the
 * above copyright notice and this permission notice appear in all
 * copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 * AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 * PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef _WLAN_LMAC_IF_DEF_H_
#define _WLAN_LMAC_IF_DEF_H_

#include "qdf_status.h"
#include "wlan_objmgr_cmn.h"
#ifdef DFS_COMPONENT_ENABLE
#include <wlan_dfs_public_struct.h>
#endif
#include "wlan_mgmt_txrx_utils_api.h"
#include "wlan_scan_public_structs.h"

#ifdef WLAN_ATF_ENABLE
#include "wlan_atf_utils_defs.h"
#endif
#ifdef WLAN_SA_API_ENABLE
#include "wlan_sa_api_utils_defs.h"
#endif
#ifdef WLAN_CONV_SPECTRAL_ENABLE
#include "wlan_spectral_public_structs.h"
#endif
#include <reg_services_public_struct.h>

#include "wlan_crypto_global_def.h"
#include "wifi_pos_public_struct.h"

#ifdef WLAN_CFR_ENABLE
#include "wlan_cfr_utils_api.h"
#endif

#include <wlan_dfs_tgt_api.h>
#include <wlan_dfs_ioctl.h>

#ifdef WLAN_IOT_SIM_SUPPORT
#include <wlan_iot_sim_public_structs.h>
#endif
#include <wlan_mgmt_txrx_rx_reo_public_structs.h>

#ifdef IPA_OFFLOAD
#include <wlan_ipa_public_struct.h>
#endif

#ifdef WLAN_FEATURE_11BE_MLO_ADV_FEATURE
#include <wlan_mlo_mgr_link_switch.h>
#endif

/* Number of dev type: Direct attach and Offload */
#define MAX_DEV_TYPE 2

#ifdef WIFI_POS_CONVERGED
/* forward declarations */
struct oem_data_req;
struct oem_data_rsp;
#endif /* WIFI_POS_CONVERGED */

#ifdef DIRECT_BUF_RX_ENABLE
/* forward declarations for direct buf rx */
struct direct_buf_rx_data;
/* Forward declaration for module_ring_params */
struct module_ring_params;
/*Forward declaration for dbr_module_config */
struct dbr_module_config;
#endif

#ifdef FEATURE_WLAN_TDLS
#include "wlan_tdls_public_structs.h"
#endif

#include <wlan_vdev_mgr_tgt_if_tx_defs.h>
#include <wlan_vdev_mgr_tgt_if_rx_defs.h>

#ifdef DCS_INTERFERENCE_DETECTION
#include <wlan_dcs_tgt_api.h>
#endif

#ifdef WLAN_FEATURE_11BE_MLO
#include "wlan_mlo_mgr_public_structs.h"
#endif
#if defined(WLAN_SUPPORT_TWT) && defined(WLAN_TWT_CONV_SUPPORTED)
#include "wlan_twt_public_structs.h"
#endif

#ifdef QCA_SUPPORT_CP_STATS
#include <wlan_cp_stats_public_structs.h>

#ifdef WLAN_FEATURE_DBAM_CONFIG
#include "wlan_coex_public_structs.h"
#endif

#ifdef WLAN_FEATURE_COAP
#include "wlan_coap_public_structs.h"
#endif

/**
 * typedef cp_stats_event - Definition of cp stats event
 * Define stats_event from external cp stats component to cp_stats_event
 */
typedef struct stats_event cp_stats_event;
/**
 * typedef stats_request_type - Definition of stats_req_type enum
 * Define stats_req_type from external cp stats component to stats_request_type
 */
typedef enum stats_req_type stats_request_type;
/**
 * typedef stats_req_info - Definition of cp stats req info
 * Define request_info from external cp stats component to stats_req_info
 */
typedef struct request_info stats_req_info;
/**
 * typedef stats_wake_lock - Definition of cp stats wake lock
 * Define wake_lock_stats from external cp stats component to stats_wake_lock
 */
typedef struct wake_lock_stats stats_wake_lock;

/**
 * typedef stats_big_data_stats_event - Definition of big data cp stats
 * Define big_data_stats_event from external cp stats component to
 * big_data_stats_event
 */
typedef struct big_data_stats_event stats_big_data_stats_event;

/**
 * struct wlan_lmac_if_cp_stats_tx_ops - defines southbound tx callbacks for
 * control plane statistics component
 * @cp_stats_attach: function pointer to register events from FW
 * @cp_stats_detach: function pointer to unregister events from FW
 * @cp_stats_legacy_attach: function pointer to register legacy stats events
 *                          from FW
 * @cp_stats_legacy_detach: function pointer to unregister legacy stats events
 *                          from FW
 * @inc_wake_lock_stats: function pointer to increase wake lock stats
 * @send_req_stats: function pointer to send request stats command to FW
 * @send_req_peer_stats: function pointer to send request peer stats command
 *                       to FW
 * @set_pdev_stats_update_period: function pointer to set pdev stats update
 *                                period to FW
 * @send_req_infra_cp_stats: function pointer to send infra cp stats request
 *                           command to FW
 * @send_req_big_data_stats: Function pointer to send big data stats
 * @send_req_telemetry_cp_stats: API to send stats request to wmi
 * @send_cstats_enable: Sends Pdev set param command to enable chipset stats
 */
struct wlan_lmac_if_cp_stats_tx_ops {
	QDF_STATUS (*cp_stats_attach)(struct wlan_objmgr_psoc *psoc);
	QDF_STATUS (*cp_stats_detach)(struct wlan_objmgr_psoc *posc);
	QDF_STATUS (*cp_stats_legacy_attach)(struct wlan_objmgr_psoc *psoc);
	QDF_STATUS (*cp_stats_legacy_detach)(struct wlan_objmgr_psoc *psoc);
	void (*inc_wake_lock_stats)(uint32_t reason,
				    stats_wake_lock *stats,
				    uint32_t *unspecified_wake_count);
	QDF_STATUS (*send_req_stats)(struct wlan_objmgr_psoc *psoc,
				     enum stats_req_type type,
				     stats_req_info *req);
	QDF_STATUS (*send_req_peer_stats)(struct wlan_objmgr_psoc *psoc,
					  stats_req_info *req);
	QDF_STATUS (*set_pdev_stats_update_period)(
					struct wlan_objmgr_psoc *psoc,
					uint8_t pdev_id, uint32_t val);
#ifdef WLAN_SUPPORT_INFRA_CTRL_PATH_STATS
	QDF_STATUS (*send_req_infra_cp_stats)(
					struct wlan_objmgr_psoc *psoc,
					struct infra_cp_stats_cmd_info *req);
#endif
#ifdef WLAN_FEATURE_BIG_DATA_STATS
	QDF_STATUS (*send_req_big_data_stats)(
					struct wlan_objmgr_psoc *psoc,
					stats_req_info *req);
#endif
#ifdef WLAN_CONFIG_TELEMETRY_AGENT
	QDF_STATUS (*send_req_telemetry_cp_stats)(
					struct wlan_objmgr_pdev *pdev,
					struct infra_cp_stats_cmd_info *req);
#endif
#ifdef WLAN_CHIPSET_STATS
	QDF_STATUS (*send_cstats_enable)(struct wlan_objmgr_psoc *psoc,
					 uint32_t param_val, uint8_t mac_id);
#endif
};

/**
 * struct wlan_lmac_if_cp_stats_rx_ops - defines southbound rx callbacks for
 * control plane statistics component
 * @cp_stats_rx_event_handler:	function pointer to rx FW events
 * @process_stats_event: function pointer to process stats event
 * @process_infra_stats_event:
 * @process_big_data_stats_event:
 * @twt_get_session_param_resp:
 */
struct wlan_lmac_if_cp_stats_rx_ops {
	QDF_STATUS (*cp_stats_rx_event_handler)(struct wlan_objmgr_vdev *vdev);
	QDF_STATUS (*process_stats_event)(struct wlan_objmgr_psoc *psoc,
					  struct stats_event *ev);
#ifdef WLAN_SUPPORT_INFRA_CTRL_PATH_STATS
	QDF_STATUS
	(*process_infra_stats_event)(struct wlan_objmgr_psoc *psoc,
				     struct infra_cp_stats_event *infra_event);
#endif /* WLAN_SUPPORT_INFRA_CTRL_PATH_STATS */
#ifdef WLAN_FEATURE_BIG_DATA_STATS
	QDF_STATUS (*process_big_data_stats_event)(
					struct wlan_objmgr_psoc *psoc,
					stats_big_data_stats_event *ev);
#endif
#if defined(WLAN_SUPPORT_TWT) && defined(WLAN_TWT_CONV_SUPPORTED)
	QDF_STATUS (*twt_get_session_param_resp)(struct wlan_objmgr_psoc *psoc,
					struct twt_session_stats_info *params);
#endif
};
#endif

#ifdef DCS_INTERFERENCE_DETECTION
/**
 * struct wlan_target_if_dcs_tx_ops - south bound tx function pointers for dcs
 * @dcs_attach: function to register event handlers with FW
 * @dcs_detach: function to de-register event handlers with FW
 * @dcs_cmd_send: function to send dcs commands to FW
 */
struct wlan_target_if_dcs_tx_ops {
	QDF_STATUS (*dcs_attach)(struct wlan_objmgr_psoc *psoc);
	QDF_STATUS (*dcs_detach)(struct wlan_objmgr_psoc *psoc);
	QDF_STATUS (*dcs_cmd_send)(struct wlan_objmgr_psoc *psoc,
				   uint32_t pdev_id,
				   bool is_host_pdev_id,
				   uint32_t dcs_enable);
};

/**
 * struct wlan_target_if_dcs_rx_ops - defines southbound rx callbacks for
 * dcs component
 * @process_dcs_event:  function pointer to rx FW events
 */
struct wlan_target_if_dcs_rx_ops {
	QDF_STATUS (*process_dcs_event)(struct wlan_objmgr_psoc *psoc,
					struct wlan_host_dcs_event *event);
};
#endif

#ifdef WLAN_MLO_GLOBAL_SHMEM_SUPPORT
/**
 * struct wlan_lmac_if_global_shmem_local_ops - local ops function pointer
 * table of local shared mem arena
 * @implemented: Whether functions pointers are implemented
 * @init_shmem_arena_ctx: Initialize shmem arena context
 * @deinit_shmem_arena_ctx: De-initialize shmem arena context
 * @get_crash_reason_address: Get the address of the crash reason associated
 * with chip_id
 * @get_recovery_mode_address: Get the address of the recovery mode associated
 * with chip_id
 * @get_no_of_chips_from_crash_info: Get the number of chips participated in the
 * mlo from global shmem crash info
 */
struct wlan_lmac_if_global_shmem_local_ops {
	bool implemented;

	QDF_STATUS (*init_shmem_arena_ctx)(void *arena_vaddr,
					   size_t arena_len,
					   uint8_t grp_id);
	QDF_STATUS (*deinit_shmem_arena_ctx)(uint8_t grp_id);
	void *(*get_crash_reason_address)(uint8_t grp_id,
					  uint8_t chip_id);
	void *(*get_recovery_mode_address)(uint8_t grp_id,
					   uint8_t chip_id);
	uint8_t (*get_no_of_chips_from_crash_info)(uint8_t grp_id);
};
#endif

#ifdef WLAN_MGMT_RX_REO_SUPPORT
/**
 * struct wlan_lmac_if_mgmt_rx_reo_low_level_ops - Low level function pointer
 * table of MGMT Rx REO module
 * @implemented: Whether functions pointers are implemented
 * @get_num_links: Get number of links to be used by MGMT Rx REO module
 * @get_valid_link_bitmap: Get valid link bitmap to be used by MGMT Rx
 * REO module
 * @get_snapshot_address: Get address of an MGMT Rx REO snapshot
 * @get_snapshot_version: Get version of MGMT Rx REO snapshot
 * @snapshot_is_valid: Check if a snapshot is valid
 * @snapshot_get_mgmt_pkt_ctr: Get management packet counter from snapshot
 * @snapshot_get_redundant_mgmt_pkt_ctr: Get redundant management packet counter
 * from snapshot
 * @snapshot_is_consistent: Check if a snapshot is consistent
 * @snapshot_get_global_timestamp: Get global timestamp from snapshot
 */
struct wlan_lmac_if_mgmt_rx_reo_low_level_ops {
	bool implemented;
	int (*get_num_links)(uint8_t grp_id);
	uint16_t (*get_valid_link_bitmap)(uint8_t grp_id);
	void* (*get_snapshot_address)
			(uint8_t grp_id, uint8_t link_id,
			 enum mgmt_rx_reo_shared_snapshot_id snapshot_id);
	int8_t (*get_snapshot_version)
			(uint8_t grp_id,
			 enum mgmt_rx_reo_shared_snapshot_id snapshot_id);
	bool (*snapshot_is_valid)(uint32_t snapshot_low,
				  uint8_t snapshot_version);
	uint16_t (*snapshot_get_mgmt_pkt_ctr)(uint32_t snapshot_low,
					      uint8_t snapshot_version);
	uint16_t (*snapshot_get_redundant_mgmt_pkt_ctr)(uint32_t snapshot_high);
	bool (*snapshot_is_consistent)(uint32_t snapshot_low,
				       uint32_t snapshot_high,
				       uint8_t snapshot_version);
	uint32_t (*snapshot_get_global_timestamp)(uint32_t snapshot_low,
						  uint32_t snapshot_high,
						  uint8_t snapshot_version);
};

/**
 * struct wlan_lmac_if_mgmt_rx_reo_tx_ops - structure of tx function
 * pointers for mgmt rx reo
 * @get_num_active_hw_links: Get number of active MLO HW links
 * @get_valid_hw_link_bitmap: Get valid MLO HW link bitmap
 * @read_mgmt_rx_reo_snapshot: Read rx-reorder snapshots
 * @get_mgmt_rx_reo_snapshot_info: Get rx-reorder snapshot info
 * @mgmt_rx_reo_filter_config:  Configure MGMT Rx REO filter
 * @schedule_delivery: Schedule delivery of management frames
 * @cancel_scheduled_delivery: Cancel schedule delivery of management frames
 * @low_level_ops:  Low level operations of MGMT Rx REO module
 */
struct wlan_lmac_if_mgmt_rx_reo_tx_ops {
	QDF_STATUS (*get_num_active_hw_links)(struct wlan_objmgr_psoc *psoc,
					      int8_t *num_active_hw_links);
	QDF_STATUS (*get_valid_hw_link_bitmap)(struct wlan_objmgr_psoc *psoc,
					       uint16_t *valid_hw_link_bitmap);
	QDF_STATUS (*read_mgmt_rx_reo_snapshot)
			(struct wlan_objmgr_pdev *pdev,
			 struct mgmt_rx_reo_snapshot_info *snapshot_info,
			 enum mgmt_rx_reo_shared_snapshot_id id,
			 struct mgmt_rx_reo_snapshot_params *value,
			 struct mgmt_rx_reo_shared_snapshot (*raw_snapshot)
			 [MGMT_RX_REO_SNAPSHOT_B2B_READ_SWAR_RETRY_LIMIT]);
	QDF_STATUS (*get_mgmt_rx_reo_snapshot_info)
			(struct wlan_objmgr_pdev *pdev,
			 enum mgmt_rx_reo_shared_snapshot_id id,
			 struct mgmt_rx_reo_snapshot_info *snapshot_info);
	QDF_STATUS (*mgmt_rx_reo_filter_config)(
					struct wlan_objmgr_pdev *pdev,
					struct mgmt_rx_reo_filter *filter);
	QDF_STATUS (*schedule_delivery)(struct wlan_objmgr_psoc *psoc);
	QDF_STATUS (*cancel_scheduled_delivery)(struct wlan_objmgr_psoc *psoc);
	struct wlan_lmac_if_mgmt_rx_reo_low_level_ops low_level_ops;
};

/**
 * struct wlan_lmac_if_mgmt_rx_reo_rx_ops - structure of rx function
 * pointers for mgmt rx reo module
 * @fw_consumed_event_handler: FW consumed event handler
 * @host_drop_handler: Handler for the frames that gets dropped in Host before
 * entering REO algorithm
 * @release_frames: Release management frames
 */
struct wlan_lmac_if_mgmt_rx_reo_rx_ops {
	QDF_STATUS (*fw_consumed_event_handler)(
			struct wlan_objmgr_pdev *pdev,
			struct mgmt_rx_reo_params *params);
	QDF_STATUS (*host_drop_handler)(
			struct wlan_objmgr_pdev *pdev,
			struct mgmt_rx_reo_params *params);
	QDF_STATUS (*release_frames)(struct wlan_objmgr_psoc *psoc);
};
#endif

/**
 * struct wlan_lmac_if_mgmt_txrx_tx_ops - structure of tx function
 *                  pointers for mgmt txrx component
 * @mgmt_tx_send: function pointer to transmit mgmt tx frame
 * @beacon_send:  function pointer to transmit beacon frame
 * @fd_action_frame_send: function pointer to transmit FD action frame
 * @tx_drain_nbuf_op: function pointer for any umac nbuf related ops for
 *                    pending mgmt frames cleanup
 * @reg_ev_handler: function pointer to register event handlers
 * @unreg_ev_handler: function pointer to unregister event handlers
 * @mgmt_rx_reo_tx_ops: management rx-reorder txops
 * @rx_frame_legacy_handler: Legacy handler for Rx frames
 */
struct wlan_lmac_if_mgmt_txrx_tx_ops {
	QDF_STATUS (*mgmt_tx_send)(struct wlan_objmgr_vdev *vdev,
			qdf_nbuf_t nbuf, u_int32_t desc_id,
			void *mgmt_tx_params);
	QDF_STATUS (*beacon_send)(struct wlan_objmgr_vdev *vdev,
			qdf_nbuf_t nbuf);
	QDF_STATUS (*fd_action_frame_send)(struct wlan_objmgr_vdev *vdev,
					   qdf_nbuf_t nbuf);
	void (*tx_drain_nbuf_op)(struct wlan_objmgr_pdev *pdev,
				 qdf_nbuf_t nbuf);
	QDF_STATUS (*reg_ev_handler)(struct wlan_objmgr_psoc *psoc);
	QDF_STATUS (*unreg_ev_handler)(struct wlan_objmgr_psoc *psoc);
	QDF_STATUS (*rx_frame_legacy_handler)(
			struct wlan_objmgr_pdev *pdev,
			qdf_nbuf_t buf,
			struct mgmt_rx_event_params *mgmt_rx_params);
#ifdef WLAN_MGMT_RX_REO_SUPPORT
	struct wlan_lmac_if_mgmt_rx_reo_tx_ops mgmt_rx_reo_tx_ops;
#endif
};

/**
 * struct wlan_lmac_if_scan_tx_ops - south bound tx function pointers for scan
 * @scan_start: function to start scan
 * @scan_cancel: function to cancel scan
 * @pno_start: start pno scan
 * @pno_stop: stop pno scan
 * @obss_disable: disable obss scan
 * @scan_reg_ev_handler: function to register for scan events
 * @scan_unreg_ev_handler: function to unregister for scan events
 * @set_chan_list:
 * @is_platform_eht_capable:
 *
 * scan module uses these functions to avail ol/da lmac services
 */
struct wlan_lmac_if_scan_tx_ops {
	QDF_STATUS (*scan_start)(struct wlan_objmgr_pdev *pdev,
			struct scan_start_request *req);
	QDF_STATUS (*scan_cancel)(struct wlan_objmgr_pdev *pdev,
			struct scan_cancel_param *req);
	QDF_STATUS (*pno_start)(struct wlan_objmgr_psoc *psoc,
			struct pno_scan_req_params *req);
	QDF_STATUS (*pno_stop)(struct wlan_objmgr_psoc *psoc,
			uint8_t vdev_id);
	QDF_STATUS (*obss_disable)(struct wlan_objmgr_psoc *psoc,
				   uint8_t vdev_id);
	QDF_STATUS (*scan_reg_ev_handler)(struct wlan_objmgr_psoc *psoc,
			void *arg);
	QDF_STATUS (*scan_unreg_ev_handler)(struct wlan_objmgr_psoc *psoc,
			void *arg);
	QDF_STATUS (*set_chan_list)(struct wlan_objmgr_pdev *pdev, void *arg);
	bool (*is_platform_eht_capable)(struct wlan_objmgr_psoc *psoc,
					uint8_t pdev_id);
};

/**
 * struct wlan_lmac_if_ftm_tx_ops - south bound tx function pointers for ftm
 * @ftm_attach: function to register event handlers with FW
 * @ftm_detach: function to de-register event handlers with FW
 * @ftm_cmd_send: function to send FTM commands to FW
 *
 * ftm module uses these functions to avail ol/da lmac services
 */
struct wlan_lmac_if_ftm_tx_ops {
	QDF_STATUS (*ftm_attach)(struct wlan_objmgr_psoc *psoc);
	QDF_STATUS (*ftm_detach)(struct wlan_objmgr_psoc *psoc);
	QDF_STATUS (*ftm_cmd_send)(struct wlan_objmgr_pdev *pdev,
				uint8_t *buf, uint32_t len, uint8_t mac_id);
};

enum wlan_mlme_cfg_id;
/**
 * struct wlan_lmac_if_mlme_tx_ops - south bound tx function pointers for mlme
 * @get_wifi_iface_id: function to get wifi interface id
 * @vdev_mlme_attach: function to register events
 * @vdev_mlme_detach: function to unregister events
 * @vdev_create_send: function to send vdev create
 * @vdev_start_send: function to send vdev start
 * @vdev_up_send: function to send vdev up
 * @vdev_delete_send: function to send vdev delete
 * @vdev_stop_send: function to send vdev stop
 * @vdev_down_send: function to send vdev down
 * @vdev_set_param_send: function to send vdev parameter
 * @vdev_set_tx_rx_decap_type: function to send vdev tx rx cap/decap type
 * @vdev_set_nac_rssi_send: function to send nac rssi
 * @vdev_set_neighbour_rx_cmd_send: function to send vdev neighbour rx cmd
 * @vdev_sifs_trigger_send: function to send vdev sifs trigger
 * @vdev_set_custom_aggr_size_cmd_send: function to send custom aggr size
 * @vdev_config_ratemask_cmd_send: function to send ratemask
 * @peer_flush_tids_send: function to flush peer tids
 * @multiple_vdev_restart_req_cmd: function to send multiple vdev restart
 * @multiple_vdev_set_param_cmd: function to send multiple vdev param
 * @beacon_send_cmd: function to send beacon
 * @beacon_cmd_send:
 * @beacon_tmpl_send: function to send beacon template
 * @vdev_fils_enable_send:
 * @vdev_bcn_miss_offload_send: function to send beacon miss offload
 * @vdev_sta_ps_param_send: function to sent STA power save config
 * @peer_delete_all_send: function to send vdev delete all peer request
 * @psoc_vdev_rsp_timer_init: function to initialize psoc vdev response timer
 * @psoc_vdev_rsp_timer_deinit: function to deinitialize psoc vdev rsp timer
 * @psoc_vdev_rsp_timer_inuse: function to determine whether the vdev rsp
 * timer is inuse or not
 * @psoc_vdev_rsp_timer_mod: function to modify the time of vdev rsp timer
 * @psoc_wake_lock_init: Initialize psoc wake lock for vdev response timer
 * @psoc_wake_lock_deinit: De-Initialize psoc wake lock for vdev response timer
 * @vdev_mgr_rsp_timer_stop:
 * @get_hw_link_id: Get hw_link_id for pdev
 * @get_psoc_mlo_group_id: Get MLO Group ID for the psoc
 * @get_psoc_mlo_chip_id: Get MLO chip ID for the psoc
 * @target_if_mlo_setup_req: MLO setup request
 * @target_if_mlo_ready: MLO ready event
 * @target_if_mlo_teardown_req: MLO teardown
 * @vdev_send_set_mac_addr: API to send set MAC address request to FW
 * @vdev_peer_set_param_send: API to send peer param to FW
 */
struct wlan_lmac_if_mlme_tx_ops {
	uint32_t (*get_wifi_iface_id) (struct wlan_objmgr_pdev *pdev);
	QDF_STATUS (*vdev_mlme_attach)(struct wlan_objmgr_psoc *psoc);
	QDF_STATUS (*vdev_mlme_detach)(struct wlan_objmgr_psoc *psoc);
	QDF_STATUS (*vdev_create_send)(struct wlan_objmgr_vdev *vdev,
				       struct vdev_create_params *param);
	QDF_STATUS (*vdev_start_send)(struct wlan_objmgr_vdev *vdev,
				      struct vdev_start_params *param);
	QDF_STATUS (*vdev_up_send)(struct wlan_objmgr_vdev *vdev,
				   struct vdev_up_params *param);
	QDF_STATUS (*vdev_delete_send)(struct wlan_objmgr_vdev *vdev,
				       struct vdev_delete_params *param);
	QDF_STATUS (*vdev_stop_send)(struct wlan_objmgr_vdev *vdev,
				     struct vdev_stop_params *param);
	QDF_STATUS (*vdev_down_send)(struct wlan_objmgr_vdev *vdev,
				     struct vdev_down_params *param);
	QDF_STATUS (*vdev_set_param_send)(struct wlan_objmgr_vdev *vdev,
					  struct vdev_set_params *param);
	QDF_STATUS (*vdev_set_tx_rx_decap_type)(struct wlan_objmgr_vdev *vdev,
						enum wlan_mlme_cfg_id param_id,
						uint32_t value);
	QDF_STATUS (*vdev_set_nac_rssi_send)(
				struct wlan_objmgr_vdev *vdev,
				struct vdev_scan_nac_rssi_params *param);
	QDF_STATUS (*vdev_set_neighbour_rx_cmd_send)(
					struct wlan_objmgr_vdev *vdev,
					struct set_neighbour_rx_params *param,
					uint8_t *mac);
	QDF_STATUS (*vdev_sifs_trigger_send)(
					struct wlan_objmgr_vdev *vdev,
					struct sifs_trigger_param *param);
	QDF_STATUS (*vdev_set_custom_aggr_size_cmd_send)(
				struct wlan_objmgr_vdev *vdev,
				struct set_custom_aggr_size_params *param);
	QDF_STATUS (*vdev_config_ratemask_cmd_send)(
					struct wlan_objmgr_vdev *vdev,
					struct config_ratemask_params *param);
	QDF_STATUS (*peer_flush_tids_send)(
					struct wlan_objmgr_vdev *vdev,
					struct peer_flush_params *param);
	QDF_STATUS (*multiple_vdev_restart_req_cmd)(
				struct wlan_objmgr_pdev *pdev,
				struct multiple_vdev_restart_params *param);
	QDF_STATUS (*multiple_vdev_set_param_cmd)(
				struct wlan_objmgr_pdev *pdev,
				struct multiple_vdev_set_param *param);
	QDF_STATUS (*beacon_cmd_send)(struct wlan_objmgr_vdev *vdev,
				      struct beacon_params *param);
	QDF_STATUS (*beacon_tmpl_send)(struct wlan_objmgr_vdev *vdev,
				       struct beacon_tmpl_params *param);
#if defined(WLAN_SUPPORT_FILS) || defined(CONFIG_BAND_6GHZ)
	QDF_STATUS (*vdev_fils_enable_send)(struct wlan_objmgr_vdev *vdev,
					    struct config_fils_params *param);
#endif
	QDF_STATUS (*vdev_bcn_miss_offload_send)(struct wlan_objmgr_vdev *vdev);
	QDF_STATUS (*vdev_sta_ps_param_send)(struct wlan_objmgr_vdev *vdev,
					     struct sta_ps_params *param);
	QDF_STATUS (*peer_delete_all_send)(
					struct wlan_objmgr_vdev *vdev,
					struct peer_delete_all_params *param);
	QDF_STATUS (*psoc_vdev_rsp_timer_init)(
				struct wlan_objmgr_psoc *psoc,
				uint8_t vdev_id);
	void (*psoc_vdev_rsp_timer_deinit)(
				struct wlan_objmgr_psoc *psoc,
				uint8_t vdev_id);
	QDF_STATUS (*psoc_vdev_rsp_timer_inuse)(
				struct wlan_objmgr_psoc *psoc,
				uint8_t vdev_id);
	QDF_STATUS (*psoc_vdev_rsp_timer_mod)(
					struct wlan_objmgr_psoc *psoc,
					uint8_t vdev_id,
					int mseconds);
	void (*psoc_wake_lock_init)(
				struct wlan_objmgr_psoc *psoc);
	void (*psoc_wake_lock_deinit)(
				struct wlan_objmgr_psoc *psoc);
	QDF_STATUS (*vdev_mgr_rsp_timer_stop)(
				struct wlan_objmgr_psoc *psoc,
				struct vdev_response_timer *vdev_rsp,
				enum wlan_vdev_mgr_tgt_if_rsp_bit clear_bit);
#if defined(WLAN_FEATURE_11BE_MLO) && defined(WLAN_MLO_MULTI_CHIP)
	uint16_t (*get_hw_link_id)(struct wlan_objmgr_pdev *pdev);
	uint8_t (*get_psoc_mlo_group_id)(struct wlan_objmgr_psoc *psoc);
	uint8_t (*get_psoc_mlo_chip_id)(struct wlan_objmgr_psoc *psoc);
	QDF_STATUS (*target_if_mlo_setup_req)(struct wlan_objmgr_pdev **pdev,
					      uint8_t num_pdevs,
					      uint8_t grp_id);
	QDF_STATUS (*target_if_mlo_ready)(struct wlan_objmgr_pdev **pdev,
					  uint8_t num_pdevs);
	QDF_STATUS (*target_if_mlo_teardown_req)(struct wlan_objmgr_pdev *pdev,
						 uint32_t grp_id, bool reset,
						 bool standby_active);
#endif
#ifdef WLAN_FEATURE_DYNAMIC_MAC_ADDR_UPDATE
QDF_STATUS (*vdev_send_set_mac_addr)(struct qdf_mac_addr mac_addr,
				     struct qdf_mac_addr mld_addr,
				     struct wlan_objmgr_vdev *vdev);
#endif
	QDF_STATUS (*vdev_peer_set_param_send)(struct wlan_objmgr_vdev *vdev,
					       uint8_t *peer_mac_addr,
					       uint32_t param_id,
					       uint32_t param_value);
};

/**
 * struct wlan_lmac_if_scan_rx_ops  - south bound rx function pointers for scan
 * @scan_ev_handler: scan event handler
 * @scan_set_max_active_scans: set max active scans allowed
 *
 * lmac modules uses this API to post scan events to scan module
 */
struct wlan_lmac_if_scan_rx_ops {
	QDF_STATUS (*scan_ev_handler)(struct wlan_objmgr_psoc *psoc,
		struct scan_event_info *event_info);
	QDF_STATUS (*scan_set_max_active_scans)(struct wlan_objmgr_psoc *psoc,
			uint32_t max_active_scans);
};

#ifdef CONVERGED_P2P_ENABLE

/* forward declarations for p2p tx ops */
struct p2p_ps_config;
struct p2p_lo_start;

/**
 * struct wlan_lmac_if_p2p_tx_ops - structure of tx function pointers
 * for P2P component
 * @set_ps:      function pointer to set power save
 * @lo_start:    function pointer to start listen offload
 * @lo_stop:     function pointer to stop listen offload
 * @set_noa:     function pointer to disable/enable NOA
 * @reg_lo_ev_handler:   function pointer to register lo event handler
 * @reg_noa_ev_handler:  function pointer to register noa event handler
 * @unreg_lo_ev_handler: function pointer to unregister lo event handler
 * @unreg_noa_ev_handler:function pointer to unregister noa event handler
 * @reg_mac_addr_rx_filter_handler: function pointer to register/unregister
 *    set mac addr status event callback.
 * @set_mac_addr_rx_filter_cmd: function pointer to set mac addr rx filter
 * @reg_mcc_quota_ev_handler: function to register mcc_quota event handler
 */
struct wlan_lmac_if_p2p_tx_ops {
	QDF_STATUS (*set_ps)(struct wlan_objmgr_psoc *psoc,
		struct p2p_ps_config *ps_config);
#ifdef FEATURE_P2P_LISTEN_OFFLOAD
	QDF_STATUS (*lo_start)(struct wlan_objmgr_psoc *psoc,
		struct p2p_lo_start *lo_start);
	QDF_STATUS (*lo_stop)(struct wlan_objmgr_psoc *psoc,
		uint32_t vdev_id);
	QDF_STATUS (*reg_lo_ev_handler)(struct wlan_objmgr_psoc *psoc,
			void *arg);
	QDF_STATUS (*unreg_lo_ev_handler)(struct wlan_objmgr_psoc *psoc,
			void *arg);
#endif
	QDF_STATUS (*set_noa)(struct wlan_objmgr_psoc *psoc,
			      uint32_t vdev_id, bool disable_noa);
	QDF_STATUS (*reg_noa_ev_handler)(struct wlan_objmgr_psoc *psoc,
					 void *arg);
	QDF_STATUS (*unreg_noa_ev_handler)(struct wlan_objmgr_psoc *psoc,
			void *arg);
	QDF_STATUS (*reg_mac_addr_rx_filter_handler)(
			struct wlan_objmgr_psoc *psoc, bool reg);
	QDF_STATUS (*set_mac_addr_rx_filter_cmd)(
			struct wlan_objmgr_psoc *psoc,
			struct set_rx_mac_filter *param);
#ifdef WLAN_FEATURE_MCC_QUOTA
	QDF_STATUS (*reg_mcc_quota_ev_handler)(struct wlan_objmgr_psoc *psoc,
					       bool reg);
#endif
};
#endif

#ifdef WLAN_ATF_ENABLE

/**
 * struct wlan_lmac_if_atf_tx_ops - ATF specific tx function pointers
 * @atf_enable_disable:           Set atf peer stats enable/disable
 * @atf_ssid_sched_policy:        Set ssid schedule policy
 * @atf_send_peer_list:           Send atf list of peers
 * @atf_set_grouping:             Set atf grouping
 * @atf_set_group_ac:             Set atf Group AC
 * @atf_send_peer_request:        Send peer requests
 * @atf_set_bwf:                  Set bandwidth fairness
 * @atf_get_peer_airtime:         Get peer airtime
 * @atf_open:                     ATF open
 * @atf_register_event_handler:   ATF register wmi event handlers
 * @atf_unregister_event_handler: ATF unregister wmi event handlers
 * @atf_set_ppdu_stats:           ATF set ppdu stats to get ATF stats
 * @atf_send_peer_list_v2:        Send atf list of peers with increased
 *                                maximum peer support
 * @atf_set_grouping_v2:          Set atf grouping with increased maximum
 *                                peer support
 */
struct wlan_lmac_if_atf_tx_ops {
	int32_t (*atf_enable_disable)(struct wlan_objmgr_vdev *vdev,
				      uint8_t value);
	int32_t (*atf_ssid_sched_policy)(struct wlan_objmgr_vdev *vdev,
					 uint8_t value);
	int32_t (*atf_send_peer_list)(struct wlan_objmgr_pdev *pdev,
				      struct pdev_atf_req *atf_req,
				      uint8_t atf_tput_based);
	int32_t (*atf_set_grouping)(struct wlan_objmgr_pdev *pdev,
				    struct pdev_atf_ssid_group_req *atf_grp_req,
				    uint8_t atf_tput_based);
	int32_t (*atf_set_group_ac)(struct wlan_objmgr_pdev *pdev,
				    struct pdev_atf_group_wmm_ac_req *atf_acreq,
				    uint8_t atf_tput_based);
	int32_t (*atf_send_peer_request)(struct wlan_objmgr_pdev *pdev,
					 struct pdev_atf_peer_ext_request *atfr,
					 uint8_t atf_tput_based);
	int32_t (*atf_set_bwf)(struct wlan_objmgr_pdev *pdev,
			       struct pdev_bwf_req *bwf_req);
	uint32_t (*atf_get_peer_airtime)(struct wlan_objmgr_peer *peer);
	void (*atf_open)(struct wlan_objmgr_psoc *psoc);
	void (*atf_register_event_handler)(struct wlan_objmgr_psoc *psoc);
	void (*atf_unregister_event_handler)(struct wlan_objmgr_psoc *psoc);
	void (*atf_set_ppdu_stats)(struct wlan_objmgr_pdev *pdev,
				   uint8_t value);
#ifdef WLAN_ATF_INCREASED_STA
	int32_t (*atf_send_peer_list_v2)(struct wlan_objmgr_pdev *pdev,
					 struct pdev_atf_req_v2 *atf_req);
	int32_t (*atf_set_grouping_v2)(struct wlan_objmgr_pdev *pdev,
				       struct pdev_atf_ssid_group_req_v2 *req);
#endif
};
#endif

#ifdef WLAN_SUPPORT_FILS
/**
 * struct wlan_lmac_if_fd_tx_ops - FILS Discovery specific Tx function pointers
 * @fd_vdev_config_fils:         Enable and configure FILS Discovery
 * @fd_register_event_handler:   Register swfda WMI event handler
 * @fd_unregister_event_handler: Un-register swfda WMI event handler
 * @fd_offload_tmpl_send:        Send FD template to FW
 */
struct wlan_lmac_if_fd_tx_ops {
	QDF_STATUS (*fd_vdev_config_fils)(struct wlan_objmgr_vdev *vdev,
					  uint32_t fd_period);
	void (*fd_register_event_handler)(struct wlan_objmgr_psoc *psoc);
	void (*fd_unregister_event_handler)(struct wlan_objmgr_psoc *psoc);
	QDF_STATUS (*fd_offload_tmpl_send)(struct wlan_objmgr_pdev *pdev,
			struct fils_discovery_tmpl_params *fd_tmpl_param);
};
#endif

#ifdef WLAN_SA_API_ENABLE

/**
 * struct wlan_lmac_if_sa_api_tx_ops - SA API specific tx function pointers
 * @sa_api_register_event_handler: Register event handler for Smart Antenna
 * @sa_api_unregister_event_handler: Unregister event handler for Smart Antenna
 * @sa_api_enable_sa: Enable Smart Antenna
 * @sa_api_set_rx_antenna: Set Rx antenna
 * @sa_api_set_tx_antenna: Set Tx antenna
 * @sa_api_set_tx_default_antenna: Set default Tx antenna
 * @sa_api_set_training_info: Set Smart Antenna training metrics
 * @sa_api_prepare_rateset: Prepare rest set
 * @sa_api_set_node_config_ops: Set Peer config operations structure
 */
struct wlan_lmac_if_sa_api_tx_ops {
	void (*sa_api_register_event_handler)(struct wlan_objmgr_psoc *psoc);
	void (*sa_api_unregister_event_handler)(struct wlan_objmgr_psoc *posc);
	void (*sa_api_enable_sa) (struct wlan_objmgr_pdev *pdev,
			uint32_t enable, uint32_t mode, uint32_t rx_antenna);
	void (*sa_api_set_rx_antenna) (struct wlan_objmgr_pdev *pdev,
			uint32_t antenna);
	void (*sa_api_set_tx_antenna) (struct wlan_objmgr_peer *peer,
			uint32_t *antenna_array);
	void (*sa_api_set_tx_default_antenna) (struct wlan_objmgr_pdev *pdev,
			u_int32_t antenna);
	void (*sa_api_set_training_info) (struct wlan_objmgr_peer *peer,
			uint32_t *rate_array,
			uint32_t *antenna_array,
			uint32_t numpkts,
			uint16_t minpkts,
			uint16_t per_threshold);
	void (*sa_api_prepare_rateset)(struct wlan_objmgr_pdev *pdev,
			struct wlan_objmgr_peer *peer,
			struct sa_rate_info *rate_info);
	void (*sa_api_set_node_config_ops) (struct wlan_objmgr_peer *peer,
			uint32_t cmd_id, uint16_t args_count,
			u_int32_t args_arr[]);
};

#endif

#ifdef WLAN_CFR_ENABLE
/**
 * struct wlan_lmac_if_cfr_tx_ops - CFR specific tx function pointers
 * @cfr_init_pdev: Initialize CFR
 * @cfr_deinit_pdev: De-initialize CFR
 * @cfr_enable_cfr_timer: Function to enable CFR timer
 * @cfr_start_capture: Function to start CFR capture
 * @cfr_stop_capture: Function to stop CFR capture
 * @cfr_config_rcc: Function to set the Repetitive channel capture params
 * @cfr_start_lut_timer: Function to start timer to flush aged-out LUT entries
 * @cfr_stop_lut_timer: Function to stop timer to flush aged-out LUT entries
 * @cfr_default_ta_ra_cfg: Function to configure default values for TA_RA mode
 * @cfr_dump_lut_enh: Function to dump LUT entries
 * @cfr_rx_tlv_process: Function to process PPDU status TLVs
 * @cfr_update_global_cfg: Function to update the global config for
 *                         a successful commit session.
 * @cfr_subscribe_ppdu_desc:
 */
struct wlan_lmac_if_cfr_tx_ops {
	QDF_STATUS (*cfr_init_pdev)(struct wlan_objmgr_psoc *psoc,
				    struct wlan_objmgr_pdev *pdev);
	QDF_STATUS (*cfr_deinit_pdev)(struct wlan_objmgr_psoc *psoc,
				      struct wlan_objmgr_pdev *pdev);
	int (*cfr_enable_cfr_timer)(struct wlan_objmgr_pdev *pdev,
				    uint32_t cfr_timer);
	int (*cfr_start_capture)(struct wlan_objmgr_pdev *pdev,
				 struct wlan_objmgr_peer *peer,
				 struct cfr_capture_params *params);
	int (*cfr_stop_capture)(struct wlan_objmgr_pdev *pdev,
				struct wlan_objmgr_peer *peer);
#ifdef WLAN_ENH_CFR_ENABLE
	QDF_STATUS (*cfr_config_rcc)(struct wlan_objmgr_pdev *pdev,
				     struct cfr_rcc_param *params);
	void (*cfr_start_lut_timer)(struct wlan_objmgr_pdev *pdev);
	void (*cfr_stop_lut_timer)(struct wlan_objmgr_pdev *pdev);
	void (*cfr_default_ta_ra_cfg)(struct cfr_rcc_param *params,
				      bool allvalid, uint16_t reset_cfg);
	void (*cfr_dump_lut_enh)(struct wlan_objmgr_pdev *pdev);
	void (*cfr_rx_tlv_process)(struct wlan_objmgr_pdev *pdev, void *nbuf);
	void (*cfr_update_global_cfg)(struct wlan_objmgr_pdev *pdev);
	QDF_STATUS (*cfr_subscribe_ppdu_desc)(struct wlan_objmgr_pdev *pdev,
					      bool is_subscribe);
#endif
};
#endif /* WLAN_CFR_ENABLE */

#ifdef WLAN_CONV_SPECTRAL_ENABLE
struct spectral_wmi_ops;
struct spectral_tgt_ops;
/**
 * struct wlan_lmac_if_sptrl_tx_ops - Spectral south bound Tx operations
 * @sptrlto_pdev_spectral_init: Initialize target_if pdev Spectral object
 * @sptrlto_pdev_spectral_deinit: De-initialize target_if pdev Spectral object
 * @sptrlto_psoc_spectral_init: Initialize target_if psoc Spectral object
 * @sptrlto_psoc_spectral_deinit: De-initialize target_if psoc Spectral object
 * @sptrlto_set_spectral_config:    Set Spectral configuration
 * @sptrlto_get_spectral_config:    Get Spectral configuration
 * @sptrlto_start_spectral_scan:    Start Spectral Scan
 * @sptrlto_stop_spectral_scan:     Stop Spectral Scan
 * @sptrlto_is_spectral_active:     Get whether Spectral is active
 * @sptrlto_is_spectral_enabled:    Get whether Spectral is enabled
 * @sptrlto_set_icm_active:         Set whether ICM is active or inactive
 * @sptrlto_get_icm_active:         Get whether ICM is active or inactive
 * @sptrlto_get_nominal_nf:         Get Nominal Noise Floor for the current
 *                                  frequency band
 * @sptrlto_set_debug_level:        Set Spectral debug level
 * @sptrlto_get_debug_level:        Get Spectral debug level
 * @sptrlto_get_chaninfo:           Get channel information
 * @sptrlto_clear_chaninfo:         Clear channel information
 * @sptrlto_get_spectral_capinfo:   Get Spectral capability information
 * @sptrlto_get_spectral_diagstats: Get Spectral diagnostic statistics
 * @sptrlto_register_spectral_wmi_ops: Register Spectral WMI operations
 * @sptrlto_register_spectral_tgt_ops: Register Spectral target operations
 * @sptrlto_register_netlink_cb: Register Spectral Netlink callbacks
 * @sptrlto_use_nl_bcast: Get whether to use Netlink broadcast/unicast
 * @sptrlto_deregister_netlink_cb: De-register Spectral Netlink callbacks
 * @sptrlto_process_spectral_report: Process spectral report
 * @sptrlto_set_dma_debug: Set DMA debug for Spectral
 * @sptrlto_direct_dma_support: Whether Direct-DMA is supported on this radio
 * @sptrlto_check_and_do_dbr_ring_debug: Start/Stop Spectral ring debug based
 *                                       on the previous state
 * @sptrlto_check_and_do_dbr_buff_debug: Start/Stop Spectral buffer debug based
 *                                       on the previous state
 * @sptrlto_register_events: Registration of WMI events for Spectral
 * @sptrlto_unregister_events: Unregistration of WMI events for Spectral
 * @sptrlto_init_pdev_feature_caps: Initialize spectral feature capabilities
 **/
struct wlan_lmac_if_sptrl_tx_ops {
	void *(*sptrlto_pdev_spectral_init)(struct wlan_objmgr_pdev *pdev);
	void (*sptrlto_pdev_spectral_deinit)(struct wlan_objmgr_pdev *pdev);
	void *(*sptrlto_psoc_spectral_init)(struct wlan_objmgr_psoc *psoc);
	void (*sptrlto_psoc_spectral_deinit)(struct wlan_objmgr_psoc *psoc);
	QDF_STATUS (*sptrlto_set_spectral_config)
					(struct wlan_objmgr_pdev *pdev,
					 const struct spectral_cp_param *param,
					 const enum spectral_scan_mode smode,
					 enum spectral_cp_error_code *err);
	QDF_STATUS (*sptrlto_get_spectral_config)
					(struct wlan_objmgr_pdev *pdev,
					 struct spectral_config *sptrl_config,
					 enum spectral_scan_mode smode);
	QDF_STATUS (*sptrlto_start_spectral_scan)
					(struct wlan_objmgr_pdev *pdev,
					 uint8_t vdev_id,
					 const enum spectral_scan_mode smode,
					 enum spectral_cp_error_code *err);
	QDF_STATUS (*sptrlto_stop_spectral_scan)
					(struct wlan_objmgr_pdev *pdev,
					 const enum spectral_scan_mode smode,
					 enum spectral_cp_error_code *err);
	bool (*sptrlto_is_spectral_active)(struct wlan_objmgr_pdev *pdev,
					   const enum spectral_scan_mode smode);
	bool (*sptrlto_is_spectral_enabled)(struct wlan_objmgr_pdev *pdev,
					    enum spectral_scan_mode smode);
	QDF_STATUS (*sptrlto_set_debug_level)(struct wlan_objmgr_pdev *pdev,
					      u_int32_t debug_level);
	u_int32_t (*sptrlto_get_debug_level)(struct wlan_objmgr_pdev *pdev);
	QDF_STATUS (*sptrlto_get_spectral_capinfo)
						(struct wlan_objmgr_pdev *pdev,
						 struct spectral_caps *scaps);
	QDF_STATUS (*sptrlto_get_spectral_diagstats)
					(struct wlan_objmgr_pdev *pdev,
					 struct spectral_diag_stats *stats);
	QDF_STATUS (*sptrlto_register_spectral_wmi_ops)(
					struct wlan_objmgr_psoc *psoc,
					struct spectral_wmi_ops *wmi_ops);
	QDF_STATUS (*sptrlto_register_spectral_tgt_ops)(
					struct wlan_objmgr_psoc *psoc,
					struct spectral_tgt_ops *tgt_ops);
	void (*sptrlto_register_netlink_cb)(
		struct wlan_objmgr_pdev *pdev,
		struct spectral_nl_cb *nl_cb);
	bool (*sptrlto_use_nl_bcast)(struct wlan_objmgr_pdev *pdev);
	void (*sptrlto_deregister_netlink_cb)(struct wlan_objmgr_pdev *pdev);
	int (*sptrlto_process_spectral_report)(
		struct wlan_objmgr_pdev *pdev,
		void *payload);
	QDF_STATUS (*sptrlto_set_dma_debug)(
		struct wlan_objmgr_pdev *pdev,
		enum spectral_dma_debug dma_debug_type,
		bool dma_debug_enable);
	bool (*sptrlto_direct_dma_support)(struct wlan_objmgr_pdev *pdev);
	QDF_STATUS (*sptrlto_check_and_do_dbr_ring_debug)(
		struct wlan_objmgr_pdev *pdev);
	QDF_STATUS (*sptrlto_check_and_do_dbr_buff_debug)(
		struct wlan_objmgr_pdev *pdev);
	QDF_STATUS (*sptrlto_register_events)(struct wlan_objmgr_psoc *psoc);
	QDF_STATUS (*sptrlto_unregister_events)(struct wlan_objmgr_psoc *psoc);
	QDF_STATUS (*sptrlto_init_pdev_feature_caps)(
		struct wlan_objmgr_pdev *pdev);
};
#endif /* WLAN_CONV_SPECTRAL_ENABLE */

#ifdef WLAN_IOT_SIM_SUPPORT
/**
 * struct wlan_lmac_if_iot_sim_tx_ops - iot_sim south bound Tx operations
 * @iot_sim_send_cmd: To send wmi simulation command
 **/
struct wlan_lmac_if_iot_sim_tx_ops {
	QDF_STATUS (*iot_sim_send_cmd)(struct wlan_objmgr_pdev *pdev,
				       struct simulation_test_params *param);
};
#endif

#ifdef WIFI_POS_CONVERGED
/*
 * struct wlan_lmac_if_wifi_pos_tx_ops - structure of firmware tx function
 * pointers for wifi_pos component
 * @wifi_pos_register_events: function pointer to register wifi_pos events
 * @wifi_pos_deregister_events: function pointer to deregister wifi_pos events
 * @data_req_tx: function pointer to send wifi_pos req to firmware
 * @wifi_pos_convert_pdev_id_host_to_target: function pointer to get target
 * pdev_id from host pdev_id.
 * @wifi_pos_convert_pdev_id_target_to_host: function pointer to get host
 * pdev_id from target pdev_id.
 * @wifi_pos_get_vht_ch_width: Function pointer to get max supported bw by FW
 * @wifi_pos_parse_measreq_chan_info: Parse channel info from LOWI measurement
 *                                    request buffer.
 * @send_rtt_pasn_auth_status: Send PASN peers authentication status
 * @send_rtt_pasn_deauth: Send PASN peer deauth command
 */
struct wlan_lmac_if_wifi_pos_tx_ops {
	QDF_STATUS (*wifi_pos_register_events)(struct wlan_objmgr_psoc *psoc);
	QDF_STATUS (*wifi_pos_deregister_events)(struct wlan_objmgr_psoc *psoc);
	QDF_STATUS (*data_req_tx)(struct wlan_objmgr_pdev *pdev,
				  struct oem_data_req *req);
	QDF_STATUS (*wifi_pos_convert_pdev_id_host_to_target)(
			struct wlan_objmgr_psoc *psoc, uint32_t host_pdev_id,
			uint32_t *target_pdev_id);
	QDF_STATUS (*wifi_pos_convert_pdev_id_target_to_host)(
			struct wlan_objmgr_psoc *psoc, uint32_t target_pdev_id,
			uint32_t *host_pdev_id);
	QDF_STATUS (*wifi_pos_get_vht_ch_width)(struct wlan_objmgr_psoc *psoc,
						enum phy_ch_width *ch_width);
	QDF_STATUS (*wifi_pos_parse_measreq_chan_info)(
			struct wlan_objmgr_pdev *pdev, uint32_t data_len,
			uint8_t *data, struct rtt_channel_info *chinfo);
	QDF_STATUS (*send_rtt_pasn_auth_status)
			(struct wlan_objmgr_psoc *psoc,
			 struct wlan_pasn_auth_status *data);
	QDF_STATUS (*send_rtt_pasn_deauth)(struct wlan_objmgr_psoc *psoc,
					   struct qdf_mac_addr *peer_mac);
};
#endif

#ifdef DIRECT_BUF_RX_ENABLE
/**
 * struct wlan_lmac_if_direct_buf_rx_tx_ops - structure of direct buf rx txops
 * @direct_buf_rx_module_register: Registration API callback for modules
 *                                 to register with direct buf rx framework
 * @direct_buf_rx_module_unregister: Unregistration API to clean up module
 *                                   specific resources in DBR
 * @direct_buf_rx_register_events: Registration of WMI events for direct
 *                                 buffer rx framework
 * @direct_buf_rx_unregister_events: Unregistraton of WMI events for direct
 *                                   buffer rx framework
 * @direct_buf_rx_print_ring_stat: Print ring status per module per pdev
 *
 * @direct_buf_rx_get_ring_params: Get ring parameters for module_id
 * @direct_buf_rx_start_ring_debug: Start DBR ring debug
 * @direct_buf_rx_stop_ring_debug: Stop DBR ring debug
 * @direct_buf_rx_start_buffer_poisoning: Start DBR buffer poisoning
 * @direct_buf_rx_stop_buffer_poisoning: Stop DBR buffer poisoning
 */
struct wlan_lmac_if_direct_buf_rx_tx_ops {
	QDF_STATUS (*direct_buf_rx_module_register)(
			struct wlan_objmgr_pdev *pdev, uint8_t mod_id,
			struct dbr_module_config *dbr_config,
			bool (*dbr_rsp_handler)
			     (struct wlan_objmgr_pdev *pdev,
			      struct direct_buf_rx_data *dbr_data));
	QDF_STATUS (*direct_buf_rx_module_unregister)(
			struct wlan_objmgr_pdev *pdev, uint8_t mod_id);
	QDF_STATUS (*direct_buf_rx_register_events)(
			struct wlan_objmgr_psoc *psoc);
	QDF_STATUS (*direct_buf_rx_unregister_events)(
			struct wlan_objmgr_psoc *psoc);
	QDF_STATUS (*direct_buf_rx_print_ring_stat)(
			struct wlan_objmgr_pdev *pdev);
	QDF_STATUS (*direct_buf_rx_get_ring_params)
		(struct wlan_objmgr_pdev *pdev,
		 struct module_ring_params *param,
		 uint8_t module_id, uint8_t srng_id);
	QDF_STATUS (*direct_buf_rx_start_ring_debug)(
		struct wlan_objmgr_pdev *pdev, uint8_t mod_id,
		uint32_t num_ring_debug_entries);
	QDF_STATUS (*direct_buf_rx_stop_ring_debug)(
		struct wlan_objmgr_pdev *pdev, uint8_t mod_id);
	QDF_STATUS (*direct_buf_rx_start_buffer_poisoning)(
		struct wlan_objmgr_pdev *pdev, uint8_t mod_id, uint32_t value);
	QDF_STATUS (*direct_buf_rx_stop_buffer_poisoning)(
		struct wlan_objmgr_pdev *pdev, uint8_t mod_id);
};
#endif /* DIRECT_BUF_RX_ENABLE */

#ifdef FEATURE_WLAN_TDLS
/* fwd declarations for tdls tx ops */
struct tdls_info;
struct tdls_peer_update_state;
struct tdls_channel_switch_params;
struct sta_uapsd_trig_params;
/**
 * struct wlan_lmac_if_tdls_tx_ops - south bound tx function pointers for tdls
 * @update_fw_state: function to update tdls firmware state
 * @update_peer_state: function to update tdls peer state
 * @set_offchan_mode: function to set tdls offchannel mode
 * @tdls_reg_ev_handler: function to register for tdls events
 * @tdls_unreg_ev_handler: function to unregister for tdls events
 *
 * tdls module uses these functions to avail ol/da lmac services
 */
struct wlan_lmac_if_tdls_tx_ops {
	QDF_STATUS (*update_fw_state)(struct wlan_objmgr_psoc *psoc,
				     struct tdls_info *req);
	QDF_STATUS (*update_peer_state)(struct wlan_objmgr_psoc *psoc,
				       struct tdls_peer_update_state *param);
	QDF_STATUS (*set_offchan_mode)(struct wlan_objmgr_psoc *psoc,
				      struct tdls_channel_switch_params *param);
	QDF_STATUS (*tdls_reg_ev_handler)(struct wlan_objmgr_psoc *psoc,
					 void *arg);
	QDF_STATUS (*tdls_unreg_ev_handler) (struct wlan_objmgr_psoc *psoc,
					    void *arg);
};

/* fwd declarations for tdls rx ops */
struct tdls_event_info;
/**
 * struct wlan_lmac_if_tdls_rx_ops  - south bound rx function pointers for tdls
 * @tdls_ev_handler: function to handler tdls event
 *
 * lmac modules uses this API to post scan events to tdls module
 */
struct wlan_lmac_if_tdls_rx_ops {
	QDF_STATUS (*tdls_ev_handler)(struct wlan_objmgr_psoc *psoc,
				     struct tdls_event_info *info);
};
#endif

/**
 * struct wlan_lmac_if_ftm_rx_ops  - south bound rx function pointers for FTM
 * @ftm_ev_handler: function to handle FTM event
 *
 * lmac modules uses this API to post FTM events to FTM module
 */
struct wlan_lmac_if_ftm_rx_ops {
	QDF_STATUS (*ftm_ev_handler)(struct wlan_objmgr_pdev *pdev,
					uint8_t *event_buf, uint32_t len);
};

/**
 * struct wlan_lmac_if_reg_tx_ops - structure of tx function
 *                  pointers for regulatory component
 * @register_master_handler: pointer to register event handler
 * @unregister_master_handler:  pointer to unregister event handler
 * @register_master_ext_handler: pointer to register ext event handler
 * @unregister_master_ext_handler: pointer to unregister ext event handler
 * @set_country_code:
 * @fill_umac_legacy_chanlist:
 * @set_wait_for_init_cc_response_event: Wake up the thread that is waiting for
 * the init cc response event.
 * @register_11d_new_cc_handler: pointer to register 11d cc event handler
 * @unregister_11d_new_cc_handler:  pointer to unregister 11d cc event handler
 * @start_11d_scan:
 * @stop_11d_scan:
 * @is_there_serv_ready_extn:
 * @set_user_country_code:
 * @register_ch_avoid_event_handler:
 * @unregister_ch_avoid_event_handler:
 * @send_ctl_info: call-back function to send CTL info to firmware
 * @get_phy_id_from_pdev_id:
 * @get_pdev_id_from_phy_id:
 * @set_tpc_power: send transmit power control info to firmware
 * @get_opclass_tbl_idx: Get opclass table index value
 * @send_afc_ind: send AFC indication info to firmware.
 * @register_afc_event_handler: pointer to register afc event handler
 * @unregister_afc_event_handler: pointer to unregister afc event handler
 * @trigger_acs_for_afc: pointer to trigger acs for afc
 * @trigger_update_channel_list: pointer to trigger_update_channel_list
 * @reg_get_min_psd:
 * @is_chip_11be:
 * @register_rate2power_table_update_event_handler: pointer to register
 *			rate2power table update event handler.
 * @unregister_rate2power_table_update_event_handler: pointer to unregister
 *			rate2power table update event handler.
 * @end_r2p_table_update_wait: Call-back function to end the wait on r2p update
 *			response from fw.
 * @is_80p80_supported: Callback function to check if the device supports a
 * 6GHz 80p80 channel.
 * @is_freq_80p80_supported: Callback function to check if the given primary
 * frequency supports 80P80 mode of operation.
 */
struct wlan_lmac_if_reg_tx_ops {
	QDF_STATUS (*register_master_handler)(struct wlan_objmgr_psoc *psoc,
					      void *arg);
	QDF_STATUS (*unregister_master_handler)(struct wlan_objmgr_psoc *psoc,
						void *arg);
	QDF_STATUS (*register_master_ext_handler)(struct wlan_objmgr_psoc *psoc,
						  void *arg);
	QDF_STATUS (*unregister_master_ext_handler)
				(struct wlan_objmgr_psoc *psoc, void *arg);
	void (*set_wait_for_init_cc_response_event)
			(struct wlan_objmgr_pdev *pdev, QDF_STATUS status);
	QDF_STATUS (*set_country_code)(struct wlan_objmgr_psoc *psoc,
						void *arg);
	QDF_STATUS (*fill_umac_legacy_chanlist)(struct wlan_objmgr_pdev *pdev,
			struct regulatory_channel *cur_chan_list);
	QDF_STATUS (*register_11d_new_cc_handler)(
			struct wlan_objmgr_psoc *psoc, void *arg);
	QDF_STATUS (*unregister_11d_new_cc_handler)(
			struct wlan_objmgr_psoc *psoc, void *arg);
	QDF_STATUS (*start_11d_scan)(struct wlan_objmgr_psoc *psoc,
			struct reg_start_11d_scan_req *reg_start_11d_scan_req);
	QDF_STATUS (*stop_11d_scan)(struct wlan_objmgr_psoc *psoc,
			struct reg_stop_11d_scan_req *reg_stop_11d_scan_req);
	bool (*is_there_serv_ready_extn)(struct wlan_objmgr_psoc *psoc);
	QDF_STATUS (*set_user_country_code)(struct wlan_objmgr_psoc *psoc,
					    uint8_t pdev_id,
					    struct cc_regdmn_s *rd);
	QDF_STATUS (*register_ch_avoid_event_handler)(
			struct wlan_objmgr_psoc *psoc, void *arg);
	QDF_STATUS (*unregister_ch_avoid_event_handler)(
			struct wlan_objmgr_psoc *psoc, void *arg);
	QDF_STATUS (*send_ctl_info)(struct wlan_objmgr_psoc *psoc,
				    struct reg_ctl_params *params);
	QDF_STATUS (*get_phy_id_from_pdev_id)(struct wlan_objmgr_psoc *psoc,
					      uint8_t pdev_id, uint8_t *phy_id);
	QDF_STATUS (*get_pdev_id_from_phy_id)(struct wlan_objmgr_psoc *psoc,
					      uint8_t phy_id, uint8_t *pdev_id);
	QDF_STATUS (*set_tpc_power)(struct wlan_objmgr_psoc *psoc,
				    uint8_t vdev_id,
				    struct reg_tpc_power_info *param);
	QDF_STATUS (*get_opclass_tbl_idx)(struct wlan_objmgr_pdev *pdev,
					  uint8_t *opclass_tbl_idx);
#ifdef CONFIG_AFC_SUPPORT
	QDF_STATUS (*send_afc_ind)(struct wlan_objmgr_psoc *psoc,
				   uint8_t pdev_id,
				   struct reg_afc_resp_rx_ind_info *param);
	QDF_STATUS (*register_afc_event_handler)(struct wlan_objmgr_psoc *psoc,
						 void *arg);
	QDF_STATUS (*unregister_afc_event_handler)
				(struct wlan_objmgr_psoc *psoc, void *arg);
	QDF_STATUS (*trigger_acs_for_afc)(struct wlan_objmgr_pdev *pdev);
	QDF_STATUS (*trigger_update_channel_list)
				(struct wlan_objmgr_pdev *pdev);
	QDF_STATUS (*reg_get_min_psd) (struct wlan_objmgr_pdev *pdev,
				       qdf_freq_t primary_freq,
				       qdf_freq_t cen320,
				       uint16_t punc_pattern, uint16_t bw,
				       int16_t *min_psd);
#endif
	bool (*is_chip_11be)(struct wlan_objmgr_psoc *psoc,
			     uint16_t phy_id);
	QDF_STATUS (*register_rate2power_table_update_event_handler)(
			struct wlan_objmgr_psoc *psoc,
			void *arg);
	QDF_STATUS (*unregister_rate2power_table_update_event_handler)(
			struct wlan_objmgr_psoc *psoc,
			void *arg);
	QDF_STATUS (*end_r2p_table_update_wait)(
			struct wlan_objmgr_psoc *psoc,
			uint32_t pdev_id);
	bool (*is_80p80_supported)(struct wlan_objmgr_pdev *pdev);
	bool (*is_freq_80p80_supported)(struct wlan_objmgr_pdev *pdev,
					qdf_freq_t freq);
};

/**
 * struct wlan_lmac_if_dfs_tx_ops - Function pointer to call offload/lmac
 *                                  functions from DFS module.
 * @dfs_enable:                         Enable DFS.
 * @dfs_get_caps:                       Get DFS capabilities.
 * @dfs_disable:                        Disable DFS
 * @dfs_gettsf64:                       Get tsf64 value.
 * @dfs_set_use_cac_prssi:              Set use_cac_prssi value.
 * @dfs_get_dfsdomain:                  Get DFS domain.
 * @dfs_is_countryCode_CHINA:           Check is country code CHINA.
 * @dfs_get_thresholds:                 Get thresholds.
 * @dfs_get_ext_busy:                   Get ext_busy.
 * @dfs_get_target_type:                Get target type.
 * @dfs_is_countryCode_KOREA_ROC3:      Check is county code Korea.
 * @dfs_get_ah_devid:                   Get ah devid.
 * @dfs_get_phymode_info:               Get phymode info.
 * @dfs_reg_ev_handler:                 Register dfs event handler.
 * @dfs_process_emulate_bang_radar_cmd: Process emulate bang radar test command.
 * @dfs_agile_ch_cfg_cmd:               Send Agile Channel Configuration command
 * @dfs_ocac_abort_cmd:                 Send Off-Channel CAC abort command.
 * @dfs_is_pdev_5ghz:                   Check if the given pdev is 5GHz.
 * @dfs_set_phyerr_filter_offload:      Config phyerr filter offload.
 * @dfs_is_tgt_offload:
 * @dfs_is_tgt_bangradar_320_supp:      To check host DFS 320MHZ support or not
 * @dfs_is_tgt_radar_found_chan_freq_eq_center_freq:
 *                                      Check if chan_freq parameter of the
 *                                      radar found wmi event points to channel
 *                                      center.
 * @dfs_send_offload_enable_cmd:        Send dfs offload enable command to fw.
 * @dfs_host_dfs_check_support:         To check Host DFS confirmation feature
 *                                      support.
 * @dfs_send_avg_radar_params_to_fw:    Send average radar parameters to FW.
 * @dfs_send_usenol_pdev_param:         Send usenol pdev param to FW.
 * @dfs_send_subchan_marking_pdev_param: Send subchan marking pdev param to FW.
 * @dfs_check_mode_switch_state:        Find if HW mode switch is in progress.
 */

struct wlan_lmac_if_dfs_tx_ops {
	QDF_STATUS (*dfs_enable)(struct wlan_objmgr_pdev *pdev,
			int *is_fastclk,
			struct wlan_dfs_phyerr_param *param,
			uint32_t dfsdomain);
	QDF_STATUS (*dfs_get_caps)(struct wlan_objmgr_pdev *pdev,
			struct wlan_dfs_caps *dfs_caps);
	QDF_STATUS (*dfs_disable)(struct wlan_objmgr_pdev *pdev,
			int no_cac);
	QDF_STATUS (*dfs_gettsf64)(struct wlan_objmgr_pdev *pdev,
			uint64_t *tsf64);
	QDF_STATUS (*dfs_set_use_cac_prssi)(struct wlan_objmgr_pdev *pdev);
	QDF_STATUS (*dfs_get_thresholds)(struct wlan_objmgr_pdev *pdev,
			struct wlan_dfs_phyerr_param *param);
	QDF_STATUS (*dfs_get_ext_busy)(struct wlan_objmgr_pdev *pdev,
			int *dfs_ext_chan_busy);
	QDF_STATUS (*dfs_get_target_type)(struct wlan_objmgr_pdev *pdev,
			uint32_t *target_type);
	QDF_STATUS (*dfs_get_ah_devid)(struct wlan_objmgr_pdev *pdev,
			uint16_t *devid);
	QDF_STATUS (*dfs_get_phymode_info)(struct wlan_objmgr_pdev *pdev,
			uint32_t chan_mode,
			uint32_t *mode_info,
			bool is_2gvht_en);
	QDF_STATUS (*dfs_reg_ev_handler)(struct wlan_objmgr_psoc *psoc);
	QDF_STATUS (*dfs_process_emulate_bang_radar_cmd)(
			struct wlan_objmgr_pdev *pdev,
			struct dfs_emulate_bang_radar_test_cmd *dfs_unit_test);
	QDF_STATUS (*dfs_agile_ch_cfg_cmd)(
			struct wlan_objmgr_pdev *pdev,
			struct dfs_agile_cac_params *adfs_params);
	QDF_STATUS (*dfs_ocac_abort_cmd)(struct wlan_objmgr_pdev *pdev);
	QDF_STATUS (*dfs_is_pdev_5ghz)(struct wlan_objmgr_pdev *pdev,
			bool *is_5ghz);
	QDF_STATUS (*dfs_set_phyerr_filter_offload)(
			struct wlan_objmgr_pdev *pdev,
			bool dfs_phyerr_filter_offload);
	bool (*dfs_is_tgt_offload)(struct wlan_objmgr_psoc *psoc);
	bool (*dfs_is_tgt_bangradar_320_supp)(struct wlan_objmgr_psoc *psoc);
	bool (*dfs_is_tgt_radar_found_chan_freq_eq_center_freq)
		 (struct wlan_objmgr_psoc *psoc);
	QDF_STATUS (*dfs_send_offload_enable_cmd)(
			struct wlan_objmgr_pdev *pdev,
			bool enable);
	QDF_STATUS (*dfs_host_dfs_check_support)(struct wlan_objmgr_pdev *pdev,
						 bool *enabled);
	QDF_STATUS (*dfs_send_avg_radar_params_to_fw)(
			struct wlan_objmgr_pdev *pdev,
			struct dfs_radar_found_params *params);
	QDF_STATUS (*dfs_send_usenol_pdev_param)(struct wlan_objmgr_pdev *pdev,
						 bool usenol);
	QDF_STATUS (*dfs_send_subchan_marking_pdev_param)(
			struct wlan_objmgr_pdev *pdev,
			bool subchanmark);
	QDF_STATUS (*dfs_check_mode_switch_state)(
			struct wlan_objmgr_pdev *pdev,
			bool *is_hw_mode_switch_in_progress);
};

/**
 * struct wlan_lmac_if_target_tx_ops - Function pointers to call target
 *                                     functions from other modules.
 * @tgt_is_tgt_type_ar900b:  To check AR900B target type.
 * @tgt_is_tgt_type_qca9984: To check QCA9984 target type.
 * @tgt_is_tgt_type_qca9888: To check QCA9888 target type.
 * @tgt_is_tgt_type_adrastea: To check QCS40X target type.
 * @tgt_is_tgt_type_qcn9000: To check QCN9000 (Pine) target type.
 * @tgt_is_tgt_type_qcn6122: To check QCN6122 (Spruce) target type.
 * @tgt_is_tgt_type_qcn9160: To check QCN9160 target type.
 * @tgt_is_tgt_type_qcn7605: To check QCN7605 target type.
 * @tgt_is_tgt_type_qcn6432: To check QCN6432 (Pebble) target type.
 * @tgt_get_tgt_type:        Get target type
 * @tgt_get_tgt_version:     Get target version
 * @tgt_get_tgt_revision:    Get target revision
 */
struct wlan_lmac_if_target_tx_ops {
	bool (*tgt_is_tgt_type_ar900b)(uint32_t);
	bool (*tgt_is_tgt_type_qca9984)(uint32_t);
	bool (*tgt_is_tgt_type_qca9888)(uint32_t);
	bool (*tgt_is_tgt_type_adrastea)(uint32_t);
	bool (*tgt_is_tgt_type_qcn9000)(uint32_t);
	bool (*tgt_is_tgt_type_qcn6122)(uint32_t);
	bool (*tgt_is_tgt_type_qcn9160)(uint32_t);
	bool (*tgt_is_tgt_type_qcn7605)(uint32_t);
	bool (*tgt_is_tgt_type_qcn6432)(uint32_t);
	uint32_t (*tgt_get_tgt_type)(struct wlan_objmgr_psoc *psoc);
	uint32_t (*tgt_get_tgt_version)(struct wlan_objmgr_psoc *psoc);
	uint32_t (*tgt_get_tgt_revision)(struct wlan_objmgr_psoc *psoc);
};

#ifdef WLAN_OFFCHAN_TXRX_ENABLE
/**
 * struct wlan_lmac_if_offchan_txrx_ops - Function pointers to check target
 *                                     capabilities related to offchan txrx.
 * @offchan_data_tid_support: To check if target supports separate tid for
 *                                     offchan data tx.
 */
struct wlan_lmac_if_offchan_txrx_ops {
	bool (*offchan_data_tid_support)(struct wlan_objmgr_pdev *pdev);
};
#endif

struct wlan_green_ap_egap_params;

#ifdef WLAN_SUPPORT_GAP_LL_PS_MODE
/**
 * struct green_ap_ll_ps_cmd_param - structure of the parameter of
 * green AP low latency power save mode command
 * @state: SAP low power state.(0 = disable, 1 = enable)
 * @bcn_interval: beacon interval (in ms)
 * @cookie: Cookie ID. Unique identifier to track power save command.
 * The value is generated by wlan_green_ap_get_cookie_id().
 */

struct green_ap_ll_ps_cmd_param {
	uint8_t state;
	uint16_t bcn_interval;
	uint32_t cookie;
};

/**
 * struct wlan_green_ap_ll_ps_event_param - Green AP low latency Power Save
 * event parameter structure
 * @bcn_mult: Beacon multiplier
 * @dialog_token: Dialog token
 * @next_tsf: Next TSF
 */
struct wlan_green_ap_ll_ps_event_param {
	uint16_t bcn_mult;
	uint32_t dialog_token;
	uint64_t next_tsf;
};
#endif

/**
 * struct wlan_lmac_if_green_ap_tx_ops - structure of tx function
 *                  pointers for green ap component
 * @enable_egap: function pointer to send enable egap indication to fw
 * @ps_on_off_send:  function pointer to send enable/disable green ap ps to fw
 * @reset_dev: Function pointer to reset device
 * @get_current_channel: function pointer to get current channel
 * @get_current_channel_flags: function pointer to get current channel flags
 * @get_capab: function pointer to get green ap capabilities
 * @ll_ps: function pointer to send low latency power save command
 */
struct wlan_lmac_if_green_ap_tx_ops {
	QDF_STATUS (*enable_egap)(struct wlan_objmgr_pdev *pdev,
				struct wlan_green_ap_egap_params *egap_params);
	QDF_STATUS (*ps_on_off_send)(struct wlan_objmgr_pdev *pdev,
				     bool value, uint8_t pdev_id);
	QDF_STATUS (*reset_dev)(struct wlan_objmgr_pdev *pdev);
	uint16_t (*get_current_channel)(struct wlan_objmgr_pdev *pdev);
	uint64_t (*get_current_channel_flags)(struct wlan_objmgr_pdev *pdev);
	QDF_STATUS (*get_capab)(struct  wlan_objmgr_pdev *pdev);
#ifdef WLAN_SUPPORT_GAP_LL_PS_MODE
	QDF_STATUS (*ll_ps)(struct wlan_objmgr_vdev *vdev,
			    struct green_ap_ll_ps_cmd_param *ll_ps_params);
#endif
};

#ifdef FEATURE_COEX
struct coex_config_params;
struct coex_multi_config;

/**
 * struct wlan_lmac_if_coex_tx_ops - south bound tx function pointers for coex
 * @coex_config_send: function pointer to send coex config to fw
 * @coex_multi_config_send: function pointer to send multiple coex config to fw
 * @coex_get_multi_config_support: function pointer to get multiple coex config
 * support or not
 */
struct wlan_lmac_if_coex_tx_ops {
	QDF_STATUS (*coex_config_send)(struct wlan_objmgr_pdev *pdev,
				       struct coex_config_params *param);
	QDF_STATUS (*coex_multi_config_send)(struct wlan_objmgr_pdev *pdev,
					     struct coex_multi_config *param);
	bool (*coex_get_multi_config_support)(struct wlan_objmgr_psoc *psoc);
};
#endif

#ifdef WLAN_FEATURE_DBAM_CONFIG
/**
 * struct wlan_lmac_if_dbam_tx_ops - south bound tx function pointers for dbam
 * @set_dbam_config: function pointer to send dbam config to fw
 * @dbam_event_attach:
 * @dbam_event_detach:
 */
struct wlan_lmac_if_dbam_tx_ops {
	QDF_STATUS (*set_dbam_config)(struct wlan_objmgr_psoc *psoc,
				      struct coex_dbam_config_params *param);
	QDF_STATUS (*dbam_event_attach)(struct wlan_objmgr_psoc *psoc);
	QDF_STATUS (*dbam_event_detach)(struct wlan_objmgr_psoc *psoc);
};

/**
 * struct wlan_lmac_if_dbam_rx_ops - defines southbound rx callback for dbam
 * @dbam_resp_event: function pointer to rx dbam response event from FW
 */
struct wlan_lmac_if_dbam_rx_ops {
	QDF_STATUS (*dbam_resp_event)(struct wlan_objmgr_psoc *psoc,
				      enum coex_dbam_comp_status dbam_resp);
};
#endif

#ifdef WLAN_FEATURE_GPIO_CFG
struct gpio_config_params;
struct gpio_output_params;

/**
 * struct wlan_lmac_if_gpio_tx_ops - south bound tx function pointers for gpio
 * @set_gpio_config: function pointert to send gpio config to fw
 * @set_gpio_output: function pointert to send gpio output to fw
 */
struct wlan_lmac_if_gpio_tx_ops {
	QDF_STATUS (*set_gpio_config)(struct wlan_objmgr_psoc *psoc,
				      struct gpio_config_params *param);
	QDF_STATUS (*set_gpio_output)(struct wlan_objmgr_psoc *psoc,
				      struct gpio_output_params *param);
};
#endif

#ifdef IPA_OFFLOAD
struct wlan_lmac_if_ipa_tx_ops {
	QDF_STATUS (*ipa_uc_offload_control_req)(
				struct wlan_objmgr_psoc *psoc,
				struct ipa_uc_offload_control_params *req);
	QDF_STATUS (*ipa_intrabss_control_req)(
				struct wlan_objmgr_psoc *psoc,
				struct ipa_intrabss_control_params *req);
};
#endif
/**
 * struct wlan_lmac_if_son_tx_ops: son tx operations
 * @son_send_null: send null packet
 * @get_peer_rate: get peer rate
 * @peer_ext_stats_enable: Enable peer ext stats
 */
struct wlan_lmac_if_son_tx_ops {
	/* Function pointer to enable/disable band steering */
	QDF_STATUS (*son_send_null)(struct wlan_objmgr_pdev *pdev,
				    u_int8_t *macaddr,
				    struct wlan_objmgr_vdev *vdev);

	u_int32_t  (*get_peer_rate)(struct wlan_objmgr_peer *peer,
				    u_int8_t type);

	QDF_STATUS (*peer_ext_stats_enable)(struct wlan_objmgr_pdev *pdev,
					    u_int8_t *peer_addr,
					    struct wlan_objmgr_vdev *vdev,
					    u_int32_t stats_count,
					    u_int32_t enable);
};

/**
 * struct wlan_lmac_if_son_rx_ops - son rx operations
 * @deliver_event: deliver mlme and other mac events
 * @process_mgmt_frame: process mgmt frames
 * @config_set: route son config from cfg80211
 * @config_get: route son config from cfg80211
 * @config_ext_set_get: route extended configs from cfg80211
 */
struct wlan_lmac_if_son_rx_ops {
	int (*deliver_event)(struct wlan_objmgr_vdev *vdev,
			     struct wlan_objmgr_peer *peer,
			     uint32_t event,
			     void *event_data);
	int (*process_mgmt_frame)(struct wlan_objmgr_vdev *vdev,
				  struct wlan_objmgr_peer *peer,
				  int subtype, u_int8_t *frame,
				  u_int16_t frame_len,
				  void *meta_data);
	int (*config_set)(struct wlan_objmgr_vdev *vdev,
			  void *params);
	int (*config_get)(struct wlan_objmgr_vdev *vdev,
			  void *params);
	int (*config_ext_set_get)(struct wlan_objmgr_vdev *vdev,
				  void *params,
				  void *wri);
};

#ifdef WLAN_FEATURE_11BE_MLO
/**
 * struct wlan_lmac_if_mlo_tx_ops - south bound tx function pointers for mlo
 * @register_events: function to register event handlers with FW
 * @unregister_events: function to de-register event handlers with FW
 * @link_set_active: function to send mlo link set active command to FW
 * @request_link_state_info_cmd: function pointer to send link state info
 * @send_link_set_bss_params_cmd: function pointer to send link set bss cmd
 * @shmem_local_ops: operations specific to WLAN_MLO_GLOBAL_SHMEM_SUPPORT
 * @send_tid_to_link_mapping: function to send T2LM command to FW
 * @send_link_removal_cmd: function to send MLO link removal command to FW
 * @send_vdev_pause: function to send MLO vdev pause to FW
 * @peer_ptqm_migrate_send: API to send peer ptqm migration request to FW
 * @send_mlo_link_switch_cnf_cmd: Send link switch status to FW
 * @send_wsi_link_info_cmd: send WSI link stats to FW
 */
struct wlan_lmac_if_mlo_tx_ops {
	QDF_STATUS (*register_events)(struct wlan_objmgr_psoc *psoc);
	QDF_STATUS (*unregister_events)(struct wlan_objmgr_psoc *psoc);
	QDF_STATUS (*link_set_active)(struct wlan_objmgr_psoc *psoc,
		struct mlo_link_set_active_param *param);
	QDF_STATUS (*request_link_state_info_cmd)(
		struct wlan_objmgr_psoc *psoc,
		struct mlo_link_state_cmd_params *param);
	QDF_STATUS (*send_link_set_bss_params_cmd)(
			struct wlan_objmgr_psoc *psoc,
			struct mlo_link_bss_params *param);

#ifdef WLAN_MLO_GLOBAL_SHMEM_SUPPORT
	struct wlan_lmac_if_global_shmem_local_ops shmem_local_ops;
#endif
	QDF_STATUS (*send_tid_to_link_mapping)(struct wlan_objmgr_vdev *vdev,
					       struct wlan_t2lm_info *t2lm);
	QDF_STATUS (*send_link_removal_cmd)(
		struct wlan_objmgr_psoc *psoc,
		const struct mlo_link_removal_cmd_params *param);
	QDF_STATUS (*send_vdev_pause)(struct wlan_objmgr_psoc *psoc,
				      struct mlo_vdev_pause *info);
#ifdef QCA_SUPPORT_PRIMARY_LINK_MIGRATE
	QDF_STATUS (*peer_ptqm_migrate_send)(
					struct wlan_objmgr_vdev *vdev,
					struct peer_ptqm_migrate_params *param);
#endif /* QCA_SUPPORT_PRIMARY_LINK_MIGRATE */
#ifdef WLAN_FEATURE_11BE_MLO_ADV_FEATURE
	QDF_STATUS
	(*send_mlo_link_switch_cnf_cmd)(struct wlan_objmgr_psoc *psoc,
					struct wlan_mlo_link_switch_cnf *params);
#endif /* WLAN_FEATURE_11BE_MLO_ADV_FEATURE */
	QDF_STATUS (*send_wsi_link_info_cmd)(
				struct wlan_objmgr_pdev *pdev,
				struct mlo_wsi_link_stats *param);
};

/**
 * struct wlan_lmac_if_mlo_rx_ops - defines southbound rx callbacks for mlo
 * @process_link_set_active_resp: function pointer to rx FW events
 * @process_mlo_vdev_tid_to_link_map_event:  function pointer to rx T2LM event
 * @mlo_link_removal_handler: function pointer for MLO link removal handler
 * @process_mlo_link_state_info_event: function pointer for mlo link state
 * @mlo_link_disable_request_handler: function ptr for mlo link disable request
 * @mlo_link_switch_request_handler: Handler function pointer to deliver link
 * switch request params from FW to host.
 * @mlo_link_state_switch_event_handler: Function pointer to handle link state
 * switch event
 */
struct wlan_lmac_if_mlo_rx_ops {
	QDF_STATUS
	(*process_link_set_active_resp)(struct wlan_objmgr_psoc *psoc,
		struct mlo_link_set_active_resp *event);
	QDF_STATUS (*process_mlo_vdev_tid_to_link_map_event)(
			struct wlan_objmgr_psoc *psoc,
			struct mlo_vdev_host_tid_to_link_map_resp *event);
	QDF_STATUS (*mlo_link_removal_handler)(
			struct wlan_objmgr_psoc *psoc,
			struct mlo_link_removal_evt_params *evt_params);
	QDF_STATUS (*process_mlo_link_state_info_event)(
			struct wlan_objmgr_psoc *psoc,
			struct ml_link_state_info_event *event);
	QDF_STATUS (*mlo_link_disable_request_handler)(
			struct wlan_objmgr_psoc *psoc,
			void *evt_params);
#ifdef WLAN_FEATURE_11BE_MLO_ADV_FEATURE
	QDF_STATUS
	(*mlo_link_switch_request_handler)(struct wlan_objmgr_psoc *psoc,
					   void *evt_params);
	QDF_STATUS
	(*mlo_link_state_switch_event_handler)(struct wlan_objmgr_psoc *psoc,
					       struct mlo_link_switch_state_info *info);
#endif /* WLAN_FEATURE_11BE_MLO_ADV_FEATURE */
};
#endif

#if defined(WLAN_SUPPORT_TWT) && defined(WLAN_TWT_CONV_SUPPORTED)
/**
 * struct wlan_lmac_if_twt_tx_ops - defines southbound tx callbacks for
 * TWT (Target Wake Time) component
 * @enable_req: function pointer to send TWT enable command to FW
 * @disable_req: function pointer to send TWT disable command to FW
 * @setup_req: function pointer to send TWT add dialog command to FW
 * @teardown_req: function pointer to send TWT delete dialog command to FW
 * @pause_req: function pointer to send TWT pause dialog command to FW
 * @resume_req: function pointer to send TWT resume dialog command to FW
 * @nudge_req: function pointer to send TWT nudge dialog command to FW
 * @set_ac_param: function pointer to send TWT access category param to FW
 * @register_events: function pointer to register events from FW
 * @deregister_events: function pointer to deregister events from FW
 */
struct wlan_lmac_if_twt_tx_ops {
	QDF_STATUS (*enable_req)(struct wlan_objmgr_psoc *psoc,
				 struct twt_enable_param *params);
	QDF_STATUS (*disable_req)(struct wlan_objmgr_psoc *psoc,
				 struct twt_disable_param *params);
	QDF_STATUS (*setup_req)(struct wlan_objmgr_psoc *psoc,
				 struct twt_add_dialog_param *params);
	QDF_STATUS (*teardown_req)(struct wlan_objmgr_psoc *psoc,
				 struct twt_del_dialog_param *params);
	QDF_STATUS (*pause_req)(struct wlan_objmgr_psoc *psoc,
				 struct twt_pause_dialog_cmd_param *params);
	QDF_STATUS (*resume_req)(struct wlan_objmgr_psoc *psoc,
				 struct twt_resume_dialog_cmd_param *params);
	QDF_STATUS (*nudge_req)(struct wlan_objmgr_psoc *psoc,
				 struct twt_nudge_dialog_cmd_param *params);
	QDF_STATUS (*set_ac_param)(struct wlan_objmgr_psoc *psoc,
				   enum twt_traffic_ac twt_ac, uint8_t mac_id);
	QDF_STATUS (*register_events)(struct wlan_objmgr_psoc *psoc);
	QDF_STATUS (*deregister_events)(struct wlan_objmgr_psoc *psoc);
};

/**
 * struct wlan_lmac_if_twt_rx_ops - defines southbound xx callbacks for
 * TWT (Target Wake Time) component
 * @twt_enable_comp_cb: function pointer to process TWT enable event
 * @twt_disable_comp_cb: function pointer to process TWT disable event
 * @twt_setup_comp_cb: function pointer to process TWT add dialog event
 * @twt_teardown_comp_cb: function pointer to process TWT del dialog event
 * @twt_pause_comp_cb: function pointer to process TWT pause dialog event
 * @twt_resume_comp_cb: function pointer to process TWT resume dialog
 * event
 * @twt_nudge_comp_cb: function pointer to process TWT nudge dialog event
 * @twt_notify_comp_cb: function pointer to process TWT notify event
 * @twt_ack_comp_cb: function pointer to process TWT ack event
 */
struct wlan_lmac_if_twt_rx_ops {
	QDF_STATUS (*twt_enable_comp_cb)(struct wlan_objmgr_psoc *psoc,
			struct twt_enable_complete_event_param *event);
	QDF_STATUS (*twt_disable_comp_cb)(struct wlan_objmgr_psoc *psoc,
			struct twt_disable_complete_event_param *event);
	QDF_STATUS (*twt_setup_comp_cb)(struct wlan_objmgr_psoc *psoc,
			struct twt_add_dialog_complete_event *event);
	QDF_STATUS (*twt_teardown_comp_cb)(struct wlan_objmgr_psoc *psoc,
			struct twt_del_dialog_complete_event_param *event);
	QDF_STATUS (*twt_pause_comp_cb)(struct wlan_objmgr_psoc *psoc,
			struct twt_pause_dialog_complete_event_param *event);
	QDF_STATUS (*twt_resume_comp_cb)(struct wlan_objmgr_psoc *psoc,
			struct twt_resume_dialog_complete_event_param *event);
	QDF_STATUS (*twt_nudge_comp_cb)(struct wlan_objmgr_psoc *psoc,
			struct twt_nudge_dialog_complete_event_param *event);
	QDF_STATUS (*twt_notify_comp_cb)(struct wlan_objmgr_psoc *psoc,
			struct twt_notify_event_param *event);
	QDF_STATUS (*twt_ack_comp_cb)(struct wlan_objmgr_psoc *psoc,
			struct twt_ack_complete_event_param *params);
};
#endif

#ifdef WLAN_FEATURE_SR
struct wlan_lmac_if_spatial_reuse_tx_ops {
	QDF_STATUS (*send_cfg)(struct wlan_objmgr_vdev *vdev, uint8_t sr_ctrl,
			       uint8_t non_srg_max_pd_offset);
	QDF_STATUS (*send_sr_prohibit_cfg)(struct wlan_objmgr_vdev *vdev,
					   bool he_siga_val15_allowed);
	QDF_STATUS (*target_if_set_sr_enable_disable)(
				struct wlan_objmgr_vdev *vdev,
				struct wlan_objmgr_pdev *pdev,
				bool is_sr_enable, int32_t srg_pd_threshold,
				int32_t non_srg_pd_threshold);
	QDF_STATUS (*target_if_sr_update)(struct wlan_objmgr_pdev *pdev,
					  uint8_t vdev_id, uint32_t val);
};
#endif

#ifdef WLAN_FEATURE_COAP
/**
 * struct wlan_lmac_if_coap_tx_ops - south bound tx function pointers for CoAP
 * @attach: function pointer to attach CoAP component
 * @detach: function pointer to detach CoAP component
 * @offload_reply_enable: function pointer to enable CoAP offload reply
 * @offload_reply_disable: function pointer to disable CoAP offload reply
 * @offload_periodic_tx_enable: function pointer to enable CoAP offload
 * periodic transmitting
 * @offload_periodic_tx_disable: function pointer to disable CoAP offload
 * periodic transmitting
 * @offload_cache_get: function pointer to get cached CoAP messages
 */
struct wlan_lmac_if_coap_tx_ops {
	QDF_STATUS (*attach)(struct wlan_objmgr_psoc *psoc);
	QDF_STATUS (*detach)(struct wlan_objmgr_psoc *psoc);
	QDF_STATUS (*offload_reply_enable)(struct wlan_objmgr_vdev *vdev,
			struct coap_offload_reply_param *param);
	QDF_STATUS (*offload_reply_disable)(struct wlan_objmgr_vdev *vdev,
					    uint32_t req_id);
	QDF_STATUS (*offload_periodic_tx_enable)(struct wlan_objmgr_vdev *vdev,
			struct coap_offload_periodic_tx_param *param);
	QDF_STATUS (*offload_periodic_tx_disable)(struct wlan_objmgr_vdev *vdev,
						  uint32_t req_id);
	QDF_STATUS (*offload_cache_get)(struct wlan_objmgr_vdev *vdev,
					uint32_t req_id);
};
#endif

#ifdef CONFIG_SAWF
/**
 * struct wlan_lmac_if_sawf_tx_ops - Target function pointers for SAWF
 *
 * @sawf_svc_create_send: function pointer to send SAWF SVC create
 * @sawf_svc_disable_send: function pointer to send SAWF SVC disable
 * @sawf_ul_svc_update_send: function pointer to update
 *                           peer uplink QoS parameters
 * @sawf_update_ul_params: function pointer to update flow uplink QoS parameters
 */
struct wlan_lmac_if_sawf_tx_ops {
	QDF_STATUS
	(*sawf_svc_create_send)(struct wlan_objmgr_pdev *pdev, void *params);
	QDF_STATUS
	(*sawf_svc_disable_send)(struct wlan_objmgr_pdev *pdev, void *params);
	QDF_STATUS
	(*sawf_ul_svc_update_send)(struct wlan_objmgr_pdev *pdev,
				   uint8_t vdev_id, uint8_t *peer_mac,
				   uint8_t ac, uint8_t add_or_sub,
				   void *svc_params);
	QDF_STATUS
	(*sawf_update_ul_params)(struct wlan_objmgr_pdev *pdev, uint8_t vdev_id,
				 uint8_t *peer_mac, uint8_t tid, uint8_t ac,
				 uint32_t service_interval, uint32_t burst_size,
				 uint32_t min_tput, uint32_t max_latency,
				 uint8_t add_or_sub);
};
#endif

/**
 * struct wlan_lmac_if_tx_ops - south bound tx function pointers
 * @mgmt_txrx_tx_ops: mgmt txrx tx ops
 * @scan: scan tx ops
 * @p2p: P2P tx ops
 * @iot_sim_tx_ops: IoT sim tx ops
 * @son_tx_ops: SON tx ops
 * @atf_tx_ops: ATF tx ops
 * @cp_stats_tx_ops: cp stats tx_ops
 * @dcs_tx_ops: DCS tx ops
 * @sa_api_tx_ops: SA API tx ops
 * @cfr_tx_ops: CFR tx ops
 * @sptrl_tx_ops: Spectral  tx ops
 * @crypto_tx_ops: Crypto tx ops
 * @wifi_pos_tx_ops: WiFi Positioning tx ops
 * @reg_ops: Regulatory tx ops
 * @dfs_tx_ops: dfs tx ops.
 * @tdls_tx_ops: TDLS tx ops
 * @fd_tx_ops: FILS tx ops
 * @mops: MLME tx ops
 * @target_tx_ops: Target tx ops
 * @offchan_txrx_ops: Off-channel tx ops
 * @dbr_tx_ops: Direct Buffer Rx tx ops
 * @green_ap_tx_ops: green_ap tx_ops
 * @ftm_tx_ops: FTM tx ops
 * @coex_ops: coex tx_ops
 * @dbam_tx_ops: coex dbam tx_ops
 * @gpio_ops: gpio tx_ops
 * @mlo_ops: MLO tx ops
 * @ipa_ops: IPA tx ops
 * @twt_tx_ops: TWT tx ops
 * @spatial_reuse_tx_ops: Spatial Reuse tx ops
 * @coap_ops: COAP tx ops
 * @sawf_tx_ops: SAWF tx ops
 *
 * Callback function tabled to be registered with umac.
 * umac will use the functional table to send events/frames to wmi
 */

struct wlan_lmac_if_tx_ops {
	/* Components to declare function pointers required by the module
	 * in component specific structure.
	 * The component specific ops structure can be declared in this file
	 * only
	 */
	struct wlan_lmac_if_mgmt_txrx_tx_ops mgmt_txrx_tx_ops;
	struct wlan_lmac_if_scan_tx_ops scan;
#ifdef CONVERGED_P2P_ENABLE
	struct wlan_lmac_if_p2p_tx_ops p2p;
#endif
#ifdef WLAN_IOT_SIM_SUPPORT
	struct wlan_lmac_if_iot_sim_tx_ops iot_sim_tx_ops;
#endif
#if defined(QCA_SUPPORT_SON) || defined(WLAN_FEATURE_SON)
	struct wlan_lmac_if_son_tx_ops son_tx_ops;
#endif
#ifdef WLAN_ATF_ENABLE
	struct wlan_lmac_if_atf_tx_ops atf_tx_ops;
#endif
#ifdef QCA_SUPPORT_CP_STATS
	struct wlan_lmac_if_cp_stats_tx_ops cp_stats_tx_ops;
#endif
#ifdef DCS_INTERFERENCE_DETECTION
	struct wlan_target_if_dcs_tx_ops dcs_tx_ops;
#endif
#ifdef WLAN_SA_API_ENABLE
	struct wlan_lmac_if_sa_api_tx_ops sa_api_tx_ops;
#endif

#ifdef WLAN_CFR_ENABLE
	struct wlan_lmac_if_cfr_tx_ops cfr_tx_ops;
#endif

#ifdef WLAN_CONV_SPECTRAL_ENABLE
	struct wlan_lmac_if_sptrl_tx_ops sptrl_tx_ops;
#endif

	struct wlan_lmac_if_crypto_tx_ops crypto_tx_ops;

#ifdef WIFI_POS_CONVERGED
	struct wlan_lmac_if_wifi_pos_tx_ops wifi_pos_tx_ops;
#endif
	struct wlan_lmac_if_reg_tx_ops reg_ops;
	struct wlan_lmac_if_dfs_tx_ops dfs_tx_ops;

#ifdef FEATURE_WLAN_TDLS
	struct wlan_lmac_if_tdls_tx_ops tdls_tx_ops;
#endif

#ifdef WLAN_SUPPORT_FILS
	struct wlan_lmac_if_fd_tx_ops fd_tx_ops;
#endif
	struct wlan_lmac_if_mlme_tx_ops mops;
	struct wlan_lmac_if_target_tx_ops target_tx_ops;

#ifdef WLAN_OFFCHAN_TXRX_ENABLE
	struct wlan_lmac_if_offchan_txrx_ops offchan_txrx_ops;
#endif

#ifdef DIRECT_BUF_RX_ENABLE
	struct wlan_lmac_if_direct_buf_rx_tx_ops dbr_tx_ops;
#endif

#ifdef WLAN_SUPPORT_GREEN_AP
	struct wlan_lmac_if_green_ap_tx_ops green_ap_tx_ops;
#endif

	struct wlan_lmac_if_ftm_tx_ops ftm_tx_ops;

#ifdef FEATURE_COEX
	struct wlan_lmac_if_coex_tx_ops coex_ops;
#endif

#ifdef WLAN_FEATURE_DBAM_CONFIG
	struct wlan_lmac_if_dbam_tx_ops dbam_tx_ops;
#endif

#ifdef WLAN_FEATURE_GPIO_CFG
	struct wlan_lmac_if_gpio_tx_ops gpio_ops;
#endif

#ifdef WLAN_FEATURE_11BE_MLO
	struct wlan_lmac_if_mlo_tx_ops mlo_ops;
#endif

#ifdef IPA_OFFLOAD
	struct wlan_lmac_if_ipa_tx_ops ipa_ops;
#endif

#if defined(WLAN_SUPPORT_TWT) && defined(WLAN_TWT_CONV_SUPPORTED)
	struct wlan_lmac_if_twt_tx_ops twt_tx_ops;
#endif

#if defined WLAN_FEATURE_SR
	struct wlan_lmac_if_spatial_reuse_tx_ops spatial_reuse_tx_ops;
#endif

#ifdef WLAN_FEATURE_COAP
	struct wlan_lmac_if_coap_tx_ops coap_ops;
#endif
#ifdef CONFIG_SAWF
	struct wlan_lmac_if_sawf_tx_ops sawf_tx_ops;
#endif
};

/**
 * struct wlan_lmac_if_mgmt_txrx_rx_ops - structure of rx function
 *                  pointers for mgmt txrx component
 * @mgmt_tx_completion_handler: function pointer to give tx completions
 *                              to mgmt txrx comp.
 * @mgmt_rx_frame_handler: function pointer to give rx frame to mgmt txrx comp.
 * @mgmt_txrx_get_nbuf_from_desc_id: function pointer to get nbuf from desc id
 * @mgmt_txrx_get_peer_from_desc_id: function pointer to get peer from desc id
 * @mgmt_txrx_get_vdev_id_from_desc_id: function pointer to get vdev id from
 *                                      desc id
 * @mgmt_txrx_get_free_desc_pool_count:
 * @mgmt_rx_frame_entry: Entry point for Rx frames into MGMT TxRx component
 * @mgmt_rx_reo_rx_ops: rxops of MGMT Rx REO module
 */
struct wlan_lmac_if_mgmt_txrx_rx_ops {
	QDF_STATUS (*mgmt_tx_completion_handler)(
			struct wlan_objmgr_pdev *pdev,
			uint32_t desc_id, uint32_t status,
			void *tx_compl_params);
	QDF_STATUS (*mgmt_rx_frame_handler)(
			struct wlan_objmgr_psoc *psoc,
			qdf_nbuf_t buf,
			struct mgmt_rx_event_params *mgmt_rx_params);
	qdf_nbuf_t (*mgmt_txrx_get_nbuf_from_desc_id)(
			struct wlan_objmgr_pdev *pdev,
			uint32_t desc_id);
	struct wlan_objmgr_peer * (*mgmt_txrx_get_peer_from_desc_id)(
			struct wlan_objmgr_pdev *pdev, uint32_t desc_id);
	uint8_t (*mgmt_txrx_get_vdev_id_from_desc_id)(
			struct wlan_objmgr_pdev *pdev,
			uint32_t desc_id);
	uint32_t (*mgmt_txrx_get_free_desc_pool_count)(
			struct wlan_objmgr_pdev *pdev);
	QDF_STATUS (*mgmt_rx_frame_entry)(
			struct wlan_objmgr_pdev *pdev,
			qdf_nbuf_t buf,
			struct mgmt_rx_event_params *mgmt_rx_params);
#ifdef WLAN_MGMT_RX_REO_SUPPORT
	struct wlan_lmac_if_mgmt_rx_reo_rx_ops mgmt_rx_reo_rx_ops;
#endif
};

/**
 * struct wlan_lmac_if_reg_rx_ops - structure of rx function pointers
 * @master_list_handler:
 * @master_list_ext_handler:
 * @afc_event_handler:
 * @reg_11d_new_cc_handler:
 * @reg_set_regdb_offloaded:
 * @reg_set_11d_offloaded:
 * @reg_set_6ghz_supported:
 * @reg_set_5dot9_ghz_supported:
 * @get_dfs_region:
 * @reg_ch_avoid_event_handler:
 * @reg_freq_to_chan:
 * @reg_set_chan_144:
 * @reg_get_chan_144:
 * @reg_program_default_cc:
 * @reg_get_current_regdomain:
 * @reg_enable_dfs_channels:
 * @reg_modify_pdev_chan_range:
 * @reg_update_pdev_wireless_modes:
 * @reg_is_range_only6g:
 * @reg_is_range_overlap_6g:
 * @reg_disable_chan_coex:
 * @reg_ignore_fw_reg_offload_ind:
 * @reg_get_unii_5g_bitmap:
 * @reg_set_ext_tpc_supported:
 * @reg_set_lower_6g_edge_ch_supp:
 * @reg_set_disable_upper_6g_edge_ch_supp:
 * @reg_display_super_chan_list:
 * @reg_display_super_chan_list: function pointer to print super channel list
 * @reg_set_afc_dev_type:
 * @reg_get_afc_dev_type:
 * @reg_set_eirp_preferred_support:
 * @reg_get_eirp_preferred_support:
 * @reg_r2p_table_update_response_handler: function pointer to handle
 *		rate2power update response from fw.
 */
struct wlan_lmac_if_reg_rx_ops {
	QDF_STATUS (*master_list_handler)(struct cur_regulatory_info
					  *reg_info);
#ifdef CONFIG_BAND_6GHZ
	QDF_STATUS (*master_list_ext_handler)(struct cur_regulatory_info
					      *reg_info);
#ifdef CONFIG_AFC_SUPPORT
	QDF_STATUS (*afc_event_handler)(struct afc_regulatory_info *afc_info);
#endif
#endif
	QDF_STATUS (*reg_11d_new_cc_handler)(struct wlan_objmgr_psoc *psoc,
			struct reg_11d_new_country *reg_11d_new_cc);
	QDF_STATUS (*reg_set_regdb_offloaded)(struct wlan_objmgr_psoc *psoc,
			bool val);
	QDF_STATUS (*reg_set_11d_offloaded)(struct wlan_objmgr_psoc *psoc,
			bool val);
	QDF_STATUS (*reg_set_6ghz_supported)(struct wlan_objmgr_psoc *psoc,
					     bool val);
	QDF_STATUS (*reg_set_5dot9_ghz_supported)(struct wlan_objmgr_psoc
						  *psoc, bool val);
	QDF_STATUS (*get_dfs_region)(struct wlan_objmgr_pdev *pdev,
			enum dfs_reg *dfs_reg);
	QDF_STATUS (*reg_ch_avoid_event_handler)(struct wlan_objmgr_psoc *psoc,
			struct ch_avoid_ind_type *ch_avoid_ind);
	uint8_t (*reg_freq_to_chan)(struct wlan_objmgr_pdev *pdev,
				    qdf_freq_t freq);
	QDF_STATUS (*reg_set_chan_144)(struct wlan_objmgr_pdev *pdev,
			bool enable_ch_144);
	bool (*reg_get_chan_144)(struct wlan_objmgr_pdev *pdev);
#ifdef WLAN_REG_PARTIAL_OFFLOAD
	QDF_STATUS (*reg_program_default_cc)(struct wlan_objmgr_pdev *pdev,
			uint16_t regdmn);
	QDF_STATUS (*reg_get_current_regdomain)(struct wlan_objmgr_pdev *pdev,
			struct cur_regdmn_info *cur_regdmn);
#endif
	QDF_STATUS (*reg_enable_dfs_channels)(struct wlan_objmgr_pdev *pdev,
					      bool dfs_enable);
	QDF_STATUS (*reg_modify_pdev_chan_range)(struct
						 wlan_objmgr_pdev *pdev);
	QDF_STATUS
	(*reg_update_pdev_wireless_modes)(struct wlan_objmgr_pdev *pdev,
					  uint64_t wireless_modes);
	bool
	(*reg_is_range_only6g)(qdf_freq_t low_freq, qdf_freq_t high_freq);
	bool
	(*reg_is_range_overlap_6g)(qdf_freq_t low_freq, qdf_freq_t high_freq);
	QDF_STATUS (*reg_disable_chan_coex)(struct wlan_objmgr_pdev *pdev,
					    uint8_t unii_5g_bitmap);
	bool (*reg_ignore_fw_reg_offload_ind)(struct wlan_objmgr_psoc *psoc);
	QDF_STATUS (*reg_get_unii_5g_bitmap)(struct wlan_objmgr_pdev *pdev,
					     uint8_t *bitmap);
	QDF_STATUS (*reg_set_ext_tpc_supported)(struct wlan_objmgr_psoc *psoc,
						bool val);
#if defined(CONFIG_BAND_6GHZ)
	QDF_STATUS
	(*reg_set_lower_6g_edge_ch_supp)(struct wlan_objmgr_psoc *psoc,
					 bool val);
	QDF_STATUS
	(*reg_set_disable_upper_6g_edge_ch_supp)(struct wlan_objmgr_psoc *psoc,
						 bool val);
	QDF_STATUS
	(*reg_display_super_chan_list)(struct wlan_objmgr_pdev *pdev);
#endif

#ifdef CONFIG_AFC_SUPPORT
	QDF_STATUS
	(*reg_set_afc_dev_type)(struct wlan_objmgr_psoc *psoc,
				enum reg_afc_dev_deploy_type
				reg_afc_dev_type);
	QDF_STATUS
	(*reg_get_afc_dev_type)(struct wlan_objmgr_psoc *psoc,
				enum reg_afc_dev_deploy_type
				*reg_afc_dev_type);
	QDF_STATUS
	(*reg_set_eirp_preferred_support)(
				struct wlan_objmgr_psoc *psoc,
				bool reg_is_eirp_support_preferred);
	QDF_STATUS
	(*reg_get_eirp_preferred_support)(
				struct wlan_objmgr_psoc *psoc,
				bool *reg_is_eirp_support_preferred);
#endif
	QDF_STATUS (*reg_r2p_table_update_response_handler)(
			struct wlan_objmgr_psoc *psoc,
			uint32_t pdev_id);
};

#ifdef CONVERGED_P2P_ENABLE

/* forward declarations for p2p rx ops */
struct p2p_noa_info;
struct p2p_lo_event;
struct p2p_set_mac_filter_evt;
struct mcc_quota_info;

/**
 * struct wlan_lmac_if_p2p_rx_ops - structure of rx function pointers
 * for P2P component
 * @lo_ev_handler:    function pointer to give listen offload event
 * @noa_ev_handler:   function pointer to give noa event
 * @add_mac_addr_filter_evt_handler: function pointer to process add mac addr
 *    rx filter event
 * @mcc_quota_ev_handler:   function pointer to receive mcc quota event
 */
struct wlan_lmac_if_p2p_rx_ops {
#ifdef FEATURE_P2P_LISTEN_OFFLOAD
	QDF_STATUS (*lo_ev_handler)(struct wlan_objmgr_psoc *psoc,
		struct p2p_lo_event *event_info);
#endif
	QDF_STATUS (*noa_ev_handler)(struct wlan_objmgr_psoc *psoc,
		struct p2p_noa_info *event_info);
	QDF_STATUS (*add_mac_addr_filter_evt_handler)(
		struct wlan_objmgr_psoc *psoc,
		struct p2p_set_mac_filter_evt *event_info);
#ifdef WLAN_FEATURE_MCC_QUOTA
	QDF_STATUS (*mcc_quota_ev_handler)(struct wlan_objmgr_psoc *psoc,
					   struct mcc_quota_info *event_info);
#endif
};
#endif

#ifdef WLAN_ATF_ENABLE

/**
 * struct wlan_lmac_if_atf_rx_ops - ATF south bound rx function pointers
 * @atf_get_fmcap:                     Get firmware capability for ATF
 * @atf_get_mode:                      Get mode of ATF
 * @atf_is_enabled:                    Check atf_mode, fwcap & atf_commit flags
 * @atf_get_msdu_desc:                 Get msdu desc for ATF
 * @atf_get_max_vdevs:                 Get maximum vdevs for a Radio
 * @atf_get_peers:                     Get number of peers for a radio
 * @atf_get_tput_based:                Get throughput based enabled/disabled
 * @atf_get_logging:                   Get logging enabled/disabled
 * @atf_get_ssidgroup:                 Get ssid group state
 * @atf_get_vdev_ac_blk_cnt:           Get AC block count for vdev
 * @atf_get_peer_blk_txbitmap:         Get peer tx traffic AC bitmap
 * @atf_get_vdev_blk_txtraffic:        Get vdev tx traffic block state
 * @atf_get_sched:                     Get ATF scheduled policy
 * @atf_get_peer_stats:                Get atf peer stats
 * @atf_get_token_allocated:           Get atf token allocated
 * @atf_get_token_utilized:            Get atf token utilized
 * @atf_set_sched:                     Set ATF schedule policy
 * @atf_set_fmcap:                     Set firmware capability for ATF
 * @atf_set_msdu_desc:                 Set msdu desc
 * @atf_set_max_vdevs:                 Set maximum vdevs number
 * @atf_set_peers:                     Set peers number
 * @atf_set_peer_stats:                Set peer stats
 * @atf_set_vdev_blk_txtraffic:        Set Block/unblock vdev tx traffic
 * @atf_peer_blk_txtraffic:            Block peer tx traffic
 * @atf_peer_unblk_txtraffic:          Unblock peer tx traffic
 * @atf_set_token_allocated:           Set atf token allocated
 * @atf_set_token_utilized:            Set atf token utilized
 * @atf_process_tx_ppdu_stats:         Process Tx PPDU stats to get ATF stats
 * @atf_process_rx_ppdu_stats:         Process Rx PPDU stats to get ATF stats
 * @atf_is_stats_enabled:              Check ATF stats enabled or not
 * @atf_set_fw_max_client_512_support: Set Max Client Support based on WMI
 *    service bit announcement from FW
 */
struct wlan_lmac_if_atf_rx_ops {
	uint32_t (*atf_get_fmcap)(struct wlan_objmgr_psoc *psoc);
	uint32_t (*atf_get_mode)(struct wlan_objmgr_psoc *psoc);
	bool (*atf_is_enabled)(struct wlan_objmgr_psoc *psoc,
			       struct wlan_objmgr_pdev *pdev);
	uint32_t (*atf_get_msdu_desc)(struct wlan_objmgr_psoc *psoc);
	uint32_t (*atf_get_max_vdevs)(struct wlan_objmgr_psoc *psoc);
	uint32_t (*atf_get_peers)(struct wlan_objmgr_psoc *psoc);
	uint32_t (*atf_get_tput_based)(struct wlan_objmgr_pdev *pdev);
	uint32_t (*atf_get_logging)(struct wlan_objmgr_pdev *pdev);
	uint32_t (*atf_get_ssidgroup)(struct wlan_objmgr_pdev *pdev);
	uint32_t (*atf_get_vdev_ac_blk_cnt)(struct wlan_objmgr_vdev *vdev);
	uint8_t (*atf_get_peer_blk_txbitmap)(struct wlan_objmgr_peer *peer);
	uint8_t (*atf_get_vdev_blk_txtraffic)(struct wlan_objmgr_vdev *vdev);
	uint32_t (*atf_get_sched)(struct wlan_objmgr_pdev *pdev);
	void (*atf_get_peer_stats)(struct wlan_objmgr_peer *peer,
				   struct atf_stats *stats);
	uint16_t (*atf_get_token_allocated)(struct wlan_objmgr_peer *peer);
	uint16_t (*atf_get_token_utilized)(struct wlan_objmgr_peer *peer);
	void (*atf_set_sched)(struct wlan_objmgr_pdev *pdev, uint32_t value);
	void (*atf_set_fmcap)(struct wlan_objmgr_psoc *psoc, uint32_t value);
	void (*atf_set_msdu_desc)(struct wlan_objmgr_psoc *psoc,
				  uint32_t value);
	void (*atf_set_max_vdevs)(struct wlan_objmgr_psoc *psoc,
				  uint32_t value);
	void (*atf_set_peers)(struct wlan_objmgr_psoc *psoc, uint32_t value);
	void (*atf_set_peer_stats)(struct wlan_objmgr_peer *peer,
				   struct atf_stats *stats);
	void (*atf_set_vdev_blk_txtraffic)(struct wlan_objmgr_vdev *vdev,
					   uint8_t value);
	void (*atf_peer_blk_txtraffic)(struct wlan_objmgr_peer *peer,
				       int8_t ac_id);
	void (*atf_peer_unblk_txtraffic)(struct wlan_objmgr_peer *peer,
					 int8_t ac_id);
	void (*atf_set_token_allocated)(struct wlan_objmgr_peer *peer,
					uint16_t value);
	void (*atf_set_token_utilized)(struct wlan_objmgr_peer *peer,
				       uint16_t value);
	void (*atf_process_tx_ppdu_stats)(struct wlan_objmgr_pdev *pdev,
					  qdf_nbuf_t msg);
	void (*atf_process_rx_ppdu_stats)(struct wlan_objmgr_pdev *pdev,
					  qdf_nbuf_t msg);
	uint8_t (*atf_is_stats_enabled)(struct wlan_objmgr_pdev *pdev);
#ifdef WLAN_ATF_INCREASED_STA
	void (*atf_set_fw_max_client_512_support)(struct wlan_objmgr_psoc *psoc,
						  uint8_t val);
#endif
};
#endif

#ifdef WLAN_SUPPORT_FILS
/**
 * struct wlan_lmac_if_fd_rx_ops - FILS Discovery specific Rx function pointers
 * @fd_is_fils_enable:      FILS enabled or not
 * @fd_alloc:               Allocate FD buffer
 * @fd_stop:                Stop and free deferred FD buffer
 * @fd_free:                Free FD frame buffer
 * @fd_get_valid_fd_period: Get valid FD period
 * @fd_swfda_handler:       SWFDA event handler
 * @fd_offload:             Offload FD frame
 * @fd_tmpl_update:         Update the FD frame template
 */
struct wlan_lmac_if_fd_rx_ops {
	uint8_t (*fd_is_fils_enable)(struct wlan_objmgr_vdev *vdev);
	void (*fd_alloc)(struct wlan_objmgr_vdev *vdev);
	void (*fd_stop)(struct wlan_objmgr_vdev *vdev);
	void (*fd_free)(struct wlan_objmgr_vdev *vdev);
	uint32_t (*fd_get_valid_fd_period)(struct wlan_objmgr_vdev *vdev,
					   uint8_t *is_modified);
	QDF_STATUS (*fd_swfda_handler)(struct wlan_objmgr_vdev *vdev);
	QDF_STATUS (*fd_offload)(struct wlan_objmgr_vdev *vdev,
				 uint32_t vdev_id);
	QDF_STATUS (*fd_tmpl_update)(struct wlan_objmgr_vdev *vdev);
};
#endif

#ifdef WLAN_SA_API_ENABLE

/**
 * struct wlan_lmac_if_sa_api_rx_ops - SA API south bound rx function pointers
 * @sa_api_get_sa_supported:
 * @sa_api_get_validate_sw:
 * @sa_api_enable_sa:
 * @sa_api_get_sa_enable:
 * @sa_api_peer_assoc_hanldler:
 * @sa_api_update_tx_feedback:
 * @sa_api_update_rx_feedback:
 * @sa_api_ucfg_set_param:
 * @sa_api_ucfg_get_param:
 * @sa_api_is_tx_feedback_enabled:
 * @sa_api_is_rx_feedback_enabled:
 * @sa_api_convert_rate_2g:
 * @sa_api_convert_rate_5g:
 * @sa_api_get_sa_mode:
 * @sa_api_get_beacon_txantenna:
 * @sa_api_cwm_action:
 */
struct wlan_lmac_if_sa_api_rx_ops {
	uint32_t (*sa_api_get_sa_supported)(struct wlan_objmgr_psoc *psoc);
	uint32_t (*sa_api_get_validate_sw)(struct wlan_objmgr_psoc *psoc);
	void (*sa_api_enable_sa)(struct wlan_objmgr_psoc *psoc, uint32_t value);
	uint32_t (*sa_api_get_sa_enable)(struct wlan_objmgr_psoc *psoc);
	void (*sa_api_peer_assoc_hanldler)(struct wlan_objmgr_pdev *pdev,
			struct wlan_objmgr_peer *peer, struct sa_rate_cap *);
	uint32_t (*sa_api_update_tx_feedback)(struct wlan_objmgr_pdev *pdev,
			struct wlan_objmgr_peer *peer,
			struct sa_tx_feedback *feedback);
	uint32_t (*sa_api_update_rx_feedback)(struct wlan_objmgr_pdev *pdev,
			struct wlan_objmgr_peer *peer,
			struct sa_rx_feedback *feedback);
	uint32_t (*sa_api_ucfg_set_param)(struct wlan_objmgr_pdev *pdev,
			char *val);
	uint32_t (*sa_api_ucfg_get_param)(struct wlan_objmgr_pdev *pdev,
			char *val);
	uint32_t (*sa_api_is_tx_feedback_enabled)
			(struct wlan_objmgr_pdev *pdev);
	uint32_t (*sa_api_is_rx_feedback_enabled)
			(struct wlan_objmgr_pdev *pdev);
	uint32_t (*sa_api_convert_rate_2g)(uint32_t rate);
	uint32_t (*sa_api_convert_rate_5g)(uint32_t rate);
	uint32_t (*sa_api_get_sa_mode)(struct wlan_objmgr_pdev *pdev);
	uint32_t (*sa_api_get_beacon_txantenna)(struct wlan_objmgr_pdev *pdev);
	uint32_t (*sa_api_cwm_action)(struct wlan_objmgr_pdev *pdev);
};
#endif

#ifdef WLAN_CFR_ENABLE

/**
 * struct wlan_lmac_if_cfr_rx_ops - CFR south bound rx function pointers
 * @cfr_support_set: Set the CFR support based on FW advert
 * @cfr_info_send: Send cfr info to upper layers
 * @cfr_capture_count_support_set: Set the capture_count support based on FW
 * advert
 * @cfr_mo_marking_support_set: Set MO marking supported based on FW advert
 * @cfr_aoa_for_rcc_support_set: Set AoA for RCC support based on FW advert
 */
struct wlan_lmac_if_cfr_rx_ops {
	void (*cfr_support_set)(struct wlan_objmgr_psoc *psoc, uint32_t value);
	uint32_t (*cfr_info_send)(struct wlan_objmgr_pdev *pdev, void *head,
				  size_t hlen, void *data, size_t dlen,
				  void *tail, size_t tlen);
	QDF_STATUS (*cfr_capture_count_support_set)(
			struct wlan_objmgr_psoc *psoc, uint32_t value);
	QDF_STATUS (*cfr_mo_marking_support_set)(struct wlan_objmgr_psoc *psoc,
						 uint32_t value);
	QDF_STATUS (*cfr_aoa_for_rcc_support_set)(
			struct wlan_objmgr_psoc *psoc, uint32_t value);
};
#endif

#ifdef WLAN_CONV_SPECTRAL_ENABLE
/**
 * struct wlan_lmac_if_sptrl_rx_ops - Spectral south bound Rx operations
 *
 * @sptrlro_get_pdev_target_handle: Get Spectral handle for pdev target
 * private data
 * @sptrlro_get_psoc_target_handle: Get Spectral handle for psoc target
 * private data
 * @sptrlro_vdev_get_chan_freq:
 * @sptrlro_vdev_get_chan_freq_seg2: Get secondary 80 center frequency
 * @sptrlro_vdev_get_ch_width:
 * @sptrlro_vdev_get_sec20chan_freq_mhz:
 * @sptrlro_spectral_is_feature_disabled_pdev: Check if spectral feature is
 * disabled for a given pdev
 * @sptrlro_spectral_is_feature_disabled_psoc: Check if spectral feature is
 * disabled for a given psoc
 */
struct wlan_lmac_if_sptrl_rx_ops {
	void * (*sptrlro_get_pdev_target_handle)(struct wlan_objmgr_pdev *pdev);
	void * (*sptrlro_get_psoc_target_handle)(struct wlan_objmgr_psoc *psoc);
	int16_t (*sptrlro_vdev_get_chan_freq)(struct wlan_objmgr_vdev *vdev);
	int16_t (*sptrlro_vdev_get_chan_freq_seg2)
					(struct wlan_objmgr_vdev *vdev);
	enum phy_ch_width (*sptrlro_vdev_get_ch_width)(
			struct wlan_objmgr_vdev *vdev);
	int (*sptrlro_vdev_get_sec20chan_freq_mhz)(
			struct wlan_objmgr_vdev *vdev,
			uint16_t *sec20chan_freq);
	bool (*sptrlro_spectral_is_feature_disabled_pdev)(
			struct wlan_objmgr_pdev *pdev);
	bool (*sptrlro_spectral_is_feature_disabled_psoc)(
			struct wlan_objmgr_psoc *psoc);
};
#endif /* WLAN_CONV_SPECTRAL_ENABLE */

#ifdef WLAN_IOT_SIM_SUPPORT
struct iot_sim_cbacks;
/**
 * struct wlan_lmac_if_iot_sim_rx_ops - iot_sim rx operations
 * @iot_sim_cmd_handler: Applies iot_sim rule in outgoing and incoming frames
 * @iot_sim_register_cb: callback registration with iot_sim
 */
struct wlan_lmac_if_iot_sim_rx_ops {
	QDF_STATUS (*iot_sim_cmd_handler)(struct wlan_objmgr_vdev *vdev,
					  qdf_nbuf_t n_buf,
					  struct beacon_tmpl_params *bcn_param,
					  bool tx,
					  struct mgmt_rx_event_params *param);
	QDF_STATUS (*iot_sim_register_cb)(struct wlan_objmgr_pdev *pdev,
					  struct iot_sim_cbacks *cb);
};
#endif

#ifdef WIFI_POS_CONVERGED
/**
 * struct wlan_lmac_if_wifi_pos_rx_ops - structure of rx function
 * pointers for wifi_pos component
 * @oem_rsp_event_rx: callback for WMI_OEM_RESPONSE_EVENTID
 * @wifi_pos_ranging_peer_create_cb: Function pointer to handle PASN peer create
 * request
 * @wifi_pos_ranging_peer_create_rsp_cb: Function pointer to handle peer create
 * confirm event for PASN Peer.
 * @wifi_pos_ranging_peer_delete_cb: Ranging peer delete handle function
 * pointer.
 * @wifi_pos_vdev_delete_all_ranging_peers_rsp_cb: Callback to handle vdev
 * delete all ranging peers response
 * @wifi_pos_vdev_delete_all_ranging_peers_cb: Delete all ranging peers for
 * given vdev. This is called before vdev delete to cleanup all the ranging
 * peers of that vdev.
 */
struct wlan_lmac_if_wifi_pos_rx_ops {
	int (*oem_rsp_event_rx)(struct wlan_objmgr_psoc *psoc,
				struct oem_data_rsp *oem_rsp);
	QDF_STATUS (*wifi_pos_ranging_peer_create_cb)
			(struct wlan_objmgr_psoc *psoc,
			 struct wlan_pasn_request *req,
			 uint8_t vdev_id,
			 uint8_t total_entries);
	QDF_STATUS (*wifi_pos_ranging_peer_create_rsp_cb)
			(struct wlan_objmgr_psoc *psoc, uint8_t vdev_id,
			 struct qdf_mac_addr *peer_mac,
			 uint8_t status);
	QDF_STATUS (*wifi_pos_ranging_peer_delete_cb)
			(struct wlan_objmgr_psoc *psoc,
			 struct wlan_pasn_request *info,
			 uint8_t vdev_id, uint8_t num_peers);
	QDF_STATUS (*wifi_pos_vdev_delete_all_ranging_peers_rsp_cb)
			(struct wlan_objmgr_psoc *psoc, uint8_t vdev_id);
	QDF_STATUS (*wifi_pos_vdev_delete_all_ranging_peers_cb)
					(struct wlan_objmgr_vdev *vdev);
};
#endif

/**
 * struct wlan_lmac_if_dfs_rx_ops - Function pointers to call dfs functions
 *                                  from lmac/offload.
 * @dfs_get_radars:                   Calls init radar table functions.
 * @dfs_process_phyerr:               Process phyerr.
 * @dfs_destroy_object:               Destroys the DFS object.
 * @dfs_radar_enable:                 Enables the radar.
 * @dfs_is_radar_enabled:             Check if the radar is enabled.
 * @dfs_control:                      Used to process ioctls related to DFS.
 * @dfs_is_precac_timer_running:      Check whether precac timer is running.
 * @dfs_agile_precac_start:           Initiate Agile PreCAC run.
 * @dfs_set_agile_precac_state:       Set agile precac state.
 * @dfs_reset_adfs_config:            Reset agile dfs variables.
 * @dfs_dfs_ocac_complete_ind:        Process offchan cac complete indication.
 * @dfs_start_precac_timer:           Start precac timer.
 * @dfs_cancel_precac_timer:          Cancel the precac timer.
 * @dfs_override_precac_timeout:      Override the default precac timeout.
 * @dfs_set_precac_enable:            Set precac enable flag.
 * @dfs_get_agile_precac_enable:      Get the agile precac enable flag
 * @dfs_set_precac_intermediate_chan: Set intermediate channel for precac.
 * @dfs_get_precac_intermediate_chan: Get intermediate channel for precac.
 * @dfs_decide_precac_preferred_chan_for_freq:
 *                                    Configure preferred channel during
 *                                    precac.
 * @dfs_get_precac_chan_state_for_freq:
 *                                    Get precac status for given channel.
 * @dfs_get_override_precac_timeout:  Get precac timeout.
 * @dfs_set_current_channel_for_freq:
 * @dfs_process_radar_ind:            Process radar found indication.
 * @dfs_dfs_cac_complete_ind:         Process cac complete indication.
 * @dfs_stop:                         Clear dfs timers.
 * @dfs_reinit_timers:                Reinitialize DFS timers.
 * @dfs_enable_stadfs:                Enable/Disable STADFS capability.
 * @dfs_is_stadfs_enabled:            Get STADFS capability value.
 * @dfs_process_phyerr_filter_offload:Process radar event.
 * @dfs_is_phyerr_filter_offload:     Check whether phyerr filter is offload.
 * @dfs_action_on_status:             Trigger the action to be taken based on
 *                                    on host dfs status received from fw.
 * @dfs_override_status_timeout:      Override the value of host dfs status
 *                                    wait timeout.
 * @dfs_get_override_status_timeout:  Get the value of host dfs status wait
 *                                    timeout.
 * @dfs_reset_spoof_test:             Checks if radar detection is enabled.
 * @dfs_is_disable_radar_marking_set: Check if dis_radar_marking param is set.
 * @dfs_set_nol_subchannel_marking:
 * @dfs_get_nol_subchannel_marking:
 * @dfs_set_bw_expand:                API to set BW Expansion feature.
 * @dfs_get_bw_expand:                API to get the status of BW Expansion
 *                                    feature.
 * @dfs_set_dfs_puncture:             API to set DFS puncturing feature.
 * @dfs_get_dfs_puncture:             API to get the status of DFS puncturing
 *                                    feature.
 * @dfs_set_bw_reduction:
 * @dfs_is_bw_reduction_needed:
 * @dfs_allow_hw_pulses:              Set or unset dfs_allow_hw_pulses which
 *                                    allow or disallow HW pulses.
 * @dfs_is_hw_pulses_allowed:         Check if HW pulses are allowed or not.
 * @dfs_set_fw_adfs_support:          Set the agile DFS FW support in DFS.
 * @dfs_reset_dfs_prevchan:           Reset DFS previous channel structure.
 * @dfs_init_tmp_psoc_nol:            Init temporary PSOC NOL structure.
 * @dfs_deinit_tmp_psoc_nol:          Deinit temporary PSOC NOL structure.
 * @dfs_save_dfs_nol_in_psoc:         Copy DFS NOL data to the PSOC copy.
 * @dfs_reinit_nol_from_psoc_copy:    Reinit DFS NOL from the PSOC NOL copy.
 * @dfs_reinit_precac_lists:          Reinit precac lists from other pdev.
 * @dfs_complete_deferred_tasks:      Process mode switch completion in DFS.
 * @dfs_set_rcac_enable:
 * @dfs_get_rcac_enable:
 * @dfs_set_rcac_freq:
 * @dfs_get_rcac_freq:
 * @dfs_is_agile_rcac_enabled:        Checks if Agile RCAC is enabled.
 * @dfs_agile_sm_deliver_evt:         API to post events to DFS Agile  SM.
 * @dfs_set_postnol_freq:             API to set frequency to switch, post NOL.
 * @dfs_set_postnol_mode:             API to set phymode to switch to, post NOL.
 * @dfs_set_postnol_cfreq2:           API to set secondary center frequency to
 *                                    switch to, post NOL.
 * @dfs_get_postnol_freq:             API to get frequency to switch, post NOL.
 * @dfs_get_postnol_mode:             API to get phymode to switch to, post NOL.
 * @dfs_get_postnol_cfreq2:           API to get secondary center frequency to
 *                                    switch to, post NOL.
 */
struct wlan_lmac_if_dfs_rx_ops {
	QDF_STATUS (*dfs_get_radars)(struct wlan_objmgr_pdev *pdev);
	QDF_STATUS (*dfs_process_phyerr)(struct wlan_objmgr_pdev *pdev,
			void *buf,
			uint16_t datalen,
			uint8_t r_rssi,
			uint8_t r_ext_rssi,
			uint32_t r_rs_tstamp,
			uint64_t r_fulltsf);
	QDF_STATUS (*dfs_destroy_object)(struct wlan_objmgr_pdev *pdev);
	QDF_STATUS (*dfs_radar_enable)(
			struct wlan_objmgr_pdev *pdev,
			int no_cac,
			uint32_t opmode,
			bool enable);
	void (*dfs_is_radar_enabled)(struct wlan_objmgr_pdev *pdev,
				     int *ignore_dfs);
	QDF_STATUS (*dfs_control)(struct wlan_objmgr_pdev *pdev,
				  u_int id,
				  void *indata,
				  uint32_t insize,
				  void *outdata,
				  uint32_t *outsize,
				  int *error);
	QDF_STATUS (*dfs_is_precac_timer_running)(struct wlan_objmgr_pdev *pdev,
						  bool *is_precac_timer_running
						  );
	QDF_STATUS (*dfs_agile_precac_start)(struct wlan_objmgr_pdev *pdev);
	QDF_STATUS (*dfs_set_agile_precac_state)(struct wlan_objmgr_pdev *pdev,
						 int agile_precac_state);
	QDF_STATUS (*dfs_reset_adfs_config)(struct wlan_objmgr_psoc *psoc);
	QDF_STATUS
	(*dfs_dfs_ocac_complete_ind)(struct wlan_objmgr_pdev *pdev,
				     struct vdev_adfs_complete_status *ocac_st);
	QDF_STATUS (*dfs_start_precac_timer)(struct wlan_objmgr_pdev *pdev);
	QDF_STATUS (*dfs_cancel_precac_timer)(struct wlan_objmgr_pdev *pdev);
	QDF_STATUS (*dfs_override_precac_timeout)(
			struct wlan_objmgr_pdev *pdev,
			int precac_timeout);
	QDF_STATUS (*dfs_set_precac_enable)(struct wlan_objmgr_pdev *pdev,
			uint32_t value);
	QDF_STATUS (*dfs_get_agile_precac_enable)(struct wlan_objmgr_pdev *pdev,
						  bool *buff);
#ifdef WLAN_DFS_PRECAC_AUTO_CHAN_SUPPORT
	QDF_STATUS (*dfs_set_precac_intermediate_chan)(struct wlan_objmgr_pdev *pdev,
						       uint32_t value);
	QDF_STATUS (*dfs_get_precac_intermediate_chan)(struct wlan_objmgr_pdev *pdev,
						       int *buff);
#ifdef CONFIG_CHAN_FREQ_API
	bool (*dfs_decide_precac_preferred_chan_for_freq)(struct
						    wlan_objmgr_pdev *pdev,
						    uint16_t *pref_chan_freq,
						    enum wlan_phymode mode);

	enum precac_chan_state (*dfs_get_precac_chan_state_for_freq)(struct
						      wlan_objmgr_pdev *pdev,
						      uint16_t pcac_freq);
#endif
#endif
	QDF_STATUS (*dfs_get_override_precac_timeout)(
			struct wlan_objmgr_pdev *pdev,
			int *precac_timeout);
#ifdef CONFIG_CHAN_FREQ_API
	QDF_STATUS
	    (*dfs_set_current_channel_for_freq)(struct wlan_objmgr_pdev *pdev,
						uint16_t ic_freq,
						uint64_t ic_flags,
						uint16_t ic_flagext,
						uint8_t ic_ieee,
						uint8_t ic_vhtop_ch_freq_seg1,
						uint8_t ic_vhtop_ch_freq_seg2,
						uint16_t dfs_ch_mhz_freq_seg1,
						uint16_t dfs_ch_mhz_freq_seg2,
						uint16_t dfs_ch_punc_pattern,
						bool *is_channel_updated);
#endif
#ifdef DFS_COMPONENT_ENABLE
	QDF_STATUS (*dfs_process_radar_ind)(struct wlan_objmgr_pdev *pdev,
			struct radar_found_info *radar_found);
	QDF_STATUS (*dfs_dfs_cac_complete_ind)(struct wlan_objmgr_pdev *pdev,
			uint32_t vdev_id);
#endif
	QDF_STATUS (*dfs_stop)(struct wlan_objmgr_pdev *pdev);
	QDF_STATUS (*dfs_reinit_timers)(struct wlan_objmgr_pdev *pdev);
	void (*dfs_enable_stadfs)(struct wlan_objmgr_pdev *pdev, bool val);
	bool (*dfs_is_stadfs_enabled)(struct wlan_objmgr_pdev *pdev);
	QDF_STATUS (*dfs_process_phyerr_filter_offload)(
			struct wlan_objmgr_pdev *pdev,
			struct radar_event_info *wlan_radar_info);
	QDF_STATUS (*dfs_is_phyerr_filter_offload)(
			struct wlan_objmgr_psoc *psoc,
			bool *is_phyerr_filter_offload);
#if defined(WLAN_DFS_PARTIAL_OFFLOAD) && defined(HOST_DFS_SPOOF_TEST)
	QDF_STATUS (*dfs_action_on_status)(struct wlan_objmgr_pdev *pdev,
			u_int32_t *dfs_status_check);
#endif
	QDF_STATUS (*dfs_override_status_timeout)(
			struct wlan_objmgr_pdev *pdev,
			int status_timeout);
	QDF_STATUS (*dfs_get_override_status_timeout)(
			struct wlan_objmgr_pdev *pdev,
			int *status_timeout);
	QDF_STATUS (*dfs_reset_spoof_test)(struct wlan_objmgr_pdev *pdev);
	QDF_STATUS (*dfs_is_disable_radar_marking_set)(struct wlan_objmgr_pdev
						 *pdev,
						 bool *disable_radar_marking);
	QDF_STATUS (*dfs_set_nol_subchannel_marking)(
			struct wlan_objmgr_pdev *pdev,
			bool value);
	QDF_STATUS (*dfs_get_nol_subchannel_marking)(
			struct wlan_objmgr_pdev *pdev,
			bool *value);
	QDF_STATUS (*dfs_set_bw_expand)(
			struct wlan_objmgr_pdev *pdev,
			bool value);
	QDF_STATUS (*dfs_get_bw_expand)(
			struct wlan_objmgr_pdev *pdev,
			bool *value);
#ifdef QCA_DFS_BW_PUNCTURE
	QDF_STATUS (*dfs_set_dfs_puncture)(
			struct wlan_objmgr_pdev *pdev,
			bool value);
	QDF_STATUS (*dfs_get_dfs_puncture)(
			struct wlan_objmgr_pdev *pdev,
			bool *value);
#endif
	QDF_STATUS (*dfs_set_bw_reduction)(struct wlan_objmgr_pdev *pdev,
			bool value);
	QDF_STATUS (*dfs_is_bw_reduction_needed)(struct wlan_objmgr_pdev *pdev,
			bool *bw_reduce);
	void (*dfs_allow_hw_pulses)(struct wlan_objmgr_pdev *pdev,
				    bool allow_hw_pulses);
	bool (*dfs_is_hw_pulses_allowed)(struct wlan_objmgr_pdev *pdev);
	void (*dfs_set_fw_adfs_support)(struct wlan_objmgr_pdev *pdev,
					bool fw_adfs_support_160,
					bool fw_adfs_support_non_160,
					bool fw_adfs_support_320);
	void (*dfs_reset_dfs_prevchan)(struct wlan_objmgr_pdev *pdev);
	void (*dfs_init_tmp_psoc_nol)(struct wlan_objmgr_pdev *pdev,
				      uint8_t num_radios);
	void (*dfs_deinit_tmp_psoc_nol)(struct wlan_objmgr_pdev *pdev);
	void (*dfs_save_dfs_nol_in_psoc)(struct wlan_objmgr_pdev *pdev,
					 uint8_t pdev_id);
	void (*dfs_reinit_nol_from_psoc_copy)(struct wlan_objmgr_pdev *pdev,
					      uint8_t pdev_id,
					      uint16_t low_5ghz_freq,
					      uint16_t high_5ghz_freq);
	void (*dfs_reinit_precac_lists)(struct wlan_objmgr_pdev *src_pdev,
					struct wlan_objmgr_pdev *dest_pdev,
					uint16_t low_5g_freq,
					uint16_t high_5g_freq);
	void (*dfs_complete_deferred_tasks)(struct wlan_objmgr_pdev *pdev);
#ifdef QCA_SUPPORT_ADFS_RCAC
	QDF_STATUS (*dfs_set_rcac_enable)(struct wlan_objmgr_pdev *pdev,
					  bool rcac_en);
	QDF_STATUS (*dfs_get_rcac_enable)(struct wlan_objmgr_pdev *pdev,
					  bool *rcac_en);
	QDF_STATUS (*dfs_set_rcac_freq)(struct wlan_objmgr_pdev *pdev,
					qdf_freq_t rcac_freq);
	QDF_STATUS (*dfs_get_rcac_freq)(struct wlan_objmgr_pdev *pdev,
					qdf_freq_t *rcac_freq);
	bool (*dfs_is_agile_rcac_enabled)(struct wlan_objmgr_pdev *pdev);
#endif
#ifdef QCA_SUPPORT_AGILE_DFS
	void (*dfs_agile_sm_deliver_evt)(struct wlan_objmgr_pdev *pdev,
					 enum dfs_agile_sm_evt event);
#endif
#ifdef QCA_SUPPORT_DFS_CHAN_POSTNOL
	QDF_STATUS (*dfs_set_postnol_freq)(struct wlan_objmgr_pdev *pdev,
					   qdf_freq_t postnol_freq);
	QDF_STATUS (*dfs_set_postnol_mode)(struct wlan_objmgr_pdev *pdev,
					   uint16_t postnol_mode);
	QDF_STATUS (*dfs_set_postnol_cfreq2)(struct wlan_objmgr_pdev *pdev,
					     qdf_freq_t postnol_cfreq2);
	QDF_STATUS (*dfs_get_postnol_freq)(struct wlan_objmgr_pdev *pdev,
					   qdf_freq_t *postnol_freq);
	QDF_STATUS (*dfs_get_postnol_mode)(struct wlan_objmgr_pdev *pdev,
					   uint8_t *postnol_mode);
	QDF_STATUS (*dfs_get_postnol_cfreq2)(struct wlan_objmgr_pdev *pdev,
					     qdf_freq_t *postnol_cfreq2);
#endif
};

/**
 * struct wlan_lmac_if_mlme_rx_ops: Function pointer to call MLME functions
 * @vdev_mgr_start_response: function to handle start response
 * @vdev_mgr_stop_response: function to handle stop response
 * @vdev_mgr_delete_response: function to handle delete response
 * @vdev_mgr_offload_bcn_tx_status_event_handle: function to handle offload
 * beacon tx
 * @vdev_mgr_tbttoffset_update_handle: function to handle tbtt offset event
 * @vdev_mgr_peer_delete_all_response: function to handle vdev delete all peer
 * event
 * @psoc_get_wakelock_info: function to get wakelock info
 * @psoc_get_vdev_response_timer_info: function to get vdev response timer
 * structure for a specific vdev id
 * @vdev_mgr_multi_vdev_restart_resp: function to handle mvr response
 * @vdev_mgr_set_mac_addr_response: Callback to get response for set MAC address
 *                                  command
 * @vdev_mgr_set_max_channel_switch_time: Set max channel switch time for the
 * given vdev list.
 * @vdev_mgr_quiet_offload: handle quiet status for given link mac addr or
 * mld addr and link id.
 * @vdev_mgr_csa_received: function to handle csa ie received event
 */
struct wlan_lmac_if_mlme_rx_ops {
	QDF_STATUS (*vdev_mgr_start_response)(
					struct wlan_objmgr_psoc *psoc,
					struct vdev_start_response *rsp);
	QDF_STATUS (*vdev_mgr_stop_response)(
					struct wlan_objmgr_psoc *psoc,
					struct vdev_stop_response *rsp);
	QDF_STATUS (*vdev_mgr_delete_response)(
					struct wlan_objmgr_psoc *psoc,
					struct vdev_delete_response *rsp);
	QDF_STATUS (*vdev_mgr_offload_bcn_tx_status_event_handle)(
							uint32_t vdev_id,
							uint32_t tx_status);
	QDF_STATUS (*vdev_mgr_tbttoffset_update_handle)(
						uint32_t num_vdevs,
						bool is_ext);
	QDF_STATUS (*vdev_mgr_peer_delete_all_response)(
					struct wlan_objmgr_psoc *psoc,
					struct peer_delete_all_response *rsp);
	QDF_STATUS (*vdev_mgr_multi_vdev_restart_resp)(
					struct wlan_objmgr_psoc *psoc,
					struct multi_vdev_restart_resp *rsp);
#ifdef FEATURE_VDEV_OPS_WAKELOCK
	struct psoc_mlme_wakelock *(*psoc_get_wakelock_info)(
				    struct wlan_objmgr_psoc *psoc);
#endif
	struct vdev_response_timer *(*psoc_get_vdev_response_timer_info)(
						struct wlan_objmgr_psoc *psoc,
						uint8_t vdev_id);
#ifdef WLAN_FEATURE_DYNAMIC_MAC_ADDR_UPDATE
	void (*vdev_mgr_set_mac_addr_response)(struct wlan_objmgr_vdev *vdev,
					       uint8_t status);
#endif
	void (*vdev_mgr_set_max_channel_switch_time)
		(struct wlan_objmgr_psoc *psoc,
		 uint32_t *vdev_ids, uint32_t num_vdevs);
#ifdef WLAN_FEATURE_11BE_MLO
	QDF_STATUS (*vdev_mgr_quiet_offload)(
			struct wlan_objmgr_psoc *psoc,
			struct vdev_sta_quiet_event *quiet_event);
#endif
	QDF_STATUS (*vdev_mgr_csa_received)(struct wlan_objmgr_psoc *psoc,
					    uint8_t vdev_id,
					    struct csa_offload_params *csa_event);
};

#ifdef WLAN_SUPPORT_GREEN_AP
struct wlan_lmac_if_green_ap_rx_ops {
	bool (*is_ps_enabled)(struct wlan_objmgr_pdev *pdev);
	bool (*is_dbg_print_enabled)(struct wlan_objmgr_pdev *pdev);
	QDF_STATUS (*ps_get)(struct wlan_objmgr_pdev *pdev, uint8_t *value);
	QDF_STATUS (*ps_set)(struct wlan_objmgr_pdev *pdev, uint8_t value);
	void (*suspend_handle)(struct wlan_objmgr_pdev *pdev);
#if defined(WLAN_SUPPORT_GAP_LL_PS_MODE)
	QDF_STATUS (*ll_ps_cb)(struct wlan_green_ap_ll_ps_event_param
			       *ll_ps_event_param);
#endif
};
#endif

/**
 * struct wlan_lmac_if_rx_ops - south bound rx function pointers
 * @mgmt_txrx_rx_ops: mgmt txrx rx ops
 * @scan: scan rx ops
 * @son_rx_ops: son rx ops
 * @p2p: p2p rx ops
 * @iot_sim_rx_ops: iot simulation rx ops
 * @atf_rx_ops: air time fairness rx ops
 * @cp_stats_rx_ops: cp stats rx ops
 * @dcs_rx_ops: dcs rx ops
 * @sa_api_rx_ops: sa api rx ops
 * @cfr_rx_ops: cfr rx ops
 * @sptrl_rx_ops: spectral rx ops
 * @crypto_rx_ops: crypto rx ops
 * @wifi_pos_rx_ops: wifi positioning rx ops
 * @reg_rx_ops: regulatory rx ops
 * @dfs_rx_ops: dfs rx ops
 * @tdls_rx_ops: tdls rx ops
 * @fd_rx_ops: fils rx ops
 * @mops: mlme rx ops
 * @green_ap_rx_ops: green ap rx ops
 * @ftm_rx_ops: ftm rx ops
 * @mlo_rx_ops: mlo rx ops
 * @twt_rx_ops: twt rx ops
 * @dbam_rx_ops: dbam rx ops
 *
 * Callback function tabled to be registered with lmac/wmi.
 * lmac will use the functional table to send events/frames to umac
 */
struct wlan_lmac_if_rx_ops {
	/*
	 * Components to declare function pointers required by the module
	 * in component specific structure.
	 * The component specific ops structure can be declared in this file
	 * only
	 */
	struct wlan_lmac_if_mgmt_txrx_rx_ops mgmt_txrx_rx_ops;
	struct wlan_lmac_if_scan_rx_ops scan;
	struct wlan_lmac_if_son_rx_ops son_rx_ops;

#ifdef CONVERGED_P2P_ENABLE
	struct wlan_lmac_if_p2p_rx_ops p2p;
#endif
#ifdef WLAN_IOT_SIM_SUPPORT
	struct wlan_lmac_if_iot_sim_rx_ops iot_sim_rx_ops;
#endif
#ifdef WLAN_ATF_ENABLE
	struct wlan_lmac_if_atf_rx_ops atf_rx_ops;
#endif
#ifdef QCA_SUPPORT_CP_STATS
	struct wlan_lmac_if_cp_stats_rx_ops cp_stats_rx_ops;
#endif
#ifdef DCS_INTERFERENCE_DETECTION
	struct wlan_target_if_dcs_rx_ops dcs_rx_ops;
#endif
#ifdef WLAN_SA_API_ENABLE
	struct wlan_lmac_if_sa_api_rx_ops sa_api_rx_ops;
#endif

#ifdef WLAN_CFR_ENABLE
	struct wlan_lmac_if_cfr_rx_ops cfr_rx_ops;
#endif

#ifdef WLAN_CONV_SPECTRAL_ENABLE
	struct wlan_lmac_if_sptrl_rx_ops sptrl_rx_ops;
#endif

	struct wlan_lmac_if_crypto_rx_ops crypto_rx_ops;
#ifdef WIFI_POS_CONVERGED
	struct wlan_lmac_if_wifi_pos_rx_ops wifi_pos_rx_ops;
#endif
	struct wlan_lmac_if_reg_rx_ops reg_rx_ops;
	struct wlan_lmac_if_dfs_rx_ops dfs_rx_ops;
#ifdef FEATURE_WLAN_TDLS
	struct wlan_lmac_if_tdls_rx_ops tdls_rx_ops;
#endif

#ifdef WLAN_SUPPORT_FILS
	struct wlan_lmac_if_fd_rx_ops fd_rx_ops;
#endif

	struct wlan_lmac_if_mlme_rx_ops mops;

#ifdef WLAN_SUPPORT_GREEN_AP
	struct wlan_lmac_if_green_ap_rx_ops green_ap_rx_ops;
#endif

	struct wlan_lmac_if_ftm_rx_ops ftm_rx_ops;
#ifdef WLAN_FEATURE_11BE_MLO
	struct wlan_lmac_if_mlo_rx_ops mlo_rx_ops;
#endif
#if defined(WLAN_SUPPORT_TWT) && defined(WLAN_TWT_CONV_SUPPORTED)
	struct wlan_lmac_if_twt_rx_ops twt_rx_ops;
#endif
#ifdef WLAN_FEATURE_DBAM_CONFIG
	struct wlan_lmac_if_dbam_rx_ops dbam_rx_ops;
#endif
};

/* Function pointer to call legacy tx_ops registration in OL/WMA.
 */
extern QDF_STATUS (*wlan_lmac_if_umac_tx_ops_register)
				(struct wlan_lmac_if_tx_ops *tx_ops);

/* Function pointer to call legacy crypto rxpn registration in OL */
extern QDF_STATUS (*wlan_lmac_if_umac_crypto_rxpn_ops_register)
				(struct wlan_lmac_if_rx_ops *rx_ops);
#ifdef WLAN_FEATURE_SON
/**
 * wlan_lmac_if_son_mod_register_rx_ops() - SON Module lmac_if rx_ops
 *                                          registration API
 * @rx_ops: lmac_if rx_ops function pointer table
 *
 * Function to register SON rx_ops with lmac_if.
 *
 * Return: void
 */
void wlan_lmac_if_son_mod_register_rx_ops(struct wlan_lmac_if_rx_ops *rx_ops);
#endif

#endif /* _WLAN_LMAC_IF_DEF_H_ */
