/******************************************************************************/
/**                                                                           */
/**   Author : T.ARNOLD NTTDATA                                               */
/**   Date   : 25.11.2025                                                     */
/**                                                                           */
/******************************************************************************/

#include "srv.h"

#include <libscmlogin/libscmlogin.h>

#include "tbl_ppuserpara.h"
#include "struct_userpara.h"
#include "defines.h"

#include "SppUserPara.h"

/*----------------------------------------------------------------------------*/
SppUserPara::SppUserPara() 
{
}

/*----------------------------------------------------------------------------*/
SppUserPara::~SppUserPara()
{
}

/*----------------------------------------------------------------------------*/
VAccess::ERet SppUserPara::insertSingle(SListProj  & pSDataSingle )
{
	CBString            strSql;
	T_ListProjppUserPara* pCond = (T_ListProjppUserPara*)&(pSDataSingle.stListProj);
	
	strSql.format("SELECT CAST(NVL(MAX(userparaid),0)+1 AS SMALLINT) FROM ppuserpara");
	long lRet = SqlRead((char*)strSql.c_str(), &pCond->USERPARAID, NULL);
	if (lRet == SQL_SUCCESS)
	{
		strSql.format("INSERT "
						"INTO ppuserpara "
						"(userparaid, name, description) "
						"VALUES (%d, '%s', '%s')",
						pCond->USERPARAID, pCond->NAME, pCond->DESCRIPTION);
	
		
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
	return VAccess::RET_ERROR;
}

/*----------------------------------------------------------------------------*/
VAccess::ERet SppUserPara::updateSingle(SListProj  & pSDataSingle )
{
	CBString            strSql;
	T_ListProjppUserPara* pCond = (T_ListProjppUserPara*)&(pSDataSingle.stListProj);
	
	strSql.format("UPDATE "
						"ppuserpara "
					"SET "
						"name = '%s', "
		                "description = '%s' "
					"WHERE "
						"userparaid = %d",
					pCond->NAME, pCond->DESCRIPTION, pCond->USERPARAID);
	
		
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
VAccess::ERet SppUserPara::deleteSingle	(SListProj  & pSDataSingle )
{
	CBString            strSql;
	T_ListProjppUserPara* pCond = (T_ListProjppUserPara*)&(pSDataSingle.stListProj);

	//TODO: check if userpara is used in ppcorappbruser table

	strSql.format("DELETE "
					"FROM ppuserpara "
					"WHERE "
					"userparaid = %d",
					pCond->USERPARAID);

		long lRet = SqlExecuteImm((char *)strSql.c_str());

	switch (lRet)
	{
	case SQL_SUCCESS:
		//log to see who deleted a user parameter
		SLog::ex().write(__FUNCTION__, VAccess::RET_OK, 
			": userpara %d, '%s' deleted by operator '%s'",  
			pCond->USERPARAID, pCond->NAME,
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
VAccess::ERet SppUserPara::selectID	(CBString &strUserParaname, short& sUserParaid)
{
	CBString            strSql;

	strSql.format("SELECT userparaid FROM ppuserpara WHERE name = '%s'", strUserParaname.c_str());

    long lRet = SqlRead((char *)strSql.c_str(), &sUserParaid, NULL);

	switch (lRet)
	{
	case SQL_SUCCESS:
		return VAccess::RET_OK;

	case SQL_NOTFOUND:
		return VAccess::RET_NOTFOUND;

	default:
		SLog::ex().write(__FUNCTION__, VAccess::RET_ERROR, "select (<%s>) = %ld", strSql.c_str(), lRet);
		assert(0);
		return VAccess::RET_ERROR;
	}
}
	
/*----------------------------------------------------------------------------*/
VAccess::ERet SppUserPara::SelTestForUpd(SListProj  & pSDataSingle )
{
	CBString         strSql;
	T_ListProjppUserPara* pProj = (T_ListProjppUserPara*)&(pSDataSingle.stListProj);
	
	
	strSql.format("SELECT userparaid, name, description, 0 FROM ppuserpara WHERE userparaid = %d", pProj->USERPARAID);

	long lRet = SqlRead((char *)strSql.c_str(), pProj, NULL);

	switch (lRet)
	{
	case SQL_SUCCESS:
		DtStructStripLast(pProj, STRUCDESC_LISTPROJPPUSERPARA);
		return VAccess::RET_OK;

	case SQL_NOTFOUND:
		return VAccess::RET_NOTFOUND;

	default:
		SLog::ex().write(__FUNCTION__, VAccess::RET_ERROR, "select (<%s>) = %ld", strSql.c_str(), lRet);
		assert(0);
		return VAccess::RET_ERROR;
	}
}

/*----------------------------------------------------------------------------*/
VAccess::ERet SppUserPara::openCursor(VData & pSDataCond)
{
	if (VAccVdooCurs::openCursor(pSDataCond) != VAccess::RET_OK)
		return VAccess::RET_OK;

	long        lRet;
	CBString   strSql;

	SListProj * pCond = (SListProj * )&pSDataCond;

	DtStructStripLast(&(pCond->stListCond), STRUCDESC_LISTCONDPPUSERPARA);
			
	if (pCond->stListCond.ALLUSERPARAS == false)
	{
		//TODO: scan for userparas
	}
	else if (pCond->stListCond.ALLUSERPARAS == true)
	{
		CBString strOrder;
		if (pCond->stListCond.SORTINDEX == 0) //id
		{
			strOrder = "ORDER BY userparaid ";
		}
		else if (pCond->stListCond.SORTINDEX == 2) //description
		{
				strOrder = "ORDER BY description ";
		}
		else if (pCond->stListCond.SORTINDEX == 3) //usercount
		{
			strOrder = "ORDER BY cnt ";
		}
		else //name
		{
			strOrder = "ORDER BY name ";
		}
		if (pCond->stListCond.SORTORDER == true) //desc
		{
			strOrder += "DESC";
		}
		else
		{
			strOrder += "ASC";
		}
		if (strlen(pCond->stListCond.USERPARANAME) > 0)
		{
			strSql.format("SELECT userparaid, name, description, 0 as cnt FROM ppuserpara WHERE name LIKE '%%%s%%' %s", pCond->stListCond.USERPARANAME, strOrder.c_str());
		}
		else
		{
			strSql.format("SELECT userparaid, name, description, 0 as cnt FROM ppuserpara %s", strOrder.c_str());
		}
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
		SLog::ex().write(__FUNCTION__, VAccess::RET_ERROR, "open cursor (<%s>) = %ld", strSql.c_str(), lRet);
		assert(0);
		return VAccess::RET_ERROR;
	}
}

/*----------------------------------------------------------------------------*/
VAccess::ERet SppUserPara::fetch(VData & pSDataProj)
{
	long         lRet;
	T_ListProjppUserPara * pProj = (T_ListProjppUserPara *)&(((SListProj *)&pSDataProj)->stListProj);

	lRet = SqlBufferedFetch(m_hCursor, 1, 100, pProj, NULL); 

	switch(lRet)
	{
	case SQL_SUCCESS:
		DtStructStripLast(pProj, STRUCDESC_LISTPROJPPUSERPARA);
		return VAccess::RET_OK;

	case SQL_NOTFOUND:
		return VAccess::RET_NOTFOUND;

	default:
		SLog::ex().write(__FUNCTION__, VAccess::RET_ERROR, "fetch userpara = %ld", lRet);
		assert(0);
		return VAccess::RET_ERROR;
	}
}

/*----------------------------------------------------------------------------*/
SppUserPara::SListProj::SListProj()
{
	clear();
}

/*----------------------------------------------------------------------------*/
bool SppUserPara::SListProj::operator == (const VData & r) const
{
	if (typeid(*this) != typeid(r))
        return false;

	return
	(
		(0 == memcmp(&stListCond  , 
		             &(((const SListProj &)r).stListCond  ), 
					 sizeof(T_ListCondppUserPara  )))
		&&
		(0 == memcmp(&stListProj, 
		             &(((const SListProj &)r).stListProj), 
					 sizeof(T_ListProjppUserPara)))
	) 
	? true : false;
}

/*----------------------------------------------------------------------------*/
void SppUserPara::SListProj::clear()
{
	memset(&stListCond, 0, sizeof(T_ListCondppUserPara));
	memset(&stListProj, 0, sizeof(T_ListProjppUserPara));
}

