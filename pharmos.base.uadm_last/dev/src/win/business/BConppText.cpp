/******************************************************************************/
/**                                                                           */
/**   Desc   : Container zur Aufnahme aller Text aus der Relation ppText	   */
/**   Author : T. Hörath PRZ GmbH                                         */
/**   Date   : 06.07.2004                                                     */
/**                                                                           */
/******************************************************************************/


/*----------------------------------------------------------------------------*/
// Includes                                                                   
/*----------------------------------------------------------------------------*/
#include "bsn.h"		// precompiled header

#include <tbl_pptext.h>
#include <struct_text.h>
#include <SppText.h>

#include "BppText.h"
#include "BConppText.h"


/**---------------------------------------------------------------------------*/
#define GIVEDATATEXT		((SppText::SListProj &)giveData())

SCBMEB_DEFINE_CONBSN(BConppText, SppText::SListProj, SppText)

/**---------------------------------------------------------------------------*/
BConppText::BConppText()
{
}

/**---------------------------------------------------------------------------*/
BConppText::~BConppText()
{
}

/**---------------------------------------------------------------------------*/
void BConppText::setAppid	(short sAppid)
{
	m_sAppid = sAppid;
}
/**---------------------------------------------------------------------------*/
const short	BConppText::getAppid	() const
{
	return m_sAppid;
}

/**---------------------------------------------------------------------------*/
void BConppText::setModid	(short sModid)
{
	m_sModid = sModid;
}
/**---------------------------------------------------------------------------*/
const short	BConppText::getModid	() const
{
	return m_sModid;
}

/**---------------------------------------------------------------------------*/
void BConppText::setLangid	(CBString strLangid)
{
	m_strLangid = strLangid;
}

/**---------------------------------------------------------------------------*/
const CBString BConppText::getLangid () const
{
	return m_strLangid;
}

/**---------------------------------------------------------------------------*/
void BConppText::setBCor(bool bCorText)
{
	m_bCorText = bCorText;
}
/**---------------------------------------------------------------------------*/
const bool BConppText::getBCor() const
{
	return m_bCorText;
}

/**---------------------------------------------------------------------------*/
IPerst::ERet BConppText::load()
{
	// set where condition
	GIVEDATATEXT.stListCond.APPLICATIONID = getAppid();
	GIVEDATATEXT.stListCond.MODULEID = getModid();
	strcpy(GIVEDATATEXT.stListCond.LANGUAGEID, getLangid().c_str());
	GIVEDATATEXT.stListCond.BCOR = getBCor();


	if (TVConBsnObject<BppText>::load() == IPerst::RET_ERROR)
		return IPerst::RET_ERROR;

	return IPerst::RET_OK;
}

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
