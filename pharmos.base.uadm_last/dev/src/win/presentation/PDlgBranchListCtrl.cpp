/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                   */
/**   Date   : 01.06.2004                                                     */
/**                                                                           */
/******************************************************************************/


#include "StdAfx.h"

#include <BppBranchReg.h>
#include <BConppBranchReg.h>

#include "PDlgBranchListCtrl.h"


/*----------------------------------------------------------------------------*/
// Defines für Default-Spalten-Nummern
/*----------------------------------------------------------------------------*/
#define VS_LC_BR_COL_ID          0
#define VS_LC_BR_COL_NAME        1

/**---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*/
bool PDlgBranchListCtrl::init()
{
	insertCol(VS_LC_BR_COL_ID    , CResString::ex().getStrTblText(IDS_LI_ID  ), LVCFMT_LEFT  , 50);
	insertCol(VS_LC_BR_COL_NAME  , CResString::ex().getStrTblText(IDS_LI_BEZ  ), LVCFMT_LEFT  , 150);
	
	return (TVConListCtrl<BConppBranchReg, BppBranchReg>::init());
}

/**---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*/
void PDlgBranchListCtrl::refreshItem(const int iItem)
{
	assert(iItem > -1);

	BppBranchReg * pBranchReg = giveCon().getBsnObj(getIdx(iItem));
	assert(pBranchReg != 0);

	SetItemText(iItem, VS_LC_BR_COL_ID     ,  pBranchReg->getBranchidStr().c_str());
	SetItemText(iItem, VS_LC_BR_COL_NAME   ,  pBranchReg->getBranchname ().c_str());
	
}

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
