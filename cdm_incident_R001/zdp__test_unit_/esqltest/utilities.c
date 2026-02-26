#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "utilities.h"
#include "macros.h"

// Add this include at the top of the file for getpid()
#if !defined(_WIN32) && !defined(_WIN64)
#include <unistd.h>
#else
// On Windows, use _getpid from <process.h>
#include <process.h>
#define getpid _getpid
#endif

// Appends a string to a dynamically allocated array of strings.
// *arr: pointer to the array of char* (may be NULL initially)
// *size: pointer to the current number of strings
// *capacity: pointer to the current capacity
// str: string to append (copied)
// Returns the new number of strings after append.
int capture_string(char*** arr, int* size, int* capacity, const char* str) {
    if (!arr || !size || !capacity || !str) return *size;
    if (*size >= *capacity) {
        int new_capacity = (*capacity > 0) ? (*capacity + (*capacity + 1) / 2) : 4;
        char** new_arr = (char**)realloc(*arr, new_capacity * sizeof(char*));
        if (!new_arr) return *size; // Allocation failed
        *arr = new_arr;
        *capacity = new_capacity;
    }
    (*arr)[*size] = _strdup(str);
    (*size)++;
    return *size;
}

// --- INI file loading and lookup ---

// Helper to trim whitespace
static void trim_whitespace(char* str) {
    char* end;
    while (*str == ' ' || *str == '\t') str++;
    end = str + strlen(str) - 1;
    while (end > str && (*end == '\n' || *end == '\r' || *end == ' ' || *end == '\t')) {
        *end = '\0';
        end--;
    }
}

// Load INI file into memory
ini_file_t* ini_load(const char* ini_path) {
    FILE* file = fopen(ini_path, "r");
    if (!file) return NULL;
    ini_file_t* ini = (ini_file_t*)calloc(1, sizeof(ini_file_t));
    ini_section_t* current_section = NULL;
    char line[512];
    while (fgets(line, sizeof(line), file)) {
        char* start = line;
        trim_whitespace(start);
        if (*start == '[') {
            char* close_bracket = strchr(start, ']');
            if (close_bracket) {
                size_t len = close_bracket - (start + 1);
                char* section_name = (char*)malloc(len + 1);
                strncpy(section_name, start + 1, len);
                section_name[len] = '\0';
                ini_section_t* section = (ini_section_t*)calloc(1, sizeof(ini_section_t));
                section->name = section_name;
                section->next = ini->sections;
                ini->sections = section;
                current_section = section;
            }
            continue;
        }
        if (!current_section || !*start || *start == ';' || *start == '#') continue;
        char* equals = strchr(start, '=');
        if (equals) {
            size_t key_len = equals - start;
            char* key = (char*)malloc(key_len + 1);
            strncpy(key, start, key_len);
            key[key_len] = '\0';
            char* value = equals + 1;
            while (*value == ' ' || *value == '\t') value++;
            // Handle continuation lines
            char value_buf[1024] = "";
            size_t total_len = 0;
            do {
                size_t vlen = strlen(value);
                while (vlen > 0 && (value[vlen - 1] == ' ' || value[vlen - 1] == '\t')) {
                    value[--vlen] = '\0';
                }
                int has_continuation = (vlen > 0 && value[vlen - 1] == '\\');
                if (has_continuation) value[vlen - 1] = '\0';
                size_t copy_len = strlen(value);
                if (total_len + copy_len < sizeof(value_buf) - 1) {
                    strcat(value_buf, value);
                    total_len += copy_len;
                }
                if (has_continuation) {
                    if (!fgets(line, sizeof(line), file)) break;
                    value = line;
                    trim_whitespace(value);
                } else {
                    break;
                }
            } while (1);
            value_buf[sizeof(value_buf) - 1] = '\0';
            ini_kv_t* kv = (ini_kv_t*)calloc(1, sizeof(ini_kv_t));
            kv->key = key;
            kv->value = _strdup(value_buf);
            kv->next = current_section->kv_list;
            current_section->kv_list = kv;
        }
    }
    fclose(file);
    return ini;
}

// Get value from loaded INI structure
int ini_get_value(ini_file_t* ini, const char* section, const char* key, char* out_value, size_t out_value_size, int iteration) {
    if (!ini || !section || !key || !out_value) return 1;
    ini_section_t* sec = ini->sections;
    while (sec) {
        if (strcmp(sec->name, section) == 0) {
            ini_kv_t* kv = sec->kv_list;
            while (kv) {
                if (strcmp(kv->key, key) == 0) {
                    strncpy(out_value, kv->value, out_value_size - 1);
                    out_value[out_value_size - 1] = '\0';
                    expand_macros(out_value, out_value_size, iteration);
                    return 0;
                }
                kv = kv->next;
            }
        }
        sec = sec->next;
    }
    return 2;
}

// Get integer value from loaded INI structure, with default fallback
int ini_get_int_value(ini_file_t* ini, const char* section, const char* key, int default_value, int iteration) {
    char value_buf[32] = {0};
    if (ini_get_value(ini, section, key, value_buf, sizeof(value_buf), iteration) == 0) {
        char* endptr = NULL;
        long val = strtol(value_buf, &endptr, 10);
        if (endptr != value_buf && *endptr == '\0') {
            return (int)val;
        }
    }
    return default_value;
}

// Free loaded INI structure
void ini_free(ini_file_t* ini) {
    if (!ini) return;
    ini_section_t* sec = ini->sections;
    while (sec) {
        ini_section_t* sec_next = sec->next;
        ini_kv_t* kv = sec->kv_list;
        while (kv) {
            ini_kv_t* kv_next = kv->next;
            free(kv->key);
            free(kv->value);
            free(kv);
            kv = kv_next;
        }
        free(sec->name);
        free(sec);
        sec = sec_next;
    }
    free(ini);
}

// Start the stopwatch
void stopwatch_start(stopwatch_t* sw) {
    if (sw) {
        sw->start_time = clock();
        sw->running = 1;
    }
}

// Stop the stopwatch
double stopwatch_stop(stopwatch_t* sw) {
	double elapsed_time = 0.0;

    if (sw && sw->running) {
        sw->end_time = clock();
        sw->running = 0;
        elapsed_time = (double)(sw->end_time - sw->start_time) / CLOCKS_PER_SEC;
    }

    return elapsed_time;
}

// Prints a flowerbox around a string
void print_flowerbox(const char* str) {
    if (!str) return;
    size_t len = strlen(str);
    // The box width includes 2 spaces and 2 vertical bars
    size_t box_width = len + 4;
    // Top border
    printf("+%.*s+\n", (int)box_width - 2, "--------------------------------------------------"
    "--------------------------------------------------"
    "--------------------------------------------------");
    // Content
    printf("| %s |\n", str);
    // Bottom border
    printf("+%.*s+\n", (int)box_width - 2, "--------------------------------------------------"
    "--------------------------------------------------"
    "--------------------------------------------------");
}

// Helper to trim whitespace from both ends
static char* trim(char* s) {
    char* end;
    while (*s == ' ' || *s == '\t') s++;
    end = s + strlen(s) - 1;
    while (end > s && (*end == ' ' || *end == '\t')) { *end = '\0'; end--; }
    return s;
}

// Draws a formatted table from a string array and header string, using the given separator
void draw_table(const char** rows, int row_count, const char* header, char separator, int limit) {
    // Parse header fields
    char header_buf[1024];
    strncpy(header_buf, header, sizeof(header_buf) - 1);
    header_buf[sizeof(header_buf) - 1] = '\0';
    int col_count = 0;
    char* col_ptrs[32];
    int col_widths[32] = {0};
    char* token = strtok(header_buf, &separator);
    while (token && col_count < 32) {
        char* trimmed = trim(token);
        col_ptrs[col_count] = trimmed;
        col_widths[col_count] = (int)strlen(trimmed);
        col_count++;
        token = strtok(NULL, &separator);
    }
    // Find max width for each column from data
    int max_row = (limit > 0 && limit < row_count) ? limit : row_count;
    for (int r = 0; r < max_row; ++r) {
        char row_buf[1024];
        strncpy(row_buf, rows[r], sizeof(row_buf) - 1);
        row_buf[sizeof(row_buf) - 1] = '\0';
        int c = 0;
        char* tok = strtok(row_buf, &separator);
        while (tok && c < col_count) {
            char* trimmed = trim(tok);
            int w = (int)strlen(trimmed);
            if (w > col_widths[c]) col_widths[c] = w;
            c++;
            tok = strtok(NULL, &separator);
        }
    }
    // Print header
    for (int c = 0; c < col_count; ++c) {
        printf("%s%s", (c == 0 ? "" : " | "), col_ptrs[c]);
        int pad = col_widths[c] - (int)strlen(col_ptrs[c]);
        for (int p = 0; p < pad; ++p) putchar(' ');
    }
    printf(" |\n");
    // Print separator line (aligned)
    for (int c = 0; c < col_count; ++c) {
        if (c == 0) {
            printf("-");
        } else {
            printf("+");
        }
        for (int p = 0; p < col_widths[c]; ++p) putchar('-');
        printf("-"); // for the space after each column
    }
    printf("+\n");
    // Print rows
    for (int r = 0; r < max_row; ++r) {
        char row_buf[1024];
        strncpy(row_buf, rows[r], sizeof(row_buf) - 1);
        row_buf[sizeof(row_buf) - 1] = '\0';
        int c = 0;
        char* tok = strtok(row_buf, &separator);
        while (tok && c < col_count) {
            char* trimmed = trim(tok);
            printf("%s%s", (c == 0 ? "" : " | "), trimmed);
            int pad = col_widths[c] - (int)strlen(trimmed);
            for (int p = 0; p < pad; ++p) putchar(' ');
            c++;
            tok = strtok(NULL, &separator);
        }
        // Fill empty columns if any
        for (; c < col_count; ++c) {
            printf("%s", (c == 0 ? "" : " | "));
            for (int p = 0; p < col_widths[c]; ++p) putchar(' ');
        }
        printf(" |\n");
    }
}

// Returns 1 if the string represents a true value, 0 otherwise
int isTrue(const char* str) {
    if (!str) return 0;
    // Trim leading/trailing whitespace
    while (*str == ' ' || *str == '\t') str++;
    size_t len = strlen(str);
    while (len > 0 && (str[len - 1] == ' ' || str[len - 1] == '\t')) len--;
    if (len == 0) return 0;

    // Copy and lowercase the string
    char buf[16];
    size_t copy_len = len < sizeof(buf) - 1 ? len : sizeof(buf) - 1;
    for (size_t i = 0; i < copy_len; ++i)
        buf[i] = (char)tolower((unsigned char)str[i]);
    buf[copy_len] = '\0';

    // Check against allowed values
    const char* allowed[] = {"true", "1", "yes", "ja", "on", "oui"};
    for (size_t i = 0; i < sizeof(allowed)/sizeof(allowed[0]); ++i) {
        if (strcmp(buf, allowed[i]) == 0) return 1;
    }
    return 0;
}

bool transaction_exists(ini_file_t* ini, int txn_idx) {
    char section[SECTION_NAME_SIZE];
    snprintf(section, sizeof(section), "transaction%02d", txn_idx);
    ini_section_t* sec = ini->sections;
    while (sec) {
        if (strcmp(sec->name, section) == 0) {
            return true;
        }
        sec = sec->next;
    }
    return false;
}