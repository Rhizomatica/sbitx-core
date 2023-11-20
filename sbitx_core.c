/* sBitx core
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

#include "sbitx_core.h"
#include "sbitx_i2c.h"
#include "sbitx_gpio.h"
#include "sbitx_si5351.h"

#include <wiringPi.h>


void hw_init(radio *radio_h)
{
    // I2C SETUP
    i2c_open(radio_h);

    // GPIO SETUP
    gpio_init(radio_h);

    // Si5351 SETUP
    setup_oscillators(radio_h);
}

void hw_shutdown(radio *radio_h)
{
    i2c_close(radio_h);

    // WiringPi has no function to close/shutdown resources
    // should we stop the Si5351 clocks?
}

void set_frequency(radio *radio_h, uint32_t frequency)
{
    radio_h->frequency = frequency;
     // Were we are setting the real frequency of the radio (in USB), without the 24 kHz offset in Ashhar implementation (just "- 24000" to replicate the behavior)
    si5351bx_setfreq(2, radio_h->frequency + radio_h->bfo_frequency);
}
