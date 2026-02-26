/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                     */
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


//---------------------------------------------------------------------------
#define GETLANGDATA		((SppLanguage::SListProj &)giveData())

SCBMEB_DEFINE_BSN(BppLanguage, SppLanguage::SListProj)

//---------------------------------------------------------------------------
BppLanguage::BppLanguage()
{
}

//---------------------------------------------------------------------------
BppLanguage::BppLanguage(const BppLanguage& r) : VBsnObject(r)
{
	cpyMember(r);
}

//---------------------------------------------------------------------------
BppLanguage::~BppLanguage()
{
}

/**---------------------------------------------------------------------------*/
BppLanguage * BppLanguage::clone() const
{
	return new BppLanguage(*this);
}

/**---------------------------------------------------------------------------*/
void BppLanguage::cpyMember(const BppLanguage & r)
{
    setData((r.giveData()));
}



/**---------------------------------------------------------------------------*/
IPerst::ERet BppLanguage::update()
{
	char szTest[] = "";
	if (strcmp (szTest,GETLANGDATA.stListProj.LANGUAGEID) == 0)
	{
		DbgMsg(__FUNCTION__ ": Langid not set");
		clearData();
	}
	else
	{
		SppLanguage objSppLang;

		VAccess::ERet eRet = objSppLang.updateSingle(GETLANGDATA);

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
			DbgMsg(__FUNCTION__ ": LangID %s not found in PPLOGINLANGUAGE!", 
				GETLANGDATA.stListProj.LANGUAGEID);

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
IPerst::ERet BppLanguage::insert()
{
	char szTest[] = "";
	if (strcmp (szTest,GETLANGDATA.stListProj.LANGUAGEID) == 0)
	{
		DbgMsg(__FUNCTION__ ": Langid not set");
		clearData();
	}
	else
	{
		SppLanguage objSppLang;

		VAccess::ERet eRet = objSppLang.insertSingle(GETLANGDATA);

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
IPerst::ERet BppLanguage::delLang()
{
	char szTest[] = "";
	if (strcmp (szTest,GETLANGDATA.stListProj.LANGUAGEID) == 0)
	{
		DbgMsg(__FUNCTION__ ": Langid not set");
		clearData();
		return IPerst::RET_ERROR;
	}
	else
	{
		IPerst::ERet eRet = deleteSingle();
		if (eRet == IPerst::RET_OK)
		{
			//ppLoginPrevious
			eRet = deletePrevious();
			if (eRet == IPerst::RET_ERROR)
			{
				return eRet;
			}
			
			//ppAppCorLang
			eRet = deleteAppCorLang();
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
			
			return IPerst::RET_OK;
		}
		
		return eRet;
	}
}
/**---------------------------------------------------------------------------*/
IPerst::ERet BppLanguage::SelForUpd()
{
	char szTest[] = "";
	if (strcmp (szTest,GETLANGDATA.stListProj.LANGUAGEID) == 0)
	{
		DbgMsg(__FUNCTION__ ": Langid not set");
		clearData();
		return IPerst::RET_ERROR;
	}
	else
	{
		SppLanguage objSppLang;
		
		VAccess::ERet eRet = objSppLang.SelTestForUpd(GETLANGDATA);

		switch (eRet)
		{
		case VAccess::RET_OK:
			break;

		case VAccess::RET_NOTFOUND:
			DbgMsg(__FUNCTION__ ": Langid %s not found in PPLOGINLANGUAGE!", 
				GETLANGDATA.stListProj.LANGUAGEID);
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
IPerst::ERet BppLanguage::deleteSingle ()
{
	char szTest[] = "";
	if (strcmp (szTest,GETLANGDATA.stListProj.LANGUAGEID) == 0)
	{
		DbgMsg(__FUNCTION__ ": Langid not set");
		clearData();
		return IPerst::RET_ERROR;
	}
	else
	{
		SppLanguage objSppLang;

		VAccess::ERet eRet = objSppLang.deleteSingle(GETLANGDATA);

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
				DbgMsg(__FUNCTION__ ": LangID %s not found in PPLOGINLANGUAGE!", 
					GETLANGDATA.stListProj.LANGUAGEID);
				clearData();

				return IPerst::RET_WARNING;
	
			default:
				assert(0);
				return IPerst::RET_ERROR;
		}

		return IPerst::RET_OK;
	}
}

/**---------------------------------------------------------------------------*/
IPerst::ERet BppLanguage::deletePrevious      ()
{
	char szTest[] = "";
	if (strcmp (szTest,GETLANGDATA.stListProj.LANGUAGEID) == 0)
	{
		DbgMsg(__FUNCTION__ ": Langid not set");
		clearData();
		return IPerst::RET_ERROR;
	}
	else
	{
		SppLanguage objSppLang;

		VAccess::ERet eRet = objSppLang.deletePrevious(GETLANGDATA);

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

		return IPerst::RET_OK;
	}
}

/**---------------------------------------------------------------------------*/
IPerst::ERet BppLanguage::deleteAppCorLang    ()
{
	char szTest[] = "";
	if (strcmp (szTest,GETLANGDATA.stListProj.LANGUAGEID) == 0)
	{
		DbgMsg(__FUNCTION__ ": Langid not set");
		clearData();
		return IPerst::RET_ERROR;
	}
	else
	{
		SppLanguage objSppLang;

		VAccess::ERet eRet = objSppLang.deleteAppCorLang(GETLANGDATA);

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

		return IPerst::RET_OK;
	}
}

/**---------------------------------------------------------------------------*/
IPerst::ERet BppLanguage::deleteppText        ()
{
	char szTest[] = "";
	if (strcmp (szTest,GETLANGDATA.stListProj.LANGUAGEID) == 0)
	{
		DbgMsg(__FUNCTION__ ": Langid not set");
		clearData();
		return IPerst::RET_ERROR;
	}
	else
	{
		SppLanguage objSppLang;

		VAccess::ERet eRet = objSppLang.deleteppText(GETLANGDATA);

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

		return IPerst::RET_OK;
	}
}

/**---------------------------------------------------------------------------*/
IPerst::ERet BppLanguage::insertCorLang       ()
{
	char szTest[] = "";
	if (strcmp (szTest,GETLANGDATA.stListProj.LANGUAGEID) == 0 || GETLANGDATA.stListCond.APPLICATIONID == 0)
	{
		DbgMsg(__FUNCTION__ ": Langid or Appid not set");
		clearData();
	}
	else
	{
		SppLanguage objSppLang;

		VAccess::ERet eRet = objSppLang.insertCorLang(GETLANGDATA);

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
IPerst::ERet BppLanguage::deleteCorLang       ()
{
	char szTest[] = "";
	if (strcmp (szTest,GETLANGDATA.stListProj.LANGUAGEID) == 0 || GETLANGDATA.stListCond.APPLICATIONID == 0)
	{
		DbgMsg(__FUNCTION__ ": Langid or Appid not set");
		clearData();
	}
	else
	{
		SppLanguage objSppLang;

		VAccess::ERet eRet = objSppLang.deleteCorLang(GETLANGDATA);
		IPerst::ERet Ret;

		switch (eRet)
		{
			IPerst::ERet Ret2;
		case VAccess::RET_OK:
			Ret2 = BSqlStat::ex().UpdateBackupMachine();
			if (Ret2 == IPerst::RET_ERROR)
			{
				DbgMsg(__FUNCTION__ ": Error on updating backup-machine!");
			}
		
			Ret = deleteCorppText();
			if (Ret == IPerst::RET_ERROR)
			{
					assert(0);
					return IPerst::RET_ERROR;
			}
			else
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
IPerst::ERet BppLanguage::deleteCorppText     ()
{
	char szTest[] = "";
	if (strcmp (szTest,GETLANGDATA.stListProj.LANGUAGEID) == 0 || GETLANGDATA.stListCond.APPLICATIONID == 0)
	{
		DbgMsg(__FUNCTION__ ": Langid or Appid not set");
		clearData();
		return IPerst::RET_ERROR;
	}
	else
	{
		SppLanguage objSppLang;

		VAccess::ERet eRet = objSppLang.deleteCorppText(GETLANGDATA);

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

		return IPerst::RET_OK;
	}
}

/**---------------------------------------------------------------------------*/
void BppLanguage::setLangid(CBString strLangid)
{
	strcpy(GETLANGDATA.stListProj.LANGUAGEID, strLangid);
}

/**---------------------------------------------------------------------------*/
void BppLanguage::setLangname(CBString strLangname)
{
	strcpy(GETLANGDATA.stListProj.NAME, strLangname);
}

/**---------------------------------------------------------------------------*/
void BppLanguage::setAppid	(short sAppid)
{
	GETLANGDATA.stListCond.APPLICATIONID = sAppid;
}

/**---------------------------------------------------------------------------*/
const CBString	BppLanguage::getLangid() const
{
	m_strBufExch = GETLANGDATA.stListProj.LANGUAGEID;
	return m_strBufExch;
}

/**---------------------------------------------------------------------------*/
const CBString BppLanguage::getLangname() const
{
	m_strBufExch = GETLANGDATA.stListProj.NAME;
	return m_strBufExch;
}
	
/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/	
