#!/bin/sh
for i in arm core h264 isp v3d uart pwm emmc pixel vec hdmi dpi;
do
    echo -n "$i "
    vcgencmd measure_clock $i
done
