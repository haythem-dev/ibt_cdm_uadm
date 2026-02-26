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

#include <tbl_ppusertype.h>
#include <struct_usertype.h>
#include <SppUsertype.h>

#include "BppUsertype.h"
#include "BConppUsertype.h"


/**---------------------------------------------------------------------------*/
#define GIVEDATAUSERTYPE		((SppUsertype::SListProj &)giveData())

SCBMEB_DEFINE_CONBSN(BConppUsertype, SppUsertype::SListProj, SppUsertype)

/**---------------------------------------------------------------------------*/
BConppUsertype::BConppUsertype()
{
	m_strName = "";
	m_bAllUsertypes = true;
	m_iSortIndex = 0;
	m_bSortOrder = true; // ascending
}

/**---------------------------------------------------------------------------*/
BConppUsertype::~BConppUsertype()
{
}

void BConppUsertype::setName(CBString sName)
{
	m_strName = sName;
}

const CBString BConppUsertype::getName() const
{
	return m_strName;
}

/**---------------------------------------------------------------------------*/
void BConppUsertype::setBAll(bool bAllUsertypes)
{
	m_bAllUsertypes = bAllUsertypes;
}
/**---------------------------------------------------------------------------*/
const bool BConppUsertype::getBAll() const
{
	return m_bAllUsertypes;
}

void BConppUsertype::setSortIndex(int iSortIndex)
{
	m_iSortIndex = iSortIndex;
}

void BConppUsertype::toggleSortOrder()
{
	m_bSortOrder = !m_bSortOrder;
}

int	BConppUsertype::getSortIndex() const
{
	return m_iSortIndex;
}

bool BConppUsertype::getSortOrder() const
{
	return m_bSortOrder;
}

/**---------------------------------------------------------------------------*/
IPerst::ERet BConppUsertype::load()
{
	// set where condition
	strcpy(GIVEDATAUSERTYPE.stListCond.USERTYPENAME, getName().c_str());
	GIVEDATAUSERTYPE.stListCond.ALLUSERTYPES = getBAll();
	GIVEDATAUSERTYPE.stListCond.SORTINDEX = (short)getSortIndex();
	GIVEDATAUSERTYPE.stListCond.SORTORDER = getSortOrder();

	if (TVConBsnObject<BppUsertype>::load() == IPerst::RET_ERROR)
		return IPerst::RET_ERROR;

	return IPerst::RET_OK;
}

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
