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
#include "struct_usergroupanalysis.h"

#include "SUserGroupAnalysis.h"

/*----------------------------------------------------------------------------*/
SUserGroupAnalysis::SUserGroupAnalysis()
{
}

/*----------------------------------------------------------------------------*/
SUserGroupAnalysis::~SUserGroupAnalysis()
{
}

/*----------------------------------------------------------------------------*/
VAccess::ERet SUserGroupAnalysis::openCursor(VData & pSDataCond)
{
	if (VAccVdooCurs::openCursor(pSDataCond) != VAccess::RET_OK)
		return VAccess::RET_OK;

	long        lRet;
	CBString   strSql;

	SListProj * pCond = (SListProj * )&pSDataCond;

	DtStructStripLast(&(pCond->stListCond), STRUCDESC_LISTCONDUSERGROUPANALYSIS);
			
	if (pCond->stListCond.APPLICATIONID > 0)
	{
			strSql.format(
                "SELECT ppa.name, "
                "       g.groupid AS GroupID, "
                "       g.name AS GroupName, "
                "       ppu.username AS UserName, "
                "       ppu.surname AS SurName, "
                "       ppu.forename AS ForeName, "
				"       ppu.country AS UserCountry, "
				"       ut.name AS UserType "
                "       FROM ppgroup g "
                "       INNER JOIN ppappright a on a.userid = 0 and a.groupid = g.groupid and a.applicationid = %d "
                "       INNER JOIN ppuser ppu on ppu.userid = a.userid "
				"       INNER JOIN ppusertype ut on ut.usertypeid = ppu.usertypeid "
                "       INNER JOIN ppapplication ppa on ppa.applicationid = %d "
                "UNION "
                "SELECT  ppa.name , "
                "        g.groupid AS GroupID, "
                "        g.name AS GroupName, "
                "        ppu.username AS UserName," 
                "        ppu.surname AS SurName, "
				"        ppu.forename AS ForeName,"
				"        ppu.country AS UserCountry,"
				"        ut.name AS UserType "
				"        FROM ppgroup g "
                " INNER JOIN ppcorappusergroup c on c.groupid = g.groupid and c.applicationid = %d "
                " INNER JOIN ppuser ppu on ppu.userid = c.userid "
				" INNER JOIN ppusertype ut on ut.usertypeid = ppu.usertypeid "
                " INNER JOIN ppapplication ppa on ppa.applicationid = %d "
                " order by ppu.surname "

                , pCond->stListCond.APPLICATIONID, pCond->stListCond.APPLICATIONID
                ,pCond->stListCond.APPLICATIONID, pCond->stListCond.APPLICATIONID
            );
	}
    else if (pCond->stListCond.USERID > 0)
    {
        strSql.format(
           " SELECT "
           "         ppa.name, "
            "        g.groupid, "
           "         g.name, "
           "         ppu.username, "
           "         ppu.surname, "
           "         ppu.forename, "
           "         ppu.country, "
		   "         ut.name "
           "         FROM 	ppgroup g "
           " INNER JOIN "
           "         ppappright a on a.userid = 0 and a.groupid = g.groupid "
           " INNER JOIN "
           "         ppuser ppu on ppu.userid = a.userid and ppu.userid = %d "
           " INNER JOIN "
		   "	     ppusertype ut on ut.usertypeid = ppu.usertypeid "
           " INNER JOIN "
           "         ppapplication ppa on ppa.applicationid = a.applicationid "
           " UNION "
           " SELECT "
           "         ppa.name, "
           "         g.groupid, "
           "         g.name, "
           "         ppu.username, "
           "         ppu.surname, "
           "         ppu.forename, "
		   "         ppu.country, "
		   "         ut.name AS UserType "
           " FROM 	ppgroup g "
           " INNER JOIN "
           "         ppcorappusergroup c on c.groupid = g.groupid "
           " INNER JOIN "
           "         ppuser ppu on ppu.userid = c.userid and ppu.userid = %d "
		   " INNER JOIN "
		   "	     ppusertype ut on ut.usertypeid = ppu.usertypeid "
           " INNER JOIN "
           "         ppapplication ppa on ppa.applicationid = c.applicationid "
           " ORDER BY ppu.surname "
               , pCond->stListCond.USERID, pCond->stListCond.USERID);

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
VAccess::ERet SUserGroupAnalysis::fetch(VData & pSDataProj)
{
	long         lRet;
	T_ListProjUserGroupAnalysis * pProj = (T_ListProjUserGroupAnalysis *)&(((SListProj*)&pSDataProj)->stListProj);

	lRet = SqlBufferedFetch(m_hCursor, 1, 500, pProj, NULL); 

	switch(lRet)
	{
	case SQL_SUCCESS:
		DtStructStripLast(pProj, STRUCDESC_LISTPROJUSERGROUPANALYSIS);
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
SUserGroupAnalysis::SListProj::SListProj()
{
	clear();
}

/*----------------------------------------------------------------------------*/
bool SUserGroupAnalysis::SListProj::operator == (const VData & r) const
{
	if (typeid(*this) != typeid(r))
        return false;

	return
	(
		(0 == memcmp(&stListCond  , 
		             &(((const SListProj &)r).stListCond  ), 
					 sizeof(T_ListCondUserGroupAnalysis  )))
		&&
		(0 == memcmp(&stListProj, 
		             &(((const SListProj &)r).stListProj), 
					 sizeof(T_ListProjUserGroupAnalysis)))
	) 
	? true : false;
}

/*----------------------------------------------------------------------------*/
void SUserGroupAnalysis::SListProj::clear()
{
	memset(&stListCond, 0, sizeof(T_ListCondUserGroupAnalysis));
	memset(&stListProj, 0, sizeof(T_ListProjUserGroupAnalysis));
}