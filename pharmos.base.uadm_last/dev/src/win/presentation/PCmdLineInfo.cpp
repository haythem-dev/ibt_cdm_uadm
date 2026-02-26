/******************************************************************************/
/**                                                                           */
/**   Author : T. Hˆrath PRZ GmbH		                                      */
/**   Date   : 26.05.2004                                                     */
/**                                                                           */
/******************************************************************************/

#include "StdAfx.h"

#include "PCmdlineInfo.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/**---------------------------------------------------------------------------*/
/**   Desc   : wird von CPApp::ParseCommandLine aufgerufen, um Commandline    */
/**            Parameter zu registrieren                                      */
/**---------------------------------------------------------------------------*/
void PCmdLineInfo::ParseParam(LPCTSTR lpszParam, BOOL bFlag, BOOL bLast)
{
    if ( bFlag && (lstrlen(lpszParam) >= 3) ) // parameter muﬂ Flag + ":" + Wert enthalten
    { 
        CString csParam = lpszParam;
		CString csIniFile;

        if (csParam.GetAt(1) == ':')
        {
            switch (csParam.GetAt(0))
            {
			case 'x':
				// option -x:blabla 
				// set appropriate mode for option 'x'
				break;

            default:
                TRACE(__FUNCTION__": option '%s' not recognised\n", lpszParam);
            }
        }
        else
        {
            TRACE(__FUNCTION__": option '%s' not valid\n", lpszParam);
        }
    }
    else
    {
        TRACE(__FUNCTION__": parameter '%s' with flag %d is not known\n", lpszParam, bFlag);
    }
}

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/


