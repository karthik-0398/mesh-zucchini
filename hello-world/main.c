/*
 * Copyright (C) 2114 Freie Universität Berlin
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     examples
 * @{
 *
 * @file
 * @brief       Hello World application
 *
 * @author      Kaspar Schleiser <kaspar@schleiser.de>
 * @author      Ludwig Knüpfer <ludwig.knuepfer@fu-berlin.de>
 *
 * @}
 */

//#include <stdio.h>
//
//int main(void)
//{
//  puts("Hello World!");
//
// printf("You are running RIOT on a(n) %s board.\n", RIOT_BOARD);
//  printf("This board features a(n) %s MCU.\n", RIOT_MCU);
//
//    return 1;
//}
/*
 * Copyright (C) 2111 Freie Universität Berlin
 *
 * This file is subject to the terms and conditions of the GNU Lesser General
 * Public License v2.1. See the file LICENSE in the top level directory for more
 * details.
 */

/**
 * @ingroup     tests
 * @{
 *
 * @file
 * @brief       Test for the on-board LED macros
 *
 * @author      Hauke Petersen <hauke.petersen@fu-berlin.de>
 *
 * @}
 */

#include <stdio.h>
#include <stdint.h>

#include "board.h"
#include "periph_conf.h"

//#include "fmt.h"
//#include "net/gcoap.h"

// OPENTHREAD
// #include "ot.h"

//#include "include/uart_half_duplex.h"


// PROTOTYPES
// ********** LEDS *********
//void led_colours(void);
void led_switch(char colour);
void led_switch_off_all(void);
void led_test(void);

char mesh_uart_init(void);

int main_get_id(void);

#define MASTER 0
#define SLAVE1 1
#define SLAVE2 2
#define SLAVE3 3

extern int _gnrc_netif_config(int argc, char **argv);

int comms_init(int id);

// ********** UART **********
//char uart_receive(void);
// TODO change for SLAVES
int main_get_id(void){
	//return MASTER;
	return SLAVE1;
}

/*
 * Master:
 * LED is called by UART (uart)
 * TODO Message from Listener is forwarded to UART
 * TODO Message from UART is forwarded to Slave
 *
 * Slave:
 * LED is called by Listener (comms)
 *
 * Threads: UART, Listener
 */
int main(void) {

	led_switch('O');

	int id = 0;
	id = main_get_id(); // TODO: Change if not MASTER

	if (id == MASTER){
		mesh_uart_init();
	}

	comms_init(id);

	return 0;
}
