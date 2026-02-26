/****************************************************************************
 *
 * Licensed Material - Property Of IBM
 *
 * "Restricted Materials of IBM"
 *
 * IBM Informix Client SDK
 * Copyright IBM Corporation 1997, 2008. All rights reserved.
 *
 *  Title:       minmmem.h
 *  Description: Restricted use memory duration definitions and prototypes
 *
 *  Used by datablade developers
 *
 ***************************************************************************
 */

/*
   The definitions of memory durations and prototypes are split into
   two separate files to get around build problems in the server. This file
   includes both of those as a convenience to the developer.
*/

#ifndef _MINMMEM_H_
#define _MINMMEM_H_

/* durations */
#ifndef _MEMDUR_H_
#include "memdur.h"
#endif /* not _MEMDUR_H_ */

/* prototypes */
#ifndef _MINMPROTO_H_
#include "minmprot.h"
#endif /* not _MINMPROTO_H_ */

#endif /* _MINMMEM_H_ */

