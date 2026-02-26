/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH										       */
/**   Date   : 02.06.2004                                                     */
/**                                                                           */
/******************************************************************************/

#include "srv.h"

#include "STest.h"



/*----------------------------------------------------------------------------*/
STest::STest() 
{
}

/*----------------------------------------------------------------------------*/
STest::~STest()
{
}
/*----------------------------------------------------------------------------*/
VAccess::ERet STest::selectParaExists(short & sCounter, CBString & strPara1, CBString& strPara2, CBString& strParabez, short sAppid )
{
	CBString   strSql;
	double		dCounter;
	
	strSql.format("SELECT count (*) "
					"FROM pplogpara "
					"WHERE "
					"para1 = '%s' "
					"and para2 = '%s' "
					"and parabez = '%s' "
					"and applicationid = %d",
					strPara1.c_str(), strPara2.c_str(), strParabez.c_str(), sAppid);
					

	long lRet = SqlRead((char *)strSql.c_str(), &dCounter, NULL);
	sCounter = (short)dCounter;

	switch (lRet)
	{
	case SQL_SUCCESS:
		return VAccess::RET_OK;

	case SQL_NOTFOUND:
		return VAccess::RET_NOTFOUND;

	default:
		SLog::ex().write(__FUNCTION__, VAccess::RET_ERROR, 
			                "select (<%s>) = %ld",
							strSql.c_str(),
							lRet);
		assert(0);
		return VAccess::RET_ERROR;
	}
}

/*----------------------------------------------------------------------------*/
VAccess::ERet STest::selectReftextExists(short & sCounter, CBString & strReftext )
{
	CBString   strSql;
	double		dCounter;
	
	strSql.format("SELECT count (*) "
					"FROM pptext "
					"WHERE reftext = '%s'",
					strReftext.c_str());
					

	long lRet = SqlRead((char *)strSql.c_str(), &dCounter, NULL);
	sCounter = (short)dCounter;

	switch (lRet)
	{
	case SQL_SUCCESS:
		return VAccess::RET_OK;

	case SQL_NOTFOUND:
		return VAccess::RET_NOTFOUND;

	default:
		SLog::ex().write(__FUNCTION__, VAccess::RET_ERROR, 
			                "select (<%s>) = %ld",
							strSql.c_str(),
							lRet);
		assert(0);
		return VAccess::RET_ERROR;
	}
}
/*----------------------------------------------------------------------------*/
VAccess::ERet STest::selectLangIDExists(short & sCounter, CBString & strLangid )
{
	CBString   strSql;
	double		dCounter;
	
	strSql.format("SELECT count (*) "
					"FROM pploginlanguage "
					"WHERE languageid = '%s'",
					strLangid.c_str());
					

	long lRet = SqlRead((char *)strSql.c_str(), &dCounter, NULL);
	sCounter = (short)dCounter;

	switch (lRet)
	{
	case SQL_SUCCESS:
		return VAccess::RET_OK;

	case SQL_NOTFOUND:
		return VAccess::RET_NOTFOUND;

	default:
		SLog::ex().write(__FUNCTION__, VAccess::RET_ERROR, 
			                "select (<%s>) = %ld",
							strSql.c_str(),
							lRet);
		assert(0);
		return VAccess::RET_ERROR;
	}
}

/*----------------------------------------------------------------------------*/
VAccess::ERet STest::selectBrIDExists(short & sCounter, short sBranchid )
{
	CBString   strSql;
	double		dCounter;
	
	strSql.format("SELECT count (*) "
					"FROM ppbranch "
					"WHERE branchid = %d",
					sBranchid);
					

	long lRet = SqlRead((char *)strSql.c_str(), &dCounter, NULL);
	sCounter = (short)dCounter;

	switch (lRet)
	{
	case SQL_SUCCESS:
		return VAccess::RET_OK;

	case SQL_NOTFOUND:
		return VAccess::RET_NOTFOUND;

	default:
		SLog::ex().write(__FUNCTION__, VAccess::RET_ERROR, 
			                "select (<%s>) = %ld",
							strSql.c_str(),
							lRet);
		assert(0);
		return VAccess::RET_ERROR;
	}
}

/*----------------------------------------------------------------------------*/
VAccess::ERet STest::selectRegIDExists(short & sCounter, short sRegid )
{
	CBString   strSql;
	double		dCounter;
	
	strSql.format("SELECT count (*) "
					"FROM ppregion "
					"WHERE regionid = %d",
					sRegid);
					

	long lRet = SqlRead((char *)strSql.c_str(), &dCounter, NULL);
	sCounter = (short)dCounter;

	switch (lRet)
	{
	case SQL_SUCCESS:
		return VAccess::RET_OK;

	case SQL_NOTFOUND:
		return VAccess::RET_NOTFOUND;

	default:
		SLog::ex().write(__FUNCTION__, VAccess::RET_ERROR, 
			                "select (<%s>) = %ld",
							strSql.c_str(),
							lRet);
		assert(0);
		return VAccess::RET_ERROR;
	}
}

/*----------------------------------------------------------------------------*/
VAccess::ERet STest::selectRiNameExists(short & sCounter, CBString & strRiName, short sAppid, short sModid )
{
	CBString   strSql;
	double		dCounter;
	
	strSql.format("SELECT count (*) "
					"FROM ppappmodright "
					"WHERE rightname = '%s' and applicationid = %d and moduleid = %d",
					strRiName.c_str(), sAppid, sModid);
					

	long lRet = SqlRead((char *)strSql.c_str(), &dCounter, NULL);
	sCounter = (short)dCounter;

	switch (lRet)
	{
	case SQL_SUCCESS:
		return VAccess::RET_OK;

	case SQL_NOTFOUND:
		return VAccess::RET_NOTFOUND;

	default:
		SLog::ex().write(__FUNCTION__, VAccess::RET_ERROR, 
			                "select (<%s>) = %ld",
							strSql.c_str(),
							lRet);
		assert(0);
		return VAccess::RET_ERROR;
	}
}

/*----------------------------------------------------------------------------*/
VAccess::ERet STest::selectAppNameExists(short & sCounter, CBString & strAppname )
{
	CBString   strSql;
	double		dCounter;
	
	strSql.format("SELECT count (*) "
					"FROM ppapplication "
					"WHERE name = '%s'",
					strAppname.c_str());
					

	long lRet = SqlRead((char *)strSql.c_str(), &dCounter, NULL);
	sCounter = (short)dCounter;

	switch (lRet)
	{
	case SQL_SUCCESS:
		return VAccess::RET_OK;

	case SQL_NOTFOUND:
		return VAccess::RET_NOTFOUND;

	default:
		SLog::ex().write(__FUNCTION__, VAccess::RET_ERROR, 
			                "select (<%s>) = %ld",
							strSql.c_str(),
							lRet);
		assert(0);
		return VAccess::RET_ERROR;
	}
}

/*----------------------------------------------------------------------------*/
VAccess::ERet STest::selectUNameExists(short & sCounter, CBString & strUsername )
{
	CBString   strSql;
	double		dCounter;
	
	strSql.format("SELECT count (*) "
					"FROM ppuser "
					"WHERE username = '%s'",
					strUsername.c_str());

	long lRet = SqlRead((char *)strSql.c_str(), &dCounter, NULL);
	sCounter = (short)dCounter;

	switch (lRet)
	{
	case SQL_SUCCESS:
		return VAccess::RET_OK;

	case SQL_NOTFOUND:
		return VAccess::RET_NOTFOUND;

	default:
		SLog::ex().write(__FUNCTION__, VAccess::RET_ERROR, 
			                "select (<%s>) = %ld",
							strSql.c_str(),
							lRet);
		assert(0);
		return VAccess::RET_ERROR;
	}
}

/*----------------------------------------------------------------------------*/
VAccess::ERet STest::selectModIDExists(short & sCounter, short sAppid, short sModid)
{
	CBString   strSql;
	double		dCounter;
	
	strSql.format("SELECT count (*) "
					"FROM ppappmodule "
					"WHERE applicationid = %d and moduleid = %d",
					sAppid, sModid );

	long lRet = SqlRead((char *)strSql.c_str(), &dCounter, NULL);
	sCounter = (short)dCounter;

	switch (lRet)
	{
	case SQL_SUCCESS:
		return VAccess::RET_OK;

	case SQL_NOTFOUND:
		return VAccess::RET_NOTFOUND;

	default:
		SLog::ex().write(__FUNCTION__, VAccess::RET_ERROR, 
			                "select (<%s>) = %ld",
							strSql.c_str(),
							lRet);
		assert(0);
		return VAccess::RET_ERROR;
	}
}

/*----------------------------------------------------------------------------*/
VAccess::ERet STest::selectGrpNameExists(short & sCounter, CBString & strGrpname )
{
	CBString   strSql;
	double		dCounter;
	
	strSql.format("SELECT count (*) "
					"FROM ppgroup "
					"WHERE name = '%s'",
					strGrpname.c_str() );

	long lRet = SqlRead((char *)strSql.c_str(), &dCounter, NULL);
	sCounter = (short)dCounter;

	switch (lRet)
	{
	case SQL_SUCCESS:
		return VAccess::RET_OK;

	case SQL_NOTFOUND:
		return VAccess::RET_NOTFOUND;

	default:
		SLog::ex().write(__FUNCTION__, VAccess::RET_ERROR, 
			                "select (<%s>) = %ld",
							strSql.c_str(),
							lRet);
		assert(0);
		return VAccess::RET_ERROR;
	}
}

/*----------------------------------------------------------------------------*/
VAccess::ERet STest::selectUsertypeNameExists(short& sCounter, CBString& strUsertypename)
{
	CBString   strSql;
	double		dCounter;

	strSql.format("SELECT count (*) "
		"FROM ppusertype "
		"WHERE name = '%s'",
		strUsertypename.c_str());

	long lRet = SqlRead((char*)strSql.c_str(), &dCounter, NULL);
	sCounter = (short)dCounter;

	switch (lRet)
	{
	case SQL_SUCCESS:
		return VAccess::RET_OK;

	case SQL_NOTFOUND:
		return VAccess::RET_NOTFOUND;

	default:
		SLog::ex().write(__FUNCTION__, VAccess::RET_ERROR,
			"select (<%s>) = %ld",
			strSql.c_str(),
			lRet);
		assert(0);
		return VAccess::RET_ERROR;
	}
}

/*----------------------------------------------------------------------------*/
VAccess::ERet STest::selectUserParaNameExists(short& sCounter, CBString& strUserParaname)
{
	CBString   strSql;
	double		dCounter;

	strSql.format("SELECT count (*) "
		"FROM ppuserpara "
		"WHERE name = '%s'",
		strUserParaname.c_str());

	long lRet = SqlRead((char*)strSql.c_str(), &dCounter, NULL);
	sCounter = (short)dCounter;

	switch (lRet)
	{
	case SQL_SUCCESS:
		return VAccess::RET_OK;

	case SQL_NOTFOUND:
		return VAccess::RET_NOTFOUND;

	default:
		SLog::ex().write(__FUNCTION__, VAccess::RET_ERROR,
			"select (<%s>) = %ld",
			strSql.c_str(),
			lRet);
		assert(0);
		return VAccess::RET_ERROR;
	}
}
