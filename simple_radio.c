#include <unistd.h>
#include <string.h>

#include "sbitx_i2c.h"
#include "sbitx_core.h"

int main()
{
    radio radio_h;
    strcpy(radio_h.i2c_device, "/dev/i2c-22");

    hw_init(&radio_h);

    sleep(15);

    return 0;
}
