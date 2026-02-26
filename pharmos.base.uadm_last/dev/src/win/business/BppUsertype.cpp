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

//---------------------------------------------------------------------------
#define GETUSERTYPEDATA		((SppUsertype::SListProj &)giveData())

SCBMEB_DEFINE_BSN(BppUsertype, SppUsertype::SListProj)

//---------------------------------------------------------------------------
BppUsertype::BppUsertype()
{
}

//---------------------------------------------------------------------------
BppUsertype::BppUsertype(const BppUsertype& r) : VBsnObject(r)
{
	cpyMember(r);
}

//---------------------------------------------------------------------------
BppUsertype::~BppUsertype()
{
}

/**---------------------------------------------------------------------------*/
BppUsertype * BppUsertype::clone() const
{
	return new BppUsertype(*this);
}

/**---------------------------------------------------------------------------*/
void BppUsertype::cpyMember(const BppUsertype & r)
{
    setData((r.giveData()));
}

/**---------------------------------------------------------------------------*/
void BppUsertype::setUsertypeid(short sUsertypeid)
{
	GETUSERTYPEDATA.stListProj.USERTYPEID = sUsertypeid;
}

/**---------------------------------------------------------------------------*/
void BppUsertype::setName(CBString strUsertypename)
{
	strcpy(GETUSERTYPEDATA.stListProj.NAME, strUsertypename);
}

/**---------------------------------------------------------------------------*/
void BppUsertype::setDescription(CBString strUsertypedesc)
{
	strcpy(GETUSERTYPEDATA.stListProj.DESCRIPTION, strUsertypedesc);
}

/**---------------------------------------------------------------------------*/
void BppUsertype::setUserCount(long lUserCount)
{
	GETUSERTYPEDATA.stListProj.USERCOUNT = lUserCount;
}

/**---------------------------------------------------------------------------*/
const short	BppUsertype::getUsertypeid() const
{
	return GETUSERTYPEDATA.stListProj.USERTYPEID;
}

/**---------------------------------------------------------------------------*/
const CBString BppUsertype::getUsertypeidStr() const
{
	short2scStr(m_strBufExch, GETUSERTYPEDATA.stListProj.USERTYPEID);
	return m_strBufExch;
}

/**---------------------------------------------------------------------------*/
const CBString BppUsertype::getName() const
{
	m_strBufExch = GETUSERTYPEDATA.stListProj.NAME;
	return m_strBufExch;
}

/**---------------------------------------------------------------------------*/
const CBString BppUsertype::getDescription() const
{
	m_strBufExch = GETUSERTYPEDATA.stListProj.DESCRIPTION;
	return m_strBufExch;
}

/**---------------------------------------------------------------------------*/
const long	BppUsertype::getUserCount() const
{
	return GETUSERTYPEDATA.stListProj.USERCOUNT;
}

/**---------------------------------------------------------------------------*/
const CBString BppUsertype::getUserCountStr() const
{
	long2scStr(m_strBufExch, GETUSERTYPEDATA.stListProj.USERCOUNT);
	return m_strBufExch;
}

/**---------------------------------------------------------------------------*/
IPerst::ERet BppUsertype::insertItem()
{
	char szTest[] = "";
	if (strcmp (szTest,GETUSERTYPEDATA.stListProj.NAME) == 0)
	{
		DbgMsg(__FUNCTION__ ": Usertypename not set");
		clearData();
	}
	else
	{
		SppUsertype objSppUsertype;

		VAccess::ERet eRet = objSppUsertype.insertSingle(GETUSERTYPEDATA);

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
IPerst::ERet BppUsertype::updateItem()
{
	if (GETUSERTYPEDATA.stListProj.USERTYPEID == 0 ) 
	{
		DbgMsg(__FUNCTION__ ": Usertypeid 0");
		clearData();
		return IPerst::RET_ERROR;
	}
	else
	{
		SppUsertype objSppUsertype;

		VAccess::ERet eRet = objSppUsertype.updateSingle(GETUSERTYPEDATA);

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
IPerst::ERet BppUsertype::deleteItem()
{
	if (GETUSERTYPEDATA.stListProj.USERTYPEID == 0)
	{
		DbgMsg(__FUNCTION__ ": Usertypeid 0");
		clearData();
		return IPerst::RET_ERROR;
	}
	else
	{
		SppUsertype objSppUsertype;

		VAccess::ERet eRet = objSppUsertype.deleteSingle(GETUSERTYPEDATA);

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
IPerst::ERet BppUsertype::selectID()
{
	char szTest[] = "";
	if (strcmp (szTest,GETUSERTYPEDATA.stListProj.NAME) == 0)
	{
		DbgMsg(__FUNCTION__ ": Usertype name not set");
		clearData();
	}
	else
	{
		SppUsertype objSppUsertype;
		short sUsertypeid;
		CBString strUN = getName();

		VAccess::ERet eRet = objSppUsertype.selectID(strUN, sUsertypeid);

		switch (eRet)
		{
		case VAccess::RET_OK:
			setUsertypeid(sUsertypeid);
			break;

		case VAccess::RET_NOTFOUND:
			DbgMsg(__FUNCTION__ ": Usertype name %s not found in PPUSERTYPE!", 
				GETUSERTYPEDATA.stListProj.NAME);
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
IPerst::ERet BppUsertype::SelForUpd()
{
	if (GETUSERTYPEDATA.stListProj.USERTYPEID == 0)
	{
		DbgMsg(__FUNCTION__ ": Usertypeid 0");
		clearData();
		return IPerst::RET_ERROR;
	}
	else
	{
		SppUsertype objSppUsertype;

		VAccess::ERet eRet = objSppUsertype.SelTestForUpd(GETUSERTYPEDATA);

		switch (eRet)
		{
		case VAccess::RET_OK:
			break;

		case VAccess::RET_NOTFOUND:
			DbgMsg(__FUNCTION__ ": Usertypeid %d not found in PPUSERTYPE!", 
				GETUSERTYPEDATA.stListProj.USERTYPEID);
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
