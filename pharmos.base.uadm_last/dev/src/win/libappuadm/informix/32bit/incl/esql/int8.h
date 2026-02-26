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
 *  Title:	int8.h
 *  Description:
 *		Header file for int8 data type.
 *
 ***************************************************************************
 */

#ifndef _INT8_H
#define _INT8_H

#ifdef __BORLANDC__
#pragma option -a8
#else
#pragma pack (8)
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include "ifxtypes.h"

#define INT8SIZE 2	   /* number of unsigned int4's in struct ifx_int8 */
#define INT8UNKNOWN -2     /* Value returned by int8 comparison function if one 
			    * of the operands is NULL.
			    */

#define INT8NULL	0  /* A int8 null will be represented internally by setting 
			    * sign equal to INT8NULL
			    */


typedef struct ifx_int8
    {
    uint4 data[INT8SIZE];
    int2 sign;		/* 0 = NULL, 1 = positive, -1 = negative */
    } ifx_int8_t;

#ifdef __cplusplus
}
#endif

#ifdef __BORLANDC__
#pragma option -a-
#else
#pragma pack ()
#endif

#endif /* _INT8_H */
