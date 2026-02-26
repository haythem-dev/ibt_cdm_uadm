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
 *  Title:	value.h
 *  Description:
 *		value header include file; multi-purpose value struct
 *
 ***************************************************************************
 */

#ifndef _VALUE
#define _VALUE

#ifdef __cplusplus
extern "C" {
#endif

#include "ifxtypes.h"

#include "decimal.h"
#include "locator.h"
#include "blob.h"
#include "int8.h"
#include "collct.h"
#include "datetime.h"

#ifdef __BORLANDC__
#pragma option -a8
#else
#pragma pack (8)
#endif

/* !null && blob type */
#define ISBLOBVALUE(val)	((val).v_ind != -1 && ISBLOBTYPE((val).v_type))

#define MAXADDR		14

#define ROW_COMMON_HDR \
	void *dummy;	/* dummy place holder */

typedef struct rowvaluet row_desc_t;
typedef struct rowvaluet row_value_t;

/* flags for td_rowflags */
#define TD_ROWREF       0x00000001L     /* to be REFed, keep fragid and rowid */
#define TD_ROWTYPE      0x00000002L     /* row is typed */
#define TD_ROWLOCAL     0x00000004L     /* row data is in td_row */
#define TD_ROWDYNA      0x00000008L     /* row value in sqt_dtab */
#define TD_ROWSAVED     0x00000010L     /* row in save set */
#define TD_ROWCONV      0x00000020L     /* binary or ascii conv. required */
#define TD_ROWBINARY    0x00000040L     /* binary result requested */
#define TD_ROWCMDDUR    0x00000080L     /* Indicates that the row descriptor
                                         * is allocated in PER_COMMAND dur.
					 */

#define TD_ROWSTMTDUR   0x00000100L     /* Indicates that the row descriptor
                                         * is allocated in PER_STATEMENT dur.
					 */
#define TD_ROWDESTROY   0x80000000     /* processing rowdestroy */

struct rowvaluet
    {
    ROW_COMMON_HDR
    };

/* flag to switch_rowtype() on whether to create the rowdesc
 * and how to link it with existing cd_rowdesc chain
 */
#define R_NCREATE  0  /* Flag error if rowdesc does not exist in
                       * td_child list
                       */
#define R_LTAIL    1  /* If created now, link to tail of list */
#define R_LHEAD    2  /* Put rowdesc at the head of the list */

#define VCHAR_COMMON_HDR \
    char *vcp;		/* data start 			*/ \
    int4 vlen;		/* data length 			*/ \
    int2 vflgs;	/* flags - see below 		*/ \
    int2 valign;	/* UDT (SQLROW, SQLUDTFIXED/VAR) alignment */
/*
 * col_t cd_coll_desc cache of cl_*typeinfo
 * and cl_data for val{load/store/ldout} 
 */
typedef struct _coll_desc
    {
    ifx_cltypeinfo_t	*cld_typeinfo;	/* collection cl_svr_typeinfo */
    int4		cld_typesize;	/* size of typeinfo */
    } coll_desc_t;

/*
** vchar_t is shared by the value_t and udt_t
*/

typedef struct _vchar
    {		
    VCHAR_COMMON_HDR
    int4 vidx;		/* string+UDT: data block size 	*/
    int4 vsstart;	/* substring start for 4GL-RDS 	*/
    int4 vsend;		/* substring end for 4GL-RDS 	*/
    int4 fixedLen;	/* Length of fixedStr		*/
    int4 vbyvaluedata;	/* data store for byvalue UDTs  */
    char *fixedStr;	/* Fixed str for FSSEARCH	*/
    unsigned char *shiftTbl;/* Shift table for FSSEARCH	*/
    } vchar_t;


typedef struct value
    {
    short v_type;
    short v_ind;		/* null indicator		*/
    int4  v_xid;
    short v_prec;		/* decimal precision		*/
    union			/* data value			*/
	{			/*  depending on v_type		*/
	vchar_t vchar;		/* char and udts		*/
	mint vint;		/* SQLSMINT			*/
	int4 vlng;		/* SQLINT			*/
	ifx_int8_t vint8;	/* SQLINT8			*/
	float vflo;		/* SQLSMFLOAT			*/
	double vdub;		/* SQLFLOAT			*/
	struct 
	    {
	    dec_t vdec;		/* SQLDECIMAL			*/
	    union
		{
		dtime_t vdatetime;
		intrvl_t vintrvl;
		} vtme;
	    } vdecstruct;
	short vaddr[MAXADDR];	/* 4GL address modifiers	*/
	tblob_t vtblob;		/* BLOB as stored in tuple	*/
	loc_t *vlocator;	/* blobs locator 		*/
	ifx_collection_t vcollection;	          /* collection */
        ifx_collval_t vtree;  /* value_t tree of collection/ROW */ 
	struct
	    {
	    VCHAR_COMMON_HDR
	    row_value_t vrow;	/* row descriptor and data */
	    } vrowstruct;	/* for row values */
	} v_val;
    } value_t;

#define CASTVALP	(struct value *)

/*
 * defines to make the union transparent
 */

#define v_charp		v_val.vchar.vcp
#define v_index		v_val.vchar.vidx
#define v_align		v_val.vchar.valign
#define v_len		v_val.vchar.vlen
#define v_flags		v_val.vchar.vflgs
#define v_sstart	v_val.vchar.vsstart
#define v_send		v_val.vchar.vsend
#define v_fixedLen	v_val.vchar.fixedLen
#define v_fixedStr	v_val.vchar.fixedStr
#define v_shiftTbl	v_val.vchar.shiftTbl
#define v_byvaluedata	v_val.vchar.vbyvaluedata 

#define v_int		v_val.vint
#define v_long		v_val.vlng
#define v_int8	        v_val.vint8
#define v_float		v_val.vflo
#define v_double	v_val.vdub
#define v_decimal	v_val.vdecstruct.vdec
#define v_idesc		v_ind
#define v_naddr		v_prec
#define v_addr		v_val.vaddr

#define v_datetime	v_val.vdecstruct.vtme.vdatetime
#define v_intrvl	v_val.vdecstruct.vtme.vintrvl

#define v_tblob		v_val.vtblob
#define v_blocator	v_val.vlocator
	
#define v_coll		v_val.vcollection
/* tchan : add macro for items in vtree */
#define v_child         v_val.vtree.ct_child
#define v_vtcoll        v_val.vtree.ct_coll

#define v_row		v_val.vrowstruct.vrow

/*
 * flags for v_flags (For FE programs)
 * used by the 4GL Debugger and Pcode Run Time
 */
#define	V_BREAK		0x1		/* break when variable is updated */ 
#define	V_SUBSTR	0x2		/* char value is a substring */
#define	V_QUOTED	0x4		/* char value is from quoted string */
#define V_ASCII0	0x8		/* ascii 0 value */
/*
 * flags for v_flags (For BE programs)
 * used for FS_[N]SEARCH patterns
 */
#define	V_FSANSIESCWARN 0x1 /* Fixed string contained a non-ANSI escape */
                             /* sequence in an non-ANSI database */
#define V_NOHASH	0x2 /* UDT not hashable */
#define V_FIXED		0x4 /* UDT is fixed length (SQLROW type only) */
#define V_BYVAL		0x8 /* UDT is by value */
#define V_QUAL      0x10 /* v_char contains qualifier
			              * (used with char type on client,
			              * and SQLUDTVAR (XID_LVARCHAR) type on server)
			              */

/* This flag is used in rvaldata(), but mainly used by the FE to null
 * terminate the SQLCHAR data rather than blank padding it as for normal
 * SQLCHAR column.
 */
#define V_NTERM		0x20	/* null terminated the SQLCHAR data */

#define FRCBOOL(x)	if (x->v_ind >= 0)\
			    switch (x->v_type) \
				{\
				case SQLSMINT: break;\
				case SQLDATE:\
				case SQLSERIAL:\
				case SQLINT: if (x->v_long != 0)\
						{\
						SET_BOOLOP_RESULT(x, 1);\
						}\
					     else\
						SET_BOOLOP_RESULT(x, 0);\
					     SET_BOOLOP_TYPE(x); \
					     break;\
			        default:     cvtosmint(x);\
					     break;\
				} \
			else \
			    { \
			    SET_BOOLOP_RESULT(x, 0);\
			    SET_BOOLOP_TYPE(x); \
			    }

#define init_int_value(valptr) \
    (valptr)->v_type = SQLSMINT; \
    (valptr)->v_ind = 0; \
    (valptr)->v_prec = 0; \
    (valptr)->v_int = 0;

#define ISLVARCHARTYPE(valp) \
        (\
        ISUDTVARTYPE((valp)->v_type) &&\
        (valp)->v_xid == XID_LVARCHAR\
        )
 
#define ISBOOLEANTYPE(valp) \
        (\
        ISUDTFIXEDTYPE((valp)->v_type) &&\
        (valp)->v_xid == XID_BOOLEAN\
        )

/*
 * macros for evaluating boolean expressions results.
 */

#define SET_BOOLOP_RESULT(valp, value) 	(valp)->v_int = value
#define SET_BOOLOP_TYPE(valp) 		(valp)->v_type = SQLSMINT
#define BOOLOP_RESULT(val) 		(val).v_int
#define BOOLOP_RESULTP(valp) 		(valp)->v_int
#define IS_BOOLOP_TYPEP(valp) 		(valp)->v_type == SQLSMINT

#define copy_value(fromP, toP) bycopy((char*)(fromP), (char *)(toP), \
				      sizeof(struct value));

#ifdef __BORLANDC__
#pragma option -a-
#else
#pragma pack ()
#endif

#ifdef __cplusplus
}
#endif

#endif  /* _VALUE */
