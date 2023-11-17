#ifndef

RADIO_H_
#define RADIO_H_

struct {

    int tx_rx;

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
}


#endif // RADIO_H_
