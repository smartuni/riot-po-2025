#ifndef VIBRATION_MODULE_H
#define VIBRATION_MODULE_H
#include <stdint.h>

typedef struct {
    uint16_t t_on_ms;
    uint16_t t_off_ms;
    uint16_t intensity;
} vibration_cycle_t;

typedef struct {
  vibration_cycle_t *cycles;
  unsigned len;
} vibration_pattern_t;

extern vibration_pattern_t vibration_short;
extern vibration_pattern_t vibration_long;
extern vibration_pattern_t vibration_three_pulses;

void vibration_module_init(void);

void vibration_start(uint16_t intensity);
void vibration_stop(void);

void vibration_pattern(vibration_pattern_t *p);

#endif // VIBRATION_MODULE_H
