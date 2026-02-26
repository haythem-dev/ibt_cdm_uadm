/******************************************************************************/
/**                                                                           */
/**   Desc   : Container zur Aufnahme aller Parameter aus der Relation ppLogPara*/
/**   Author : T. Hörath PRZ GmbH                                         */
/**   Date   : 03.08.2004                                                     */
/**                                                                           */
/******************************************************************************/


/*----------------------------------------------------------------------------*/
// Includes                                                                   
/*----------------------------------------------------------------------------*/
#include "bsn.h"		// precompiled header

#include <tbl_pplogpara.h>
#include <struct_para.h>
#include <SppPara.h>

#include "BppPara.h"
#include "BConppPara.h"


/**---------------------------------------------------------------------------*/
#define GIVEDATAPARA		((SppPara::SListProj &)giveData())

SCBMEB_DEFINE_CONBSN(BConppPara, SppPara::SListProj, SppPara)

/**---------------------------------------------------------------------------*/
BConppPara::BConppPara()
{
}

/**---------------------------------------------------------------------------*/
BConppPara::~BConppPara()
{
}

/**---------------------------------------------------------------------------*/
void BConppPara::setAppid	(short sAppid)
{
	m_sAppid = sAppid;
}
/**---------------------------------------------------------------------------*/
const short	BConppPara::getAppid	() const
{
	return m_sAppid;
}

/**---------------------------------------------------------------------------*/
IPerst::ERet BConppPara::load()
{
	// set where condition
	GIVEDATAPARA.stListCond.APPLICATIONID = getAppid();

	if (TVConBsnObject<BppPara>::load() == IPerst::RET_ERROR)
		return IPerst::RET_ERROR;

	return IPerst::RET_OK;
}

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
