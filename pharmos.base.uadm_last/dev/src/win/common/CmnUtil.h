/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH	                                         */
/**   Date   : 11.02.2003                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once


#ifndef __CMNUTIL_H_
#define __CMNUTIL_H_

/*----------------------------------------------------------------------------*/
// Debug Stuff
/*----------------------------------------------------------------------------*/


// because of detecting memory leaks
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>


#if defined(_DEBUG) && defined(WIN32)
extern "C" __declspec(dllimport) void __stdcall OutputDebugStringA(const char * lpOutputString);
void DbgMsg(const char * spFormat, ...);  // für Debug-Ausgaben ohne MFC
#else
#define DbgMsg	__noop
#endif	_DEBUG

using sccmn::CBString;
/*----------------------------------------------------------------------------*/
// Convert Stuff
/*----------------------------------------------------------------------------*/
void    long2scStr      (CBString & strNumber, long lNo);
void    short2scStr     (CBString& strNumber, short sNo);


#endif

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
