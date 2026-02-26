#define ESQLTEST_SQL_BUF_SIZE 1000
#define SECTION_NAME_SIZE 32

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
#include <ctype.h>
#include <stdbool.h>
#include "dbio.h"
#include "utilities.h"

static void test_capture_entries(int entry_count) {
    char** entries = NULL;
    int size = 0, capacity = 0;
    char buf[64];
    for (int i = 0; i < entry_count; ++i) {
        snprintf(buf, sizeof(buf), "RandomEntry_%d_%d", i, rand());
        capture_string(&entries, &size, &capacity, buf);
    }
    printf("Captured %d entries:\n", size);
    for (int i = 0; i < size; ++i) {
        printf("  [%02d] %s\n", i, entries[i]);
        free(entries[i]);
    }
    free(entries);
}

void process_transaction(const char* dbname, ini_file_t* ini, int txn_idx, bool use_persistent) {
    char section[SECTION_NAME_SIZE];
    snprintf(section, sizeof(section), "transaction%02d", txn_idx);

    char skip[8] = {0};
    ini_get_value(ini, section, "skip", skip, sizeof(skip), txn_idx);
    if (isTrue(skip)) {
        printf("Skipping transaction %d due to skip property.\n", txn_idx);
        return;
    }

    char sql[ESQLTEST_SQL_BUF_SIZE] = {0};
    if (ini_get_value(ini, section, "sql", sql, sizeof(sql), txn_idx) != 0) {
        return;
    }

    int sleep_seconds = ini_get_int_value(ini, section, "sleep", 0, txn_idx);
    int repeat_count = ini_get_int_value(ini, section, "repeat", 1, txn_idx);
    bool has_iteration = (repeat_count > 1);

    for (int iter = 0; iter < repeat_count; ++iter) {
        char expanded_sql[ESQLTEST_SQL_BUF_SIZE];
        strncpy(expanded_sql, sql, sizeof(expanded_sql) - 1);
        expanded_sql[sizeof(expanded_sql) - 1] = (char)'\\0';
        ini_get_value(ini, section, "sql", expanded_sql, sizeof(expanded_sql), iter);

        printf("Executing transaction %d%s: %s\n", txn_idx, has_iteration ? " (iteration %d)" : "", expanded_sql);
        clock_t start = clock();

        // Use persistent or regular connection based on configuration
        int result;
        if (use_persistent) {
            result = dbtest_transaction_persistent(dbname, expanded_sql, ini, section);
        } else {
            result = dbtest_transaction(dbname, expanded_sql, ini, section);
        }

        clock_t end = clock();
        double elapsed = (double)(end - start) / CLOCKS_PER_SEC;
        printf("Transaction %d%s returned: %d\n", txn_idx, has_iteration ? " (iteration %d)" : "", result);

        if (sleep_seconds > 0) {
            printf("Sleeping for %d seconds...\n", sleep_seconds);
            #ifdef _WIN32
            Sleep(sleep_seconds * 1000);
            #else
            sleep(sleep_seconds);
            #endif
        }
    }
}

typedef struct {
    int total_transactions;
    int successful_transactions;
    int skipped_transactions;
    const char* config_filename;
} status_t;

typedef struct {
    char section[SECTION_NAME_SIZE];
    char skip[8];
    char sql[ESQLTEST_SQL_BUF_SIZE];
    int sleep_seconds;
    int repeat_count;
    bool has_iteration;
    char expanded_sql[ESQLTEST_SQL_BUF_SIZE];
} transaction_t;

int main(int argc, char* argv[]) {
    status_t status = {0};
    status.config_filename = "config.ini";
    // Support named parameter: --config <filename>
    for (int i = 1; i < argc - 1; ++i) {
        if (strcmp(argv[i], "--config") == 0 && argv[i+1] && argv[i+1][0]) {
            status.config_filename = argv[i+1];
            continue;
        }
    }
    // Fallback: positional parameter
    if (argc > 1 && argv[1] && argv[1][0] && strcmp(argv[1], "--config") != 0) {
        status.config_filename = argv[1];
    }
    ini_file_t* ini = ini_load(status.config_filename);
    if (!ini) {
        printf("Failed to read INI file: %s\n", status.config_filename);
        return 1;
    }
    int entry_count = ini_get_int_value(ini, "general", "test_capture_entries_count", 0, 0);
    if (entry_count > 0) {
        test_capture_entries(entry_count);
    }
    char dbname[128] = {0};
    int ini_result = ini_get_value(ini, "general", "database", dbname, sizeof(dbname), 0);
    if (ini_result != 0) {
        printf("Failed to read database name from INI file.\n");
        ini_free(ini);
        return 1;
    }
    printf("Database name read from INI: %s\n", dbname);

    // Check for persistent connection configuration
    char persistent_conn[8] = {0};
    ini_get_value(ini, "general", "persistent_connection", persistent_conn, sizeof(persistent_conn), 0);
    bool use_persistent = isTrue(persistent_conn);

    printf("DEBUG: persistent_connection config value: '%s'\n", persistent_conn);
    printf("DEBUG: isTrue() result: %d\n", use_persistent ? 1 : 0);

    if (use_persistent) {
        printf("Using persistent connection mode.\n");
    } else {
        printf("Using regular connection mode (connect/disconnect per transaction).\n");
    }

    double total_time = 0.0;

    for (int txn_idx = 0; ; ++txn_idx) {
        if (!transaction_exists(ini, txn_idx)) break;

        transaction_t txn = {0};
        snprintf(txn.section, sizeof(txn.section), "transaction%02d", txn_idx);

        ini_get_value(ini, txn.section, "skip", txn.skip, sizeof(txn.skip), txn_idx);
        if (isTrue(txn.skip)) {
            ++status.skipped_transactions;
            printf("Skipping transaction %d due to skip property.\n", txn_idx);
            continue;
        }

        if (ini_get_value(ini, txn.section, "sql", txn.sql, sizeof(txn.sql), txn_idx) != 0) {
            continue;
        }

        txn.sleep_seconds = ini_get_int_value(ini, txn.section, "sleep", 0, txn_idx);
        txn.repeat_count = ini_get_int_value(ini, txn.section, "repeat", 1, txn_idx);
        txn.has_iteration = (txn.repeat_count > 1);

        for (int iter = 0; iter < txn.repeat_count; ++iter) {
            ++status.total_transactions;
            strncpy(txn.expanded_sql, txn.sql, sizeof(txn.expanded_sql) - 1);
            txn.expanded_sql[sizeof(txn.expanded_sql) - 1] = '\0';
            ini_get_value(ini, txn.section, "sql", txn.expanded_sql, sizeof(txn.expanded_sql), iter);

            if (txn.has_iteration) {
                printf("Executing transaction %d (iteration %d): %s\n", txn_idx, iter, txn.expanded_sql);
            } else {
                printf("Executing transaction %d: %s\n", txn_idx, txn.expanded_sql);
            }
            clock_t start = clock();

            // Use persistent or regular connection based on configuration
            int result;
            if (use_persistent) {
                result = dbtest_transaction_persistent(dbname, txn.expanded_sql, ini, txn.section);
            } else {
                result = dbtest_transaction(dbname, txn.expanded_sql, ini, txn.section);
            }

            clock_t end = clock();
            double elapsed = (double)(end - start) / CLOCKS_PER_SEC;
            total_time += elapsed;
            if (txn.has_iteration) {
                printf("Transaction %d (iteration %d) returned: %d\n", txn_idx, iter, result);
            } else {
                printf("Transaction %d returned: %d\n", txn_idx, result);
            }

            if (result == 0) {
                ++status.successful_transactions;
            }

            if (txn.sleep_seconds > 0) {
                printf("Sleeping for %d seconds...\n", txn.sleep_seconds);
                #ifdef _WIN32
                Sleep(txn.sleep_seconds * 1000);
                #else
                sleep(txn.sleep_seconds);
                #endif
            }
        }
    }

    // Clean up persistent connection if it was used
    if (use_persistent && dbio_is_connected()) {
        printf("Closing persistent database connection.\n");
        dbio_disconnect();
    }

    char summary[256];
    snprintf(summary, sizeof(summary),
        "%d transactions processed, successful: %d, skipped: %d, total TXN time: %.3fs, average per TXN: %.3fs",
        status.total_transactions, status.successful_transactions, status.skipped_transactions, total_time,
        (status.successful_transactions > 0 ? total_time/status.total_transactions : 0.0f));
    print_flowerbox(summary);
    ini_free(ini);
    return 0;
}