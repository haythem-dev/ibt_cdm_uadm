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
 * (c)  Copyright IBM Corporation 2002. All rights reserved.
 *
 *  Title:	sqlda.h
 *  Description:
 *		SQL Data Description Area
 *
 ***************************************************************************
 */


#ifndef _SQLDA
#define _SQLDA

#ifdef __BORLANDC__
#pragma option -a8
#else
#pragma pack (8)
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include "ifxtypes.h"

typedef struct sqlvar_struct
    {
    int2 sqltype;		/* variable type		*/
    int4 sqllen;		/* length in bytes		*/
    char *sqldata;		/* pointer to data		*/
    int2 *sqlind;		/* pointer to indicator	*/
    char  *sqlname;		/* variable name		*/
    char  *sqlformat;	/* reserved for future use 	*/
    int2 sqlitype;		/* ind variable type		*/
    int2 sqlilen;		/* ind length in bytes		*/
    char *sqlidata;		/* ind data pointer		*/
    int4  sqlxid;       /* extended id type             */
    char *sqltypename;  /* extended type name           */
    int2 sqltypelen;   /* length of extended type name */
    int2 sqlownerlen;  /* length of owner name         */
    int2 sqlsourcetype;/* source type for distinct of built-ins */
    char *sqlownername; /* owner name                   */
    int4 sqlsourceid;	/* extended id of source type   */

    /*
     * sqlilongdata is new.  It supports data that exceeds the 32k
     * limit.  sqlilen and sqlidata are for backward compatibility
     * and they have maximum value of <32K.
     */
    char *sqlilongdata;	/* for data field beyond 32K	*/
    int4 sqlflags;		/* for internal use only        */
    void *sqlreserved;	/* reserved for future use      */
    } ifx_sqlvar_t;

typedef struct sqlda
    {
    int2 sqld;
    ifx_sqlvar_t *sqlvar;
    char desc_name[19];	/* descriptor name 			*/
    int2 desc_occ;		/* size of sqlda structure 	*/
    struct sqlda *desc_next;	/* pointer to next sqlda struct */
    void *reserved;		/* reserved for future use	*/
    } ifx_sqlda_t;

#ifdef __cplusplus
}
#endif

#ifdef __BORLANDC__
#pragma option -a-
#else
#pragma pack ()
#endif

#endif /* _SQLDA */
