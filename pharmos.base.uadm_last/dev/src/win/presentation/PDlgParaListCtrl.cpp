/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                   */
/**   Date   : 03.08.2004                                                     */
/**                                                                           */
/******************************************************************************/


#include "StdAfx.h"

#include <BppPara.h>
#include <BConppPara.h>

#include "PDlgParaListCtrl.h"


/*----------------------------------------------------------------------------*/
// Defines für Default-Spalten-Nummern
/*----------------------------------------------------------------------------*/
#define VS_LC_PARA_COL_1        0
#define VS_LC_PARA_COL_2        1
#define VS_LC_PARA_COL_BEZ      2
#define VS_LC_PARA_COL_VAL		3

/**---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*/
bool PDlgParaListCtrl::init()
{
	insertCol(VS_LC_PARA_COL_1    , CResString::ex().getStrTblText(IDS_LI_PARA1  ), LVCFMT_LEFT  , 100);
	insertCol(VS_LC_PARA_COL_2  ,   CResString::ex().getStrTblText(IDS_LI_PARA2  ), LVCFMT_LEFT  , 100);
	insertCol(VS_LC_PARA_COL_BEZ  , CResString::ex().getStrTblText(IDS_LI_PARABEZ  ), LVCFMT_LEFT  , 130);
	insertCol(VS_LC_PARA_COL_VAL  , CResString::ex().getStrTblText(IDS_LI_PARAVAL  ), LVCFMT_LEFT  , 200);
	
	return (TVConListCtrl<BConppPara, BppPara>::init());
}

/**---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*/
void PDlgParaListCtrl::refreshItem(const int iItem)
{
	assert(iItem > -1);

	BppPara * pPara = giveCon().getBsnObj(getIdx(iItem));
	assert(pPara != 0);

	SetItemText(iItem, VS_LC_PARA_COL_1     ,  pPara->getPara1().c_str());
	SetItemText(iItem, VS_LC_PARA_COL_2   ,    pPara->getPara2().c_str());
	SetItemText(iItem, VS_LC_PARA_COL_BEZ   ,  pPara->getParabez().c_str());
	SetItemText(iItem, VS_LC_PARA_COL_VAL   ,  pPara->getParavalue().c_str());
}

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
