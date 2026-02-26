/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH                                             */
/**   Date   : 24.08.2004                                                     */
/**                                                                           */
/******************************************************************************/

#include "srv.h"

#include "SLog.h"


#define FUNC_WRITE		"_writeLog"


/**---------------------------------------------------------------------------*/
VSingleton<SLog>::ExemplarPtr VSingleton<SLog>::m_objExemplar;

/**---------------------------------------------------------------------------*/
SLog::SLog() : m_sVZ        (0), 
               m_lUID       (0),
			   m_spFuncWrite(FUNC_WRITE)
{
}

/**---------------------------------------------------------------------------*/
SLog::~SLog()
{
}

/*----------------------------------------------------------------------------*/
VAccess::ERet SLog::write(const char * const spFunction, 
							 VAccess::ERet   eError, 
							 const char * const spMsg, ...)
{	
	char szInMsg [2048];
	char szOutMsg[2048];
	int  iOutLen;

	char chError;

	switch (eError)
	{
		case RET_ERROR:
			chError = 'F';
			break;
		case RET_OK:
			chError = 'O';
			break;
		case RET_WARNING:
			chError = 'W';
			break;
		//case RET_NOTFOUND:
		//case RET_LOCKED:
		default:
			chError = 'W';
			break;
	}

	sprintf(szInMsg, "%cVZ=%02d UID=%04d %s ", 
		             chError,
		             m_sVZ, 
				     m_lUID,
				     spFunction);

	va_list argList;

	va_start( argList, spMsg );

	vsprintf(szInMsg + strlen(szInMsg), spMsg, argList );

	va_end( argList );

	long lRet;

	if ((lRet = m_objVdooActivConn->callLibFunc(m_spFuncWrite, 
									                  szInMsg , (int)strlen(szInMsg),
									                  szOutMsg, &iOutLen             )) != CON_SUCCESS)
	{
		DbgMsg(__FUNCTION__ ": call failed (%ld)", lRet);

		return VAccess::RET_ERROR;
	}

	return VAccess::RET_OK;
}

/*----------------------------------------------------------------------------*/
void SLog::setVZ (short sVZ)
{
	m_sVZ = sVZ;
}

/*----------------------------------------------------------------------------*/
void SLog::setUID(long  lUID)
{
	m_lUID = lUID;
}

/*----------------------------------------------------------------------------*/
void SLog::setActivConn	(CVdooConn* objVdooActivConn)
{
	m_objVdooActivConn = objVdooActivConn;
}

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
