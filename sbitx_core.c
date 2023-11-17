
#include "sbitx_core.h"
#include "sbitx_i2c.h"

#include <wiringPi.h>

radio *internal_radio_h;

void hw_init(radio *radio_h)
{
    internal_radio_h = radio_h;
    // I2C SETUP
    i2c_open(radio_h);

    // GPIO SETUP
	wiringPiSetup();

    char pins[13] = {0, 2, 3, 6, 7, 10, 11, 12, 13, 14, 21, 25, 27};
	for (int i = 0; i < 13; i++){
		pinMode(pins[i], INPUT);
		pullUpDnControl(pins[i], PUD_UP);
	}

    // Initialize our two encoders (front pannel knobs) struct
	enc_init(&radio_h->enc_a, ENC_FAST, ENC1_B, ENC1_A);
	enc_init(&radio_h->enc_b, ENC_FAST, ENC2_A, ENC2_B);

    radio_h->volume_ticks = 0;
    radio_h->tuning_ticks = 0;

    // Setting the callback for the encoders interrupts
    wiringPiISR(ENC2_A, INT_EDGE_BOTH, tuning_isr_b);
    wiringPiISR(ENC2_B, INT_EDGE_BOTH, tuning_isr_b);

    wiringPiISR(ENC1_A, INT_EDGE_BOTH, tuning_isr_a);
    wiringPiISR(ENC1_B, INT_EDGE_BOTH, tuning_isr_a);

    wiringPiISR(ENC1_SW, INT_EDGE_BOTH, knob_a_pressed);
    wiringPiISR(ENC2_SW, INT_EDGE_BOTH, knob_b_pressed);


}


void enc_init(encoder *e, int speed, int pin_a, int pin_b)
{
	e->pin_a = pin_a;
	e->pin_b = pin_b;
	e->speed = speed;
	e->history = 5;
}

void knob_a_pressed(void)
{
    internal_radio_h->knob_a_pressed++;
}

void knob_b_pressed(void)
{
    internal_radio_h->knob_b_pressed++;
}


void tuning_isr_a(void)
{
	int tuning = enc_read(&internal_radio_h->enc_a);
    static bool first = true;
    if (!first)
    {
        if (tuning < 0)
            internal_radio_h->volume_ticks++;
        if (tuning > 0)
            internal_radio_h->volume_ticks--;
    }
    else
        first = false;
}

void tuning_isr_b(void){
	int tuning = enc_read(&internal_radio_h->enc_b);
    static bool first = true;
    if (!first)
    {
        if (tuning < 0)
            internal_radio_h->tuning_ticks++;
        if (tuning > 0)
            internal_radio_h->tuning_ticks--;
    }
    else
        first = false;
}

int enc_state (encoder *e)
{
	return (digitalRead(e->pin_a) ? 1 : 0) + (digitalRead(e->pin_b) ? 2: 0);
}

int enc_read(encoder *e)
{
    int result = 0;
    int newState;

    newState = enc_state(e); // Get current state

    if (newState != e->prev_state)
        delay (1);

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
