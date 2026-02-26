/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH										       */
/**   Date   : 25.05.2004                                                     */
/**                                                                           */
/******************************************************************************/

#include "srv.h"
#include "defines.h"
#include <libscmlogin/libscmlogin.h>

#include "tbl_ppuser.h"
#include "struct_user.h"

#include "SppUser.h"


// selectList	: SELECT u.userid, u.username, u.surname, u.forename
//					FROM ppuser u ORDER BY u.surname


// selectSingle : SELECT u.password, u.description
//							u.mailuser, u.emailuser, u.changepw, u.activ, u.lastlogin
//				  FROM ppuser u 
//                WHERE u.userid = %ld

/*----------------------------------------------------------------------------*/
SppUser::SppUser() 
{
}

/*----------------------------------------------------------------------------*/
SppUser::~SppUser()
{
}

/*----------------------------------------------------------------------------*/
VAccess::ERet SppUser::selectID	(CBString &strUsername, long & iUserid)
{
	CBString            strSql;

	strSql.format("SELECT "
						"u.userid "
					"FROM ppuser u "
                   "WHERE u.username = '%s'",
				   strUsername.c_str());

    long lRet = SqlRead((char *)strSql.c_str(), &iUserid, NULL);

	switch (lRet)
	{
	case SQL_SUCCESS:
		return VAccess::RET_OK;

	case SQL_NOTFOUND:
		return VAccess::RET_NOTFOUND;

	default:
		SLog::ex().write(__FUNCTION__, VAccess::RET_ERROR, 
			                      "unique select (<%s>) = %ld",
								  strSql.c_str(),
								  lRet);
		assert(0);
		return VAccess::RET_ERROR;
	}
}

VAccess::ERet SppUser::deactivateUserAndSetInactive(SComplSingle& pSDataSingle)
{
	CBString            strSql;
	T_ListProjppUser* pCond = (T_ListProjppUser*)&(pSDataSingle.stSingleCond);
	T_SingleProjppUser* pProj = (T_SingleProjppUser*)&(pSDataSingle.stSingleProj);

	strSql.format("UPDATE "
		"ppuser "
		"SET "
		"activ = 0 "
		"WHERE "
		"userid = %d",
		pCond->USERID);

	long lRet = SqlExecuteImm((char*)strSql.c_str());

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
VAccess::ERet SppUser::deleteSingle(SComplSingle  & pSDataSingle )
{
	CBString            strSql;
	T_ListProjppUser   * pCond = (T_ListProjppUser *)&(pSDataSingle.stSingleCond);
	
	strSql.format("DELETE "
					"FROM ppuser "
					"WHERE userid = %d",
					pCond->USERID);

		long lRet = SqlExecuteImm((char *)strSql.c_str());

	switch (lRet)
	{
	case SQL_SUCCESS:
		//log to see who deleted a user
		SLog::ex().write(__FUNCTION__, VAccess::RET_OK, 
			": user %d deleted by operator '%s'",  
			pCond->USERID,
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
VAccess::ERet SppUser::SelTestForUpd(SComplSingle  & pSDataSingle )
{
	CBString         strSql;
	T_ListProjppUser   * pCond = (T_ListProjppUser *)&(pSDataSingle.stSingleCond);
	T_SingleProjppUser * pProj = (T_SingleProjppUser *)&(pSDataSingle.stSingleProj);

	
	strSql.format("SELECT "
						  "u.userid,"
						  "u.username, "
						  "u.surname, "
						  "u.forename, "
						  "u.country, "
						  "ut.name "
                   "FROM ppuser u, ppusertype ut "
				   "WHERE u.userid = %d AND u.usertypeid = ut.usertypeid",
				   pCond->USERID);
				  

	long lRet = SqlRead((char *)strSql.c_str(), pCond, NULL);

	if (lRet == SQL_SUCCESS)
	{
		DtStructStripLast(pCond, STRUCDESC_LISTPROJPPUSER);

		strSql.format("SELECT "
						"u.password,"
						"u.description,"
                        "u.mailuser,"
						"u.emailuser,"
						"u.changepw,"
						"u.activ,"
						"u.lastlogin, "
						"u.ntuser, "
						"u.lastpwchange, "
						"u.reference, "
						"u.external, "
						"u.pwd_enc, "
						"u.country, "
						"u.usertypeid "
                   "FROM ppuser u "
                   "WHERE u.userid = %d",
				   pCond->USERID);
				  

		long lRet = SqlRead((char *)strSql.c_str(), pProj, NULL);

		DtStructStripLast(pProj, STRUCDESC_SINGLEPROJPPUSER);
	}

	switch (lRet)
	{
	case SQL_SUCCESS:
		return VAccess::RET_OK;

	case SQL_NOTFOUND:
		return VAccess::RET_NOTFOUND;

	default:
		SLog::ex().write(__FUNCTION__, VAccess::RET_ERROR, 
			                      "unique select (<%s>) = %ld",
								  strSql.c_str(),
								  lRet);
		assert(0);
		return VAccess::RET_ERROR;
	}
}

/*----------------------------------------------------------------------------*/
VAccess::ERet SppUser::deleteFromGroup(SComplSingle  & pSDataSingle )
{
	CBString            strSql;
	T_ListProjppUser   * pCond = (T_ListProjppUser *)&(pSDataSingle.stSingleCond);
	
	strSql.format("DELETE "
					"FROM ppcorappusergroup "
					"WHERE userid = %d",
					pCond->USERID);

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
VAccess::ERet SppUser::deleteAppRight(SComplSingle  & pSDataSingle )
{
	CBString            strSql;
	T_ListProjppUser   * pCond = (T_ListProjppUser *)&(pSDataSingle.stSingleCond);
	
	strSql.format("DELETE "
					"FROM ppappright "
					"WHERE userid = %d",
					pCond->USERID);

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
VAccess::ERet SppUser::deletePrevious(SComplSingle  & pSDataSingle )
{
	CBString            strSql;
	T_ListProjppUser   * pCond = (T_ListProjppUser *)&(pSDataSingle.stSingleCond);
	
	strSql.format("DELETE "
					"FROM pploginprevious "
					"WHERE userid = %d",
					pCond->USERID);

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
VAccess::ERet SppUser::updateSingle(SComplSingle  & pSDataSingle )
{
	CBString            strSql;
	T_ListProjppUser   * pCond = (T_ListProjppUser *)&(pSDataSingle.stSingleCond);
	T_SingleProjppUser * pProj = (T_SingleProjppUser *)&(pSDataSingle.stSingleProj);


	strSql.format("UPDATE "
						"ppuser "
					"SET "
						"username = '%s', password = '%s', surname = '%s', "
						"forename = '%s', description = '%s', mailuser = '%s', "
						"emailuser = '%s', changepw = %d, activ = %d , lastlogin = %d, "
						"ntuser = '%s', lastpwchange = %ld, reference = '%s', external = %d, "
						"pwd_enc = '%s', country = '%s', usertypeid = %d "
					"WHERE "
						"userid = %d",
					pCond->USERNAME, pProj->PASSWORD, pCond->SURNAME, 
					pCond->FORENAME, pProj->DESCRIPTION, pProj->MAILUSER, 
					pProj->EMAILUSER, pProj->CHANGEPW, pProj->ACTIV, pProj->LASTLOGIN,
					pProj->NTUSER, pProj->LASTPWCHANGE, pProj->REFERENCE,pProj->EXTERNAL, 
					pProj->PWD_ENC, pProj->COUNTRY,pProj->USERTYPEID, pCond->USERID);
	
	
	long lRet = SqlUpdateCur( m_UpdHandle,
		&(pCond->USERNAME), &(pProj->PASSWORD), &(pCond->SURNAME), 
		&(pCond->FORENAME), &(pProj->DESCRIPTION), &(pProj->MAILUSER), 
		&(pProj->EMAILUSER), &(pProj->CHANGEPW), &(pProj->ACTIV),&(pProj->LASTLOGIN), &(pProj->NTUSER),&(pProj->LASTPWCHANGE), 
		&(pProj->REFERENCE), &(pProj->EXTERNAL), &(pProj->PWD_ENC), &(pProj->COUNTRY), &(pProj->USERTYPEID), NULL);
	
	switch (lRet)
	{
	case SQL_SUCCESS: 
		SqlCloseCs(m_UpdHandle);
		SqlCommitWork();
		SSqlStat::ex().setStat(strSql);
		SLog::ex().write(__FUNCTION__, VAccess::RET_OK, 
			                    ": userid '%d' UNLOCKED after updating",
								pCond->USERID);

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
VAccess::ERet SppUser::insertSingle(SComplSingle  & pSDataSingle )
{
	CBString            strSql;
	T_ListProjppUser   * pCond = (T_ListProjppUser *)&(pSDataSingle.stSingleCond);
	T_SingleProjppUser * pProj = (T_SingleProjppUser *)&(pSDataSingle.stSingleProj);


	strSql.format("INSERT "
					"INTO ppuser "
					"(username, password, surname, forename, description, mailuser, emailuser, "
					"changepw, activ, ntuser, reference, external, pwd_enc, country, usertypeid ) "
					"VALUES ('%s', '%s', '%s', '%s', '%s', '%s', "
					"'%s', %d, %d, '%s', '%s', %d, '%s', '%s', %d )",
					pCond->USERNAME, pProj->PASSWORD, pCond->SURNAME, pCond->FORENAME,
					pProj->DESCRIPTION, pProj->MAILUSER, pProj->EMAILUSER, pProj->CHANGEPW,
					pProj->ACTIV, pProj->NTUSER, pProj->REFERENCE, pProj->EXTERNAL,
					pProj->PWD_ENC, pProj->COUNTRY, pProj->USERTYPEID);
	
		
	long lRet = SqlExecuteImm((char *)strSql.c_str());

	switch (lRet)
	{
	case SQL_SUCCESS:
		SSqlStat::ex().setStat(strSql);
		//log to see who inserted a user
		SLog::ex().write(__FUNCTION__, VAccess::RET_OK, 
			": new user '%s' inserted by operator '%s'",  
			pCond->USERNAME,
			scmod::ppBLoginData::ex().GetUsername().c_str());
		
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
VAccess::ERet SppUser::selectSingle(SComplSingle  & pSDataSingle, bool bLock)
{
	CBString            strSql;
	T_ListProjppUser   * pCond = (T_ListProjppUser *)&(pSDataSingle.stSingleCond);
	T_SingleProjppUser * pProj = (T_SingleProjppUser *)&(pSDataSingle.stSingleProj);

	// separate the load of the LIST and SINGLE as its containing different fields
	// and the SINGLE part can be locked for update
	// load LIST part
	strSql.format("SELECT "
						  "u.userid,"
						  "u.username, "
						  "u.surname, "
						  "u.forename, "
						  "u.country, "
						  "ut.name "
				   "FROM ppuser u, ppusertype ut "
				   "WHERE u.userid = %ld AND u.usertypeid = ut.usertypeid ",
					pCond->USERID);
	long lRet = SqlRead((char*)strSql.c_str(), pCond, NULL);

	// load SINGLE part
	if ( bLock == true )
	{
		strSql.format("SELECT "
						"u.username,"
                        "u.password,"
						"u.surname,"
						"u.forename,"
						"u.description,"
                        "u.mailuser,"
						"u.emailuser,"
						"u.changepw,"
						"u.activ,"
						"u.lastlogin, "
						"u.ntuser, "
						"u.lastpwchange, "
						"u.reference, "
						"u.external, "
						"u.pwd_enc, "
						"u.country, "
						"u.usertypeid "
                   "FROM ppuser u "
                   "WHERE u.userid = %ld "
				   "FOR UPDATE",
				   pCond->USERID);
	}
	else
	{
		strSql.format("SELECT "
						"u.username,"
                        "u.password,"
						"u.surname,"
						"u.forename,"
						"u.description,"
                        "u.mailuser,"
						"u.emailuser,"
						"u.changepw,"
						"u.activ,"
						"u.lastlogin, "
						"u.ntuser, "
						"u.lastpwchange, "
						"u.reference, "
						"u.external, "
						"u.pwd_enc, "
						"u.country, "
						"u.usertypeid "
                   "FROM ppuser u "
                   "WHERE u.userid = %ld "
				   ,pCond->USERID);
	}
	
	lRet = SqlBeginWork();/* Transaktion starten, einzelnen Datensatz für Update sperren */ 
	m_UpdHandle = -1;
	lRet = SqlExecuteImm( "set lock mode to wait 5" ); 
    if (lRet == VAccess::RET_OK)
	{
		lRet = SqlOpenCs(  &m_UpdHandle, (char *)strSql.c_str() ); 
	}
	if (lRet == VAccess::RET_OK)
	{
		lRet = SqlFetch( m_UpdHandle, 1, 
				pCond->USERNAME, pProj->PASSWORD, pCond->SURNAME, pCond->FORENAME,
				pProj->DESCRIPTION, pProj->MAILUSER, pProj->EMAILUSER, 
				&(pProj->CHANGEPW),	&(pProj->ACTIV), &(pProj->LASTLOGIN), &(pProj->NTUSER), &(pProj->LASTPWCHANGE), 
				pProj->REFERENCE, &(pProj->EXTERNAL), &(pProj->PWD_ENC), &(pProj->COUNTRY), &(pProj->USERTYPEID), NULL);
	}
	
	switch (lRet) 
	{
	case SQL_SUCCESS: 
		if (bLock == true)
		{
			SLog::ex().write(__FUNCTION__, VAccess::RET_OK, 
			                    ": userid '%d' LOCKED FOR UPDATE by User '%s'",
								pCond->USERID,
								scmod::ppBLoginData::ex().GetUsername().c_str());
		}
		DtStructStripLast(pCond, STRUCDESC_LISTPROJPPUSER);
		DtStructStripLast(pProj, STRUCDESC_SINGLEPROJPPUSER);
		return VAccess::RET_OK;

	case SQL_NOTFOUND:
		return VAccess::RET_NOTFOUND;

	case SQL_LOCKED:
		SLog::ex().write(__FUNCTION__, VAccess::RET_OK, 
			                    ": User '%s' tried to work on locked userid '%d'",
								scmod::ppBLoginData::ex().GetUsername().c_str(),
								pCond->USERID);

		return VAccess::RET_LOCKED;

	default:
		SLog::ex().write(__FUNCTION__, VAccess::RET_ERROR, 
			                      "select-fetch-opencursor (<%s>) = %ld",
								  strSql.c_str(),
								  lRet);
		assert(0);
		return VAccess::RET_ERROR;
	}
}

/*----------------------------------------------------------------------------*/
VAccess::ERet SppUser::openCursor(VData & pSDataCond)
{
	if (VAccVdooCurs::openCursor(pSDataCond) != VAccess::RET_OK)
		return VAccess::RET_OK;

	long        lRet;
	CBString   strSql;

	SListProj * pCond = (SListProj  *)&pSDataCond;

	if (pCond->stListCond.APPLICATIONID == 0 && pCond->stListCond.GROUPID == 0 && pCond->stListCond.CORUSRGRP == false) 
	{ //alle Benutzer laden
		DtStructStripLast(&(pCond->stListCond), STRUCDESC_LISTCONDPPUSER);
		
		strSql.format("SELECT "
						  "u.userid,"
						  "u.username, "
						  "u.surname, "
						  "u.forename, "
			              "u.country, "
			              "ut.name "
                   "FROM ppuser u "
				   "INNER JOIN  ppusertype ut "
			       "ON u.usertypeid = ut.usertypeid "
				   "ORDER BY u.surname");
	}
	else if (pCond->stListCond.APPLICATIONID != 0 && 
			 pCond->stListCond.GROUPID == 0 && 
			 pCond->stListCond.CORUSRGRP == false &&
			 pCond->stListCond.BRANCHREGID <= 0)
	{ //alle Benutzer für diese Applikation laden
		DtStructStripLast(&(pCond->stListCond), STRUCDESC_LISTCONDPPUSER);
		
		strSql.format("SELECT "
						  "u.userid,"
						  "u.username, "
						  "u.surname, "
						  "u.forename, "
						  "u.country, "
						  "ut.name "
					"FROM ppuser u, ppappright a, ppusertype ut "
					"WHERE a.applicationid = %d and a.groupid = 0 and a.userid = u.userid and u.usertypeid = ut.usertypeid "
					
					"UNION "
					"SELECT "
						"u.userid,"
						  "u.username, "
						  "u.surname, "
						  "u.forename, "
						  "u.country, "
						  "ut.name "
					"FROM ppuser u, ppusertype ut "
					"WHERE u.usertypeid = ut.usertypeid and EXISTS ( SELECT 1 FROM ppcorappusergroup c "
							"WHERE c.applicationid = %d and c.userid = u.userid )"
					"ORDER BY 3",
				   pCond->stListCond.APPLICATIONID, pCond->stListCond.APPLICATIONID);

	}
	else if (pCond->stListCond.APPLICATIONID != 0 && 
			 pCond->stListCond.GROUPID == 0 && 
			 pCond->stListCond.CORUSRGRP == false &&
			 pCond->stListCond.BRANCHREGID > 0)
	{ //alle Benutzer für diese Applikation + bestimmte Branch / Region laden
		DtStructStripLast(&(pCond->stListCond), STRUCDESC_LISTCONDPPUSER);
		
		strSql.format("SELECT "
						  "u.userid,"
						  "u.username, "
						  "u.surname, "
						  "u.forename, "
						  "u.country, "
			              "ut.name "
					"FROM ppuser u, ppappright a, ppusertype ut "
					"WHERE a.applicationid = %d and a.groupid = 0 and a.userid = u.userid and u.usertypeid = ut.usertypeid and a.branchregionid = %d "
					
					"UNION "
					"SELECT "
						"u.userid,"
						  "u.username, "
						  "u.surname, "
						  "u.forename, "
			              "u.country, "
			              "ut.name "
					"FROM ppuser u, ppusertype ut"
					"WHERE u.usertypeid = ut.usertypeid and EXISTS ( SELECT 1 FROM ppcorappusergroup c, ppappright a "
							"WHERE c.applicationid = %d and c.userid = u.userid "
							"and c.applicationid = a.applicationid and a.branchregionid = %d "
							"and a.groupid = c.groupid) "
					"ORDER BY 3",
				pCond->stListCond.APPLICATIONID, pCond->stListCond.BRANCHREGID, 
				pCond->stListCond.APPLICATIONID, pCond->stListCond.BRANCHREGID);

	}
	else if (pCond->stListCond.APPLICATIONID != 0 && pCond->stListCond.GROUPID != 0 && pCond->stListCond.CORUSRGRP == true)
	{ // alle Benutzer für diese Gruppe laden
		DtStructStripLast(&(pCond->stListCond), STRUCDESC_LISTCONDPPUSER);
			
		strSql.format("SELECT "
						  "u.userid,"
						  "u.username, "
						  "u.surname, "
						  "u.forename, "
			              "u.country, "
			              "ut.name "
					"FROM ppcorappusergroup c, ppuser u, ppusertype ut "
					"WHERE c.applicationid = %d and c.groupid = %d and c.userid = u.userid and u.usertypeid = ut.usertypeid "
				    "ORDER BY u.surname",
					pCond->stListCond.APPLICATIONID, pCond->stListCond.GROUPID);
	}
	else if (pCond->stListCond.APPLICATIONID != 0 && pCond->stListCond.GROUPID != 0 && pCond->stListCond.CORUSRGRP == false)
	{ // alle Benutzer, die nicht in dieser Gruppe sind
		DtStructStripLast(&(pCond->stListCond), STRUCDESC_LISTCONDPPUSER);
			
		strSql.format("SELECT "
						  "u.userid, "
						  "u.username, "
						  "u.surname, "
						  "u.forename, "
			              "u.country, "
			              "ut.name "
					"FROM ppuser u, ppusertype ut "
					"WHERE u.usertypeid = ut.usertypeid and u.userid NOT IN "
					"(SELECT "
						"c.userid "
					"FROM ppcorappusergroup c "
					" WHERE c.applicationid = %d and c.groupid = %d) "
					"ORDER BY u.surname",
					pCond->stListCond.APPLICATIONID, pCond->stListCond.GROUPID);
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
VAccess::ERet SppUser::fetch(VData & pSDataProj)
{
	long         lRet;
	T_ListProjppUser * pProj = (T_ListProjppUser *)&(((SListProj *)&pSDataProj)->stListProj);

	lRet = SqlBufferedFetch(m_hCursor, 1, 100, pProj, NULL); 

	switch(lRet)
	{
	case SQL_SUCCESS:
		DtStructStripLast(pProj, STRUCDESC_LISTPROJPPUSER);
		return VAccess::RET_OK;

	case SQL_NOTFOUND:
		return VAccess::RET_NOTFOUND;

	default:
		SLog::ex().write(__FUNCTION__, VAccess::RET_ERROR, 
			                      "fetch user = %ld",
								  lRet);
		assert(0);
		return VAccess::RET_ERROR;
	}
}

/*----------------------------------------------------------------------------*/
SppUser::SListProj::SListProj()
{
	clear();
}

/*----------------------------------------------------------------------------*/
bool SppUser::SListProj::operator == (const VData & r) const
{
	if (typeid(*this) != typeid(r))
        return false;

	return (memcmp(&stListProj, 
		           &(((const SListProj &)r).stListProj), 
				   sizeof(T_ListProjppUser)) == 0)
		   ? true : false;
}

/*----------------------------------------------------------------------------*/
void SppUser::SListProj::clear()
{
	memset(&stListProj, 0, sizeof(T_ListProjppUser));
}

/*----------------------------------------------------------------------------*/
SppUser::SComplSingle::SComplSingle()
{
	clear();
}

/*----------------------------------------------------------------------------*/
bool SppUser::SComplSingle::operator == (const VData & r) const
{
	if (typeid(*this) != typeid(r))
        return false;

	return
	(
		(0 == memcmp(&stSingleCond  , 
		             &(((const SComplSingle &)r).stSingleCond  ), 
					 sizeof(T_ListProjppUser  )))
		&&
		(0 == memcmp(&stSingleProj, 
		             &(((const SComplSingle &)r).stSingleProj), 
					 sizeof(T_SingleProjppUser)))
	) 
	? true : false;
}

/*----------------------------------------------------------------------------*/
void SppUser::SComplSingle::clear()
{
	memset(&stSingleCond, 0, sizeof(T_ListProjppUser));
	memset(&stSingleProj, 0, sizeof(T_SingleProjppUser));
}


