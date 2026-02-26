/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH	                                       */
/**   Date   : 25.05.2004                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once

#ifndef __PCMN_H___
#define __PCMN_H___

/*----------------------------------------------------------------------------*/
CString     GetCtrlString(CWnd * pWnd);

INT_PTR		MsgBoxOK(const char * const spMsg,  UINT uiType = MB_ICONEXCLAMATION, const char * const spCaption = NULL);
INT_PTR		MsgBoxOK(UINT uiMsg, UINT uiType = MB_ICONEXCLAMATION, UINT uiCaption = AFX_IDS_APP_TITLE);
INT_PTR		MsgBoxYesNo(const char * const spMsg, UINT uiType = MB_ICONQUESTION | MB_DEFBUTTON2, const char * const spCaption = NULL);
INT_PTR		MsgBoxYesNo(UINT uiMsg,  UINT uiType = MB_ICONQUESTION | MB_DEFBUTTON2, UINT uiCaption = AFX_IDS_APP_TITLE);

#define CHAR_A 0x41
#define CHAR_Z 0x5A

#endif
/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
