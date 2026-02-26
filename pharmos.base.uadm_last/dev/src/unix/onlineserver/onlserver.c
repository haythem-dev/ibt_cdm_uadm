/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH                                         */
/**   Date   : 24.08.2004                                                     */
/**                                                                           */
/******************************************************************************/

#include "onlserver.h"


/*----------------------------------------------------------------------------*/
long getDate()
{
   struct tm *t;        
   time_t     tt;

   tt = time(0L);   
   t  = localtime(&tt);

   return (long)(t->tm_year + 1900) * 10000L +
          (long)(t->tm_mon + 1) * 100L +
          (long)(t->tm_mday);
}


/*----------------------------------------------------------------------------*/
char * getTimeStamp()
{
    static char szTimeStr[64];
    struct tm * pTm;
    time_t      tSec;

    tSec = time(NULL);
    pTm  = localtime( &tSec );

    sprintf( szTimeStr, "%02d.%02d.%02d %02d:%02d:%02d",
                        pTm->tm_mday, pTm->tm_mon + 1, (pTm->tm_year+1900)%100,
                        pTm->tm_hour, pTm->tm_min    , pTm->tm_sec );

    return szTimeStr;
}

/*----------------------------------------------------------------------------*/
void stripLast(char * spString)
{
	int iLen;

	for (iLen = (int)strlen(spString) - 1; 
		(spString[iLen] == ' ') && (iLen >= 0); 
		iLen-- )
	{
		spString[iLen] = '\0';
	}

	return;
}


/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/




