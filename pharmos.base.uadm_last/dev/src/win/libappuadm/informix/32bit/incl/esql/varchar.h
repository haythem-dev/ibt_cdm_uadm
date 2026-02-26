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
 *  Title:	varchar.h
 *  Description:
 *		header file for varying length character data type
 *
 ***************************************************************************
 */

/*
 * VARCHAR macros
 */
#ifdef __cplusplus
extern "C" {
#endif

#define MAXVCLEN		(255)
#define VCLENGTH(len)		(VCMAX(len)+1)
#define VCMIN(size)		(((size) >> 8) & 0x00ff)
#define VCMAX(size)		((size) & 0x00ff)
#define VCSIZ(max, min)		((((min) << 8) & 0xff00) + ((max) & 0x00ff))

#ifdef __cplusplus
}
#endif
