/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                   */
/**   Date   : 16.08.2004                                                     */
/**                                                                           */
/******************************************************************************/


#include "StdAfx.h"

#include <BppPrev.h>
#include <BConppPrev.h>

#include "PDlgPrevListCtrl.h"


/*----------------------------------------------------------------------------*/
// Defines für Default-Spalten-Nummern
/*----------------------------------------------------------------------------*/
#define VS_LC_COL_GLOBUSER		0
#define VS_LC_COL_USERNAME      1
#define VS_LC_COL_LANG		    2
#define VS_LC_COL_BRREG		    3


/**---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*/
bool PDlgPrevListCtrl::init()
{
	insertCol(VS_LC_COL_GLOBUSER  , CResString::ex().getStrTblText(IDS_VIEW_PREV_LI_WINNAME  ), LVCFMT_LEFT  , 100);
	insertCol(VS_LC_COL_USERNAME  , CResString::ex().getStrTblText(IDS_VIEW_USER_LI_USERNAME  ), LVCFMT_LEFT  , 100);
	insertCol(VS_LC_COL_LANG  ,		CResString::ex().getStrTblText(IDS_VIEW_PREV_LI_LANG  ),		LVCFMT_LEFT  , 60);
	insertCol(VS_LC_COL_BRREG  ,	CResString::ex().getStrTblText(IDS_VIEW_PREV_LI_AREA  ),		LVCFMT_LEFT  , 50);
	
	return (TVConListCtrl<BConppPrev, BppPrev>::init());
}

/**---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*/
void PDlgPrevListCtrl::refreshItem(const int iItem)
{
	assert(iItem > -1);

	BppPrev * pPrev = giveCon().getBsnObj(getIdx(iItem));
	assert(pPrev != 0);

	SetItemText(iItem, VS_LC_COL_GLOBUSER   , pPrev->getGlobUser   ().c_str());
	SetItemText(iItem, VS_LC_COL_USERNAME  , pPrev->getUsername  ().c_str());
	SetItemText(iItem, VS_LC_COL_LANG  , pPrev->getLanguageID  ().c_str());
	SetItemText(iItem, VS_LC_COL_BRREG  , pPrev->getBrRegIDstr  ().c_str());
}

/**---------------------------------------------------------------------------*/

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
