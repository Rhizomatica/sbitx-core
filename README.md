# sbitx-core

This is the sbitx-core. Sbitx-core is intendend to be a simple C
code implementation of the sBitx functionatilies. The following features
are currently implemented:

* I2C communication to the Si5351a and ATTiny85 (power fwd and ref readings)
* GPIO using the WiringPi library, for all kind of radio controls (encoders, lpf bank, tx/rx)
* Si5351a control functions

The source simple_radio.c demonstrates how to use some of currently implemented features. The source ptt_on.c puts the radio in transmit mode in a desired frequency.


This code expects the I2C bus to be kernel I2C interface. Make sure you have one of the dtoverlays
in /boot/config.txt

For loading the I2C Bit-Banged (BB) driver in the appropriate sBitx pins, with kernel RTC clock driver loaded (for use as system clock, this is what I use):
```
dtoverlay=i2c-rtc-gpio,ds1307,bus=2,i2c_gpio_sda=13,i2c_gpio_scl=6
```
For just loading I2C bus:

```
dtoverlay=i2c-gpio,bus=2,i2c_gpio_sda=13,i2c_gpio_scl=6
```

And comment out all lines starting with "gpio=".

For detecting the I2C interfaces use:

```
i2cdetect -l

```

With gpio and RTC, it lists as:

```
i2c-22  i2c             i2c-gpio-rtc@0                          I2C adapter
```

By looking the I2C bus where the I2C BB driver is loaded, use the appropriate device file, eg., /dev/i2c-22 or /dev/i2c-2, in the sBitx core API.

# Compilation

Just type "make" to build the two example applications. 


# Usage

Both syntax are identical:

```
# ptt_on <frequency in Hz>
# simple_radio <frequency in Hz>
```

* ptt_on: enables the transmission line, and disables at exit (use Ctrl+C)
* simple_radio: a simple radio implementation which allows for debugging most of radio features

# Tinkerer tips

In order to test the transmitter, adjust alsamixer playback level to 0, play single tone inside the passband (try anywhere between 8000 and 36000 Hz, example below) and slowly rise playback volume.

```
ffmpeg -f lavfi -i "sine=frequency=24000:sample_rate=96000:duration=0" -ac 2 -map_channel -1 -map_channel 0.0.0 -f alsa  hw:0,0
```

For testing reception, I use a gnuradio-companion script, which has
nice spectrum view modes, available in the "docs/" directory of this repository.


# Author

Rafael Diniz
