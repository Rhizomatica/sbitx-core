/* sBitx core
 * Copyright (C) 2023-2024 Rhizomatica
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

#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "gpiolib/gpiolib.h"
#include "sbitx_gpio.h"

// global radio handle pointer used for the callback functions
radio *radio_gpio_h;

struct poll_gpio_state {
    unsigned int num;
    unsigned int gpio;
    const char *name;
    int level;
};

int num_poll_gpios;
struct poll_gpio_state *poll_gpios;

// for now this initializes the GPIO and also initializes the structures for
// encoder/knobs for easy reading by application
void gpio_init(radio *radio_h)
{
    // we need the radio handler available for the callbacks.
    radio_gpio_h = radio_h;

    int ret = gpiolib_init();
    if (ret < 0)
    {
        printf("Failed to initialise gpiolib - %d\n", ret);
        return ;
    }

    ret = gpiolib_mmap();
    if (ret)
    {
        if (ret == EACCES && geteuid())
            printf("Must be root\n");
        else
            printf("Failed to mmap gpiolib - %s\n", strerror(ret));
        return ;
    }

    unsigned pins[8] = {ENC1_A, ENC1_B, ENC1_SW, ENC2_A, ENC2_B, ENC2_SW, PTT, DASH};
    for (int i = 0; i < 8; i++)
    {
        // if gpio_num_is_valid(ENC1_A);
        gpio_set_dir(pins[i], DIR_INPUT);
        gpio_set_pull(pins[i], PULL_UP);
    }

    gpio_set_dir(TX_LINE, DIR_OUTPUT);
    // gpio_set_dir(TX_POWER, DIR_OUTPUT);
    gpio_set_dir(LPF_A, DIR_OUTPUT);
    gpio_set_dir(LPF_B, DIR_OUTPUT);
    gpio_set_dir(LPF_C, DIR_OUTPUT);
    gpio_set_dir(LPF_D, DIR_OUTPUT);

    //setup the LPFs and TX lines to initial state
    gpio_set_drive(LPF_A, DRIVE_LOW);
    gpio_set_drive(LPF_B, DRIVE_LOW);
    gpio_set_drive(LPF_C, DRIVE_LOW);
    gpio_set_drive(LPF_D, DRIVE_LOW);
    gpio_set_drive(TX_LINE, DRIVE_LOW);
    // gpio_set_drive(TX_POWER, DRIVE_LOW);

    // Initialize our two encoder structs (front pannel knobs)
    enc_init(&radio_h->enc_a, ENC_FAST, ENC1_B, ENC1_A);
    enc_init(&radio_h->enc_b, ENC_FAST, ENC2_A, ENC2_B);

    radio_h->volume_ticks = 0;
    radio_h->tuning_ticks = 0;

    // we need to import the poll functions
#if 0
    // Setting the callback for the encoders interrupts
    wiringPiISR(ENC2_A, INT_EDGE_BOTH, tuning_isr_b);
    wiringPiISR(ENC2_B, INT_EDGE_BOTH, tuning_isr_b);

    wiringPiISR(ENC1_A, INT_EDGE_BOTH, tuning_isr_a);
    wiringPiISR(ENC1_B, INT_EDGE_BOTH, tuning_isr_a);

    wiringPiISR(ENC1_SW, INT_EDGE_FALLING, knob_a_pressed);
    wiringPiISR(ENC2_SW, INT_EDGE_FALLING, knob_b_pressed);

    wiringPiISR(PTT, INT_EDGE_BOTH, ptt_change);
    wiringPiISR(DASH, INT_EDGE_BOTH, dash_change);
#endif
}


void enc_init(encoder *e, int speed, int pin_a, int pin_b)
{
    e->pin_a = pin_a;
    e->pin_b = pin_b;
    e->speed = speed;
    e->history = 5;
}

void ptt_change()
{
    if (gpio_get_level(PTT) == 0)
        radio_gpio_h->key_down = true;
    else
        radio_gpio_h->key_down = false;
}

void dash_change()
{
    if (gpio_get_level(DASH) == 0)
        radio_gpio_h->dash_down = true;
    else
        radio_gpio_h->dash_down = false;
}

void knob_a_pressed(void)
{
    radio_gpio_h->knob_a_pressed++;
}

void knob_b_pressed(void)
{
    radio_gpio_h->knob_b_pressed++;
}


void tuning_isr_a(void)
{
    static bool first = true;

    int tuning = enc_read(&radio_gpio_h->enc_a);

    if (!first)
    {
        if (tuning < 0)
            radio_gpio_h->volume_ticks++;
        if (tuning > 0)
            radio_gpio_h->volume_ticks--;
    }
    else
        first = false;
}

void tuning_isr_b(void)
{
    static bool first = true;

    int tuning = enc_read(&radio_gpio_h->enc_b);

    if (!first)
    {
        if (tuning < 0)
            radio_gpio_h->tuning_ticks++;
        if (tuning > 0)
            radio_gpio_h->tuning_ticks--;
    }
    else
        first = false;
}

int enc_state (encoder *e)
{
    return (gpio_get_level(e->pin_a) ? 1 : 0) + (gpio_get_level(e->pin_b) ? 2: 0);
}

int enc_read(encoder *e)
{
    int result = 0;
    int newState;

    newState = enc_state(e); // Get current state

    if (newState != e->prev_state)
        usleep(1000); // 1ms

    if (enc_state(e) != newState || newState == e->prev_state)
        return 0;

    //these transitions point to the encoder being rotated anti-clockwise
    if ((e->prev_state == 0 && newState == 2) ||
        (e->prev_state == 2 && newState == 3) ||
        (e->prev_state == 3 && newState == 1) ||
        (e->prev_state == 1 && newState == 0))
    {
        e->history--;
    }
    //these transitions point to the enccoder being rotated clockwise
    if ((e->prev_state == 0 && newState == 1) ||
        (e->prev_state == 1 && newState == 3) ||
        (e->prev_state == 3 && newState == 2) ||
        (e->prev_state == 2 && newState == 0))
    {
        e->history++;
    }
    e->prev_state = newState; // Record state for next pulse interpretation
    if (e->history > e->speed)
    {
        result = 1;
        e->history = 0;
    }
    if (e->history < -e->speed)
    {
        result = -1;
        e->history = 0;
    }

    return result;
}

// imported from pinctrl.c libgpio cli
