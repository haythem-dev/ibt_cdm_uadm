/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                   */
/**   Date   : 15.06.2004                                                     */
/**                                                                           */
/******************************************************************************/


#include "StdAfx.h"

#include <BppAppRight.h>
#include <BConppAppRight.h>

#include "PDlgRightListCtrl.h"


/**---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*/
bool PDlgRightListCtrl::init()
{
	insertCol(VS_LC_COL_RIGHTNAME	, CResString::ex().getStrTblText(IDS_RIGHT  ), LVCFMT_LEFT  , 100);
	insertCol(VS_LC_COL_RIGHTDESC	, CResString::ex().getStrTblText(IDS_DESC    ), LVCFMT_LEFT  , 200);
	insertCol(VS_LC_COL_RIGHTDESC_EN, CResString::ex().getStrTblText(IDS_DESC ), LVCFMT_LEFT  , 200);
	insertCol(VS_LC_COL_RIGHTDESC_FR, CResString::ex().getStrTblText(IDS_DESC ), LVCFMT_LEFT  , 200);
	insertCol(VS_LC_COL_RIGHTDESC_HR, CResString::ex().getStrTblText(IDS_DESC ), LVCFMT_LEFT  , 200);
	
	return (TVConListCtrl<BConppAppRight, BppAppRight>::init());
}

/**---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*/
void PDlgRightListCtrl::refreshItem(const int iItem)
{
	assert(iItem > -1);

	BppAppRight * pRight = giveCon().getBsnObj(getIdx(iItem));
	assert(pRight != 0);

	SetItemText(iItem, VS_LC_COL_RIGHTNAME   ,  pRight->getRightname().c_str());
	SetItemText(iItem, VS_LC_COL_RIGHTDESC   ,  pRight->getRightDesc().c_str());
	SetItemText(iItem, VS_LC_COL_RIGHTDESC_EN,  pRight->getRightDesc_EN().c_str());
	SetItemText(iItem, VS_LC_COL_RIGHTDESC_FR,  pRight->getRightDesc_FR().c_str());
	SetItemText(iItem, VS_LC_COL_RIGHTDESC_HR,  pRight->getRightDesc_HR().c_str());
}

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
