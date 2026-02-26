/****************************************************************************
 *
 *                               IBM INC.
 *
 *                           PROPRIETARY DATA
 *
 * Licensed Material - Property Of IBM
 *
 * "Restricted Materials of IBM"
 *
 * IBM Informix Client SDK
 *
 * (c)  Copyright IBM Corporation 2002,2007. All rights reserved.
 *
 *  Title:	sqlca.h
 *  Description:
 *		SQL Control Area
 *
 ***************************************************************************
 */

#ifndef SQLCA_INCL
#define SQLCA_INCL

#ifdef __BORLANDC__
#pragma option -a8
#else
#pragma pack (8)
#endif

#ifdef STATIC
#include "ifxdef.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include "ifxtypes.h"

typedef struct sqlca_s
{
    int4 sqlcode;
#ifdef DB2CLI
    char sqlerrm[600]; /* error message parameters */
#else /* DB2CLI */
    char sqlerrm[72]; /* error message parameters */
#endif /* DB2CLI */
    char sqlerrp[8];
    int4 sqlerrd[6];
		    /* 0 - estimated number of rows returned */
		    /* 1 - serial value after insert or  ISAM error code */
		    /* 2 - number of rows processed */
		    /* 3 - estimated cost */
		    /* 4 - offset of the error into the SQL statement */
		    /* 5 - rowid after insert  */
#ifdef _FGL_
    char sqlawarn[8];
#else
    struct sqlcaw_s
	{
	char sqlwarn0; /* = W if any of sqlwarn[1-7] = W */
	char sqlwarn1; /* = W if any truncation occurred or
				database has transactions or
			        no privileges revoked */
	char sqlwarn2; /* = W if a null value returned or
				ANSI database */ 
	char sqlwarn3; /* = W if no. in select list != no. in into list or
				turbo backend or no privileges granted */
	char sqlwarn4; /* = W if no where clause on prepared update, delete or
				incompatible float format */
	char sqlwarn5; /* = W if non-ANSI statement */
	char sqlwarn6; /* = W if server is in data replication secondary mode */
	char sqlwarn7; /* = W if database locale is different from proc_locale
			*/
	} sqlwarn;
#endif
    } ifx_sqlca_t;

#define SQLNOTFOUND 100

#ifdef DB2CLI
#define sqlca ifmxsqlca
extern struct sqlca_s sqlca;
#else /* DB2CLI */
extern struct sqlca_s sqlca;
#endif /* DB2CLI */

#ifdef __cplusplus
}
#endif

#ifdef __BORLANDC__
#pragma option -a-
#else
#pragma pack ()
#endif

#endif /* SQLCA_INCL */
