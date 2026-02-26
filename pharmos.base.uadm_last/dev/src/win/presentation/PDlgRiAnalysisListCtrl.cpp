/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                   */
/**   Date   : 30.08.2004                                                     */
/**                                                                           */
/******************************************************************************/


#include "StdAfx.h"

#include <BRightAnalysis.h>
#include <BConRightAnalysis.h>

#include "PDlgRiAnalysisListCtrl.h"


/*----------------------------------------------------------------------------*/
// Defines für Default-Spalten-Nummern
/*----------------------------------------------------------------------------*/
#define VS_LC_COL_ANALYSIS_APP	        0
#define VS_LC_COL_ANALYSIS_MOD          1
#define VS_LC_COL_ANALYSIS_AREA         2
#define VS_LC_COL_ANALYSIS_USERNAME     3
#define VS_LC_COL_ANALYSIS_SURNAME      4
#define VS_LC_COL_ANALYSIS_FORENAME     5
#define VS_LC_COL_ANALYSIS_RIGHT        6
#define VS_LC_COL_ANALYSIS_RIGHTDESC    7
#define VS_LC_COL_ANALYSIS_RIGHTDESC_EN 8
#define VS_LC_COL_ANALYSIS_RIGHTDESC_FR 9
#define VS_LC_COL_ANALYSIS_RIGHTDESC_HR 10

/**---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*/
bool PDlgRiAnalysisListCtrl::init()
{
	insertCol(VS_LC_COL_ANALYSIS_APP  , CResString::ex().getStrTblText(IDS_APP   ), LVCFMT_LEFT  , 50);
	insertCol(VS_LC_COL_ANALYSIS_MOD  , CResString::ex().getStrTblText(IDS_MOD  ), LVCFMT_LEFT  , 70);
	insertCol(VS_LC_COL_ANALYSIS_AREA , CResString::ex().getStrTblText(IDS_VIEW_PREV_LI_AREA   ), LVCFMT_LEFT  , 70);
	insertCol(VS_LC_COL_ANALYSIS_USERNAME  , CResString::ex().getStrTblText(IDS_USERNAME  ), LVCFMT_LEFT  , 70);
	insertCol(VS_LC_COL_ANALYSIS_SURNAME   , CResString::ex().getStrTblText(IDS_SURNAME   ), LVCFMT_LEFT  , 70);
	insertCol(VS_LC_COL_ANALYSIS_FORENAME  , CResString::ex().getStrTblText(IDS_FORENAME  ), LVCFMT_LEFT  , 70);
	insertCol(VS_LC_COL_ANALYSIS_RIGHT     , CResString::ex().getStrTblText(IDS_RIGHT   ), LVCFMT_LEFT  , 100);

	insertCol(VS_LC_COL_ANALYSIS_RIGHTDESC	, CResString::ex().getStrTblText(IDS_DESC ), LVCFMT_LEFT  , 50);
	insertCol(VS_LC_COL_ANALYSIS_RIGHTDESC_EN, CResString::ex().getStrTblText(IDS_DESC ), LVCFMT_LEFT  , 50);
	insertCol(VS_LC_COL_ANALYSIS_RIGHTDESC_FR, CResString::ex().getStrTblText(IDS_DESC ), LVCFMT_LEFT  , 50);
	insertCol(VS_LC_COL_ANALYSIS_RIGHTDESC_HR, CResString::ex().getStrTblText(IDS_DESC ), LVCFMT_LEFT  , 50);
		
	return (TVConListCtrl<BConRightAnalysis, BRightAnalysis>::init());
}

/**---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*/
void PDlgRiAnalysisListCtrl::refreshItem(const int iItem)
{
	assert(iItem > -1);

	BRightAnalysis * pAnaly = giveCon().getBsnObj(getIdx(iItem));
	assert(pAnaly != 0);

	SetItemText(iItem, VS_LC_COL_ANALYSIS_APP   ,  pAnaly->getAppname().c_str());
	SetItemText(iItem, VS_LC_COL_ANALYSIS_MOD   ,  pAnaly->getModname().c_str());
	SetItemText(iItem, VS_LC_COL_ANALYSIS_AREA  ,  pAnaly->getAreaname().c_str());
	SetItemText(iItem, VS_LC_COL_ANALYSIS_USERNAME  ,  pAnaly->getUsername().c_str());
	SetItemText(iItem, VS_LC_COL_ANALYSIS_SURNAME   ,  pAnaly->getSurname().c_str());
	SetItemText(iItem, VS_LC_COL_ANALYSIS_FORENAME  ,  pAnaly->getForename().c_str());
	SetItemText(iItem, VS_LC_COL_ANALYSIS_RIGHT     ,  pAnaly->getRightname().c_str());	
	SetItemText(iItem, VS_LC_COL_ANALYSIS_RIGHTDESC   ,  pAnaly->getRightDesc().c_str());
	SetItemText(iItem, VS_LC_COL_ANALYSIS_RIGHTDESC_EN,  pAnaly->getRightDesc_EN().c_str());
	SetItemText(iItem, VS_LC_COL_ANALYSIS_RIGHTDESC_FR,  pAnaly->getRightDesc_FR().c_str());
	SetItemText(iItem, VS_LC_COL_ANALYSIS_RIGHTDESC_HR,  pAnaly->getRightDesc_HR().c_str());
}

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
