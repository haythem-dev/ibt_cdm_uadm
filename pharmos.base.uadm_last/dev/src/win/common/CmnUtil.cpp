/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH						                    */
/**   Date   : 25.05.2004                                                     */
/**                                                                           */
/******************************************************************************/


/*----------------------------------------------------------------------------*/
// Includes
/*----------------------------------------------------------------------------*/
#include "cmn.h"			// precompiled header

#include "CmnUtil.h"


/**---------------------------------------------------------------------------*/
#if defined(_DEBUG) && defined(WIN32)

void DbgMsg(const char * spFormat, ...)
{
	va_list argList;
	char    szMsg[2048];

	va_start( argList, spFormat );

	vsprintf(szMsg, spFormat, argList );

	va_end( argList );

	strcat(szMsg, "\n");
	OutputDebugStringA(szMsg);
}

#endif

/**---------------------------------------------------------------------------*/
void long2scStr (CBString & strNumber, long lNo)
{
	strNumber.ltos(lNo);
}

/**---------------------------------------------------------------------------*/
void short2scStr(CBString& strNumber, short sNo)
{
	strNumber.itos(sNo);
}

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
