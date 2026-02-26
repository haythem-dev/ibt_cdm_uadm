/******************************************************************************/
/**                                                                           */
/**   Desc   : Container zur Aufnahme aller Previous-Einträge aus der Relation ppLoginPrevious*/
/**   Author : T. Hörath PRZ GmbH                                         */
/**   Date   : 16.08.2004                                                     */
/**                                                                           */
/******************************************************************************/


/*----------------------------------------------------------------------------*/
// Includes                                                                   
/*----------------------------------------------------------------------------*/
#include "bsn.h"		// precompiled header

#include <tbl_pploginprevious.h>
#include <struct_previous.h>
#include <SppPrev.h>

#include "BppPrev.h"
#include "BConppPrev.h"


/**---------------------------------------------------------------------------*/
#define GIVEDATAPREV		((SppPrev::SListProj &)giveData())

SCBMEB_DEFINE_CONBSN(BConppPrev, SppPrev::SListProj, SppPrev)

/**---------------------------------------------------------------------------*/
BConppPrev::BConppPrev()
{
}

/**---------------------------------------------------------------------------*/
BConppPrev::~BConppPrev()
{
}

/**---------------------------------------------------------------------------*/
void BConppPrev::setAppid	(short sAppid)
{
	m_sAppid = sAppid;
}
/**---------------------------------------------------------------------------*/
const short	BConppPrev::getAppid	() const
{
	return m_sAppid;
}

/**---------------------------------------------------------------------------*/
IPerst::ERet BConppPrev::load()
{
	// set where condition
	GIVEDATAPREV.stListCond.APPLICATIONID = getAppid();

	if (TVConBsnObject<BppPrev>::load() == IPerst::RET_ERROR)
		return IPerst::RET_ERROR;

	return IPerst::RET_OK;
}

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
