/******************************************************************************/
/**                                                                           */
/**   Desc   : Container zur Aufnahme aller Module aus der Relation ppAppModule*/
/**   Author : T. Hörath PRZ GmbH                                         */
/**   Date   : 14.06.2004                                                     */
/**                                                                           */
/******************************************************************************/


/*----------------------------------------------------------------------------*/
// Includes                                                                   
/*----------------------------------------------------------------------------*/
#include "bsn.h"		// precompiled header

#include <tbl_ppappmodule.h>
#include <struct_appmodule.h>
#include <SppMod.h>

#include "BppAppMod.h"
#include "BConppAppMod.h"


/**---------------------------------------------------------------------------*/
#define GIVEDATAMODULE		((SppMod::SListProj &)giveData())

SCBMEB_DEFINE_CONBSN(BConppAppMod, SppMod::SListProj, SppMod)

/**---------------------------------------------------------------------------*/
BConppAppMod::BConppAppMod()
{
}

/**---------------------------------------------------------------------------*/
BConppAppMod::~BConppAppMod()
{
}

/**---------------------------------------------------------------------------*/
void BConppAppMod::setAppid	(short sAppid)
{
	m_sAppid = sAppid;
}
/**---------------------------------------------------------------------------*/
const short	BConppAppMod::getAppid	() const
{
	return m_sAppid;
}

/**---------------------------------------------------------------------------*/
IPerst::ERet BConppAppMod::load()
{
	// set where condition
	GIVEDATAMODULE.stListCond.APPLICATIONID = getAppid();

	if (TVConBsnObject<BppAppMod>::load() == IPerst::RET_ERROR)
		return IPerst::RET_ERROR;

	return IPerst::RET_OK;
}

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
