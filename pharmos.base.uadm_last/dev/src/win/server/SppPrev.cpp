/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH										       */
/**   Date   : 16.08.2004                                                     */
/**                                                                           */
/******************************************************************************/

#include "srv.h"

#include "tbl_pploginprevious.h"
#include "tbl_ppuser.h"
#include "struct_previous.h"

#include "SppPrev.h"

/*----------------------------------------------------------------------------*/
SppPrev::SppPrev() 
{
}

/*----------------------------------------------------------------------------*/
SppPrev::~SppPrev()
{
}

/*----------------------------------------------------------------------------*/
VAccess::ERet SppPrev::deleteSingle	(SListProj  & pSDataSingle )
{
	CBString            strSql;
	T_ListCondppPrev   * pCond = (T_ListCondppPrev *)&(pSDataSingle.stListCond);
	T_ListProjppPrev   * pCond2 = (T_ListProjppPrev *)&(pSDataSingle.stListProj);

	
	strSql.format("DELETE "
					"FROM pploginprevious "
					"WHERE "
					"applicationid = %d and globaluser = '%s'",
					pCond->APPLICATIONID, pCond2->GLOBALUSER);

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
VAccess::ERet SppPrev::openCursor(VData & pSDataCond)
{
	if (VAccVdooCurs::openCursor(pSDataCond) != VAccess::RET_OK)
		return VAccess::RET_OK;

	long        lRet;
	CBString   strSql;

	SListProj * pCond = (SListProj * )&pSDataCond;

	DtStructStripLast(&(pCond->stListCond), STRUCDESC_LISTCONDPREVIOUS);
			
	strSql.format("SELECT "
						  "p.globaluser, "
						  "p.userid, "
						  "u.username, "
						  "p.languageid, "
						  "p.branchregionid "
					"FROM pploginprevious p, ppuser u "
					"WHERE p.applicationid = %d and p.userid = u.userid "
				    "ORDER BY p.globaluser",
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
VAccess::ERet SppPrev::fetch(VData & pSDataProj)
{
	long         lRet;
	T_ListProjppPrev * pProj = (T_ListProjppPrev *)&(((SListProj *)&pSDataProj)->stListProj);

	lRet = SqlBufferedFetch(m_hCursor, 1, 100, pProj, NULL); 

	switch(lRet)
	{
	case SQL_SUCCESS:
		DtStructStripLast(pProj, STRUCDESC_LISTPROJPREVIOUS);
		return VAccess::RET_OK;

	case SQL_NOTFOUND:
		return VAccess::RET_NOTFOUND;

	default:
		SLog::ex().write(__FUNCTION__, VAccess::RET_ERROR, 
			        "fetch previous = %ld",
					lRet);
		assert(0);
		return VAccess::RET_ERROR;
	}
}

/*----------------------------------------------------------------------------*/
SppPrev::SListProj::SListProj()
{
	clear();
}

/*----------------------------------------------------------------------------*/
bool SppPrev::SListProj::operator == (const VData & r) const
{
	if (typeid(*this) != typeid(r))
        return false;

	return
	(
		(0 == memcmp(&stListCond  , 
		             &(((const SListProj &)r).stListCond  ), 
					 sizeof(T_ListCondppPrev  )))
		&&
		(0 == memcmp(&stListProj, 
		             &(((const SListProj &)r).stListProj), 
					 sizeof(T_ListProjppPrev)))
	) 
	? true : false;
}

/*----------------------------------------------------------------------------*/
void SppPrev::SListProj::clear()
{
	memset(&stListCond, 0, sizeof(T_ListCondppPrev));
	memset(&stListProj, 0, sizeof(T_ListProjppPrev));
}

