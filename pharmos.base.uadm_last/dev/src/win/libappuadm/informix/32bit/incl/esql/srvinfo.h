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
 *  Title:	srvinfo.h
 *  Description:
 *		Server information structure and its related macros
 *
 ***************************************************************************
 */
#ifndef SRVINFO_H
#define SRVINFO_H

#ifdef __BORLANDC__
#pragma option -a8
#else
#pragma pack (8)
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include "ifxtypes.h"

#define PVNAMELEN	80

typedef struct SrvInfo
	{
	muint SQLIVersion;	/* version of SQLI, see sqlmtype.h */
	muint TypeList;		/* type of server, bit map */
	muint CapList;		/* server's capabilities, bit map */
	muint DBList;		/* database characteristics */
	muint Reserved1;		/* reserved */
	muint Reserved2;		/* reserved */
	char ProdVersion[PVNAMELEN+1];	/* product name */
	} ifx_srvinfo_t;


/*
 * Server's types (used with INFO_TYPE submessage of SQ_INFO)
 * These values could be sent in the ORed format or as a series
 * of four-byte values
 */
#define TYP_SE		0x00000001L	/* on if SE, off OnLine */
#define TYP_MT		0x00000002L	/* on if multithreaded */
#define TYP_RDA		0x00000004L	/* on if RDA gateway */
#define TYP_DRDA 	0x00000008L	/* on if DRDA gateway */

#define TYP_OWS       0x00000010L     /* on if OnLine Workgroup Server */
 
#define TYP_EDASQL    0x00000020L     /* on if EDA/SQL gateway */
#define TYP_EGM       0x00000040L     /* on if Enterprise Gateway Manager */
 
/*
 * Capabilities (used with INFO_CAPABILITY submessage of SQ_INFO
 * These values could be sent in the ORed format or as a series
 * of four-byte values.
 * The capabilities can also be used to describe the client SQLI.
 */
#define CAP_NLS		0x00000001L	/* on if NLS ready */
#define CAP_NCHAR	0x00000002L	/* on if able to treat CHAR */
					/* as NCHAR */
#define CAP_MCOLL	0x00000004L	/* on if multiple coll seq */
#define CAP_MLANG	0x00000008L	/* on if multiple lang */
#define CAP_MBYTE	0x00000010L	/* on if multiple byte */
#define CAP_STAR	0x00000020L	/* on if distributed capable */
#define CAP_XA		0x00000040L	/* on if XA capable */

mint __cdecl ifx_srvinfo(ifx_srvinfo_t *info);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#ifdef __BORLANDC__
#pragma option -a-
#else
#pragma pack ()
#endif

#endif /* SRVINFO_H */
