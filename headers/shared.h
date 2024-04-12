#ifndef COMMON_H
#define COMMON_H

typedef struct Shared_pointer Shared_pointer;
typedef Shared_pointer** Shared_data;

Shared_data create_shared_data(int array_size, int simulation_delay);
void free_shared_data(Shared_data data);

void set_has_quitted(Shared_data data, bool quitted);
bool has_quitted(Shared_data data);

void set_is_paused(Shared_data data, bool paused);
bool is_paused(Shared_data data);

void set_is_shuffling(Shared_data data, bool shuffling);
bool is_shuffling(Shared_data data);

void set_time(Shared_data data, unsigned int time);
unsigned int get_time(Shared_data data);

void set_corrected_time(Shared_data data, unsigned long time);
unsigned long get_corrected_time(Shared_data data);

void set_lps(Shared_data data, unsigned int lps);
unsigned int get_lps(Shared_data data);

void set_simulation_delay(Shared_data data, unsigned long delay);
unsigned long get_simulation_delay(Shared_data data);

void set_sort_function(Shared_data data, int shift);
unsigned int get_sort_function_index(Shared_data data);
const Sort_function* get_sort_function(Shared_data data);
short sort_function_init(Shared_data data);
short sort_function_sort(Shared_data data);
void sort_function_free(Shared_data data);

void lock_info(Shared_data data);
void unlock_info(Shared_data data);
Sort_info* lock_and_get_info(Shared_data data);
Sort_info* get_info(Shared_data data);
short reset_info(Shared_data data);

char* get_sort_name(Shared_data data);
char* get_sort_complexity(Shared_data data);
int get_array_len(Shared_data data);
void set_array_value(Shared_data data, int i, int value);
int get_array_value(Shared_data data, int i);
void set_cursor(Shared_data data, int cursor);
int get_cursor(Shared_data data);

#endif