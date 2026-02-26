/****************************************************************************
 *
 * Licensed Material - Property Of IBM
 *
 * "Restricted Materials of IBM"
 *
 * IBM Informix Client SDK
 * Copyright IBM Corporation 2002, 2010
 *
 *  Title:       datetime.h
 *  Description: include for DATETIME and INTERVAL
 *
 ***************************************************************************
 */

#ifndef _DATETIME_H
#define _DATETIME_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ifxtypes.h"
#include "decimal.h"

#ifdef __BORLANDC__
#pragma option -a8
#else
#pragma pack (8)
#endif

typedef struct dtime
	{
	int2 dt_qual;
	dec_t dt_dec;
	}	dtime_t;

typedef struct intrvl
	{
	int2 in_qual;
	dec_t in_dec;
	}	intrvl_t;

/* time units for datetime qualifier */

#define TU_YEAR 0
#define TU_MONTH 2
#define TU_DAY 4
#define TU_HOUR 6
#define TU_MINUTE 8
#define TU_SECOND 10
#define TU_FRAC 12
#define TU_F1 11
#define TU_F2 12
#define TU_F3 13
#define TU_F4 14
#define TU_F5 15

/* TU_END - end time unit in datetime qualifier */
/* TU_START - start time unit in datetime qualifier */
/* TU_LEN - length in  digits of datetime qualifier */

#define TU_END(qual) (qual & 0xf)
#define TU_START(qual) ((qual>>4) & 0xf)
#define TU_LEN(qual) ((qual>>8) & 0xff)

/* TU_ENCODE - encode length, start and end time unit to form qualifier */
/* TU_DTENCODE - encode datetime qual */
/* TU_IENCODE - encode interval qual */

#define TU_ENCODE(len,s,e) (((len)<<8) | ((s)<<4) | (e))
#define TU_DTENCODE(s,e) TU_ENCODE(((e)-(s)+((s)==TU_YEAR?4:2)), s, e)
#define TU_IENCODE(len,s,e) TU_ENCODE(((e)-(s)+(len)),s,e)
#define TU_FLEN(len) (TU_LEN(len)-(TU_END(len)-TU_START(len)))

/* TU_FIELDNO - field number of the given TU_ macro.
   (e.g. year is 0, month is 1, day is 2, TU_F1 is  7, TU_F5 is 11)
 */

#define TU_FIELDNO(f)   (f > TU_SECOND ? (5+(f-TU_SECOND)) : (f/2))

/* TU_CURRQUAL - default qualifier used by current */

#define TU_CURRQUAL TU_ENCODE(17,TU_YEAR,TU_F3)

#ifdef __BORLANDC__
#pragma option -a-
#else
#pragma pack ()
#endif

#ifdef __cplusplus
}
#endif

#endif /* _DATETIME_H */
