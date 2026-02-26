/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                     */
/**   Date   : 01.06.2004                                                     */
/**                                                                           */
/******************************************************************************/

/*----------------------------------------------------------------------------*/
// Includes
/*----------------------------------------------------------------------------*/
#include "bsn.h"		// precompiled header

#include <tbl_ppapplication.h>
#include <struct_application.h>
#include <SppApp.h>

#include "BppApp.h"

//---------------------------------------------------------------------------
#define GETCONAPPDATA		((SppApp::SListAppProj &)giveData())

SCBMEB_DEFINE_BSN(BppApp, SppApp::SListAppProj) 

//---------------------------------------------------------------------------
BppApp::BppApp()
{
}

//---------------------------------------------------------------------------
BppApp::BppApp(const BppApp& r) : VBsnObject(r)
{
	cpyMember(r);
}

//---------------------------------------------------------------------------
BppApp::~BppApp()
{
}

/**---------------------------------------------------------------------------*/
BppApp * BppApp::clone() const
{
	return new BppApp(*this);
}

/**---------------------------------------------------------------------------*/
void BppApp::cpyMember(const BppApp & r)
{
    setData((r.giveData()));
}

/**---------------------------------------------------------------------------*/
const long	BppApp::getAppid () const
{
	return GETCONAPPDATA.stListAppProj.APPLICATIONID;
}

/**---------------------------------------------------------------------------*/
const CBString	BppApp::getAppidStr() const
{
	long2scStr(m_strBufExch, GETCONAPPDATA.stListAppProj.APPLICATIONID);
	return m_strBufExch;
}
/**---------------------------------------------------------------------------*/
const CBString BppApp::getAppname() const
{
	m_strBufExch = GETCONAPPDATA.stListAppProj.NAME;
	return m_strBufExch;
}

/**---------------------------------------------------------------------------*/
const CBString BppApp::getAppname_EN () const
{
	m_strBufExch = GETCONAPPDATA.stListAppProj.NAME_EN;
	return m_strBufExch;
}

/**---------------------------------------------------------------------------*/
const CBString BppApp::getAppname_FR () const
{
	m_strBufExch = GETCONAPPDATA.stListAppProj.NAME_FR;
	return m_strBufExch;
}

/**---------------------------------------------------------------------------*/
const CBString BppApp::getAppname_HR () const
{
	m_strBufExch = GETCONAPPDATA.stListAppProj.NAME_HR;
	return m_strBufExch;
}

/**---------------------------------------------------------------------------*/
const CBString BppApp::getAppDesc() const
{
	m_strBufExch = GETCONAPPDATA.stListAppProj.DESCRIPTION;
	return m_strBufExch;
}

/**---------------------------------------------------------------------------*/
void BppApp::setAppid(long lAppid)
{
	GETCONAPPDATA.stListAppProj.APPLICATIONID = lAppid;
}

/**---------------------------------------------------------------------------*/
void  BppApp::setAppname(CBString strAppname)
{
	strcpy(GETCONAPPDATA.stListAppProj.NAME, strAppname);
}

/**---------------------------------------------------------------------------*/
void  BppApp::setAppname_EN       (CBString strAppname)
{
	strcpy(GETCONAPPDATA.stListAppProj.NAME_EN, strAppname);
}

/**---------------------------------------------------------------------------*/
void  BppApp::setAppname_FR       (CBString strAppname)
{
	strcpy(GETCONAPPDATA.stListAppProj.NAME_FR, strAppname);
}

/**---------------------------------------------------------------------------*/
void  BppApp::setAppname_HR       (CBString strAppname)
{
	strcpy(GETCONAPPDATA.stListAppProj.NAME_HR, strAppname);
}

/**---------------------------------------------------------------------------*/
void BppApp::setAppDesc(CBString strAppDesc)
{
	strcpy(GETCONAPPDATA.stListAppProj.DESCRIPTION , strAppDesc);
}

/**---------------------------------------------------------------------------*/
IPerst::ERet BppApp::update()
{
	if (GETCONAPPDATA.stListAppProj.APPLICATIONID == 0)
	{
		DbgMsg(__FUNCTION__ ": AppID 0");
		clearData();
	}
	else
	{
		SppApp objSppApp;

		VAccess::ERet eRet = objSppApp.updateSingle(GETCONAPPDATA);

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
			DbgMsg(__FUNCTION__ ": AppID %d not found in PPAPPLICATION!", 
				GETCONAPPDATA.stListAppProj.APPLICATIONID);

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
IPerst::ERet BppApp::insert()
{
	char szTest[] = "";
	if (strcmp (szTest,GETCONAPPDATA.stListAppProj.NAME) == 0)
	{
		DbgMsg(__FUNCTION__ ": Appname not set");
		clearData();
	}
	else
	{
		SppApp objSppApp;

		VAccess::ERet eRet = objSppApp.insertSingle(GETCONAPPDATA);

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
IPerst::ERet BppApp::delApp()
{
	if (GETCONAPPDATA.stListAppProj.APPLICATIONID == 0) 
	{
		DbgMsg(__FUNCTION__ ": Appid 0");
		clearData();
		return IPerst::RET_ERROR;
	}
	else
	{
		IPerst::ERet eRet = delppApplication();
		if (eRet == IPerst::RET_OK)
		{
			//ppLoginPrevious
			eRet = delPrevious();
			if (eRet == IPerst::RET_ERROR)
			{
				return eRet;
			}

			//ppCorAppUserGroup
			eRet = delCorAppUserGroup();
			if (eRet == IPerst::RET_ERROR)
			{
				return eRet;
			}

			//ppCorAppLang
			eRet = delCorAppLang();
			if (eRet == IPerst::RET_ERROR)
			{
				return eRet;
			}

			//ppText
			eRet = delppText();
			if (eRet == IPerst::RET_ERROR)
			{
				return eRet;
			}

			//ppAppModule
			eRet = delAppModule();
			if (eRet == IPerst::RET_ERROR)
			{
				return eRet;
			}
			
			//ppAppRight
			eRet = delCorAppRight();
			if (eRet == IPerst::RET_ERROR)
			{
				return eRet;
			}

			//ppAppModRight
			eRet = delCorAppModRight();
			if (eRet == IPerst::RET_ERROR)
			{
				return eRet;
			}
			
			//ppCorAppBranchReg
			eRet = delCorAppBranchReg();
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
IPerst::ERet BppApp::delPrevious()
{
	SppApp objSppApp;

	VAccess::ERet eRet = objSppApp.deletePrevious(GETCONAPPDATA);

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
			DbgMsg(__FUNCTION__ ": AppID %d not found in PPLOGINPREVIOUS!", 
				GETCONAPPDATA.stListAppProj.APPLICATIONID);
			clearData();

			return IPerst::RET_WARNING;
	
		default:
			assert(0);
			return IPerst::RET_ERROR;
	}

	return IPerst::RET_OK;
}

/**---------------------------------------------------------------------------*/
IPerst::ERet BppApp::delppApplication()
{
	SppApp objSppApp;

	VAccess::ERet eRet = objSppApp.deleteSingle(GETCONAPPDATA);

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
			DbgMsg(__FUNCTION__ ": AppID %d not found in PPAPPLICATION!", 
				GETCONAPPDATA.stListAppProj.APPLICATIONID);
			clearData();

			return IPerst::RET_WARNING;
	
		default:
			assert(0);
			return IPerst::RET_ERROR;
	}

	return IPerst::RET_OK;
}

/**---------------------------------------------------------------------------*/
IPerst::ERet BppApp::delCorAppUserGroup  ()
{
	SppApp objSppApp;

	VAccess::ERet eRet = objSppApp.deleteCorAppUserGroup(GETCONAPPDATA);

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
			DbgMsg(__FUNCTION__ ": AppID %d not found in PPCORAPPUSERGROUP!", 
				GETCONAPPDATA.stListAppProj.APPLICATIONID);
			clearData();

			return IPerst::RET_WARNING;
	
		default:
			assert(0);
			return IPerst::RET_ERROR;
	}

	return IPerst::RET_OK;
}

/**---------------------------------------------------------------------------*/
IPerst::ERet BppApp::delCorAppLang()
{
	SppApp objSppApp;

	VAccess::ERet eRet = objSppApp.deleteCorAppLang(GETCONAPPDATA);

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
			DbgMsg(__FUNCTION__ ": AppID %d not found in PPCORAPPLANG!", 
				GETCONAPPDATA.stListAppProj.APPLICATIONID);
			clearData();

			return IPerst::RET_WARNING;
	
		default:
			assert(0);
			return IPerst::RET_ERROR;
	}

	return IPerst::RET_OK;
}

/**---------------------------------------------------------------------------*/
IPerst::ERet BppApp::delppText()
{
	SppApp objSppApp;

	VAccess::ERet eRet = objSppApp.deleteppText(GETCONAPPDATA);

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
			DbgMsg(__FUNCTION__ ": AppID %d not found in PPTEXT!", 
				GETCONAPPDATA.stListAppProj.APPLICATIONID);
			clearData();

			return IPerst::RET_WARNING;
	
		default:
			assert(0);
			return IPerst::RET_ERROR;
	}

	return IPerst::RET_OK;
}

/**---------------------------------------------------------------------------*/
IPerst::ERet BppApp::delAppModule()
{
	SppApp objSppApp;

	VAccess::ERet eRet = objSppApp.deleteAppModule(GETCONAPPDATA);

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
			DbgMsg(__FUNCTION__ ": AppID %d not found in PPAPPMODULE!", 
				GETCONAPPDATA.stListAppProj.APPLICATIONID);
			clearData();

			return IPerst::RET_WARNING;
	
		default:
			assert(0);
			return IPerst::RET_ERROR;
	}

	return IPerst::RET_OK;
}

/**---------------------------------------------------------------------------*/
IPerst::ERet BppApp::delCorAppBranchReg  ()
{
	SppApp objSppApp;

	VAccess::ERet eRet = objSppApp.deleteCorAppBranchReg(GETCONAPPDATA);

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
			DbgMsg(__FUNCTION__ ": AppID %d not found in PPCORAPPBRANCHREG!", 
				GETCONAPPDATA.stListAppProj.APPLICATIONID);
			clearData();

			return IPerst::RET_WARNING;
	
		default:
			assert(0);
			return IPerst::RET_ERROR;
	}

	return IPerst::RET_OK;
}

/**---------------------------------------------------------------------------*/
IPerst::ERet BppApp::delCorAppModRight	()
{
	SppApp objSppApp;

	VAccess::ERet eRet = objSppApp.deleteAppModRight(GETCONAPPDATA);

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
			DbgMsg(__FUNCTION__ ": AppID %d not found in PPAPPMODRIGHT!", 
				GETCONAPPDATA.stListAppProj.APPLICATIONID);
			clearData();

			return IPerst::RET_WARNING;
	
		default:
			assert(0);
			return IPerst::RET_ERROR;
	}

	return IPerst::RET_OK;
}

/**---------------------------------------------------------------------------*/
IPerst::ERet BppApp::delCorAppRight()
{
	SppApp objSppApp;

	VAccess::ERet eRet = objSppApp.deleteAppRight(GETCONAPPDATA);

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
			DbgMsg(__FUNCTION__ ": AppID %d not found in PPAPPRIGHT!", 
				GETCONAPPDATA.stListAppProj.APPLICATIONID);
			clearData();

			return IPerst::RET_WARNING;
	
		default:
			assert(0);
			return IPerst::RET_ERROR;
	}

	return IPerst::RET_OK;
}

/**---------------------------------------------------------------------------*/
IPerst::ERet BppApp::SelForUpd()
{
	if (GETCONAPPDATA.stListAppProj.APPLICATIONID == 0) 
	{
		DbgMsg(__FUNCTION__ ": Appid 0");
		clearData();
		return IPerst::RET_ERROR;
	}
	else
	{
		SppApp objSppApp;
		
		VAccess::ERet eRet = objSppApp.SelTestForUpd(GETCONAPPDATA);

		switch (eRet)
		{
		case VAccess::RET_OK:
			break;

		case VAccess::RET_NOTFOUND:
			DbgMsg(__FUNCTION__ ": Appid %d not found in PPAPPLICATION!", 
				GETCONAPPDATA.stListAppProj.APPLICATIONID);
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
IPerst::ERet BppApp::selectID()
{
	char szTest[] = "";
	if (strcmp (szTest,GETCONAPPDATA.stListAppProj.NAME) == 0)
	{
		DbgMsg(__FUNCTION__ ": Appname not set");
		clearData();
	}
	else
	{
		SppApp objSppApp;
		long lAppid;
		CBString strAN = getAppname();

		VAccess::ERet eRet = objSppApp.selectID(strAN, lAppid);

		switch (eRet)
		{
		case VAccess::RET_OK:
			setAppid(lAppid);
			break;

		case VAccess::RET_NOTFOUND:
			DbgMsg(__FUNCTION__ ": Appname %s not found in PPAPPLICATION!", 
				GETCONAPPDATA.stListAppProj.NAME);
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
