/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                     */
/**   Date   : 14.06.2004                                                     */
/**                                                                           */
/******************************************************************************/

/*----------------------------------------------------------------------------*/
// Includes
/*----------------------------------------------------------------------------*/
#include "bsn.h"		// precompiled header

#include <tbl_ppappmodule.h>
#include <struct_appmodule.h>
#include <SppMod.h>

#include "BppAppMod.h"

//---------------------------------------------------------------------------
#define GETMODULEDATA		((SppMod::SListProj &)giveData())

SCBMEB_DEFINE_BSN(BppAppMod, SppMod::SListProj)

//---------------------------------------------------------------------------
BppAppMod::BppAppMod()
{
}

//---------------------------------------------------------------------------
BppAppMod::BppAppMod(const BppAppMod& r) : VBsnObject(r)
{
	cpyMember(r);
}

//---------------------------------------------------------------------------
BppAppMod::~BppAppMod()
{
}

/**---------------------------------------------------------------------------*/
BppAppMod * BppAppMod::clone() const
{
	return new BppAppMod(*this);
}

/**---------------------------------------------------------------------------*/
void BppAppMod::cpyMember(const BppAppMod & r)
{
    setData((r.giveData()));
}

/**---------------------------------------------------------------------------*/
IPerst::ERet BppAppMod::insertMod()
{
	if (GETMODULEDATA.stListProj.MODULEID == 0 || GETMODULEDATA.stListCond.APPLICATIONID == 0) 
	{
		DbgMsg(__FUNCTION__ ": Moduleid or Appid 0");
		clearData();
		return IPerst::RET_ERROR;
	}
	else
	{
		SppMod objSppMod;

		VAccess::ERet eRet = objSppMod.insertMod(GETMODULEDATA);

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
IPerst::ERet BppAppMod::SelForUpd()
{
	if (GETMODULEDATA.stListProj.MODULEID == 0 || GETMODULEDATA.stListCond.APPLICATIONID == 0) 
	{
		DbgMsg(__FUNCTION__ ": Appid 0 or Modid 0");
		clearData();
		return IPerst::RET_ERROR;
	}
	else
	{
		SppMod objSppMod;
		
		VAccess::ERet eRet = objSppMod.SelTestForUpd(GETMODULEDATA);

		switch (eRet)
		{
		case VAccess::RET_OK:
			break;

		case VAccess::RET_NOTFOUND:
			DbgMsg(__FUNCTION__ ": Modid %d not found in PPAPPMODULE!", 
				GETMODULEDATA.stListProj.MODULEID);
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
IPerst::ERet BppAppMod::deleteModule()
{
	if (GETMODULEDATA.stListProj.MODULEID == 0 || GETMODULEDATA.stListCond.APPLICATIONID == 0) 
	{
		DbgMsg(__FUNCTION__ ": Moduleid or Appid 0");
		clearData();
		return IPerst::RET_ERROR;
	}
	else
	{
		IPerst::ERet eRet = deleteMod();
		if (eRet == IPerst::RET_OK)
		{
			//AppModRight
			eRet = deleteAppModRight();
			if (eRet == IPerst::RET_ERROR)
			{
				return eRet;
			}

			//ppText
			eRet = deleteppText();
			if (eRet == IPerst::RET_ERROR)
			{
				return eRet;
			}

			//AppRight
			eRet = deleteAppRight();
			if (eRet == IPerst::RET_ERROR)
			{
				return eRet;
			}

		}
		
		return eRet;
	}
}

/**---------------------------------------------------------------------------*/
IPerst::ERet BppAppMod::deleteMod()
{
	if (GETMODULEDATA.stListProj.MODULEID == 0 || GETMODULEDATA.stListCond.APPLICATIONID == 0) 
	{
		DbgMsg(__FUNCTION__ ": Moduleid or Appid 0");
		clearData();
		return IPerst::RET_ERROR;
	}
	else
	{
		SppMod objSppMod;

		VAccess::ERet eRet = objSppMod.deleteMod(GETMODULEDATA);

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
IPerst::ERet BppAppMod::deleteAppModRight   ()
{
	if (GETMODULEDATA.stListProj.MODULEID == 0 || GETMODULEDATA.stListCond.APPLICATIONID == 0) 
	{
		DbgMsg(__FUNCTION__ ": Moduleid or Appid 0");
		clearData();
		return IPerst::RET_ERROR;
	}
	else
	{
		SppMod objSppMod;

		VAccess::ERet eRet = objSppMod.deleteAppModRight(GETMODULEDATA);

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
IPerst::ERet BppAppMod::deleteppText()
{
if (GETMODULEDATA.stListProj.MODULEID == 0 || GETMODULEDATA.stListCond.APPLICATIONID == 0) 
	{
		DbgMsg(__FUNCTION__ ": Moduleid or Appid 0");
		clearData();
		return IPerst::RET_ERROR;
	}
	else
	{
		SppMod objSppMod;

		VAccess::ERet eRet = objSppMod.deleteppText(GETMODULEDATA);

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
IPerst::ERet BppAppMod::deleteAppRight()
{
if (GETMODULEDATA.stListProj.MODULEID == 0 || GETMODULEDATA.stListCond.APPLICATIONID == 0) 
	{
		DbgMsg(__FUNCTION__ ": Moduleid or Appid 0");
		clearData();
		return IPerst::RET_ERROR;
	}
	else
	{
		SppMod objSppMod;

		VAccess::ERet eRet = objSppMod.deleteAppRight(GETMODULEDATA);

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
IPerst::ERet BppAppMod::updateMod()
{
	if (GETMODULEDATA.stListProj.MODULEID == 0 || GETMODULEDATA.stListCond.APPLICATIONID == 0) 
	{
		DbgMsg(__FUNCTION__ ": Moduleid or Appid 0");
		clearData();
		return IPerst::RET_ERROR;
	}
	else
	{
		SppMod objSppMod;

		VAccess::ERet eRet = objSppMod.updateMod(GETMODULEDATA);

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
void BppAppMod::setModuleid(short sModid)
{
	GETMODULEDATA.stListProj.MODULEID = sModid;
}

/**---------------------------------------------------------------------------*/
void BppAppMod::setAppid	(short sAppid)
{
	GETMODULEDATA.stListCond.APPLICATIONID = sAppid;
}

/**---------------------------------------------------------------------------*/
void BppAppMod::setName(CBString strModname)
{
	strcpy(GETMODULEDATA.stListProj.NAME, strModname);
}

/**---------------------------------------------------------------------------*/
void BppAppMod::setName_EN	        (CBString strModname)
{
	strcpy(GETMODULEDATA.stListProj.NAME_EN, strModname);
}

/**---------------------------------------------------------------------------*/
void BppAppMod::setName_FR	        (CBString strModname)
{
	strcpy(GETMODULEDATA.stListProj.NAME_FR, strModname);
}

/**---------------------------------------------------------------------------*/
void BppAppMod::setName_HR	        (CBString strModname)
{
	strcpy(GETMODULEDATA.stListProj.NAME_HR, strModname);
}

/**---------------------------------------------------------------------------*/
const short BppAppMod::getAppid() const
{
	return GETMODULEDATA.stListCond.APPLICATIONID;
}

/**---------------------------------------------------------------------------*/
const short	BppAppMod::getModuleid() const
{
	return GETMODULEDATA.stListProj.MODULEID;
}

/**---------------------------------------------------------------------------*/
const CBString	BppAppMod::getModuleidStr() const
{
	long2scStr(m_strBufExch, GETMODULEDATA.stListProj.MODULEID);
	return m_strBufExch;
}

/**---------------------------------------------------------------------------*/
const CBString  BppAppMod::getModname() const
{
	m_strBufExch = GETMODULEDATA.stListProj.NAME;
	return m_strBufExch;
}

/**---------------------------------------------------------------------------*/
const CBString  BppAppMod::getModname_EN	    () const
{
	m_strBufExch = GETMODULEDATA.stListProj.NAME_EN;
	return m_strBufExch;
}

/**---------------------------------------------------------------------------*/
const CBString  BppAppMod::getModname_FR	    () const
{
	m_strBufExch = GETMODULEDATA.stListProj.NAME_FR;
	return m_strBufExch;
}

/**---------------------------------------------------------------------------*/
const CBString  BppAppMod::getModname_HR	    () const
{
	m_strBufExch = GETMODULEDATA.stListProj.NAME_HR;
	return m_strBufExch;
}

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/	
