/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH                                         */
/**   Date   : 24.08.2004                                                     */
/**                                                                           */
/******************************************************************************/

#ifndef PRZ_INC_UADM_ONLSRV_H
#define	PRZ_INC_UADM_ONLSRV_H


/*----------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <assert.h>

#include <libscsvoodoo/libscsvoodoo.h>

#ifdef WIN32
#include <process.h>
#define getpid	_getpid 
#else
#include <unistd.h>
#include <errno.h>
#endif

/*----------------------------------------------------------------------------*/

#define PROG_VERSION     "02.00.00.00" // Unix Version
#define PROG_NAME        "uadm"
#define PROG_LOGFILE     PROG_NAME".log"

#define DEF_LOGFILE      "error.log"
#define WWS_PATH         "WSS"


#define SRV_ERROR   'F'
#define SRV_WARNING 'W'
#define SRV_OK      'O'

/*----------------------------------------------------------------------------*/

long   getDate		();
char * getTimeStamp	();
long   writeLog		(char chError, const char * const spMsg, ...);
void   stripLast	(char * spString);


#endif


/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
