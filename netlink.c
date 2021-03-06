/*
 * From:
 *    https://lkml.org/lkml/2005/9/28/347
 *
 * Original filename:
 *    test_cn_proc.c
 *
 * See also:
 *    /usr/include/linux/netlink.h
 *    /usr/include/linux/connector.h
 *    /usr/include/linux/cn_proc.h
 *    /usr/include/linux/rtnetlink.h
 */

/*
 * test process event connector - test_cn_proc.c
 *
 * Listens for process events (fork, exec, change uid/gid/..., and exit)
 * received through a kernel connector and prints them.
 *
 * Copyright (C) Matt Helsley, IBM Corp. 2005
 * Derived from fcctl.c by Guillaume Thouvenin
 * Original copyright notice follows:
 *
 * Copyright (C) 2005 BULL SA.
 * Written by Guillaume Thouvenin <guillaume.thouvenin@bull.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <linux/connector.h>
#include <linux/netlink.h>
#include "linux/cn_proc.h"

#include <signal.h>
#include <setjmp.h>

#define SEND_MESSAGE_LEN (NLMSG_LENGTH(sizeof(struct cn_msg) + \
				       sizeof(enum proc_cn_mcast_op)))
#define RECV_MESSAGE_LEN (NLMSG_LENGTH(sizeof(struct cn_msg) + \
				       sizeof(struct proc_event)))

#define SEND_MESSAGE_SIZE    (NLMSG_SPACE(SEND_MESSAGE_LEN))
#define RECV_MESSAGE_SIZE    (NLMSG_SPACE(RECV_MESSAGE_LEN))

#define max(x,y) ((y)<(x)?(x):(y))
#define min(x,y) ((y)>(x)?(x):(y))

#define BUFF_SIZE (max(max(SEND_MESSAGE_SIZE, RECV_MESSAGE_SIZE), 1024))
#define MIN_RECV_SIZE (min(SEND_MESSAGE_SIZE, RECV_MESSAGE_SIZE))

#define PROC_CN_MCAST_LISTEN (1)
#define PROC_CN_MCAST_IGNORE (2)

/*
 * SIGINT causes the program to exit gracefully
 * this could happen any time after the LISTEN message has
 * been sent
 */
#define INTR_SIG SIGINT
sigjmp_buf g_jmp;
void handle_intr (int signum)
{
	siglongjmp(g_jmp, signum);
}
void handle_msg (struct cn_msg *cn_hdr)
{
	struct proc_event *ev;
	/* print the message */
	printf("cn seq: %d\ncn ack: %d\n",
		cn_hdr->seq, cn_hdr->ack);
	ev = (struct proc_event*)cn_hdr->data;
	printf("ev cpu: %d\n", ev->cpu);
	switch(ev->what){
	case PROC_EVENT_FORK:
		printf("event: fork\n\tparent:\t%d %d\n\tchild: %d %d\n",
		       ev->event_data.fork.parent_pid,
		       ev->event_data.fork.parent_tgid,
		       ev->event_data.fork.child_pid,
		       ev->event_data.fork.child_tgid);
		break;
	case PROC_EVENT_EXEC:
		printf("event: exec\n\tprocess:\t%d %d\n",
		       ev->event_data.exec.process_pid,
		       ev->event_data.exec.process_tgid);
		break;
	case PROC_EVENT_UID:
		printf("event: uid\n\tprocess:\t%d %d\n\t     real: %d\n\teffective: %d\n",
		       ev->event_data.id.process_pid,
		       ev->event_data.id.process_tgid,
		       ev->event_data.id.r.ruid,
		       ev->event_data.id.e.euid);
		break;
	case PROC_EVENT_GID:
		printf("event: gid\n\tprocess:\t%d %d\n\t     real: %d\n\teffective: %d\n",
		       ev->event_data.id.process_pid,
		       ev->event_data.id.process_tgid,
		       ev->event_data.id.r.rgid,
		       ev->event_data.id.e.egid);
		break;
	case PROC_EVENT_EXIT:
		printf("event: exit\n\tprocess:\t%d %d\n\texit code:\t%d\n",
		       ev->event_data.exit.process_pid,
		       ev->event_data.exit.process_tgid,
		       ev->event_data.exit.exit_code);
		break;
	default:
		printf("event <unknown>\n");
		break;
	}
}
int main(int argc, char **argv)
{
	int sk_nl;
	int err;
	struct sockaddr_nl my_nla, kern_nla, from_nla;
	socklen_t from_nla_len;
	char buff[BUFF_SIZE];
	int rc = -1;
	struct nlmsghdr *nl_hdr;
	struct cn_msg *cn_hdr;
	enum proc_cn_mcast_op *mcop_msg;
	size_t recv_len = 0;
	if (getuid() != 0) {
		printf("Only root can start/stop the fork connector\n");
		return 0;
	}
	if (argc != 1)
		return 0;
	/*
	 * Create an endpoint for communication. Use the kernel user
	 * interface device (PF_NETLINK) which is a datagram oriented
	 * service (SOCK_DGRAM). The protocol used is the connector
	 * protocol (NETLINK_CONNECTOR)
	 */
	sk_nl = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_CONNECTOR);
	if (sk_nl == -1) {
		printf("socket sk_nl error");
		return rc;
	}
	my_nla.nl_family = AF_NETLINK;
	my_nla.nl_groups = CN_IDX_PROC;
	my_nla.nl_pid = getpid();

	kern_nla.nl_family = AF_NETLINK;
	kern_nla.nl_groups = CN_IDX_PROC;
	kern_nla.nl_pid = 1;

	err = bind(sk_nl, (struct sockaddr *)&my_nla, sizeof(my_nla));
	if (err == -1) {
		printf("binding sk_nl error");
		goto close_and_exit;
	}
	nl_hdr = (struct nlmsghdr *)buff;
	cn_hdr = (struct cn_msg *)NLMSG_DATA(nl_hdr);
	mcop_msg = (enum proc_cn_mcast_op*)&cn_hdr->data[0];
	if (sigsetjmp(g_jmp, INTR_SIG) != 0) {
		printf("sending proc connector: PROC_CN_MCAST_IGNORE... ");
		memset(buff, 0, sizeof(buff));
		*mcop_msg = PROC_CN_MCAST_IGNORE;
	} else {
		printf("sending proc connector: PROC_CN_MCAST_LISTEN... ");
		memset(buff, 0, sizeof(buff));
		*mcop_msg = PROC_CN_MCAST_LISTEN;
		signal(INTR_SIG, handle_intr);
	}
	/* fill the netlink header */
	nl_hdr->nlmsg_len = SEND_MESSAGE_LEN;
	nl_hdr->nlmsg_type = NLMSG_DONE;
	nl_hdr->nlmsg_flags = 0;
	nl_hdr->nlmsg_seq = 0;
	nl_hdr->nlmsg_pid = getpid();
	/* fill the connector header */
	cn_hdr->id.idx = CN_IDX_PROC;
	cn_hdr->id.val = CN_VAL_PROC;
	cn_hdr->seq = 0;
	cn_hdr->ack = 0;
	cn_hdr->len = sizeof(enum proc_cn_mcast_op);
	if (send(sk_nl, nl_hdr, nl_hdr->nlmsg_len, 0) != nl_hdr->nlmsg_len) {
		printf("failed to send proc connector mcast ctl op!\n");
		goto close_and_exit;
	}
	printf("sent\n");
	if (*mcop_msg == PROC_CN_MCAST_IGNORE) {
		rc = 0;
		goto close_and_exit;
	}
	printf("Reading process events from proc connector.\n"
		"Hit Ctrl-C to exit\n");
	for(memset(buff, 0, sizeof(buff)), from_nla_len = sizeof(from_nla);
	  ; memset(buff, 0, sizeof(buff)), from_nla_len = sizeof(from_nla)) {
		struct nlmsghdr *nlh = (struct nlmsghdr*)buff;
		memcpy(&from_nla, &kern_nla, sizeof(from_nla));
		recv_len = recvfrom(sk_nl, buff, BUFF_SIZE, 0,
				(struct sockaddr*)&from_nla, &from_nla_len);
		if (recv_len < 1)
			continue;
		while (NLMSG_OK(nlh, recv_len)) {
			cn_hdr = NLMSG_DATA(nlh);
			if (nlh->nlmsg_type == NLMSG_NOOP)
				continue;
			if ((nlh->nlmsg_type == NLMSG_ERROR) ||
			    (nlh->nlmsg_type == NLMSG_OVERRUN))
				break;
			handle_msg(cn_hdr);
			if (nlh->nlmsg_type == NLMSG_DONE)
				break;
			nlh = NLMSG_NEXT(nlh, recv_len);
		}
	}
close_and_exit:
	close(sk_nl);
	exit(rc);

	return 0;
}
