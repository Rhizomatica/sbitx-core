# sbitx-core

This is the sbitx-core. Sbitx-core is intendend to be a simple C
code implementation of the sBitx functionatilies. The following features
are currently implemented:

* I2C communication to the Si5351a and ATTiny85 (power fwd and ref readings)
* GPIO using the WiringPi library, for all kind of radio controls (encoders, lpf bank, tx/rx)
* Si5351a control functions

The source simple_radio.c demonstrates how to use some of currently implemented features.

# Tinkerer tips

In order to test the transmitter, adjust alsamixer playback level to 0, play single tone inside the passband (try anywhere between 8000 and 36000 Hz, example below) and slowly rise playback volume.

```
ffmpeg -f lavfi -i "sine=frequency=24000:sample_rate=96000:duration=0" -ac 2 -map_channel -1 -map_channel 0.0.0 -f alsa  hw:0,0
```

For testing reception, I use a gnuradio-companion script, which has
nice spectrum view modes, available in the "docs/" directory of this repository.


# Author

Rafael Diniz
