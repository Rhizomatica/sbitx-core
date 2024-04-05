#!/bin/sh
echo ondemand > /sys/bus/cpu/devices/cpu0/cpufreq/scaling_governor
echo ondemand > /sys/bus/cpu/devices/cpu1/cpufreq/scaling_governor
echo ondemand > /sys/bus/cpu/devices/cpu2/cpufreq/scaling_governor
echo ondemand > /sys/bus/cpu/devices/cpu3/cpufreq/scaling_governor
