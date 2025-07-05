#include "pwm_custom.h"

int execute_pwm(int pin, uint32_t duration, int frequency) {
    puts("Entered function");
    // Zeitstempel 1
    uint32_t start = ztimer_now(ZTIMER_MSEC);
    uint32_t time_passed = ztimer_now(ZTIMER_MSEC);

    int channel = pin - 2;
     // PWM initialisieren
    if (pwm_init(PWM_DEVI, channel, frequency, PWM_RES) != 0) {
        puts("PWM-Init fehlgeschlagen");
        return -1;
    }

    while (time_passed < duration) {
        printf("Time: %ld\n", time_passed);
        // Duty-Cycle schrittweise erhöhen
        for (int duty = 0; duty <= (int) PWM_RES; duty += 100) {
            pwm_set(PWM_DEVI, channel, duty);
            ztimer_sleep(ZTIMER_MSEC, 100);
        }

        // Duty-Cycle schrittweise verringern
        for (int duty = (int) PWM_RES; duty >= 0; duty -= 100) {
            pwm_set(PWM_DEVI, channel, duty);
            ztimer_sleep(ZTIMER_MSEC, 100);
        }

        // jetzige Zeit
        uint32_t now = ztimer_now(ZTIMER_MSEC);
        // Differenz
        time_passed = now - start;
    }

    puts("Return to main");
    return 0;
}