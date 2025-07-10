#ifndef _MY_DISPLAY_DEMO_H
#define _MY_DISPLAY_DEMO_H
#include <stdbool.h>

void display_demo(void);

void init_display(void);

void display_menu(char *entry01 /*, int entry01_len*/, char *entry02/*, int entry02_len*/, bool aboveMore, bool belowMore, int selected);

void display_gate_menu_box(char* text, int num_after_text, bool upper, bool selected, bool gate_state_open, bool target_match, int job_prio, bool more_content);

void display_ordinary_menu(char *text, int num_after_text, bool use_num, bool upper, bool selected, bool more_content);

void display_menu_header(char *text, int num_after_text, bool use_num);

void new_page(void);

bool next_page(void);

#endif
