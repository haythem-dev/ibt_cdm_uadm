/*****************************************************************************/
/*                                                                           */
/*   header     : DadeDb.h                                                   */
/*   Author     : A. Schmidt-Rehschuh (644)                                  */
/*   made       : 01.09.97                                                   */
/*   language   : C/Esql                                                     */
/*   Revised    : Haythem Ben Abdelaziz 08.25                                */
/*   declaration: standard-functions for database calls. made for dade       */
/*                                                                           */
/*****************************************************************************/

#ifndef __DADEDB__1234
#define __DADEDB__1234

/*** Include *****************************************************************/

#include "cprztype.h"
#include <stdlib.h>
#include <string.h>

/*** Type-Declaration ********************************************************/

typedef enum { 
    DIRTY_READ = 0, 
    COMMITTED_READ, 
    CURSOR_STABILITY, 
    REPEATABLE_READ 
} sqlread;

typedef enum { 
    IS_NOTUPDATED = -3, 
    IS_WARNING = -2, 
    IS_ERROR, 
    IS_OK, 
    IS_NOTFOUND,
    IS_FETCH_INTERRUPT, 
    IS_FASTFETCH, 
    IS_LOCKED,
    IS_EXISTS 
} retClient;

typedef enum { 
    SET_CLOSE = 0, 
    SET_FETCH, 
    SET_OPEN, 
    SET_MIRROR 
} fHandle;

/* Named struct to allow member functions in C++ */
#if defined (__cplusplus) || defined (__CPLUS__)
struct ErrorInfo {
    char spError[81];   /* errormessage */
    long lUpdateNo;     /* number of columns of update/delete/insert */
    long lSqlcode;      /* sqlca.sqlcode */
    long lSqlcisam;     /* sqlca.sqlcisam[1] */
    char spErrm[48];    /* sqlca.sqlerrm */
    
    void Init()
    {
        spError[0] = '\0';
        lUpdateNo = 0;
        lSqlcode = 0;    
        lSqlcisam = 0;
        spErrm[0] = '\0';
    }
};
typedef struct ErrorInfo tError;
#else
/* C version - plain struct without member functions */
typedef struct {
    char spError[81];   /* errormessage */
    long lUpdateNo;     /* number of columns of update/delete/insert */
    long lSqlcode;      /* sqlca.sqlcode */
    long lSqlcisam;     /* sqlca.sqlcisam[1] */
    char spErrm[48];    /* sqlca.sqlerrm */
} tError;
#endif

/*** SQL-Standard-Define *****************************************************/

#if !defined (__S390IBM__) && !defined (__ORACLE__)
#define SQLNOTFOUND 100
#define SQLEXISTS   -239
#endif

#if defined (__S390IBM__)
#define SQLNOTFOUND 100
#define SQLEXISTS   -239
#endif

#if defined (__ORACLE__)
#define SQLNOTFOUND 1403
#define SQLEXISTS   1403   /* Oracle uses same code for both */
#endif

/*** Function Declarations ***************************************************/

#if defined (__cplusplus) || defined (__CPLUS__)
extern "C" {
#endif

/* Database connection functions - platform specific */
#if !defined (__S390IBM__) && !defined (__ORACLE__)
    retClient OpenDb(const char *spDatabase, tError *stError);
    retClient CloseDb(tError *stError);
    retClient ConnectDb(const char *spDatabase, tError *stError);
#endif

#ifdef __S390IBM__
    retClient OpenDb(const char *spDatabase, char *spSsid, tError *stError);
    retClient CloseDb(const char *spDatabase, char *spSsid, tError *stError);
    retClient ConnectDb(const char *spDatabase, char *spSsid, tError *stError);
#endif

#ifdef __ORACLE__
    retClient ConnectDb(const char *spDatabase, const char *spUser, 
                       const char *spPassword, tError *stError);
#endif

/* Transaction and session management functions */
retClient SetIsolation(sqlread iSqlRead, tError *stError);
retClient SetLockMode(int iSec, tError *stError);
retClient SetExplainOn(tError *stError);
retClient SetExplainOff(tError *stError);
retClient Commit(char *spFun, tError *stError);
retClient Begin(char *spFun, tError *stError);
retClient Rollback(char *spFun, tError *stError);

/* Utility functions */
retClient GetNumberOfBranch(void *vBranchNo, dataTyp d, tError *stError);
retClient CheckSql(char *spFun, tError *stError, char *spForm, ...);
int GetValueOfStream(char *spBuf, void *vValue, int iLen, dataTyp dTyp);
int GetValueOfStreamLen(char *spBuf, void *vValue, int iLen, int iByte,
                       dataTyp dTyp, int iDec);

#if defined (__cplusplus) || defined (__CPLUS__)
}
#endif

/* C++ helper function for tError initialization */
#if defined (__cplusplus) || defined (__CPLUS__)
inline void InitError(tError *stError)
{
    if (stError) {
        stError->Init();
    }
}
#endif

#endif /* __DADEDB__1234 */