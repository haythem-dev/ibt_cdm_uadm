/******************************************************************************/
/**                                                                           */
/**   Desc   : Container zur Rechte-Analyse									  */
/**   Author : T. Hörath PRZ GmbH                                             */
/**   Date   : 30.08.2004                                                     */
/**                                                                           */
/******************************************************************************/


/*----------------------------------------------------------------------------*/
// Includes                                                                   
/*----------------------------------------------------------------------------*/
#include "bsn.h"		// precompiled header

#include <tbl_ppappmodule.h>
#include <tbl_ppapplication.h>
#include <tbl_ppappmodright.h>
#include <tbl_ppuser.h>
#include <struct_rightanalysis.h>
#include <SRightAnalysis.h>

#include "BRightAnalysis.h"
#include "BConRightAnalysis.h"


/**---------------------------------------------------------------------------*/
#define GIVEDATAANALYSIS		((SRightAnalysis::SListProj &)giveData())

SCBMEB_DEFINE_CONBSN(BConRightAnalysis, SRightAnalysis::SListProj, SRightAnalysis)

/**---------------------------------------------------------------------------*/
BConRightAnalysis::BConRightAnalysis() : m_sBranchRegid (0)
{
}

/**---------------------------------------------------------------------------*/
BConRightAnalysis::~BConRightAnalysis()
{
}

/**---------------------------------------------------------------------------*/
void BConRightAnalysis::setAppid	(short sAppid)
{
	m_sAppid = sAppid;
}

/**---------------------------------------------------------------------------*/
void BConRightAnalysis::setUserid	(short sUserid)
{
	m_sUserid = sUserid;
}
/**---------------------------------------------------------------------------*/
void BConRightAnalysis::setBranchRegid	(short sBranchRegid)
{
	m_sBranchRegid = sBranchRegid;
}

/**---------------------------------------------------------------------------*/
void BConRightAnalysis::setUsrOnly	(bool bUsrOnly)
{
	m_bUsrOnly = bUsrOnly;
}

/**---------------------------------------------------------------------------*/
void BConRightAnalysis::setAppOnly	(bool bAppOnly)
{
	m_bAppOnly = bAppOnly;
}

/**---------------------------------------------------------------------------*/
IPerst::ERet BConRightAnalysis::load()
{
	// set where condition
	GIVEDATAANALYSIS.stListCond.APPLICATIONID = m_sAppid;
	GIVEDATAANALYSIS.stListCond.USERID = m_sUserid;
	GIVEDATAANALYSIS.stListCond.BRANCHREGID = m_sBranchRegid;
	GIVEDATAANALYSIS.stListCond.APPONLY = m_bAppOnly;
	GIVEDATAANALYSIS.stListCond.USRONLY = m_bUsrOnly;

	if (TVConBsnObject<BRightAnalysis>::load() == IPerst::RET_ERROR)
		return IPerst::RET_ERROR;

	return IPerst::RET_OK;
}

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
