#include <stdio.h>
#include "ztimer.h"
#include "pwm/pwm_custom.h"

int main(void)
{
    uint32_t test =  PWM_RES;
    (void) test;

    ztimer_sleep(ZTIMER_MSEC, 3000);
    puts("PWM starten");
    int result = execute_pwm(9, 7000U, PWM_RES);
    if(result == -1) {
        puts("Fehler");
    }
    puts("PWM fertig");
    return 0;
}