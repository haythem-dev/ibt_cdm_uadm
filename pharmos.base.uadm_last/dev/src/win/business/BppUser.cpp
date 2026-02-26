/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                     */
/**   Date   : 25.05.2004                                                     */
/**                                                                           */
/******************************************************************************/

/*----------------------------------------------------------------------------*/
// Includes
/*----------------------------------------------------------------------------*/
#include "bsn.h"		// precompiled header

#include <tbl_ppusertype.h>
#include <tbl_ppuser.h>
#include <struct_user.h>
#include <SppUser.h>

#include "BppUser.h"

#define PASSWORT_LAENGE		8

//---------------------------------------------------------------------------
#define GETCONUSERDATA		((SppUser::SListProj &)giveData())
#define GETUSERDATA			((SppUser::SComplSingle &)giveData())

SCBMEB_DEFINE_BSN(BppUser, SppUser::SComplSingle)

//---------------------------------------------------------------------------
BppUser::BppUser()
{
	 m_objSppUser = new SppUser;
	   assert(m_objSppUser);

   // Initialize data
   clearData();
}

//---------------------------------------------------------------------------
BppUser::BppUser(const BppUser& r) : VBsnObject(r)
{
	cpyMember(r);
	 m_objSppUser = new SppUser;
	   assert(m_objSppUser);
}

//---------------------------------------------------------------------------
BppUser::~BppUser()
{
	if (m_objSppUser != NULL)
	{
		delete m_objSppUser;
		m_objSppUser = NULL;
	}
}

/**---------------------------------------------------------------------------*/
BppUser * BppUser::clone() const
{
	return new BppUser(*this);
}

/**---------------------------------------------------------------------------*/
void BppUser::cpyMember(const BppUser & r)
{
    setData((r.giveData()));
}

/**---------------------------------------------------------------------------*/
IPerst::ERet BppUser::selectID()
{
	char szTest[] = "";
	if (strcmp (szTest,GETCONUSERDATA.stListProj.USERNAME) == 0)
	{
		DbgMsg(__FUNCTION__ ": Username not set");
		clearData();
	}
	else
	{
		SppUser objSppUser;
		long lUserid;
		CBString strUN = getUsername();

		VAccess::ERet eRet = objSppUser.selectID(strUN, lUserid);

		switch (eRet)
		{
		case VAccess::RET_OK:
			setUserid(lUserid);
			break;

		case VAccess::RET_NOTFOUND:
			DbgMsg(__FUNCTION__ ": Username %s not found in PPUSER!", 
				GETCONUSERDATA.stListProj.USERNAME);
			clearData();

			return IPerst::RET_WARNING;
	
		default:
			assert(0);
			return IPerst::RET_ERROR;
		}
	}

	return IPerst::RET_OK;
}

/**---------------------------------------------------------------------------*/
IPerst::ERet BppUser::SelForUpd()
{
	if (GETUSERDATA.stSingleCond.USERID == 0) 
	{
		DbgMsg(__FUNCTION__ ": Userid 0");
		clearData();
		return IPerst::RET_ERROR;
	}
	else
	{
		SppUser objSppUser;
		
		VAccess::ERet eRet = objSppUser.SelTestForUpd(GETUSERDATA);

		switch (eRet)
		{
		case VAccess::RET_OK:
			break;

		case VAccess::RET_NOTFOUND:
			DbgMsg(__FUNCTION__ ": Userid %d not found in PPUSER!", 
				GETUSERDATA.stSingleCond.USERID);
			clearData();

			return IPerst::RET_WARNING;
	
		default:
			assert(0);
			return IPerst::RET_ERROR;
		}
	}

	return IPerst::RET_OK;
}


IPerst::ERet BppUser::deactivateUserAndSetInactive()
{
	if (GETUSERDATA.stSingleCond.USERID == 0)
	{
		DbgMsg(__FUNCTION__ ": Userid 0");
		clearData();
		return IPerst::RET_ERROR;
	}
	else
	{
		SppUser objSppUser;

		VAccess::ERet eRet = objSppUser.deactivateUserAndSetInactive(GETUSERDATA);

		switch (eRet)
		{
		case VAccess::RET_OK:
			break;

		case VAccess::RET_NOTFOUND:
			DbgMsg(__FUNCTION__ ": Userid %d not found in PPUSER!",
				GETUSERDATA.stSingleCond.USERID);
			clearData();

			return IPerst::RET_WARNING;

		default:
			assert(0);
			return IPerst::RET_ERROR;
		}
	}

	return IPerst::RET_OK;
}

/**---------------------------------------------------------------------------*/
IPerst::ERet BppUser::update()
{
	if (GETCONUSERDATA.stListProj.USERID == 0)
	{
		DbgMsg(__FUNCTION__ ": UserID 0");
		clearData();
	}
	else
	{
		//SppUser objSppUser;

		VAccess::ERet eRet = m_objSppUser->updateSingle(GETUSERDATA);

		switch (eRet)
		{
		IPerst::ERet Ret;
		case VAccess::RET_OK:
			Ret = BSqlStat::ex().UpdateBackupMachine();
			if (Ret == IPerst::RET_ERROR)
			{
				DbgMsg(__FUNCTION__ ": Error on updating backup-machine!");
			}
			break;

		case VAccess::RET_NOTFOUND:
			DbgMsg(__FUNCTION__ ": UserID %d not found in PPUSER!", 
				GETCONUSERDATA.stListProj.USERID);

			clearData();

			return IPerst::RET_WARNING;
	
		default:
			assert(0);
			return IPerst::RET_ERROR;
		}
	}

	return IPerst::RET_OK;
}

/**---------------------------------------------------------------------------*/
IPerst::ERet BppUser::insert()
{
	char szTest[] = "";
	if (strcmp (szTest,GETCONUSERDATA.stListProj.USERNAME) == 0)
	{
		DbgMsg(__FUNCTION__ ": Username not set");
		clearData();
	}
	else
	{
		SppUser objSppUser;

		VAccess::ERet eRet = objSppUser.insertSingle(GETUSERDATA);

		switch (eRet)
		{
		IPerst::ERet Ret;
		case VAccess::RET_OK:
			Ret = BSqlStat::ex().UpdateBackupMachine();
			if (Ret == IPerst::RET_ERROR)
			{
				DbgMsg(__FUNCTION__ ": Error on updating backup-machine!");
			}
			break;

		case VAccess::RET_NOTFOUND:
			clearData();

			return IPerst::RET_WARNING;
	
		default:
			assert(0);
			return IPerst::RET_ERROR;
		}
	}

	return IPerst::RET_OK;
}

/**---------------------------------------------------------------------------*/
IPerst::ERet BppUser::delObj()
{
	if (GETCONUSERDATA.stListProj.USERID == 0) 
	{
		DbgMsg(__FUNCTION__ ": Userid 0");
		clearData();
		return IPerst::RET_ERROR;
	}
	else
	{
		IPerst::ERet eRet = delppUser();
		if (eRet == IPerst::RET_OK)
		{
			//ppLoginPrevious
			eRet = delLoginPrevious();
			if (eRet == IPerst::RET_ERROR)
			{
				return eRet;
			}
			
			//ppAppRight
			eRet = delAppRight();
			if (eRet == IPerst::RET_ERROR)
			{
				return eRet;
			}
			
			//ppCorAppUserGroup
			eRet = delCorAppUserGroup();
			if (eRet == IPerst::RET_ERROR)
			{
				return eRet;
			}
			
			return IPerst::RET_OK;
		}
		
		return eRet;
	}
}

/**---------------------------------------------------------------------------*/
IPerst::ERet BppUser::delppUser ()
{
	SppUser objSppUser;

	VAccess::ERet eRet = objSppUser.deleteSingle(GETUSERDATA);

	switch (eRet)
	{
		IPerst::ERet Ret;
		case VAccess::RET_OK:
			Ret = BSqlStat::ex().UpdateBackupMachine();
			if (Ret == IPerst::RET_ERROR)
			{
				DbgMsg(__FUNCTION__ ": Error on updating backup-machine!");
			}
			break;

		case VAccess::RET_NOTFOUND:
			DbgMsg(__FUNCTION__ ": UserID %d not found in PPUSER!", 
				GETCONUSERDATA.stListProj.USERID);
			clearData();

			return IPerst::RET_WARNING;
	
		default:
			assert(0);
			return IPerst::RET_ERROR;
	}

	return IPerst::RET_OK;
}

/**---------------------------------------------------------------------------*/
IPerst::ERet BppUser::delLoginPrevious    ()
{
	SppUser objSppUser;

	VAccess::ERet eRet = objSppUser.deletePrevious(GETUSERDATA);

	switch (eRet)
	{
		IPerst::ERet Ret;
		case VAccess::RET_OK:
			Ret = BSqlStat::ex().UpdateBackupMachine();
			if (Ret == IPerst::RET_ERROR)
			{
				DbgMsg(__FUNCTION__ ": Error on updating backup-machine!");
			}
			break;

		case VAccess::RET_NOTFOUND:
			DbgMsg(__FUNCTION__ ": UserID %d not found in PPLOGINPREVIOUS!", 
				GETCONUSERDATA.stListProj.USERID);
			clearData();

			return IPerst::RET_WARNING;
	
		default:
			assert(0);
			return IPerst::RET_ERROR;
	}

	return IPerst::RET_OK;
}

/**---------------------------------------------------------------------------*/
IPerst::ERet BppUser::delAppRight         ()
{
	SppUser objSppUser;

	VAccess::ERet eRet = objSppUser.deleteAppRight(GETUSERDATA);

	switch (eRet)
	{
		IPerst::ERet Ret;
		case VAccess::RET_OK:
			Ret = BSqlStat::ex().UpdateBackupMachine();
			if (Ret == IPerst::RET_ERROR)
			{
				DbgMsg(__FUNCTION__ ": Error on updating backup-machine!");
			}
			break;

		case VAccess::RET_NOTFOUND:
			DbgMsg(__FUNCTION__ ": UserID %d not found in PPAPPRIGHT!", 
				GETCONUSERDATA.stListProj.USERID);
			clearData();

			return IPerst::RET_WARNING;
	
		default:
			assert(0);
			return IPerst::RET_ERROR;
	}

	return IPerst::RET_OK;
}

/**---------------------------------------------------------------------------*/
IPerst::ERet BppUser::delCorAppUserGroup  ()
{
	SppUser objSppUser;

	VAccess::ERet eRet = objSppUser.deleteFromGroup(GETUSERDATA);

	switch (eRet)
	{
		IPerst::ERet Ret;
		case VAccess::RET_OK:
			Ret = BSqlStat::ex().UpdateBackupMachine();
			if (Ret == IPerst::RET_ERROR)
			{
				DbgMsg(__FUNCTION__ ": Error on updating backup-machine!");
			}
			break;

		case VAccess::RET_NOTFOUND:
			DbgMsg(__FUNCTION__ ": UserID %d not found in PPCORAPPUSERGROUP!", 
				GETCONUSERDATA.stListProj.USERID);
			clearData();

			return IPerst::RET_WARNING;
	
		default:
			assert(0);
			return IPerst::RET_ERROR;
	}

	return IPerst::RET_OK;
}

/**---------------------------------------------------------------------------*/
IPerst::ERet BppUser::load(bool bLock)
{
	if (GETCONUSERDATA.stListProj.USERID == 0)
	{
		DbgMsg(__FUNCTION__ ": UserID 0");
		clearData();
	}
	else
	{
		//SppUser objSppUser;

		VAccess::ERet eRet = m_objSppUser->selectSingle(GETUSERDATA, bLock);

		switch (eRet)
		{
		case VAccess::RET_OK:
			break;

		case VAccess::RET_NOTFOUND:
			DbgMsg(__FUNCTION__ ": UserID %d not found in PPUSER!", 
				GETCONUSERDATA.stListProj.USERID);

			clearData();

			return IPerst::RET_WARNING;

		case VAccess::RET_LOCKED:
			DbgMsg(__FUNCTION__ ": UserID %d locked for update in PPUSER!", 
				GETCONUSERDATA.stListProj.USERID);

			return IPerst::RET_WARNING;
	
		default:
			assert(0);
			return IPerst::RET_ERROR;
		}
	}

	return IPerst::RET_OK;
}

/**---------------------------------------------------------------------------*/
void BppUser::setUserid(long lUserid)
{
	GETCONUSERDATA.stListProj.USERID = lUserid;
	GETUSERDATA.stSingleCond.USERID = lUserid; 
}

/**---------------------------------------------------------------------------*/
void  BppUser::setUsername(CBString strUsername)
{
	strcpy(GETUSERDATA.stSingleCond.USERNAME, strUsername);
	strcpy(GETCONUSERDATA.stListProj.USERNAME, strUsername);
}
/**---------------------------------------------------------------------------*/
void  BppUser::setNtUser(CBString strNtUser)
{
	strcpy(GETUSERDATA.stSingleProj.NTUSER, strNtUser);
}

/**---------------------------------------------------------------------------*/
void   BppUser::setPHXPasswd(CBString strPasswd)
{
	strcpy(GETUSERDATA.stSingleProj.PASSWORD, strPasswd);
}

/**---------------------------------------------------------------------------*/
void   BppUser::setAESPasswd(CBString strPasswd)
{
	strcpy(GETUSERDATA.stSingleProj.PWD_ENC, strPasswd);
}

/**---------------------------------------------------------------------------*/
void BppUser::setSurname  (CBString strSurname)
{
	strcpy(GETUSERDATA.stSingleCond.SURNAME, strSurname);
	strcpy(GETCONUSERDATA.stListProj.SURNAME, strSurname);
}

/**---------------------------------------------------------------------------*/
void BppUser::setForename (CBString strForename)
{
	strcpy(GETUSERDATA.stSingleCond.FORENAME, strForename);
	strcpy(GETCONUSERDATA.stListProj.FORENAME, strForename);
}

/**---------------------------------------------------------------------------*/
void BppUser::setDescription  (CBString strDescription)
{
	strcpy(GETUSERDATA.stSingleProj.DESCRIPTION, strDescription);
}

/**---------------------------------------------------------------------------*/
void BppUser::setMailuser  (CBString strMailuser)
{
	strcpy(GETUSERDATA.stSingleProj.MAILUSER, strMailuser);
}

/**---------------------------------------------------------------------------*/
void BppUser::setEmailuser (CBString strEmailuser)
{
	strcpy(GETUSERDATA.stSingleProj.EMAILUSER, strEmailuser);
}

/**---------------------------------------------------------------------------*/
void BppUser::setReference (CBString strRef)
{
	strcpy(GETUSERDATA.stSingleProj.REFERENCE, strRef);
}

/**---------------------------------------------------------------------------*/
void BppUser::setExternal (short sExternal)
{
	GETUSERDATA.stSingleProj.EXTERNAL = sExternal;
}

/**---------------------------------------------------------------------------*/
void BppUser::setChangePW (short sChangePW)
{
	GETUSERDATA.stSingleProj.CHANGEPW = sChangePW;
}

/**---------------------------------------------------------------------------*/
void BppUser::setActiv  (short sActiv)
{
	GETUSERDATA.stSingleProj.ACTIV = sActiv;
}

/**---------------------------------------------------------------------------*/
void BppUser::setUserTypeID (short sUserTypeID)
{
	GETUSERDATA.stSingleProj.USERTYPEID = sUserTypeID;
}

/**---------------------------------------------------------------------------*/
void BppUser::setUserCountry (CBString strCountry)
{
	strcpy(GETUSERDATA.stSingleProj.COUNTRY, strCountry);
}

/**---------------------------------------------------------------------------*/
const long	BppUser::getUserid () const
{
	return GETCONUSERDATA.stListProj.USERID;
}

/**---------------------------------------------------------------------------*/
const CBString	BppUser::getUseridStr() const
{
	long2scStr(m_strBufExch, GETCONUSERDATA.stListProj.USERID);
	return m_strBufExch;
}
/**---------------------------------------------------------------------------*/
const CBString BppUser::getPHXPasswd() const
{
	m_strBufExch = GETUSERDATA.stSingleProj.PASSWORD;
	return m_strBufExch;
}

/**---------------------------------------------------------------------------*/
const CBString BppUser::getAESPasswd() const
{
	m_strBufExch = GETUSERDATA.stSingleProj.PWD_ENC;
	return m_strBufExch;
}

/**---------------------------------------------------------------------------*/
const CBString BppUser::getSurname() const
{
	m_strBufExch = GETCONUSERDATA.stListProj.SURNAME;
	//m_strBufExch = GETUSERDATA->stSingleProj.SURNAME;
	return m_strBufExch;
}
/**---------------------------------------------------------------------------*/
const CBString BppUser::getForename() const
{
	m_strBufExch = GETCONUSERDATA.stListProj.FORENAME;
	//m_strBufExch = GETUSERDATA->stSingleProj.FORENAME;
	return m_strBufExch;
}
/**---------------------------------------------------------------------------*/
const CBString BppUser::getDescription () const
{
	m_strBufExch = GETUSERDATA.stSingleProj.DESCRIPTION;
	return m_strBufExch;
}

/**---------------------------------------------------------------------------*/
const CBString BppUser::getNtUser () const
{
	m_strBufExch = GETUSERDATA.stSingleProj.NTUSER;
	return m_strBufExch;
}

/**---------------------------------------------------------------------------*/
const CBString  BppUser::getMailuser () const
{
	m_strBufExch = GETUSERDATA.stSingleProj.MAILUSER;
	return m_strBufExch;
}
/**---------------------------------------------------------------------------*/
const CBString  BppUser::getEmailuser() const
{
	m_strBufExch = GETUSERDATA.stSingleProj.EMAILUSER;
	return m_strBufExch;
}
/**---------------------------------------------------------------------------*/
const short	BppUser::getChangepw() const
{
	return GETUSERDATA.stSingleProj.CHANGEPW;
}

/**---------------------------------------------------------------------------*/
const CBString	BppUser::getReference() const
{
	m_strBufExch = GETUSERDATA.stSingleProj.REFERENCE;
	return m_strBufExch;
}
/**---------------------------------------------------------------------------*/
const short	BppUser::getExternal () const
{
	return GETUSERDATA.stSingleProj.EXTERNAL;
}

/**---------------------------------------------------------------------------*/
const short	BppUser::getActiv () const
{
	return GETUSERDATA.stSingleProj.ACTIV;
}
	
/**---------------------------------------------------------------------------*/
const CBString BppUser::getUsername() const
{
	m_strBufExch = GETCONUSERDATA.stListProj.USERNAME;
	return m_strBufExch;
}

/**---------------------------------------------------------------------------*/
const long BppUser::getLastLogin() const
{
	return GETUSERDATA.stSingleProj.LASTLOGIN;
}

/**---------------------------------------------------------------------------*/
const CBString	BppUser::getLastLoginStr() const
{
	long2scStr(m_strBufExch, GETUSERDATA.stSingleProj.LASTLOGIN);
	return m_strBufExch;
}

/**---------------------------------------------------------------------------*/
const long	BppUser::getLastChangePw		() const
{
	return GETUSERDATA.stSingleProj.LASTPWCHANGE;
}

/**---------------------------------------------------------------------------*/
const short BppUser::getUserTypeID() const
{
	return GETUSERDATA.stSingleProj.USERTYPEID;
}

/**---------------------------------------------------------------------------*/
const CBString BppUser::getUserCountry() const
{
	m_strBufExch = GETUSERDATA.stSingleProj.COUNTRY;
	return m_strBufExch;
}

const CBString BppUser::getListUserCountry() const
{
	m_strBufExch = GETCONUSERDATA.stListProj.COUNTRY;
	return m_strBufExch;
}

const CBString BppUser::getListUserType() const
{
	m_strBufExch = GETCONUSERDATA.stListProj.USERTYPE;
	return m_strBufExch;
}

void BppUser::setUserCountryName(CBString strCountry)
{
	strcpy(GETCONUSERDATA.stListProj.COUNTRY, strCountry);
}

void BppUser::setUserTypeName(CBString strUserTypeName)
{
	strcpy(GETCONUSERDATA.stListProj.USERTYPE, strUserTypeName);
}

/*
bool BppUser::PWCrypt(CBString& strPassword)
{
	// crypt password	
	strPassword.erasespace(CBString::FIRST_END);

	if (strPassword.empty() != true) //keine Verschlüsselung, wenn leer!
	{
		char szInput [PASSWORT_LAENGE + 1];
		char szOutput[PASSWORT_LAENGE + 1];

		memset(szInput, ' ', PASSWORT_LAENGE);
		memcpy(szInput, 
			   strPassword, 
			   strPassword.length() > PASSWORT_LAENGE ? PASSWORT_LAENGE
			                                              : strPassword.length());
		szInput[PASSWORT_LAENGE] = '\0';

		SCCutCrypt(szInput, szOutput, PASSWORT_LAENGE);

		strPassword = szOutput;
	}// crypt password	

 return true;
}

bool BppUser::PWDeCrypt(CBString& strPassword)
{
	// decrypt password	
	strPassword.erasespace(CBString::FIRST_END);

	if (strPassword.empty() != true) //keine Verschlüsselung, wenn leer!
	{
		char szInput [PASSWORT_LAENGE + 1];
		char szOutput[PASSWORT_LAENGE + 1];

		memset(szInput, ' ', 8);
		memcpy(szInput, 
			   strPassword, 
			   strPassword.length() > PASSWORT_LAENGE ? PASSWORT_LAENGE
			                                              : strPassword.length());
		szInput[PASSWORT_LAENGE] = '\0';

		SCCutDecrypt(szInput,szOutput, PASSWORT_LAENGE);

		strPassword = szOutput;
	}// decrypt password	
	
	return true;
}
*/

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/	
