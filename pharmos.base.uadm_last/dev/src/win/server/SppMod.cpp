/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH										       */
/**   Date   : 14.06.2004                                                     */
/**                                                                           */
/******************************************************************************/

#include "srv.h"

#include <libscmlogin/libscmlogin.h>

#include "tbl_ppappmodule.h"
#include "struct_appmodule.h"

#include "SppMod.h"

/*----------------------------------------------------------------------------*/
SppMod::SppMod() 
{
}

/*----------------------------------------------------------------------------*/
SppMod::~SppMod()
{
}

/*----------------------------------------------------------------------------*/
VAccess::ERet SppMod::insertMod(SListProj  & pSDataSingle )
{
	CBString            strSql;
	T_ListCondppAppMod   * pCond = (T_ListCondppAppMod *)&(pSDataSingle.stListCond);
	T_ListProjppAppMod   * pCond2 = (T_ListProjppAppMod *)&(pSDataSingle.stListProj);
	
	strSql.format("INSERT "
					"INTO ppappmodule "
					"(applicationid, moduleid, name, name_en, name_fr, name_hr ) "
					"VALUES (%d, %d, '%s', '%s', '%s', '%s')",
					pCond->APPLICATIONID, pCond2->MODULEID, pCond2->NAME,
					pCond2->NAME_EN, pCond2->NAME_FR, pCond2->NAME_HR );
	
		
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
VAccess::ERet SppMod::deleteMod(SListProj  & pSDataSingle )
{
	CBString            strSql;
	T_ListCondppAppMod   * pCond = (T_ListCondppAppMod *)&(pSDataSingle.stListCond);
	T_ListProjppAppMod   * pCond2 = (T_ListProjppAppMod *)&(pSDataSingle.stListProj);
	
	strSql.format("DELETE "
					"FROM ppappmodule "
					"WHERE "
					"applicationid = %d and moduleid = %d",
					pCond->APPLICATIONID, pCond2->MODULEID);

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
VAccess::ERet SppMod::deleteAppModRight	(SListProj  & pSDataSingle )
{
	CBString            strSql;
	T_ListCondppAppMod   * pCond = (T_ListCondppAppMod *)&(pSDataSingle.stListCond);
	T_ListProjppAppMod   * pCond2 = (T_ListProjppAppMod *)&(pSDataSingle.stListProj);
	
	strSql.format("DELETE "
					"FROM ppappmodright "
					"WHERE "
					"applicationid = %d and moduleid = %d",
					pCond->APPLICATIONID, pCond2->MODULEID);

		long lRet = SqlExecuteImm((char *)strSql.c_str());

	switch (lRet)
	{
	case SQL_SUCCESS:
		//log to see who deleted a module 
		SLog::ex().write(__FUNCTION__, VAccess::RET_OK, 
			": Mod %d, '%s' deleted by operator '%s'",  
			pCond2->MODULEID, pCond2->NAME,
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
VAccess::ERet SppMod::deleteppText(SListProj  & pSDataSingle )
{
	CBString            strSql;
	T_ListCondppAppMod   * pCond = (T_ListCondppAppMod *)&(pSDataSingle.stListCond);
	T_ListProjppAppMod   * pCond2 = (T_ListProjppAppMod *)&(pSDataSingle.stListProj);
	
	strSql.format("DELETE "
					"FROM pptext "
					"WHERE "
					"applicationid = %d and moduleid = %d",
					pCond->APPLICATIONID, pCond2->MODULEID);

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
VAccess::ERet SppMod::deleteAppRight(SListProj  & pSDataSingle )
{
	CBString            strSql;
	T_ListCondppAppMod   * pCond = (T_ListCondppAppMod *)&(pSDataSingle.stListCond);
	T_ListProjppAppMod   * pCond2 = (T_ListProjppAppMod *)&(pSDataSingle.stListProj);
	
	strSql.format("DELETE "
					"FROM ppappright "
					"WHERE "
					"applicationid = %d and moduleid = %d",
					pCond->APPLICATIONID, pCond2->MODULEID);

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
VAccess::ERet SppMod::updateMod(SListProj  & pSDataSingle )
{
	CBString            strSql;
	T_ListCondppAppMod * pCond = (T_ListCondppAppMod *)&(pSDataSingle.stListCond);
	T_ListProjppAppMod * pProj = (T_ListProjppAppMod *)&(pSDataSingle.stListProj);

	strSql.format("UPDATE "
						"ppappmodule "
					"SET "
						"name = '%s', name_en = '%s', name_fr = '%s', name_hr = '%s' "
					"WHERE "
						"applicationid = %d and moduleid = %d",
						pProj->NAME, pProj->NAME_EN, pProj->NAME_FR, pProj->NAME_HR,   
						pCond->APPLICATIONID, pProj->MODULEID);
	
		
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
VAccess::ERet SppMod::SelTestForUpd(SListProj  & pSDataSingle )
{
	CBString         strSql;
	T_ListCondppAppMod   * pCond = (T_ListCondppAppMod *)&(pSDataSingle.stListCond);
	T_ListProjppAppMod *   pProj = (T_ListProjppAppMod *)&(pSDataSingle.stListProj);
	
	strSql.format("SELECT moduleid, name, name_en, name_fr, name_hr "
					"FROM ppappmodule "
					"WHERE applicationid = %d and moduleid = %d"
					,pCond->APPLICATIONID, pProj->MODULEID);

	long lRet = SqlRead((char *)strSql.c_str(), pProj, NULL);

	switch (lRet)
	{
	case SQL_SUCCESS:
		DtStructStripLast(pProj, STRUCDESC_LISTPROJPPAPPMOD);
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
VAccess::ERet SppMod::openCursor(VData & pSDataCond)
{
	if (VAccVdooCurs::openCursor(pSDataCond) != VAccess::RET_OK)
		return VAccess::RET_OK;

	long        lRet;
	CBString   strSql;

	SListProj * pCond = (SListProj * )&pSDataCond;

	DtStructStripLast(&(pCond->stListCond), STRUCDESC_LISTCONDPPAPPMOD);
			
	strSql.format("SELECT "
						  "m.moduleid, "
						  "m.name, "
						  "name_en, "
						  "name_fr, "
						  "name_hr "
					"FROM ppappmodule m "
					"WHERE m.applicationid = %d "
				    "ORDER BY m.moduleid",
				   pCond->stListCond.APPLICATIONID);
	
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
VAccess::ERet SppMod::fetch(VData & pSDataProj)
{
	long         lRet;
	T_ListProjppAppMod * pProj = (T_ListProjppAppMod *)&(((SListProj *)&pSDataProj)->stListProj);

	lRet = SqlBufferedFetch(m_hCursor, 1, 100, pProj, NULL); 

	switch(lRet)
	{
	case SQL_SUCCESS:
		DtStructStripLast(pProj, STRUCDESC_LISTPROJPPAPPMOD);
		return VAccess::RET_OK;

	case SQL_NOTFOUND:
		return VAccess::RET_NOTFOUND;

	default:
		SLog::ex().write(__FUNCTION__, VAccess::RET_ERROR, 
			                "fetch module = %ld",
							lRet);
		assert(0);
		return VAccess::RET_ERROR;
	}
}

/*----------------------------------------------------------------------------*/
SppMod::SListProj::SListProj()
{
	clear();
}

/*----------------------------------------------------------------------------*/
bool SppMod::SListProj::operator == (const VData & r) const
{
	if (typeid(*this) != typeid(r))
        return false;

	return
	(
		(0 == memcmp(&stListCond  , 
		             &(((const SListProj &)r).stListCond  ), 
					 sizeof(T_ListCondppAppMod  )))
		&&
		(0 == memcmp(&stListProj, 
		             &(((const SListProj &)r).stListProj), 
					 sizeof(T_ListProjppAppMod)))
	) 
	? true : false;
}

/*----------------------------------------------------------------------------*/
void SppMod::SListProj::clear()
{
	memset(&stListCond, 0, sizeof(T_ListCondppAppMod));
	memset(&stListProj, 0, sizeof(T_ListProjppAppMod));
}

