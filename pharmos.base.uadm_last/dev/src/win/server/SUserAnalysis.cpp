/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH										       */
/**   Date   : 06.10.2006                                                     */
/**                                                                           */
/******************************************************************************/

#include "srv.h"

#include "tbl_ppappmodule.h"
#include "tbl_ppuser.h"
#include "tbl_ppappmodright.h"
#include "tbl_ppbranch.h"
#include "tbl_ppapplication.h"
#include "struct_useranalysis.h"

#include "SUserAnalysis.h"

/*----------------------------------------------------------------------------*/
SUserAnalysis::SUserAnalysis() 
{
}

/*----------------------------------------------------------------------------*/
SUserAnalysis::~SUserAnalysis()
{
}

/*----------------------------------------------------------------------------*/
VAccess::ERet SUserAnalysis::openCursor(VData & pSDataCond)
{
	if (VAccVdooCurs::openCursor(pSDataCond) != VAccess::RET_OK)
		return VAccess::RET_OK;

	long        lRet;
	CBString   strSql;

	SListProj * pCond = (SListProj * )&pSDataCond;

	DtStructStripLast(&(pCond->stListCond), STRUCDESC_LISTCONDUSERANALYSIS);
			
	if (pCond->stListCond.BRANCHREGID == 0)
	{
			strSql.format(
					"SELECT "
						  "a.name, "
						  "m.name, "
						  "u.username, "
						  "u.surname, "
						  "u.forename, "
						  "reg.name, "
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
					"WHERE r.applicationid = %d and r.rightname = '%s' and r.moduleid = %d "
							"and r.userid = u.userid "
							"and r.applicationid = a.applicationid "
							"and r.moduleid = m.moduleid "
							"and m.applicationid = r.applicationid "
							"and r.branchregionid > 999 "
							"and r.branchregionid = reg.regionid "
							"and r.applicationid = amr.applicationid "
							"and r.moduleid = amr.moduleid "
							"and r.rightname = amr.rightname "
											
				"UNION "
					"SELECT "
						  "a.name, "
						  "m.name, "
						  "u.username, "
						  "u.surname, "
						  "u.forename, "
						  "br.name, "
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
					"WHERE r.applicationid = %d and r.rightname = '%s' and r.moduleid = %d "
							"and r.userid = u.userid "
							"and r.applicationid = a.applicationid "
							"and r.moduleid = m.moduleid "
							"and m.applicationid = r.applicationid "
							"and r.branchregionid < 1000 "
							"and r.branchregionid = br.branchid "
							"and r.applicationid = amr.applicationid "
							"and r.moduleid = amr.moduleid "
							"and r.rightname = amr.rightname "
				
				"UNION " 
					"SELECT "
						  "a.name, "
						  "m.name, "
						  "u.username, "
						  "u.surname, "
						  "u.forename, "
						  "br.name, "
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
					"WHERE c.applicationid = %d and c.userid = u.userid) "
						"and r.applicationid = %d and r.rightname = '%s' and r.moduleid = %d "
						"and r.applicationid = a.applicationid "
						"and r.moduleid = m.moduleid "
						"and m.applicationid = r.applicationid "
						"and r.branchregionid < 1000 "
						"and r.branchregionid = br.branchid "
						"and r.applicationid = amr.applicationid "
						"and r.moduleid = amr.moduleid "
						"and r.rightname = amr.rightname "

				"UNION " 
					"SELECT "
						  "a.name, "
						  "m.name, "
						  "u.username, "
						  "u.surname, "
						  "u.forename, "
						  "reg.name, "
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
						"WHERE c.applicationid = %d and c.userid = u.userid) "	
						"and r.applicationid = %d and r.rightname = '%s' and r.moduleid = %d "
						"and r.applicationid = a.applicationid "
						"and r.moduleid = m.moduleid "
						"and m.applicationid = r.applicationid "
						"and r.branchregionid > 999 "
						"and r.branchregionid = reg.regionid "
						"and r.applicationid = amr.applicationid "
						"and r.moduleid = amr.moduleid "
						"and r.rightname = amr.rightname "
					"ORDER BY 4, 5, 6"

					,pCond->stListCond.APPLICATIONID, pCond->stListCond.RIGHTNAME, pCond->stListCond.MODULEID,
					pCond->stListCond.APPLICATIONID, pCond->stListCond.RIGHTNAME, pCond->stListCond.MODULEID,
					pCond->stListCond.APPLICATIONID, pCond->stListCond.APPLICATIONID, pCond->stListCond.RIGHTNAME, pCond->stListCond.MODULEID,
					pCond->stListCond.APPLICATIONID, pCond->stListCond.APPLICATIONID, pCond->stListCond.RIGHTNAME, pCond->stListCond.MODULEID);
	}

	else //special branch / region
	{
			strSql.format(
					"SELECT "
						  "a.name, "
						  "m.name, "
						  "u.username, "
						  "u.surname, "
						  "u.forename, "
						  "reg.name, "
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
					"WHERE r.applicationid = %d and r.rightname = '%s' and r.moduleid = %d "
							"and r.userid = u.userid "
							"and r.applicationid = a.applicationid "
							"and r.moduleid = m.moduleid "
							"and m.applicationid = r.applicationid "
							"and r.branchregionid = %d "
							"and r.branchregionid = reg.regionid "
							"and r.applicationid = amr.applicationid "
							"and r.moduleid = amr.moduleid "
							"and r.rightname = amr.rightname "
				
				"UNION "
					"SELECT "
						  "a.name, "
						  "m.name, "
						  "u.username, "
						  "u.surname, "
						  "u.forename, "
						  "br.name, "
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
					"WHERE r.applicationid = %d and r.rightname = '%s' and r.moduleid = %d "
							"and r.userid = u.userid "
							"and r.applicationid = a.applicationid "
							"and r.moduleid = m.moduleid "
							"and m.applicationid = r.applicationid "
							"and r.branchregionid = %d "
							"and r.branchregionid = br.branchid "
							"and r.applicationid = amr.applicationid "
							"and r.moduleid = amr.moduleid "
							"and r.rightname = amr.rightname "
				
				"UNION "
					"SELECT "
						  "a.name, "
						  "m.name, "
						  "u.username, "
						  "u.surname, "
						  "u.forename, "
						  "br.name, "
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
					"WHERE c.applicationid = %d and c.userid = r.userid) "
						"and u.userid = r.userid "
						"and r.applicationid = %d and r.rightname = '%s' and r.moduleid = %d "
						"and r.applicationid = a.applicationid "
						"and r.moduleid = m.moduleid "
						"and m.applicationid = r.applicationid "
						"and r.branchregionid = %d "
						"and r.branchregionid = br.branchid "
						"and r.applicationid = amr.applicationid "
						"and r.moduleid = amr.moduleid "
						"and r.rightname = amr.rightname "

				"UNION "
					"SELECT "
						  "a.name, "
						  "m.name, "
						  "u.username, "
						  "u.surname, "
						  "u.forename, "
						  "reg.name, "
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
						"WHERE c.applicationid = %d and c.userid = r.userid) "	
						"and u.userid = r.userid "
						"and r.applicationid = %d and r.rightname = '%s' and r.moduleid = %d "
						"and r.applicationid = a.applicationid "
						"and r.moduleid = m.moduleid "
						"and m.applicationid = r.applicationid "
						"and r.branchregionid = %d "
						"and r.branchregionid = reg.regionid "
						"and r.applicationid = amr.applicationid "
						"and r.moduleid = amr.moduleid "
						"and r.rightname = amr.rightname "
					"ORDER BY 6, 4, 5"

					,pCond->stListCond.APPLICATIONID, pCond->stListCond.RIGHTNAME, pCond->stListCond.MODULEID, pCond->stListCond.BRANCHREGID,
					pCond->stListCond.APPLICATIONID, pCond->stListCond.RIGHTNAME, pCond->stListCond.MODULEID, pCond->stListCond.BRANCHREGID,
					pCond->stListCond.APPLICATIONID, pCond->stListCond.APPLICATIONID, pCond->stListCond.RIGHTNAME, pCond->stListCond.MODULEID, pCond->stListCond.BRANCHREGID,
					pCond->stListCond.APPLICATIONID, pCond->stListCond.APPLICATIONID, pCond->stListCond.RIGHTNAME, pCond->stListCond.MODULEID, pCond->stListCond.BRANCHREGID);
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
VAccess::ERet SUserAnalysis::fetch(VData & pSDataProj)
{
	long         lRet;
	T_ListProjUserAnalysis * pProj = (T_ListProjUserAnalysis *)&(((SListProj*)&pSDataProj)->stListProj);

	lRet = SqlBufferedFetch(m_hCursor, 1, 500, pProj, NULL); 

	switch(lRet)
	{
	case SQL_SUCCESS:
		DtStructStripLast(pProj, STRUCDESC_LISTPROJUSERANALYSIS);
		return VAccess::RET_OK;

	case SQL_NOTFOUND:
		return VAccess::RET_NOTFOUND;

	default:
		SLog::ex().write(__FUNCTION__, VAccess::RET_ERROR, 
			                "fetch user analysis = %ld",
							lRet);
		assert(0);
		return VAccess::RET_ERROR;
	}
}

/*----------------------------------------------------------------------------*/
SUserAnalysis::SListProj::SListProj()
{
	clear();
}

/*----------------------------------------------------------------------------*/
bool SUserAnalysis::SListProj::operator == (const VData & r) const
{
	if (typeid(*this) != typeid(r))
        return false;

	return
	(
		(0 == memcmp(&stListCond  , 
		             &(((const SListProj &)r).stListCond  ), 
					 sizeof(T_ListCondUserAnalysis  )))
		&&
		(0 == memcmp(&stListProj, 
		             &(((const SListProj &)r).stListProj), 
					 sizeof(T_ListProjUserAnalysis)))
	) 
	? true : false;
}

/*----------------------------------------------------------------------------*/
void SUserAnalysis::SListProj::clear()
{
	memset(&stListCond, 0, sizeof(T_ListCondUserAnalysis));
	memset(&stListProj, 0, sizeof(T_ListProjUserAnalysis));
}