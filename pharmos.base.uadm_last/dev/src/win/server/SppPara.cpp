/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH										       */
/**   Date   : 03.08.2004                                                     */
/**                                                                           */
/******************************************************************************/

#include "srv.h"

#include "tbl_pplogpara.h"
#include "struct_para.h"

#include "SppPara.h"

/*----------------------------------------------------------------------------*/
SppPara::SppPara() 
{
}

/*----------------------------------------------------------------------------*/
SppPara::~SppPara()
{
}

/*----------------------------------------------------------------------------*/
VAccess::ERet SppPara::insertPara(SListProj  & pSDataSingle )
{
	CBString            strSql;
	T_ListCondppPara   * pCond = (T_ListCondppPara *)&(pSDataSingle.stListCond);
	T_ListProjppPara   * pCond2 = (T_ListProjppPara *)&(pSDataSingle.stListProj);
	

	strSql.format("INSERT "
					"INTO pplogpara "
					"(applicationid, para1, para2, parabez, paravalue) "
					"VALUES (%d, '%s', '%s', '%s', '%s')",
					pCond->APPLICATIONID, pCond2->PARA1, pCond2->PARA2, pCond2->PARABEZ, pCond2->PARAVALUE);
	
		
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
VAccess::ERet SppPara::SelTestForUpd(SListProj  & pSDataSingle )
{
	CBString         strSql;
	T_ListCondppPara   * pCond = (T_ListCondppPara *)&(pSDataSingle.stListCond);
	T_ListProjppPara *   pProj = (T_ListProjppPara *)&(pSDataSingle.stListProj);

	
	strSql.format("SELECT para1, para2, parabez, paravalue "
					"FROM pplogpara "
					"WHERE applicationid = %d and para1 = '%s' and para2 = '%s' and parabez = '%s'"
					,pCond->APPLICATIONID, pProj->PARA1, pProj->PARA2, pProj->PARABEZ);

	long lRet = SqlRead((char *)strSql.c_str(), pProj, NULL);

	switch (lRet)
	{
	case SQL_SUCCESS:
		DtStructStripLast(pProj, STRUCDESC_LISTPROJPPPARA);
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
VAccess::ERet SppPara::deletePara(SListProj  & pSDataSingle )
{
	CBString            strSql;
	T_ListCondppPara   * pCond = (T_ListCondppPara *)&(pSDataSingle.stListCond);
	T_ListProjppPara   * pCond2 = (T_ListProjppPara *)&(pSDataSingle.stListProj);
	
	strSql.format("DELETE "
					"FROM pplogpara "
					"WHERE "
					"applicationid = %d and para1 = '%s' and para2 = '%s' and parabez = '%s'",
					pCond->APPLICATIONID, pCond2->PARA1, pCond2->PARA2, pCond2->PARABEZ);

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
VAccess::ERet SppPara::updatePara(SListProj  & pSDataSingle )
{
	CBString            strSql;
	T_ListCondppPara * pCond = (T_ListCondppPara *)&(pSDataSingle.stListCond);
	T_ListProjppPara * pProj = (T_ListProjppPara *)&(pSDataSingle.stListProj);


	strSql.format("UPDATE "
						"pplogpara "
					"SET "
						"paravalue = '%s' "
					"WHERE "
						"applicationid = %d and para1 = '%s' and para2 = '%s' and parabez = '%s'",
						pProj->PARAVALUE, pCond->APPLICATIONID, pProj->PARA1, pProj->PARA2, pProj->PARABEZ);

	
		
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
VAccess::ERet SppPara::openCursor(VData & pSDataCond)
{
	if (VAccVdooCurs::openCursor(pSDataCond) != VAccess::RET_OK)
		return VAccess::RET_OK;

	long        lRet;
	CBString   strSql;

	SListProj * pCond = (SListProj * )&pSDataCond;

	DtStructStripLast(&(pCond->stListCond), STRUCDESC_LISTCONDPPPARA);
			
	strSql.format("SELECT "
						  "lp.para1, "
						  "lp.para2, "
						  "lp.parabez, "
						  "lp.paravalue "
					"FROM pplogpara lp "
					"WHERE lp.applicationid = %d "
				    "ORDER BY lp.parabez, lp.para1, lp.para2",
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
VAccess::ERet SppPara::fetch(VData & pSDataProj)
{
	long         lRet;
	T_ListProjppPara * pProj = (T_ListProjppPara *)&(((SListProj *)&pSDataProj)->stListProj);

	lRet = SqlBufferedFetch(m_hCursor, 1, 100, pProj, NULL); 

	switch(lRet)
	{
	case SQL_SUCCESS:
		DtStructStripLast(pProj, STRUCDESC_LISTPROJPPPARA);
		return VAccess::RET_OK;

	case SQL_NOTFOUND:
		return VAccess::RET_NOTFOUND;

	default:
		SLog::ex().write(__FUNCTION__, VAccess::RET_ERROR, 
			        "fetch parameter = %ld",
					lRet);
		assert(0);
		return VAccess::RET_ERROR;
	}
}

/*----------------------------------------------------------------------------*/
SppPara::SListProj::SListProj()
{
	clear();
}

/*----------------------------------------------------------------------------*/
bool SppPara::SListProj::operator == (const VData & r) const
{
	if (typeid(*this) != typeid(r))
        return false;

	return
	(
		(0 == memcmp(&stListCond  , 
		             &(((const SListProj &)r).stListCond  ), 
					 sizeof(T_ListCondppPara  )))
		&&
		(0 == memcmp(&stListProj, 
		             &(((const SListProj &)r).stListProj), 
					 sizeof(T_ListProjppPara)))
	) 
	? true : false;
}

/*----------------------------------------------------------------------------*/
void SppPara::SListProj::clear()
{
	memset(&stListCond, 0, sizeof(T_ListCondppPara));
	memset(&stListProj, 0, sizeof(T_ListProjppPara));
}

