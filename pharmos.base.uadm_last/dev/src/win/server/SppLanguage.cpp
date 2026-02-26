/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH										      */
/**   Date   : 18.06.2004                                                     */
/**                                                                           */
/******************************************************************************/

#include "srv.h"

#include <libscmlogin/libscmlogin.h>

#include "tbl_pploginlanguage.h"
#include "struct_language.h"
#include "defines.h"

#include "SppLanguage.h"

/*----------------------------------------------------------------------------*/
SppLanguage::SppLanguage() 
{
}

/*----------------------------------------------------------------------------*/
SppLanguage::~SppLanguage()
{
}

/*----------------------------------------------------------------------------*/
VAccess::ERet SppLanguage::insertCorLang(SListProj  & pSDataSingle )
{
	CBString            strSql;
	T_ListCondLanguage   * pCond = (T_ListCondLanguage *)&(pSDataSingle.stListCond);
	T_ListProjLanguage   * pCond2 = (T_ListProjLanguage *)&(pSDataSingle.stListProj);
	

	strSql.format("INSERT "
					"INTO ppcorapplang "
					"(languageid, applicationid) "
					"VALUES ('%s', %d)",
					pCond2->LANGUAGEID, pCond->APPLICATIONID);
	
		
	long lRet = SqlExecuteImm((char *)strSql.c_str());

	switch (lRet)
	{
	case SQL_SUCCESS:
		SSqlStat::ex().setStat(strSql);
		return VAccess::RET_OK;

	case SQL_NOTFOUND:
		return VAccess::RET_NOTFOUND;

	case SQL_UNIQUE_EXISTS:
	case SQL_UNIQUE_CONSTRAINT_EXISTS:
		return VAccess::RET_WARNING;

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
VAccess::ERet SppLanguage::deleteCorLang(SListProj  & pSDataSingle )
{
	CBString            strSql;
	T_ListCondLanguage   * pCond = (T_ListCondLanguage *)&(pSDataSingle.stListCond);
	T_ListProjLanguage   * pCond2 = (T_ListProjLanguage *)&(pSDataSingle.stListProj);
	
	strSql.format("DELETE "
					"FROM ppcorapplang "
					"WHERE "
					"languageid = '%s' and applicationid = %d",
					pCond2->LANGUAGEID, pCond->APPLICATIONID);

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
VAccess::ERet SppLanguage::deleteCorppText	(SListProj  & pSDataSingle )
{
	CBString            strSql;
	T_ListProjLanguage   * pCond2 = (T_ListProjLanguage *)&(pSDataSingle.stListProj);
	T_ListCondLanguage   * pCond = (T_ListCondLanguage *)&(pSDataSingle.stListCond);
	
	strSql.format("DELETE "
					"FROM pptext "
					"WHERE "
					"languageid = '%s' and applicationid = %d",
					pCond2->LANGUAGEID, pCond->APPLICATIONID);

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
VAccess::ERet SppLanguage::insertSingle(SListProj  & pSDataSingle )
{
	CBString            strSql;
	T_ListProjLanguage   * pCond = (T_ListProjLanguage *)&(pSDataSingle.stListProj);
	
	strSql.format("INSERT "
					"INTO pploginlanguage "
					"(languageid, name) "
					"VALUES ('%s','%s')",
					pCond->LANGUAGEID, pCond->NAME);
	
		
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
VAccess::ERet SppLanguage::SelTestForUpd(SListProj  & pSDataSingle )
{
	CBString         strSql;
	T_ListProjLanguage *   pProj = (T_ListProjLanguage *)&(pSDataSingle.stListProj);

	
	strSql.format("SELECT languageid, name "
					"FROM pploginlanguage "
					"WHERE languageid = '%s'"
					,pProj->LANGUAGEID);

	long lRet = SqlRead((char *)strSql.c_str(), pProj, NULL);

	switch (lRet)
	{
	case SQL_SUCCESS:
		DtStructStripLast(pProj, STRUCDESC_LISTPROJLANGUGAE);
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
VAccess::ERet SppLanguage::updateSingle(SListProj  & pSDataSingle )
{
	CBString            strSql;
	T_ListProjLanguage   * pCond = (T_ListProjLanguage *)&(pSDataSingle.stListProj);
	
	strSql.format("UPDATE "
						"pploginlanguage "
					"SET "
						"name = '%s' "
					"WHERE "
						"languageid = '%s'",
					pCond->NAME, pCond->LANGUAGEID);
	
		
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
VAccess::ERet SppLanguage::deleteSingle(SListProj  & pSDataSingle )
{
	CBString            strSql;
	T_ListProjLanguage   * pCond = (T_ListProjLanguage *)&(pSDataSingle.stListProj);
	
	strSql.format("DELETE "
					"FROM pploginlanguage "
					"WHERE "
					"languageid = '%s'",
					pCond->LANGUAGEID);

		long lRet = SqlExecuteImm((char *)strSql.c_str());

	switch (lRet)
	{
	case SQL_SUCCESS:
		//log to see who deleted a language
		SLog::ex().write(__FUNCTION__, VAccess::RET_OK, 
			": language '%s' deleted by operator '%s'",  
			pCond->LANGUAGEID,
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
VAccess::ERet SppLanguage::deletePrevious(SListProj  & pSDataSingle )
{
	CBString            strSql;
	T_ListProjLanguage   * pCond = (T_ListProjLanguage *)&(pSDataSingle.stListProj);
	
	strSql.format("DELETE "
					"FROM pploginprevious "
					"WHERE "
					"languageid = '%s'",
					pCond->LANGUAGEID);

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
VAccess::ERet SppLanguage::deleteAppCorLang(SListProj  & pSDataSingle )
{
	CBString            strSql;
	T_ListProjLanguage   * pCond = (T_ListProjLanguage *)&(pSDataSingle.stListProj);
	
	strSql.format("DELETE "
					"FROM ppcorapplang "
					"WHERE "
					"languageid = '%s'",
					pCond->LANGUAGEID);

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
VAccess::ERet SppLanguage::deleteppText(SListProj  & pSDataSingle )
{
	CBString            strSql;
	T_ListProjLanguage   * pCond = (T_ListProjLanguage *)&(pSDataSingle.stListProj);
	
	strSql.format("DELETE "
					"FROM pptext "
					"WHERE "
					"languageid = '%s'",
					pCond->LANGUAGEID);

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
VAccess::ERet SppLanguage::openCursor(VData & pSDataCond)
{	
	if (VAccVdooCurs::openCursor(pSDataCond) != VAccess::RET_OK)
		return VAccess::RET_OK;

	long        lRet;
	CBString   strSql;

	SListProj * pCond = (SListProj * )&pSDataCond;

	DtStructStripLast(&(pCond->stListCond), STRUCDESC_LISTCONDLANGUAGE);

	if(pCond->stListCond.ALLLANG == true && pCond->stListCond.LANGLIST == false)
	{			
		strSql.format("SELECT DISTINCT "
						  "l.languageid, "
						  "l.name "
					"FROM pploginlanguage l "
					"WHERE l.languageid NOT IN "
						"(SELECT "
						"a.languageid "
						"FROM ppcorapplang a "
						"WHERE a.applicationid = %d) "
				    "ORDER BY l.languageid",
					pCond->stListCond.APPLICATIONID);
	}
	else if(pCond->stListCond.ALLLANG == false && pCond->stListCond.LANGLIST == false)
	{ // alle zugeordneten Sprachen laden!
		strSql.format("SELECT DISTINCT "
						  "l.languageid, "
						  "l.name "
					"FROM pploginlanguage l, ppcorapplang a "
					"WHERE a.applicationid = %d and a.languageid = l.languageid "
					"ORDER BY l.languageid",
					pCond->stListCond.APPLICATIONID);
	}
	else if(pCond->stListCond.ALLLANG == true && pCond->stListCond.LANGLIST == true)
	{ // alle zugeordneten Sprachen laden!
		strSql.format("SELECT DISTINCT "
						  "l.languageid, "
						  "l.name "
					"FROM pploginlanguage l "
					"ORDER BY l.languageid");
	}
	
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
VAccess::ERet SppLanguage::fetch(VData & pSDataProj)
{
	long         lRet;
	T_ListProjLanguage * pProj = (T_ListProjLanguage *)&(((SListProj *)&pSDataProj)->stListProj);

	lRet = SqlBufferedFetch(m_hCursor, 1, 100, pProj, NULL); 

	switch(lRet)
	{
	case SQL_SUCCESS:
		DtStructStripLast(pProj, STRUCDESC_LISTPROJLANGUGAE);
		return VAccess::RET_OK;

	case SQL_NOTFOUND:
		return VAccess::RET_NOTFOUND;

	default:
		SLog::ex().write(__FUNCTION__, VAccess::RET_ERROR, 
			        "fetch language = %ld",
					lRet);
		assert(0);
		return VAccess::RET_ERROR;
	}
}

/*----------------------------------------------------------------------------*/
SppLanguage::SListProj::SListProj()
{
	clear();
}

/*----------------------------------------------------------------------------*/
bool SppLanguage::SListProj::operator == (const VData & r) const
{
	if (typeid(*this) != typeid(r))
        return false;

	return
	(
		(0 == memcmp(&stListCond  , 
		             &(((const SListProj &)r).stListCond  ), 
					 sizeof(T_ListCondLanguage  )))
		&&
		(0 == memcmp(&stListProj, 
		             &(((const SListProj &)r).stListProj), 
					 sizeof(T_ListProjLanguage)))
	) 
	? true : false;
}

/*----------------------------------------------------------------------------*/
void SppLanguage::SListProj::clear()
{
	memset(&stListCond, 0, sizeof(T_ListCondLanguage));
	memset(&stListProj, 0, sizeof(T_ListProjLanguage));
}

