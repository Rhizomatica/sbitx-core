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

#ifndef RADIO_H_
#define RADIO_H_

#include <stdint.h>
#include <pthread.h>


#define OPERATING_MODE_VOICE 0 // local operation
#define OPERATING_MODE_DATA 1 // data operation

#define RADIO_RX 0
#define RADIO_TX 1

#define MODE_LSB 0
#define MODE_USB 1

#define AGC_OFF 0
#define AGC_SLOW 0
#define AGC_MED 0
#define AGC_FAST 0

typedef struct {

    int radio_rx_tx; // tx or rx

    int operating_mode; // ANALOG or DIGITAL

    int analog_frequency;
    int digital_frequency;
    int frequency;

    int analog_mode;
    int digital_mode;
    int mode;


    int agc_analog; // OFF, SLOW, MED, FAST
    int agc_digital;
    int agc;

    int mic_gain; // 0 - 100

    int analog_to_digital_timeout; // in seconds

    // oque fazer com o knob de frequencia no painel do radio?
    // bloquear o botao de freq quando tiver em digital? sim!
    // websocket interface
    // operating mode change
    // connected_status
    // digital_frequency
    // analog_frequency
    // frequency
    // operating mode

    // UI fonia - soh vai mudar a analog_frequency
    // radio config - voice analog frequency / digital data frequency
    // radio config - operating mode (analog / digital)
    // oque fazer quando tiver ptt quando tiver no modo digital

    int step_size;
    int volume;

    int serial_number;
} radio_features;



#endif // RADIO_H_
