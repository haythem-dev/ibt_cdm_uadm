/******************************************************************************/
/**                                                                           */
/**   Desc   : Container zur Aufnahme aller Groups aus der Relation ppGroup   */
/**   Author : T. Hörath PRZ GmbH                                         */
/**   Date   : 09.06.2004                                                     */
/**                                                                           */
/******************************************************************************/


/*----------------------------------------------------------------------------*/
// Includes                                                                   
/*----------------------------------------------------------------------------*/
#include "bsn.h"		// precompiled header

#include <tbl_ppgroup.h>
#include <struct_group.h>
#include <SppGroup.h>

#include "BppGroup.h"
#include "BConppGroup.h"


/**---------------------------------------------------------------------------*/
#define GIVEDATAGROUP		((SppGroup::SListProj &)giveData())

SCBMEB_DEFINE_CONBSN(BConppGroup, SppGroup::SListProj, SppGroup)

/**---------------------------------------------------------------------------*/
BConppGroup::BConppGroup()
{
}

/**---------------------------------------------------------------------------*/
BConppGroup::~BConppGroup()
{
}

/**---------------------------------------------------------------------------*/
void BConppGroup::setAppid	(short sAppid)
{
	m_sAppid = sAppid;
}
/**---------------------------------------------------------------------------*/
const short	BConppGroup::getAppid	() const
{
	return m_sAppid;
}

/**---------------------------------------------------------------------------*/
void BConppGroup::setBAll(bool bAllGrp)
{
	m_bAllGrp = bAllGrp;
}
/**---------------------------------------------------------------------------*/
const bool BConppGroup::getBAll() const
{
	return m_bAllGrp;
}

/**---------------------------------------------------------------------------*/
IPerst::ERet BConppGroup::load()
{
	// set where condition
	GIVEDATAGROUP.stListCond.APPLICATIONID = getAppid();
	GIVEDATAGROUP.stListCond.ALLGRP = getBAll();


	if (TVConBsnObject<BppGroup>::load() == IPerst::RET_ERROR)
		return IPerst::RET_ERROR;

	return IPerst::RET_OK;
}

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
