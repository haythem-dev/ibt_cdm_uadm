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
#include "struct_userrightanalysis.h"

#include "SUserRightAnalysis.h"

/*----------------------------------------------------------------------------*/
SUserRightAnalysis::SUserRightAnalysis()
{
}

/*----------------------------------------------------------------------------*/
SUserRightAnalysis::~SUserRightAnalysis()
{
}

/*----------------------------------------------------------------------------*/
VAccess::ERet SUserRightAnalysis::openCursor(VData & pSDataCond)
{
	if (VAccVdooCurs::openCursor(pSDataCond) != VAccess::RET_OK)
		return VAccess::RET_OK;

	long        lRet;
	CBString   strSql;

	SListProj * pCond = (SListProj * )&pSDataCond;

	DtStructStripLast(&(pCond->stListCond), STRUCDESC_LISTCONDUSERRIGHTANALYSIS);
			
    if (pCond->stListCond.APPLICATIONID > 0 && pCond->stListCond.BRANCHID > 0 && pCond->stListCond.USERID <= 0)
    {
            strSql.format(
               "SELECT                          "
               "        a.name,                 "
               "        m.name,                 "
               "        u.username,             "
               "        u.surname,              "
               "        u.forename,             "
               "        reg.name,               "
               "        reg.regionid,           "
               "        r.rightname,            "
               "        amr.description,        "
               "        amr.description_en      "
               "FROM 	ppapplication a,        "
               "        ppappmodule m,          "
               "        ppregion reg,           "
               "        ppuser u,               "
               "        ppappright r,           "
               "        ppappmodright amr       "
               "WHERE   r.applicationid = %d and r.groupid = 0 and "
               "        r.userid = u.userid and r.applicationid = a.applicationid and "
               "        r.moduleid = m.moduleid and m.applicationid = r.applicationid and r.branchregionid > 999 and "
               "        r.branchregionid = %d and reg.regionid = %d and r.applicationid = amr.applicationid and "
               "        r.moduleid = amr.moduleid and r.rightname = amr.rightname "
           "UNION "
               "SELECT "
                      " a.name,            "
                      " m.name,            "
                      " u.username,        "
                      " u.surname,         "
                      " u.forename,        "
                      " br.name,           "
                      " br.branchid,       "
                      " r.rightname,       "
                      " amr.description,   "
                      " amr.description_en "
               " FROM   ppapplication a, "
               "         ppappmodule m, "
               "         ppbranch br, "
               "         ppuser u, "
               "         ppappright r, "
               "         ppappmodright amr "
               " WHERE   r.applicationid = %d and r.groupid = 0 and " 
               "         r.userid = u.userid and r.applicationid = a.applicationid and "
               "         r.moduleid = m.moduleid and m.applicationid = r.applicationid and r.branchregionid < 1000 and "
               "         r.branchregionid = %d and br.branchid = %d and r.applicationid = amr.applicationid and "
               "         r.moduleid = amr.moduleid and r.rightname = amr.rightname "
           " UNION "
               " SELECT  a.name, "
               "         m.name, "
               "         u.username, "
               "         u.surname, "
               "         u.forename, "
               "         br.name, "
               "         br.branchid, "
               "         r.rightname, "
               "         amr.description, "
               "         amr.description_en "
               " FROM    ppapplication a, "
               "         ppuser u, " 
               "         ppappmodule m, "
               "         ppappright r, "
               "         ppbranch br, " 
               "         ppappmodright amr " 
               " WHERE   r.groupid IN(SELECT c.groupid FROM ppcorappusergroup c WHERE c.applicationid = %d and c.userid = u.userid) and "
               "         r.applicationid = %d and r.groupid = 0 and r.applicationid = a.applicationid and " 
               "         r.moduleid = m.moduleid and m.applicationid = r.applicationid and r.branchregionid < 1000 and "
               "         r.branchregionid = %d and br.branchid = %d and r.applicationid = amr.applicationid and "
               "         r.moduleid = amr.moduleid and r.rightname = amr.rightname "
           " UNION "
               " SELECT "
               "         a.name, "
               "         m.name, "
               "         u.username, " 
               "         u.surname, "
               "         u.forename, "
               "         reg.name, "
               "         reg.regionid, "
               "         r.rightname, "
               "         amr.description, "
               "         amr.description_en "
               " FROM    ppapplication a, "
               "         ppappmodule m, "
               "         ppregion reg, "
               "         ppuser u, "
               "         ppappright r, "
               "         ppappmodright amr "
               " WHERE   r.groupid IN(SELECT c.groupid FROM ppcorappusergroup c WHERE c.applicationid = %d and c.userid = u.userid) and " 
               "         r.applicationid = %d and r.groupid = 0 and r.applicationid = a.applicationid and " 
               "         r.moduleid = m.moduleid and m.applicationid = r.applicationid and r.branchregionid > 999 and " 
               "         r.branchregionid = %d and reg.regionid = %d and r.applicationid = amr.applicationid and " 
               "         r.moduleid = amr.moduleid and r.rightname = amr.rightname "
               " ORDER BY 4, 5, 6 "

            ,pCond->stListCond.APPLICATIONID, pCond->stListCond.BRANCHID, pCond->stListCond.BRANCHID 
            ,pCond->stListCond.APPLICATIONID, pCond->stListCond.BRANCHID, pCond->stListCond.BRANCHID 
            ,pCond->stListCond.APPLICATIONID, pCond->stListCond.APPLICATIONID, pCond->stListCond.BRANCHID, pCond->stListCond.BRANCHID 
            ,pCond->stListCond.APPLICATIONID, pCond->stListCond.APPLICATIONID,pCond->stListCond.BRANCHID, pCond->stListCond.BRANCHID );

    }
    else if (pCond->stListCond.APPLICATIONID > 0)
	{
            strSql.format(
               "SELECT                          "
               "        a.name,                 "
               "        m.name,                 "
               "        u.username,             "
               "        u.surname,              "
               "        u.forename,             "
               "        reg.name,               "
               "        reg.regionid,           "
               "        r.rightname,            "
               "        amr.description,        "
               "        amr.description_en      "
               "FROM 	ppapplication a,        "
               "        ppappmodule m,          "
               "        ppregion reg,           "
               "        ppuser u,               "
               "        ppappright r,           "
               "        ppappmodright amr       "
               "WHERE   r.applicationid = %d and r.groupid = 0 and "
               "        r.userid = u.userid and r.applicationid = a.applicationid and "
               "        r.moduleid = m.moduleid and m.applicationid = r.applicationid and r.branchregionid > 999 and "
               "        r.branchregionid = reg.regionid and r.applicationid = amr.applicationid and "
               "        r.moduleid = amr.moduleid and r.rightname = amr.rightname "
           "UNION "
               "SELECT "
                      " a.name,            "
                      " m.name,            "
                      " u.username,        "
                      " u.surname,         "
                      " u.forename,        "
                      " br.name,           "
                      " br.branchid,       "
                      " r.rightname,       "
                      " amr.description,   "
                      " amr.description_en "
               " FROM   ppapplication a, "
               "         ppappmodule m, "
               "         ppbranch br, "
               "         ppuser u, "
               "         ppappright r, "
               "         ppappmodright amr "
               " WHERE   r.applicationid = %d and r.groupid = 0 and " 
               "         r.userid = u.userid and r.applicationid = a.applicationid and "
               "         r.moduleid = m.moduleid and m.applicationid = r.applicationid and r.branchregionid < 1000 and "
               "         r.branchregionid = br.branchid and r.applicationid = amr.applicationid and "
               "         r.moduleid = amr.moduleid and r.rightname = amr.rightname "
           " UNION "
               " SELECT  a.name, "
               "         m.name, "
               "         u.username, "
               "         u.surname, "
               "         u.forename, "
               "         br.name, "
               "         br.branchid, "
               "         r.rightname, "
               "         amr.description, "
               "         amr.description_en "
               " FROM    ppapplication a, "
               "         ppuser u, " 
               "         ppappmodule m, "
               "         ppappright r, "
               "         ppbranch br, " 
               "         ppappmodright amr " 
               " WHERE   r.groupid IN(SELECT c.groupid FROM ppcorappusergroup c WHERE c.applicationid = %d and c.userid = u.userid) and "
               "         r.applicationid = %d and r.groupid = 0 and r.applicationid = a.applicationid and " 
               "         r.moduleid = m.moduleid and m.applicationid = r.applicationid and r.branchregionid < 1000 and "
               "         r.branchregionid = br.branchid and r.applicationid = amr.applicationid and "
               "         r.moduleid = amr.moduleid and r.rightname = amr.rightname "
           " UNION "
               " SELECT "
               "         a.name, "
               "         m.name, "
               "         u.username, " 
               "         u.surname, "
               "         u.forename, "
               "         reg.name, "
               "         reg.regionid, "
               "         r.rightname, "
               "         amr.description, "
               "         amr.description_en "
               " FROM    ppapplication a, "
               "         ppappmodule m, "
               "         ppregion reg, "
               "         ppuser u, "
               "         ppappright r, "
               "         ppappmodright amr "
               " WHERE   r.groupid IN(SELECT c.groupid FROM ppcorappusergroup c WHERE c.applicationid = %d and c.userid = u.userid) and " 
               "         r.applicationid = %d and r.groupid = 0 and r.applicationid = a.applicationid and " 
               "         r.moduleid = m.moduleid and m.applicationid = r.applicationid and r.branchregionid > 999 and " 
               "         r.branchregionid = reg.regionid and r.applicationid = amr.applicationid and " 
               "         r.moduleid = amr.moduleid and r.rightname = amr.rightname "
               " ORDER BY 4, 5, 6 "

                    ,pCond->stListCond.APPLICATIONID, pCond->stListCond.APPLICATIONID, pCond->stListCond.APPLICATIONID 
                    , pCond->stListCond.APPLICATIONID, pCond->stListCond.APPLICATIONID, pCond->stListCond.APPLICATIONID
                    , pCond->stListCond.APPLICATIONID, pCond->stListCond.APPLICATIONID, pCond->stListCond.APPLICATIONID);
    }
    else if (pCond->stListCond.USERID > 0 && pCond->stListCond.BRANCHID > 0 && pCond->stListCond.APPLICATIONID <= 0)
    {
                strSql.format(
           "     SELECT "
           "             ppa.name As Appln, "
           "             ppam.name As Module, "
           "             ppu.username As UserName, "
           "             ppu.surname, ppu.forename, "
           "             ppreg.name As BrName, "
           "             ppreg.regionid As Brnch, "
           "             ppar.rightname As AssignedRight, "
           "             ppamr.description, "
           "             ppamr.description_en "
           "     FROM 	ppapplication ppa, "
           "             ppappmodule ppam, "
           "             ppregion ppreg, "
           "             ppuser ppu, "
           "             ppappright ppar, "
           "             ppappmodright ppamr "
           "             WHERE   ppu.userid = %d and ppar.groupid = 0 and "
           "             ppar.userid = ppu.userid and ppar.applicationid = ppa.applicationid and "
           "             ppar.moduleid = ppam.moduleid and ppam.applicationid = ppar.applicationid and ppar.branchregionid > 999 and "
           "             ppar.branchregionid = %d and ppreg.regionid = %d and ppar.applicationid = ppamr.applicationid and "
           "             ppar.moduleid = ppamr.moduleid and ppar.rightname = ppamr.rightname "
           " UNION " 
           "     SELECT  ppa.name As Appln, "
           "             ppam.name As Module, "
           "             ppu.username  As UserName, "
           "             ppu.surname, ppu.forename, "
           "             ppbr.name  As BrName, "
           "             ppbr.branchid As Brnch, "
           "             ppar.rightname As AssignedRight, "
           "             ppamr.description, "
           "             ppamr.description_en "
           "             FROM   ppapplication ppa, "
           "             ppappmodule ppam, "
           "             ppbranch ppbr, "
           "             ppuser ppu, "
           "             ppappright ppar, "
           "             ppappmodright ppamr "
           "     WHERE   ppu.userid = %d and ppar.groupid = 0 and ppar.userid = ppu.userid and "
           "             ppar.applicationid = ppa.applicationid and ppar.moduleid = ppam.moduleid and "
           "             ppam.applicationid = ppar.applicationid and ppar.branchregionid < 1000 and "
           "             ppar.branchregionid = %d and ppbr.branchid = %d and ppar.applicationid = ppamr.applicationid and "
           "             ppar.moduleid = ppamr.moduleid and ppar.rightname = ppamr.rightname "
           " UNION "
           "     SELECT  a.name As Appln, "
           "             m.name As Module, "
           "             u.username As UserName, "
           "             u.surname, u.forename, "
           "             br.name As BrName, "
           "             br.branchid As Brnch, "
           "             r.rightname As AssignedRight, "
           "             amr.description, "
           "             amr.description_en "
           "     FROM    ppapplication a, "
           "             ppuser u, "
           "             ppappmodule m, "
           "             ppappright r, "
           "             ppbranch br, "
           "             ppappmodright amr "
           "     WHERE   r.groupid IN(SELECT c.groupid FROM ppcorappusergroup c WHERE c.applicationid = r.applicationid and c.userid = u.userid) and "
           "             r.userid = %d and r.groupid = 0 and r.applicationid = a.applicationid and "
           "             r.moduleid = m.moduleid and m.applicationid = r.applicationid and r.branchregionid < 1000 and "
           "             r.branchregionid = %d and br.branchid = %d and r.applicationid = amr.applicationid and "
           "             r.moduleid = amr.moduleid and r.rightname = amr.rightname "
           " UNION "
           "     SELECT  a.name As Appln, "
           "             m.name As Module, "
           "             u.username As UserName, "
           "             u.surname, u.forename, "
           "             reg.name As BrName, "
           "             reg.regionid As Brnch, "
           "             r.rightname As AssignedRight, "
           "             amr.description, "
           "             amr.description_en "
           "     FROM    ppapplication a, "
           "             ppappmodule m, "
           "             ppregion reg, "
           "             ppuser u, "
           "             ppappright r, "
           "             ppappmodright amr "
           "     WHERE   r.groupid IN(SELECT c.groupid FROM ppcorappusergroup c WHERE c.applicationid = r.applicationid and c.userid = u.userid) and "
           "             r.userid = %d and r.groupid = 0 and r.applicationid = a.applicationid and "
           "             r.moduleid = m.moduleid and m.applicationid = r.applicationid and r.branchregionid > 999 and "
           "             r.branchregionid = %d and reg.regionid = %d and r.applicationid = amr.applicationid and r.moduleid = amr.moduleid and "
           "             r.rightname = amr.rightname "
           "     ORDER BY 7 "
            ,pCond->stListCond.USERID ,pCond->stListCond.BRANCHID, pCond->stListCond.BRANCHID,
            pCond->stListCond.USERID, pCond->stListCond.BRANCHID, pCond->stListCond.BRANCHID,
            pCond->stListCond.USERID, pCond->stListCond.BRANCHID, pCond->stListCond.BRANCHID,
            pCond->stListCond.USERID, pCond->stListCond.BRANCHID, pCond->stListCond.BRANCHID
        );
    }
    else if (pCond->stListCond.USERID > 0)
    {
        strSql.format(
           "     SELECT "
           "             ppa.name As Appln, "
           "             ppam.name As Module, "
           "             ppu.username As UserName, "
           "             ppu.surname, ppu.forename, "
           "             ppreg.name As BrName, "
           "             ppreg.regionid As Brnch, "
           "             ppar.rightname As AssignedRight, "
           "             ppamr.description, "
           "             ppamr.description_en "
           "     FROM 	ppapplication ppa, "
           "             ppappmodule ppam, "
           "             ppregion ppreg, "
           "             ppuser ppu, "
           "             ppappright ppar, "
           "             ppappmodright ppamr "
           "             WHERE   ppu.userid = %d and ppar.groupid = 0 and "
           "             ppar.userid = ppu.userid and ppar.applicationid = ppa.applicationid and "
           "             ppar.moduleid = ppam.moduleid and ppam.applicationid = ppar.applicationid and ppar.branchregionid > 999 and "
           "             ppar.branchregionid = ppreg.regionid and ppar.applicationid = ppamr.applicationid and "
           "             ppar.moduleid = ppamr.moduleid and ppar.rightname = ppamr.rightname "
           " UNION " 
           "     SELECT  ppa.name As Appln, "
           "             ppam.name As Module, "
           "             ppu.username  As UserName, "
           "             ppu.surname, ppu.forename, "
           "             ppbr.name  As BrName, "
           "             ppbr.branchid As Brnch, "
           "             ppar.rightname As AssignedRight, "
           "             ppamr.description, "
           "             ppamr.description_en "
           "             FROM   ppapplication ppa, "
           "             ppappmodule ppam, "
           "             ppbranch ppbr, "
           "             ppuser ppu, "
           "             ppappright ppar, "
           "             ppappmodright ppamr "
           "     WHERE   ppu.userid = %d and ppar.groupid = 0 and ppar.userid = ppu.userid and "
           "             ppar.applicationid = ppa.applicationid and ppar.moduleid = ppam.moduleid and "
           "             ppam.applicationid = ppar.applicationid and ppar.branchregionid < 1000 and "
           "             ppar.branchregionid = ppbr.branchid and ppar.applicationid = ppamr.applicationid and "
           "             ppar.moduleid = ppamr.moduleid and ppar.rightname = ppamr.rightname "
           " UNION "
           "     SELECT  a.name As Appln, "
           "             m.name As Module, "
           "             u.username As UserName, "
           "             u.surname, u.forename, "
           "             br.name As BrName, "
           "             br.branchid As Brnch, "
           "             r.rightname As AssignedRight, "
           "             amr.description, "
           "             amr.description_en "
           "     FROM    ppapplication a, "
           "             ppuser u, "
           "             ppappmodule m, "
           "             ppappright r, "
           "             ppbranch br, "
           "             ppappmodright amr "
           "     WHERE   r.groupid IN(SELECT c.groupid FROM ppcorappusergroup c WHERE c.applicationid = r.applicationid and c.userid = u.userid) and "
           "             r.userid = %d and r.groupid = 0 and r.applicationid = a.applicationid and "
           "             r.moduleid = m.moduleid and m.applicationid = r.applicationid and r.branchregionid < 1000 and "
           "             r.branchregionid = br.branchid and r.applicationid = amr.applicationid and "
           "             r.moduleid = amr.moduleid and r.rightname = amr.rightname "
           " UNION "
           "     SELECT  a.name As Appln, "
           "             m.name As Module, "
           "             u.username As UserName, "
           "             u.surname, u.forename, "
           "             reg.name As BrName, "
           "             reg.regionid As Brnch, "
           "             r.rightname As AssignedRight, "
           "             amr.description, "
           "             amr.description_en "
           "     FROM    ppapplication a, "
           "             ppappmodule m, "
           "             ppregion reg, "
           "             ppuser u, "
           "             ppappright r, "
           "             ppappmodright amr "
           "     WHERE   r.groupid IN(SELECT c.groupid FROM ppcorappusergroup c WHERE c.applicationid = r.applicationid and c.userid = u.userid) and "
           "             r.userid = %d and r.groupid = 0 and r.applicationid = a.applicationid and "
           "             r.moduleid = m.moduleid and m.applicationid = r.applicationid and r.branchregionid > 999 and "
           "             r.branchregionid = reg.regionid and r.applicationid = amr.applicationid and r.moduleid = amr.moduleid and "
           "             r.rightname = amr.rightname "
           "     ORDER BY 7 "
            ,pCond->stListCond.USERID , pCond->stListCond.USERID, pCond->stListCond.USERID, pCond->stListCond.USERID
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
VAccess::ERet SUserRightAnalysis::fetch(VData & pSDataProj)
{
	long         lRet;
	T_ListProjUserRightAnalysis * pProj = (T_ListProjUserRightAnalysis *)&(((SListProj*)&pSDataProj)->stListProj);

	lRet = SqlBufferedFetch(m_hCursor, 1, 500, pProj, NULL); 

	switch(lRet)
	{
	case SQL_SUCCESS:
		DtStructStripLast(pProj, STRUCDESC_LISTPROJUSERRIGHTANALYSIS);
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
SUserRightAnalysis::SListProj::SListProj()
{
	clear();
}

/*----------------------------------------------------------------------------*/
bool SUserRightAnalysis::SListProj::operator == (const VData & r) const
{
	if (typeid(*this) != typeid(r))
        return false;

	return
	(
		(0 == memcmp(&stListCond  , 
		             &(((const SListProj &)r).stListCond  ), 
					 sizeof(T_ListCondUserRightAnalysis  )))
		&&
		(0 == memcmp(&stListProj, 
		             &(((const SListProj &)r).stListProj), 
					 sizeof(T_ListProjUserRightAnalysis )))
	) 
	? true : false;
}

/*----------------------------------------------------------------------------*/
void SUserRightAnalysis::SListProj::clear()
{
	memset(&stListCond, 0, sizeof(T_ListCondUserRightAnalysis));
	memset(&stListProj, 0, sizeof(T_ListProjUserRightAnalysis));
} 