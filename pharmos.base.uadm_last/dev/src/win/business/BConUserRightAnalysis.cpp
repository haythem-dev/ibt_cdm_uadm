/******************************************************************************/
/**                                                                           */
/**   Desc   : Container zur Rechte-Analyse									  */
/**   Author : T. Hörath PRZ GmbH                                             */
/**   Date   : 06.10.2006                                                     */
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
#include <struct_userrightanalysis.h>
#include <SUserRightAnalysis.h>

#include "BUserRightAnalysis.h"
#include "BConUserRightAnalysis.h"


/**---------------------------------------------------------------------------*/
#define GIVEDATAUSERANALYSIS		((SUserRightAnalysis::SListProj &)giveData())

SCBMEB_DEFINE_CONBSN(BConUserRightAnalysis, SUserRightAnalysis::SListProj, SUserRightAnalysis)

/**---------------------------------------------------------------------------*/
BConUserRightAnalysis::BConUserRightAnalysis() : m_sAppid(0), m_sBrRegid(0)
{
}

/**---------------------------------------------------------------------------*/
BConUserRightAnalysis::~BConUserRightAnalysis()
{
}

/**---------------------------------------------------------------------------*/
void BConUserRightAnalysis::setAppid	(short sAppid)
{
	m_sAppid = sAppid;
}

/**---------------------------------------------------------------------------*/
void BConUserRightAnalysis::setUserid(long sUserid)
{
    m_sUserId = sUserid;
}

/**---------------------------------------------------------------------------*/
void BConUserRightAnalysis::setBranchRegid	(short sBrRegid)
{
	m_sBrRegid = sBrRegid;
}

/**---------------------------------------------------------------------------*/
IPerst::ERet BConUserRightAnalysis::load()
{
	// set where condition
    GIVEDATAUSERANALYSIS.stListCond.APPLICATIONID = m_sAppid;
    GIVEDATAUSERANALYSIS.stListCond.USERID	= m_sUserId;
    GIVEDATAUSERANALYSIS.stListCond.BRANCHID = m_sBrRegid;

	if (TVConBsnObject<BUserRightAnalysis>::load() == IPerst::RET_ERROR)
		return IPerst::RET_ERROR;

	return IPerst::RET_OK;
}

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
