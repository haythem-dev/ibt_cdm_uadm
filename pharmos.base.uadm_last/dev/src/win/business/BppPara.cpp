/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                     */
/**   Date   : 03.08.2004                                                     */
/**                                                                           */
/******************************************************************************/

/*----------------------------------------------------------------------------*/
// Includes
/*----------------------------------------------------------------------------*/
#include "bsn.h"		// precompiled header

#include <tbl_pplogpara.h>
#include <struct_para.h>
#include <SppPara.h>

#include "BppPara.h"

//---------------------------------------------------------------------------
#define GETPARADATA		((SppPara::SListProj &)giveData())

SCBMEB_DEFINE_BSN(BppPara, SppPara::SListProj)

//---------------------------------------------------------------------------
BppPara::BppPara()
{
}

//---------------------------------------------------------------------------
BppPara::BppPara(const BppPara& r) : VBsnObject(r)
{
	cpyMember(r);
}

//---------------------------------------------------------------------------
BppPara::~BppPara()
{
}

/**---------------------------------------------------------------------------*/
BppPara * BppPara::clone() const
{
	return new BppPara(*this);
}

/**---------------------------------------------------------------------------*/
void BppPara::cpyMember(const BppPara & r)
{
    setData((r.giveData()));
}

/**---------------------------------------------------------------------------*/
IPerst::ERet BppPara::insertPara()
{
	if (GETPARADATA.stListCond.APPLICATIONID == 0) 
	{
		DbgMsg(__FUNCTION__ ": Appid 0");
		clearData();
		return IPerst::RET_ERROR;
	}
	else
	{
		SppPara objSppPara;

		VAccess::ERet eRet = objSppPara.insertPara(GETPARADATA);

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
IPerst::ERet BppPara::SelForUpd()
{
	if (GETPARADATA.stListCond.APPLICATIONID == 0) 
	{
		DbgMsg(__FUNCTION__ ": Appid 0");
		clearData();
		return IPerst::RET_ERROR;
	}
	else
	{
		SppPara objSppPara;
		
		VAccess::ERet eRet = objSppPara.SelTestForUpd(GETPARADATA);

		switch (eRet)
		{
		case VAccess::RET_OK:
			break;

		case VAccess::RET_NOTFOUND:
			DbgMsg(__FUNCTION__ ": Appid %d not found in PPLOGPARA!", 
				GETPARADATA.stListCond.APPLICATIONID);
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
IPerst::ERet BppPara::deletePara()
{
	if (GETPARADATA.stListCond.APPLICATIONID == 0) 
	{
		DbgMsg(__FUNCTION__ ": Appid 0");
		clearData();
		return IPerst::RET_ERROR;
	}
	else
	{
		SppPara objSppPara;

		VAccess::ERet eRet = objSppPara.deletePara(GETPARADATA);

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
IPerst::ERet BppPara::updatePara()
{
	if (GETPARADATA.stListCond.APPLICATIONID == 0) 
	{
		DbgMsg(__FUNCTION__ ": Appid 0");
		clearData();
		return IPerst::RET_ERROR;
	}
	else
	{
		SppPara objSppPara;

		VAccess::ERet eRet = objSppPara.updatePara(GETPARADATA);

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
void BppPara::setAppid	(short sAppid)
{
	GETPARADATA.stListCond.APPLICATIONID = sAppid;
}

/**---------------------------------------------------------------------------*/
void BppPara::setPara1(CBString strPara1)
{
	strcpy(GETPARADATA.stListProj.PARA1, strPara1);
}

/**---------------------------------------------------------------------------*/
void BppPara::setPara2(CBString strPara2)
{
	strcpy(GETPARADATA.stListProj.PARA2, strPara2);
}

/**---------------------------------------------------------------------------*/
void BppPara::setParabez(CBString strParabez)
{
	strcpy(GETPARADATA.stListProj.PARABEZ, strParabez);
}

/**---------------------------------------------------------------------------*/
void BppPara::setParavalue(CBString strParavalue)
{
	strcpy(GETPARADATA.stListProj.PARAVALUE, strParavalue);
}

/**---------------------------------------------------------------------------*/
const short BppPara::getAppid() const
{
	return GETPARADATA.stListCond.APPLICATIONID;
}


/**---------------------------------------------------------------------------*/
const CBString BppPara::getPara1() const
{
	m_strBufExch = GETPARADATA.stListProj.PARA1;
	return m_strBufExch;
}

/**---------------------------------------------------------------------------*/
const CBString BppPara::getPara2() const
{
	m_strBufExch = GETPARADATA.stListProj.PARA2;
	return m_strBufExch;
}

/**---------------------------------------------------------------------------*/
const CBString BppPara::getParabez() const
{
	m_strBufExch = GETPARADATA.stListProj.PARABEZ;
	return m_strBufExch;
}

/**---------------------------------------------------------------------------*/
const CBString BppPara::getParavalue() const
{
	m_strBufExch = GETPARADATA.stListProj.PARAVALUE;
	return m_strBufExch;
}

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/	
