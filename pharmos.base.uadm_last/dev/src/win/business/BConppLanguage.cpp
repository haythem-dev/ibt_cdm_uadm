/******************************************************************************/
/**                                                                           */
/**   Desc   : Container zur Aufnahme aller Languages aus der Relation ppLoginLanguage   */
/**   Author : T. Hörath PRZ GmbH                                         */
/**   Date   : 05.07.2004                                                     */
/**                                                                           */
/******************************************************************************/


/*----------------------------------------------------------------------------*/
// Includes                                                                   
/*----------------------------------------------------------------------------*/
#include "bsn.h"		// precompiled header

#include <tbl_pploginlanguage.h>
#include <struct_language.h>
#include <SppLanguage.h>

#include "BppLanguage.h"
#include "BConppLanguage.h"


/**---------------------------------------------------------------------------*/
#define GIVEDATALANG		((SppLanguage::SListProj &)giveData())

SCBMEB_DEFINE_CONBSN(BConppLanguage, SppLanguage::SListProj, SppLanguage)

/**---------------------------------------------------------------------------*/
BConppLanguage::BConppLanguage()
{
}

/**---------------------------------------------------------------------------*/
BConppLanguage::~BConppLanguage()
{
}

/**---------------------------------------------------------------------------*/
void BConppLanguage::setAppid	(short sAppid)
{
	m_sAppid = sAppid;
}
/**---------------------------------------------------------------------------*/
const short	BConppLanguage::getAppid	() const
{
	return m_sAppid;
}

/**---------------------------------------------------------------------------*/
void BConppLanguage::setBList	(bool bLangList)
{
	m_bLangList = bLangList;
}

/**---------------------------------------------------------------------------*/
const bool BConppLanguage::getBList	() const
{
	return m_bLangList;
}

/**---------------------------------------------------------------------------*/
void BConppLanguage::setBAll(bool bAllLang)
{
	m_bAllLang = bAllLang;
}
/**---------------------------------------------------------------------------*/
const bool BConppLanguage::getBAll() const
{
	return m_bAllLang;
}

/**---------------------------------------------------------------------------*/
IPerst::ERet BConppLanguage::load()
{
	// set where condition
	GIVEDATALANG.stListCond.APPLICATIONID = getAppid();
	GIVEDATALANG.stListCond.ALLLANG = getBAll();
	GIVEDATALANG.stListCond.LANGLIST = getBList();


	if (TVConBsnObject<BppLanguage>::load() == IPerst::RET_ERROR)
		return IPerst::RET_ERROR;

	return IPerst::RET_OK;
}

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
