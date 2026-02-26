/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                   */
/**   Date   : 05.07.2004                                                     */
/**                                                                           */
/******************************************************************************/


#include "StdAfx.h"

#include <BppLanguage.h>
#include <BConppLanguage.h>

#include "PDlgLangListCtrl.h"


/*----------------------------------------------------------------------------*/
// Defines für Default-Spalten-Nummern
/*----------------------------------------------------------------------------*/
#define VS_LC_COL_LANGID		0
#define VS_LC_COL_LANGNAME		1


/**---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*/
bool PDlgLangListCtrl::init()
{
	insertCol(VS_LC_COL_LANGID  , CResString::ex().getStrTblText(IDS_LI_ID  ), LVCFMT_LEFT  , 40);
	insertCol(VS_LC_COL_LANGNAME  , CResString::ex().getStrTblText(IDS_LI_BEZ  ), LVCFMT_LEFT  , 150);
	
	return (TVConListCtrl<BConppLanguage, BppLanguage>::init());
}

/**---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*/
void PDlgLangListCtrl::refreshItem(const int iItem)
{
	assert(iItem > -1);

	BppLanguage * pLang = giveCon().getBsnObj(getIdx(iItem));
	assert(pLang != 0);

	SetItemText(iItem, VS_LC_COL_LANGID , pLang->getLangid().c_str());
	SetItemText(iItem, VS_LC_COL_LANGNAME , pLang->getLangname().c_str());
}

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
