/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                   */
/**   Date   : 06.07.2004                                                     */
/**                                                                           */
/******************************************************************************/


#include "StdAfx.h"

#include <BppText.h>
#include <BConppText.h>

#include "PDlgTextListCtrl.h"


/*----------------------------------------------------------------------------*/
// Defines für Default-Spalten-Nummern
/*----------------------------------------------------------------------------*/
#define VS_LC_COL_REFTEXT		0

/**---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*/
bool PDlgTextListCtrl::init()
{
	insertCol(VS_LC_COL_REFTEXT  , CResString::ex().getStrTblText(IDS_LI_REFTEXT  ), LVCFMT_LEFT  , 240);
	
	return (TVConListCtrl<BConppText, BppText>::init());
}

/**---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*/
void PDlgTextListCtrl::refreshItem(const int iItem)
{
	assert(iItem > -1);

	BppText * pText = giveCon().getBsnObj(getIdx(iItem));
	assert(pText != 0);

	SetItemText(iItem, VS_LC_COL_REFTEXT , pText->getReftext().c_str());
	}

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
