/*
 * netlink interface
 *
 * Copyright (c) 2017 Goodix
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/timer.h>
#include <linux/time.h>
#include <linux/types.h>
#include <net/sock.h>
#include <net/netlink.h>
#include "gf_spi.h"

#define NETLINK_TEST 25
#define MAX_MSGSIZE 32

static int pid = -1;
static struct sock *nl_sk;


/*
*    Only GF_NETLINK_UI_DISAPPEAR GF_NETLINK_UI_READY GF_NETLINK_FINGER_DOWN GF_NETLINK_FINGER_UP
*    event have extra parameters. other events only have event(data[0]).
*    data[0]:event value,
*    if event == GF_NETLINK_UI_DISAPPEAR or GF_NETLINK_UI_READY, data[1]:HBM status, data[1]:ui status
*    if event ==	GF_NETLINK_FINGER_DOWN	GF_NETLINK_FINGER_UP, data[1]:x coordinate; data[2]:y coordinate.
*/
int sendnlmsg(const int command, void *data)
{
	struct sk_buff *skb;
	struct nlmsghdr *nlh;
	int len = NLMSG_SPACE(MAX_MSGSIZE);
	int ret = 0;
	char *msg = NULL;
	struct fp_notify_event *fp_event = NULL;
	struct touchpanel_coordinate *tp_coordinate = NULL;

	if (!nl_sk || !pid)
		return -ENODEV;

	switch (command)
	{
		case GF_NETLINK_SCREEN_ON:
		case GF_NETLINK_SCREEN_OFF:
		case GF_NETLINK_SCREEN_AOD:
		case GF_NETLINK_IRQ:
			len = MAX_MSGSIZE;
			break;
		case GF_NETLINK_UI_DISAPPEAR:
		case GF_NETLINK_UI_READY:
			len = sizeof(struct fp_notify_event) + MAX_MSGSIZE;
			fp_event = (struct fp_notify_event *)data;
			break;
		case GF_NETLINK_FINGER_DOWN:
		case GF_NETLINK_FINGER_UP:
			len = sizeof(struct touchpanel_coordinate) + MAX_MSGSIZE;
			tp_coordinate = (struct touchpanel_coordinate *)data;
			break;
	}

	skb = alloc_skb(len, GFP_ATOMIC);
	if (!skb)
		return -ENOMEM;

	nlh = nlmsg_put(skb, 0, 0, 0, MAX_MSGSIZE, 0);
	if (!nlh) {
		kfree_skb(skb);
		return -EMSGSIZE;
	}

	NETLINK_CB(skb).portid = 0;
	NETLINK_CB(skb).dst_group = 0;

	msg = (char *)NLMSG_DATA(nlh);
	memcpy(msg, (char *)&command, sizeof(char));

	 if (((GF_NETLINK_FINGER_DOWN == command) || (GF_NETLINK_FINGER_UP == command)) && (NULL != data)) {
		memcpy(msg + 1, (char *)tp_coordinate, sizeof(struct touchpanel_coordinate));
	} else if (((GF_NETLINK_UI_DISAPPEAR == command) || (GF_NETLINK_UI_READY == command)) && (NULL != data)) {
		memcpy(msg + 1, (char *)fp_event, sizeof(struct fp_notify_event));
	}

	pr_info("send message: %d\n", *(char *)NLMSG_DATA(nlh));

	ret = netlink_unicast(nl_sk, skb, pid, MSG_DONTWAIT);
	if (ret > 0)
		ret = 0;

	return ret;
}

static void nl_data_ready(struct sk_buff *__skb)
{
	struct sk_buff *skb;
	struct nlmsghdr *nlh;
	char str[100];

	skb = skb_get(__skb);
	if (skb->len >= NLMSG_SPACE(0)) {
		nlh = nlmsg_hdr(skb);

		memcpy(str, NLMSG_DATA(nlh), sizeof(str));
		pid = nlh->nlmsg_pid;

		kfree_skb(skb);
	}

}


int netlink_init(void)
{
	struct netlink_kernel_cfg netlink_cfg;

	memset(&netlink_cfg, 0, sizeof(struct netlink_kernel_cfg));

	netlink_cfg.groups = 0;
	netlink_cfg.flags = 0;
	netlink_cfg.input = nl_data_ready;
	netlink_cfg.cb_mutex = NULL;

	nl_sk = netlink_kernel_create(&init_net, NETLINK_TEST,
			&netlink_cfg);

	if (!nl_sk) {
		pr_err("create netlink socket error\n");
		return 1;
	}

	return 0;
}

void netlink_exit(void)
{
	if (nl_sk != NULL) {
		netlink_kernel_release(nl_sk);
		nl_sk = NULL;
	}

	pr_info("self module exited\n");
}
