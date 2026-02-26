/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                   */
/**   Date   : 26.05.2004                                                     */
/**                                                                           */
/******************************************************************************/


#include "StdAfx.h"

#include <BppUser.h>
#include <BConppUser.h>

#include "PDlgUserListCtrl.h"


/*----------------------------------------------------------------------------*/
// Defines für Default-Spalten-Nummern
/*----------------------------------------------------------------------------*/
#define VS_LC_COL_SURNAME		1
#define VS_LC_COL_FORENAME      2
#define VS_LC_COL_USERNAME      0
#define VS_LC_COL_USERCOUNTRY   3
#define VS_LC_COL_USERTYPE	    4

/**---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*/
bool PDlgUserListCtrl::init()
{
	insertCol(VS_LC_COL_SURNAME    , CResString::ex().getStrTblText(IDS_VIEW_USER_LI_SURNAME    ), LVCFMT_LEFT  , 70);
	insertCol(VS_LC_COL_FORENAME   , CResString::ex().getStrTblText(IDS_VIEW_USER_LI_FORENAME   ), LVCFMT_LEFT  , 60);
	insertCol(VS_LC_COL_USERNAME   , CResString::ex().getStrTblText(IDS_VIEW_USER_LI_USERNAME   ), LVCFMT_LEFT  , 100);
	insertCol(VS_LC_COL_USERCOUNTRY, CResString::ex().getStrTblText(IDS_VIEW_USER_LI_USERCOUNTRY), LVCFMT_LEFT  , 55);
	insertCol(VS_LC_COL_USERTYPE   , CResString::ex().getStrTblText(IDS_VIEW_USER_LI_USERTYPE   ), LVCFMT_LEFT  , 100);

	return (TVConListCtrl<BConppUser, BppUser>::init());
}

/**---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*/
void PDlgUserListCtrl::refreshItem(const int iItem)
{
	assert(iItem > -1);

	BppUser * pUser = giveCon().getBsnObj(getIdx(iItem));
	assert(pUser != 0);

	SetItemText(iItem, VS_LC_COL_SURNAME   , pUser->getSurname   ().c_str());
	SetItemText(iItem, VS_LC_COL_FORENAME  , pUser->getForename  ().c_str());
	SetItemText(iItem, VS_LC_COL_USERNAME  , pUser->getUsername  ().c_str());
	SetItemText(iItem, VS_LC_COL_USERCOUNTRY, pUser->getListUserCountry().c_str());
	SetItemText(iItem, VS_LC_COL_USERTYPE  , pUser->getListUserType().c_str());
}

/**---------------------------------------------------------------------------*/
bool PDlgUserListCtrl::searchSurname(const CBString& strFirst)
{
	BppUser*		pUser = 0;
	unsigned int    uiCnt = 0;
	CBString		cbFirstChar;
	CBString		cbSurname;

	int iSel = -1;
	getNextSelectedBsnObj(&iSel);
	
	if ( iSel >= 0 )
		unselectBsnObj(getIdx(iSel));
	
	while ((pUser = giveCon().getBsnObj(uiCnt++)) != 0)
	{
		cbSurname = pUser->getSurname();
		if ( !cbSurname.empty() )
		{
			cbFirstChar = cbSurname.substr(0,1);
			cbFirstChar.upper();

			if ( cbFirstChar == strFirst )
			{
				selectBsnObj( uiCnt - 1 );
				return true;
				break;
			}
		}
	}
	selectBsnObj(getIdx(iSel)); // alte Selektion wieder setzen
	return false; //Key nicht gefunden
}

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
