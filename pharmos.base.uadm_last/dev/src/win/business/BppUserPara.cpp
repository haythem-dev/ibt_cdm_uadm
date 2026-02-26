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
#include <SppUserPara.h>

#include "BppUserPara.h"

//---------------------------------------------------------------------------
#define GETUSERPARADATA		((SppUserPara::SListProj &)giveData())

SCBMEB_DEFINE_BSN(BppUserPara, SppUserPara::SListProj)

//---------------------------------------------------------------------------
BppUserPara::BppUserPara()
{
}

//---------------------------------------------------------------------------
BppUserPara::BppUserPara(const BppUserPara& r) : VBsnObject(r)
{
	cpyMember(r);
}

//---------------------------------------------------------------------------
BppUserPara::~BppUserPara()
{
}

/**---------------------------------------------------------------------------*/
BppUserPara * BppUserPara::clone() const
{
	return new BppUserPara(*this);
}

/**---------------------------------------------------------------------------*/
void BppUserPara::cpyMember(const BppUserPara & r)
{
    setData((r.giveData()));
}

/**---------------------------------------------------------------------------*/
void BppUserPara::setUserParaid(short sUserParaid)
{
	GETUSERPARADATA.stListProj.USERPARAID = sUserParaid;
}

/**---------------------------------------------------------------------------*/
void BppUserPara::setName(CBString strUserParaname)
{
	strcpy(GETUSERPARADATA.stListProj.NAME, strUserParaname);
}

/**---------------------------------------------------------------------------*/
void BppUserPara::setDescription(CBString strUserParadesc)
{
	strcpy(GETUSERPARADATA.stListProj.DESCRIPTION, strUserParadesc);
}

/**---------------------------------------------------------------------------*/
void BppUserPara::setUserCount(long lUserCount)
{
	GETUSERPARADATA.stListProj.USERCOUNT = lUserCount;
}

/**---------------------------------------------------------------------------*/
const short	BppUserPara::getUserParaid() const
{
	return GETUSERPARADATA.stListProj.USERPARAID;
}

/**---------------------------------------------------------------------------*/
const CBString BppUserPara::getUserParaidStr() const
{
	short2scStr(m_strBufExch, GETUSERPARADATA.stListProj.USERPARAID);
	return m_strBufExch;
}

/**---------------------------------------------------------------------------*/
const CBString BppUserPara::getName() const
{
	m_strBufExch = GETUSERPARADATA.stListProj.NAME;
	return m_strBufExch;
}

/**---------------------------------------------------------------------------*/
const CBString BppUserPara::getDescription() const
{
	m_strBufExch = GETUSERPARADATA.stListProj.DESCRIPTION;
	return m_strBufExch;
}

/**---------------------------------------------------------------------------*/
const long	BppUserPara::getUserCount() const
{
	return GETUSERPARADATA.stListProj.USERCOUNT;
}

/**---------------------------------------------------------------------------*/
const CBString BppUserPara::getUserCountStr() const
{
	long2scStr(m_strBufExch, GETUSERPARADATA.stListProj.USERCOUNT);
	return m_strBufExch;
}

/**---------------------------------------------------------------------------*/
IPerst::ERet BppUserPara::insertItem()
{
	char szTest[] = "";
	if (strcmp (szTest,GETUSERPARADATA.stListProj.NAME) == 0)
	{
		DbgMsg(__FUNCTION__ ": Usertypename not set");
		clearData();
	}
	else
	{
		SppUserPara objSppUserPara;

		VAccess::ERet eRet = objSppUserPara.insertSingle(GETUSERPARADATA);

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
IPerst::ERet BppUserPara::updateItem()
{
	if (GETUSERPARADATA.stListProj.USERPARAID == 0 ) 
	{
		DbgMsg(__FUNCTION__ ": Usertypeid 0");
		clearData();
		return IPerst::RET_ERROR;
	}
	else
	{
		SppUserPara objSppUserPara;

		VAccess::ERet eRet = objSppUserPara.updateSingle(GETUSERPARADATA);

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
IPerst::ERet BppUserPara::deleteItem()
{
	if (GETUSERPARADATA.stListProj.USERPARAID == 0)
	{
		DbgMsg(__FUNCTION__ ": Usertypeid 0");
		clearData();
		return IPerst::RET_ERROR;
	}
	else
	{
		SppUserPara objSppUserPara;

		VAccess::ERet eRet = objSppUserPara.deleteSingle(GETUSERPARADATA);

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
IPerst::ERet BppUserPara::selectID()
{
	char szTest[] = "";
	if (strcmp (szTest,GETUSERPARADATA.stListProj.NAME) == 0)
	{
		DbgMsg(__FUNCTION__ ": Usertype name not set");
		clearData();
	}
	else
	{
		SppUserPara objSppUserPara;
		short sUserparaid;
		CBString strUN = getName();

		VAccess::ERet eRet = objSppUserPara.selectID(strUN, sUserparaid);

		switch (eRet)
		{
		case VAccess::RET_OK:
			setUserParaid(sUserparaid);
			break;

		case VAccess::RET_NOTFOUND:
			DbgMsg(__FUNCTION__ ": UserPara name %s not found in PPUSERPARA!", 
				GETUSERPARADATA.stListProj.NAME);
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
IPerst::ERet BppUserPara::SelForUpd()
{
	if (GETUSERPARADATA.stListProj.USERPARAID == 0)
	{
		DbgMsg(__FUNCTION__ ": UserPara id 0");
		clearData();
		return IPerst::RET_ERROR;
	}
	else
	{
		SppUserPara objSppUserPara;

		VAccess::ERet eRet = objSppUserPara.SelTestForUpd(GETUSERPARADATA);

		switch (eRet)
		{
		case VAccess::RET_OK:
			break;

		case VAccess::RET_NOTFOUND:
			DbgMsg(__FUNCTION__ ": UserPara id %d not found in PPUSERPARA!", 
				GETUSERPARADATA.stListProj.USERPARAID);
			clearData();

			return IPerst::RET_WARNING;
	
		default:
			assert(0);
			return IPerst::RET_ERROR;
		}
	}

	return IPerst::RET_OK;
}

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/	
