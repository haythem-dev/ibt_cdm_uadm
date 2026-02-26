/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                     */
/**   Date   : 25.05.2004                                                     */
/**                                                                           */
/******************************************************************************/

/*----------------------------------------------------------------------------*/
// Includes
/*----------------------------------------------------------------------------*/
#include "bsn.h"		// precompiled header


#include <STest.h>

#include "BTest.h"

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
BTest::BTest()
{
	m_pSTest = new STest;
		assert(m_pSTest);
}

//---------------------------------------------------------------------------
BTest::~BTest()
{
	if (m_pSTest != NULL)
	{
		delete m_pSTest;
		m_pSTest = NULL;
	}
}

/**---------------------------------------------------------------------------*/
void  BTest::setLangid(CBString strLangid)
{
	m_strLangid = strLangid;
}

/**---------------------------------------------------------------------------*/
void  BTest::setUsername(CBString strUsername)
{
	m_strUsername = strUsername;
}
/**---------------------------------------------------------------------------*/
void BTest::setPara1(CBString strPara1)
{
	m_strPara1 = strPara1;
}

/**---------------------------------------------------------------------------*/
void BTest::setPara2(CBString strPara2)
{
	m_strPara2 = strPara2;
}

/**---------------------------------------------------------------------------*/
void BTest::setParaBez(CBString strParaBez)
{
	m_strParaBez = strParaBez;
}

/**---------------------------------------------------------------------------*/
void BTest::setAppname(CBString strAppname)
{
	m_strAppname = strAppname;
}

/**---------------------------------------------------------------------------*/
void BTest::setReftext(CBString strReftext)
{
	m_strReftext = strReftext;
}
	
/**---------------------------------------------------------------------------*/
void BTest::setUsertypeName(CBString strUsertypeName)
{
	m_strUsertypeName = strUsertypeName;
}

/**---------------------------------------------------------------------------*/
void BTest::setUserParaName(CBString strUserParaName)
{
	m_strUserParaName = strUserParaName;
}
	
/**---------------------------------------------------------------------------*/
const CBString BTest::getUsername() const
{
	return m_strUsername;
}

/**---------------------------------------------------------------------------*/
const short BTest::getCounter() const
{
	return m_sCounter;
}

/**---------------------------------------------------------------------------*/
bool BTest::SelParaExists(bool & bExists)
{
	int iRet = m_pSTest->selectParaExists(m_sCounter,m_strPara1,m_strPara2,m_strParaBez,m_sAppid);
	
	if (iRet == IPerst::RET_OK)
	{
		if (m_sCounter > 0)
			bExists = true; //Parameter bereits vorhanden
		else
			bExists = false;

		return true;
	}
	else
	{
		//SQL-Abfrage fehlgeschlagen
		return false;
	}
}

/**---------------------------------------------------------------------------*/
bool BTest::SelReftextExists	(bool & bExists)
{
	int iRet = m_pSTest->selectReftextExists(m_sCounter,m_strReftext);
	
	if (iRet == IPerst::RET_OK)
	{
		if (m_sCounter > 0)
			bExists = true; //Reftext bereits vorhanden
		else
			bExists = false;

		return true;
	}
	else
	{
		//SQL-Abfrage fehlgeschlagen
		return false;
	}
}

/**---------------------------------------------------------------------------*/
bool BTest::SelLangidExists		(bool & bExists)
{
	int iRet = m_pSTest->selectLangIDExists(m_sCounter,m_strLangid);
	
	if (iRet == IPerst::RET_OK)
	{
		if (m_sCounter > 0)
			bExists = true; //Langid bereits vorhanden
		else
			bExists = false;

		return true;
	}
	else
	{
		//SQL-Abfrage fehlgeschlagen
		return false;
	}
}

/**---------------------------------------------------------------------------*/
bool BTest::SelModIDExists(bool & bExists)
{
	int iRet = m_pSTest->selectModIDExists(m_sCounter,m_sAppid,m_sModid);
	
	if (iRet == IPerst::RET_OK)
	{
		if (m_sCounter > 0)
			bExists = true; //Modid bereits vorhanden
		else
			bExists = false;

		return true;
	}
	else
	{
		//SQL-Abfrage fehlgeschlagen
		return false;
	}
}

/**---------------------------------------------------------------------------*/
void BTest::setAppid(short sAppid)
{
	m_sAppid = sAppid;
}

/**---------------------------------------------------------------------------*/
void BTest::setModid(short sModid)
{
	m_sModid = sModid;
}

/**---------------------------------------------------------------------------*/
void BTest::setBrRegid(short sBrRegid)
{
	m_sBrRegid = sBrRegid;
}
	
/**---------------------------------------------------------------------------*/
void BTest::setRiname(CBString strRiName)
{
	m_strRiName = strRiName;
}

/**---------------------------------------------------------------------------*/
void BTest::setGrpname(CBString strGrpName)
{
	m_strGrpName = strGrpName;
}

/**---------------------------------------------------------------------------*/
bool BTest::SelGrpNameExists	(bool & bExists)
{
	int iRet = m_pSTest->selectGrpNameExists(m_sCounter,m_strGrpName);
	
	if (iRet == IPerst::RET_OK)
	{
		if (m_sCounter > 0)
			bExists = true; //grpname bereits vorhanden
		else
			bExists = false;

		return true;
	}
	else
	{
		//SQL-Abfrage fehlgeschlagen
		return false;
	}
}

/**---------------------------------------------------------------------------*/
bool BTest::SelUsertypeNameExists(bool& bExists)
{
	int iRet = m_pSTest->selectUsertypeNameExists(m_sCounter, m_strUsertypeName);

	if (iRet == IPerst::RET_OK)
	{
		if (m_sCounter > 0)
			bExists = true; //UsertypeName bereits vorhanden
		else
			bExists = false;

		return true;
	}
	else
	{
		//SQL-Abfrage fehlgeschlagen
		return false;
	}
}

/**---------------------------------------------------------------------------*/
bool BTest::SelUserParaNameExists(bool& bExists)
{
	int iRet = m_pSTest->selectUserParaNameExists(m_sCounter, m_strUserParaName);

	if (iRet == IPerst::RET_OK)
	{
		if (m_sCounter > 0)
			bExists = true; //UserParaName bereits vorhanden
		else
			bExists = false;

		return true;
	}
	else
	{
		//SQL-Abfrage fehlgeschlagen
		return false;
	}
}

/**---------------------------------------------------------------------------*/
bool BTest::SelRiNameExists(bool & bExists)
{
	int iRet = m_pSTest->selectRiNameExists(m_sCounter,m_strRiName, m_sAppid, m_sModid);
	
	if (iRet == IPerst::RET_OK)
	{
		if (m_sCounter > 0)
			bExists = true; //appname bereits vorhanden
		else
			bExists = false;

		return true;
	}
	else
	{
		//SQL-Abfrage fehlgeschlagen
		return false;
	}
}

/**---------------------------------------------------------------------------*/
bool BTest::SelAppNameExists	(bool & bExists)
{
	int iRet = m_pSTest->selectAppNameExists(m_sCounter,m_strAppname);
	
	if (iRet == IPerst::RET_OK)
	{
		if (m_sCounter > 0)
			bExists = true; //appname bereits vorhanden
		else
			bExists = false;

		return true;
	}
	else
	{
		//SQL-Abfrage fehlgeschlagen
		return false;
	}
}

/**---------------------------------------------------------------------------*/
bool BTest::SelUNameExists(bool & bExists)
{
	int iRet = m_pSTest->selectUNameExists(m_sCounter,m_strUsername);
	
	if (iRet == IPerst::RET_OK)
	{
		if (m_sCounter > 0)
			bExists = true; //Username bereits vorhanden
		else
			bExists = false;

		return true;
	}
	else
	{
		//SQL-Abfrage fehlgeschlagen
		return false;
	}
}

/**---------------------------------------------------------------------------*/
bool BTest::SelBrIDExists(bool & bExists)
{
	int iRet = m_pSTest->selectBrIDExists(m_sCounter,m_sBrRegid);
	
	if (iRet == IPerst::RET_OK)
	{
		if (m_sCounter > 0)
			bExists = true; //Branchid bereits vorhanden
		else
			bExists = false;

		return true;
	}
	else
	{
		//SQL-Abfrage fehlgeschlagen
		return false;
	}
}

/**---------------------------------------------------------------------------*/
bool BTest::SelRegIDExists(bool & bExists)
{
	int iRet = m_pSTest->selectRegIDExists(m_sCounter,m_sBrRegid);
	
	if (iRet == IPerst::RET_OK)
	{
		if (m_sCounter > 0)
			bExists = true; //Regid bereits vorhanden
		else
			bExists = false;

		return true;
	}
	else
	{
		//SQL-Abfrage fehlgeschlagen
		return false;
	}
}

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/	
