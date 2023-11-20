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

#ifndef SBITX_CORE_H_
#define SBITX_CORE_H_

#include <stdbool.h>
#include <stdint.h>
#include <pthread.h>


/* Pin definitions following WirinpiPi numbering // BCM numbering commented */
#define ENC1_A  13  // BCM  9
#define ENC1_B  12  // BCM 10
#define ENC1_SW 14  // BCM 11

#define ENC2_A   0  // BCM 17
#define ENC2_B   2  // BCM 27
#define ENC2_SW  3  // BCM 22

#define PTT      7  // BCM  4
#define DASH    21  // BCM  5

#define TX_LINE 4   // BCM 23
#define TX_POWER 27 // BCM 16
#define LPF_A 5     // BCM 24
#define LPF_B 6     // BCM 25
#define LPF_C 10    // BCM  8
#define LPF_D 11    // BCM  7


/* Encoder speed defines */
#define ENC_FAST 1
#define ENC_SLOW 5

/* tx/rx states */
#define IN_RX 0
#define IN_TX 1

//encoder state variables
typedef struct
{
	int pin_a,  pin_b;
	int speed;
	int prev_state;
	int history;
} encoder;

// radio variables
typedef struct
{
    // I2C
    char i2c_device[64];
    int i2c_bus;
    pthread_mutex_t i2c_mutex;

    // Radio status
    uint32_t frequency;
    uint32_t bfo_frequency;

    encoder enc_a;
    encoder enc_b;

    int32_t volume_ticks;
    int32_t tuning_ticks;

    uint32_t knob_a_pressed;
    uint32_t knob_b_pressed;

    bool key_down;
    bool dash_down;

    bool txrx_relay;
} radio;


void hw_init(radio *radio_h);
void hw_shutdown(radio *radio_h);

void set_frequency(radio *radio_h, uint32_t frequency);


#endif // SBITX_CORE_H_
