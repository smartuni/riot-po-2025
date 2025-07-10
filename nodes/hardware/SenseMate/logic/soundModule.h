#ifndef SOUNDMODULE_H
#define SOUNDMODULE_H

#include <stdint.h>

typedef struct {
    uint16_t freq;
    uint16_t t_ms;
} beep_t;

typedef struct {
    beep_t *beeps;
    unsigned len;
} beep_sequence_t;

extern beep_sequence_t sound_startup;
extern beep_sequence_t sound_downlink_rx;
extern beep_sequence_t sound_uplink_tx;
extern beep_sequence_t sound_ble_rx;
extern beep_sequence_t sound_ble_rx_news;
extern beep_sequence_t sound_ble_tx;

void sound_module_init(void);

void sound_play(beep_sequence_t *bs);
void sound_play_blocking(beep_sequence_t *bs);
#endif // SOUNDMODULE_H
