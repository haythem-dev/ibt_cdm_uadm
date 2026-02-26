/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                   */
/**   Date   : 01.06.2004                                                     */
/**                                                                           */
/******************************************************************************/


#include "StdAfx.h"

#include <BppApp.h>
#include <BConppApp.h>

#include "PDlgAppListCtrl.h"


/*----------------------------------------------------------------------------*/
// Defines für Default-Spalten-Nummern
/*----------------------------------------------------------------------------*/
#define VS_LC_COL_APPNAME          0
#define VS_LC_COL_APPDESC          1
#define VS_LC_COL_APPNAME_EN       2
#define VS_LC_COL_APPNAME_FR       3
#define VS_LC_COL_APPNAME_HR       4

/**---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*/
bool PDlgAppListCtrl::init()
{
	insertCol(VS_LC_COL_APPNAME  , CResString::ex().getStrTblText(IDS_APP_LI_APPNAME  ), LVCFMT_LEFT  , 100);
	insertCol(VS_LC_COL_APPDESC  , CResString::ex().getStrTblText(IDS_DESC  ), LVCFMT_LEFT  , 200);
	insertCol(VS_LC_COL_APPNAME_EN  , CResString::ex().getStrTblText(IDS_APP_LI_APPNAME  ), LVCFMT_LEFT  , 80);
	insertCol(VS_LC_COL_APPNAME_FR  , CResString::ex().getStrTblText(IDS_APP_LI_APPNAME  ), LVCFMT_LEFT  , 80);
	insertCol(VS_LC_COL_APPNAME_HR  , CResString::ex().getStrTblText(IDS_APP_LI_APPNAME  ), LVCFMT_LEFT  , 80);
	
	return (TVConListCtrl<BConppApp, BppApp>::init());
}

/**---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*/
void PDlgAppListCtrl::refreshItem(const int iItem)
{
	assert(iItem > -1);

	BppApp * pApp = giveCon().getBsnObj(getIdx(iItem));
	assert(pApp != 0);

	SetItemText(iItem, VS_LC_COL_APPNAME   ,  pApp->getAppname	().c_str());
	SetItemText(iItem, VS_LC_COL_APPDESC   ,  pApp->getAppDesc  ().c_str());
	SetItemText(iItem, VS_LC_COL_APPNAME_EN   ,  pApp->getAppname_EN  ().c_str());
	SetItemText(iItem, VS_LC_COL_APPNAME_FR   ,  pApp->getAppname_FR  ().c_str());
	SetItemText(iItem, VS_LC_COL_APPNAME_HR   ,  pApp->getAppname_HR  ().c_str());
}

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
