/* sBitx core sample application
 * Copyright (C) 2023 Rhizomatica
 * Author: Rafael Diniz <rafael@riseup.net>
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 *
 */

/* PTT ON - This sample application enables the power line and transmits in the desired frequency */

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

#include "sbitx_core.h"

int shutdown = 0;

void exit_ptt(int sig)
{
    shutdown = 1;
}

int main(int argc, char *argv[])
{
    radio radio_h;

    signal(SIGINT, exit_ptt);

    if (argc != 2)
    {
        printf("Usage:\n%s <frequency in Hz>\n", argv[0]);
        return EXIT_FAILURE;
    }
    uint32_t frequency = atoi(argv[1]);

    // these are mandatory fields to be filled before hw_init()
    memset(&radio_h, 0, sizeof(radio));
    strcpy(radio_h.i2c_device, "/dev/i2c-22");
    radio_h.bfo_frequency = 40035000;
    radio_h.bridge_compensation = 100;

    hw_init(&radio_h);

    set_frequency(&radio_h, frequency);
    printf("Frequency: %u.\n", frequency);

    tr_switch(&radio_h, IN_TX);
    printf("PTT ON.\n\n");

    // hide cursor
    printf("\e[?25l");

    while (!shutdown)
    {
        if (update_power_measurements(&radio_h))
        {
            printf("   FWD PWR: %.1f   REF PWR: %.1f   SWR: %.1f     \r",
                   (float) get_fwd_power(&radio_h) / 10,
                   (float) get_ref_power(&radio_h) / 10,
                   (float) get_swr(&radio_h) / 10);

        }
        else
        {
            printf("   FWD PWR: ERROR   REF PWR: ERROR   SWR: ERROR     \r");
        }
        usleep(50000);// 50 ms
    }

    // re-enable cursor
    printf("\e[?25h");

    tr_switch(&radio_h, IN_RX);
    printf("\n\nPTT OFF.\nExiting.\n");

    hw_shutdown(&radio_h);

    return EXIT_SUCCESS;
}
