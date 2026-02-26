/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                     */
/**   Date   : 09.06.2004                                                     */
/**                                                                           */
/******************************************************************************/

/*----------------------------------------------------------------------------*/
// Includes
/*----------------------------------------------------------------------------*/
#include "bsn.h"		// precompiled header

#include <tbl_ppgroup.h>
#include <struct_group.h>
#include <SppGroup.h>

#include "BppGroup.h"

//---------------------------------------------------------------------------
#define GETGROUPDATA		((SppGroup::SListProj &)giveData())

SCBMEB_DEFINE_BSN(BppGroup, SppGroup::SListProj)

//---------------------------------------------------------------------------
BppGroup::BppGroup()
{
}

//---------------------------------------------------------------------------
BppGroup::BppGroup(const BppGroup& r) : VBsnObject(r)
{
	cpyMember(r);
}

//---------------------------------------------------------------------------
BppGroup::~BppGroup()
{
}

/**---------------------------------------------------------------------------*/
BppGroup * BppGroup::clone() const
{
	return new BppGroup(*this);
}

/**---------------------------------------------------------------------------*/
void BppGroup::cpyMember(const BppGroup & r)
{
    setData((r.giveData()));
}

/**---------------------------------------------------------------------------*/
IPerst::ERet BppGroup::insertCor()
{
	if (GETGROUPDATA.stListProj.GROUPID == 0 || GETGROUPDATA.stListCond.USERID == 0 || GETGROUPDATA.stListCond.APPLICATIONID == 0) 
	{
		DbgMsg(__FUNCTION__ ": Userid or Appid or Groupid 0");
		clearData();
		return IPerst::RET_ERROR;
	}
	else
	{
		SppGroup objSppGroup;

		VAccess::ERet eRet = objSppGroup.insertCorUser(GETGROUPDATA);

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

		case VAccess::RET_WARNING:
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
IPerst::ERet BppGroup::deleteCor()
{
	if (GETGROUPDATA.stListProj.GROUPID == 0 || GETGROUPDATA.stListCond.USERID == 0 || GETGROUPDATA.stListCond.APPLICATIONID == 0) 
	{
		DbgMsg(__FUNCTION__ ": Userid or Appid or Groupid 0");
		clearData();
		return IPerst::RET_ERROR;
	}
	else
	{
		SppGroup objSppGroup;

		VAccess::ERet eRet = objSppGroup.deleteCorUser(GETGROUPDATA);

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
IPerst::ERet BppGroup::deleteAllCorUser()
{
	if (GETGROUPDATA.stListProj.GROUPID == 0 ||  GETGROUPDATA.stListCond.APPLICATIONID == 0) 
	{
		DbgMsg(__FUNCTION__ ": Appid or Groupid 0");
		clearData();
		return IPerst::RET_ERROR;
	}
	else
	{
		SppGroup objSppGroup;

		VAccess::ERet eRet = objSppGroup.deleteAllCorUser(GETGROUPDATA);

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
void BppGroup::setGroupid(long lGroupid)
{
	GETGROUPDATA.stListProj.GROUPID = lGroupid;
}

/**---------------------------------------------------------------------------*/
void BppGroup::setAppid	(short sAppid)
{
	GETGROUPDATA.stListCond.APPLICATIONID = sAppid;
}

/**---------------------------------------------------------------------------*/
void BppGroup::setUserid(short sUserid)
{
	GETGROUPDATA.stListCond.USERID = sUserid;
}

/**---------------------------------------------------------------------------*/
void BppGroup::setName(CBString strGroupname)
{
	strcpy(GETGROUPDATA.stListProj.NAME, strGroupname);
}

/**---------------------------------------------------------------------------*/
const long	BppGroup::getGroupid() const
{
	return GETGROUPDATA.stListProj.GROUPID;
}

/**---------------------------------------------------------------------------*/
const CBString	BppGroup::getGroupidStr() const
{
	long2scStr(m_strBufExch, GETGROUPDATA.stListProj.GROUPID);
	return m_strBufExch;
}

/**---------------------------------------------------------------------------*/
const CBString  BppGroup::getName() const
{
	m_strBufExch = GETGROUPDATA.stListProj.NAME;
	return m_strBufExch;
}

/**---------------------------------------------------------------------------*/
IPerst::ERet BppGroup::insert()
{
	char szTest[] = "";
	if (strcmp (szTest,GETGROUPDATA.stListProj.NAME) == 0)
	{
		DbgMsg(__FUNCTION__ ": Grpname not set");
		clearData();
	}
	else
	{
		SppGroup objSppGroup;

		VAccess::ERet eRet = objSppGroup.insertSingle(GETGROUPDATA);

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
IPerst::ERet BppGroup::update()
{
	if (GETGROUPDATA.stListProj.GROUPID == 0 ) 
	{
		DbgMsg(__FUNCTION__ ": Groupid 0");
		clearData();
		return IPerst::RET_ERROR;
	}
	else
	{
		SppGroup objSppGroup;

		VAccess::ERet eRet = objSppGroup.updateSingle(GETGROUPDATA);

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
IPerst::ERet BppGroup::delGrp()
{
	if (GETGROUPDATA.stListProj.GROUPID == 0) 
	{
		DbgMsg(__FUNCTION__ ": Grpid 0");
		clearData();
		return IPerst::RET_ERROR;
	}
	else
	{
		IPerst::ERet eRet = deleteGroup();
		if (eRet == IPerst::RET_OK)
		{
			//ppCorAppUserGroup
			eRet = deleteAppCorUserGroup();
			if (eRet == IPerst::RET_ERROR)
			{
				return eRet;
			}			
			//ppAppRight
			eRet = deleteAppRight();
			if (eRet == IPerst::RET_ERROR)
			{
				return eRet;
			}

			return IPerst::RET_OK;
		}
		
		return eRet;
	}
}

/**---------------------------------------------------------------------------*/
IPerst::ERet BppGroup::selectID()
{
	char szTest[] = "";
	if (strcmp (szTest,GETGROUPDATA.stListProj.NAME) == 0)
	{
		DbgMsg(__FUNCTION__ ": Grpname not set");
		clearData();
	}
	else
	{
		SppGroup objSppGroup;
		long lGrpid;
		CBString strGN = getName();

		VAccess::ERet eRet = objSppGroup.selectID(strGN, lGrpid);

		switch (eRet)
		{
		case VAccess::RET_OK:
			setGroupid(lGrpid);
			break;

		case VAccess::RET_NOTFOUND:
			DbgMsg(__FUNCTION__ ": Grpname %s not found in PPGROUP!", 
				GETGROUPDATA.stListProj.NAME);
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
IPerst::ERet BppGroup::SelForUpd()
{
	if (GETGROUPDATA.stListProj.GROUPID == 0) 
	{
		DbgMsg(__FUNCTION__ ": Groupid 0");
		clearData();
		return IPerst::RET_ERROR;
	}
	else
	{
		SppGroup objSppGroup;
		
		VAccess::ERet eRet = objSppGroup.SelTestForUpd(GETGROUPDATA);

		switch (eRet)
		{
		case VAccess::RET_OK:
			break;

		case VAccess::RET_NOTFOUND:
			DbgMsg(__FUNCTION__ ": Groupid %d not found in PPGROUP!", 
				GETGROUPDATA.stListProj.GROUPID);
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
IPerst::ERet BppGroup::deleteGroup()
{
	if (GETGROUPDATA.stListProj.GROUPID == 0 ) 
	{
		DbgMsg(__FUNCTION__ ": Groupid 0");
		clearData();
		return IPerst::RET_ERROR;
	}
	else
	{
		SppGroup objSppGroup;

		VAccess::ERet eRet = objSppGroup.deleteSingle(GETGROUPDATA);

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
IPerst::ERet BppGroup::deleteAppRight()
{
	if (GETGROUPDATA.stListProj.GROUPID == 0 ) 
	{
		DbgMsg(__FUNCTION__ ": Groupid 0");
		clearData();
		return IPerst::RET_ERROR;
	}
	else
	{
		SppGroup objSppGroup;

		VAccess::ERet eRet = objSppGroup.deleteAppRight(GETGROUPDATA);

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
IPerst::ERet BppGroup::deleteAppCorUserGroup()
{
	if (GETGROUPDATA.stListProj.GROUPID == 0 ) 
	{
		DbgMsg(__FUNCTION__ ": Groupid 0");
		clearData();
		return IPerst::RET_ERROR;
	}
	else
	{
		SppGroup objSppGroup;

		VAccess::ERet eRet = objSppGroup.deleteAppCorUserGroup(GETGROUPDATA);

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

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/	
