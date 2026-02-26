/******************************************************************************/
/**                                                                           */
/**   Desc   : Container zur Aufnahme aller User aus der Relation ppUser      */
/**   Author : T. Hörath PRZ GmbH                                         */
/**   Date   : 25.05.2004                                                     */
/**                                                                           */
/******************************************************************************/


/*----------------------------------------------------------------------------*/
// Includes                                                                   
/*----------------------------------------------------------------------------*/
#include "bsn.h"		// precompiled header

#include <tbl_ppuser.h>
#include <struct_user.h>
#include <SppUser.h>

#include "BppUser.h"
#include "BConppUser.h"


/**---------------------------------------------------------------------------*/
#define GIVEDATAUSER		((SppUser::SListProj &)giveData())

SCBMEB_DEFINE_CONBSN(BConppUser, SppUser::SListProj, SppUser)

/**---------------------------------------------------------------------------*/
BConppUser::BConppUser() : m_sBranchRegid(0)
{
}

/**---------------------------------------------------------------------------*/
BConppUser::~BConppUser()
{
}

/**---------------------------------------------------------------------------*/
void BConppUser::setAppid	(short sAppid)
{
	m_sAppid = sAppid;
}
/**---------------------------------------------------------------------------*/
const short	BConppUser::getAppid	() const
{
	return m_sAppid;
}

/**---------------------------------------------------------------------------*/
void BConppUser::setGroupid  (short sGroupid)
{
	m_sGroupid = sGroupid;
}

/**---------------------------------------------------------------------------*/
const short  BConppUser::getGroupid  () const
{
	return m_sGroupid;
}
/**---------------------------------------------------------------------------*/
void BConppUser::setBranchRegid	(short sBranchRegid)
{
	m_sBranchRegid = sBranchRegid;
}
/**---------------------------------------------------------------------------*/
void BConppUser::setCorUsrGrp  (bool bCorUsrGrp)
{
	m_bCorUsrGrp = bCorUsrGrp;
}
/**---------------------------------------------------------------------------*/
const bool BConppUser::getCorUsrGrp  () const
{
	return m_bCorUsrGrp;
}


/**---------------------------------------------------------------------------*/
IPerst::ERet BConppUser::load()
{
	// set where conditions
	GIVEDATAUSER.stListCond.APPLICATIONID = getAppid();
	GIVEDATAUSER.stListCond.GROUPID		 = getGroupid();
	GIVEDATAUSER.stListCond.BRANCHREGID	 = m_sBranchRegid;
	GIVEDATAUSER.stListCond.CORUSRGRP	 = getCorUsrGrp();

	if (TVConBsnObject<BppUser>::load() == IPerst::RET_ERROR)
		return IPerst::RET_ERROR;

	// load holt sich per default nur bestimmte Anzahl an Datensätzen
	
	// load all mebers from the database
/*	BppUser * BsnObj;
	unsigned int uiCount = 0;
	BsnObj = getBsnObj(uiCount);

	while (BsnObj != 0)
	{		
		uiCount++;
		BsnObj = getBsnObj(uiCount);
	}
*/
	return IPerst::RET_OK;
}

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
