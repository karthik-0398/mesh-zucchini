#include <stdio.h>
/*#include "net/af.h"
 #include "net/protnum.h"
 #include "net/ipv6/addr.h"
 #include "net/sock/udp.h"
 #include "xtimer.h"
 #include "net/sock.h"
 */
uint8_t buf[7];

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "net/sock/udp.h"

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
#define SLAVE1 1
#define SLAVE2 2
#define SLAVE3 3

//static char udp_stack[THREAD_STACKSIZE_MAIN];

#define UDP_MSG_QUEUE_SIZE 8
static msg_t udp_msg_queue[UDP_MSG_QUEUE_SIZE];

// ********* SERVER DEFINES *********
#define SERVER_MSG_QUEUE_SIZE   (8)
#define SERVER_BUFFER_SIZE      (64)

// ********* SERVER VARIABLES *********
kernel_pid_t listener_pid = -1;
static int server_socket = -1;
char server_buffer[SERVER_BUFFER_SIZE];
static char server_stack[THREAD_STACKSIZE_DEFAULT];
static msg_t server_msg_queue[SERVER_MSG_QUEUE_SIZE];

// ********** PROTOTYPES ********
static int comms_speak_to_server(char *data, int id);

void led_switch(char colour);

/*
 * \brief Thread for the server
 * args ... contains the port as char*
 * TODO need to send error via msg queue to UART
 */
static void* _listener(void* args) {
	struct sockaddr_in6 server_addr;
	uint16_t port = 8808;
	msg_init_queue(server_msg_queue, SERVER_MSG_QUEUE_SIZE);
	server_socket = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);

	int id = -1;

	 //((int) args);
	id = atoi((char *)args); // TODO (char *) ???
	//puts("ID of device");
	//puts((char *)id);

	// id = MASTER;
	id = SLAVE1;

	server_addr.sin6_family = AF_INET6;
	memset(&server_addr.sin6_addr, 0, sizeof(server_addr.sin6_addr));
	server_addr.sin6_port = htons(port);
	if (server_socket < 0) {
		comms_speak_to_server("Server init failed", id);
		server_socket = 0;
		return NULL;
	}

	if (bind(server_socket, (struct sockaddr*) &server_addr,
			sizeof(server_addr)) < 0) {
		server_socket = -1;
		comms_speak_to_server("Socket binding failed", id);
		return NULL;
	}

	// TODO: send id of this server to the computer
	// and tell webserver, that this server is alive
	comms_speak_to_server("UDP server started", id);

	// Server is listening for messages from client
	while (1) {
		int res;
		struct sockaddr_in6 src;
		socklen_t src_len = sizeof(struct sockaddr_in6);
		if ((res = recvfrom(server_socket, server_buffer, sizeof(server_buffer),
				0, (struct sockaddr*) &src, &src_len)) < 0) {
			comms_speak_to_server("No message received", id);
		} else if (res == 0) {
			comms_speak_to_server("Peer shut down", id);
		} else {
			if(id == MASTER){ // MASTER to UART (forward from SLAVE)
				// send to UART
				puts("sending to UART"); // server_buffer
				// puts(data);
				// uart_write(UART_DEV(0), (uint8_t*) data, sizeof(data));
			}else{ // SLAVE to MASTER
				comms_speak_to_server("Changed LED status", id);
				led_switch(server_buffer[0]);
				xtimer_sleep(5);
			}
			// data that is receives is server_buffer TODO
		}
		recvfrom(server_socket, server_buffer, sizeof(server_buffer),
						0, (struct sockaddr*) &src, &src_len);
	}
	return NULL;
}

/*
 * \brief Master sends message to UART
 * \brief Slave sends message to Master
 */
static int comms_speak_to_server(char *data, int id) { // was static before

		puts((char*)id);
		(void) data;
		struct sockaddr_in6 src, dst;
		//size_t data_len = strlen(data);
		uint16_t port = 8808;
		int s;
		char *addr_str = addr_str = "fe80::2121:5329:36b7:6e46";

		char *message = malloc(sizeof(char)*32);

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
			//j = sendto(s, data, data_len, 0, (struct sockaddr *)&dst, sizeof(dst));
			if(j >= 0){ // < 0 is bad
				break;
			}
		}
		if(strlen(message) > 1){
			//sendto(s, message, data_len, 0, (struct sockaddr *)&dst, sizeof(dst));
		}
		close(s);
	return 0;
}

/*
 * \brief Slave forwards message to other Slave
 */
void comms_slave_forwards(void) {

}

// TODO IDEA: every board that sends a message to the
// server just sends it's name at the beginning
int comms_init(int id) {

	msg_init_queue(udp_msg_queue, UDP_MSG_QUEUE_SIZE); // do I need this??

	// ********* SERVER *********
//	if (server_socket >= 0) {
//		message = "Server is already running";
//	}

	listener_pid = thread_create(server_stack, sizeof(server_stack),
				THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
				_listener, (char*)id, "UDP SERVER");

	if (listener_pid <= KERNEL_PID_UNDEF) {
		server_socket = -1;
//		message = "Thread init failed";
		return 1;
	}

	return 0;
}
