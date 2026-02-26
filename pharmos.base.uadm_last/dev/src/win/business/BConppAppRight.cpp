/******************************************************************************/
/**                                                                           */
/**   Desc   : Container zur Aufnahme aller Rechte aus der Relation ppAppRight*/
/**   Author : T. Hörath PRZ GmbH                                         */
/**   Date   : 15.06.2004                                                     */
/**                                                                           */
/******************************************************************************/


/*----------------------------------------------------------------------------*/
// Includes                                                                   
/*----------------------------------------------------------------------------*/
#include "bsn.h"		// precompiled header

#include <tbl_ppappright.h>
#include <tbl_ppappmodright.h>
#include <struct_appright.h>
#include <SppAppRight.h>
#include "defines.h"

#include "BppAppRight.h"
#include "BConppAppRight.h"


/**---------------------------------------------------------------------------*/
#define GIVEDATARIGHTS		((SppAppRight::SListProj &)giveData())

SCBMEB_DEFINE_CONBSN(BConppAppRight, SppAppRight::SListProj, SppAppRight)

/**---------------------------------------------------------------------------*/
BConppAppRight::BConppAppRight() : m_sCategory(RIGHT_CATEGORY_NONE)
{
}

/**---------------------------------------------------------------------------*/
BConppAppRight::~BConppAppRight()
{
}

/**---------------------------------------------------------------------------*/
void BConppAppRight::setAppid	(short sAppid)
{
	m_sAppid = sAppid;
}
/**---------------------------------------------------------------------------*/
const short	BConppAppRight::getAppid	() const
{
	return m_sAppid;
}
/**---------------------------------------------------------------------------*/
void BConppAppRight::setBranchRegid	(short sBranchRegid)
{
	m_sBranchRegid = sBranchRegid;
}
/**---------------------------------------------------------------------------*/
const short	BConppAppRight::getBranchRegid	() const
{
	return m_sBranchRegid;
}

/**---------------------------------------------------------------------------*/
void BConppAppRight::setGroupid	(short sGroupid)
{
	m_sGroupid = sGroupid;
}
/**---------------------------------------------------------------------------*/
const short	BConppAppRight::getGroupid	() const
{
	return m_sGroupid;
}

/**---------------------------------------------------------------------------*/
void BConppAppRight::setModid	(short sModid)
{
	m_sModid = sModid;
}
/**---------------------------------------------------------------------------*/
const short	BConppAppRight::getModid	() const
{
	return m_sModid;
}

/**---------------------------------------------------------------------------*/
void BConppAppRight::setUserid	(short sUserid)
{
	m_sUserid = sUserid;
}
/**---------------------------------------------------------------------------*/
const short	BConppAppRight::getUserid	() const
{
	return m_sUserid;
}
/**---------------------------------------------------------------------------*/
void BConppAppRight::setRightCategory	(short sCategory)
{
	m_sCategory = sCategory;
}
/**---------------------------------------------------------------------------*/
const short	BConppAppRight::getRightCategory	() const
{
	return m_sCategory;
}

/**---------------------------------------------------------------------------*/
void BConppAppRight::setBCorRights    (bool  bCorRight)
{
	m_bCorRight = bCorRight;
}

/**---------------------------------------------------------------------------*/
const bool	BConppAppRight::getBCorRights    () const
{
	return m_bCorRight;
}

/**---------------------------------------------------------------------------*/
void BConppAppRight::setBCorGroupRights    (bool  bCorGroupRight)
{
	m_bCorGroupRight = bCorGroupRight;
}

/**---------------------------------------------------------------------------*/
const bool	BConppAppRight::getBCorGroupRights    () const
{
	return m_bCorGroupRight;
}
/**---------------------------------------------------------------------------*/
void BConppAppRight::setBModRights	(bool bModRights)
{
	m_bModRights = bModRights;
}
	
/**---------------------------------------------------------------------------*/
const bool BConppAppRight::getBModRights	() const
{
	return m_bModRights;
}

/**---------------------------------------------------------------------------*/
IPerst::ERet BConppAppRight::load()
{
	// set where condition
	GIVEDATARIGHTS.stListCond.APPLICATIONID	= getAppid();
	GIVEDATARIGHTS.stListCond.BRANCHREGID	= getBranchRegid();
	GIVEDATARIGHTS.stListCond.GROUPID		= getGroupid();
	GIVEDATARIGHTS.stListCond.MODULEID		= getModid();
	GIVEDATARIGHTS.stListCond.USERID		= getUserid();
	GIVEDATARIGHTS.stListCond.CORAPPRIGHT	= getBCorRights();
	GIVEDATARIGHTS.stListCond.CORGROUPRIGHT	= getBCorGroupRights();
	GIVEDATARIGHTS.stListCond.MODRIGHTS  	= getBModRights();
	GIVEDATARIGHTS.stListCond.CATEGORY		= getRightCategory();

	if (TVConBsnObject<BppAppRight>::load() == IPerst::RET_ERROR)
		return IPerst::RET_ERROR;

	return IPerst::RET_OK;
}

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
