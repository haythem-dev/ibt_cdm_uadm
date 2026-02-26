/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH										       */
/**   Date   : 14.06.2004                                                     */
/**                                                                           */
/******************************************************************************/

#include "srv.h"

#include <libscmlogin/libscmlogin.h>

#include "tbl_ppcorappbranchreg.h"
#include "tbl_ppregion.h"
#include "tbl_ppbranch.h"
#include "struct_branchreg.h"
#include "defines.h"

#include "SppBranchReg.h"

/*----------------------------------------------------------------------------*/
SppBranchReg::SppBranchReg() 
{
}

/*----------------------------------------------------------------------------*/
SppBranchReg::~SppBranchReg()
{
}

/*----------------------------------------------------------------------------*/
VAccess::ERet SppBranchReg::insertAppBranchReg(SListProj  & pSDataSingle )
{
	CBString            strSql;
	T_ListCondppBranchReg   * pCond = (T_ListCondppBranchReg *)&(pSDataSingle.stListCond);
	T_ListProjppBranchReg   * pCond2 = (T_ListProjppBranchReg *)&(pSDataSingle.stListProj);
	

	strSql.format("INSERT "
					"INTO ppcorappbranchreg "
					"(applicationid, branchid, regionid) "
					"VALUES (%d, %d, %d)",
					pCond->APPLICATIONID, pCond2->BRANCHID, pCond2->REGIONID);
	
		
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
VAccess::ERet SppBranchReg::insertBranch(SListProj  & pSDataSingle )
{
	CBString            strSql;
	T_ListProjppBranch   * pCond = (T_ListProjppBranch *)&(pSDataSingle.stListBranch);
	

	strSql.format("INSERT "
					"INTO ppbranch "
					"(branchid, name) "
					"VALUES (%d, '%s')",
					pCond->BRANCHID, pCond->BRANCHNAME);
	
		
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
VAccess::ERet SppBranchReg::insertReg(SListProj  & pSDataSingle )
{
	CBString            strSql;
	T_ListProjppRegion   * pCond = (T_ListProjppRegion *)&(pSDataSingle.stListRegion);
	

	strSql.format("INSERT "
					"INTO ppregion "
					"(regionid, name) "
					"VALUES (%d, '%s')",
					pCond->REGIONID, pCond->REGNAME);
	
		
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
VAccess::ERet SppBranchReg::deleteCORReg	  (SListProj  & pSDataSingle )
{
	CBString            strSql;
	T_ListCondppBranchReg   * pCond = (T_ListCondppBranchReg *)&(pSDataSingle.stListCond);
	T_ListProjppBranchReg   * pCond2 = (T_ListProjppBranchReg *)&(pSDataSingle.stListProj);
	
	strSql.format("DELETE "
					"FROM ppcorappbranchreg "
					"WHERE "
					"applicationid = %d and regionid = %d",
					pCond->APPLICATIONID, pCond2->REGIONID);

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
VAccess::ERet SppBranchReg::deleteAppBranchReg(SListProj  & pSDataSingle )
{
	CBString            strSql;
	T_ListCondppBranchReg   * pCond = (T_ListCondppBranchReg *)&(pSDataSingle.stListCond);
	T_ListProjppBranchReg   * pCond2 = (T_ListProjppBranchReg *)&(pSDataSingle.stListProj);
	
	strSql.format("DELETE "
					"FROM ppcorappbranchreg "
					"WHERE "
					"applicationid = %d and branchid = %d and regionid = %d",
					pCond->APPLICATIONID, pCond2->BRANCHID, pCond2->REGIONID);

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
VAccess::ERet SppBranchReg::deleteBranch(SListProj  & pSDataSingle )
{
	CBString            strSql;
	T_ListProjppBranch   * pCond = (T_ListProjppBranch *)&(pSDataSingle.stListBranch);
	
	strSql.format("DELETE "
					"FROM ppbranch "
					"WHERE "
					"branchid = %d",
					pCond->BRANCHID);

		long lRet = SqlExecuteImm((char *)strSql.c_str());

	switch (lRet)
	{
	case SQL_SUCCESS:
		//log to see who deleted a branch 
		SLog::ex().write(__FUNCTION__, VAccess::RET_OK, 
			": branch %d deleted by operator '%s'",  
			pCond->BRANCHID,
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
VAccess::ERet SppBranchReg::deleteRegPrevious	(SListProj  & pSDataSingle )
{
	CBString            strSql;
	T_ListProjppRegion   * pCond = (T_ListProjppRegion *)&(pSDataSingle.stListRegion);
	
	strSql.format("DELETE "
					"FROM pploginprevious "
					"WHERE "
					"branchregionid = %d",
					pCond->REGIONID);

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
VAccess::ERet SppBranchReg::deleteRegCorBranch	(SListProj  & pSDataSingle )
{
	CBString            strSql;
	T_ListProjppRegion   * pCond = (T_ListProjppRegion *)&(pSDataSingle.stListRegion);
	
	strSql.format("DELETE "
					"FROM ppcorappbranchreg "
					"WHERE "
					"regionid = %d",
					pCond->REGIONID);

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
VAccess::ERet SppBranchReg::deleteRegAppRight	(SListProj  & pSDataSingle )
{
	CBString            strSql;
	T_ListProjppRegion   * pCond = (T_ListProjppRegion *)&(pSDataSingle.stListRegion);
	
	strSql.format("DELETE "
					"FROM ppappright "
					"WHERE "
					"branchregionid = %d",
					pCond->REGIONID);

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
VAccess::ERet SppBranchReg::deleteBrPrevious	(SListProj  & pSDataSingle )
{
	CBString            strSql;
	T_ListProjppBranch   * pCond = (T_ListProjppBranch *)&(pSDataSingle.stListBranch);
	
	strSql.format("DELETE "
					"FROM pploginprevious "
					"WHERE "
					"branchregionid = %d",
					pCond->BRANCHID);

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
VAccess::ERet SppBranchReg::deleteBrCorReg(SListProj  & pSDataSingle )
{
	CBString            strSql;
	T_ListProjppBranch   * pCond = (T_ListProjppBranch *)&(pSDataSingle.stListBranch);
	
	strSql.format("DELETE "
					"FROM ppcorappbranchreg "
					"WHERE "
					"branchid = %d",
					pCond->BRANCHID);

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
VAccess::ERet SppBranchReg::deleteBrAppRight	(SListProj  & pSDataSingle )
{
	CBString            strSql;
	T_ListProjppBranch   * pCond = (T_ListProjppBranch *)&(pSDataSingle.stListBranch);
	
	strSql.format("DELETE "
					"FROM ppappright "
					"WHERE "
					"branchregionid = %d",
					pCond->BRANCHID);

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
VAccess::ERet SppBranchReg::deleteReg(SListProj  & pSDataSingle )
{
	CBString            strSql;
	T_ListProjppRegion   * pCond = (T_ListProjppRegion *)&(pSDataSingle.stListRegion);
	
	strSql.format("DELETE "
					"FROM ppregion "
					"WHERE "
					"regionid = %d",
					pCond->REGIONID);

		long lRet = SqlExecuteImm((char *)strSql.c_str());

	switch (lRet)
	{
	case SQL_SUCCESS:
		//log to see who deleted a region 
		SLog::ex().write(__FUNCTION__, VAccess::RET_OK, 
			": region %d deleted by operator '%s'",  
			pCond->REGIONID,
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
VAccess::ERet SppBranchReg::updateBranch(SListProj  & pSDataSingle )
{
	CBString            strSql;
	T_ListProjppBranch * pCond = (T_ListProjppBranch *)&(pSDataSingle.stListBranch);


	strSql.format("UPDATE "
						"ppbranch "
					"SET "
						"name = '%s' "
					"WHERE "
						"branchid = %d",
						pCond->BRANCHNAME, pCond->BRANCHID);
	
		
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
VAccess::ERet SppBranchReg::updateReg(SListProj  & pSDataSingle )
{
	CBString            strSql;
	T_ListProjppRegion * pCond = (T_ListProjppRegion *)&(pSDataSingle.stListRegion);


	strSql.format("UPDATE "
						"ppregion "
					"SET "
						"name = '%s' "
					"WHERE "
						"regionid = %d",
						pCond->REGNAME, pCond->REGIONID);
	
		
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
VAccess::ERet SppBranchReg::SelTestForUpdBr(SListProj  & pSDataSingle )
{
	CBString         strSql;
	T_ListProjppBranch *   pProj = (T_ListProjppBranch *)&(pSDataSingle.stListBranch);

	
	strSql.format("SELECT branchid, name "
					"FROM ppbranch "
					"WHERE branchid = %d"
					,pProj->BRANCHID);

	long lRet = SqlRead((char *)strSql.c_str(), pProj, NULL);

	switch (lRet)
	{
	case SQL_SUCCESS:
		DtStructStripLast(pProj, STRUCDESC_LISTPROJPPBRANCH);
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
VAccess::ERet SppBranchReg::SelTestForUpdReg(SListProj  & pSDataSingle )
{
	CBString         strSql;
	T_ListProjppRegion *   pProj = (T_ListProjppRegion *)&(pSDataSingle.stListRegion);

	
	strSql.format("SELECT regionid, name "
					"FROM ppregion "
					"WHERE regionid = %d"
					,pProj->REGIONID);

	long lRet = SqlRead((char *)strSql.c_str(), pProj, NULL);

	switch (lRet)
	{
	case SQL_SUCCESS:
		DtStructStripLast(pProj, STRUCDESC_LISTPROJPPREGION);
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
VAccess::ERet SppBranchReg::delCorBrFromPrev	(SListProj  & pSDataSingle )
{
	CBString            strSql;
	T_ListProjppBranch   * pCond = (T_ListProjppBranch *)&(pSDataSingle.stListBranch);
	T_ListCondppBranchReg   * pCond2 = (T_ListCondppBranchReg *)&(pSDataSingle.stListCond);
	
	strSql.format("DELETE "
					"FROM pploginprevious "
					"WHERE "
					"branchregionid = %d and applicationid = %d",
					pCond->BRANCHID, pCond2->APPLICATIONID);

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
VAccess::ERet SppBranchReg::delCorBrFromAppRight(SListProj  & pSDataSingle )
{
	CBString            strSql;
	T_ListProjppBranch   * pCond = (T_ListProjppBranch *)&(pSDataSingle.stListBranch);
	T_ListCondppBranchReg   * pCond2 = (T_ListCondppBranchReg *)&(pSDataSingle.stListCond);
	
	strSql.format("DELETE "
					"FROM ppappright "
					"WHERE "
					"branchregionid = %d and applicationid = %d",
					pCond->BRANCHID, pCond2->APPLICATIONID);

	long lRet = SqlExecuteImm((char *)strSql.c_str());

	switch (lRet)
	{
	case SQL_SUCCESS:
		//log to see who erased a branch 
		SLog::ex().write(__FUNCTION__, VAccess::RET_OK, 
			": branch %d erased from app %d by operator '%s'",  
			pCond->BRANCHID, pCond2->APPLICATIONID,
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
VAccess::ERet SppBranchReg::delCorRegFromPrev	 (SListProj  & pSDataSingle )
{
	CBString            strSql;
	T_ListProjppRegion   * pCond = (T_ListProjppRegion *)&(pSDataSingle.stListRegion);
	T_ListCondppBranchReg   * pCond2 = (T_ListCondppBranchReg *)&(pSDataSingle.stListCond);
	
	strSql.format("DELETE "
					"FROM pploginprevious "
					"WHERE "
					"branchregionid = %d and applicationid = %d",
					pCond->REGIONID, pCond2->APPLICATIONID);

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
VAccess::ERet SppBranchReg::delCorRegFromAppRight(SListProj  & pSDataSingle )
{
	CBString            strSql;
	T_ListProjppRegion   * pCond = (T_ListProjppRegion *)&(pSDataSingle.stListRegion);
	T_ListCondppBranchReg   * pCond2 = (T_ListCondppBranchReg *)&(pSDataSingle.stListCond);
	
	strSql.format("DELETE "
					"FROM ppappright "
					"WHERE "
					"branchregionid = %d and applicationid = %d",
					pCond->REGIONID, pCond2->APPLICATIONID);

	long lRet = SqlExecuteImm((char *)strSql.c_str());

	switch (lRet)
	{
	case SQL_SUCCESS:
		//log to see who erased a region 
		SLog::ex().write(__FUNCTION__, VAccess::RET_OK, 
			": region %d erased from app %d by operator '%s'",  
			pCond->REGIONID, pCond2->APPLICATIONID,
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
VAccess::ERet SppBranchReg::openCursor(VData & pSDataCond)
{
	if (VAccVdooCurs::openCursor(pSDataCond) != VAccess::RET_OK)
		return VAccess::RET_OK;

	long        lRet;
	CBString   strSql;

	SListProj * pCond = (SListProj * )&pSDataCond;

	DtStructStripLast(&(pCond->stListCond), STRUCDESC_LISTCONDPPBRANCHREG);
			
	if( pCond->stListCond.SELBRANCH == true && pCond->stListCond.SELREG == false && pCond->stListCond.USERID > 0)
	{
		m_bRegion = false;
		m_bBranch = true;

		strSql.format("select b.branchid, b.name "
						"from ppbranch b, ppappright r "
						"where r.applicationid = %d and r.branchregionid < 1000 "
							"and r.branchregionid = b.branchid and r.userid = %ld "
						"union "
					  "select b.branchid, b.name "
						"from ppbranch b, ppappright r "
						"where r.applicationid = %d and r.branchregionid < 1000 "
							"and r.branchregionid = b.branchid and "
							"r.groupid IN (SELECT c.groupid FROM ppcorappusergroup c "
											"WHERE c.applicationid = %d and c.userid = %ld) "
						"ORDER BY 1",
					pCond->stListCond.APPLICATIONID, pCond->stListCond.USERID,
					pCond->stListCond.APPLICATIONID, pCond->stListCond.APPLICATIONID, pCond->stListCond.USERID	);
	}
	else if( pCond->stListCond.SELBRANCH == false && pCond->stListCond.SELREG == true && pCond->stListCond.USERID > 0)
	{
		m_bRegion = true;
		m_bBranch = false;

		strSql.format("select b.regionid, b.name "
						"from ppregion b, ppappright r "
						"where r.applicationid = %d and r.branchregionid > 999 "
							"and r.branchregionid = b.regionid and r.userid = %ld "
						"union "
					  "select b.regionid, b.name "
						"from ppregion b, ppappright r "
						"where r.applicationid = %d and r.branchregionid > 999 "
							"and r.branchregionid = b.regionid and "
							"r.groupid IN (SELECT c.groupid FROM ppcorappusergroup c "
											"WHERE c.applicationid = %d and c.userid = %ld) "
						"ORDER BY 1",
					pCond->stListCond.APPLICATIONID, pCond->stListCond.USERID,
					pCond->stListCond.APPLICATIONID, pCond->stListCond.APPLICATIONID, pCond->stListCond.USERID	);
	}
	else if( pCond->stListCond.SELBRANCH == true && pCond->stListCond.SELREG == true && pCond->stListCond.SELCOR == true)
	{
		m_bRegion = false;
		m_bBranch = true;

		strSql.format("SELECT DISTINCT "
						  "c.branchid,"
						  "b.name "
					"FROM "
						"ppcorappbranchreg c, "
						"ppbranch b "
					"WHERE "
						"c.applicationid = %d "
						"and b.branchid = c.branchid "
						"and c.regionid = %d "
					"ORDER BY c.branchid",
					pCond->stListCond.APPLICATIONID, pCond->stListRegion.REGIONID);
	}
	else if( pCond->stListCond.SELBRANCH == false && pCond->stListCond.SELREG == false && pCond->stListCond.SELCOR == false)
	{
		m_bRegion = false;
		m_bBranch = true;

		strSql.format("SELECT DISTINCT "
						  "b.branchid, "
						  "b.name "
					"FROM ppbranch b "
					"WHERE b.branchid NOT IN "
					"(SELECT "
						"c.branchid "
					"FROM ppcorappbranchreg c "
					"WHERE c.applicationid = %d and c.regionid = %d) "
					"ORDER BY b.branchid",
					pCond->stListCond.APPLICATIONID, pCond->stListRegion.REGIONID);
	}	
	else if (pCond->stListCond.SELBRANCH == true && pCond->stListCond.SELCOR == true && pCond->stListCond.SELREG == false)
	{
		m_bRegion = false;
		m_bBranch = true;

		strSql.format("SELECT DISTINCT "
						  "c.branchid,"
						  "b.name "
					"FROM "
						"ppcorappbranchreg c, "
						"ppbranch b "
					"WHERE "
						"c.applicationid = %d "
						"and b.branchid = c.branchid "
						"and c.regionid = 0 "
					"ORDER BY c.branchid",
				   pCond->stListCond.APPLICATIONID);
	}
	else if (pCond->stListCond.SELREG == true && pCond->stListCond.SELCOR == true && pCond->stListCond.SELBRANCH == false)
	{
		m_bRegion = true;
		m_bBranch = false;
		
		strSql.format("SELECT DISTINCT "
						  "c.regionid,"
						  "r.name "
					"FROM "
						"ppcorappbranchreg c, "
						"ppregion r "
					"WHERE "
						"c.applicationid = %d "
						"and r.regionid = c.regionid "
					"ORDER BY c.regionid",
				   pCond->stListCond.APPLICATIONID);
	}
	else if (pCond->stListCond.SELREG == true && pCond->stListCond.SELCOR == false && pCond->stListCond.SELBRANCH == false)
	{
		m_bRegion = true;
		m_bBranch = false;

		strSql.format("SELECT DISTINCT "
						  "r.regionid, "
						  "r.name "
					"FROM ppregion r "
					"WHERE r.regionid NOT IN "
					"(SELECT "
						"c.regionid "
					"FROM ppcorappbranchreg c "
					"WHERE c.applicationid = %d) "
					"ORDER BY r.regionid",
					pCond->stListCond.APPLICATIONID);
	}
    else if(pCond->stListCond.SELBRANCH == true && pCond->stListCond.SELCOR == false && pCond->stListCond.SELREG == false && pCond->stListCond.APPLICATIONID == -1 )
    { //query for UA31
        m_bRegion = false;
		m_bBranch = true;
        strSql.format("SELECT DISTINCT "
						  "b.branchid, "
						  "b.name "
					"FROM ppbranch b order by b.name ASC "
					);
    }
	else if (pCond->stListCond.SELBRANCH == true && pCond->stListCond.SELCOR == false && pCond->stListCond.SELREG == false)
	{
		m_bRegion = false;
		m_bBranch = true;

		strSql.format("SELECT DISTINCT "
						  "b.branchid, "
						  "b.name "
					"FROM ppbranch b "
					"WHERE b.branchid NOT IN "
					"(SELECT "
						"c.branchid "
					"FROM ppcorappbranchreg c "
					"WHERE c.applicationid = %d and c.regionid = 0) "
					"ORDER BY b.branchid",
					pCond->stListCond.APPLICATIONID);
	}
	/*//noch nicht benötigt!
		m_bRegion = false;
		m_bBranch = false;

		strSql.format("SELECT DISTINCT "
						  "b.branchid,"
						  "r.regionid, "
						  "b.name, "
						  "r.name "
					"FROM "
						"ppcorappbranchreg c, "
						"outer ppbranch b, "
						"outer ppregion r "
					"WHERE "
						"c.applicationid = %d "
						"and b.branchid = c.branchid "
						"and r.regionid = c.regionid "
				    "ORDER BY b.name, r.name",
				   pCond->stListCond.APPLICATIONID);*/
	
	
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
VAccess::ERet SppBranchReg::fetch(VData & pSDataProj)
{
	long         lRet;
	
	if( m_bRegion == false && m_bBranch == false)
	{
		T_ListProjppBranchReg * pProj = (T_ListProjppBranchReg *)&(((SListProj *)&pSDataProj)->stListProj);
		lRet = SqlBufferedFetch(m_hCursor, 1, 100, pProj, NULL); 
		DtStructStripLast(pProj, STRUCDESC_LISTPROJPPBRANCHREG);
	}
	else if (m_bBranch == true)
	{
		T_ListProjppBranch * pProj = (T_ListProjppBranch *)&(((SListProj *)&pSDataProj)->stListBranch);
		lRet = SqlBufferedFetch(m_hCursor, 1, 100, pProj, NULL); 
		DtStructStripLast(pProj, STRUCDESC_LISTPROJPPBRANCH);
	}
	else if (m_bRegion == true)
	{
		T_ListProjppRegion * pProj = (T_ListProjppRegion *)&(((SListProj *)&pSDataProj)->stListRegion);
		lRet = SqlBufferedFetch(m_hCursor, 1, 100, pProj, NULL); 
		DtStructStripLast(pProj, STRUCDESC_LISTPROJPPREGION);
	}

	
	switch(lRet)
	{
	case SQL_SUCCESS:
		return VAccess::RET_OK;

	case SQL_NOTFOUND:
		return VAccess::RET_NOTFOUND;

	default:
		SLog::ex().write(__FUNCTION__, VAccess::RET_ERROR, 
			        "fetch branchreg = %ld",
					lRet);
		assert(0);
		return VAccess::RET_ERROR;
	}
}

/*----------------------------------------------------------------------------*/
SppBranchReg::SListProj::SListProj()
{
	clear();
}

/*----------------------------------------------------------------------------*/
bool SppBranchReg::SListProj::operator == (const VData & r) const
{
	if (typeid(*this) != typeid(r))
        return false;

	return
	(
		(0 == memcmp(&stListCond  , 
		             &(((const SListProj &)r).stListCond  ), 
					 sizeof(T_ListCondppBranchReg  )))
		&&
		(0 == memcmp(&stListProj, 
		             &(((const SListProj &)r).stListProj), 
					 sizeof(T_ListProjppBranchReg)))

		&&
		(0 == memcmp(&stListBranch, 
		             &(((const SListProj &)r).stListBranch), 
					 sizeof(T_ListProjppBranch)))

		&&
		(0 == memcmp(&stListRegion, 
		             &(((const SListProj &)r).stListRegion), 
					 sizeof(T_ListProjppRegion)))

	) 
	? true : false;
}

/*----------------------------------------------------------------------------*/
void SppBranchReg::SListProj::clear()
{
	memset(&stListCond, 0, sizeof(T_ListCondppBranchReg));
	memset(&stListProj, 0, sizeof(T_ListProjppBranchReg));
	memset(&stListBranch, 0, sizeof(T_ListProjppBranch));
	memset(&stListRegion, 0, sizeof(T_ListProjppRegion));
}