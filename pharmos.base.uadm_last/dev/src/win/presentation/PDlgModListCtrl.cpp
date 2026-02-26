/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                   */
/**   Date   : 14.06.2004                                                     */
/**                                                                           */
/******************************************************************************/


#include "StdAfx.h"

#include <BppAppMod.h>
#include <BConppAppMod.h>

#include "PDlgModListCtrl.h"


/*----------------------------------------------------------------------------*/
// Defines für Default-Spalten-Nummern
/*----------------------------------------------------------------------------*/
#define VS_LC_COL_MODNR	           0
#define VS_LC_COL_MODNAME          1
#define VS_LC_COL_MODNAME_EN       2
#define VS_LC_COL_MODNAME_FR       3
#define VS_LC_COL_MODNAME_HR       4

/**---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*/
bool PDlgModListCtrl::init()
{
	insertCol(VS_LC_COL_MODNR    , CResString::ex().getStrTblText(IDS_LI_ID   ), LVCFMT_LEFT  , 25);
	insertCol(VS_LC_COL_MODNAME  , CResString::ex().getStrTblText(IDS_LI_BEZ  ), LVCFMT_LEFT  , 150);
	insertCol(VS_LC_COL_MODNAME_EN  , CResString::ex().getStrTblText(IDS_LI_BEZ  ), LVCFMT_LEFT  , 150);
	insertCol(VS_LC_COL_MODNAME_FR  , CResString::ex().getStrTblText(IDS_LI_BEZ  ), LVCFMT_LEFT  , 150);
	insertCol(VS_LC_COL_MODNAME_HR  , CResString::ex().getStrTblText(IDS_LI_BEZ  ), LVCFMT_LEFT  , 150);
	
	return (TVConListCtrl<BConppAppMod, BppAppMod>::init());
}

/**---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*/
void PDlgModListCtrl::refreshItem(const int iItem)
{
	assert(iItem > -1);

	BppAppMod * pMod = giveCon().getBsnObj(getIdx(iItem));
	assert(pMod != 0);

	SetItemText(iItem, VS_LC_COL_MODNR     ,  pMod->getModuleidStr().c_str());
	SetItemText(iItem, VS_LC_COL_MODNAME   ,  pMod->getModname	  ().c_str());
	SetItemText(iItem, VS_LC_COL_MODNAME_EN   ,  pMod->getModname_EN	  ().c_str());
	SetItemText(iItem, VS_LC_COL_MODNAME_FR   ,  pMod->getModname_FR	  ().c_str());
	SetItemText(iItem, VS_LC_COL_MODNAME_HR   ,  pMod->getModname_HR	  ().c_str());
}

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
