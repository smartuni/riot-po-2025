#include "include/displayDemo.h"
#include "ztimer.h"
#include <stdio.h>
#include <list.h>
#include "board.h"
#include "periph/gpio.h"
#include "ztimer.h"
#include "include/interrupts.h"
#include "include/events_creation.h"
#include "tables.h"
#include "mate_lorawan.h"
#include "include/new_menu.h"
#include "include/soundModule.h"
#include "include/vibrationModule.h"

#include "mate_ble.h"

//void fill_tables_test(void); //for menu testing purposes

char ble_send_stack[2*THREAD_STACKSIZE_DEFAULT];
char ble_reicv_stack[2*THREAD_STACKSIZE_DEFAULT];

int lorawan_started = -1;

int main(void) {
    //ztimer_sleep(ZTIMER_MSEC, 3000); //if use term activate sleep to see all prints
    init_interrupt();
    init_sound_module();
    init_vibration_module();
    event_post(&sound_queue, &start_sound_event);
    //startup_sound();

    //printf("Display demo started.\n");
    init_display();
    printf("Device Type: %d device id: %d\n",DEVICE_TYPE, DEVICE_ID);
    printf("Display initialized.\n");
    //init_menu();
    init_new_menu();
    //set_current_meustate(INIT);
    update_menu_display();
    init_event();
    init_tables();
    //is_state_entry receiveide_is_state;
    //int hallo = get_is_state_entry(1, &receiveide_is_state);
    //printf("Gate Id %d\n", receiveide_is_state.gateID);
    //fill_tables_test();
    
    update_menu_from_tables();
    //(void) test_merge;
    //(void) hallo;
    // const is_state_entry* table = get_is_state_table();
    // for (int i = 0; i < 255; i++){
    //     printf("ID: %d, State: %d, Time: %d\n", table[i].gateID, table[i].state, table[i].gateTime);
    // }

    

    puts("starting ble");
    if (BLE_SUCCESS == ble_init()){
        puts("Ble init complete");
    } else {
        puts("BLE not started");
    }

    thread_create(
        ble_send_stack,
        sizeof(ble_send_stack),
        THREAD_PRIORITY_MAIN - 2,
        THREAD_CREATE_STACKTEST,
        ble_send_loop,
        NULL,
       "bleSend"
    );

    ble_receive_thread_args_t args = {
        .receive_queue = &events_creation_queue,
        .receive_event = &eventBleNews,

    };

    thread_create(
        ble_reicv_stack,
        sizeof(ble_reicv_stack),
        THREAD_PRIORITY_MAIN - 3,
        THREAD_CREATE_STACKTEST,
        ble_receive_loop,
        &args,
       "bleRecv"
    );

    lorawan_started = start_lorawan();

    puts("entering main loop");
    while (1)
    {
        if(lorawan_started == -1){
            lorawan_started = start_lorawan();
        }
        ztimer_sleep(ZTIMER_MSEC, 1000);
        //refresh_display();
        increment_device_timestamp();
    }
    
    printf("Display demo finished.\n");

    return 0;
}

void fill_tables_test(void){
    is_state_entry test;
    test.gateID = 1;
    test.state = 1;
    test.gateTime = 20000;
    int test_merge = set_is_state_entry(&test);
    test.gateID = 2;
    test.state = 1;
    test.gateTime = 20000;
    test_merge = set_is_state_entry(&test);
    test.gateID = 3;
    test.state = 1;
    test.gateTime = 20000;
    test_merge = set_is_state_entry(&test);
    test.gateID = 4;
    test.state = 1;
    test.gateTime = 20000;
    test_merge = set_is_state_entry(&test);
    test.gateID = 5;
    test.state = 1;
    test.gateTime = 20000;
    test_merge = set_is_state_entry(&test);
    test.gateID = 6;
    test.state = 1;
    test.gateTime = 20000;
    test_merge = set_is_state_entry(&test);
    test.gateID = 7;
    test.state = 1;
    test.gateTime = 20000;
    test_merge = set_is_state_entry(&test);
    printf("Test merge: %d\n", test_merge);

    seen_status_entry test_seen;
    test_seen.gateID = 1;
    test_seen.status = 1;
    test_seen.gateTime = 2000;
    test_seen.senseMateID = 3;
    int test_seen_merge = set_seen_status_entry(&test_seen);
    test_seen.gateID = 2;
    test_seen.status = 0;
    test_seen.gateTime = 25000;
    test_seen.senseMateID = 5;
    test_seen_merge = set_seen_status_entry(&test_seen);
    test_seen.gateID = 3;
    test_seen.status = 1;
    test_seen.gateTime = 30000;
    test_seen.senseMateID = 7;
    test_seen_merge = set_seen_status_entry(&test_seen);
    test_seen.gateID = 4;
    test_seen.status = 0;
    test_seen.gateTime = 28647;
    test_seen.senseMateID = 5;
    test_seen_merge = set_seen_status_entry(&test_seen);
    test_seen.gateID = 5;
    test_seen.status = 0;
    test_seen.gateTime = 1000;
    test_seen.senseMateID = 3;
    test_seen_merge = set_seen_status_entry(&test_seen);
    test_seen.gateID = 6;
    test_seen.status = 1;
    test_seen.gateTime = 5000;
    test_seen.senseMateID = 7;
    test_seen_merge = set_seen_status_entry(&test_seen);
    test_seen.gateID = 7;
    test_seen.status = 1;
    test_seen.gateTime = 10000;
    test_seen.senseMateID = 5;
    test_seen_merge = set_seen_status_entry(&test_seen);
    printf("Test merge: %d\n", test_seen_merge);
    
    target_state_entry test_target;
    test_target.gateID = 1;
    test_target.state = 1;
    test_target.timestamp = 20000;
    int test_target_merge = set_target_state_entry(&test_target);
    test_target.gateID = 2;
    test_target.state = 0;
    test_target.timestamp = 25000;
    test_target_merge = set_target_state_entry(&test_target);
    test_target.gateID = 3;
    test_target.state = 1;
    test_target.timestamp = 30000;
    test_target_merge = set_target_state_entry(&test_target);
    test_target.gateID = 4;
    test_target.state = 0;
    test_target.timestamp = 28647;
    test_target_merge = set_target_state_entry(&test_target);
    test_target.gateID = 5;
    test_target.state = 0;
    test_target.timestamp = 1000;
    test_target_merge = set_target_state_entry(&test_target);
    test_target.gateID = 6;
    test_target.state = 1;
    test_target.timestamp = 5000;
    test_target_merge = set_target_state_entry(&test_target);
    test_target.gateID = 7;
    test_target.state = 1;
    test_target.timestamp = 10000;
    test_target_merge = set_target_state_entry(&test_target);
    printf("Test merge: %d\n", test_target_merge);

    jobs_entry test_job;
    test_job.gateID = 1;
    test_job.done = 1;
    test_job.priority = 1;
    int test_job_merge = set_jobs_entry(&test_job);
    test_job.gateID = 2;
    test_job.done = 0;
    test_job.priority = 2;
    test_job_merge = set_jobs_entry(&test_job);
    test_job.gateID = 3;
    test_job.done = 0;
    test_job.priority = 3;
    test_job_merge = set_jobs_entry(&test_job);
    test_job.gateID = 4;
    test_job.done = 0;
    test_job.priority = 2;
    test_job_merge = set_jobs_entry(&test_job);
    test_job.gateID = 5;
    test_job.done = 0;
    test_job.priority = 0;
    test_job_merge = set_jobs_entry(&test_job);
    test_job.gateID = 6;
    test_job.done = 0;
    test_job.priority = 3;
    test_job_merge = set_jobs_entry(&test_job);
    test_job.gateID = 7;
    test_job.done = 0;
    test_job.priority = 2;
    test_job_merge = set_jobs_entry(&test_job);
    printf("Test merge: %d\n", test_job_merge);

    timestamp_entry test_timestamp;
    test_timestamp.gateID = 1;
    test_timestamp.timestamp = 55000;
    test_timestamp.rssi = -50;
    int test_timestamp_merge = set_timestamp_entry(&test_timestamp);
    test_timestamp.gateID = 2;
    test_timestamp.timestamp = 60000;
    test_timestamp.rssi = -100;
    test_timestamp_merge = set_timestamp_entry(&test_timestamp);
    test_timestamp.gateID = 3;
    test_timestamp.timestamp = 66000;
    test_timestamp.rssi = 0;
    test_timestamp_merge = set_timestamp_entry(&test_timestamp);
    test_timestamp.gateID = 4;
    test_timestamp.timestamp = 70000;
    test_timestamp.rssi = 170;
    test_timestamp_merge = set_timestamp_entry(&test_timestamp);
    test_timestamp.gateID = 5;
    test_timestamp.timestamp = 8000;
    test_timestamp.rssi = -20;
    test_timestamp_merge = set_timestamp_entry(&test_timestamp);
    test_timestamp.gateID = 6;
    test_timestamp.timestamp = 9000;
    test_timestamp.rssi = -30;
    test_timestamp_merge = set_timestamp_entry(&test_timestamp);
    test_timestamp.gateID = 7;
    test_timestamp.timestamp = 10000;
    test_timestamp.rssi = 0;
    test_timestamp_merge = set_timestamp_entry(&test_timestamp);
    printf("Test merge: %d\n", test_timestamp_merge);
}