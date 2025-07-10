#include "soundModule.h"
#include <stdio.h>
#include "board.h"
#include "ztimer.h"
#include "periph/gpio.h"
#include "thread.h"

#define MSG_TYPE_BEEP_SEQUENCE         (0x42AB)
#define MSG_TYPE_PLAYBACK_PAYLOAD      (0x42AC)
#define BEEP_SEQ_INIT(BEEPS) {.beeps = BEEPS, .len = ARRAY_SIZE(BEEPS)}

/* Size of reception message queue */
#define QUEUE_SIZE 8
/* Message queue for reception thread] */
static msg_t notify_msg_queue[QUEUE_SIZE];
static kernel_pid_t pid = KERNEL_PID_UNDEF;
static char thread_stack[THREAD_STACKSIZE_SMALL];

gpio_t sound = GPIO_PIN(0, 8);
gpio_mode_t sound_mode = GPIO_OUT;

typedef struct {
    beep_sequence_t *beep_sequence; /*< sequence to be played */
    mutex_t *lock;  /*< mutex to unlock after finishing playback.
                        NULL for async playback */
} playback_payload_t;

beep_t _startup_beeps[] = {
    {.freq=1318, .t_ms=500},
    {.freq=659,  .t_ms=500},
    {.freq=988,  .t_ms=500},
    {.freq=880,  .t_ms=500},
    {.freq=1318, .t_ms=600},
    {.freq=988,  .t_ms=800}
};

beep_t _downlink_rx_beeps[] = {
    {.freq=988, .t_ms=400}, // h''
    {.freq=880, .t_ms=400}, // a''
    {.freq=784, .t_ms=500}, // g''
};

beep_t _uplink_tx_beeps[] = {
    {.freq=784, .t_ms=300},
    {.freq=880, .t_ms=300},
    {.freq=988, .t_ms=300},
};

beep_t _ble_rx_beeps[] = {
    {.freq=1000, .t_ms=50},
    {.freq=2000, .t_ms=50},
};

beep_t _ble_rx_news_beeps[] = {
    {.freq=932, .t_ms=300},
    {.freq=831, .t_ms=400},
};

beep_t _ble_tx_beeps[] = {
    {.freq=831, .t_ms=400},
    {.freq=932, .t_ms=300},
};

beep_sequence_t sound_startup = BEEP_SEQ_INIT(_startup_beeps);
beep_sequence_t sound_downlink_rx = BEEP_SEQ_INIT(_downlink_rx_beeps);
beep_sequence_t sound_uplink_tx = BEEP_SEQ_INIT(_uplink_tx_beeps);
beep_sequence_t sound_ble_rx = BEEP_SEQ_INIT(_ble_rx_beeps);
beep_sequence_t sound_ble_rx_news = BEEP_SEQ_INIT(_ble_rx_news_beeps);
beep_sequence_t sound_ble_tx = BEEP_SEQ_INIT(_ble_tx_beeps);

void _beep(int frequency, int duration_ms) {
    // Play sound at a specific frequency for a given duration
    int delay = 1000000 / frequency; // Calculate delay based on frequency
    int cycles = (duration_ms * 1000) / delay; // Calculate number of cycles based on duration

    for (int i = 0; i < cycles; i++) {
        gpio_toggle(sound);
        ztimer_sleep(ZTIMER_USEC, delay / 2); // Half period for toggle
    }
    gpio_clear(sound); // Ensure sound is off after playing
}

static void _play_beep_seq(beep_sequence_t *bs) {
    for (unsigned i = 0; i < bs->len; i++) {
        _beep(bs->beeps[i].freq, bs->beeps[i].t_ms);
    }
}

static void* thread_sound_function(void *arg) {
    (void)arg; // Unused argument

    msg_t msg;
    /* initialize the message queue] */
    msg_init_queue(notify_msg_queue, QUEUE_SIZE);

    while (1) {
        /* wait until we get a message]*/
        msg_receive(&msg);
        
        if (msg.type == MSG_TYPE_BEEP_SEQUENCE) {
            beep_sequence_t *bs = (beep_sequence_t*)msg.content.ptr;
            _play_beep_seq(bs);
        } else if (msg.type == MSG_TYPE_PLAYBACK_PAYLOAD) {
            playback_payload_t *pp = (playback_payload_t*)msg.content.ptr;
            _play_beep_seq(pp->beep_sequence);
            if (pp->lock) {
                /* unblock waiting thread */
                mutex_unlock(pp->lock);
            }
        }

    }
    return NULL;
}

void sound_module_init(void) {
    // Initialize the sound module
    gpio_init(sound, sound_mode);
    gpio_clear(sound);

    pid = thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, thread_sound_function, NULL, "sound_thread");
    printf("Sound module initialized.\n");
}

void sound_play(beep_sequence_t *bs) {
    msg_t m = { .type = MSG_TYPE_BEEP_SEQUENCE, .content.ptr = bs };
    /* offload playing the sound to the thread */
    msg_send(&m, pid);
}

void sound_play_blocking(beep_sequence_t *bs) {
    mutex_t sync_lock = MUTEX_INIT_LOCKED;
    playback_payload_t pp = { .lock = &sync_lock, .beep_sequence = bs };
    msg_t m = { .type = MSG_TYPE_PLAYBACK_PAYLOAD, .content.ptr = &pp };
    /* offload playing the sound to the thread */
    msg_send(&m, pid);

    /* wait till playback has finished */
    mutex_lock(&sync_lock);
}
