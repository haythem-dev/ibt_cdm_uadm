/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH										       */
/**   Date   : 09.06.2004                                                     */
/**                                                                           */
/******************************************************************************/

#include "srv.h"

#include <libscmlogin/libscmlogin.h>

#include "tbl_ppgroup.h"
#include "struct_group.h"
#include "defines.h"

#include "SppGroup.h"

/*----------------------------------------------------------------------------*/
SppGroup::SppGroup() 
{
}

/*----------------------------------------------------------------------------*/
SppGroup::~SppGroup()
{
}

/*----------------------------------------------------------------------------*/
VAccess::ERet SppGroup::insertSingle(SListProj  & pSDataSingle )
{
	CBString            strSql;
	T_ListProjppGroup   * pCond = (T_ListProjppGroup *)&(pSDataSingle.stListProj);
	
	strSql.format("INSERT "
					"INTO ppgroup "
					"(name) "
					"VALUES ('%s')",
					pCond->NAME);
	
		
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
VAccess::ERet SppGroup::updateSingle(SListProj  & pSDataSingle )
{
	CBString            strSql;
	T_ListProjppGroup   * pCond = (T_ListProjppGroup *)&(pSDataSingle.stListProj);
	
	strSql.format("UPDATE "
						"ppgroup "
					"SET "
						"name = '%s'"
					"WHERE "
						"groupid = %d",
					pCond->NAME, pCond->GROUPID);
	
		
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
VAccess::ERet SppGroup::deleteSingle	(SListProj  & pSDataSingle )
{
	CBString            strSql;
	T_ListProjppGroup   * pCond = (T_ListProjppGroup *)&(pSDataSingle.stListProj);
	
	strSql.format("DELETE "
					"FROM ppgroup "
					"WHERE "
					"groupid = %d",
					pCond->GROUPID);

		long lRet = SqlExecuteImm((char *)strSql.c_str());

	switch (lRet)
	{
	case SQL_SUCCESS:
		//log to see who deleted a group
		SLog::ex().write(__FUNCTION__, VAccess::RET_OK, 
			": group %d, '%s' deleted by operator '%s'",  
			pCond->GROUPID, pCond->NAME,
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
VAccess::ERet SppGroup::deleteAppRight(SListProj  & pSDataSingle )
{
	CBString            strSql;
	T_ListProjppGroup   * pCond = (T_ListProjppGroup *)&(pSDataSingle.stListProj);
	
	strSql.format("DELETE "
					"FROM ppappright "
					"WHERE "
					"groupid = %d",
					pCond->GROUPID);

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
VAccess::ERet SppGroup::deleteAppCorUserGroup(SListProj  & pSDataSingle )
{
	CBString            strSql;
	T_ListProjppGroup   * pProj = (T_ListProjppGroup *)&(pSDataSingle.stListProj);
	T_ListCondppGroup   * pCond = (T_ListCondppGroup *)&(pSDataSingle.stListCond);
	
	if ( pCond->APPLICATIONID > 0 )
	{
		strSql.format("DELETE "
					"FROM ppcorappusergroup "
					"WHERE "
					"groupid = %d and applicationid = %d",
					pProj->GROUPID, pCond->APPLICATIONID);
	}
	else
	{
		strSql.format("DELETE "
					"FROM ppcorappusergroup "
					"WHERE "
					"groupid = %d",
					pProj->GROUPID);
	}

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
VAccess::ERet SppGroup::insertCorUser(SListProj  & pSDataSingle )
{
	CBString            strSql;
	T_ListCondppGroup   * pCond = (T_ListCondppGroup *)&(pSDataSingle.stListCond);
	T_ListProjppGroup   * pCond2 = (T_ListProjppGroup *)&(pSDataSingle.stListProj);
	

	strSql.format("INSERT "
					"INTO ppcorappusergroup "
					"(groupid, userid, applicationid) "
					"VALUES (%d, %d, %d)",
					pCond2->GROUPID, pCond->USERID, pCond->APPLICATIONID);
	
		
	long lRet = SqlExecuteImm((char *)strSql.c_str());

	switch (lRet) 
	{
	case SQL_SUCCESS:
		//log to see who put a user to a group
		SLog::ex().write(__FUNCTION__, VAccess::RET_OK, 
			": user %d set to group %d for App %d by operator '%s'",  
			pCond->USERID, pCond2->GROUPID, pCond->APPLICATIONID,
			scmod::ppBLoginData::ex().GetUsername().c_str());
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
VAccess::ERet SppGroup::deleteCorUser(SListProj  & pSDataSingle )
{
	CBString            strSql;
	T_ListCondppGroup   * pCond = (T_ListCondppGroup *)&(pSDataSingle.stListCond);
	T_ListProjppGroup   * pCond2 = (T_ListProjppGroup *)&(pSDataSingle.stListProj);
	
	strSql.format("DELETE "
					"FROM ppcorappusergroup "
					"WHERE "
					"userid = %d and groupid = %d and applicationid = %d",
					pCond->USERID, pCond2->GROUPID, pCond->APPLICATIONID);

		long lRet = SqlExecuteImm((char *)strSql.c_str());

	switch (lRet)
	{
	case SQL_SUCCESS:
		//log to see who erased a user from a group
		SLog::ex().write(__FUNCTION__, VAccess::RET_OK, 
			": user %d erased from group %d for App %d by operator '%s'",  
			pCond->USERID, pCond2->GROUPID, pCond->APPLICATIONID,
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
VAccess::ERet SppGroup::deleteAllCorUser(SListProj  & pSDataSingle )
{
	CBString            strSql;
	T_ListCondppGroup   * pCond = (T_ListCondppGroup *)&(pSDataSingle.stListCond);
	T_ListProjppGroup   * pCond2 = (T_ListProjppGroup *)&(pSDataSingle.stListProj);
	
	strSql.format("DELETE "
					"FROM ppcorappusergroup "
					"WHERE "
					"groupid = %d and applicationid = %d",
					pCond2->GROUPID, pCond->APPLICATIONID);

	long lRet = SqlExecuteImm((char *)strSql.c_str());

	switch (lRet)
	{
	case SQL_SUCCESS:
		//log to see who erased a user from a group
		SLog::ex().write(__FUNCTION__, VAccess::RET_OK, 
			": all user erased from group %d for App %d by operator '%s'",  
			pCond2->GROUPID, pCond->APPLICATIONID,
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
VAccess::ERet SppGroup::selectID	(CBString &strGroupname, long & lGroupid)
{
	CBString            strSql;

	strSql.format("SELECT "
						"g.groupid "
					"FROM ppgroup g "
                   "WHERE g.name = '%s'",
				   strGroupname.c_str());

    long lRet = SqlRead((char *)strSql.c_str(), &lGroupid, NULL);

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
VAccess::ERet SppGroup::SelTestForUpd(SListProj  & pSDataSingle )
{
	CBString         strSql;
	T_ListProjppGroup   * pProj = (T_ListProjppGroup *)&(pSDataSingle.stListProj);
	
	
	strSql.format("SELECT groupid, name "
					"FROM ppgroup "
					"WHERE groupid = %d"
					,pProj->GROUPID);

	long lRet = SqlRead((char *)strSql.c_str(), pProj, NULL);

	switch (lRet)
	{
	case SQL_SUCCESS:
		DtStructStripLast(pProj, STRUCDESC_LISTPROJPPGROUP);
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
VAccess::ERet SppGroup::openCursor(VData & pSDataCond)
{
	if (VAccVdooCurs::openCursor(pSDataCond) != VAccess::RET_OK)
		return VAccess::RET_OK;

	long        lRet;
	CBString   strSql;

	SListProj * pCond = (SListProj * )&pSDataCond;

	DtStructStripLast(&(pCond->stListCond), STRUCDESC_LISTCONDPPGROUP);
			
	if (pCond->stListCond.ALLGRP == false)
	{
		strSql.format("SELECT "
						  "g.groupid, "
						  "g.name "
					"FROM ppgroup g, ppappright a "
					"WHERE a.applicationid = %d and a.userid = 0 and a.groupid = g.groupid "

					"UNION "
					"SELECT "
						  "g.groupid, "
						  "g.name "
					"FROM ppgroup g "
					"WHERE EXISTS ( SELECT 1 FROM ppcorappusergroup c WHERE "
					"c.applicationid = %d and c.groupid = g.groupid) "
				    "ORDER BY 2",
				   pCond->stListCond.APPLICATIONID, pCond->stListCond.APPLICATIONID);
	}
	else if (pCond->stListCond.ALLGRP == true)
	{
		strSql.format("SELECT DISTINCT "
						  "g.groupid, "
						  "g.name "
					"FROM ppgroup g "
					"ORDER BY g.name"
				   );
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
VAccess::ERet SppGroup::fetch(VData & pSDataProj)
{
	long         lRet;
	T_ListProjppGroup * pProj = (T_ListProjppGroup *)&(((SListProj *)&pSDataProj)->stListProj);

	lRet = SqlBufferedFetch(m_hCursor, 1, 100, pProj, NULL); 

	switch(lRet)
	{
	case SQL_SUCCESS:
		DtStructStripLast(pProj, STRUCDESC_LISTPROJPPGROUP);
		return VAccess::RET_OK;

	case SQL_NOTFOUND:
		return VAccess::RET_NOTFOUND;

	default:
		SLog::ex().write(__FUNCTION__, VAccess::RET_ERROR, 
			                "fetch group = %ld",
							lRet);
		assert(0);
		return VAccess::RET_ERROR;
	}
}

/*----------------------------------------------------------------------------*/
SppGroup::SListProj::SListProj()
{
	clear();
}

/*----------------------------------------------------------------------------*/
bool SppGroup::SListProj::operator == (const VData & r) const
{
	if (typeid(*this) != typeid(r))
        return false;

	return
	(
		(0 == memcmp(&stListCond  , 
		             &(((const SListProj &)r).stListCond  ), 
					 sizeof(T_ListCondppGroup  )))
		&&
		(0 == memcmp(&stListProj, 
		             &(((const SListProj &)r).stListProj), 
					 sizeof(T_ListProjppGroup)))
	) 
	? true : false;
}

/*----------------------------------------------------------------------------*/
void SppGroup::SListProj::clear()
{
	memset(&stListCond, 0, sizeof(T_ListCondppGroup));
	memset(&stListProj, 0, sizeof(T_ListProjppGroup));
}

