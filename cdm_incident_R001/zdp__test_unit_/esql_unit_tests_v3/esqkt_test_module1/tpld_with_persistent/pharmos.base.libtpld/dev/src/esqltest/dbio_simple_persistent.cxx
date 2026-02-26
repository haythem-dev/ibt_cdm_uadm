#define DBIO_CONN_STR_SIZE 250
#define DBIO_SQL_STMT_SIZE 1000
#define DBIO_RESULT_SIZE 1024
#define DBIO_HEADER_SIZE 256

exec sql include sqlca;
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utilities.h"

#ifdef __cplusplus
extern "C" {
#endif

// Simple persistent connection state
static int g_persistent_active = 0;

// Internal function to print SQL error details
static void print_sql_error(const char* context) {
    int msg_len;
    char errmsg[400];

    rgetlmsg(sqlca.sqlcode, errmsg, sizeof(errmsg), &msg_len);

    // Truncate trailing newline if present
    if (msg_len > 0 && (errmsg[msg_len - 1] == '\n' || errmsg[msg_len - 1] == '\r')) {
        errmsg[msg_len - 1] = '\0';
        msg_len--;
    }

    printf("%s failed: SQLCODE = %ld [%s].\n",
        context, sqlca.sqlcode, errmsg);
}

// Simple persistent connection control
int dbio_start_persistent_session(const char* dbname) {
    if (g_persistent_active) {
        return 0; // Already active
    }
    
    exec sql begin declare section;
    char conn_str[DBIO_CONN_STR_SIZE];
    exec sql end declare section;
    
    snprintf(conn_str, sizeof(conn_str), "%s", dbname);
    
    exec sql connect to :conn_str;
    if (sqlca.sqlcode != 0) {
        print_sql_error("Persistent Session Connect");
        return 1;
    }
    
    g_persistent_active = 1;
    printf("Persistent session started for: %s\n", dbname);
    return 0;
}

int dbio_end_persistent_session(void) {
    if (!g_persistent_active) {
        return 0; // Already inactive
    }
    
    exec sql disconnect current;
    if (sqlca.sqlcode != 0) {
        print_sql_error("Persistent Session Disconnect");
    } else {
        printf("Persistent session ended.\n");
    }
    
    g_persistent_active = 0;
    return 0;
}

int dbio_is_persistent_active(void) {
    return g_persistent_active;
}

// Modified transaction function that respects persistent mode
int dbtest_transaction_smart(const char* dbname, const char* transaction_sql, ini_file_t* ini, const char* section) {
    exec sql begin declare section;
    char conn_str[DBIO_CONN_STR_SIZE];
    char sql_stmt[DBIO_SQL_STMT_SIZE];
    char result[DBIO_RESULT_SIZE];
    exec sql end declare section;

    snprintf(conn_str, sizeof(conn_str), "%s", dbname);
    snprintf(sql_stmt, sizeof(sql_stmt), "%s", transaction_sql);

    // Connect only if not in persistent mode
    if (!g_persistent_active) {
        exec sql connect to :conn_str;
        if (sqlca.sqlcode != 0) {
            print_sql_error("Connection");
            return 1;
        } else {
            printf("Connection successful.\n");
        }
    } else {
        printf("Using persistent connection.\n");
    }

    exec sql begin work;

    stopwatch_t sw;
    stopwatch_start(&sw);

    char header[DBIO_HEADER_SIZE] = {0};
    ini_get_value(ini, section, "header", header, sizeof(header), 0);
    int has_header = (header[0] != '\0');
    if (has_header) {
        char** results = NULL;
        int size = 0, capacity = 0;
        char separator_str[8] = {0};
        char separator = ';';
        // Prefer transaction-specific separator over general
        ini_get_value(ini, section, "separator", separator_str, sizeof(separator_str), 0);
        if (!separator_str[0]) {
            ini_get_value(ini, "general", "separator", separator_str, sizeof(separator_str), 0);
        }
        if (separator_str[0]) separator = separator_str[0];
        exec sql prepare smart_stmt from :sql_stmt;
        exec sql declare smart_cursor cursor for smart_stmt;
        exec sql open smart_cursor;
        if (sqlca.sqlcode != 0) {
            print_sql_error("Open cursor");
            exec sql free smart_stmt;
            if (!g_persistent_active) {
                exec sql disconnect current;
            }
            return 2;
        }
        while (1) {
            exec sql fetch smart_cursor into :result;
            if (sqlca.sqlcode != 0) break;
            capture_string(&results, &size, &capacity, result);
        }
        exec sql close smart_cursor;
        exec sql free smart_stmt;
        int limit = ini_get_int_value(ini, section, "limit", 0, 0);
        draw_table((const char**)results, size, header, separator, limit);
        printf("Returned %d row(s) from SELECT.\n", size);
        for (int i = 0; i < size; ++i) {
            free(results[i]);
        }
        free(results);
    } else {
        exec sql execute immediate :sql_stmt;
        if (sqlca.sqlcode != 0) {
            print_sql_error("Transaction execution");
            if (!g_persistent_active) {
                exec sql disconnect current;
            }
            return 2;
        } else {
            printf("Transaction executed successfully.");
        }
    }

    exec sql commit work;
    if (sqlca.sqlcode != 0) {
        print_sql_error("Commit");
        if (!g_persistent_active) {
            exec sql disconnect current;
        }
        return 3;
    } else {
        printf("..Transaction committed successfully.");
    }

    printf("..Execution time: %.3f seconds\n", stopwatch_stop(&sw));

    // Disconnect only if not in persistent mode
    if (!g_persistent_active) {
        exec sql disconnect current;
        if (sqlca.sqlcode != 0) {
            print_sql_error("Disconnect");
            return 4;
        } else {
            printf("Disconnected successfully.\n");
        }
    }

    return 0;
}

#ifdef __cplusplus
}
#endif