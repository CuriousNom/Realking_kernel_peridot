// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2012-2021, The Linux Foundation. All rights reserved.
 * Copyright (c) 2023-2024 Qualcomm Innovation Center, Inc. All rights reserved.
 */

#include "ipa_i.h"
#include "ipahal.h"

static const u32 ipa_hdr_bin_sz[IPA_HDR_BIN_MAX] = { 8, 16, 24, 36, 64, 128};
static const u32 ipa_hdr_proc_ctx_bin_sz[IPA_HDR_PROC_CTX_BIN_MAX] = { 32, 64};

#define HDR_TYPE_IS_VALID(type) \
	((type) >= 0 && (type) < IPA_HDR_L2_MAX)

#define HDR_PROC_TYPE_IS_VALID(type) \
	((type) >= 0 && (type) < IPA_HDR_PROC_MAX)

/**
 * ipa3_generate_hdr_hw_tbl() - generates the headers table
 * @loc:	[in] storage type of the header table buffer (local or system)
 * @mem:	[out] buffer to put the header table
 *
 * Returns:	0 on success, negative on failure
 */
static int ipa3_generate_hdr_hw_tbl(enum hdr_tbl_storage loc, struct ipa_mem_buffer *mem)
{
	struct ipa3_hdr_entry *entry;
	gfp_t flag = GFP_KERNEL;

	mem->size = (ipa3_ctx->hdr_tbl[loc].end) ? ipa3_ctx->hdr_tbl[loc].end : ipa_hdr_bin_sz[0];

	if (mem->size == 0) {
		IPAERR("%s hdr tbl empty\n", loc == HDR_TBL_LCL ? "SRAM" : "DDR");
		return -EPERM;
	}
	IPADBG_LOW("tbl_sz=%d\n", mem->size);

alloc:
	mem->base = dma_alloc_coherent(ipa3_ctx->pdev, mem->size,
			&mem->phys_base, flag);
	if (!mem->base) {
		if (flag == GFP_KERNEL) {
			flag = GFP_ATOMIC;
			goto alloc;
		}
		IPAERR("fail to alloc DMA buff of size %d\n", mem->size);
		return -ENOMEM;
	}

	list_for_each_entry(entry, &ipa3_ctx->hdr_tbl[loc].head_hdr_entry_list, link) {
		IPADBG_LOW("hdr of len %d ofst=%d\n", entry->hdr_len,
				entry->offset_entry->offset);
		ipahal_cp_hdr_to_hw_buff(mem->base, entry->offset_entry->offset,
				entry->hdr, entry->hdr_len);
	}

	return 0;
}

static int ipa3_hdr_proc_ctx_to_hw_format(struct ipa_mem_buffer *mem,
	u64 hdr_sys_addr)
{
	struct ipa3_hdr_proc_ctx_entry *entry;
	int ret;
	int ep;
	struct ipa_ep_cfg *cfg_ptr;
	struct ipa_l2tp_header_remove_procparams *l2p_hdr_rm_ptr;
	u32 hdr_lcl_addr;

	hdr_lcl_addr = ipa3_ctx->ipa_wrapper_base +
		ipa3_ctx->ctrl->ipa_reg_base_ofst +
		ipahal_get_reg_n_ofst(IPA_SW_AREA_RAM_DIRECT_ACCESS_n,
			ipa3_ctx->smem_restricted_bytes / 4) +
		IPA_MEM_PART(apps_hdr_ofst);

	list_for_each_entry(entry,
			&ipa3_ctx->hdr_proc_ctx_tbl.head_proc_ctx_entry_list,
			link) {
		IPADBG_LOW("processing type %d ofst=%d\n",
			entry->type, entry->offset_entry->offset);

		if (entry->l2tp_params.is_dst_pipe_valid) {
			ep = ipa_get_ep_mapping(entry->l2tp_params.dst_pipe);

			if (ep >= 0) {
				cfg_ptr = &ipa3_ctx->ep[ep].cfg;
				l2p_hdr_rm_ptr =
					&entry->l2tp_params.hdr_remove_param;
				l2p_hdr_rm_ptr->hdr_ofst_pkt_size_valid =
					cfg_ptr->hdr.hdr_ofst_pkt_size_valid;
				l2p_hdr_rm_ptr->hdr_ofst_pkt_size =
					cfg_ptr->hdr.hdr_ofst_pkt_size;
				l2p_hdr_rm_ptr->hdr_endianness =
					cfg_ptr->hdr_ext.hdr_little_endian ?
					0 : 1;
			}
		}

		/* Check the pointer and header length to avoid dangerous overflow in HW */
		if (unlikely(!entry->hdr || !entry->hdr->offset_entry ||
			!entry->offset_entry ||
			entry->hdr->hdr_len > ipa_hdr_bin_sz[IPA_HDR_BIN_MAX - 1])) {
			IPAERR_RL("Found invalid hdr entry\n");
			return -EINVAL;
		}

		ret = ipahal_cp_proc_ctx_to_hw_buff(entry->type, mem->base,
				entry->offset_entry->offset,
				entry->hdr->hdr_len,
				(entry->hdr->is_lcl) ? hdr_lcl_addr : hdr_sys_addr,
				entry->hdr->offset_entry,
				&entry->l2tp_params,
				&entry->eogre_params,
				&entry->generic_params,
				&entry->rtp_params,
				ipa3_ctx->use_64_bit_dma_mask);
		if (ret)
			return ret;
	}

	return 0;
}

/**
 * ipa3_generate_hdr_proc_ctx_hw_tbl() -
 * generates the headers processing context table.
 * @mem:		[out] buffer to put the processing context table
 * @aligned_mem:	[out] actual processing context table (with alignment).
 *			Processing context table needs to be 8 Bytes aligned.
 *
 * Returns:	0 on success, negative on failure
 */
static int ipa3_generate_hdr_proc_ctx_hw_tbl(u64 hdr_sys_addr,
	struct ipa_mem_buffer *mem, struct ipa_mem_buffer *aligned_mem)
{
	gfp_t flag = GFP_KERNEL;
	int ret;

	mem->size = (ipa3_ctx->hdr_proc_ctx_tbl.end) ? : 4;

	/* make sure table is aligned */
	mem->size += IPA_HDR_PROC_CTX_TABLE_ALIGNMENT_BYTE;

	IPADBG_LOW("tbl_sz=%d\n", ipa3_ctx->hdr_proc_ctx_tbl.end);

alloc:
	mem->base = dma_alloc_coherent(ipa3_ctx->pdev, mem->size,
			&mem->phys_base, flag);
	if (!mem->base) {
		if (flag == GFP_KERNEL) {
			flag = GFP_ATOMIC;
			goto alloc;
		}
		IPAERR("fail to alloc DMA buff of size %d\n", mem->size);
		return -ENOMEM;
	}

	aligned_mem->phys_base =
		IPA_HDR_PROC_CTX_TABLE_ALIGNMENT(mem->phys_base);
	aligned_mem->base = mem->base +
		(aligned_mem->phys_base - mem->phys_base);
	aligned_mem->size = mem->size - IPA_HDR_PROC_CTX_TABLE_ALIGNMENT_BYTE;
	memset(aligned_mem->base, 0, aligned_mem->size);
	ret = ipa3_hdr_proc_ctx_to_hw_format(aligned_mem, hdr_sys_addr);
	if (ret) {
		dma_free_coherent(ipa3_ctx->pdev, mem->size, mem->base, mem->phys_base);
		return ret;
	}
	return ret;
}

/**
 * __ipa_commit_hdr_v3_0() - Commits the header table from memory to HW
 *
 * Returns:	0 on success, negative on failure
 */
int __ipa_commit_hdr_v3_0(void)
{
	struct ipa3_desc desc[4];
	struct ipa_mem_buffer hdr_mem[HDR_TBLS_TOTAL] = {0};
	struct ipa_mem_buffer ctx_mem;
	struct ipa_mem_buffer aligned_ctx_mem;
	struct ipahal_imm_cmd_dma_shared_mem dma_cmd_hdr = {0};
	struct ipahal_imm_cmd_dma_shared_mem dma_cmd_ctx = {0};
	struct ipahal_imm_cmd_register_write reg_write_cmd = {0};
	struct ipahal_imm_cmd_hdr_init_system hdr_init_cmd = {0};
	struct ipahal_imm_cmd_pyld *hdr_cmd_pyld[HDR_TBLS_TOTAL] = {0};
	struct ipahal_imm_cmd_pyld *ctx_cmd_pyld = NULL;
	struct ipahal_imm_cmd_pyld *coal_cmd_pyld = NULL;
	int rc = -EFAULT;
	int i;
	int num_cmd = 0;
	u32 hdr_tbl_size, proc_ctx_size;
	u32 proc_ctx_ofst;
	u32 proc_ctx_size_ddr;
	struct ipahal_imm_cmd_register_write reg_write_coal_close = { 0 };
	struct ipahal_reg_valmask valmask;
	enum hdr_tbl_storage loc;

	memset(desc, 0, 3 * sizeof(struct ipa3_desc));

	/* Generate structures for both SRAM and DDR header tables */
	for (loc = HDR_TBL_LCL; loc < HDR_TBLS_TOTAL; loc++) {
		hdr_tbl_size = (loc == HDR_TBL_LCL) ?
			IPA_MEM_PART(apps_hdr_size) : IPA_MEM_PART(apps_hdr_size_ddr);

		if (hdr_tbl_size) {
			if (ipa3_generate_hdr_hw_tbl(loc, &hdr_mem[loc])) {
				IPAERR("fail to generate %s HDR HW TBL\n",
				       loc == HDR_TBL_LCL ? "SRAM" : "DDR");
				goto end;
			}

			if (hdr_mem[loc].size > hdr_tbl_size) {
				IPAERR("%s HDR tbl too big needed %d avail %d\n",
				       loc == HDR_TBL_LCL ? "SRAM" : "DDR",
				       hdr_mem[loc].size, hdr_tbl_size);
				goto free_dma;
			}
		}
	}

	/* IC to close the coal frame before HPS Clear if coal is enabled */
	if (ipa_get_ep_mapping(IPA_CLIENT_APPS_WAN_COAL_CONS) != -1
		&& !ipa3_ctx->ulso_wa) {
		u32 offset = 0;

		i = ipa_get_ep_mapping(IPA_CLIENT_APPS_WAN_COAL_CONS);
		reg_write_coal_close.skip_pipeline_clear = false;
		reg_write_coal_close.pipeline_clear_options = IPAHAL_HPS_CLEAR;
		if (ipa3_ctx->ipa_hw_type < IPA_HW_v5_0)
			offset = ipahal_get_reg_ofst(
				IPA_AGGR_FORCE_CLOSE);
		else
			offset = ipahal_get_ep_reg_offset(
				IPA_AGGR_FORCE_CLOSE_n, i);
		reg_write_coal_close.offset = offset;
		ipahal_get_aggr_force_close_valmask(i, &valmask);
		reg_write_coal_close.value = valmask.val;
		reg_write_coal_close.value_mask = valmask.mask;
		coal_cmd_pyld = ipahal_construct_imm_cmd(
			IPA_IMM_CMD_REGISTER_WRITE,
			&reg_write_coal_close, false);
		if (!coal_cmd_pyld) {
			IPAERR("failed to construct coal close IC\n");
			goto end;
		}
		ipa3_init_imm_cmd_desc(&desc[num_cmd], coal_cmd_pyld);
		++num_cmd;
	}

	/* Local (SRAM) header table configuration */
	if (IPA_MEM_PART(apps_hdr_size)) {
		dma_cmd_hdr.is_read = false; /* write operation */
		dma_cmd_hdr.skip_pipeline_clear = false;
		dma_cmd_hdr.pipeline_clear_options = IPAHAL_HPS_CLEAR;
		dma_cmd_hdr.system_addr = hdr_mem[HDR_TBL_LCL].phys_base;
		dma_cmd_hdr.size = hdr_mem[HDR_TBL_LCL].size;
		dma_cmd_hdr.local_addr =
			ipa3_ctx->smem_restricted_bytes +
			IPA_MEM_PART(apps_hdr_ofst);
		hdr_cmd_pyld[HDR_TBL_LCL] = ipahal_construct_imm_cmd(IPA_IMM_CMD_DMA_SHARED_MEM,
								     &dma_cmd_hdr, false);
		if (!hdr_cmd_pyld[HDR_TBL_LCL]) {
			IPAERR("fail construct dma_shared_mem cmd\n");
			goto end;
		}

		ipa3_init_imm_cmd_desc(&desc[num_cmd], hdr_cmd_pyld[HDR_TBL_LCL]);
		++num_cmd;
		IPA_DUMP_BUFF(hdr_mem[HDR_TBL_LCL].base,
			      hdr_mem[HDR_TBL_LCL].phys_base,
			      hdr_mem[HDR_TBL_LCL].size);

	}

	/* System (DDR) header table configuration */
	if (IPA_MEM_PART(apps_hdr_size_ddr)) {
		hdr_init_cmd.hdr_table_addr = hdr_mem[HDR_TBL_SYS].phys_base;
		hdr_cmd_pyld[HDR_TBL_SYS] = ipahal_construct_imm_cmd(IPA_IMM_CMD_HDR_INIT_SYSTEM,
								     &hdr_init_cmd, false);
		if (!hdr_cmd_pyld[HDR_TBL_SYS]) {
			IPAERR("fail construct hdr_init_system cmd\n");
			goto free_dma;
		}

		ipa3_init_imm_cmd_desc(&desc[num_cmd], hdr_cmd_pyld[HDR_TBL_SYS]);
		++num_cmd;
		IPA_DUMP_BUFF(hdr_mem[HDR_TBL_SYS].base,
			      hdr_mem[HDR_TBL_SYS].phys_base,
			      hdr_mem[HDR_TBL_SYS].size);
	}

	/* The header memory passed to the HPC here is DDR (system),
	   but the actual header base will be determined later for each header */
	if (ipa3_generate_hdr_proc_ctx_hw_tbl(hdr_mem[HDR_TBL_SYS].phys_base,
					      &ctx_mem,
					      &aligned_ctx_mem)) {
		IPAERR("fail to generate HDR PROC CTX HW TBL\n");
		goto end;
	}

	proc_ctx_size = IPA_MEM_PART(apps_hdr_proc_ctx_size);
	proc_ctx_ofst = IPA_MEM_PART(apps_hdr_proc_ctx_ofst);
	if (ipa3_ctx->hdr_proc_ctx_tbl_lcl) {
		if (aligned_ctx_mem.size > proc_ctx_size) {
			IPAERR("tbl too big needed %d avail %d\n",
				aligned_ctx_mem.size,
				proc_ctx_size);
			goto end;
		} else {
			dma_cmd_ctx.is_read = false; /* Write operation */
			dma_cmd_ctx.skip_pipeline_clear = false;
			dma_cmd_ctx.pipeline_clear_options = IPAHAL_HPS_CLEAR;
			dma_cmd_ctx.system_addr = aligned_ctx_mem.phys_base;
			dma_cmd_ctx.size = aligned_ctx_mem.size;
			dma_cmd_ctx.local_addr =
				ipa3_ctx->smem_restricted_bytes +
				proc_ctx_ofst;
			ctx_cmd_pyld = ipahal_construct_imm_cmd(
				IPA_IMM_CMD_DMA_SHARED_MEM,
				&dma_cmd_ctx, false);
			if (!ctx_cmd_pyld) {
				IPAERR("fail construct dma_shared_mem cmd\n");
				goto end;
			}
		}
	} else {
		proc_ctx_size_ddr = IPA_MEM_PART(apps_hdr_proc_ctx_size_ddr);
		if (aligned_ctx_mem.size > proc_ctx_size_ddr) {
			IPAERR("tbl too big, needed %d avail %d\n",
				aligned_ctx_mem.size,
				proc_ctx_size_ddr);
			goto end;
		} else {
			reg_write_cmd.skip_pipeline_clear = false;
			reg_write_cmd.pipeline_clear_options =
				IPAHAL_HPS_CLEAR;
			reg_write_cmd.offset =
				ipahal_get_reg_ofst(
				IPA_SYS_PKT_PROC_CNTXT_BASE);
			reg_write_cmd.value = aligned_ctx_mem.phys_base;
			reg_write_cmd.value_mask =
				~(IPA_HDR_PROC_CTX_TABLE_ALIGNMENT_BYTE - 1);
			ctx_cmd_pyld = ipahal_construct_imm_cmd(
				IPA_IMM_CMD_REGISTER_WRITE,
				&reg_write_cmd, false);
			if (!ctx_cmd_pyld) {
				IPAERR("fail construct register_write cmd\n");
				goto end;
			}
		}
	}
	ipa3_init_imm_cmd_desc(&desc[num_cmd], ctx_cmd_pyld);
	++num_cmd;
	IPA_DUMP_BUFF(ctx_mem.base, ctx_mem.phys_base, ctx_mem.size);

	if (ipa3_send_cmd(num_cmd, desc))
		IPAERR("fail to send immediate command\n");
	else
		rc = 0;

	if (!rc && hdr_mem[HDR_TBL_SYS].base) {
		if (ipa3_ctx->hdr_sys_mem.phys_base) {
			dma_free_coherent(ipa3_ctx->pdev,
					  ipa3_ctx->hdr_sys_mem.size,
					  ipa3_ctx->hdr_sys_mem.base,
					  ipa3_ctx->hdr_sys_mem.phys_base);
		}
		ipa3_ctx->hdr_sys_mem = hdr_mem[HDR_TBL_SYS];
	}

	else {
		dma_free_coherent(ipa3_ctx->pdev, hdr_mem[HDR_TBL_SYS].size,
		hdr_mem[HDR_TBL_SYS].base,hdr_mem[HDR_TBL_SYS].phys_base);
        }

	if (ipa3_ctx->hdr_proc_ctx_tbl_lcl) {
		dma_free_coherent(ipa3_ctx->pdev, ctx_mem.size, ctx_mem.base,
			ctx_mem.phys_base);
	} else {
		if (!rc) {
			if (ipa3_ctx->hdr_proc_ctx_mem.phys_base)
				dma_free_coherent(ipa3_ctx->pdev,
					ipa3_ctx->hdr_proc_ctx_mem.size,
					ipa3_ctx->hdr_proc_ctx_mem.base,
					ipa3_ctx->hdr_proc_ctx_mem.phys_base);
			ipa3_ctx->hdr_proc_ctx_mem = ctx_mem;
		}
		else {
			dma_free_coherent(ipa3_ctx->pdev, ctx_mem.size,
			ctx_mem.base,ctx_mem.phys_base);
		}
	}
	goto end;

free_dma:
	if (hdr_mem[HDR_TBL_SYS].base) {
		dma_free_coherent(ipa3_ctx->pdev,
				  hdr_mem[HDR_TBL_SYS].size,
				  hdr_mem[HDR_TBL_SYS].base,
				  hdr_mem[HDR_TBL_SYS].phys_base);
	}

end:
	if (hdr_mem[HDR_TBL_LCL].base) {
		dma_free_coherent(ipa3_ctx->pdev,
				  hdr_mem[HDR_TBL_LCL].size,
				  hdr_mem[HDR_TBL_LCL].base,
				  hdr_mem[HDR_TBL_LCL].phys_base);
	}

	if (coal_cmd_pyld)
		ipahal_destroy_imm_cmd(coal_cmd_pyld);

	if (ctx_cmd_pyld)
		ipahal_destroy_imm_cmd(ctx_cmd_pyld);

	if (hdr_cmd_pyld[HDR_TBL_SYS])
		ipahal_destroy_imm_cmd(hdr_cmd_pyld[HDR_TBL_SYS]);

	if (hdr_cmd_pyld[HDR_TBL_LCL])
		ipahal_destroy_imm_cmd(hdr_cmd_pyld[HDR_TBL_LCL]);

	return rc;
}

static int __ipa_add_hdr_proc_ctx(struct ipa_hdr_proc_ctx_add *proc_ctx,
	bool add_ref_hdr, bool user_only)
{
	struct ipa3_hdr_entry *hdr_entry;
	struct ipa3_hdr_proc_ctx_entry *entry;
	struct ipa3_hdr_proc_ctx_offset_entry *offset;
	u32 bin;
	struct ipa3_hdr_proc_ctx_tbl *htbl = &ipa3_ctx->hdr_proc_ctx_tbl;
	int id;
	int needed_len;
	int mem_size;

	IPADBG_LOW("Add processing type %d hdr_hdl %d\n",
		proc_ctx->type, proc_ctx->hdr_hdl);

	if (!HDR_PROC_TYPE_IS_VALID(proc_ctx->type)) {
		IPAERR_RL("invalid processing type %d\n", proc_ctx->type);
		return -EINVAL;
	}

	hdr_entry = ipa3_id_find(proc_ctx->hdr_hdl);
	if (!hdr_entry) {
		IPAERR_RL("hdr_hdl is invalid\n");
		return -EINVAL;
	}
	if (hdr_entry->cookie != IPA_HDR_COOKIE) {
		IPAERR_RL("Invalid header cookie %u\n", hdr_entry->cookie);
		WARN_ON_RATELIMIT_IPA(1);
		return -EINVAL;
	}
	IPADBG("Associated header is name=%s\n", hdr_entry->name);

	entry = kmem_cache_zalloc(ipa3_ctx->hdr_proc_ctx_cache, GFP_KERNEL);
	if (!entry) {
		IPAERR("failed to alloc proc_ctx object\n");
		return -ENOMEM;
	}

	INIT_LIST_HEAD(&entry->link);

	entry->type = proc_ctx->type;
	entry->hdr = hdr_entry;
	entry->l2tp_params = proc_ctx->l2tp_params;
	entry->eogre_params = proc_ctx->eogre_params;
	entry->generic_params = proc_ctx->generic_params;
	if (add_ref_hdr)
		hdr_entry->ref_cnt++;
	entry->cookie = IPA_PROC_HDR_COOKIE;
	entry->ipacm_installed = user_only;

	needed_len = ipahal_get_proc_ctx_needed_len(proc_ctx->type);
	if ((needed_len < 0) ||
		((needed_len > ipa_hdr_proc_ctx_bin_sz[IPA_HDR_PROC_CTX_BIN0])
			&&
			(needed_len >
			ipa_hdr_proc_ctx_bin_sz[IPA_HDR_PROC_CTX_BIN1]))) {
		IPAERR_RL("unexpected needed len %d\n", needed_len);
		WARN_ON_RATELIMIT_IPA(1);
		goto bad_len;
	}

	if (needed_len <= ipa_hdr_proc_ctx_bin_sz[IPA_HDR_PROC_CTX_BIN0])
		bin = IPA_HDR_PROC_CTX_BIN0;
	else
		bin = IPA_HDR_PROC_CTX_BIN1;

	mem_size = (ipa3_ctx->hdr_proc_ctx_tbl_lcl) ?
		IPA_MEM_PART(apps_hdr_proc_ctx_size) :
		IPA_MEM_PART(apps_hdr_proc_ctx_size_ddr);
	if (list_empty(&htbl->head_free_offset_list[bin])) {
		if (htbl->end + ipa_hdr_proc_ctx_bin_sz[bin] > mem_size) {
			IPAERR_RL("hdr proc ctx table overflow\n");
			goto bad_len;
		}

		offset = kmem_cache_zalloc(ipa3_ctx->hdr_proc_ctx_offset_cache,
					   GFP_KERNEL);
		if (!offset) {
			IPAERR("failed to alloc offset object\n");
			goto bad_len;
		}
		INIT_LIST_HEAD(&offset->link);
		/*
		 * for a first item grow, set the bin and offset which are set
		 * in stone
		 */
		offset->offset = htbl->end;
		offset->bin = bin;
		offset->ipacm_installed = user_only;
		htbl->end += ipa_hdr_proc_ctx_bin_sz[bin];
		list_add(&offset->link,
				&htbl->head_offset_list[bin]);
	} else {
		/* get the first free slot */
		offset =
		    list_first_entry(&htbl->head_free_offset_list[bin],
				struct ipa3_hdr_proc_ctx_offset_entry, link);
		offset->ipacm_installed = user_only;
		list_move(&offset->link, &htbl->head_offset_list[bin]);
	}

	entry->offset_entry = offset;
	list_add(&entry->link, &htbl->head_proc_ctx_entry_list);
	htbl->proc_ctx_cnt++;
	IPADBG("add proc ctx of sz=%d cnt=%d ofst=%d\n", needed_len,
			htbl->proc_ctx_cnt, offset->offset);

	id = ipa3_id_alloc(entry);
	if (id < 0) {
		IPAERR_RL("failed to alloc id\n");
		WARN_ON_RATELIMIT_IPA(1);
		goto ipa_insert_failed;
	}
	entry->id = id;
	proc_ctx->proc_ctx_hdl = id;
	entry->ref_cnt++;

	return 0;

ipa_insert_failed:
	list_move(&offset->link,
		&htbl->head_free_offset_list[offset->bin]);
	entry->offset_entry = NULL;
	list_del(&entry->link);
	htbl->proc_ctx_cnt--;

bad_len:
	if (add_ref_hdr)
		hdr_entry->ref_cnt--;
	entry->cookie = 0;
	kmem_cache_free(ipa3_ctx->hdr_proc_ctx_cache, entry);
	return -EPERM;
}

static int __ipa_add_rtp_hdr_proc_ctx(struct ipa_hdr_proc_ctx_add *proc_ctx,
	struct ipa_rtp_hdr_proc_ctx_params rtp_params, bool add_ref_hdr, bool user_only)
{
	struct ipa3_hdr_entry *hdr_entry;
	struct ipa3_hdr_proc_ctx_entry *entry;
	struct ipa3_hdr_proc_ctx_offset_entry *offset;
	u32 bin;
	struct ipa3_hdr_proc_ctx_tbl *htbl = &ipa3_ctx->hdr_proc_ctx_tbl;
	int id;
	int needed_len;
	int mem_size;

	IPADBG_LOW("Add processing type %d hdr_hdl %d\n",
		proc_ctx->type, proc_ctx->hdr_hdl);

	if (!HDR_PROC_TYPE_IS_VALID(proc_ctx->type)) {
		IPAERR_RL("invalid processing type %d\n", proc_ctx->type);
		return -EINVAL;
	}

	hdr_entry = ipa3_id_find(proc_ctx->hdr_hdl);
	if (!hdr_entry) {
		IPAERR_RL("hdr_hdl is invalid\n");
		return -EINVAL;
	}
	if (hdr_entry->cookie != IPA_HDR_COOKIE) {
		IPAERR_RL("Invalid header cookie %u\n", hdr_entry->cookie);
		WARN_ON_RATELIMIT_IPA(1);
		return -EINVAL;
	}
	IPADBG("Associated header is name=%s\n", hdr_entry->name);

	entry = kmem_cache_zalloc(ipa3_ctx->hdr_proc_ctx_cache, GFP_KERNEL);
	if (!entry) {
		IPAERR("failed to alloc proc_ctx object\n");
		return -ENOMEM;
	}

	INIT_LIST_HEAD(&entry->link);

	entry->type = proc_ctx->type;
	entry->hdr = hdr_entry;
	entry->rtp_params = rtp_params;
	if (add_ref_hdr)
		hdr_entry->ref_cnt++;
	entry->cookie = IPA_PROC_HDR_COOKIE;
	entry->ipacm_installed = user_only;

	needed_len = ipahal_get_proc_ctx_needed_len(proc_ctx->type);
	if ((needed_len < 0) ||
		((needed_len > ipa_hdr_proc_ctx_bin_sz[IPA_HDR_PROC_CTX_BIN0])
			&&
			(needed_len >
			ipa_hdr_proc_ctx_bin_sz[IPA_HDR_PROC_CTX_BIN1]))) {
		IPAERR_RL("unexpected needed len %d\n", needed_len);
		WARN_ON_RATELIMIT_IPA(1);
		goto bad_len;
	}

	if (needed_len <= ipa_hdr_proc_ctx_bin_sz[IPA_HDR_PROC_CTX_BIN0])
		bin = IPA_HDR_PROC_CTX_BIN0;
	else
		bin = IPA_HDR_PROC_CTX_BIN1;

	mem_size = (ipa3_ctx->hdr_proc_ctx_tbl_lcl) ?
		IPA_MEM_PART(apps_hdr_proc_ctx_size) :
		IPA_MEM_PART(apps_hdr_proc_ctx_size_ddr);
	if (list_empty(&htbl->head_free_offset_list[bin])) {
		if (htbl->end + ipa_hdr_proc_ctx_bin_sz[bin] > mem_size) {
			IPAERR_RL("hdr proc ctx table overflow\n");
			goto bad_len;
		}

		offset = kmem_cache_zalloc(ipa3_ctx->hdr_proc_ctx_offset_cache,
					   GFP_KERNEL);
		if (!offset) {
			IPAERR("failed to alloc offset object\n");
			goto bad_len;
		}
		INIT_LIST_HEAD(&offset->link);
		/*
		 * for a first item grow, set the bin and offset which are set
		 * in stone
		 */
		offset->offset = htbl->end;
		offset->bin = bin;
		offset->ipacm_installed = user_only;
		htbl->end += ipa_hdr_proc_ctx_bin_sz[bin];
		list_add(&offset->link,
				&htbl->head_offset_list[bin]);
	} else {
		/* get the first free slot */
		offset =
		    list_first_entry(&htbl->head_free_offset_list[bin],
				struct ipa3_hdr_proc_ctx_offset_entry, link);
		offset->ipacm_installed = user_only;
		list_move(&offset->link, &htbl->head_offset_list[bin]);
	}

	entry->offset_entry = offset;
	list_add(&entry->link, &htbl->head_proc_ctx_entry_list);
	htbl->proc_ctx_cnt++;
	IPADBG("add proc ctx of sz=%d cnt=%d ofst=%d\n", needed_len,
			htbl->proc_ctx_cnt, offset->offset);

	id = ipa3_id_alloc(entry);
	if (id < 0) {
		IPAERR_RL("failed to alloc id\n");
		WARN_ON_RATELIMIT_IPA(1);
		goto ipa_insert_failed;
	}
	entry->id = id;
	proc_ctx->proc_ctx_hdl = id;
	entry->ref_cnt++;

	return 0;

ipa_insert_failed:
	list_move(&offset->link,
		&htbl->head_free_offset_list[offset->bin]);
	entry->offset_entry = NULL;
	list_del(&entry->link);
	htbl->proc_ctx_cnt--;

bad_len:
	if (add_ref_hdr)
		hdr_entry->ref_cnt--;
	entry->cookie = 0;
	kmem_cache_free(ipa3_ctx->hdr_proc_ctx_cache, entry);
	return -EPERM;
}

static int __ipa_add_hdr(struct ipa_hdr_add *hdr, bool user,
	struct ipa3_hdr_entry **entry_out)
{
	struct ipa3_hdr_entry *entry, *entry_t, *next;
	struct ipa_hdr_offset_entry *offset = NULL;
	u32 bin;
	struct ipa3_hdr_tbl *htbl;
	int id;
	int mem_size;
	enum hdr_tbl_storage hdr_tbl_loc;

	if (hdr->hdr_len > IPA_HDR_MAX_SIZE) {
		IPAERR_RL("bad param\n");
		goto error;
	}

	if (!HDR_TYPE_IS_VALID(hdr->type)) {
		IPAERR_RL("invalid hdr type %d\n", hdr->type);
		goto error;
	}

	entry = kmem_cache_zalloc(ipa3_ctx->hdr_cache, GFP_KERNEL);
	if (!entry)
		goto error;

	INIT_LIST_HEAD(&entry->link);

	memcpy(entry->hdr, hdr->hdr, hdr->hdr_len);
	entry->hdr_len = hdr->hdr_len;
	strscpy(entry->name, hdr->name, IPA_RESOURCE_NAME_MAX);
	entry->is_partial = hdr->is_partial;
	entry->type = hdr->type;
	entry->is_eth2_ofst_valid = hdr->is_eth2_ofst_valid;
	entry->eth2_ofst = hdr->eth2_ofst;
	entry->cookie = IPA_HDR_COOKIE;
	entry->ipacm_installed = user;
	entry->is_lcl = ((IPA_MEM_PART(apps_hdr_size_ddr) &&
			 (entry->is_partial || (hdr->status == IPA_HDR_TO_DDR_PATTERN))) ||
			 !IPA_MEM_PART(apps_hdr_size)) ? false : true;

	/* check to see if adding header entry with duplicate name */
	for (hdr_tbl_loc = HDR_TBL_LCL; hdr_tbl_loc < HDR_TBLS_TOTAL; hdr_tbl_loc++) {
		list_for_each_entry_safe(entry_t, next,
			&ipa3_ctx->hdr_tbl[hdr_tbl_loc].head_hdr_entry_list, link) {

			/* return if adding the same name */
			if (!strcmp(entry_t->name, entry->name) && (user == true)) {
				IPAERR_RL("IPACM Trying to add hdr %s len=%d, duplicate entry, return old one\n",
					entry->name, entry->hdr_len);

				/* return the original entry */
				if (entry_out)
					*entry_out = entry_t;

				kmem_cache_free(ipa3_ctx->hdr_cache, entry);
				return 0;
			}
		}
	}

	if (hdr->hdr_len <= ipa_hdr_bin_sz[IPA_HDR_BIN0])
		bin = IPA_HDR_BIN0;
	else if (hdr->hdr_len <= ipa_hdr_bin_sz[IPA_HDR_BIN1])
		bin = IPA_HDR_BIN1;
	else if (hdr->hdr_len <= ipa_hdr_bin_sz[IPA_HDR_BIN2])
		bin = IPA_HDR_BIN2;
	else if (hdr->hdr_len <= ipa_hdr_bin_sz[IPA_HDR_BIN3])
		bin = IPA_HDR_BIN3;
	else if (hdr->hdr_len <= ipa_hdr_bin_sz[IPA_HDR_BIN4])
		bin = IPA_HDR_BIN4;
	/* Starting from IPA4.5, HW supports larger headers. */
	else if ((hdr->hdr_len <= ipa_hdr_bin_sz[IPA_HDR_BIN5]) &&
		 (ipa3_ctx->ipa_hw_type >= IPA_HW_v4_5))
		 bin = IPA_HDR_BIN5;
	else {
		IPAERR_RL("unexpected hdr len %d\n", hdr->hdr_len);
		goto bad_hdr_len;
	}

	htbl = entry->is_lcl ? &ipa3_ctx->hdr_tbl[HDR_TBL_LCL] : &ipa3_ctx->hdr_tbl[HDR_TBL_SYS];
	mem_size = entry->is_lcl ? IPA_MEM_PART(apps_hdr_size) : IPA_MEM_PART(apps_hdr_size_ddr);

	if (list_empty(&htbl->head_free_offset_list[bin])) {
		/*
		 * In case of a local header entry,
		 * first iteration will check against SRAM partition space,
		 * and the second iteration will check against DDR partition space.
		 * In case of a system header entry, the loop will iterate only once,
		 * and check against DDR partition space.
		 */
		while (htbl->end + ipa_hdr_bin_sz[bin] > mem_size) {
			if (entry->is_lcl) {
				/* if header does not fit to SRAM table, place it in DDR */
				IPADBG_LOW("SRAM header table was full allocting DDR header table! Requested: %d Left: %d name %s, end %d\n",
						ipa_hdr_bin_sz[bin], mem_size - htbl->end, entry->name, htbl->end);
				htbl = &ipa3_ctx->hdr_tbl[HDR_TBL_SYS];
				mem_size = IPA_MEM_PART(apps_hdr_size_ddr);
				entry->is_lcl = false;
			}

			/* check if DDR free list */
			if (list_empty(&htbl->head_free_offset_list[bin])) {
				if (!entry->is_lcl && (htbl->end + ipa_hdr_bin_sz[bin] > mem_size)) {
					IPAERR("No space in DDR header buffer! Requested: %d Left: %d name %s, end %d\n",
							ipa_hdr_bin_sz[bin], mem_size - htbl->end, entry->name, htbl->end);
					goto bad_hdr_len;
				}

				IPADBG_LOW("No free offset in DDR allocating new offset Requested: %d Left: %d name %s, end %d\n",
						ipa_hdr_bin_sz[bin], mem_size - htbl->end, entry->name, htbl->end);
				goto create_entry;
			} else {
				/* get the first free slot */
				offset = list_first_entry(&htbl->head_free_offset_list[bin],
						struct ipa_hdr_offset_entry, link);
				list_move(&offset->link, &htbl->head_offset_list[bin]);
				entry->offset_entry = offset;
				offset->ipacm_installed = user;
				goto free_list;
			}
		}
create_entry:
		offset = kmem_cache_zalloc(ipa3_ctx->hdr_offset_cache,
					   GFP_KERNEL);
		if (!offset) {
			IPAERR("failed to alloc hdr offset object\n");
			goto bad_hdr_len;
		}
		INIT_LIST_HEAD(&offset->link);
		/*
		 * for a first item grow, set the bin and offset which
		 * are set in stone
		 */
		offset->offset = htbl->end;
		offset->bin = bin;
		htbl->end += ipa_hdr_bin_sz[bin];
		list_add(&offset->link,
				&htbl->head_offset_list[bin]);
		entry->offset_entry = offset;
		offset->ipacm_installed = user;
	} else {
		/* get the first free slot */
		offset = list_first_entry(&htbl->head_free_offset_list[bin],
			struct ipa_hdr_offset_entry, link);
		list_move(&offset->link, &htbl->head_offset_list[bin]);
		entry->offset_entry = offset;
		offset->ipacm_installed = user;
	}

free_list:

	list_add(&entry->link, &htbl->head_hdr_entry_list);
	htbl->hdr_cnt++;
	IPADBG("add hdr of sz=%d hdr_cnt=%d ofst=%d to %s table\n",
			hdr->hdr_len,
			htbl->hdr_cnt,
			entry->offset_entry->offset,
			entry->is_lcl ? "SRAM" : "DDR");

	id = ipa3_id_alloc(entry);
	if (id < 0) {
		IPAERR_RL("failed to alloc id\n");
		WARN_ON_RATELIMIT_IPA(1);
		goto ipa_insert_failed;
	}
	entry->id = id;
	hdr->hdr_hdl = id;
	entry->ref_cnt++;
	if (entry_out)
		*entry_out = entry;

	return 0;

ipa_insert_failed:
	if (offset)
		list_move(&offset->link,
			  &htbl->head_free_offset_list[offset->bin]);
	entry->offset_entry = NULL;
	htbl->hdr_cnt--;
	list_del(&entry->link);

bad_hdr_len:
	entry->cookie = 0;
	kmem_cache_free(ipa3_ctx->hdr_cache, entry);
error:
	return -EPERM;
}

static int __ipa3_del_hdr_proc_ctx(u32 proc_ctx_hdl,
	bool release_hdr, bool by_user)
{
	struct ipa3_hdr_proc_ctx_entry *entry;
	struct ipa3_hdr_proc_ctx_tbl *htbl = &ipa3_ctx->hdr_proc_ctx_tbl;

	entry = ipa3_id_find(proc_ctx_hdl);
	if (!entry || (entry->cookie != IPA_PROC_HDR_COOKIE)) {
		IPAERR_RL("bad param\n");
		return -EINVAL;
	}

	IPADBG("del proc ctx cnt=%d ofst=%d\n",
		htbl->proc_ctx_cnt, entry->offset_entry->offset);

	if (by_user && entry->user_deleted) {
		IPAERR_RL("proc_ctx already deleted by user\n");
		return -EINVAL;
	}

	if (by_user)
		entry->user_deleted = true;

	if (--entry->ref_cnt) {
		IPADBG("proc_ctx_hdl %x ref_cnt %d\n",
			proc_ctx_hdl, entry->ref_cnt);
		return 0;
	}

	if (release_hdr)
		__ipa3_del_hdr(entry->hdr->id, false);

	/* move the offset entry to appropriate free list */
	list_move(&entry->offset_entry->link,
		&htbl->head_free_offset_list[entry->offset_entry->bin]);
	list_del(&entry->link);
	htbl->proc_ctx_cnt--;
	entry->cookie = 0;
	kmem_cache_free(ipa3_ctx->hdr_proc_ctx_cache, entry);

	/* remove the handle from the database */
	ipa3_id_remove(proc_ctx_hdl);

	return 0;
}

static int __ipa_add_hpc_hdr_insertion(struct ipa_hdr_add *hdr, bool user)
{
	struct ipa3_hdr_entry *entry = NULL;
	struct ipa_hdr_proc_ctx_add proc_ctx;

	hdr->status = IPA_HDR_TO_DDR_PATTERN;

	if (__ipa_add_hdr(hdr, user, &entry))
		goto error;

	IPADBG("adding processing context for header %s\n", hdr->name);
	proc_ctx.type = IPA_HDR_PROC_NONE;
	proc_ctx.hdr_hdl = hdr->hdr_hdl;
	if (__ipa_add_hdr_proc_ctx(&proc_ctx, true, user)) {
		IPAERR("failed to add hdr proc ctx\n");
		goto fail_add_proc_ctx;
	}
	entry->proc_ctx = (struct ipa3_hdr_proc_ctx_entry *)ipa3_id_find(proc_ctx.proc_ctx_hdl);
	WARN_ON_RATELIMIT_IPA(!entry->proc_ctx);
	entry->proc_ctx->ref_cnt++;

	return 0;

fail_add_proc_ctx:
	__ipa3_del_hdr(hdr->hdr_hdl, user);
error:
	return -EPERM;
}

int __ipa3_del_hdr(u32 hdr_hdl, bool by_user)
{
	struct ipa3_hdr_entry *entry;
	struct ipa3_hdr_tbl *htbl;

	entry = ipa3_id_find(hdr_hdl);
	if (entry == NULL) {
		IPAERR_RL("lookup failed\n");
		return -EINVAL;
	}

	if (entry->cookie != IPA_HDR_COOKIE) {
		IPAERR_RL("bad parm\n");
		return -EINVAL;
	}

	htbl = entry->is_lcl ? &ipa3_ctx->hdr_tbl[HDR_TBL_LCL] : &ipa3_ctx->hdr_tbl[HDR_TBL_SYS];

	IPADBG("del hdr of len=%d hdr_cnt=%d ofst=%d\n", entry->hdr_len, htbl->hdr_cnt,
		entry->offset_entry->offset);

	if (by_user && entry->user_deleted) {
		IPAERR_RL("proc_ctx already deleted by user\n");
		return -EINVAL;
	}

	if (by_user) {
		if (!strcmp(entry->name, IPA_LAN_RX_HDR_NAME)) {
			IPADBG("Trying to delete hdr %s offset=%u\n",
				entry->name, entry->offset_entry->offset);
			if (!entry->offset_entry->offset) {
				IPAERR_RL(
				"User cannot delete default header\n");
				return -EPERM;
			}
		}
		entry->user_deleted = true;
	}

	if (--entry->ref_cnt) {
		IPADBG("hdr_hdl %x ref_cnt %d\n", hdr_hdl, entry->ref_cnt);
		return 0;
	}

	if (entry->proc_ctx)
		__ipa3_del_hdr_proc_ctx(entry->proc_ctx->id, false, false);
	else
		/* move the offset entry to appropriate free list */
		list_move(&entry->offset_entry->link,
			&htbl->head_free_offset_list[entry->offset_entry->bin]);
	list_del(&entry->link);
	htbl->hdr_cnt--;
	entry->cookie = 0;
	kmem_cache_free(ipa3_ctx->hdr_cache, entry);

	/* remove the handle from the database */
	ipa3_id_remove(hdr_hdl);

	return 0;
}

/**
 * ipa3_add_hdr_hpc() - add the specified headers to SW and optionally commit them
 * to IPA HW
 * @hdrs:	[inout] set of headers to add
 *
 * Returns:	0 on success, negative on failure
 *
 * Note:	Should not be called from atomic context
 */
int ipa3_add_hdr_hpc(struct ipa_ioc_add_hdr *hdrs)
{
	return ipa3_add_hdr_hpc_usr(hdrs, false);
}
EXPORT_SYMBOL(ipa3_add_hdr_hpc);

/**
 * ipa3_add_hdr_hpc_usr() - add the specified headers to SW
 * and optionally commit them to IPA HW
 * @hdrs:		[inout] set of headers to add
 * @user_only:	[in] indicate installed from user
 *
 * Returns:	0 on success, negative on failure
 *
 * Note:	Should not be called from atomic context
 */
int ipa3_add_hdr_hpc_usr(struct ipa_ioc_add_hdr *hdrs, bool user_only)
{
	int i;
	int result = -EFAULT;

	if (hdrs == NULL || hdrs->num_hdrs == 0) {
		IPAERR_RL("bad parm\n");
		return -EINVAL;
	}

	mutex_lock(&ipa3_ctx->lock);
	IPADBG("adding %d headers to IPA driver internal data struct\n",
		hdrs->num_hdrs);
	for (i = 0; i < hdrs->num_hdrs; i++) {
		if (__ipa_add_hpc_hdr_insertion(&hdrs->hdr[i], user_only)) {
			IPAERR_RL("failed to add hdr hpc %d\n", i);
			hdrs->hdr[i].status = -1;
		}
		else {
			hdrs->hdr[i].status = 0;
		}
	}

	if (hdrs->commit) {
		IPADBG("committing all headers to IPA core");
		if (ipa3_ctx->ctrl->ipa3_commit_hdr()) {
			result = -EPERM;
			goto bail;
		}
	}
	result = 0;
bail:
	mutex_unlock(&ipa3_ctx->lock);
	return result;
}

/**
 * ipa3_del_hdr_hpc_usr() - Remove the specified headers from SW
 * and optionally commit them to IPA HW
 * @hdls:	[inout] set of headers to delete
 * @by_user:	Operation requested by user?
 *
 * Returns:	0 on success, negative on failure
 *
 * Note:	Should not be called from atomic context
 */
int ipa3_del_hdr_hpc_usr(struct ipa_ioc_del_hdr *hdls, bool by_user)
{
	int i;
	int result = 0;
	struct ipa3_hdr_entry *entry;
	struct ipa3_hdr_proc_ctx_entry *proc_ctx_entry;

	if (hdls == NULL || hdls->num_hdls == 0) {
		IPAERR_RL("bad parm\n");
		return -EINVAL;
	}

	mutex_lock(&ipa3_ctx->lock);
	for (i = 0; i < hdls->num_hdls; i++) {
		entry = (struct ipa3_hdr_entry *)ipa3_id_find(hdls->hdl[i].hdl);
		if (entry) {
			proc_ctx_entry = entry->proc_ctx;
			/* Header API changed under the hood --> need to NULL proc_ctx in header entry to
			   comply and avoid outdated code reach. need to be handled better in the future
			 */
			entry->proc_ctx = NULL;
			entry->ref_cnt--;
			result = __ipa3_del_hdr(hdls->hdl[i].hdl, by_user) != 0;
			if (proc_ctx_entry) {
				proc_ctx_entry->ref_cnt--;
				result = __ipa3_del_hdr_proc_ctx(proc_ctx_entry->id, false, false) != 0;
			}
		}
		hdls->hdl[i].status = result;
	}

	if (hdls->commit) {
		if (ipa3_ctx->ctrl->ipa3_commit_hdr()) {
			result = -EPERM;
			goto bail;
		}
	}
	result = 0;
bail:
	mutex_unlock(&ipa3_ctx->lock);
	return result;
}

/**
 * ipa3_del_hdr_hpc() - add the specified headers to SW and
 * optionally commit them to IPA HW
 * @hdrs:	[inout] set of headers to add
 *
 * Returns:	0 on success, negative on failure
 *
 * Note:	Should not be called from atomic context
 */
int ipa3_del_hdr_hpc(struct ipa_ioc_del_hdr *hdrs)
{
	return ipa3_del_hdr_hpc_usr(hdrs, false);
}
EXPORT_SYMBOL(ipa3_del_hdr_hpc);

/**
 * ipa_add_hdr() - add the specified headers to SW and optionally commit them
 * to IPA HW
 * @hdrs:	[inout] set of headers to add
 *
 * Returns:	0 on success, negative on failure
 *
 * Note:	Should not be called from atomic context
 */
int ipa_add_hdr(struct ipa_ioc_add_hdr *hdrs)
{
	return ipa3_add_hdr_usr(hdrs, false);
}
EXPORT_SYMBOL(ipa_add_hdr);

/**
 * ipa3_add_hdr_usr() - add the specified headers to SW
 * and optionally commit them to IPA HW
 * @hdrs:		[inout] set of headers to add
 * @user_only:	[in] indicate installed from user
 *
 * Returns:	0 on success, negative on failure
 *
 * Note:	Should not be called from atomic context
 */
int ipa3_add_hdr_usr(struct ipa_ioc_add_hdr *hdrs, bool user_only)
{
	int i;
	int result = -EFAULT;

	if (hdrs == NULL || hdrs->num_hdrs == 0) {
		IPAERR_RL("bad parm\n");
		return -EINVAL;
	}

	mutex_lock(&ipa3_ctx->lock);
	IPADBG("adding %d headers to IPA driver internal data struct\n",
			hdrs->num_hdrs);
	for (i = 0; i < hdrs->num_hdrs; i++) {
		if (__ipa_add_hdr(&hdrs->hdr[i], user_only, NULL)) {
			IPAERR_RL("failed to add hdr %d\n", i);
			hdrs->hdr[i].status = -1;
		} else {
			hdrs->hdr[i].status = 0;
		}
	}

	if (hdrs->commit) {
		IPADBG("committing all headers to IPA core");
		if (ipa3_ctx->ctrl->ipa3_commit_hdr()) {
			result = -EPERM;
			goto bail;
		}
	}
	result = 0;
bail:
	mutex_unlock(&ipa3_ctx->lock);
	return result;
}

/**
 * ipa3_del_hdr_by_user() - Remove the specified headers
 * from SW and optionally commit them to IPA HW
 * @hdls:	[inout] set of headers to delete
 * @by_user:	Operation requested by user?
 *
 * Returns:	0 on success, negative on failure
 *
 * Note:	Should not be called from atomic context
 */
int ipa3_del_hdr_by_user(struct ipa_ioc_del_hdr *hdls, bool by_user)
{
	int i;
	int result = -EFAULT;

	if (hdls == NULL || hdls->num_hdls == 0) {
		IPAERR_RL("bad parm\n");
		return -EINVAL;
	}

	mutex_lock(&ipa3_ctx->lock);
	for (i = 0; i < hdls->num_hdls; i++) {
		if (__ipa3_del_hdr(hdls->hdl[i].hdl, by_user)) {
			IPAERR_RL("failed to del hdr %i\n", i);
			hdls->hdl[i].status = -1;
		} else {
			hdls->hdl[i].status = 0;
		}
	}

	if (hdls->commit) {
		if (ipa3_ctx->ctrl->ipa3_commit_hdr()) {
			result = -EPERM;
			goto bail;
		}
	}
	result = 0;
bail:
	mutex_unlock(&ipa3_ctx->lock);
	return result;
}

/**
 * ipa_del_hdr() - Remove the specified headers from SW
 * and optionally commit them to IPA HW
 * @hdls:	[inout] set of headers to delete
 *
 * Returns:	0 on success, negative on failure
 *
 * Note:	Should not be called from atomic context
 */
int ipa_del_hdr(struct ipa_ioc_del_hdr *hdls)
{
	return ipa3_del_hdr_by_user(hdls, false);
}
EXPORT_SYMBOL(ipa_del_hdr);

/**
 * ipa3_add_hdr_proc_ctx() - add the specified headers to SW
 * and optionally commit them to IPA HW
 * @proc_ctxs:	[inout] set of processing context headers to add
 * @user_only:	[in] indicate installed by user-space module
 *
 * Returns:	0 on success, negative on failure
 *
 * Note:	Should not be called from atomic context
 */
int ipa3_add_hdr_proc_ctx(struct ipa_ioc_add_hdr_proc_ctx *proc_ctxs,
							bool user_only)
{
	int i;
	int result = -EFAULT;

	if (proc_ctxs == NULL || proc_ctxs->num_proc_ctxs == 0) {
		IPAERR_RL("bad parm\n");
		return -EINVAL;
	}

	mutex_lock(&ipa3_ctx->lock);
	IPADBG("adding %d header processing contextes to IPA driver\n",
			proc_ctxs->num_proc_ctxs);
	for (i = 0; i < proc_ctxs->num_proc_ctxs; i++) {
		if (__ipa_add_hdr_proc_ctx(&proc_ctxs->proc_ctx[i],
				true, user_only)) {
			IPAERR_RL("failed to add hdr proc ctx %d\n", i);
			proc_ctxs->proc_ctx[i].status = -1;
		} else {
			proc_ctxs->proc_ctx[i].status = 0;
		}
	}

	if (proc_ctxs->commit) {
		IPADBG("committing all headers to IPA core");
		if (ipa3_ctx->ctrl->ipa3_commit_hdr()) {
			result = -EPERM;
			goto bail;
		}
	}
	result = 0;
bail:
	mutex_unlock(&ipa3_ctx->lock);
	return result;
}

/**
 * ipa3_add_rtp_hdr_proc_ctx() - add the specified headers to SW
 * and optionally commit them to IPA HW
 * @proc_ctxs:	[inout] set of processing context headers to add
 * @rtp_params: [in] set of rtp_params to be configured
 * @user_only:	[in] indicate installed by user-space module
 *
 * Returns:	0 on success, negative on failure
 *
 * Note:	Should not be called from atomic context
 */
int ipa3_add_rtp_hdr_proc_ctx(struct ipa_ioc_add_hdr_proc_ctx *proc_ctxs,
	struct ipa_rtp_hdr_proc_ctx_params rtp_params, bool user_only)
{
	int i;
	int result = -EFAULT;

	if (proc_ctxs == NULL || proc_ctxs->num_proc_ctxs == 0) {
		IPAERR_RL("bad parm\n");
		return -EINVAL;
	}

	mutex_lock(&ipa3_ctx->lock);
	IPADBG("adding %d rtp header processing contexts to IPA driver\n",
			proc_ctxs->num_proc_ctxs);
	for (i = 0; i < proc_ctxs->num_proc_ctxs; i++) {
		if (__ipa_add_rtp_hdr_proc_ctx(&proc_ctxs->proc_ctx[i],
				rtp_params, true, user_only)) {
			IPAERR_RL("failed to add hdr proc ctx %d\n", i);
			proc_ctxs->proc_ctx[i].status = -1;
		} else {
			proc_ctxs->proc_ctx[i].status = 0;
		}
	}

	if (proc_ctxs->commit) {
		IPADBG("committing all headers to IPA core");
		if (ipa3_ctx->ctrl->ipa3_commit_hdr()) {
			result = -EPERM;
			goto bail;
		}
	}
	result = 0;
bail:
	mutex_unlock(&ipa3_ctx->lock);
	return result;
}

/**
 * ipa3_del_hdr_proc_ctx_by_user() -
 * Remove the specified processing context headers from SW and
 * optionally commit them to IPA HW.
 * @hdls:	[inout] set of processing context headers to delete
 * @by_user:	Operation requested by user?
 *
 * Returns:	0 on success, negative on failure
 *
 * Note:	Should not be called from atomic context
 */
int ipa3_del_hdr_proc_ctx_by_user(struct ipa_ioc_del_hdr_proc_ctx *hdls,
	bool by_user)
{
	int i;
	int result;

	if (hdls == NULL || hdls->num_hdls == 0) {
		IPAERR_RL("bad parm\n");
		return -EINVAL;
	}

	mutex_lock(&ipa3_ctx->lock);
	for (i = 0; i < hdls->num_hdls; i++) {
		if (__ipa3_del_hdr_proc_ctx(hdls->hdl[i].hdl, true, by_user)) {
			IPAERR_RL("failed to del hdr %i\n", i);
			hdls->hdl[i].status = -1;
		} else {
			hdls->hdl[i].status = 0;
		}
	}

	if (hdls->commit) {
		if (ipa3_ctx->ctrl->ipa3_commit_hdr()) {
			result = -EPERM;
			goto bail;
		}
	}
	result = 0;
bail:
	mutex_unlock(&ipa3_ctx->lock);
	return result;
}

/**
 * ipa3_del_hdr_proc_ctx() -
 * Remove the specified processing context headers from SW and
 * optionally commit them to IPA HW.
 * @hdls:	[inout] set of processing context headers to delete
 *
 * Returns:	0 on success, negative on failure
 *
 * Note:	Should not be called from atomic context
 */
int ipa3_del_hdr_proc_ctx(struct ipa_ioc_del_hdr_proc_ctx *hdls)
{
	return ipa3_del_hdr_proc_ctx_by_user(hdls, false);
}
EXPORT_SYMBOL(ipa3_del_hdr_proc_ctx);

/**
 * ipa3_commit_hdr() - commit to IPA HW the current header table in SW
 *
 * Returns:	0 on success, negative on failure
 *
 * Note:	Should not be called from atomic context
 */
int ipa3_commit_hdr(void)
{
	int result = -EFAULT;

	/*
	 * issue a commit on the routing module since routing rules point to
	 * header table entries
	 */
	if (ipa3_commit_rt(IPA_IP_v4))
		return -EPERM;
	if (ipa3_commit_rt(IPA_IP_v6))
		return -EPERM;

	mutex_lock(&ipa3_ctx->lock);
	if (ipa3_ctx->ctrl->ipa3_commit_hdr()) {
		result = -EPERM;
		goto bail;
	}
	result = 0;
bail:
	mutex_unlock(&ipa3_ctx->lock);
	return result;
}
EXPORT_SYMBOL(ipa3_commit_hdr);

/**
 * ipa3_reset_hdr() - reset the current header table in SW (does not commit to
 * HW)
 *
 * @user_only:	[in] indicate delete rules installed by userspace
 * Returns:	0 on success, negative on failure
 *
 * Note:	Should not be called from atomic context
 */
int ipa3_reset_hdr(bool user_only)
{
	struct ipa3_hdr_entry *entry;
	struct ipa3_hdr_entry *next;
	struct ipa3_hdr_proc_ctx_entry *ctx_entry;
	struct ipa3_hdr_proc_ctx_entry *ctx_next;
	struct ipa_hdr_offset_entry *off_entry;
	struct ipa_hdr_offset_entry *off_next;
	struct ipa3_hdr_proc_ctx_offset_entry *ctx_off_entry;
	struct ipa3_hdr_proc_ctx_offset_entry *ctx_off_next;
	struct ipa3_hdr_proc_ctx_tbl *htbl_proc = &ipa3_ctx->hdr_proc_ctx_tbl;
	enum hdr_tbl_storage hdr_tbl_loc;
	int i;

	/*
	 * issue a reset on the routing module since routing rules point to
	 * header table entries
	 */
	if (ipa3_reset_rt(IPA_IP_v4, user_only))
		IPAERR_RL("fail to reset v4 rt\n");
	if (ipa3_reset_rt(IPA_IP_v6, user_only))
		IPAERR_RL("fail to reset v6 rt\n");

	mutex_lock(&ipa3_ctx->lock);
	IPADBG("reset hdr\n");
	for (hdr_tbl_loc = HDR_TBL_LCL; hdr_tbl_loc < HDR_TBLS_TOTAL; hdr_tbl_loc++) {
		list_for_each_entry_safe(entry, next,
				&ipa3_ctx->hdr_tbl[hdr_tbl_loc].head_hdr_entry_list, link) {

			/* do not remove the default header */
			if (!strcmp(entry->name, IPA_LAN_RX_HDR_NAME)) {
				IPADBG("Trying to remove hdr %s offset=%u\n",
					entry->name, entry->offset_entry->offset);
				if (!entry->offset_entry->offset) {
					IPADBG("skip default header\n");
					continue;
				}
			}

			if (ipa3_id_find(entry->id) == NULL) {
				mutex_unlock(&ipa3_ctx->lock);
				IPAERR_RL("Invalid header ID\n");
				WARN_ON_RATELIMIT_IPA(1);
				return -EFAULT;
			}

			if (!user_only || entry->ipacm_installed) {
				if (entry->proc_ctx) {
					entry->proc_ctx->hdr = NULL;
					entry->proc_ctx = NULL;
				}
				/* move the offset entry to free list */
				entry->offset_entry->ipacm_installed = false;
				list_move(&entry->offset_entry->link,
				&ipa3_ctx->hdr_tbl[hdr_tbl_loc].head_free_offset_list[
					entry->offset_entry->bin]);

				/* delete the hdr entry from headers list */
				list_del(&entry->link);
				ipa3_ctx->hdr_tbl[hdr_tbl_loc].hdr_cnt--;
				entry->ref_cnt = 0;
				entry->cookie = 0;

				/* remove the handle from the database */
				ipa3_id_remove(entry->id);
				kmem_cache_free(ipa3_ctx->hdr_cache, entry);
			}
		}

		/* only clean up offset_list and free_offset_list on global reset */
		if (!user_only) {
			for (i = 0; i < IPA_HDR_BIN_MAX; i++) {
				list_for_each_entry_safe(off_entry, off_next,
						&ipa3_ctx->hdr_tbl[hdr_tbl_loc].head_offset_list[i],
						link) {
					/**
					 * do not remove the default exception
					 * header which is at offset 0
					 */
					if (off_entry->offset == 0)
						continue;
					list_del(&off_entry->link);
					kmem_cache_free(ipa3_ctx->hdr_offset_cache,
						off_entry);
				}
				list_for_each_entry_safe(off_entry, off_next,
					  &ipa3_ctx->hdr_tbl[hdr_tbl_loc].head_free_offset_list[i],
					  link) {
					list_del(&off_entry->link);
					kmem_cache_free(ipa3_ctx->hdr_offset_cache,
						off_entry);
				}
			}
			/* there is one header of size 8 */
			ipa3_ctx->hdr_tbl[hdr_tbl_loc].end = 8;
			ipa3_ctx->hdr_tbl[hdr_tbl_loc].hdr_cnt = 1;
		}
	}

	IPADBG("reset hdr proc ctx\n");
	list_for_each_entry_safe(
		ctx_entry,
		ctx_next,
		&(htbl_proc->head_proc_ctx_entry_list),
		link) {

		if (ipa3_id_find(ctx_entry->id) == NULL) {
			mutex_unlock(&ipa3_ctx->lock);
			IPAERR_RL("Invalid proc header ID\n");
			WARN_ON_RATELIMIT_IPA(1);
			return -EFAULT;
		}

		if (!user_only ||
				ctx_entry->ipacm_installed) {
			/* move the offset entry to appropriate free list */
			list_move(&ctx_entry->offset_entry->link,
				&htbl_proc->head_free_offset_list[
					ctx_entry->offset_entry->bin]);
			list_del(&ctx_entry->link);
			htbl_proc->proc_ctx_cnt--;
			ctx_entry->ref_cnt = 0;
			ctx_entry->cookie = 0;

			/* remove the handle from the database */
			ipa3_id_remove(ctx_entry->id);
			kmem_cache_free(ipa3_ctx->hdr_proc_ctx_cache,
				ctx_entry);
		}
	}
	/* only clean up offset_list and free_offset_list on global reset */
	if (!user_only) {
		for (i = 0; i < IPA_HDR_PROC_CTX_BIN_MAX; i++) {
			list_for_each_entry_safe(ctx_off_entry, ctx_off_next,
				&(htbl_proc->head_offset_list[i]), link) {
				list_del(&ctx_off_entry->link);
				kmem_cache_free(
					ipa3_ctx->hdr_proc_ctx_offset_cache,
					ctx_off_entry);
			}
			list_for_each_entry_safe(ctx_off_entry, ctx_off_next,
				&(htbl_proc->head_free_offset_list[i]), link) {
				list_del(&ctx_off_entry->link);
				kmem_cache_free(
					ipa3_ctx->hdr_proc_ctx_offset_cache,
					ctx_off_entry);
			}
		}
		htbl_proc->end = 0;
		htbl_proc->proc_ctx_cnt = 0;
	}

	/* commit the change to IPA-HW */
	if (ipa3_ctx->ctrl->ipa3_commit_hdr()) {
		IPAERR("fail to commit hdr\n");
		WARN_ON_RATELIMIT_IPA(1);
		mutex_unlock(&ipa3_ctx->lock);
		return -EFAULT;
	}

	mutex_unlock(&ipa3_ctx->lock);
	return 0;
}
EXPORT_SYMBOL(ipa3_reset_hdr);

static struct ipa3_hdr_entry *__ipa_find_hdr(const char *name)
{
	struct ipa3_hdr_entry *entry;
	enum hdr_tbl_storage hdr_tbl_loc;

	if (strnlen(name, IPA_RESOURCE_NAME_MAX) == IPA_RESOURCE_NAME_MAX) {
		IPAERR_RL("Header name too long: %s\n", name);
		return NULL;
	}
	for (hdr_tbl_loc = HDR_TBL_LCL; hdr_tbl_loc < HDR_TBLS_TOTAL; hdr_tbl_loc++) {
		list_for_each_entry(entry,
				    &ipa3_ctx->hdr_tbl[hdr_tbl_loc].head_hdr_entry_list,
				    link) {
			if (!strcmp(name, entry->name))
				return entry;
		}
	}

	return NULL;
}

static struct ipa3_hdr_proc_ctx_entry* __ipa_find_hdr_proc_ctx(const char *name)
{
	struct ipa3_hdr_entry *entry;

	entry = __ipa_find_hdr(name);
	if (entry && entry->proc_ctx)
		return entry->proc_ctx;

	return NULL;
}

/**
 * ipa_get_hdr() - Lookup the specified header resource
 * @lookup:	[inout] header to lookup and its handle
 *
 * lookup the specified header resource and return handle if it exists
 *
 * Returns:	0 on success, negative on failure
 *
 * Note:	Should not be called from atomic context
 *		Caller should call ipa3_put_hdr later if this function succeeds
 */
int ipa_get_hdr(struct ipa_ioc_get_hdr *lookup)
{
	struct ipa3_hdr_entry *entry;
	int result = -1;

	if (lookup == NULL) {
		IPAERR_RL("bad parm\n");
		return -EINVAL;
	}
	mutex_lock(&ipa3_ctx->lock);
	lookup->name[IPA_RESOURCE_NAME_MAX-1] = '\0';
	entry = __ipa_find_hdr(lookup->name);
	if (entry) {
		lookup->hdl = entry->id;
		result = 0;
	}
	mutex_unlock(&ipa3_ctx->lock);

	return result;
}
EXPORT_SYMBOL(ipa_get_hdr);

/**
 * ipa3_get_hdr_offset() - Get the the offset of the specified header resource
 * @name: [in] name of header to lookup
 * @offset: [out] offset of the specified header
 *
 * lookup the specified header resource and return its offset if exists
 *
 * Returns:	0 on success, negative on failure
 *
 * Note: Should not be called from atomic context
 */
int ipa3_get_hdr_offset(char* name, u32* offset)
{
	struct ipa3_hdr_entry *entry;
	int result = -1;
	if (!name || !offset) {
		IPAERR_RL("bad parm\n");
		return -EINVAL;
	}

	mutex_lock(&ipa3_ctx->lock);
	name[IPA_RESOURCE_NAME_MAX-1] = '\0';
	entry = __ipa_find_hdr(name);
	if (entry && entry->offset_entry) {
		*offset = entry->offset_entry->offset;
		result = 0;
	}

	mutex_unlock(&ipa3_ctx->lock);
	return result;
}

/**
 * ipa3_get_hdr_proc_ctx_hdl() - Lookup the specified hpc resource
 * @lookup:	[inout] hpc to lookup and its handle
 *
 * Lookup the specified hpc resource and return handle if it exists.
 * The hpc returned is identified by the hpc pointed by the hdr associated
 * with lookup->name.
 *
 * Returns:	0 on success, negative on failure
 *
 * Note:	Should not be called from atomic context
 */
int ipa3_get_hdr_proc_ctx_hdl(struct ipa_ioc_get_hdr *lookup)
{
	struct ipa3_hdr_proc_ctx_entry *entry;
	int result = -1;
	if (lookup == NULL) {
		IPAERR_RL("bad parm\n");
		return -EINVAL;
	}

	mutex_lock(&ipa3_ctx->lock);
	lookup->name[IPA_RESOURCE_NAME_MAX-1] = '\0';
	entry = __ipa_find_hdr_proc_ctx(lookup->name);
	if (entry) {
		lookup->hdl = entry->id;
		result = 0;
	}

	mutex_unlock(&ipa3_ctx->lock);
	return result;
}

/**
 * ipa3_get_hdr_proc_ctx_offset() - Lookup the specified hpc resource
 * @name:	[in] hpc name to lookup
 * @offset:	[out] offset to of hpc to return
 *
 * Lookup the specified hpc resource and return offset if it exists.
 * The hpc offset returned is of the hpc identified by the hpc pointed
 * by the hdr associated with name. The offset returned is in 32B
 * and includes the hpc table start offset.
 *
 * Returns:	0 on success, negative on failure
 *
 * Note:	Should not be called from atomic context
 */
int ipa3_get_hdr_proc_ctx_offset(char* name, u32* offset)
{
	struct ipa3_hdr_proc_ctx_entry *entry;
	int result = -1;

	if (!name || !offset) {
		IPAERR_RL("bad parm\n");
		return -EINVAL;
	}

	mutex_lock(&ipa3_ctx->lock);
	name[IPA_RESOURCE_NAME_MAX-1] = '\0';
	entry = __ipa_find_hdr_proc_ctx(name);
	if (entry && entry->offset_entry) {
		/* offset is in 32 Bytes chunks */
		*offset = (entry->offset_entry->offset +
		ipa3_ctx->hdr_proc_ctx_tbl.start_offset) >> 5;
		result = 0;
	}

	mutex_unlock(&ipa3_ctx->lock);
	return result;
}

/**
 * __ipa3_release_hdr() - drop reference to header and cause
 * deletion if reference count permits
 * @hdr_hdl:	[in] handle of header to be released
 *
 * Returns:	0 on success, negative on failure
 */
int __ipa3_release_hdr(u32 hdr_hdl)
{
	int result = 0;

	if (__ipa3_del_hdr(hdr_hdl, false)) {
		IPADBG("fail to del hdr %x\n", hdr_hdl);
		result = -EFAULT;
		goto bail;
	}

	/* commit for put */
	if (ipa3_ctx->ctrl->ipa3_commit_hdr()) {
		IPAERR("fail to commit hdr\n");
		result = -EFAULT;
		goto bail;
	}

bail:
	return result;
}

/**
 * __ipa3_release_hdr_proc_ctx() - drop reference to processing context
 *  and cause deletion if reference count permits
 * @proc_ctx_hdl:	[in] handle of processing context to be released
 *
 * Returns:	0 on success, negative on failure
 */
int __ipa3_release_hdr_proc_ctx(u32 proc_ctx_hdl)
{
	int result = 0;

	if (__ipa3_del_hdr_proc_ctx(proc_ctx_hdl, true, false)) {
		IPADBG("fail to del hdr %x\n", proc_ctx_hdl);
		result = -EFAULT;
		goto bail;
	}

	/* commit for put */
	if (ipa3_ctx->ctrl->ipa3_commit_hdr()) {
		IPAERR("fail to commit hdr\n");
		result = -EFAULT;
		goto bail;
	}

bail:
	return result;
}

/**
 * ipa3_put_hdr() - Release the specified header handle
 * @hdr_hdl:	[in] the header handle to release
 *
 * Returns:	0 on success, negative on failure
 *
 * Note:	Should not be called from atomic context
 */
int ipa3_put_hdr(u32 hdr_hdl)
{
	struct ipa3_hdr_entry *entry;
	int result = -EFAULT;

	mutex_lock(&ipa3_ctx->lock);

	entry = ipa3_id_find(hdr_hdl);
	if (entry == NULL) {
		IPAERR_RL("lookup failed\n");
		result = -EINVAL;
		goto bail;
	}

	if (entry->cookie != IPA_HDR_COOKIE) {
		IPAERR_RL("invalid header entry\n");
		result = -EINVAL;
		goto bail;
	}

	result = 0;
bail:
	mutex_unlock(&ipa3_ctx->lock);
	return result;
}

/**
 * ipa3_copy_hdr() - Lookup the specified header resource and return a copy of
 * it
 * @copy:	[inout] header to lookup and its copy
 *
 * lookup the specified header resource and return a copy of it (along with its
 * attributes) if it exists, this would be called for partial headers
 *
 * Returns:	0 on success, negative on failure
 *
 * Note:	Should not be called from atomic context
 */
int ipa3_copy_hdr(struct ipa_ioc_copy_hdr *copy)
{
	struct ipa3_hdr_entry *entry;
	int result = -EFAULT;

	if (copy == NULL) {
		IPAERR_RL("bad parm\n");
		return -EINVAL;
	}
	mutex_lock(&ipa3_ctx->lock);
	copy->name[IPA_RESOURCE_NAME_MAX-1] = '\0';
	entry = __ipa_find_hdr(copy->name);
	if (entry) {
		memcpy(copy->hdr, entry->hdr, entry->hdr_len);
		copy->hdr_len = entry->hdr_len;
		copy->type = entry->type;
		copy->is_partial = entry->is_partial;
		copy->is_eth2_ofst_valid = entry->is_eth2_ofst_valid;
		copy->eth2_ofst = entry->eth2_ofst;
		result = 0;
	}
	mutex_unlock(&ipa3_ctx->lock);

	return result;
}

/**
 * ipa3_get_hdr_bin_size() - Get header bin size from specified index
 *
 * @index:	[in] index in the bin sizes array
 *
 * Returns:	bin size on success, MAX_UINT32 on failure
 */
u32 ipa3_get_hdr_bin_size(int index)
{
	if (index < 0 || index >= IPA_HDR_BIN_MAX)
		return U32_MAX;
	return ipa_hdr_bin_sz[index];
}
