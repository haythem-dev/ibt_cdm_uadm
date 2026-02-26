/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH										       */
/**   Date   : 01.06.2004                                                     */
/**                                                                           */
/******************************************************************************/

#include "srv.h"

#include <libscmlogin/libscmlogin.h>

#include "tbl_ppapplication.h"
#include "struct_application.h"

#include "SppApp.h"


// selectList	: SELECT a.applicationid, a.name FROM ppapplication a
//						ORDER BY a.name

/*----------------------------------------------------------------------------*/
SppApp::SppApp() 
{
}

/*----------------------------------------------------------------------------*/
SppApp::~SppApp()
{
}


/*----------------------------------------------------------------------------*/
VAccess::ERet SppApp::insertSingle(SListAppProj  & pSDataSingle )
{
	CBString            strSql;
	T_ListProjApp   * pCond = (T_ListProjApp *)&(pSDataSingle.stListAppProj);
	
	strSql.format("INSERT "
					"INTO ppapplication "
					"(name, description, name_en, name_fr, name_hr) "
					"VALUES ('%s', '%s', '%s', '%s', '%s')",
					pCond->NAME, pCond->DESCRIPTION, pCond->NAME_EN, pCond->NAME_FR, pCond->NAME_HR);
	
		
	long lRet = SqlExecuteImm((char *)strSql.c_str());

	switch (lRet)
	{
	case SQL_SUCCESS:
		SSqlStat::ex().setStat(strSql);
		return VAccess::RET_OK;

	case SQL_NOTFOUND:
		return VAccess::RET_NOTFOUND;

	default:
		SLog::ex().write(__FUNCTION__, VAccess::RET_ERROR, 
			                "insert (<%s>) = %ld",
							strSql.c_str(),
							lRet);
		assert(0);
		return VAccess::RET_ERROR;
	}
}

/*----------------------------------------------------------------------------*/
VAccess::ERet SppApp::updateSingle(SListAppProj  & pSDataSingle )
{
	CBString            strSql;
	T_ListProjApp   * pCond = (T_ListProjApp *)&(pSDataSingle.stListAppProj);

	strSql.format("UPDATE "
						"ppapplication "
					"SET "
						"name = '%s', description = '%s', name_en = '%s' , name_fr = '%s' , name_hr = '%s' "
					"WHERE "
						"applicationid = %d",
					pCond->NAME, pCond->DESCRIPTION, 
					pCond->NAME_EN, pCond->NAME_FR, pCond->NAME_HR,
					pCond->APPLICATIONID );
	
		
	long lRet = SqlExecuteImm((char *)strSql.c_str());

	switch (lRet)
	{
	case SQL_SUCCESS:
		SSqlStat::ex().setStat(strSql);
		return VAccess::RET_OK;

	case SQL_NOTFOUND:
		return VAccess::RET_NOTFOUND;

	default:
		SLog::ex().write(__FUNCTION__, VAccess::RET_ERROR, 
			                "update (<%s>) = %ld",
							strSql.c_str(),
							lRet);
		assert(0);
		return VAccess::RET_ERROR;
	}
}

/*----------------------------------------------------------------------------*/
VAccess::ERet SppApp::deleteSingle(SListAppProj  & pSDataSingle )
{
	CBString         strSql;
	T_ListProjApp   * pCond = (T_ListProjApp *)&(pSDataSingle.stListAppProj);
	
	strSql.format("DELETE "
					"FROM ppapplication "
					"WHERE applicationid = %d",
					pCond->APPLICATIONID);

		long lRet = SqlExecuteImm((char *)strSql.c_str());

	switch (lRet)
	{
	case SQL_SUCCESS:
		//log to see who deleted an application
		SLog::ex().write(__FUNCTION__, VAccess::RET_OK, 
			": App %d, '%s' deleted by operator '%s'",  
			pCond->APPLICATIONID, pCond->NAME,
			scmod::ppBLoginData::ex().GetUsername().c_str());
		SSqlStat::ex().setStat(strSql);
		return VAccess::RET_OK;

	case SQL_NOTFOUND:
		return VAccess::RET_NOTFOUND;

	default:
		SLog::ex().write(__FUNCTION__, VAccess::RET_ERROR, 
			                "delete (<%s>) = %ld",
							strSql.c_str(),
							lRet);
		assert(0);
		return VAccess::RET_ERROR;
	}
}

/*----------------------------------------------------------------------------*/
VAccess::ERet SppApp::deletePrevious	(SListAppProj  & pSDataSingle )
{
	CBString         strSql;
	T_ListProjApp   * pCond = (T_ListProjApp *)&(pSDataSingle.stListAppProj);
	
	strSql.format("DELETE "
					"FROM pploginprevious "
					"WHERE applicationid = %d",
					pCond->APPLICATIONID);

		long lRet = SqlExecuteImm((char *)strSql.c_str());

	switch (lRet)
	{
	case SQL_SUCCESS:
		SSqlStat::ex().setStat(strSql);
		return VAccess::RET_OK;

	case SQL_NOTFOUND:
		return VAccess::RET_NOTFOUND;

	default:
		SLog::ex().write(__FUNCTION__, VAccess::RET_ERROR, 
			                "delete (<%s>) = %ld",
							strSql.c_str(),
							lRet);
		assert(0);
		return VAccess::RET_ERROR;
	}
}

/*----------------------------------------------------------------------------*/
VAccess::ERet SppApp::deleteCorAppUserGroup	(SListAppProj  & pSDataSingle )
{
	CBString         strSql;
	T_ListProjApp   * pCond = (T_ListProjApp *)&(pSDataSingle.stListAppProj);
	
	strSql.format("DELETE "
					"FROM ppcorappusergroup "
					"WHERE applicationid = %d",
					pCond->APPLICATIONID);

		long lRet = SqlExecuteImm((char *)strSql.c_str());

	switch (lRet)
	{
	case SQL_SUCCESS:
		SSqlStat::ex().setStat(strSql);
		return VAccess::RET_OK;

	case SQL_NOTFOUND:
		return VAccess::RET_NOTFOUND;

	default:
		SLog::ex().write(__FUNCTION__, VAccess::RET_ERROR, 
			                "delete (<%s>) = %ld",
							strSql.c_str(),
							lRet);
		assert(0);
		return VAccess::RET_ERROR;
	}
}

/*----------------------------------------------------------------------------*/
VAccess::ERet SppApp::deleteCorAppLang(SListAppProj  & pSDataSingle )
{
	CBString         strSql;
	T_ListProjApp   * pCond = (T_ListProjApp *)&(pSDataSingle.stListAppProj);
	
	strSql.format("DELETE "
					"FROM ppcorapplang "
					"WHERE applicationid = %d",
					pCond->APPLICATIONID);

		long lRet = SqlExecuteImm((char *)strSql.c_str());

	switch (lRet)
	{
	case SQL_SUCCESS:
		SSqlStat::ex().setStat(strSql);
		return VAccess::RET_OK;

	case SQL_NOTFOUND:
		return VAccess::RET_NOTFOUND;

	default:
		SLog::ex().write(__FUNCTION__, VAccess::RET_ERROR, 
			                "delete (<%s>) = %ld",
							strSql.c_str(),
							lRet);
		assert(0);
		return VAccess::RET_ERROR;
	}
}

/*----------------------------------------------------------------------------*/
VAccess::ERet SppApp::deleteppText(SListAppProj  & pSDataSingle )
{
	CBString         strSql;
	T_ListProjApp   * pCond = (T_ListProjApp *)&(pSDataSingle.stListAppProj);
	
	strSql.format("DELETE "
					"FROM pptext "
					"WHERE applicationid = %d",
					pCond->APPLICATIONID);

		long lRet = SqlExecuteImm((char *)strSql.c_str());

	switch (lRet)
	{
	case SQL_SUCCESS:
		SSqlStat::ex().setStat(strSql);
		return VAccess::RET_OK;

	case SQL_NOTFOUND:
		return VAccess::RET_NOTFOUND;

	default:
		SLog::ex().write(__FUNCTION__, VAccess::RET_ERROR, 
			                "delete (<%s>) = %ld",
							strSql.c_str(),
							lRet);
		assert(0);
		return VAccess::RET_ERROR;
	}
}

/*----------------------------------------------------------------------------*/
VAccess::ERet SppApp::deleteAppModule(SListAppProj  & pSDataSingle )
{
	CBString         strSql;
	T_ListProjApp   * pCond = (T_ListProjApp *)&(pSDataSingle.stListAppProj);
	
	strSql.format("DELETE "
					"FROM ppappmodule "
					"WHERE applicationid = %d",
					pCond->APPLICATIONID);

		long lRet = SqlExecuteImm((char *)strSql.c_str());

	switch (lRet)
	{
	case SQL_SUCCESS:
		SSqlStat::ex().setStat(strSql);
		return VAccess::RET_OK;

	case SQL_NOTFOUND:
		return VAccess::RET_NOTFOUND;

	default:
		SLog::ex().write(__FUNCTION__, VAccess::RET_ERROR, 
			                "delete (<%s>) = %ld",
							strSql.c_str(),
							lRet);
		assert(0);
		return VAccess::RET_ERROR;
	}
}

/*----------------------------------------------------------------------------*/
VAccess::ERet SppApp::deleteCorAppBranchReg	(SListAppProj  & pSDataSingle )
{
	CBString         strSql;
	T_ListProjApp   * pCond = (T_ListProjApp *)&(pSDataSingle.stListAppProj);
	
	strSql.format("DELETE "
					"FROM ppcorappbranchreg "
					"WHERE applicationid = %d",
					pCond->APPLICATIONID);

		long lRet = SqlExecuteImm((char *)strSql.c_str());

	switch (lRet)
	{
	case SQL_SUCCESS:
		SSqlStat::ex().setStat(strSql);
		return VAccess::RET_OK;

	case SQL_NOTFOUND:
		return VAccess::RET_NOTFOUND;

	default:
		SLog::ex().write(__FUNCTION__, VAccess::RET_ERROR, 
			                "delete (<%s>) = %ld",
							strSql.c_str(),
							lRet);
		assert(0);
		return VAccess::RET_ERROR;
	}
}

/*----------------------------------------------------------------------------*/
VAccess::ERet SppApp::deleteAppModRight(SListAppProj  & pSDataSingle )
{
	CBString         strSql;
	T_ListProjApp   * pCond = (T_ListProjApp *)&(pSDataSingle.stListAppProj);
	
	strSql.format("DELETE "
					"FROM ppappmodright "
					"WHERE applicationid = %d",
					pCond->APPLICATIONID);

		long lRet = SqlExecuteImm((char *)strSql.c_str());

	switch (lRet)
	{
	case SQL_SUCCESS:
		SSqlStat::ex().setStat(strSql);
		return VAccess::RET_OK;

	case SQL_NOTFOUND:
		return VAccess::RET_NOTFOUND;

	default:
		SLog::ex().write(__FUNCTION__, VAccess::RET_ERROR, 
			                "delete (<%s>) = %ld",
							strSql.c_str(),
							lRet);
		assert(0);
		return VAccess::RET_ERROR;
	}
}

/*----------------------------------------------------------------------------*/
VAccess::ERet SppApp::deleteAppRight(SListAppProj  & pSDataSingle )
{
	CBString         strSql;
	T_ListProjApp   * pCond = (T_ListProjApp *)&(pSDataSingle.stListAppProj);
	
	strSql.format("DELETE "
					"FROM ppappright "
					"WHERE applicationid = %d",
					pCond->APPLICATIONID);

		long lRet = SqlExecuteImm((char *)strSql.c_str());

	switch (lRet)
	{
	case SQL_SUCCESS:
		SSqlStat::ex().setStat(strSql);
		return VAccess::RET_OK;

	case SQL_NOTFOUND:
		return VAccess::RET_NOTFOUND;

	default:
		SLog::ex().write(__FUNCTION__, VAccess::RET_ERROR, 
			                "delete (<%s>) = %ld",
							strSql.c_str(),
							lRet);
		assert(0);
		return VAccess::RET_ERROR;
	}
}

/*----------------------------------------------------------------------------*/
VAccess::ERet SppApp::selectID(CBString &strAppname, long & lAppid)
{
	CBString            strSql;

	strSql.format("SELECT "
						"a.applicationid "
					"FROM ppapplication a "
                   "WHERE a.name = '%s'",
				   strAppname.c_str());

    long lRet = SqlRead((char *)strSql.c_str(), &lAppid, NULL);

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
VAccess::ERet SppApp::SelTestForUpd(SListAppProj  & pSDataSingle )
{
	CBString         strSql;
	T_ListProjApp   * pCond = (T_ListProjApp *)&(pSDataSingle.stListAppProj);
	T_ListProjApp *   pProj = (T_ListProjApp *)&(pSDataSingle.stListAppProj);

	
	strSql.format("SELECT applicationid, name, description, name_en, name_fr, name_hr "
					"FROM ppapplication "
					"WHERE applicationid = %d"
					,pCond->APPLICATIONID);

	long lRet = SqlRead((char *)strSql.c_str(), pProj, NULL);

	switch (lRet)
	{
	case SQL_SUCCESS:
		DtStructStripLast(pProj, STRUCDESC_LISTPROJAPP);
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
VAccess::ERet SppApp::openCursor(VData & pSDataCond)
{
	if (VAccVdooCurs::openCursor(pSDataCond) != VAccess::RET_OK)
		return VAccess::RET_OK;

	long        lRet;
	CBString   strSql;
	
	strSql.format("SELECT "
						  "a.applicationid," 
						  "a.name,"
						  "a.description, "
						  "name_en, "
						  "name_fr, "
						  "name_hr "
                   "FROM ppapplication a "
				   "ORDER BY a.name");


	if ((lRet = SqlDeclareCsWh(&m_hCursor, (char *)strSql.c_str(), NULL)) != SQL_SUCCESS)
		return VAccess::RET_ERROR;

	lRet = SqlOpenDeclCs(m_hCursor, NULL);

	switch(lRet)
	{
	case SQL_SUCCESS:
		return VAccess::RET_OK;

	case SQL_NOTFOUND:
		return VAccess::RET_NOTFOUND;

	default:
		SLog::ex().write(__FUNCTION__, VAccess::RET_ERROR, 
			                "open cursor (<%s>) = %ld",
							strSql.c_str(),
							lRet);
		assert(0);
		return VAccess::RET_ERROR;
	}
}

/*----------------------------------------------------------------------------*/
VAccess::ERet SppApp::fetch(VData & pSDataProj)
{
	long         lRet;
	T_ListProjApp * pProj = (T_ListProjApp *)&(((SListAppProj *)&pSDataProj)->stListAppProj);

	lRet = SqlBufferedFetch(m_hCursor, 1, 100, pProj, NULL); 

	switch(lRet)
	{
	case SQL_SUCCESS:
		DtStructStripLast(pProj, STRUCDESC_LISTPROJAPP);
		return VAccess::RET_OK;

	case SQL_NOTFOUND:
		return VAccess::RET_NOTFOUND;

	default:
		SLog::ex().write(__FUNCTION__, VAccess::RET_ERROR, 
			                "fetch application = %ld",
							lRet);
		assert(0);
		return VAccess::RET_ERROR;
	}
}

/*----------------------------------------------------------------------------*/
SppApp::SListAppProj::SListAppProj()
{
	clear();
}

/*----------------------------------------------------------------------------*/
bool SppApp::SListAppProj::operator == (const VData & r) const
{
	if (typeid(*this) != typeid(r))
        return false;

	return (memcmp(&stListAppProj, 
		           &(((const SListAppProj &)r).stListAppProj), 
				   sizeof(T_ListProjApp)) == 0)
		   ? true : false;
}

/*----------------------------------------------------------------------------*/
void SppApp::SListAppProj::clear()
{
	memset(&stListAppProj, 0, sizeof(T_ListProjApp));
}
