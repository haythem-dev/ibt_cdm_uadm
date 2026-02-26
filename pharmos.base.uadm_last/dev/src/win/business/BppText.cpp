/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                     */
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


//---------------------------------------------------------------------------
#define GETTEXTDATA		((SppText::SListProj &)giveData())

SCBMEB_DEFINE_BSN(BppText, SppText::SListProj)

//---------------------------------------------------------------------------
BppText::BppText()
{
}

//---------------------------------------------------------------------------
BppText::BppText(const BppText& r) : VBsnObject(r)
{
	cpyMember(r);
}

//---------------------------------------------------------------------------
BppText::~BppText()
{
}

/**---------------------------------------------------------------------------*/
BppText * BppText::clone() const
{
	return new BppText(*this);
}

/**---------------------------------------------------------------------------*/
void BppText::cpyMember(const BppText & r)
{
    setData((r.giveData()));
}

/**---------------------------------------------------------------------------*/
IPerst::ERet BppText::selTextEN(CBString & strTextEN)
{
	char szTest[] = "";
	if ( (strcmp (szTest,GETTEXTDATA.stListCond.LANGUAGEID) == 0) || GETTEXTDATA.stListCond.APPLICATIONID == 0 || GETTEXTDATA.stListCond.MODULEID == 0 || (strcmp (szTest,GETTEXTDATA.stListProj.REFTEXT) == 0))
	{
		DbgMsg(__FUNCTION__ ": Langid or Appid or Modid or Reftext not set");
		clearData();
	}
	else
	{
		SppText objSppText;

		VAccess::ERet eRet = objSppText.selectTextEN(GETTEXTDATA,strTextEN);

		switch (eRet)
		{
		case VAccess::RET_OK:
			break;

		case VAccess::RET_NOTFOUND:
			DbgMsg(__FUNCTION__ ": Reftext %s or Langid %s not found in PPTEXT!", 
				GETTEXTDATA.stListProj.REFTEXT, GETTEXTDATA.stListCond.LANGUAGEID);

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
IPerst::ERet BppText::update()
{
	char szTest[] = "";
	if ( (strcmp (szTest,GETTEXTDATA.stListCond.LANGUAGEID) == 0) || GETTEXTDATA.stListCond.APPLICATIONID == 0 || GETTEXTDATA.stListCond.MODULEID == 0 || (strcmp (szTest,GETTEXTDATA.stListProj.REFTEXT) == 0))
	{
		DbgMsg(__FUNCTION__ ": Langid or Appid or Modid or Reftext not set");
		clearData();
	}
	else
	{
		SppText objSppText;

		VAccess::ERet eRet = objSppText.updateText(GETTEXTDATA);

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
			DbgMsg(__FUNCTION__ ": Reftext %s or Langid %s not found in PPTEXT!", 
				GETTEXTDATA.stListProj.REFTEXT, GETTEXTDATA.stListCond.LANGUAGEID);

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
IPerst::ERet BppText::insert()
{
	char szTest[] = "";
	if ( (strcmp (szTest,GETTEXTDATA.stListCond.LANGUAGEID) == 0) || GETTEXTDATA.stListCond.APPLICATIONID == 0 || GETTEXTDATA.stListCond.MODULEID == 0 || (strcmp (szTest,GETTEXTDATA.stListProj.REFTEXT) == 0))
	{
		DbgMsg(__FUNCTION__ ": Langid or Appid or Modid or Reftext not set");
		clearData();
	}
	else
	{
		SppText objSppText;

		VAccess::ERet eRet = objSppText.insertText(GETTEXTDATA);

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
IPerst::ERet BppText::SelForUpd()
{
	char szTest[] = "";
	if ( (strcmp (szTest,GETTEXTDATA.stListCond.LANGUAGEID) == 0) || GETTEXTDATA.stListCond.APPLICATIONID == 0 || GETTEXTDATA.stListCond.MODULEID == 0 || (strcmp (szTest,GETTEXTDATA.stListProj.REFTEXT) == 0))
	{
		DbgMsg(__FUNCTION__ ": Langid or Appid or Modid or Reftext not set");
		clearData();
	}
	else
	{
		SppText objSppText;
		
		VAccess::ERet eRet = objSppText.SelTestForUpd(GETTEXTDATA);

		switch (eRet)
		{
		case VAccess::RET_OK:
			break;

		case VAccess::RET_NOTFOUND:
			DbgMsg(__FUNCTION__ ": No data for update found in PPTEXT!");
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
IPerst::ERet BppText::delRefText () 
{
	char szTest[] = "";
	if ( GETTEXTDATA.stListCond.APPLICATIONID == 0 || GETTEXTDATA.stListCond.MODULEID == 0 || (strcmp (szTest,GETTEXTDATA.stListProj.REFTEXT) == 0))
	{
		DbgMsg(__FUNCTION__ ": Appid or Modid or Reftext not set");
		clearData();
	}
	else
	{
		SppText objSppText;

		VAccess::ERet eRet = objSppText.deleteRefText(GETTEXTDATA);

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
				DbgMsg(__FUNCTION__ ": Reftext %s or Langid %s not found in PPTEXT!", 
					GETTEXTDATA.stListProj.REFTEXT, GETTEXTDATA.stListCond.LANGUAGEID);
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
IPerst::ERet BppText::delLangText () 
{
	char szTest[] = "";
	if ( (strcmp (szTest,GETTEXTDATA.stListCond.LANGUAGEID) == 0) || GETTEXTDATA.stListCond.APPLICATIONID == 0 || GETTEXTDATA.stListCond.MODULEID == 0 || (strcmp (szTest,GETTEXTDATA.stListProj.REFTEXT) == 0))
	{
		DbgMsg(__FUNCTION__ ": Langid or Appid or Modid or Reftext not set");
		clearData();
	}
	else
	{
		SppText objSppText;

		VAccess::ERet eRet = objSppText.deleteLangText(GETTEXTDATA);

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
				DbgMsg(__FUNCTION__ ": Reftext %s or Langid %s not found in PPTEXT!", 
					GETTEXTDATA.stListProj.REFTEXT, GETTEXTDATA.stListCond.LANGUAGEID);
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
void BppText::setLangid(CBString strLangid)
{
	strcpy(GETTEXTDATA.stListCond.LANGUAGEID, strLangid);
}

/**---------------------------------------------------------------------------*/
void BppText::setReftext(CBString strReftext)
{
	strcpy(GETTEXTDATA.stListProj.REFTEXT, strReftext);
}

/**---------------------------------------------------------------------------*/
void BppText::setText(CBString strText)
{
	strcpy(GETTEXTDATA.stListProj.TEXT, strText);
}


/**---------------------------------------------------------------------------*/
void BppText::setAppid	(short sAppid)
{
	GETTEXTDATA.stListCond.APPLICATIONID = sAppid;
}

/**---------------------------------------------------------------------------*/
void BppText::setModid	(short sModid)
{
	GETTEXTDATA.stListCond.MODULEID = sModid;
}

/**---------------------------------------------------------------------------*/
const CBString	BppText::getReftext() const
{
	m_strBufExch = GETTEXTDATA.stListProj.REFTEXT;
	return m_strBufExch;
}

/**---------------------------------------------------------------------------*/
const CBString	BppText::getLangid() const
{
	m_strBufExch = GETTEXTDATA.stListCond.LANGUAGEID;
	return m_strBufExch;
}

/**---------------------------------------------------------------------------*/
const CBString BppText::getText() const
{
	m_strBufExch = GETTEXTDATA.stListProj.TEXT;
	return m_strBufExch;
}

/**---------------------------------------------------------------------------*/
const short	BppText::getAppid			() const
{
	return GETTEXTDATA.stListCond.APPLICATIONID;
}

/**---------------------------------------------------------------------------*/
const short	BppText::getModid			() const
{
	return GETTEXTDATA.stListCond.MODULEID;
}
	
	
/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/	
