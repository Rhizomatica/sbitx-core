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

/* Pin definitions following WirinpiPi numbering // GPIO numbering commented */
#define ENC1_A  13  // GPIO 17 // Pin 11
#define ENC1_B  12  // GPIO 27 // Pin 13
#define ENC1_SW 14  // GPIO 22 // Pin 15

#define ENC2_A   0  // GPIO 10 // Pin 19
#define ENC2_B   2  // GPIO 9  // Pin 21
#define ENC2_SW  3  // GPIO 11 // Pin 23

#define PTT      7  // GPIO  4 // Pin 7
#define DASH    21  // GPIO  5 // Pin 29

#define TX_LINE 4   // GPIO 23 // Pin 16
#define TX_POWER 27 // GPIO 16 // Pin 36
#define LPF_A 5     // GPIO 24 // Pin 18
#define LPF_B 6     // GPIO 25 // Pin 22
#define LPF_C 10    // GPIO  8 // Pin 24
#define LPF_D 11    // GPIO  7 // Pin 26

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
    bool txrx_state; // IN_RX or IN_TX

    // front panel controls and status
    encoder enc_a;
    encoder enc_b;

    int32_t volume_ticks;
    int32_t tuning_ticks;

    uint32_t knob_a_pressed;
    uint32_t knob_b_pressed;

    bool key_down; // this is the ptt button
    bool dash_down;

    // raw values from read from the ATTiny 10bit ADC over I2C
    uint32_t fwd_power;
    uint32_t ref_power;

    uint32_t bridge_compensation;

} radio;


void hw_init(radio *radio_h);
void hw_shutdown(radio *radio_h);

void set_frequency(radio *radio_h, uint32_t frequency);
void tr_switch(radio *radio_h, bool txrx_state);

// disconnect all LPFs
void lpf_off(radio *radio_h);
// set appropriate LPF according to the frequency
void lpf_set(radio *radio_h);

// fwd, ref and swr measurements functions
// call update_power_measurements for a reading
bool update_power_measurements(radio *radio_h);
uint32_t get_fwd_power(radio *radio_h);
uint32_t get_ref_power(radio *radio_h);
uint32_t get_swr(radio *radio_h);

#endif // SBITX_CORE_H_
