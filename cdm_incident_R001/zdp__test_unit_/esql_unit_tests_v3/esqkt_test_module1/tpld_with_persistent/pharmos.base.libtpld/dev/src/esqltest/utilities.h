#pragma once
#include <time.h>

#define SECTION_NAME_SIZE 32

#ifdef __cplusplus
extern "C" {
#endif

// Reads a value from an INI file given the file path, section, and key.
// The value is written to the output parameter 'out_value' (as a string).
// Returns 0 on success, non-zero on failure.
int read_ini_value(const char* ini_path, const char* section, const char* key, char* out_value, size_t out_value_size);

// --- INI file in-memory structure ---
typedef struct ini_kv_t {
    char* key;
    char* value;
    struct ini_kv_t* next;
} ini_kv_t;

typedef struct ini_section_t {
    char* name;
    ini_kv_t* kv_list;
    struct ini_section_t* next;
} ini_section_t;

typedef struct ini_file_t {
    ini_section_t* sections;
} ini_file_t;

ini_file_t* ini_load(const char* ini_path);
int ini_get_value(ini_file_t* ini, const char* section, const char* key, char* out_value, size_t out_value_size, int iteration);
int ini_get_int_value(ini_file_t* ini, const char* section, const char* key, int default_value, int iteration);
void ini_free(ini_file_t* ini);

void print_flowerbox(const char* str);

// Simple stopwatch structure
typedef struct {
    clock_t start_time;
    clock_t end_time;
    int running;
} stopwatch_t;

// Start the stopwatch
void stopwatch_start(stopwatch_t* sw);

// Stop the stopwatch
double stopwatch_stop(stopwatch_t* sw);

// Appends a string to a dynamically allocated array of strings.
// *arr: pointer to the array of char* (may be NULL initially)
// *size: pointer to the current number of strings
// *capacity: pointer to the current capacity
// str: string to append (copied)
// Returns the new number of strings after append.
int capture_string(char*** arr, int* size, int* capacity, const char* str);

// Draws a formatted table from a string array and header string, using the given separator
void draw_table(const char** rows, int row_count, const char* header, char separator, int limit);

int isTrue(const char* str);

// Add this declaration
bool transaction_exists(ini_file_t* ini, int txn_idx);

#ifdef __cplusplus
}
#endif
