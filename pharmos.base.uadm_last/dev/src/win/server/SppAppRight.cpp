/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH										       */
/**   Date   : 15.06.2004                                                     */
/**                                                                           */
/******************************************************************************/

#include "srv.h"

#include <libscmlogin/libscmlogin.h>

#include "tbl_ppappright.h"
#include "tbl_ppappmodright.h"
#include "struct_appright.h"
#include "defines.h"

#include "SppAppRight.h"

/*----------------------------------------------------------------------------*/
SppAppRight::SppAppRight() 
{
}

/*----------------------------------------------------------------------------*/
SppAppRight::~SppAppRight()
{
}

/*----------------------------------------------------------------------------*/
VAccess::ERet SppAppRight::insertAppModRight(SListProj  & pSDataSingle )
{
	CBString            strSql;
	T_ListProjppAppRight * pCond2 = (T_ListProjppAppRight *)&(pSDataSingle.stListProj);
	T_ListCondppAppRight * pCond = (T_ListCondppAppRight *)&(pSDataSingle.stListCond);

	strSql.format("INSERT "
					"INTO ppappmodright "
					"(applicationid, moduleid, rightname, description, description_en, description_fr, description_hr, category ) "
					"VALUES (%d, %d, '%s', '%s', '%s', '%s', '%s', %d)",
					pCond->APPLICATIONID, pCond->MODULEID, 
					pCond2->RIGHTNAME, pCond2->RIGHTDESC,
					pCond2->RIGHTDESC_EN, pCond2->RIGHTDESC_FR, pCond2->RIGHTDESC_HR, pCond2->CATEGORY);
			
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
VAccess::ERet SppAppRight::insertAppRight(SListProj  & pSDataSingle )
{
	CBString            strSql;
	T_ListProjppAppRight * pCond2 = (T_ListProjppAppRight *)&(pSDataSingle.stListProj);
	T_ListCondppAppRight * pCond = (T_ListCondppAppRight *)&(pSDataSingle.stListCond);

	strSql.format("INSERT "
					"INTO ppappright "
					"(groupid, userid, applicationid, moduleid, rightname, branchregionid) "
					"VALUES (%d, %d, %d, %d, '%s', %d)",
					pCond->GROUPID, pCond->USERID, pCond->APPLICATIONID,
					pCond->MODULEID, pCond2->RIGHTNAME, pCond->BRANCHREGID);// TH logging of username !?
	
		
	long lRet = SqlExecuteImm((char *)strSql.c_str());

	switch (lRet)
	{
	case SQL_SUCCESS:
		//log to see who gave right to a user 
		SLog::ex().write(__FUNCTION__, VAccess::RET_OK, 
			": new right '%s' given to user %d / group %d for App %d, Mod %d, Area %d by operator '%s'",  
			pCond2->RIGHTNAME, pCond->USERID, pCond->GROUPID, pCond->APPLICATIONID, pCond->MODULEID,
			pCond->BRANCHREGID, scmod::ppBLoginData::ex().GetUsername().c_str());
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
VAccess::ERet SppAppRight::deleteFromAppRight(SListProj  & pSDataSingle )
{
	CBString            strSql;
	T_ListProjppAppRight * pCond2 = (T_ListProjppAppRight *)&(pSDataSingle.stListProj);
	T_ListCondppAppRight * pCond = (T_ListCondppAppRight *)&(pSDataSingle.stListCond);

	strSql.format("DELETE "
					"FROM ppappright "
					"WHERE "
					"applicationid = %d and moduleid = %d and rightname = '%s'",
					pCond->APPLICATIONID, pCond->MODULEID, pCond2->RIGHTNAME);
	
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
VAccess::ERet SppAppRight::deleteAppRight(SListProj  & pSDataSingle )
{
	CBString            strSql;
	T_ListProjppAppRight * pCond2 = (T_ListProjppAppRight *)&(pSDataSingle.stListProj);
	T_ListCondppAppRight * pCond = (T_ListCondppAppRight *)&(pSDataSingle.stListCond);

	CBString strTemp = pCond2->RIGHTNAME;
	if (strTemp.empty())
	{
		strSql.format("DELETE "
					"FROM ppappright "
					"WHERE "
					"applicationid = %d and moduleid = %d "
					"and groupid = %d and userid = %d and branchregionid = %d",
					pCond->APPLICATIONID, pCond->MODULEID,
					pCond->GROUPID, pCond->USERID, pCond->BRANCHREGID);
	}
	else
	{
		strSql.format("DELETE "
					"FROM ppappright "
					"WHERE "
					"applicationid = %d and moduleid = %d and rightname = '%s' "
					"and groupid = %d and userid = %d and branchregionid = %d",
					pCond->APPLICATIONID, pCond->MODULEID, pCond2->RIGHTNAME,
					pCond->GROUPID, pCond->USERID, pCond->BRANCHREGID);
	}
	
	long lRet = SqlExecuteImm((char *)strSql.c_str());

	switch (lRet)
	{
	case SQL_SUCCESS:
		//log to see who removed right from a user
		if (strTemp.empty())
		{
			SLog::ex().write(__FUNCTION__, VAccess::RET_OK, 
			": all rights removed from user %d / group %d for App %d, Mod %d, Area %d by operator '%s'",  
			pCond->USERID, pCond->GROUPID, pCond->APPLICATIONID, pCond->MODULEID,
			pCond->BRANCHREGID, scmod::ppBLoginData::ex().GetUsername().c_str());
		}
		else
		{
			SLog::ex().write(__FUNCTION__, VAccess::RET_OK, 
			": right '%s' removed from user %d / group %d for App %d, Mod %d, Area %d by operator '%s'",  
			pCond2->RIGHTNAME, pCond->USERID, pCond->GROUPID, pCond->APPLICATIONID, pCond->MODULEID,
			pCond->BRANCHREGID, scmod::ppBLoginData::ex().GetUsername().c_str());
		}
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
VAccess::ERet SppAppRight::SelTestForUpd(SListProj  & pSDataSingle )
{
	CBString         strSql;
	T_ListCondppAppRight   * pCond = (T_ListCondppAppRight *)&(pSDataSingle.stListCond);
	T_ListProjppAppRight *   pProj = (T_ListProjppAppRight *)&(pSDataSingle.stListProj);

	
	strSql.format("SELECT rightname, description, description_en, description_fr, description_hr, category "
					"FROM ppappmodright "
					"WHERE applicationid = %d and moduleid = %d and rightname = '%s'"
					,pCond->APPLICATIONID, pCond->MODULEID, pProj->RIGHTNAME);

	
	long lRet = SqlRead((char *)strSql.c_str(), pProj, NULL);

	switch (lRet)
	{
	case SQL_SUCCESS:
		DtStructStripLast(pProj, STRUCDESC_LISTPROJPPAPPRIGHT);
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
VAccess::ERet SppAppRight::deleteAppModRight(SListProj  & pSDataSingle )
{
	CBString            strSql;
	T_ListProjppAppRight * pCond2 = (T_ListProjppAppRight *)&(pSDataSingle.stListProj);
	T_ListCondppAppRight * pCond = (T_ListCondppAppRight *)&(pSDataSingle.stListCond);

	strSql.format("DELETE "
					"FROM ppappmodright "
					"WHERE "
					"applicationid = %d and moduleid = %d and rightname = '%s'",
					pCond->APPLICATIONID, pCond->MODULEID, pCond2->RIGHTNAME);
	
		long lRet = SqlExecuteImm((char *)strSql.c_str());

	switch (lRet)
	{
	case SQL_SUCCESS:
		//log to see who deleted a right
		SLog::ex().write(__FUNCTION__, VAccess::RET_OK, 
			": right '%s' deleted for App %d, Mod %d by operator '%s'",  
			pCond2->RIGHTNAME, pCond->APPLICATIONID, pCond->MODULEID,
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
VAccess::ERet SppAppRight::updateApplRight(SListProj  & pSDataSingle, CBString& strPrevRightname  )
{
	CBString            strSql;
	T_ListProjppAppRight * pCond2 = (T_ListProjppAppRight *)&(pSDataSingle.stListProj);
	T_ListCondppAppRight * pCond = (T_ListCondppAppRight *)&(pSDataSingle.stListCond);

	strSql.format("UPDATE "
						"ppappright "
					"SET "
						"rightname = '%s' "
					"WHERE "
						"applicationid = %d and moduleid = %d and rightname = '%s'",
						pCond2->RIGHTNAME, pCond->APPLICATIONID, 
						pCond->MODULEID, strPrevRightname.c_str());
	
		
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
VAccess::ERet SppAppRight::updateApplModRight(SListProj  & pSDataSingle , CBString& strPrevRightname )
{
	CBString            strSql;
	T_ListProjppAppRight * pCond2 = (T_ListProjppAppRight *)&(pSDataSingle.stListProj);
	T_ListCondppAppRight * pCond = (T_ListCondppAppRight *)&(pSDataSingle.stListCond);

	strSql.format("UPDATE "
						"ppappmodright "
					"SET "
						"rightname = '%s', description = '%s', description_en = '%s', "
						"description_fr = '%s', description_hr = '%s', category = %d "
					"WHERE "
						"applicationid = %d and moduleid = %d and rightname = '%s'",
						pCond2->RIGHTNAME, pCond2->RIGHTDESC, 
						pCond2->RIGHTDESC_EN, pCond2->RIGHTDESC_FR, pCond2->RIGHTDESC_HR, pCond2->CATEGORY, 
						pCond->APPLICATIONID, 
						pCond->MODULEID, strPrevRightname.c_str());

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
VAccess::ERet SppAppRight::updateAppModRight(SListProj  & pSDataSingle )
{
	CBString            strSql;
	T_ListProjppAppRight * pCond2 = (T_ListProjppAppRight *)&(pSDataSingle.stListProj);
	T_ListCondppAppRight * pCond = (T_ListCondppAppRight *)&(pSDataSingle.stListCond);

	strSql.format("UPDATE "
						"ppappmodright "
					"SET "
						"description = '%s', description_en = '%s', "
						"description_fr = '%s', description_hr = '%s', category = %d "
					"WHERE "
						"applicationid = %d and moduleid = %d and rightname = '%s'",
						pCond2->RIGHTDESC, 
						pCond2->RIGHTDESC_EN, pCond2->RIGHTDESC_FR, pCond2->RIGHTDESC_HR, pCond2->CATEGORY, 
						pCond->APPLICATIONID, 
						pCond->MODULEID, pCond2->RIGHTNAME);

			
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
VAccess::ERet SppAppRight::openCursor(VData & pSDataCond)
{
	if (VAccVdooCurs::openCursor(pSDataCond) != VAccess::RET_OK)
		return VAccess::RET_OK;

	long        lRet;
	CBString   strSql;

	SListProj * pCond = (SListProj * )&pSDataCond;

	DtStructStripLast(&(pCond->stListCond), STRUCDESC_LISTCONDPPAPPRIGHT);
			
	if(pCond->stListCond.CORAPPRIGHT == false && pCond->stListCond.CORGROUPRIGHT == false && pCond->stListCond.MODRIGHTS == false)
	{	//further rights
		strSql.format("SELECT "
						  "mr.rightname, "
						  "mr.description, "
						  "mr.description_en, "
						  "mr.description_fr, "
						  "mr.description_hr, "
						  "mr.category "
					"FROM ppappmodright mr "
					"WHERE mr.applicationid = %d and mr.moduleid = %d "
					"and mr.rightname NOT IN "
						"(SELECT "
						"ar.rightname "
						"FROM ppappright ar "
						"WHERE ar.applicationid = %d and ar.moduleid = %d "
						"and ar.groupid = %d and ar.userid = %d and ar.branchregionid = %d) ",
					pCond->stListCond.APPLICATIONID, pCond->stListCond.MODULEID,
					pCond->stListCond.APPLICATIONID, pCond->stListCond.MODULEID,
					pCond->stListCond.GROUPID, pCond->stListCond.USERID, 
					pCond->stListCond.BRANCHREGID);

		if ( pCond->stListCond.CATEGORY > RIGHT_CATEGORY_NONE )
		{
			CBString   strHelp;
			strHelp.format("and mr.category = %d ", pCond->stListCond.CATEGORY);
			strSql.append(strHelp.c_str());
		}

		strSql.append("ORDER BY mr.rightname");
	}
	else if (pCond->stListCond.CORAPPRIGHT == true && pCond->stListCond.CORGROUPRIGHT == false && pCond->stListCond.MODRIGHTS == false)
	{	//correlated rights	
		strSql.format("SELECT DISTINCT "
						  "mr.rightname, "
						  "mr.description, "
						  "mr.description_en, "
						  "mr.description_fr, "
						  "mr.description_hr, "
						  "mr.category "
					"FROM ppappmodright mr, ppappright ar "
					"WHERE ar.applicationid = %d and ar.moduleid = %d and "
					"ar.groupid = %d and ar.userid = %d and "
					"ar.branchregionid = %d and mr.rightname = ar.rightname "
					"and mr.applicationid = %d and mr.moduleid = %d ",
					pCond->stListCond.APPLICATIONID, pCond->stListCond.MODULEID, 
					pCond->stListCond.GROUPID, pCond->stListCond.USERID, 
					pCond->stListCond.BRANCHREGID, pCond->stListCond.APPLICATIONID, 
					pCond->stListCond.MODULEID);

		if ( pCond->stListCond.CATEGORY > RIGHT_CATEGORY_NONE )
		{
			CBString   strHelp;
			strHelp.format("and mr.category = %d ", pCond->stListCond.CATEGORY);
			strSql.append(strHelp.c_str());
		}

		strSql.append("ORDER BY mr.rightname");
	}
	else if (pCond->stListCond.CORAPPRIGHT == false && pCond->stListCond.CORGROUPRIGHT == true && pCond->stListCond.MODRIGHTS == false)
	{
		strSql.format("SELECT DISTINCT "
						  "mr.rightname, "
						  "mr.description, "
						  "mr.description_en, "
						  "mr.description_fr, "
						  "mr.description_hr, "
						  "mr.category "
					"FROM ppappmodright mr, ppappright ar "
					"WHERE ar.groupid IN "
						"(SELECT ug.groupid "
						"FROM ppcorappusergroup ug "
						"WHERE ug.applicationid = %d and ug.userid = %d) "
					"and ar.applicationid = %d and ar.moduleid = %d and "
					"ar.userid = 0 and "
					"ar.branchregionid = %d and mr.rightname = ar.rightname "
					"and mr.applicationid = %d and mr.moduleid = %d ",
					pCond->stListCond.APPLICATIONID, pCond->stListCond.USERID, 
					pCond->stListCond.APPLICATIONID, pCond->stListCond.MODULEID, 
					pCond->stListCond.BRANCHREGID, pCond->stListCond.APPLICATIONID, 
					pCond->stListCond.MODULEID);

		if ( pCond->stListCond.CATEGORY > RIGHT_CATEGORY_NONE )
		{
			CBString   strHelp;
			strHelp.format("and mr.category = %d ", pCond->stListCond.CATEGORY);
			strSql.append(strHelp.c_str());
		}

		strSql.append("ORDER BY mr.rightname");
	}
	else if (pCond->stListCond.MODRIGHTS == true && pCond->stListCond.CORAPPRIGHT == false && pCond->stListCond.CORGROUPRIGHT == false) 
	{
		strSql.format("SELECT DISTINCT "
						  "mr.rightname, "
						  "mr.description, "
						  "mr.description_en, "
						  "mr.description_fr, "
						  "mr.description_hr, "
						  "mr.category "
					"FROM ppappmodright mr "
					"WHERE mr.applicationid = %d and mr.moduleid = %d ",
					pCond->stListCond.APPLICATIONID, pCond->stListCond.MODULEID);

		if ( pCond->stListCond.CATEGORY > RIGHT_CATEGORY_NONE )
		{
			CBString   strHelp;
			strHelp.format("and mr.category = %d ", pCond->stListCond.CATEGORY);
			strSql.append(strHelp.c_str());
		}

		strSql.append("ORDER BY mr.rightname");
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
VAccess::ERet SppAppRight::fetch(VData & pSDataProj)
{
	long         lRet;
	T_ListProjppAppRight * pProj = (T_ListProjppAppRight *)&(((SListProj *)&pSDataProj)->stListProj);

	lRet = SqlBufferedFetch(m_hCursor, 1, 100, pProj, NULL); 

	switch(lRet)
	{
	case SQL_SUCCESS:
		DtStructStripLast(pProj, STRUCDESC_LISTPROJPPAPPRIGHT);
		return VAccess::RET_OK;

	case SQL_NOTFOUND:
		return VAccess::RET_NOTFOUND;

	default:
		SLog::ex().write(__FUNCTION__, VAccess::RET_ERROR, 
			        "fetch appright = %ld",
					lRet);
		assert(0);
		return VAccess::RET_ERROR;
	}
}

/*----------------------------------------------------------------------------*/
SppAppRight::SListProj::SListProj()
{
	clear();
}

/*----------------------------------------------------------------------------*/
bool SppAppRight::SListProj::operator == (const VData & r) const
{
	if (typeid(*this) != typeid(r))
        return false;

	return
	(
		(0 == memcmp(&stListCond  , 
		             &(((const SListProj &)r).stListCond  ), 
					 sizeof(T_ListCondppAppRight  )))
		&&
		(0 == memcmp(&stListProj, 
		             &(((const SListProj &)r).stListProj), 
					 sizeof(T_ListProjppAppRight)))
	) 
	? true : false;
}

/*----------------------------------------------------------------------------*/
void SppAppRight::SListProj::clear()
{
	memset(&stListCond, 0, sizeof(T_ListCondppAppRight));
	memset(&stListProj, 0, sizeof(T_ListProjppAppRight));
}