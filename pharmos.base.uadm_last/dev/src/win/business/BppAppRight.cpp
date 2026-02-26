/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                     */
/**   Date   : 15.06.2004                                                     */
/**                                                                           */
/******************************************************************************/

/*----------------------------------------------------------------------------*/
// Includes
/*----------------------------------------------------------------------------*/
#include "bsn.h"		// precompiled header

#include <tbl_ppappright.h>
#include <tbl_ppappmodright.h>
#include <struct_appright.h>
#include <SppAppRight.h>

#include "BppAppRight.h"

//---------------------------------------------------------------------------
#define GETRIGHTDATA		((SppAppRight::SListProj &)giveData())

SCBMEB_DEFINE_BSN(BppAppRight, SppAppRight::SListProj)

//---------------------------------------------------------------------------
BppAppRight::BppAppRight()
{
}

//---------------------------------------------------------------------------
BppAppRight::BppAppRight(const BppAppRight& r) : VBsnObject(r)
{
	cpyMember(r);
}

//---------------------------------------------------------------------------
BppAppRight::~BppAppRight()
{
}

/**---------------------------------------------------------------------------*/
BppAppRight * BppAppRight::clone() const
{
	return new BppAppRight(*this);
}

/**---------------------------------------------------------------------------*/
void BppAppRight::cpyMember(const BppAppRight & r)
{
   setData((r.giveData()));
}

/**---------------------------------------------------------------------------*/
IPerst::ERet BppAppRight::insertAppRight()
{
	if (GETRIGHTDATA.stListCond.APPLICATIONID == 0 || GETRIGHTDATA.stListCond.MODULEID == 0 || GETRIGHTDATA.stListCond.BRANCHREGID == 0) 
	{
		DbgMsg(__FUNCTION__ ": Moduleid or Appid or BranchRegid 0");
		clearData();
		return IPerst::RET_ERROR;
	}
	else
	{
		SppAppRight objSppAppRight;

		VAccess::ERet eRet = objSppAppRight.insertAppRight(GETRIGHTDATA);

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
			return IPerst::RET_WARNING;

		default:
			assert(0);
			return IPerst::RET_ERROR;
		}
	}

	return IPerst::RET_OK;
}

/**---------------------------------------------------------------------------*/
IPerst::ERet BppAppRight::SelForUpd()
{
	if (GETRIGHTDATA.stListCond.APPLICATIONID == 0 || GETRIGHTDATA.stListCond.MODULEID == 0) 
	{
		DbgMsg(__FUNCTION__ ": Appid 0 or Modid 0");
		clearData();
		return IPerst::RET_ERROR;
	}
	else
	{
		SppAppRight objSppAppRight;
		
		VAccess::ERet eRet = objSppAppRight.SelTestForUpd(GETRIGHTDATA);

		switch (eRet)
		{
		case VAccess::RET_OK:
			break;

		case VAccess::RET_NOTFOUND:
			DbgMsg(__FUNCTION__ ": Rightname %s not found in PPAPPMODRIGHT!", 
				GETRIGHTDATA.stListProj.RIGHTNAME);
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
IPerst::ERet BppAppRight::delRight()
{
	if (GETRIGHTDATA.stListCond.APPLICATIONID == 0 || GETRIGHTDATA.stListCond.MODULEID == 0) 
	{
		DbgMsg(__FUNCTION__ ": Moduleid or Appid 0");
		clearData();
		return IPerst::RET_ERROR;
	}
	else
	{
		IPerst::ERet eRet = deleteAppModRight();
		if (eRet == IPerst::RET_OK)
		{
			//AppRight
			eRet = delFromAppRight();
			if (eRet == IPerst::RET_ERROR)
			{
				return eRet;
			}

		}
		return eRet;
	}
}

/**---------------------------------------------------------------------------*/
IPerst::ERet BppAppRight::deleteAppModRight()
{
	if (GETRIGHTDATA.stListCond.APPLICATIONID == 0 || GETRIGHTDATA.stListCond.MODULEID == 0) 
	{
		DbgMsg(__FUNCTION__ ": Moduleid or Appid 0");
		clearData();
		return IPerst::RET_ERROR;
	}
	else
	{
		SppAppRight objSppAppRight;

		VAccess::ERet eRet = objSppAppRight.deleteAppModRight(GETRIGHTDATA);

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
IPerst::ERet BppAppRight::delFromAppRight()
{
	if (GETRIGHTDATA.stListCond.APPLICATIONID == 0 || GETRIGHTDATA.stListCond.MODULEID == 0) 
	{
		DbgMsg(__FUNCTION__ ": Moduleid or Appid 0");
		clearData();
		return IPerst::RET_ERROR;
	}
	else
	{
		SppAppRight objSppAppRight;

		VAccess::ERet eRet = objSppAppRight.deleteFromAppRight(GETRIGHTDATA);

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
IPerst::ERet BppAppRight::insertAppModRight()
{
	if (GETRIGHTDATA.stListCond.APPLICATIONID == 0 || GETRIGHTDATA.stListCond.MODULEID == 0) 
	{
		DbgMsg(__FUNCTION__ ": Moduleid or Appid 0");
		clearData();
		return IPerst::RET_ERROR;
	}
	else
	{
		SppAppRight objSppAppRight;

		VAccess::ERet eRet = objSppAppRight.insertAppModRight(GETRIGHTDATA);

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
IPerst::ERet BppAppRight::deleteAppRight()
{
	if (GETRIGHTDATA.stListCond.APPLICATIONID == 0 || GETRIGHTDATA.stListCond.MODULEID == 0 || GETRIGHTDATA.stListCond.BRANCHREGID == 0) 
	{
		DbgMsg(__FUNCTION__ ": Moduleid or Appid or BranchRegid 0");
		clearData();
		return IPerst::RET_ERROR;
	}
	else
	{
		SppAppRight objSppAppRight;

		VAccess::ERet eRet = objSppAppRight.deleteAppRight(GETRIGHTDATA);

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
IPerst::ERet BppAppRight::updateApplRight	    ()
{
	if (GETRIGHTDATA.stListCond.APPLICATIONID == 0 || GETRIGHTDATA.stListCond.MODULEID == 0) 
	{
		DbgMsg(__FUNCTION__ ": Moduleid or Appid 0");
		clearData();
		return IPerst::RET_ERROR;
	}
	else
	{
		SppAppRight objSppAppRight;

		VAccess::ERet eRet = objSppAppRight.updateApplRight(GETRIGHTDATA, m_strPrevRightname);

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
IPerst::ERet BppAppRight::updateApplModRight  ()
{
	if (GETRIGHTDATA.stListCond.APPLICATIONID == 0 || GETRIGHTDATA.stListCond.MODULEID == 0) 
	{
		DbgMsg(__FUNCTION__ ": Moduleid or Appid 0");
		clearData();
		return IPerst::RET_ERROR;
	}
	else
	{
		SppAppRight objSppAppRight;

		VAccess::ERet eRet = objSppAppRight.updateApplModRight(GETRIGHTDATA, m_strPrevRightname);

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
IPerst::ERet BppAppRight::updateAppModRight()
{
	if (GETRIGHTDATA.stListCond.APPLICATIONID == 0 || GETRIGHTDATA.stListCond.MODULEID == 0) 
	{
		DbgMsg(__FUNCTION__ ": Moduleid or Appid 0");
		clearData();
		return IPerst::RET_ERROR;
	}
	else
	{
		SppAppRight objSppAppRight;

		VAccess::ERet eRet = objSppAppRight.updateAppModRight(GETRIGHTDATA);

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
void BppAppRight::setAppid(short sAppid)
{
	GETRIGHTDATA.stListCond.APPLICATIONID = sAppid;
}
/**---------------------------------------------------------------------------*/
void BppAppRight::setModid(short sModid)
{
	GETRIGHTDATA.stListCond.MODULEID = sModid;
}
/**---------------------------------------------------------------------------*/
void BppAppRight::setUserid(short sUserid)
{
	GETRIGHTDATA.stListCond.USERID = sUserid;
}
/**---------------------------------------------------------------------------*/
void BppAppRight::setBranchRegid(short sBranchRegid)
{
	GETRIGHTDATA.stListCond.BRANCHREGID = sBranchRegid;
}
/**---------------------------------------------------------------------------*/
void BppAppRight::setGroupid(short sGroupid)
{
	GETRIGHTDATA.stListCond.GROUPID = sGroupid;
}
/**---------------------------------------------------------------------------*/
void BppAppRight::setRightCategory(short sCategory)
{
	GETRIGHTDATA.stListProj.CATEGORY = sCategory;
}
/**---------------------------------------------------------------------------*/
void BppAppRight::setRightname(CBString strRightname)
{
	strcpy(GETRIGHTDATA.stListProj.RIGHTNAME, strRightname);
}
/**---------------------------------------------------------------------------*/
void BppAppRight::setRightDesc_EN	(CBString strRightDesc)
{
	strcpy(GETRIGHTDATA.stListProj.RIGHTDESC_EN, strRightDesc);
}

/**---------------------------------------------------------------------------*/
void BppAppRight::setRightDesc_FR	(CBString strRightDesc)
{
	strcpy(GETRIGHTDATA.stListProj.RIGHTDESC_FR, strRightDesc);
}

/**---------------------------------------------------------------------------*/
void BppAppRight::setRightDesc_HR	(CBString strRightDesc)
{
	strcpy(GETRIGHTDATA.stListProj.RIGHTDESC_HR, strRightDesc);
}

/**---------------------------------------------------------------------------*/
void  BppAppRight::setPrevRightName	(CBString strPrevRightname)
{
	m_strPrevRightname = strPrevRightname;
}

/**---------------------------------------------------------------------------*/
void BppAppRight::setRightDesc(CBString strRightDesc)
{
	strcpy(GETRIGHTDATA.stListProj.RIGHTDESC, strRightDesc);
}
/**---------------------------------------------------------------------------*/
const CBString  BppAppRight::getRightDesc_EN() const
{
	m_strBufExch = GETRIGHTDATA.stListProj.RIGHTDESC_EN;
	return m_strBufExch;
}

/**---------------------------------------------------------------------------*/
const CBString  BppAppRight::getRightDesc_FR() const
{
	m_strBufExch = GETRIGHTDATA.stListProj.RIGHTDESC_FR;
	return m_strBufExch;
}

/**---------------------------------------------------------------------------*/
const CBString  BppAppRight::getRightDesc_HR() const
{
	m_strBufExch = GETRIGHTDATA.stListProj.RIGHTDESC_HR;
	return m_strBufExch;
}

/**---------------------------------------------------------------------------*/
const CBString  BppAppRight::getRightDesc() const
{
	m_strBufExch = GETRIGHTDATA.stListProj.RIGHTDESC;
	return m_strBufExch;
}

/**---------------------------------------------------------------------------*/
const CBString  BppAppRight::getRightname() const
{
	m_strBufExch = GETRIGHTDATA.stListProj.RIGHTNAME;
	return m_strBufExch;
}

/**---------------------------------------------------------------------------*/
const short	BppAppRight::getAppid() const
{
	return GETRIGHTDATA.stListCond.APPLICATIONID;
}

/**---------------------------------------------------------------------------*/
const short BppAppRight::getModid() const
{
	return GETRIGHTDATA.stListCond.MODULEID;
}

/**---------------------------------------------------------------------------*/
const short	BppAppRight::getRightCategory() const
{
	return GETRIGHTDATA.stListProj.CATEGORY;
}


/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/	
