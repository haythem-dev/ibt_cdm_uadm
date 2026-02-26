/******************************************************************************/
/**                                                                           */
/**   Author : T.ARNOLD NTTDATA                                               */
/**   Date   : 25.11.2025                                                     */
/**                                                                           */
/******************************************************************************/


#include "StdAfx.h"

#include <BppUserPara.h>
#include <BConppUserPara.h>

#include "PDlgUserParaListCtrl.h"


/*----------------------------------------------------------------------------*/
// Defines für Default-Spalten-Nummern
/*----------------------------------------------------------------------------*/
#define VS_LC_COL_USERPARAID		0
#define VS_LC_COL_USERPARANAME		1
#define VS_LC_COL_USERPARADESC		2
#define VS_LC_COL_USERPARACNT		3

/**---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*/
bool PDlgUserParaListCtrl::init()
{
	insertCol(VS_LC_COL_USERPARAID, CResString::ex().getStrTblText(IDS_DLG_USERPARA_LI_ID), LVCFMT_RIGHT, 40);
	insertCol(VS_LC_COL_USERPARANAME, CResString::ex().getStrTblText(IDS_DLG_USERPARA_LI_NAME), LVCFMT_LEFT, 220);
	insertCol(VS_LC_COL_USERPARADESC, CResString::ex().getStrTblText(IDS_DLG_USERPARA_LI_DESC), LVCFMT_LEFT, 300);
	insertCol(VS_LC_COL_USERPARACNT, CResString::ex().getStrTblText(IDS_DLG_USERPARA_LI_COUNT), LVCFMT_RIGHT, 50);

	return (TVConListCtrl<BConppUserPara, BppUserPara>::init());
}

/**---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*/
void PDlgUserParaListCtrl::refreshItem(const int iItem)
{
	assert(iItem > -1);

	BppUserPara* pUserpara = giveCon().getBsnObj(getIdx(iItem));
	assert(pUserpara != 0);

	SetItemText(iItem, VS_LC_COL_USERPARAID, pUserpara->getUserParaidStr().c_str());
	SetItemText(iItem, VS_LC_COL_USERPARANAME, pUserpara->getName().c_str());
	SetItemText(iItem, VS_LC_COL_USERPARADESC, pUserpara->getDescription().c_str());
	SetItemText(iItem, VS_LC_COL_USERPARACNT, pUserpara->getUserCountStr().c_str());
}

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
