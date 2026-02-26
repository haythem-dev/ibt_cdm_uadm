/******************************************************************************/
/**                                                                           */
/**   Desc   : Container zur Aufnahme aller Branches / Regionen				*/
/**   Author : T. Hörath PRZ GmbH                                         */
/**   Date   : 14.06.2004                                                     */
/**                                                                           */
/******************************************************************************/


/*----------------------------------------------------------------------------*/
// Includes                                                                   
/*----------------------------------------------------------------------------*/
#include "bsn.h"		// precompiled header

#include <tbl_ppcorappbranchreg.h>
#include <tbl_ppregion.h>
#include <tbl_ppbranch.h>
#include <struct_branchreg.h>
#include <SppBranchReg.h>

#include "BppBranchReg.h"
#include "BConppBranchReg.h"


/**---------------------------------------------------------------------------*/
#define GIVEDATABRANCHREG		((SppBranchReg::SListProj &)giveData())

SCBMEB_DEFINE_CONBSN(BConppBranchReg, SppBranchReg::SListProj, SppBranchReg)

/**---------------------------------------------------------------------------*/
BConppBranchReg::BConppBranchReg() : m_lUserid(0)
{
}

/**---------------------------------------------------------------------------*/
BConppBranchReg::~BConppBranchReg()
{
}

/**---------------------------------------------------------------------------*/
void BConppBranchReg::setRegid	(short sRegid)
{
	m_sRegid = sRegid;
}
/**---------------------------------------------------------------------------*/
const short	BConppBranchReg::getRegid	() const
{
	return m_sRegid;
}

/**---------------------------------------------------------------------------*/
void BConppBranchReg::setAppid	(short sAppid)
{
	m_sAppid = sAppid;
}
/**---------------------------------------------------------------------------*/
void BConppBranchReg::setUserid	(long lUserid)
{
	m_lUserid = lUserid;
}
/**---------------------------------------------------------------------------*/
const short	BConppBranchReg::getAppid	() const
{
	return m_sAppid;
}
/**---------------------------------------------------------------------------*/
void BConppBranchReg::setBSelReg  (bool bReg)
{
	m_bReg = bReg;
}
/**---------------------------------------------------------------------------*/
void BConppBranchReg::setBSelBranch(bool bBranch)
{
	m_bBranch = bBranch;
}
/**---------------------------------------------------------------------------*/
const bool BConppBranchReg::getBSelReg  () const
{
	return m_bReg;
}

/**---------------------------------------------------------------------------*/
void BConppBranchReg::setBSelCor  (bool bCor)
{
	m_bCor = bCor;
}

/**---------------------------------------------------------------------------*/
const bool BConppBranchReg::getBSelCor  () const
{
	return m_bCor;
}

/**---------------------------------------------------------------------------*/
const bool BConppBranchReg::getBSelBranch() const
{
	return m_bBranch;
}

/**---------------------------------------------------------------------------*/
IPerst::ERet BConppBranchReg::load()
{
	// set where condition
	GIVEDATABRANCHREG.stListCond.APPLICATIONID = getAppid();
	GIVEDATABRANCHREG.stListCond.SELBRANCH	  = getBSelBranch();
	GIVEDATABRANCHREG.stListCond.SELREG		  = getBSelReg();
	GIVEDATABRANCHREG.stListCond.SELCOR		  = getBSelCor();
	GIVEDATABRANCHREG.stListRegion.REGIONID	  = getRegid();
	GIVEDATABRANCHREG.stListCond.USERID		  = m_lUserid;

	if (TVConBsnObject<BppBranchReg>::load() == IPerst::RET_ERROR)
		return IPerst::RET_ERROR;

	return IPerst::RET_OK;
}

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
