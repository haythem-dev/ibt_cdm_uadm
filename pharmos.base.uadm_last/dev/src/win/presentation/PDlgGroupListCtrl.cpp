/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                   */
/**   Date   : 26.05.2004                                                     */
/**                                                                           */
/******************************************************************************/


#include "StdAfx.h"

#include <BppGroup.h>
#include <BConppGroup.h>

#include "PDlgGroupListCtrl.h"


/*----------------------------------------------------------------------------*/
// Defines für Default-Spalten-Nummern
/*----------------------------------------------------------------------------*/
#define VS_LC_COL_GROUPNAME		0

/**---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*/
bool PDlgGroupListCtrl::init()
{
	insertCol(VS_LC_COL_GROUPNAME  , CResString::ex().getStrTblText(IDS_DLG_GROUP_LI_NAME  ), LVCFMT_LEFT  , 220);
	
	return (TVConListCtrl<BConppGroup, BppGroup>::init());
}

/**---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*/
void PDlgGroupListCtrl::refreshItem(const int iItem)
{
	assert(iItem > -1);

	BppGroup * pGroup = giveCon().getBsnObj(getIdx(iItem));
	assert(pGroup != 0);

	SetItemText(iItem, VS_LC_COL_GROUPNAME , pGroup->getName   ().c_str());
}

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
