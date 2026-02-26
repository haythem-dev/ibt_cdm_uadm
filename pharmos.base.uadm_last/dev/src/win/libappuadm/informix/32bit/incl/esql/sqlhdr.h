/****************************************************************************
 *
 * Licensed Material - Property Of IBM
 *
 * "Restricted Materials of IBM"
 *
 * IBM Informix Client SDK
 *
 * Copyright IBM Corporation 2005, 2011.
 *
 * Title:       sqlhdr.h
 * Description: header file for all embedded sql programs
 *
 ***************************************************************************
 */

#ifndef _SQLHDR
#define _SQLHDR

#ifdef __cplusplus
extern "C" {
#endif

#include "ifxtypes.h"

#include "sqlda.h"
#include "sqlca.h"
#include "collct.h"
#include "int8.h"
#include "locator.h"
#include "datetime.h"
#include "value.h"
#include "sqlproto.h"

#ifdef __BORLANDC__
#pragma option -a8
#else
#pragma pack (8)
#endif

#if !defined(_WIN32) && !defined(_WIN64) && !defined(__STDC__) && !defined(__cplusplus)
#define const 
#endif

typedef struct _ifx_squlist
    {
    char **_SQUulist;			/* columns to be updated */
    struct _squlist *_SQUnext;	/* next update list */
    mint _SQUid;					/* update list id */
    } ifx_squlist_t;

typedef struct _ifx_cursor
    {
    struct _ifx_cursor  *_SQCcursptr;   /* pointer to the statement/cursor
                                         * block.
                                         */
    int2 _SQCstmttype;			/* SQ_SELECT, SQ_INSERT, etc. */
    int2 _SQCsqlid;			/* SQL's id for this cursor */
    int2 _SQCnfields;			/* number of result fields
					 * (number supplied by SQL)
					 */
    int2 _SQCnibind;			/* number of input args */
    int2 _SQCnobind;			/* number of output args */
    int2 _SQCnrettype;			/* number of fields sent for the
					 * rettype msg
					 */
    int4 _SQCtcount;			/* tuples remaining in buffer */
    int4 _SQCtsize;			/* length of data expected from
					 * SQL
					 */
    int4 _SQCtbsize;			/* tuple buffer size */
    int4 _SQCflags;			/* CROPEN, CREOF, CRSINGLE ,CRPREPARE*/
		/* used for scroll cursors */
    int4 _SQCfirst;			/* rowid of 1st tuple in buffer */
    int4 _SQClast;			/* rowid of last tuple in buffer */

    ifx_sqlvar_t *_SQCibind;	/* pointer to first in array of 
					 * binding structures for arguments
					 * to be taken from the user
					 * program and sent to SQL;
					 */
    ifx_sqlvar_t *_SQCobind;	/* pointer to first in array of
					 * binding structures for values
					 * to be received from SQL and
					 * supplied to the user program;
					 */
    ifx_sqlvar_t *_SQCrettype;	/* pointer to first in array of
					 * binding structure for values
					 * to be sent thru the rettype
					 * msg
					 */
    ifx_sqlvar_t *_SQCfields;	/* pointer to first in array of
					 * structures describing the data
					 * to be received from SQL;
					 * (fields described by SQL)
					 */
    ifx_sqlda_t  *_SQCsqlda;	/* pointer to sqlda */
    char **_SQCcommand;			/* pointer to ptrs to pieces of
					 * the command 
					 */
    char *_SQCstrtab;			/* pointer to table of strings - the
					 * names of the attributes to be
					 * received from SQL
					 * (table supplied by SQL)
					 */
    char *_SQCtbuf;				/* tuple buffer */
    char *_SQCtuple;			/* pointer to current tuple within
					 * buffer
					 */
    char *_SQCname;				/* cursor name */

    char *_SQCapiname;			/* column name buffer when */
					/* DBAPICODE is set */
    mint _SQCposition;			/* Used only for INSERT AT stmts */
    mint _SQCiscollstmt;			/* Set if cursor or stmt is for  */
					/* collection table */
    mint _SQCcl_putcnt;			/* Number of variable length rows
					 * inserted using an insert cursor.
					 */
    ifx_collection_t *_SQCcollection;	/* pointer to collection var    */
    ifx_coll_cur_t _SQCcollcur;         /* collection manager cursor block */
    ifx_cl_tupinfo_t *_SQCcl_tupleinfo; /* collection tuple information */
    ifx_literal_t *_SQClitvalues;	/* pointer to the input values  
					 * specified for the collection table */
    ifx_literal_t *_SQC_savlitvalues;	/* pointer to the saved
					 * literal values that have
					 * the QMARKBIND entries in
					 * it */
    ifx_namelist_t *_SQCnamelist;	/* Pointer to column names 
					 * specified in the projection
					 * list for SELECT, UPDATE, INSERT
					 * and DELETE			*/
    int2 _SQCcl_num; 			/* number of collection columns */
	/* the following 4 fields are for open-fetch-close optimization */
    int2 _SQCopen_state;               /* see open_state below		*/
    char *_SQCdesc_name;	        /* saved sql desc name 		*/
    ifx_sqlda_t *_SQCidesc;            /* saved idesc value 		*/
    int2 _SQCreopt;			/* saved reoptimization flag 	*/
    void *_SQCcls;			/* reserved for fast path */
    void *_SQCcur7;			/* points to the corresponding 
					 * 7.x cursor structure		*/
    int4 _SQCflags3;			/* reserved flag for future use */
    void *_SQCreserved;			/* reserved for future use */
    } ifx_cursor_t;


typedef struct _ifx_statement
    {
    int2 _SQSstmttype;			/* SQ_SELECT, SQ_INSERT, etc. */
    int2 _SQSsqlid;			/* SQL's id for this cursor */
    void  *_SQSreserved;		/* reserved for future use */
    } ifx_statement_t;

typedef struct _ifx_hostvar {
    char *hostaddr;    			/* address of host variable */
    int2 fieldtype;			/* field entry requested by GET */
    int2 hosttype;			/* host type */
    int4 hostlen;			/* length of field type */
    int2 qualifier;			/* qualifier for DATETIME/INTERVAL */
    char *btname;                       /* Base type name if provided in */
                                        /* host variable declaration */
    char *btownername;                  /* Base type owner name */
    char *cstr;                         /* Collection or row host string */
    void *reserved;			/* reserved for future use */
    } ifx_hostvar_t;

/*
 * CURSOR state when (OPTOFC) Open-Fetch-Close Optimization is being used
 */
#define CURSOR_NOT_OPEN		0
#define CURSOR_USER_OPEN	1
#define CURSOR_OPEN_FETCH	2
#define CURSOR_FETCHING		3

/*
 * SQL field type codes
 */
#define XSQLD 		0
#define XSQLTYPE 	1
#define XSQLLEN 	2
#define XSQLPRECISION	3
#define XSQLNULLABLE	4
#define XSQLIND		5
#define XSQLDATA	6
#define XSQLNAME	7
#define XSQLSCALE	8
#define XSQLILEN	9
#define XSQLITYPE	10
#define XSQLIDATA	11
#define XSQLXID         12
#define XSQLTYPENAME    13
#define XSQLTYPELEN     14
#define XSQLOWNERLEN    15
#define XSQLOWNERNAME   16
#define XSQLSOURCETYPE	17
#define XSQLSOURCEID	18


/*
 * Specifications for FETCH
 */
typedef struct _fetchspec
    {
    int4 fval;			/* scroll quantity */
    mint fdir;			/* direction of FETCH (NEXT, PREVIOUS..) */
    mint findchk;		/* check for indicator? */
    } _FetchSpec;

/*
 * Connection type
 */

#define	IFX_CONN_TYPE_NO_AUTH	0x0
#define	IFX_CONN_TYPE_USER_AUTH	0x1
#define	IFX_CONN_TYPE_CRED_AUTH	0x2

/*
 * User connection structure
 */
typedef struct ifx_user_struct
    {
    char	*name;
    char	*passwd;
    } ifx_user_t;

/*
 * ASF connection structure
 */
typedef struct ifx_connect_struct
    {
    int2	conn_type;		/* connection type */
    void	*conn_cred_hdl;		/* connection credential handle */
					/* could be pointer to ifx_user_t */
					/* or any CSS supported credentials */
    } ifx_conn_t;

/*
 * Types stored in csblock_t (iqutil.c)
 */
#define IQ_CURSOR	   0x0000
#define IQ_STMT		   0x0001
#define IQ_ALL		   0x0002
#define IQ_SKIP_CUR_CHK	   0x0100
#define IQ_SKIP_DOWNSHIFT  0x0200

/*
 * The following defines will be used to check the installed version of
 * the libraries against the ones used during compilation. A handshaking
 * method was introduced with shared libraries to indicate if the
 * API changed between one release of the library to the other.
 */
#define IFX_GENLIB_ID  1    /* identifier for libgen.so */
#define IFX_OSLIB_ID   2    /* identifier for libos.so  */
#define IFX_SQLILIB_ID 3    /* identifier for libsql.so */
#define IFX_GLSLIB_ID  4    /* identifier for libgls.so */

/*
 * provide macro definition for the library versions
 * being used while generating client's application executable.
 */
#define CLIENT_GEN_VER		710	/* libgen.so used for compiling application */
#define CLIENT_OS_VER		710	/* libos.so used for compiling application  */
#define CLIENT_SQLI_VER		720	/* libsql.so used for compiling application */
#define CLIENT_GLS_VER		710	/* libgls.so used for compiling application */

#ifdef IFX_THREAD

/* defines for dynamic thread functions */
#define TH_ONCE                 0
#define TH_MUTEXATTR_CREATE     1
#define TH_MUTEXATTR_SETKIND    2
#define TH_MUTEXATTR_DELETE     3
#define TH_MUTEX_INIT           4
#define TH_MUTEX_DESTROY        5
#define TH_MUTEX_LOCK           6
#define TH_MUTEX_UNLOCK         7
#define TH_MUTEX_TRYLOCK        8
#define TH_CONDATTR_CREATE      9
#define TH_CONDATTR_DELETE      10
#define TH_COND_INIT            11
#define TH_COND_DESTROY         12
#define TH_COND_TIMEDWAIT       13
#define TH_KEYCREATE            14
#define TH_GETSPECIFIC          15
#define TH_SETSPECIFIC          16
#define TH_SELF                 17
 
/* Number of dynamic thread functions */
#define TH_MAXIMUM              18

mint ifxOS_set_thrfunc(mint func, mulong (*funcptr)());

#endif  /* IFX_THREAD */

/* defines for SqlFreeMem, FreeType */
#define CURSOR_FREE 1
#define STRING_FREE 2
#define SQLDA_FREE  3
#define CONN_FREE   4
#define LOC_BUFFER_FREE 5


/*
 * This global variable FetBufSize (Fetch Buffer Size) will allow
 * the application to over-ride cursor->_SQCtbsize which dictates the
 * size of the buffer that holds the data that the BE will fill.
 */
int4   *(fnsqlcode());
char   (*(fnsqlstate()))[6];
struct sqlca_s *(fnsqlca());
struct LoginInfoStructTag *(fninetlogin());
int2 *(fnFetArrSize());
int2 *(fnFetBufSize());
int2 *(fnOptMsg());
int    *(fndbfltprec());

/* Functions that replaces global errors and status */
#define SQLCODE    (*(fnsqlcode()))
#define SQLSTATE   (*(fnsqlstate()))

#ifdef DB2CLI
#undef sqlca
#endif

#define sqlca      (*(fnsqlca()))
#define InetLogin  (*(fninetlogin()))
#define FetArrSize (*(fnFetArrSize()))
#define FetBufSize (*(fnFetBufSize()))
#define OptMsg     (*(fnOptMsg()))
#define dbfltprec  (*(fndbfltprec()))

#ifdef __BORLANDC__
#define SetConnect 		setconnect
#define GetConnect 		getconnect
#define ReleaseConnect 	releaseconnect
#define Mondecm 		mondecm
#define Monsep 			monsep
#define SqlFreeMem  	sqlfreemem
#define ASF_Call 		asf_call
#endif

#ifdef __BORLANDC__
#pragma option -a-
#else
#pragma pack ()
#endif

#ifdef __cplusplus
}
#endif

#endif	/* _SQLHDR */
