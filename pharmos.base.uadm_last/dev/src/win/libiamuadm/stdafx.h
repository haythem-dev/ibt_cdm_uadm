/******************************************************************************/
/**                                                                           */
/**   File   : StdAfx.h                                                       */
/**   Desc   : Include-Datei für Standard-System-Include-Dateien,             */
/**            oder projektspezifische Include-Dateien, die häufig benutzt,   */
/**            aber in unregelmäßigen Abständen geändert werden.              */
/**   Author : T. Hörath PRZ GmbH	                                       */
/**   Date   : 25.05.2004                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once

#ifndef __STDAFX_H___
#define __STDAFX_H___

/*----------------------------------------------------------------------------*/
// MFC-Includes
/*----------------------------------------------------------------------------*/
#define VC_EXTRALEAN		    // Selten verwendete Teile der Windows-Header nicht einbinden

#define WINVER 0x0601
#define _WIN32_WINNT 0x0601     // Windows 7

//#include <afxwin.h>             // MFC-Kern- und -Standardkomponenten
#include <afxext.h>             // MFC-Erweiterungen
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			    // MFC-Unterstützung für gängige Windows-Steuerelemente
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <afxtempl.h>
#include <afxdtctl.h>

#include <assert.h>
#include <afxdb.h>

/*----------------------------------------------------------------------------*/
// Common Library Includes
/*----------------------------------------------------------------------------*/
#include <libsccstring/libsccstring.h>
#include <libsccpattern/libsccpattern.h>
#include <libscmlogin/libscmlogin.h>
#include <libsccdate/libsccdate.h>

/*----------------------------------------------------------------------------*/
// Business Layer Library Includes
/*----------------------------------------------------------------------------*/
#include <libscbmeb/libscbmeb.h>

/*----------------------------------------------------------------------------*/
// GUI Common Library Includes
/*----------------------------------------------------------------------------*/
#include <libscgmmultilang/libscgmmultilang.h>
#include <libscgmctrl/libscgmctrl.h>
#include <libscgmmeb/libscgmmeb.h>

/*----------------------------------------------------------------------------*/
//namespaces für servclnt
/*----------------------------------------------------------------------------*/
using scbsn::VBsnObject;
using scbsn::TVConBsnObject;
using scbsn::IPerst;
using scbsn::VObject;
using scgui::TVConListCtrl;
using scgui::CWinAppMultiLang;
using scgui::CDialogMultiLang;
using scgui::CFormViewMultiLang;
using scgui::CFrameWndMultiLang;
using sccmn::CBString;
using scgui::CResString;
using scmod::ppBLogIn;
using scmod::ppBLoginData;
using sccmn::VSingleton;
using scgui::MsgBox1;
using scgui::MsgBox2;
using scgui::CPrint;

/*----------------------------------------------------------------------------*/
// Common Includes: Util-Routines
/*----------------------------------------------------------------------------*/
#include <CmnUtil.h>
#include <defines.h>

/*----------------------------------------------------------------------------*/
// GUI Includes
/*----------------------------------------------------------------------------*/
//#include "PCmn.h"
//#include "resource.h"

#endif
/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
