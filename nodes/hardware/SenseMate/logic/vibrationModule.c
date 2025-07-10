#include "vibrationModule.h"
#include <stdio.h>
#include "board.h"
#include "ztimer.h"
#include "periph/gpio.h"
#include "periph/pwm.h"

//gpio_t vibration = GPIO_PIN(1, 9);
//gpio_mode_t vibtation_mode = GPIO_OUT;

#define MSG_TYPE_VIBRA_PATTERN (0xB0B0)
#define PWM_DEV_VIBRA    (PWM_DEV(0))
#define PWM_CHAN_VIBRA   (0)
#define PWM_MODE_VIBRA   PWM_LEFT
#define PWM_FREQ_VIBRA   (1000LU)
#define PWM_RES_VIBRA    (1000LU)

#define VIBRATION_PATTERN_INIT(CYCLES) {.cycles = CYCLES, .len = ARRAY_SIZE(CYCLES)}

/* Size of reception message queue */
#define QUEUE_SIZE 8
/* Message queue for reception thread] */
static msg_t notify_msg_queue[QUEUE_SIZE];
static kernel_pid_t pid = KERNEL_PID_UNDEF;
static char thread_stack[THREAD_STACKSIZE_SMALL];

vibration_cycle_t _short_cycle[]  = {{.t_on_ms = 500,  .t_off_ms = 0,   .intensity = UINT16_MAX/2}};
vibration_cycle_t _long_cycle[]   = {{.t_on_ms = 1000, .t_off_ms = 0,   .intensity = UINT16_MAX/2}};
vibration_cycle_t _three_cycles[] = {{.t_on_ms = 100,  .t_off_ms = 200, .intensity = UINT16_MAX},
                                     {.t_on_ms = 100,  .t_off_ms = 200, .intensity = UINT16_MAX},
                                     {.t_on_ms = 100,  .t_off_ms = 0,   .intensity = UINT16_MAX},
                                    };

vibration_pattern_t vibration_short = VIBRATION_PATTERN_INIT(_short_cycle);
vibration_pattern_t vibration_long = VIBRATION_PATTERN_INIT(_long_cycle);
vibration_pattern_t vibration_three_pulses = VIBRATION_PATTERN_INIT(_three_cycles);

void vibration_start(uint16_t intensity) {
    uint16_t pwm_val = PWM_RES_VIBRA * intensity / UINT16_MAX;
    // Start vibration
    pwm_set(PWM_DEV_VIBRA, PWM_CHAN_VIBRA, pwm_val);
    pwm_poweron(PWM_DEV_VIBRA);
}

void vibration_stop(void) {
    // Stop vibration
    pwm_set(PWM_DEV_VIBRA, PWM_CHAN_VIBRA, 0);
    pwm_poweroff(PWM_DEV_VIBRA);
}

static void* thread_vibration_function(void *arg) {
    (void)arg; // Unused argument

    msg_t msg;
    /* initialize the message queue] */
    msg_init_queue(notify_msg_queue, QUEUE_SIZE);

    while (1) {
        /* wait until we get a message]*/
        msg_receive(&msg);

        if (msg.type == MSG_TYPE_VIBRA_PATTERN) {
            vibration_pattern_t *vp = (vibration_pattern_t*)msg.content.ptr;
            if (vp) {
                for (unsigned i = 0; i < vp->len; i++) {
                    vibration_cycle_t *c = vp->cycles;
                    if (c->t_on_ms > 0) {
                        vibration_start(c->intensity);
                        ztimer_sleep(ZTIMER_MSEC, c->t_on_ms);
                        vibration_stop();
                    }
                    if (c->t_off_ms > 0) {
                        ztimer_sleep(ZTIMER_MSEC, c->t_off_ms);
                    }
                }
            }
        }
    }
    return NULL;
}

void vibration_module_init(void) {
    pwm_init(PWM_DEV_VIBRA, PWM_CHAN_VIBRA, 1000, PWM_RES_VIBRA);
    pwm_set(PWM_DEV_VIBRA, PWM_CHAN_VIBRA, 0);
    pwm_poweroff(PWM_DEV_VIBRA);

    pid = thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, thread_vibration_function, NULL, "vibra_thread");
    printf("Vibration module initialized.\n");
}

void vibration_pattern(vibration_pattern_t *p) {
    msg_t m = { .type = MSG_TYPE_VIBRA_PATTERN, .content.ptr = p };

    /* offload vibration pattern to the thread */
    msg_send(&m, pid);
}
