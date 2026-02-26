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

// Function declarations
static void print_sql_error(const char* context);
int dbio_connect(const char* dbname);
int dbio_disconnect(void);
int dbio_is_connected(void);

// Global connection state management
static int g_is_connected = 0;
static char g_current_dbname[DBIO_CONN_STR_SIZE] = {0};

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

// Connection management functions
int dbio_connect(const char* dbname) {
    if (g_is_connected && strcmp(g_current_dbname, dbname) == 0) {
        return 0; // Already connected to the same database
    }
    
    if (g_is_connected) {
        dbio_disconnect(); // Disconnect from current database first
    }
    
    exec sql begin declare section;
    char conn_str[DBIO_CONN_STR_SIZE];
    exec sql end declare section;
    
    snprintf(conn_str, sizeof(conn_str), "%s", dbname);
    
    exec sql connect to :conn_str;
    if (sqlca.sqlcode != 0) {
        print_sql_error("Persistent Connection");
        g_is_connected = 0;
        return 1;
    }
    
    g_is_connected = 1;
    strncpy(g_current_dbname, dbname, sizeof(g_current_dbname) - 1);
    g_current_dbname[sizeof(g_current_dbname) - 1] = '\0';
    printf("Persistent connection established to: %s\n", dbname);
    return 0;
}

int dbio_disconnect(void) {
    if (!g_is_connected) {
        return 0; // Already disconnected
    }
    
    exec sql disconnect current;
    if (sqlca.sqlcode != 0) {
        print_sql_error("Persistent Disconnect");
        // Reset state even if disconnect failed
        g_is_connected = 0;
        g_current_dbname[0] = '\0';
        return 1;
    }
    
    printf("Persistent connection closed.\n");
    g_is_connected = 0;
    g_current_dbname[0] = '\0';
    return 0;
}

int dbio_is_connected(void) {
    return g_is_connected;
}

// New function: executes a transaction SQL statement (e.g., INSERT) on the database
// and measures the time taken to execute the transaction using the stopwatch.
int dbtest_transaction(const char* dbname, const char* transaction_sql, ini_file_t* ini, const char* section) {
    exec sql begin declare section;
    char conn_str[DBIO_CONN_STR_SIZE];
    char sql_stmt[DBIO_SQL_STMT_SIZE];
    char result[DBIO_RESULT_SIZE];
    exec sql end declare section;

    snprintf(conn_str, sizeof(conn_str), "%s", dbname);
    snprintf(sql_stmt, sizeof(sql_stmt), "%s", transaction_sql);

    exec sql connect to :conn_str;
    if (sqlca.sqlcode != 0) {
        print_sql_error("Connection");
        return 1;
    } else {
        printf("Connection successful.\n");
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
        exec sql prepare s1 from :sql_stmt;
        exec sql declare c cursor for s1;
        exec sql open c;
        if (sqlca.sqlcode != 0) {
            print_sql_error("Open cursor");
            exec sql free s1;
            exec sql disconnect current;
            return 2;
        }
        while (1) {
            exec sql fetch c into :result;
            if (sqlca.sqlcode != 0) break;
            capture_string(&results, &size, &capacity, result);
        }
        exec sql close c;
        exec sql free s1;
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
            exec sql disconnect current;
            return 2;
        } else {
            printf("Transaction executed successfully.");
        }
    }

    exec sql commit work;
    if (sqlca.sqlcode != 0) {
        print_sql_error("Commit");
        exec sql disconnect current;
        return 3;
    } else {
        printf("..Transaction committed successfully.");
    }

    printf("..Execution time: %.3f seconds\n", stopwatch_stop(&sw));

    exec sql disconnect current;
    if (sqlca.sqlcode != 0) {
        print_sql_error("Disconnect");
        return 4;
    } else {
        printf("Disconnected successfully.\n");
    }

    return 0;
}

// New persistent connection version - no connect/disconnect per transaction
int dbtest_transaction_persistent(const char* dbname, const char* transaction_sql, ini_file_t* ini, const char* section) {
    exec sql begin declare section;
    char sql_stmt[DBIO_SQL_STMT_SIZE];
    char result[DBIO_RESULT_SIZE];
    exec sql end declare section;

    snprintf(sql_stmt, sizeof(sql_stmt), "%s", transaction_sql);

    // Ensure connection exists (but don't reconnect if already connected)
    if (!g_is_connected) {
        if (dbio_connect(dbname) != 0) {
            return 1;
        }
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
        exec sql prepare stmt_persistent from :sql_stmt;
        exec sql declare cursor_persistent cursor for stmt_persistent;
        exec sql open cursor_persistent;
        if (sqlca.sqlcode != 0) {
            print_sql_error("Open cursor");
            exec sql free stmt_persistent;
            return 2;
        }
        while (1) {
            exec sql fetch cursor_persistent into :result;
            if (sqlca.sqlcode != 0) break;
            capture_string(&results, &size, &capacity, result);
        }
        exec sql close cursor_persistent;
        exec sql free stmt_persistent;
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
            return 2;
        } else {
            printf("Transaction executed successfully.");
        }
    }

    exec sql commit work;
    if (sqlca.sqlcode != 0) {
        print_sql_error("Commit");
        return 3;
    } else {
        printf("..Transaction committed successfully.");
    }

    printf("..Execution time: %.3f seconds\n", stopwatch_stop(&sw));

    // NO DISCONNECT - Connection remains open for next transaction

    return 0;
}

#ifdef __cplusplus
}
#endif