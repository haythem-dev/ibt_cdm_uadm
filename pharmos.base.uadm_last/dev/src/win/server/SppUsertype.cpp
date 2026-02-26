/******************************************************************************/
/**                                                                           */
/**   Author : T.ARNOLD NTTDATA                                               */
/**   Date   : 25.11.2025                                                     */
/**                                                                           */
/******************************************************************************/

#include "srv.h"

#include <libscmlogin/libscmlogin.h>

#include "tbl_ppusertype.h"
#include "struct_usertype.h"
#include "defines.h"

#include "SppUsertype.h"

/*----------------------------------------------------------------------------*/
SppUsertype::SppUsertype() 
{
}

/*----------------------------------------------------------------------------*/
SppUsertype::~SppUsertype()
{
}

/*----------------------------------------------------------------------------*/
VAccess::ERet SppUsertype::insertSingle(SListProj  & pSDataSingle )
{
	CBString            strSql;
	T_ListProjppUsertype* pCond = (T_ListProjppUsertype*)&(pSDataSingle.stListProj);
	
	strSql.format("SELECT CAST(NVL(MAX(usertypeid),0)+1 AS SMALLINT) FROM ppusertype");
	long lRet = SqlRead((char*)strSql.c_str(), &pCond->USERTYPEID, NULL);
	if (lRet == SQL_SUCCESS)
	{
		strSql.format("INSERT "
						"INTO ppusertype "
						"(usertypeid, name, description) "
						"VALUES (%d, '%s', '%s')",
						pCond->USERTYPEID, pCond->NAME, pCond->DESCRIPTION);
	
		
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
VAccess::ERet SppUsertype::updateSingle(SListProj  & pSDataSingle )
{
	CBString            strSql;
	T_ListProjppUsertype* pCond = (T_ListProjppUsertype*)&(pSDataSingle.stListProj);
	
	strSql.format("UPDATE "
						"ppusertype "
					"SET "
						"name = '%s', "
		                "description = '%s' "
					"WHERE "
						"usertypeid = %d",
					pCond->NAME, pCond->DESCRIPTION, pCond->USERTYPEID);
	
		
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
VAccess::ERet SppUsertype::deleteSingle	(SListProj  & pSDataSingle )
{
	CBString            strSql;
	T_ListProjppUsertype* pCond = (T_ListProjppUsertype*)&(pSDataSingle.stListProj);

	//TODO: check if usertype is used in ppuser table

	strSql.format("DELETE "
					"FROM ppusertype "
					"WHERE "
					"usertypeid = %d",
					pCond->USERTYPEID);

		long lRet = SqlExecuteImm((char *)strSql.c_str());

	switch (lRet)
	{
	case SQL_SUCCESS:
		//log to see who deleted a group
		SLog::ex().write(__FUNCTION__, VAccess::RET_OK, 
			": usertype %d, '%s' deleted by operator '%s'",  
			pCond->USERTYPEID, pCond->NAME,
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
VAccess::ERet SppUsertype::selectID	(CBString &strUsertypename, short& sUsertypeid)
{
	CBString            strSql;

	strSql.format("SELECT usertypeid FROM ppusertype WHERE name = '%s'", strUsertypename.c_str());

    long lRet = SqlRead((char *)strSql.c_str(), &sUsertypeid, NULL);

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
VAccess::ERet SppUsertype::SelTestForUpd(SListProj  & pSDataSingle )
{
	CBString         strSql;
	T_ListProjppUsertype* pProj = (T_ListProjppUsertype*)&(pSDataSingle.stListProj);
	
	
	strSql.format("SELECT usertypeid, name, description, 0 FROM ppusertype WHERE usertypeid = %d", pProj->USERTYPEID);

	long lRet = SqlRead((char *)strSql.c_str(), pProj, NULL);

	switch (lRet)
	{
	case SQL_SUCCESS:
		DtStructStripLast(pProj, STRUCDESC_LISTPROJPPUSERTYPE);
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
VAccess::ERet SppUsertype::openCursor(VData & pSDataCond)
{
	if (VAccVdooCurs::openCursor(pSDataCond) != VAccess::RET_OK)
		return VAccess::RET_OK;

	long        lRet;
	CBString   strSql;

	SListProj * pCond = (SListProj * )&pSDataCond;

	DtStructStripLast(&(pCond->stListCond), STRUCDESC_LISTCONDPPUSERTYPE);
			
	if (pCond->stListCond.ALLUSERTYPES == false)
	{
		//TODO: scan for usertypes
	}
	else if (pCond->stListCond.ALLUSERTYPES == true)
	{
		CBString strOrder;
		if (pCond->stListCond.SORTINDEX == 0) //id
		{
			strOrder = "ORDER BY usertypeid ";
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
		if (strlen(pCond->stListCond.USERTYPENAME) > 0)
		{
			strSql.format("SELECT usertypeid, name, description, 0 as cnt FROM ppusertype WHERE name LIKE '%%%s%%' %s", pCond->stListCond.USERTYPENAME, strOrder.c_str());
		}
		else
		{
			strSql.format("SELECT usertypeid, name, description, 0 as cnt FROM ppusertype %s", strOrder.c_str());
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
VAccess::ERet SppUsertype::fetch(VData & pSDataProj)
{
	long         lRet;
	T_ListProjppUsertype * pProj = (T_ListProjppUsertype *)&(((SListProj *)&pSDataProj)->stListProj);

	lRet = SqlBufferedFetch(m_hCursor, 1, 100, pProj, NULL); 

	switch(lRet)
	{
	case SQL_SUCCESS:
		DtStructStripLast(pProj, STRUCDESC_LISTPROJPPUSERTYPE);
		return VAccess::RET_OK;

	case SQL_NOTFOUND:
		return VAccess::RET_NOTFOUND;

	default:
		SLog::ex().write(__FUNCTION__, VAccess::RET_ERROR, "fetch usertype = %ld", lRet);
		assert(0);
		return VAccess::RET_ERROR;
	}
}

/*----------------------------------------------------------------------------*/
SppUsertype::SListProj::SListProj()
{
	clear();
}

/*----------------------------------------------------------------------------*/
bool SppUsertype::SListProj::operator == (const VData & r) const
{
	if (typeid(*this) != typeid(r))
        return false;

	return
	(
		(0 == memcmp(&stListCond  , 
		             &(((const SListProj &)r).stListCond  ), 
					 sizeof(T_ListCondppUsertype  )))
		&&
		(0 == memcmp(&stListProj, 
		             &(((const SListProj &)r).stListProj), 
					 sizeof(T_ListProjppUsertype)))
	) 
	? true : false;
}

/*----------------------------------------------------------------------------*/
void SppUsertype::SListProj::clear()
{
	memset(&stListCond, 0, sizeof(T_ListCondppUsertype));
	memset(&stListProj, 0, sizeof(T_ListProjppUsertype));
}

