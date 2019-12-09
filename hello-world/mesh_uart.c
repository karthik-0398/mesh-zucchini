/*
 * mesh_uart.c
 *
 *  Created on: 2 Dec 2019
 *      Author: Prinzessin
 *
 * Copyright (C) 2018 Inria
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#include <stdio.h>
#include <string.h>

#include "thread.h"
#include "msg.h"
#include "xtimer.h"

#include "board.h"

#include "periph/uart.h"

// get ip address of Master
#include "net/ipv6/addr.h"
#include "net/gnrc.h"
#include "net/gnrc/netif.h"

// #include "stdio_uart.h" // new

void led_switch(char colour);
static int comms_speak_to_slave(char *data, int receiver);

kernel_pid_t pid;
static char stack[THREAD_STACKSIZE_MAIN];

#define MSG_QUEUE_SIZE 8
static msg_t msg_queue[MSG_QUEUE_SIZE];

#define MASTER 0
#define SLAVE1 1
#define SLAVE2 2
#define SLAVE3 3

/* needed for posix usleep */
#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 600
#endif

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include "thread.h"

#include "periph/uart.h"



#define MASTER 0
//static char udp_stack[THREAD_STACKSIZE_MAIN];

/*
 * data ... message sent by server
 * reply ... message sent to server
 * don't call static functions here!!
 */
static void uart_cb(void *arg, uint8_t data) {

	char reply[30] = "Master received the message";

	(void) arg;
	msg_t msg1;
	msg1.content.value = data;

	msg_send(&msg1, pid);
	xtimer_sleep(5);
	uart_write(UART_DEV(0), (uint8_t*) reply, sizeof(reply));

}

static int comms_speak_to_slave(char *data, int receiver) { // was static before

	struct sockaddr_in6 src, dst;
	size_t data_len = strlen(data);
	uint16_t port = 8808;
	int s;
	char *addr_str = malloc(sizeof(char) * 32);
	if (receiver == SLAVE1) {
		addr_str = "fe80::213b:5329:36b7:6e5e";
	} else if (receiver == SLAVE2) {
		addr_str = "fe80::2112:6321:622a:6d62";
	} else if (receiver == SLAVE3) {
		addr_str = "fe80::213b:5329:36b7:6e5e"; // TODO adapt for each device
	} else {
		addr_str = "fe80::213b:5329:36b7:6e5e"; // default board 1
	}

	char *message = malloc(sizeof(char) * 32);

	src.sin6_family = AF_INET6;
	dst.sin6_family = AF_INET6;
	memset(&src.sin6_addr, 0, sizeof(src.sin6_addr));
		if (inet_pton(AF_INET6, addr_str, &dst.sin6_addr) != 1){
			message = "Unable to parse destination address";
			return 1;
		}

		dst.sin6_port = htons(port);
		src.sin6_port = htons(port);
		s = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
		if(s < 0){
			message = "Socket init failed";
			return 1;
		}
		unsigned int i;
		int j;
		for (i = 0; i < 3; i++){
			j = sendto(s, data, data_len, 0, (struct sockaddr *)&dst, sizeof(dst));
			if(j >= 0){ // < 0 is bad
				break;
			}
		}
		if(strlen(message) > 1){
			uart_write(UART_DEV(0), (uint8_t*) message, sizeof(message));
		}
		close(s);
	return 0;
}

/*
 * \brief
 *
 * msg.content.value ... message sent by server
 *
 */
static void* printer(void *arg) {
	(void) arg;

	msg_init_queue(msg_queue, MSG_QUEUE_SIZE);
	while (1) {
		msg_t msg;
		msg_receive(&msg);
		led_switch((char) msg.content.value);
		char * d = malloc(sizeof(char) * 30);
		sprintf(d,"%c", (int)msg.content.value);
		// Forward message from UART to SLAVE
		comms_speak_to_slave(d, SLAVE1);
		comms_speak_to_slave(d, SLAVE2);
		comms_speak_to_slave(d, SLAVE3);
	}
	return NULL;
}

// write thread for sending error messages and reply from uart_cb

/*
 * \brief Sending and receiving UART messages
 *
 * Start thread that uses the printer function
 * UART is initialized with a callback function
 * Baudrate: 115200
 */
char mesh_uart_init(void) {
	char colour = 'G';

	gnrc_netif_t *netif = NULL;
	while ((netif = gnrc_netif_iter(netif))) {
		ipv6_addr_t ipv6_addrs[GNRC_NETIF_IPV6_ADDRS_NUMOF];
		int res = gnrc_netapi_get(netif->pid, NETOPT_IPV6_ADDR, 0, ipv6_addrs,
								  sizeof(ipv6_addrs));
		if (res < 0) {
			continue;
		}
		for (unsigned i = 0; i < (unsigned)(res / sizeof(ipv6_addr_t)); i++) {
			char ipv6_addr[IPV6_ADDR_MAX_STR_LEN];

			ipv6_addr_to_str(ipv6_addr, &ipv6_addrs[i], IPV6_ADDR_MAX_STR_LEN);
			printf("Master IP Address: %s\n", ipv6_addr);
		}
	}

	pid = thread_create(stack, sizeof(stack),
	THREAD_PRIORITY_MAIN - 1, 0, printer,
	NULL, "thread");

	if (pid == -1) {
		// error
	}

	/* initialize UART 0 with a callback */
	uart_init(UART_DEV(0), 115200, uart_cb, NULL);

	return colour;
}

