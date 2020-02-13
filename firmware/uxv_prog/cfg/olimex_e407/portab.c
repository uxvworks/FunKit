/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

/**
 * @file    portab.c
 * @brief   Application portability module code.
 *
 * @addtogroup application_portability
 * @{
 */
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hal.h"
#include "portab.h"

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Module local types.                                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module local variables.                                                   */
/*===========================================================================*/

/** The UDP pcb used  */
static struct udp_pcb* my_udp_pcb;

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

/** UDP recv callback for the sntp pcb */
static void my_udp_recv(void *arg, struct udp_pcb* pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
    static uint32_t count = 0;
    
    LWIP_UNUSED_ARG(arg);
    LWIP_UNUSED_ARG(pcb);
  //  LWIP_UNUSED_ARG(addr);
  //  LWIP_UNUSED_ARG(port);
    
    palToggleLine(LINE_LED1);
    count++;
    udp_sendto(my_udp_pcb, p, addr, port);
    
    pbuf_free(p);
}

#define MY_UDP_PORT 7667

static void my_udp_init(void)
{

    if (my_udp_pcb == NULL) {
        my_udp_pcb = udp_new_ip_type(IPADDR_TYPE_ANY);
        LWIP_ASSERT("Failed to allocate udp pcb", my_udp_pcb != NULL);

        if (my_udp_pcb != NULL) {
            udp_recv(my_udp_pcb, my_udp_recv, NULL);

            ip_set_option(my_udp_pcb, SOF_INHERITED);
            udp_bind(my_udp_pcb, IP_ANY_TYPE, MY_UDP_PORT);

        }
    }
}


/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

void portab_setup(void)
{

    lwipInit(NULL);
    my_udp_init();

}

/** @} */
