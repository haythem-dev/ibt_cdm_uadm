/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH										      */
/**   Date   : 06.07.2004                                                     */
/**                                                                           */
/******************************************************************************/

#include "srv.h"

#include "tbl_pptext.h"
#include "struct_text.h"

#include "SppText.h"

/*----------------------------------------------------------------------------*/
SppText::SppText() 
{
}

/*----------------------------------------------------------------------------*/
SppText::~SppText()
{
}

/*----------------------------------------------------------------------------*/
VAccess::ERet SppText::insertText(SListProj  & pSDataSingle )
{
	CBString            strSql;
	T_ListProjppText   * pCond = (T_ListProjppText *)&(pSDataSingle.stListProj);
	T_ListCondppText   * pCond2 = (T_ListCondppText *)&(pSDataSingle.stListCond);
	
	strSql.format("INSERT "
					"INTO pptext "
					"(reftext, applicationid, moduleid, languageid, text) "
					"VALUES ('%s', %d, %d, '%s', '%s')", 
					pCond->REFTEXT, pCond2->APPLICATIONID, pCond2->MODULEID, pCond2->LANGUAGEID, pCond->TEXT);
	
		
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
VAccess::ERet SppText::updateText(SListProj  & pSDataSingle ) 
{
	CBString            strSql;
	T_ListProjppText   * pCond = (T_ListProjppText *)&(pSDataSingle.stListProj);
	T_ListCondppText   * pCond2 = (T_ListCondppText *)&(pSDataSingle.stListCond);
	
	
	strSql.format("UPDATE "
						"pptext "
					"SET "
						"text = '%s' "
					"WHERE "
						"languageid = '%s' "
						"and reftext = '%s' "
						"and applicationid = %d "
						"and moduleid = %d",
						pCond->TEXT, pCond2->LANGUAGEID, pCond->REFTEXT,
						pCond2->APPLICATIONID, pCond2->MODULEID);
	
		
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
VAccess::ERet SppText::deleteLangText(SListProj  & pSDataSingle )
{
	CBString            strSql;
	T_ListProjppText   * pCond = (T_ListProjppText *)&(pSDataSingle.stListProj);
	T_ListCondppText   * pCond2 = (T_ListCondppText *)&(pSDataSingle.stListCond);
	
	strSql.format("DELETE "
					"FROM pptext "
					"WHERE "
					"languageid = '%s' "
					"and applicationid = %d "
					"and moduleid = %d "
					"and reftext = '%s'",
					pCond2->LANGUAGEID, pCond2->APPLICATIONID, pCond2->MODULEID, pCond->REFTEXT);

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
VAccess::ERet SppText::deleteRefText(SListProj  & pSDataSingle ) 
{
	CBString            strSql;
	T_ListProjppText   * pCond = (T_ListProjppText *)&(pSDataSingle.stListProj);
	T_ListCondppText   * pCond2 = (T_ListCondppText *)&(pSDataSingle.stListCond);
	
	strSql.format("DELETE "
					"FROM pptext "
					"WHERE "
					"applicationid = %d "
					"and moduleid = %d "
					"and reftext = '%s'",
					pCond2->APPLICATIONID, pCond2->MODULEID, pCond->REFTEXT);

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
VAccess::ERet SppText::selectTextEN(SListProj  & pSDataSingle, CBString & strTextEN)
{
	CBString            strSql;
	T_ListProjppText   * pCond = (T_ListProjppText *)&(pSDataSingle.stListProj);
	T_ListCondppText   * pCond2 = (T_ListCondppText *)&(pSDataSingle.stListCond);
	
	char	cTXT		  [L_PPTEXT_TEXT + 1];

	strSql.format("SELECT "
					"text "
					"FROM pptext "
					"WHERE "
					"applicationid = %d "
					"and moduleid = %d "
					"and reftext = '%s' "
					"and languageid = 'en'",
					pCond2->APPLICATIONID, pCond2->MODULEID, pCond->REFTEXT);

		long lRet = SqlRead((char *)strSql.c_str(), cTXT, NULL);


	switch (lRet)
	{
	case SQL_SUCCESS:
		strTextEN = cTXT;
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
VAccess::ERet SppText::SelTestForUpd(SListProj  & pSDataSingle )
{
	CBString         strSql;
	T_ListCondppText   * pCond = (T_ListCondppText *)&(pSDataSingle.stListCond);
	T_ListProjppText *   pProj = (T_ListProjppText *)&(pSDataSingle.stListProj);

	
	strSql.format("SELECT reftext, text "
					"FROM pptext "
					"WHERE applicationid = %d and moduleid = %d and languageid = '%s' and reftext = '%s'"
					,pCond->APPLICATIONID, pCond->MODULEID, pCond->LANGUAGEID, pProj->REFTEXT);

	long lRet = SqlRead((char *)strSql.c_str(), pProj, NULL);


	switch (lRet)
	{
	case SQL_SUCCESS:
		DtStructStripLast(pProj, STRUCDESC_LISTPROJTEXT);
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
VAccess::ERet SppText::openCursor(VData & pSDataCond)
{	
	if (VAccVdooCurs::openCursor(pSDataCond) != VAccess::RET_OK)
		return VAccess::RET_OK;

	long        lRet;
	CBString   strSql;

	SListProj * pCond = (SListProj * )&pSDataCond;

	DtStructStripLast(&(pCond->stListCond), STRUCDESC_LISTCONDTEXT);

	if(pCond->stListCond.BCOR == true)
	{			
		strSql.format("SELECT "
						  "t.reftext, "
						  "t.text "
					"FROM pptext t "
					"WHERE "
						"t.applicationid = %d and t.moduleid = %d and t.languageid = '%s' "
				    "ORDER BY t.reftext",
					pCond->stListCond.APPLICATIONID, pCond->stListCond.MODULEID, pCond->stListCond.LANGUAGEID);
	}
	else if(pCond->stListCond.BCOR == false)
	{				
		strSql.format("SELECT "
						  "t.reftext, "
						  "t.text "
					"FROM pptext t "
					"WHERE "
						"t.applicationid = %d and t.moduleid = %d and t.languageid = 'en' "
						"and t.reftext NOT IN "
							"(SELECT t.reftext "
							"FROM pptext t "
							"WHERE t.applicationid = %d and t.moduleid = %d and t.languageid = '%s') "
						"ORDER BY t.reftext",
					pCond->stListCond.APPLICATIONID, pCond->stListCond.MODULEID,
					pCond->stListCond.APPLICATIONID, pCond->stListCond.MODULEID, 
					pCond->stListCond.LANGUAGEID);
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
VAccess::ERet SppText::fetch(VData & pSDataProj)
{
	long         lRet;
	T_ListProjppText * pProj = (T_ListProjppText *)&(((SListProj *)&pSDataProj)->stListProj);

	lRet = SqlBufferedFetch(m_hCursor, 1, 100, pProj, NULL); 

	switch(lRet)
	{
	case SQL_SUCCESS:
		DtStructStripLast(pProj, STRUCDESC_LISTPROJTEXT);
		return VAccess::RET_OK;

	case SQL_NOTFOUND:
		return VAccess::RET_NOTFOUND;

	default:
		SLog::ex().write(__FUNCTION__, VAccess::RET_ERROR, 
			        "fetch text= %ld",
					lRet);
		assert(0);
		return VAccess::RET_ERROR;
	}
}

/*----------------------------------------------------------------------------*/
SppText::SListProj::SListProj()
{
	clear();
}

/*----------------------------------------------------------------------------*/
bool SppText::SListProj::operator == (const VData & r) const
{
	if (typeid(*this) != typeid(r))
        return false;

	return
	(
		(0 == memcmp(&stListCond  , 
		             &(((const SListProj &)r).stListCond  ), 
					 sizeof(T_ListCondppText  )))
		&&
		(0 == memcmp(&stListProj, 
		             &(((const SListProj &)r).stListProj), 
					 sizeof(T_ListProjppText)))
	) 
	? true : false;
}

/*----------------------------------------------------------------------------*/
void SppText::SListProj::clear()
{
	memset(&stListCond, 0, sizeof(T_ListCondppText));
	memset(&stListProj, 0, sizeof(T_ListProjppText));
}

