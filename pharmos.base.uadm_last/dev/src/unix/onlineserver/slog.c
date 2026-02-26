/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH                                         */
/**   Date   : 24.08.2004                                                     */
/**                                                                           */
/******************************************************************************/


#include "onlserver.h"

/*----------------------------------------------------------------------------*/
static long writeFile(const char * const spFile, const char * const spTxt)
{
    static char * pWwsPath = NULL;
    char          szLogFile[ 512];
    FILE        * pFile;

    if (pWwsPath == (char *)NULL)
    {
		if ((pWwsPath = getenv(WWS_PATH)) == (char *)NULL)
			return -99;
    }

	strcpy(szLogFile, pWwsPath);
	strcat(szLogFile, "/de/uadm/log/");
	strcat(szLogFile, spFile);

    if ( (pFile = fopen(szLogFile, "a") ) == (FILE *)NULL)
        return errno;

	fputs(spTxt, pFile);
	fputc('\n' , pFile);

    fclose(pFile);

	return 0;
}

/*----------------------------------------------------------------------------*/
long writeLog(char chError, const char * const spMsg, ...)
{
	long    lRet;
	char    szMsg[2048];
	va_list argList;

    sprintf(szMsg, "%c %s pid=%05d %s-%s ",
                   chError,
                   getTimeStamp(),
				   getpid(),
                   PROG_NAME,
                   PROG_VERSION);

	va_start( argList, spMsg );

	vsprintf(szMsg + strlen(szMsg), spMsg, argList );

	va_end( argList );

    if ((lRet = writeFile(PROG_LOGFILE, szMsg)) != 0)
		return lRet;

    if (chError == 'F')
	{
        if ((lRet = writeFile(DEF_LOGFILE, szMsg)) != 0)
			return lRet;
	}

	return lRet;
}

/*----------------------------------------------------------------------------*/
long _writeLog(char * pInBuf, int inLen, char * pOutBuf, int * piOutLen)
{
    inLen    = inLen;
    pOutBuf  = pOutBuf;
    piOutLen = piOutLen;

    return writeLog(*pInBuf, "%s", pInBuf + 1);
}

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
