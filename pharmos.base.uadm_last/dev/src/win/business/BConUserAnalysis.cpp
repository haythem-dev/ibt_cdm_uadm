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
#include <struct_useranalysis.h>
#include <SUserAnalysis.h>

#include "BUserAnalysis.h"
#include "BConUserAnalysis.h"


/**---------------------------------------------------------------------------*/
#define GIVEDATAUSERANALYSIS		((SUserAnalysis::SListProj &)giveData())

SCBMEB_DEFINE_CONBSN(BConUserAnalysis, SUserAnalysis::SListProj, SUserAnalysis)

/**---------------------------------------------------------------------------*/
BConUserAnalysis::BConUserAnalysis() : m_sAreaid (0)
{
}

/**---------------------------------------------------------------------------*/
BConUserAnalysis::~BConUserAnalysis()
{
}

/**---------------------------------------------------------------------------*/
void BConUserAnalysis::setAppid	(short sAppid)
{
	m_sAppid = sAppid;
}

/**---------------------------------------------------------------------------*/
void BConUserAnalysis::setModid	(short sModid)
{
	m_sModid = sModid;
}

/**---------------------------------------------------------------------------*/
void BConUserAnalysis::setAreaid	(short sAreaid)
{
	m_sAreaid = sAreaid;
}

/**---------------------------------------------------------------------------*/
void BConUserAnalysis::setRightname(CBString cbRightname)
{
	m_cbRightname = cbRightname;
}

/**---------------------------------------------------------------------------*/
CBString BConUserAnalysis::getRightname()
{
	return m_cbRightname;
}


/**---------------------------------------------------------------------------*/
IPerst::ERet BConUserAnalysis::load()
{
	// set where condition
	GIVEDATAUSERANALYSIS.stListCond.APPLICATIONID	= m_sAppid;
	GIVEDATAUSERANALYSIS.stListCond.MODULEID		= m_sModid;
	GIVEDATAUSERANALYSIS.stListCond.BRANCHREGID		= m_sAreaid;
	strcpy (GIVEDATAUSERANALYSIS.stListCond.RIGHTNAME, m_cbRightname.c_str() );


	if (TVConBsnObject<BUserAnalysis>::load() == IPerst::RET_ERROR)
		return IPerst::RET_ERROR;

	return IPerst::RET_OK;
}

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
