#ifndef _PWM_CUSTOM_H
#define _PWM_CUSTOM_H

    #include <stdio.h>
    #include "ztimer.h"
    #include "periph/pwm.h"

    #define PWM_DEVI         (PWM_DEV(1))   // PWM-Gerät 0 verwenden
    #define PWM_CHANNEL     (0)            // Kanal 0
    #define PWM_FREQ        (1000U)        // 1 kHz Frequenz
    #define PWM_RES         (1000U)        // Auflösung = 1000 Schritte (für Duty-Cycle)

    int execute_pwm(int pin, uint32_t duration, int frequency);

#endif