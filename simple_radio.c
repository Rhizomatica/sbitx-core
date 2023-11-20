#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "sbitx_i2c.h"
#include "sbitx_core.h"

int main(int argc, char *argv[])
{
    radio radio_h;

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

    hw_init(&radio_h);

    set_frequency(&radio_h, frequency);

    int loop_counter = 30;
    while(loop_counter--)
    {
        printf("\n");
        printf("volume_ticks: %d\n", radio_h.volume_ticks);
        printf("tuning_ticks: %d\n", radio_h.tuning_ticks);
        printf("knob A pressed: %u\n", radio_h.knob_a_pressed);
        printf("knob B pressed: %u\n", radio_h.knob_b_pressed);
        sleep(1);
    }


    return EXIT_SUCCESS;
}
