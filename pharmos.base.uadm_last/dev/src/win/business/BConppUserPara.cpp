/******************************************************************************/
/**                                                                           */
/**   Author : T.ARNOLD NTTDATA                                               */
/**   Date   : 25.11.2025                                                     */
/**                                                                           */
/******************************************************************************/


/*----------------------------------------------------------------------------*/
// Includes                                                                   
/*----------------------------------------------------------------------------*/
#include "bsn.h"		// precompiled header

#include <tbl_ppuserpara.h>
#include <struct_userpara.h>
#include <SppUserpara.h>

#include "BppUserpara.h"
#include "BConppUserpara.h"


/**---------------------------------------------------------------------------*/
#define GIVEDATAUSERPARA		((SppUserPara::SListProj &)giveData())

SCBMEB_DEFINE_CONBSN(BConppUserPara, SppUserPara::SListProj, SppUserPara)

/**---------------------------------------------------------------------------*/
BConppUserPara::BConppUserPara()
{
	m_strName = "";
	m_bAllUserParas = true;
	m_iSortIndex = 0;
	m_bSortOrder = true; // ascending
}

/**---------------------------------------------------------------------------*/
BConppUserPara::~BConppUserPara()
{
}

void BConppUserPara::setName(CBString sName)
{
	m_strName = sName;
}

const CBString BConppUserPara::getName() const
{
	return m_strName;
}

/**---------------------------------------------------------------------------*/
void BConppUserPara::setBAll(bool bAllUserParas)
{
	m_bAllUserParas = bAllUserParas;
}
/**---------------------------------------------------------------------------*/
const bool BConppUserPara::getBAll() const
{
	return m_bAllUserParas;
}

void BConppUserPara::setSortIndex(int iSortIndex)
{
	m_iSortIndex = iSortIndex;
}

void BConppUserPara::toggleSortOrder()
{
	m_bSortOrder = !m_bSortOrder;
}

int	BConppUserPara::getSortIndex() const
{
	return m_iSortIndex;
}

bool BConppUserPara::getSortOrder() const
{
	return m_bSortOrder;
}

/**---------------------------------------------------------------------------*/
IPerst::ERet BConppUserPara::load()
{
	// set where condition
	strcpy(GIVEDATAUSERPARA.stListCond.USERPARANAME, getName().c_str());
	GIVEDATAUSERPARA.stListCond.ALLUSERPARAS = getBAll();
	GIVEDATAUSERPARA.stListCond.SORTINDEX = (short)getSortIndex();
	GIVEDATAUSERPARA.stListCond.SORTORDER = getSortOrder();

	if (TVConBsnObject<BppUserPara>::load() == IPerst::RET_ERROR)
		return IPerst::RET_ERROR;

	return IPerst::RET_OK;
}

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
