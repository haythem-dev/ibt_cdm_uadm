/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                     */
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

//---------------------------------------------------------------------------
#define GETPREVDATA		((SppPrev::SListProj &)giveData())

SCBMEB_DEFINE_BSN(BppPrev, SppPrev::SListProj)

//---------------------------------------------------------------------------
BppPrev::BppPrev()
{
}

//---------------------------------------------------------------------------
BppPrev::BppPrev(const BppPrev& r) : VBsnObject(r)
{
	cpyMember(r);
}

//---------------------------------------------------------------------------
BppPrev::~BppPrev()
{
}

/**---------------------------------------------------------------------------*/
BppPrev * BppPrev::clone() const
{
	return new BppPrev(*this);
}

/**---------------------------------------------------------------------------*/
void BppPrev::cpyMember(const BppPrev & r)
{
    setData((r.giveData()));
}

/**---------------------------------------------------------------------------*/
IPerst::ERet BppPrev::deletePrev()
{
	if (GETPREVDATA.stListCond.APPLICATIONID == 0) 
	{
		DbgMsg(__FUNCTION__ ": Appid 0");
		clearData();
		return IPerst::RET_ERROR;
	}
	else
	{
		SppPrev objSppPrev;

		VAccess::ERet eRet = objSppPrev.deleteSingle(GETPREVDATA);

		switch (eRet)
		{
		IPerst::ERet Ret;
		case VAccess::RET_OK:
			Ret = BSqlStat::ex().UpdateBackupMachine();
			if (Ret == IPerst::RET_ERROR)
			{
				DbgMsg(__FUNCTION__ ": Error on updating backup-machine!");
			}
			break;

		case VAccess::RET_NOTFOUND:
			clearData();

			return IPerst::RET_WARNING;
	
		default:
			assert(0);
			return IPerst::RET_ERROR;
		}
	}

	return IPerst::RET_OK;
}


/**---------------------------------------------------------------------------*/
void BppPrev::setAppid	(short sAppid)
{
	GETPREVDATA.stListCond.APPLICATIONID = sAppid;
}


/**---------------------------------------------------------------------------*/
void BppPrev::setGlobUser(CBString strGlobUser)
{
	strcpy(GETPREVDATA.stListProj.GLOBALUSER, strGlobUser);
}

/**---------------------------------------------------------------------------*/
const short BppPrev::getAppid() const
{
	return GETPREVDATA.stListCond.APPLICATIONID;
}


/**---------------------------------------------------------------------------*/
const CBString BppPrev::getGlobUser() const
{
	m_strBufExch = GETPREVDATA.stListProj.GLOBALUSER;
	return m_strBufExch;
}

/**---------------------------------------------------------------------------*/
const CBString BppPrev::getUsername 	    () const
{
	m_strBufExch = GETPREVDATA.stListProj.USERNAME;
	return m_strBufExch;
}

/**---------------------------------------------------------------------------*/
const CBString BppPrev::getLanguageID 	    () const
{
	m_strBufExch = GETPREVDATA.stListProj.LANGUAGEID;
	return m_strBufExch;
}

/**---------------------------------------------------------------------------*/
const short BppPrev::getBrRegID			() const
{
	return GETPREVDATA.stListProj.BRANCHREGIONID;
}

/**---------------------------------------------------------------------------*/
const CBString BppPrev::getBrRegIDstr 	    () const
{
	long2scStr(m_strBufExch, GETPREVDATA.stListProj.BRANCHREGIONID);
	return m_strBufExch;
}


/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/	
