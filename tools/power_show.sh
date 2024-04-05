#!/bin/sh
echo -n "CPU 0: "
cat /sys/bus/cpu/devices/cpu0/cpufreq/scaling_governor
echo -n "CPU 1: "
cat /sys/bus/cpu/devices/cpu1/cpufreq/scaling_governor
echo -n "CPU 2: "
cat /sys/bus/cpu/devices/cpu2/cpufreq/scaling_governor
echo -n "CPU 3: "
cat /sys/bus/cpu/devices/cpu3/cpufreq/scaling_governor
