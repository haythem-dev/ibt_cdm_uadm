#ifndef _DBIO_H_
#define _DBIO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "utilities.h"

// Executes a transaction SQL statement (e.g., INSERT) on the database
int dbtest_transaction(const char* dbname, const char* transaction_sql, ini_file_t* ini, const char* section);

#ifdef __cplusplus
}
#endif

#endif