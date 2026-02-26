/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH										       */
/**   Date   : 06.06.2005                                                     */
/**                                                                           */
/******************************************************************************/

#include "srv.h"

#include "tbl_ibiuser.h"
#include "struct_ibiuser.h"
#include "SConnectOdbc.h"

#include "SBiDefault.h"


#define ROWSETSIZE	100
/*----------------------------------------------------------------------------*/
SBiDefault::SBiDefault() : VAccOdbcCurs(TSConnectOdbc::ex().giveDatabase(), ATTRIBNO_LISTPROJIBIUSER)
{
}

/*----------------------------------------------------------------------------*/
SBiDefault::~SBiDefault()
{
	closeCursor();
}

/*----------------------------------------------------------------------------*/
VAccess::ERet	SBiDefault::selcountIBIUser(SListProj  & pSDataSingle,  int * piCountIbiUsr)
{
	CBString strSql;
	T_ListCondIbiUser   * pCond = (T_ListCondIbiUser *)&(pSDataSingle.stListCond);
	
	
	strSql.format("SELECT count (*) FROM IBI_User "
			      "WHERE userid = '%s'",
				  pCond->USERID );

	CRecordset objRec(&(TSConnectOdbc::ex().giveDatabase()));
	
	//open-cursor
	try
	{
		if (objRec.Open(CRecordset::forwardOnly, 
			                 strSql.c_str(),
							 CRecordset::readOnly           ) == FALSE)
		{
			m_strErrorMsg.format(__FUNCTION__ "open cursor (<%s>) failed", strSql.c_str());
			return VAccess::RET_ERROR;
		}
	}
	catch (CDBException* e)
	{
		m_strErrorMsg.format(__FUNCTION__"open cursor (<%s>) open error: %s (%ld)", 
								strSql.c_str(),
								e->m_strError.GetString(),
								e->m_nRetCode);
		e->Delete();
		return VAccess::RET_ERROR;
	}

	//read
	try
	{
		CDBVariant objResult;
		objRec.GetFieldValue( (short)0, objResult );
		*piCountIbiUsr = objResult.m_iVal;
	}
	catch (CDBException* e)
	{
		m_strErrorMsg.format(__FUNCTION__"getfield selcountIBIUser-exp error: %s (%ld)", 
								e->m_strError.GetString(),
								e->m_nRetCode);
		e->Delete();
		// fall through to close-cursor
	}

	//close-cursor
	try
	{
		objRec.Close();
	}
	catch (CDBException* e)
	{
		m_strErrorMsg.format(__FUNCTION__"close cursor selcountIBIUser-exp error: %s (%ld)", 
								e->m_strError.GetString(),
								e->m_nRetCode);
		e->Delete();
		return VAccess::RET_ERROR;
	}

	return VAccess::RET_OK;
}

/*----------------------------------------------------------------------------*/
VAccess::ERet SBiDefault::insertIBIUser(SListProj  & pSDataSingle )
{
	CBString            strSql;
	T_ListCondIbiUser   * pCond = (T_ListCondIbiUser *)&(pSDataSingle.stListCond);
	T_ListProjIbiUser   * pProj = (T_ListProjIbiUser *)&(pSDataSingle.stListProj);
	
	strSql.format("INSERT INTO "
						"IBI_User ( userid, lastname, firstname, remark, email, "
						"defaultbranchno, defaultcountrycode, defaultformat, defaultstyle) "
						"VALUES ( '%s', '%s', '%s', '%s', '%s', %d, '%s', '%s', '%s' ) "
						, pCond->USERID, pCond->LASTNAME, 
						pProj->FIRSTNAME, pProj->REMARK, pProj->EMAIL, 
						pProj->BRANCHID, pProj->DEFAULTCOUNTRYCODE,
						pProj->DEFAULTFORMAT, pProj->DEFAULTSTYLE);
		
	try
	{ 
		TSConnectOdbc::ex().giveDatabase().BeginTrans();
		TSConnectOdbc::ex().giveDatabase().ExecuteSQL(strSql.c_str());
	}
	catch (CDBException* e)
	{
		m_strErrorMsg.format("ODBC execute '%s' error: %s (%ld)", 
								strSql.c_str(),
								e->m_strError.GetString(),
								e->m_nRetCode);
		e->Delete();
		return VAccess::RET_ERROR;
	}

	switch (TSConnectOdbc::ex().giveDatabase().getRowsProcessed())
	{
	case 1:
		TSConnectOdbc::ex().giveDatabase().CommitTrans();
		return VAccess::RET_OK;

	default:
		TSConnectOdbc::ex().giveDatabase().Rollback();
		m_strErrorMsg.format("ODBC execute '%s' error: 0 rows processed", strSql.c_str());
		return VAccess::RET_ERROR;
	}
}

/*----------------------------------------------------------------------------*/
VAccess::ERet SBiDefault::updateIBIUser	(SListProj  & pSDataSingle )
{
	CBString            strSql;
	T_ListCondIbiUser   * pCond = (T_ListCondIbiUser *)&(pSDataSingle.stListCond);
	T_ListProjIbiUser   * pProj = (T_ListProjIbiUser *)&(pSDataSingle.stListProj);
	
	strSql.format("UPDATE IBI_User "
					"SET remark = '%s', email = '%s', "
					"defaultbranchno = %d , defaultcountrycode = '%s', "
					"defaultformat = '%s', defaultstyle = '%s' "
					"WHERE "
					"userid = '%s'",
					pProj->REMARK, pProj->EMAIL, pProj->BRANCHID, pProj->DEFAULTCOUNTRYCODE,
					pProj->DEFAULTFORMAT, pProj->DEFAULTSTYLE,
					pCond->USERID);

	try
	{ 
		TSConnectOdbc::ex().giveDatabase().BeginTrans();
		TSConnectOdbc::ex().giveDatabase().ExecuteSQL(strSql.c_str());
	}
	catch (CDBException* e)
	{
		m_strErrorMsg.format("ODBC execute '%s' error: %s (%ld)", 
								strSql.c_str(),
								e->m_strError.GetString(),
								e->m_nRetCode);
		e->Delete();
		return VAccess::RET_ERROR;
	}

	switch (TSConnectOdbc::ex().giveDatabase().getRowsProcessed())
	{
	case 1:
		TSConnectOdbc::ex().giveDatabase().CommitTrans();
		return VAccess::RET_OK;

	default:
		TSConnectOdbc::ex().giveDatabase().Rollback();
		m_strErrorMsg.format("ODBC execute '%s' error: 0 rows processed", strSql.c_str());
		return VAccess::RET_ERROR;
	}
}

/*----------------------------------------------------------------------------*/
VAccess::ERet SBiDefault::selcountIBIUserCountryR (SListProj  & pSDataSingle, int * piCountCountryR )
{
	CBString strSql;
	T_ListCondIbiUser   * pCond = (T_ListCondIbiUser *)&(pSDataSingle.stListCond);
	T_ListProjIbiUser   * pProj = (T_ListProjIbiUser *)&(pSDataSingle.stListProj);
	
	
	strSql.format("SELECT count (*) FROM IBI_UserCountryRights "
			      "WHERE userid = '%s' and countrycode = '%s'",
				  pCond->USERID, pProj->DEFAULTCOUNTRYCODE );

	CRecordset objRec(&(TSConnectOdbc::ex().giveDatabase()));
	
	//open-cursor
	try
	{
		if (objRec.Open(CRecordset::forwardOnly, 
			                 strSql.c_str(),
							 CRecordset::readOnly           ) == FALSE)
		{
			m_strErrorMsg.format(__FUNCTION__ "open cursor (<%s>) failed", strSql.c_str());
			return VAccess::RET_ERROR;
		}
	}
	catch (CDBException* e)
	{
		m_strErrorMsg.format(__FUNCTION__"open cursor (<%s>) open error: %s (%ld)", 
								strSql.c_str(),
								e->m_strError.GetString(),
								e->m_nRetCode);
		e->Delete();
		return VAccess::RET_ERROR;
	}

	//read
	try
	{
		CDBVariant objResult;
		objRec.GetFieldValue( (short)0, objResult );
		*piCountCountryR = objResult.m_iVal;
	}
	catch (CDBException* e)
	{
		m_strErrorMsg.format(__FUNCTION__"getfield selcountIBIUserCountryR-exp error: %s (%ld)", 
								e->m_strError.GetString(),
								e->m_nRetCode);
		e->Delete();
		// fall through to close-cursor
	}

	//close-cursor
	try
	{
		objRec.Close();
	}
	catch (CDBException* e)
	{
		m_strErrorMsg.format(__FUNCTION__"close cursor selcountIBIUserCountryR-exp error: %s (%ld)", 
								e->m_strError.GetString(),
								e->m_nRetCode);
		e->Delete();
		return VAccess::RET_ERROR;
	}

	return VAccess::RET_OK;
}

/*----------------------------------------------------------------------------*/
VAccess::ERet SBiDefault::insertIBIUserCountryRights (SListProj  & pSDataSingle )
{
	CBString            strSql;
	T_ListCondIbiUser   * pCond = (T_ListCondIbiUser *)&(pSDataSingle.stListCond);
	T_ListProjIbiUser   * pProj = (T_ListProjIbiUser *)&(pSDataSingle.stListProj);
	
	strSql.format("INSERT INTO "
						"IBI_UserCountryRights ( userid, countrycode ) "
						"VALUES ( '%s', '%s') "
						, pCond->USERID, pProj->DEFAULTCOUNTRYCODE );
		
	try
	{ 
		TSConnectOdbc::ex().giveDatabase().BeginTrans();
		TSConnectOdbc::ex().giveDatabase().ExecuteSQL(strSql.c_str());
	}
	catch (CDBException* e)
	{
		m_strErrorMsg.format("ODBC execute '%s' error: %s (%ld)", 
								strSql.c_str(),
								e->m_strError.GetString(),
								e->m_nRetCode);
		e->Delete();
		return VAccess::RET_ERROR;
	}

	switch (TSConnectOdbc::ex().giveDatabase().getRowsProcessed())
	{
	case 1:
		TSConnectOdbc::ex().giveDatabase().CommitTrans();
		return VAccess::RET_OK;

	default:
		TSConnectOdbc::ex().giveDatabase().Rollback();
		m_strErrorMsg.format("ODBC execute '%s' error: 0 rows processed", strSql.c_str());
		return VAccess::RET_ERROR;
	}
}

/*----------------------------------------------------------------------------*/
VAccess::ERet SBiDefault::openCursor(VData & pSDataCond)
{
	return VAccess::RET_OK;
}

/*----------------------------------------------------------------------------*/
VAccess::ERet SBiDefault::fetch(VData & pSDataProj)
{
	return VAccess::RET_OK;
}

/*----------------------------------------------------------------------------*/
SBiDefault::SListProj::SListProj()
{
	clear();
}

/*----------------------------------------------------------------------------*/
bool SBiDefault::SListProj::operator == (const VData & r) const
{
	if (typeid(*this) != typeid(r))
        return false;

	return
	(
		(0 == memcmp(&stListCond  , 
		             &(((const SListProj &)r).stListCond  ), 
					 sizeof(T_ListCondIbiUser  )))
		&&
		(0 == memcmp(&stListProj, 
		             &(((const SListProj &)r).stListProj), 
					 sizeof(T_ListProjIbiUser)))
	) 
	? true : false;
}

/*----------------------------------------------------------------------------*/
void SBiDefault::SListProj::clear()
{
	memset(&stListCond, 0, sizeof(T_ListCondIbiUser));
	memset(&stListProj, 0, sizeof(T_ListProjIbiUser));
}

