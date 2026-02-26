/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH                                         */
/**   Date   : 25.05.2004                                                    */
/**                                                                           */
/******************************************************************************/

/*----------------------------------------------------------------------------*/
#include "StdAfx.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


/*----------------------------------------------------------------------------*/
CString GetCtrlString(CWnd * pWnd)
{
	CString csTxt;

	pWnd->GetWindowText(csTxt);	

	csTxt.Remove('&');

	return csTxt;
}

/*----------------------------------------------------------------------------*/
INT_PTR	MsgBoxOK(const char * const spMsg, UINT uiType, const char * const spCaption)
{
	return MsgBox1(spMsg, 
		CResString::ex().getStrTblText(IDS_MSG_OK), 
		uiType,
		spCaption);
}

/*----------------------------------------------------------------------------*/
INT_PTR	MsgBoxOK(UINT uiMsg, UINT uiType, UINT uiCaption)
{
	return MsgBox1(uiMsg, 
		IDS_MSG_OK, 
		uiType,
		uiCaption);
}

/*----------------------------------------------------------------------------*/
INT_PTR	MsgBoxYesNo(const char * const spMsg, UINT uiType, const char * const spCaption)
{
	return MsgBox2(spMsg, 
		CResString::ex().getStrTblText(IDS_MSG_YES), 
		CResString::ex().getStrTblText(IDS_MSG_NO), 
		uiType,
		spCaption);
}

/*----------------------------------------------------------------------------*/
INT_PTR	MsgBoxYesNo(UINT uiMsg, UINT uiType, UINT uiCaption)
{
	return MsgBox2(uiMsg, 
		IDS_MSG_YES, 
		IDS_MSG_NO, 
		uiType,
		uiCaption);
}


/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/


