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
#include <struct_usergroupanalysis.h>
#include <SUserGroupAnalysis.h>

#include "BUserGroupAnalysis.h"
#include "BConUserGroupAnalysis.h"


/**---------------------------------------------------------------------------*/
#define GIVEDATAUSERANALYSIS		((SUserGroupAnalysis::SListProj &)giveData())

SCBMEB_DEFINE_CONBSN(BConUserGroupAnalysis, SUserGroupAnalysis::SListProj, SUserGroupAnalysis)

/**---------------------------------------------------------------------------*/
BConUserGroupAnalysis::BConUserGroupAnalysis() : m_sAppid(0),m_sUserid(0)
{
}

/**---------------------------------------------------------------------------*/
BConUserGroupAnalysis::~BConUserGroupAnalysis()
{
}

/**---------------------------------------------------------------------------*/
void BConUserGroupAnalysis::setAppid	(short sAppid)
{
	m_sAppid = sAppid;
}

/**---------------------------------------------------------------------------*/
void BConUserGroupAnalysis::setUserid(long sUserid)
{
    m_sUserid = sUserid;
}

/**---------------------------------------------------------------------------*/
IPerst::ERet BConUserGroupAnalysis::load()
{
	// set where condition
    GIVEDATAUSERANALYSIS.stListCond.APPLICATIONID = m_sAppid;
    GIVEDATAUSERANALYSIS.stListCond.USERID	= m_sUserid;

	if (TVConBsnObject<BUserGroupAnalysis>::load() == IPerst::RET_ERROR)
		return IPerst::RET_ERROR;

	return IPerst::RET_OK;
}

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
