/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH										       */
/**   Date   : 14.06.2004                                                     */
/**                                                                           */
/******************************************************************************/

#include "srv.h"

#include "tbl_ppappmodule.h"
#include "tbl_ppuser.h"
#include "tbl_ppappmodright.h"
#include "tbl_ppbranch.h"
#include "struct_rightanalysis.h"

#include "SRightAnalysis.h"

/*----------------------------------------------------------------------------*/
SRightAnalysis::SRightAnalysis() 
{
}

/*----------------------------------------------------------------------------*/
SRightAnalysis::~SRightAnalysis()
{
}

/*----------------------------------------------------------------------------*/
VAccess::ERet SRightAnalysis::openCursor(VData & pSDataCond)
{
	if (VAccVdooCurs::openCursor(pSDataCond) != VAccess::RET_OK)
		return VAccess::RET_OK;

	long        lRet;
	CBString   strSql;

	SListProj * pCond = (SListProj * )&pSDataCond;

	DtStructStripLast(&(pCond->stListCond), STRUCDESC_LISTCONDANALYSIS);
			
	if (pCond->stListCond.APPONLY == true && pCond->stListCond.USRONLY == true && pCond->stListCond.BRANCHREGID == 0)
	{
			strSql.format(
					"SELECT "
						  "a.name, "
						  "m.name, "
						  "u.username, "
						  "u.surname, "
						  "u.forename, "
						  "reg.name, "
						  "r.rightname, "
						  "amr.description, "
						  "amr.description_en, "
						  "amr.description_fr, "
						  "amr.description_hr "
					"FROM ppapplication a, "
							"ppappmodule m, "
							"ppregion reg, "
							"ppuser u, "
							"ppappright r, "
							"ppappmodright amr "
					"WHERE r.applicationid = %d and r.userid = %d "
							"and r.userid = u.userid "
							"and r.applicationid = a.applicationid "
							"and r.applicationid = amr.applicationid "
							"and r.moduleid = m.moduleid "
							"and r.moduleid = amr.moduleid "
							"and r.rightname = amr.rightname "
							"and m.applicationid = r.applicationid "
							"and r.branchregionid > 999 "
							"and r.branchregionid = reg.regionid "
				"UNION "
					"SELECT "
						  "a.name, "
						  "m.name, "
						  "u.username, "
						  "u.surname, "
						  "u.forename, "
						  "br.name, "
						  "r.rightname, "
						  "amr.description, "
						  "amr.description_en, "
						  "amr.description_fr, "
						  "amr.description_hr "
					"FROM ppapplication a, "
							"ppappmodule m, "
							"ppbranch br, "
							"ppuser u, "
							"ppappright r, "
							"ppappmodright amr "
					"WHERE r.applicationid = %d and r.userid = %d "
							"and r.userid = u.userid "
							"and r.applicationid = a.applicationid "
							"and r.applicationid = amr.applicationid "
							"and r.moduleid = m.moduleid "
							"and r.moduleid = amr.moduleid "
							"and r.rightname = amr.rightname "
							"and m.applicationid = r.applicationid "
							"and r.branchregionid < 1000 "
							"and r.branchregionid = br.branchid "
				"UNION "
					"SELECT "
						  "a.name, "
						  "m.name, "
						  "u.username, "
						  "u.surname, "
						  "u.forename, "
						  "br.name, "
						  "r.rightname, "
						  "amr.description, "
						  "amr.description_en, "
						  "amr.description_fr, "
						  "amr.description_hr "
					"FROM ppapplication a, "
							"ppuser u, "
							"ppappmodule m, "
							"ppappright r, "
							"ppbranch br, "
							"ppappmodright amr "
					"WHERE "
					"r.groupid IN "
					"(SELECT c.groupid "
					"FROM ppcorappusergroup c "
					"WHERE c.applicationid = %d and c.userid = %d) "
						"and u.userid = %d "
						"and r.applicationid = %d "
						"and r.applicationid = a.applicationid "
						"and r.applicationid = amr.applicationid "
						"and r.moduleid = m.moduleid "
						"and r.moduleid = amr.moduleid "
						"and r.rightname = amr.rightname "
						"and m.applicationid = r.applicationid "
						"and r.branchregionid < 1000 "
						"and r.branchregionid = br.branchid "

				"UNION "
					"SELECT "
						  "a.name, "
						  "m.name, "
						  "u.username, "
						  "u.surname, "
						  "u.forename, "
						  "reg.name, "
						  "r.rightname, "
						  "amr.description, "
						  "amr.description_en, "
						  "amr.description_fr, "
						  "amr.description_hr "
					"FROM ppapplication a, "
							"ppappmodule m, "
							"ppregion reg, "
							"ppuser u, "
							"ppappright r, "
							"ppappmodright amr "
					"WHERE "
						"r.groupid IN "
						"(SELECT c.groupid "
						"FROM ppcorappusergroup c "
						"WHERE c.applicationid = %d and c.userid = %d) "	
						"and u.userid = %d "
						"and r.applicationid = %d "
						"and r.applicationid = a.applicationid "
						"and r.applicationid = amr.applicationid "
						"and r.moduleid = m.moduleid "
						"and r.moduleid = amr.moduleid "
						"and r.rightname = amr.rightname "
						"and m.applicationid = r.applicationid "
						"and r.branchregionid > 999 "
						"and r.branchregionid = reg.regionid "
					"ORDER BY  2, 6, 3, 7"
					,pCond->stListCond.APPLICATIONID, pCond->stListCond.USERID,
					pCond->stListCond.APPLICATIONID, pCond->stListCond.USERID,
					pCond->stListCond.APPLICATIONID, pCond->stListCond.USERID,pCond->stListCond.USERID, pCond->stListCond.APPLICATIONID,
					pCond->stListCond.APPLICATIONID, pCond->stListCond.USERID,pCond->stListCond.USERID, pCond->stListCond.APPLICATIONID);
	}


	else if (pCond->stListCond.APPONLY == true && pCond->stListCond.USRONLY == false && pCond->stListCond.BRANCHREGID == 0)
	{
			strSql.format(
					"SELECT "
						  "a.name, "
						  "m.name, "
						  "u.username, "
						  "u.surname, "
						  "u.forename, "
						  "reg.name, "
						  "r.rightname, "
						  "amr.description, "
						  "amr.description_en, "
						  "amr.description_fr, "
						  "amr.description_hr "
					"FROM ppapplication a, "
							"ppappmodule m, "
							"ppregion reg, "
							"ppuser u, "
							"ppappright r, "
							"ppappmodright amr "
					"WHERE r.applicationid = %d "
							"and r.userid = u.userid "
							"and r.applicationid = a.applicationid "
							"and r.applicationid = amr.applicationid "
							"and r.moduleid = m.moduleid "
							"and r.moduleid = amr.moduleid "
							"and r.rightname = amr.rightname "
							"and m.applicationid = r.applicationid "
							"and r.branchregionid > 999 "
							"and r.branchregionid = reg.regionid "
							
				"UNION "
					"SELECT "
						  "a.name, "
						  "m.name, "
						  "u.username, "
						  "u.surname, "
						  "u.forename, "
						  "br.name, "
						  "r.rightname, "
						  "amr.description, "
						  "amr.description_en, "
						  "amr.description_fr, "
						  "amr.description_hr "
					"FROM ppapplication a, "
							"ppappmodule m, "
							"ppbranch br, "
							"ppuser u, "
							"ppappright r, "
							"ppappmodright amr "
					"WHERE r.applicationid = %d "
							"and r.userid = u.userid "
							"and r.applicationid = a.applicationid "
							"and r.applicationid = amr.applicationid "
							"and r.moduleid = m.moduleid "
							"and r.moduleid = amr.moduleid "
							"and r.rightname = amr.rightname "
							"and m.applicationid = r.applicationid "
							"and r.branchregionid < 1000 "
							"and r.branchregionid = br.branchid "
					
				"UNION "
					"SELECT "
						  "a.name, "
						  "m.name, "
						  "u.username, "
						  "u.surname, "
						  "u.forename, "
						  "br.name, "
						  "r.rightname, "
						  "amr.description, "
						  "amr.description_en, "
						  "amr.description_fr, "
						  "amr.description_hr "
					"FROM ppapplication a, "
							"ppappmodule m, "
							"ppbranch br, "
							"ppuser u, "
							"ppappright r, "
							"ppappmodright amr "
					"WHERE r.groupid IN "
						"(SELECT c.groupid FROM ppcorappusergroup c "
						"WHERE c.applicationid = %d) "
						"and u.userid IN "
						"(SELECT c.userid FROM ppcorappusergroup c "
						"WHERE c.applicationid = %d and c.groupid = r.groupid) "
						"and r.applicationid = %d "
						"and r.applicationid = a.applicationid "
						"and r.applicationid = amr.applicationid "
						"and r.moduleid = m.moduleid "
						"and r.moduleid = amr.moduleid "
						"and r.rightname = amr.rightname "
						"and m.applicationid = r.applicationid "
						"and r.branchregionid < 1000 "
						"and r.branchregionid = br.branchid "
					
				"UNION "
					"SELECT "
						  "a.name, "
						  "m.name, "
						  "u.username, "
						  "u.surname, "
						  "u.forename, "
						  "reg.name, "
						  "r.rightname, "
						  "amr.description, "
						  "amr.description_en, "
						  "amr.description_fr, "
						  "amr.description_hr "
					"FROM ppapplication a, "
							"ppappmodule m, "
							"ppregion reg, "
							"ppuser u, "
							"ppappright r, "
							"ppappmodright amr "
					"WHERE r.groupid IN "
						"(SELECT c.groupid FROM ppcorappusergroup c "
						"WHERE c.applicationid = %d) "
						"and u.userid IN "
						"(SELECT c.userid FROM ppcorappusergroup c "
						"WHERE c.applicationid = %d and c.groupid = r.groupid) "
						"and r.applicationid = %d "
						"and r.applicationid = a.applicationid "
						"and r.applicationid = amr.applicationid "
						"and r.moduleid = m.moduleid "
						"and r.moduleid = amr.moduleid "
						"and r.rightname = amr.rightname "
						"and m.applicationid = r.applicationid "
						"and r.branchregionid > 999 "
						"and r.branchregionid = reg.regionid "
		//			"ORDER BY  2, 6, 3, 7 "
					,pCond->stListCond.APPLICATIONID, 
					pCond->stListCond.APPLICATIONID,
					pCond->stListCond.APPLICATIONID, pCond->stListCond.APPLICATIONID, pCond->stListCond.APPLICATIONID,
					pCond->stListCond.APPLICATIONID, pCond->stListCond.APPLICATIONID, pCond->stListCond.APPLICATIONID );
	}
	if (pCond->stListCond.APPONLY == true && pCond->stListCond.USRONLY == true && pCond->stListCond.BRANCHREGID > 0)
	{
			strSql.format(
					"SELECT "
						  "a.name, "
						  "m.name, "
						  "u.username, "
						  "u.surname, "
						  "u.forename, "
						  "reg.name, "
						  "r.rightname, "
						  "amr.description, "
						  "amr.description_en, "
						  "amr.description_fr, "
						  "amr.description_hr "
					"FROM ppapplication a, "
							"ppappmodule m, "
							"ppregion reg, "
							"ppuser u, "
							"ppappright r, "
							"ppappmodright amr "
					"WHERE r.applicationid = %d and r.userid = %d "
							"and r.userid = u.userid "
							"and r.applicationid = a.applicationid "
							"and r.applicationid = amr.applicationid "
							"and r.moduleid = m.moduleid "
							"and r.moduleid = amr.moduleid "
							"and r.rightname = amr.rightname "
							"and m.applicationid = r.applicationid "
							"and r.branchregionid = %d "
							"and r.branchregionid = reg.regionid "
				"UNION "
					"SELECT "
						  "a.name, "
						  "m.name, "
						  "u.username, "
						  "u.surname, "
						  "u.forename, "
						  "br.name, "
						  "r.rightname, "
						  "amr.description, "
						  "amr.description_en, "
						  "amr.description_fr, "
						  "amr.description_hr "
					"FROM ppapplication a, "
							"ppappmodule m, "
							"ppbranch br, "
							"ppuser u, "
							"ppappright r, "
							"ppappmodright amr "
					"WHERE r.applicationid = %d and r.userid = %d "
							"and r.userid = u.userid "
							"and r.applicationid = a.applicationid "
							"and r.applicationid = amr.applicationid "
							"and r.moduleid = m.moduleid "
							"and r.moduleid = amr.moduleid "
							"and r.rightname = amr.rightname "
							"and m.applicationid = r.applicationid "
							"and r.branchregionid = %d "
							"and r.branchregionid = br.branchid "
				"UNION "
					"SELECT "
						  "a.name, "
						  "m.name, "
						  "u.username, "
						  "u.surname, "
						  "u.forename, "
						  "br.name, "
						  "r.rightname, "
						  "amr.description, "
						  "amr.description_en, "
						  "amr.description_fr, "
						  "amr.description_hr "
					"FROM ppapplication a, "
							"ppuser u, "
							"ppappmodule m, "
							"ppappright r, "
							"ppbranch br, "
							"ppappmodright amr "
					"WHERE "
					"r.groupid IN "
					"(SELECT c.groupid "
					"FROM ppcorappusergroup c "
					"WHERE c.applicationid = %d and c.userid = %d) "
						"and u.userid = %d "
						"and r.applicationid = %d "
						"and r.applicationid = a.applicationid "
						"and r.applicationid = amr.applicationid "
						"and r.moduleid = m.moduleid "
						"and r.moduleid = amr.moduleid "
						"and r.rightname = amr.rightname "
						"and m.applicationid = r.applicationid "
						"and r.branchregionid = %d "
						"and r.branchregionid = br.branchid "

				"UNION "
					"SELECT "
						  "a.name, "
						  "m.name, "
						  "u.username, "
						  "u.surname, "
						  "u.forename, "
						  "reg.name, "
						  "r.rightname, "
						  "amr.description, "
						  "amr.description_en, "
						  "amr.description_fr, "
						  "amr.description_hr "
					"FROM ppapplication a, "
							"ppappmodule m, "
							"ppregion reg, "
							"ppuser u, "
							"ppappright r, "
							"ppappmodright amr "
					"WHERE "
						"r.groupid IN "
						"(SELECT c.groupid "
						"FROM ppcorappusergroup c "
						"WHERE c.applicationid = %d and c.userid = %d) "	
						"and u.userid = %d "
						"and r.applicationid = %d "
						"and r.applicationid = a.applicationid "
						"and r.applicationid = amr.applicationid "
						"and r.moduleid = m.moduleid "
						"and r.moduleid = amr.moduleid "
						"and r.rightname = amr.rightname "
						"and m.applicationid = r.applicationid "
						"and r.branchregionid = %d "
						"and r.branchregionid = reg.regionid "
					"ORDER BY  2, 6, 3, 7"
					,pCond->stListCond.APPLICATIONID, pCond->stListCond.USERID, pCond->stListCond.BRANCHREGID, 
					pCond->stListCond.APPLICATIONID, pCond->stListCond.USERID,pCond->stListCond.BRANCHREGID,
					pCond->stListCond.APPLICATIONID, pCond->stListCond.USERID,pCond->stListCond.USERID, pCond->stListCond.APPLICATIONID,pCond->stListCond.BRANCHREGID,
					pCond->stListCond.APPLICATIONID, pCond->stListCond.USERID,pCond->stListCond.USERID, pCond->stListCond.APPLICATIONID,pCond->stListCond.BRANCHREGID);
	}
	else if (pCond->stListCond.APPONLY == true && pCond->stListCond.USRONLY == false && pCond->stListCond.BRANCHREGID > 0)
	{
			strSql.format(
					"SELECT "
						  "a.name, "
						  "m.name, "
						  "u.username, "
						  "u.surname, "
						  "u.forename, "
						  "reg.name, "
						  "r.rightname, "
						  "amr.description, "
						  "amr.description_en, "
						  "amr.description_fr, "
						  "amr.description_hr "
					"FROM ppapplication a, "
							"ppappmodule m, "
							"ppregion reg, "
							"ppuser u, "
							"ppappright r, "
							"ppappmodright amr "
					"WHERE r.applicationid = %d "
							"and r.userid = u.userid "
							"and r.applicationid = a.applicationid "
							"and r.applicationid = amr.applicationid "
							"and r.moduleid = m.moduleid "
							"and r.moduleid = amr.moduleid "
							"and r.rightname = amr.rightname "
							"and m.applicationid = r.applicationid "
							"and r.branchregionid = %d "
							"and r.branchregionid = reg.regionid "
							
				"UNION "
					"SELECT "
						  "a.name, "
						  "m.name, "
						  "u.username, "
						  "u.surname, "
						  "u.forename, "
						  "br.name, "
						  "r.rightname, "
						  "amr.description, "
						  "amr.description_en, "
						  "amr.description_fr, "
						  "amr.description_hr "
					"FROM ppapplication a, "
							"ppappmodule m, "
							"ppbranch br, "
							"ppuser u, "
							"ppappright r, "
							"ppappmodright amr "
					"WHERE r.applicationid = %d "
							"and r.userid = u.userid "
							"and r.applicationid = a.applicationid "
							"and r.applicationid = amr.applicationid "
							"and r.moduleid = m.moduleid "
							"and r.moduleid = amr.moduleid "
							"and r.rightname = amr.rightname "
							"and m.applicationid = r.applicationid "
							"and r.branchregionid = %d "
							"and r.branchregionid = br.branchid "
					
				"UNION "
					"SELECT "
						  "a.name, "
						  "m.name, "
						  "u.username, "
						  "u.surname, "
						  "u.forename, "
						  "br.name, "
						  "r.rightname, "
						  "amr.description, "
						  "amr.description_en, "
						  "amr.description_fr, "
						  "amr.description_hr "
					"FROM ppapplication a, "
							"ppappmodule m, "
							"ppbranch br, "
							"ppuser u, "
							"ppappright r, "
							"ppappmodright amr "
					"WHERE r.groupid IN "
						"(SELECT c.groupid FROM ppcorappusergroup c "
						"WHERE c.applicationid = %d) "
						"and u.userid IN "
						"(SELECT c.userid FROM ppcorappusergroup c "
						"WHERE c.applicationid = %d and c.groupid = r.groupid) "
						"and r.applicationid = %d "
						"and r.applicationid = a.applicationid "
						"and r.applicationid = amr.applicationid "
						"and r.moduleid = m.moduleid "
						"and r.moduleid = amr.moduleid "
						"and r.rightname = amr.rightname "
						"and m.applicationid = r.applicationid "
						"and r.branchregionid = %d "
						"and r.branchregionid = br.branchid "
					
				"UNION "
					"SELECT "
						  "a.name, "
						  "m.name, "
						  "u.username, "
						  "u.surname, "
						  "u.forename, "
						  "reg.name, "
						  "r.rightname, "
						  "amr.description, "
						  "amr.description_en, "
						  "amr.description_fr, "
						  "amr.description_hr "
					"FROM ppapplication a, "
							"ppappmodule m, "
							"ppregion reg, "
							"ppuser u, "
							"ppappright r, "
							"ppappmodright amr "
					"WHERE r.groupid IN "
						"(SELECT c.groupid FROM ppcorappusergroup c "
						"WHERE c.applicationid = %d) "
						"and u.userid IN "
						"(SELECT c.userid FROM ppcorappusergroup c "
						"WHERE c.applicationid = %d and c.groupid = r.groupid) "
						"and r.applicationid = %d "
						"and r.applicationid = a.applicationid "
						"and r.applicationid = amr.applicationid "
						"and r.moduleid = m.moduleid "
						"and r.moduleid = amr.moduleid "
						"and r.rightname = amr.rightname "
						"and m.applicationid = r.applicationid "
						"and r.branchregionid = %d "
						"and r.branchregionid = reg.regionid "
		//			"ORDER BY  2, 6, 3, 7 "
					,pCond->stListCond.APPLICATIONID, pCond->stListCond.BRANCHREGID,
					pCond->stListCond.APPLICATIONID, pCond->stListCond.BRANCHREGID,
					pCond->stListCond.APPLICATIONID, pCond->stListCond.APPLICATIONID, pCond->stListCond.APPLICATIONID, pCond->stListCond.BRANCHREGID,
					pCond->stListCond.APPLICATIONID, pCond->stListCond.APPLICATIONID, pCond->stListCond.APPLICATIONID, pCond->stListCond.BRANCHREGID );
	}
	/*else if (pCond->stListCond.APPONLY == false && pCond->stListCond.USRONLY == true)
	{
		//not used
	}*/
	
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
VAccess::ERet SRightAnalysis::fetch(VData & pSDataProj)
{
	long         lRet;
	T_ListProjAnalysis * pProj = (T_ListProjAnalysis *)&(((SListProj*)&pSDataProj)->stListProj);

	lRet = SqlBufferedFetch(m_hCursor, 1, 500, pProj, NULL); 

	switch(lRet)
	{
	case SQL_SUCCESS:
		DtStructStripLast(pProj, STRUCDESC_LISTPROJANALYSIS);
		return VAccess::RET_OK;

	case SQL_NOTFOUND:
		return VAccess::RET_NOTFOUND;

	default:
		SLog::ex().write(__FUNCTION__, VAccess::RET_ERROR, 
			                "fetch analysis = %ld",
							lRet);
		assert(0);
		return VAccess::RET_ERROR;
	}
}

/*----------------------------------------------------------------------------*/
SRightAnalysis::SListProj::SListProj()
{
	clear();
}

/*----------------------------------------------------------------------------*/
bool SRightAnalysis::SListProj::operator == (const VData & r) const
{
	if (typeid(*this) != typeid(r))
        return false;

	return
	(
		(0 == memcmp(&stListCond  , 
		             &(((const SListProj &)r).stListCond  ), 
					 sizeof(T_ListCondAnalysis  )))
		&&
		(0 == memcmp(&stListProj, 
		             &(((const SListProj &)r).stListProj), 
					 sizeof(T_ListProjAnalysis)))
	) 
	? true : false;
}

/*----------------------------------------------------------------------------*/
void SRightAnalysis::SListProj::clear()
{
	memset(&stListCond, 0, sizeof(T_ListCondAnalysis));
	memset(&stListProj, 0, sizeof(T_ListProjAnalysis));
}

