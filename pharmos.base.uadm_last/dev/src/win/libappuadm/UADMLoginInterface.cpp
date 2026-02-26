// CUADMLoginInterface.cpp: implementation file
//

#include "pch.h"
#include "framework.h"
#include "UADMLoginInterface.h"
#include "aes_util.h" // for encryptNew and decryptNew functions

/////////////////////////////////////////////////////////////////////////////
// UADM Login Interface 
namespace UADM
{

	CUADMLoginInterface::CUADMLoginInterface(std::vector<std::string>* pvecDebugInfos)
	{
		m_intVoodooConn = -1;
		m_pobjIFXConn = NULL;
		m_pobjUADMEntry = NULL;
		m_blnPreferVoodoo = false;
		m_pobjUADMEnvironment = NULL;
		m_enmUADMConnectionType = UADM_CONNECTION_TYPE_NONE;
		m_enmUADMLastConnectionType = m_enmUADMConnectionType;
		m_strUADMLastServername = "";
		m_strUADMLastDatabase = "";
		m_strUADMLastPortNo = "";
		m_strLastErrorMessage = "";
		m_enmUADMEncryptionType = UADM_PASSWORD_ENCRYPTION_TYPE_PHOENIX; // defaulted to internal encryption type
		m_intLoginMaxRetries = 3;
		m_blnLockUserAfterMaxRetries = false;
		m_intPasswordHistoryCount = 6;
		m_intMaxPasswordValidityDays = 90;
		m_blnAllowLocalLogin = false;

#ifdef EXLUDE_INFORMIX
		m_blnPreferVoodoo = true;
#endif
		m_pvecDebugInfos = pvecDebugInfos;
		// debug info
		m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::constructor");
	}

	CUADMLoginInterface::~CUADMLoginInterface()
	{
		closeConnection(true);
	}

	void CUADMLoginInterface::reset()
	{
		m_lngUserID = -1;
		m_strUsername = "";
		m_strEncPwd = "";
		m_strFirstname = "";
		m_strLastname = "";
		m_strMail = "";
		m_strEMail = "";
		m_strDomainAccount = "";
		m_strDescription = "";
		m_strOSAccount = "";
		m_blnIsActive = false;
		m_blnForcePasswordChange = false;
		m_lngLastPasswordChangeDate = 0;
		m_lngLastLoginDate = 0;
		m_lngCurrentDate = 0;
		m_lngAppID = -1;
		m_strAppName = "";
		m_strAppDesc = "";
		m_strLanguage = "";
		m_strLanguageName = "";
	}

	void CUADMLoginInterface::setConnectionTypePreference(int intPref)
	{
		// preference defined as:
		// 0 = prefer informix first, then voodoo
		// 1 = prefer voodoo first, then informix
		if (intPref == 1)
		{
			m_blnPreferVoodoo = true;
		}
		else
		{
			m_blnPreferVoodoo = false;
		}
	}

	void CUADMLoginInterface::initialize(UADM_APP_ENVIRONMENT* pobjUADMEnvironment)
	{
		m_pobjUADMEnvironment = pobjUADMEnvironment;
	}

	bool CUADMLoginInterface::openConnection()
	{
		// debug info
		m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::openConnection - start");

		if (m_pobjUADMEnvironment == NULL)
		{
			m_strLastErrorMessage = "Environment not initialized";
			m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::openConnection - " + m_strLastErrorMessage);
			return false; // environment not initialized
		}
		if ((m_intVoodooConn >= 0) || (m_pobjIFXConn != NULL))
		{
			closeConnection(false); // already opened, close the existing connection first
		}
		long result = 0;
		bool ret = false;
		if (m_blnPreferVoodoo == true)
		{
			// try voodoo first with primary and secondary server
			ret = openVoodooConnection();
			if (ret == false)
			{
				// try informix second with primary and secondary server
				ret = openInformixConnection();
			}
		}
		else
		{
			// try informix first with primary and secondary server
			ret = openInformixConnection();
			if (ret == false)
			{
				// try voodoo second with primary and secondary server
				ret = openVoodooConnection();
			}
		}

		if (ret == true)
		{
			// common initialization after connection established
			ret = initSession();
		}
		if (ret == false)
		{
			// no more options
			m_enmUADMConnectionType = UADM_CONNECTION_TYPE_NONE;
			m_enmUADMLastConnectionType = m_enmUADMConnectionType;
			m_strUADMLastServername = "";
			m_strUADMLastDatabase = "";
			m_strUADMLastPortNo = "";
			m_strLastErrorMessage = "Failed to connect to any UADM service (voodoo main, voodoo backup, informix main, informix backup)";
			m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::openConnection - " + m_strLastErrorMessage);
			reset(); // reset user data
		}
		// debug info
		m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::openConnection - done with connection type "+ getLastUADMConnectionType());
		return ret; 
	}

	bool CUADMLoginInterface::closeConnection(bool blnReset)
	{
		// debug info
		m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::closeConnection - start");
		bool ret = false;
		if ((m_enmUADMConnectionType == UADM_CONNECTION_TYPE_VOODOO_PRIMARY) || (m_enmUADMConnectionType == UADM_CONNECTION_TYPE_VOODOO_SECONDARY))
		{
			if (m_intVoodooConn >= 0)
			{
				// close database first
				SqlExecuteImm("close database");
				// call the NmpCloseHandle function to stop pipe connection and to release memory
				if (NmpCloseHandle(m_intVoodooConn) == 0)
				{
					ret = true;
				}
				m_intVoodooConn = -1;
			}
			else
			{
				ret = true; // already closed or never opened
			}
		}
		else if ((m_enmUADMConnectionType == UADM_CONNECTION_TYPE_INFORMIX_PRIMARY) || (m_enmUADMConnectionType == UADM_CONNECTION_TYPE_INFORMIX_SECONDARY))
		{
			if (m_pobjIFXConn != NULL)
			{
#ifndef EXLUDE_INFORMIX
				// call the informix disconnection to stop direct database server connection and to release memory
				if (m_pobjIFXConn->Close() == TRUE)
#endif
				{
					ret = true;
				}
				delete m_pobjIFXConn;
				m_pobjIFXConn = NULL; // release connection object
			}
			else
			{
				ret = true; // already closed or never opened
			}
		}
		else
		{
			ret = true; // no connection established
		}
		m_enmUADMConnectionType = UADM_CONNECTION_TYPE_NONE; // reset connection type
		if (blnReset == true)
		{
		reset(); // reset user data
		}
		// debug info
		m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::closeConnection - done");
		return ret;
	}

	bool CUADMLoginInterface::openVoodooConnection()
	{
		bool ret = false;
		// option1: connect to primary voodoo server
		std::string primserver = convertConnectionData(m_pobjUADMEnvironment->strPrimaryServer, false); // convert primary server name if needed
		if ((primserver.empty() == false) && (m_pobjUADMEnvironment->strPrimaryDatabase.empty() == false) && (m_pobjUADMEnvironment->strVoodooPipeName.empty() == false))
		{
			if (ViaEshuOpenEx(&m_intVoodooConn, primserver.c_str(), m_pobjUADMEnvironment->strVoodooPipeName.c_str(), m_pobjUADMEnvironment->intVoodooPipePort) == 0)
			{
				m_enmUADMConnectionType = UADM_CONNECTION_TYPE_VOODOO_PRIMARY; // connected to primary voodoo server
				// do the database connection
				std::string dbconnect = "database " + m_pobjUADMEnvironment->strPrimaryDatabase;
				if (SqlExecuteImm(dbconnect.c_str()) == 0)
				{
					m_enmUADMLastConnectionType = m_enmUADMConnectionType;
					m_strUADMLastServername = primserver;
					m_strUADMLastDatabase = m_pobjUADMEnvironment->strPrimaryDatabase;
					m_strUADMLastPortNo = m_pobjUADMEnvironment->strVoodooPipeName + ":" + m_pobjUADMEnvironment->strVoodooPipePort;
					ret = true; // successfully opened and initialized
				}
			}
		}
		if (ret == false)
		{
			// option2: connect to secondary voodoo server
			std::string secserver = convertConnectionData(m_pobjUADMEnvironment->strSecondaryServer, false); // convert secondary server name if needed
			if ((secserver.empty() == false) && (m_pobjUADMEnvironment->strSecondaryDatabase.empty() == false) && (m_pobjUADMEnvironment->strVoodooPipeName.empty() == false))
			{
				if (ViaEshuOpenEx(&m_intVoodooConn, secserver.c_str(), m_pobjUADMEnvironment->strVoodooPipeName.c_str(), m_pobjUADMEnvironment->intVoodooPipePort) == 0)
				{
					m_enmUADMConnectionType = UADM_CONNECTION_TYPE_VOODOO_SECONDARY; // connected to secondary voodoo server
					// do the database connection
					std::string dbconnect = "database " + m_pobjUADMEnvironment->strSecondaryDatabase;
					if (SqlExecuteImm(dbconnect.c_str()) == 0)
					{
						m_enmUADMLastConnectionType = m_enmUADMConnectionType;
						m_strUADMLastServername = secserver;
						m_strUADMLastDatabase = m_pobjUADMEnvironment->strSecondaryDatabase;
						m_strUADMLastPortNo = m_pobjUADMEnvironment->strVoodooPipeName + ":" + m_pobjUADMEnvironment->strVoodooPipePort;
						ret = true; // successfully opened and initialized
					}
				}
			}
		}
		return ret;
	}

	bool CUADMLoginInterface::openInformixConnection()
	{
		bool ret = false;

#ifndef EXLUDE_INFORMIX
		m_pobjIFXConn = new ITConnection(); // create new informix connection object

		// option1: connect to primary informix server
		std::string primserver = convertConnectionData(m_pobjUADMEnvironment->strPrimaryServer, true); // convert primary service name if needed
		if ((primserver.empty() == false) && (m_pobjUADMEnvironment->strPrimaryDatabase.empty() == false))
		{
			ITDBInfo dbinfo;
			dbinfo.SetSystem(primserver.c_str());
			dbinfo.SetDatabase(m_pobjUADMEnvironment->strPrimaryDatabase.c_str());
			if (m_pobjIFXConn->Open(dbinfo) == TRUE)
			{
				m_enmUADMConnectionType = UADM_CONNECTION_TYPE_INFORMIX_PRIMARY; // connected to primary informix server
				m_enmUADMLastConnectionType = m_enmUADMConnectionType;
				m_strUADMLastServername = m_pobjUADMEnvironment->strPrimaryServer;
				m_strUADMLastDatabase = m_pobjUADMEnvironment->strPrimaryDatabase;
				m_strUADMLastPortNo = "";
				ret = true; // successfully opened and initialized
			}
			else
			{
				ITString errortext = m_pobjIFXConn->ErrorText();
				m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::openInformixConnection - Informix primary connection error: " + std::string(errortext));
			}
		}
		if (ret == false)
		{
			// option2: connect to secondary informix server
			std::string secserver = convertConnectionData(m_pobjUADMEnvironment->strSecondaryServer, true); // convert secondary server name if needed
			if ((secserver.empty() == false) && (m_pobjUADMEnvironment->strSecondaryDatabase.empty() == false))
			{
				ITDBInfo dbinfo;
				dbinfo.SetSystem(secserver.c_str());
				dbinfo.SetDatabase(m_pobjUADMEnvironment->strSecondaryDatabase.c_str());
				if (m_pobjIFXConn->Open(dbinfo) == TRUE)
				{
					m_enmUADMConnectionType = UADM_CONNECTION_TYPE_INFORMIX_SECONDARY; // connected to secondary informix server
					m_enmUADMLastConnectionType = m_enmUADMConnectionType;
					m_strUADMLastServername = m_pobjUADMEnvironment->strSecondaryServer;
					m_strUADMLastDatabase = m_pobjUADMEnvironment->strSecondaryDatabase;
					m_strUADMLastPortNo = "";
					ret = true; // successfully opened and initialized
				}
			}
		}
		if (ret == false)
		{
			// none of the informix connection have been established, so delete the connection object again
			if (m_pobjIFXConn != NULL)
			{
				delete m_pobjIFXConn;
				m_pobjIFXConn = NULL; // release connection object
			}
		}
#endif
		return ret;
	}

	bool CUADMLoginInterface::initSession()
	{
		// debug info
		m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::initSession - start");

		if ((m_enmUADMConnectionType == UADM_CONNECTION_TYPE_NONE) || ((m_intVoodooConn < 0) && (m_pobjIFXConn == NULL)) || (m_pobjUADMEnvironment == NULL))
		{
			// no server connection established
			m_strLastErrorMessage = "Server connection not established";
			m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::initSession - " + m_strLastErrorMessage);
			return false;
		}
		// set isolation to prevent locks
		int result = executeSQL("SET ISOLATION TO DIRTY READ");
		if (result != 0)
		{
			m_strLastErrorMessage = "Failed to set isolation to dirty read";
			m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::initSession - " + m_strLastErrorMessage);
			return false;
		}
		// in case of locks, set wait to 10 secs, before error will be raised
		result = executeSQL("SET LOCK MODE TO WAIT 10");
		if (result != 0)
		{
			m_strLastErrorMessage = "Failed to set lock mode to wait 10";
			m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::initSession - " + m_strLastErrorMessage);
			return false;
		}
		// get the uadm parameters from the database
		std::string strVal = "";
		std::string strSQL = "SELECT p.paravalue FROM pplogpara p, ppapplication a WHERE p.applicationid=a.applicationid AND a.name='UADM' AND p.para1='0' AND p.para2='0'";
		// get encryption type
		if (readSQLValue(strSQL + " AND p.parabez='UADM_ENC_TYPE'", strVal) == 0)
		{
			int enctype = atoi(strVal.c_str());
			if (enctype == 1)
			{
				m_enmUADMEncryptionType = UADM_PASSWORD_ENCRYPTION_TYPE_PHOENIX;
			}
			else if (enctype == 2)
			{
				m_enmUADMEncryptionType = UADM_PASSWORD_ENCRYPTION_TYPE_AES;
			}
		}
		m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::initSession - Encryption Type: " + std::to_string((long long)m_enmUADMEncryptionType));
		// get max login retries
		strVal = "";
		if (readSQLValue(strSQL + " AND p.parabez='UADM_LOGIN_MAX_RETRIES'", strVal) == 0)
		{
			m_intLoginMaxRetries = atoi(strVal.c_str());
		}
		m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::initSession - Max Login Retries: " + std::to_string((long long)m_intLoginMaxRetries));
		// get lock flag in case of max login retries reached
		strVal = "";
		if (readSQLValue(strSQL + " AND p.parabez='UADM_LOGIN_MAX_RETRIES_LOCK'", strVal) == 0)
		{
			m_blnLockUserAfterMaxRetries = (atoi(strVal.c_str()) == 1);
		}
		m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::initSession - Lock User after max retries: " + std::to_string((long long)m_blnLockUserAfterMaxRetries));
		// get password history count
		strVal = "";
		if (readSQLValue(strSQL + " AND p.parabez='UADM_PWD_HISTORY_COUNT'", strVal) == 0)
		{
			m_intPasswordHistoryCount = atoi(strVal.c_str());
		}
		m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::initSession - Password history count: " + std::to_string((long long)m_intPasswordHistoryCount));
		// get max password validity days
		strVal = "";
		if (readSQLValue(strSQL + " AND p.parabez='UADM_PWD_MAX_VALID_DAYS'", strVal) == 0)
		{
			m_intMaxPasswordValidityDays = atoi(strVal.c_str());
		}
		m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::initSession - Password max validity days: " + std::to_string((long long)m_intMaxPasswordValidityDays));
		// get allow local login flag
		strVal = "";
		if (readSQLValue(strSQL + " AND p.parabez='UADM_ALLOW_LOCAL_LOGIN'", strVal) == 0)
		{
			m_blnAllowLocalLogin = (atoi(strVal.c_str()) == 1);
		}
		m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::initSession - Allow local login: " + std::to_string((long long)m_blnAllowLocalLogin));
		// debug info
		m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::initSession - done");
		return true;
	}

	bool CUADMLoginInterface::isConnected(UADM_APP_ENVIRONMENT* pobjUADMEnvironment)
	{
		// check if the connection is established
		if ((m_enmUADMConnectionType == UADM_CONNECTION_TYPE_NONE) || (m_pobjUADMEnvironment == NULL) || (m_pobjUADMEnvironment != pobjUADMEnvironment) || ((m_intVoodooConn < 0) && (m_pobjIFXConn == NULL)))
		{
			return false; // no connection to this environment established
		}
		return true; // connection is established
	}

	std::string CUADMLoginInterface::getLastUADMConnectionType()
	{
		if (m_enmUADMLastConnectionType == UADM_CONNECTION_TYPE_VOODOO_PRIMARY)
		{
			return "Voodoo Primary";
		}
		else if (m_enmUADMLastConnectionType == UADM_CONNECTION_TYPE_VOODOO_SECONDARY)
		{
			return "Voodoo Secondary";
		}
		else if (m_enmUADMLastConnectionType == UADM_CONNECTION_TYPE_INFORMIX_PRIMARY)
		{
			return "Informix Primary";
		}
		else if (m_enmUADMLastConnectionType == UADM_CONNECTION_TYPE_INFORMIX_SECONDARY)
		{
			return "Informix Secondary";
		}
		return "None";
	}

	std::string CUADMLoginInterface::getLastUADMEncryptionType()
	{
		if (m_enmUADMEncryptionType == UADM_PASSWORD_ENCRYPTION_TYPE_PHOENIX)
		{
			return "PHX";
		}
		else if (m_enmUADMEncryptionType == UADM_PASSWORD_ENCRYPTION_TYPE_AES)
		{
			return "AES";
		}
		return "None";
	}

	bool CUADMLoginInterface::getAppEntries(std::string m_strAppName, std::vector<UADM_APP_LANGUAGE*>* pvecLanguages, std::vector<UADM_APP_ENTRY*>* pvecEntries)
	{
		// debug info
		m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::getAppEntries - start");

		if ((m_enmUADMConnectionType == UADM_CONNECTION_TYPE_NONE) || (m_pobjUADMEnvironment == NULL) || ((m_intVoodooConn < 0) && (m_pobjIFXConn == NULL)) || (m_strAppName.empty() == true) || (pvecLanguages == NULL) || (pvecEntries == NULL))
		{
			m_strLastErrorMessage = "Invalid parameters for getting application entries from UADM";
			m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::getAppEntries - " + m_strLastErrorMessage);
			return false; // no connection established or invalid parameters
		}
		// getting the default entries for this app from the uadm database
		if (verifyApplication(m_strAppName) == false)
		{
			m_strLastErrorMessage = "Application " + m_strAppName + " not found";
			m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::getAppEntries - " + m_strLastErrorMessage);
			return false; // application not found
		}
		long         sqlStatus = 0;
		int          sqlHandle;
		std::string  sqlQuery;

		// get the default languages for this application
		sqlQuery = "SELECT l.languageid as languageid, l.name as languagename FROM pploginlanguage l, ppcorapplang c WHERE c.applicationid = " + std::to_string((long long)m_lngAppID) + " AND c.languageid = l.languageid ORDER BY languageid";
		// Voodoo
		if (((m_enmUADMConnectionType == UADM_CONNECTION_TYPE_VOODOO_PRIMARY) || (m_enmUADMConnectionType == UADM_CONNECTION_TYPE_VOODOO_SECONDARY)) && (m_intVoodooConn >= 0))
		{
			char tmpLanguage[3];
			char tmpLanguageName[31];
			sqlStatus = SqlOpenCs(&sqlHandle, sqlQuery.c_str());
			if (sqlStatus == SQL_SUCCESS)
			{
				do  // loop result
				{
					// fetch mapping must be exact to the column sizes in the database
					sqlStatus = SqlFetch(sqlHandle, 1, &tmpLanguage, &tmpLanguageName, NULL);
					if (sqlStatus == SQL_SUCCESS)
					{
						// store in vector
						UADM_APP_LANGUAGE* pLang = new UADM_APP_LANGUAGE();
						pLang->strLanguage = toUpper(trim(std::string(tmpLanguage)));
						pLang->strLangName = trim(std::string(tmpLanguageName));
						pLang->strLangFile = ""; // will be filled later
						pvecLanguages->push_back(pLang);
					}
				} while (sqlStatus == SQL_SUCCESS);
			}
			else
			{
				m_strLastErrorMessage = "Failed to read languages for application " + m_strAppName;
				m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::getAppEntries - " + m_strLastErrorMessage);
				return false;
			}
			SqlCloseCs(sqlHandle);
		}
		// Informix
		else if (((m_enmUADMConnectionType == UADM_CONNECTION_TYPE_INFORMIX_PRIMARY) || (m_enmUADMConnectionType == UADM_CONNECTION_TYPE_INFORMIX_SECONDARY)) && (m_pobjIFXConn != NULL))
		{
#ifndef EXLUDE_INFORMIX
			// execute SQL via informix connection
			ITQuery dbQuery(*m_pobjIFXConn);
			ITSet* dbResultSet = dbQuery.ExecToSet(ITString(sqlQuery.c_str()));
			ITRow* dbRow = NULL;
			if (dbResultSet != NULL)
			{
				while ((dbRow = (ITRow*)dbResultSet->Fetch()) != NULL)
				{
					// store in vector
					UADM_APP_LANGUAGE* pLang = new UADM_APP_LANGUAGE();
					pLang->strLanguage = toUpper(trim(std::string(dbRow->Column(0)->Printable().Data())));
					pLang->strLangName = trim(std::string(dbRow->Column(1)->Printable().Data()));
					pLang->strLangFile = ""; // will be filled later
					pvecLanguages->push_back(pLang);
					dbRow->Release();
				}
				dbResultSet->Close();
			}
#endif
		}

		// SELECT_REGION_BY_APPLICATION_ID
		sqlQuery = "SELECT DISTINCT r.regionid as areaid, r.name as areaname FROM ppregion r, ppcorappbranchreg c WHERE c.applicationid = " + std::to_string((long long)m_lngAppID) + " and c.regionid = r.regionid ORDER BY r.name";

		// Voodoo
		if (((m_enmUADMConnectionType == UADM_CONNECTION_TYPE_VOODOO_PRIMARY) || (m_enmUADMConnectionType == UADM_CONNECTION_TYPE_VOODOO_SECONDARY)) && (m_intVoodooConn >= 0))
		{
			short tmpID;
			char  tmpName[31];

			sqlStatus = SqlOpenCs(&sqlHandle, sqlQuery.c_str());
			if (sqlStatus == SQL_SUCCESS)
			{
				do  // loop result
				{
					// fetch mapping must be exact to the column sizes in the database
					sqlStatus = SqlFetch(sqlHandle, 1, &tmpID, &tmpName, NULL);
					if (sqlStatus == SQL_SUCCESS)
					{
						// store in entry vector
						UADM_APP_ENTRY* pEntry = new UADM_APP_ENTRY();
						pEntry->intEntryNo = (int)pvecEntries->size(); // set the entry number
						pEntry->strEntryName = ""; // set the entry name, will be filled later
						pEntry->strEnvironment = m_pobjUADMEnvironment->strEnvironment; // set the environment from the environment object
						pEntry->strCountry = resolveCountry(tmpID); // set the country based on the region branch
						pEntry->intRegionBranchNo = tmpID;
						pEntry->strRegionBranchNo = std::to_string((long long)tmpID); // convert to string
						pEntry->strRegionBranchName = trim(std::string(tmpName));
						pEntry->strServer = ""; // dependend on region branch, will be filled later
						pEntry->strDatabase = ""; // dependend on region branch, will be filled later
						pEntry->strServiceName = ""; // dependend on region branch, will be filled later
						pEntry->intPortNo = 0; // dependend on region branch, will be filled later
						pEntry->strPortNo = ""; // dependend on region branch, will be filled later
						pvecEntries->push_back(pEntry);
					}
				} 
				while (sqlStatus == SQL_SUCCESS);
			}
			else
			{
				m_strLastErrorMessage = "Failed to read regions for application " + m_strAppName;
				m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::getAppEntries - " + m_strLastErrorMessage);
				return false;
			}
			SqlCloseCs(sqlHandle);
		}
		// Informix
		else if (((m_enmUADMConnectionType == UADM_CONNECTION_TYPE_INFORMIX_PRIMARY) || (m_enmUADMConnectionType == UADM_CONNECTION_TYPE_INFORMIX_SECONDARY)) && (m_pobjIFXConn != NULL))
		{
#ifndef EXLUDE_INFORMIX
			// execute SQL via informix connection
			ITQuery dbQuery(*m_pobjIFXConn);
			ITSet* dbResultSet = dbQuery.ExecToSet(ITString(sqlQuery.c_str()));
			ITRow* dbRow = NULL;
			if (dbResultSet != NULL)
			{
				while ((dbRow = (ITRow*)dbResultSet->Fetch()) != NULL)
				{
					// store in entry vector
					UADM_APP_ENTRY* pEntry = new UADM_APP_ENTRY();
					pEntry->intEntryNo = (int)pvecEntries->size(); // set the entry number
					pEntry->strEntryName = ""; // set the entry name, will be filled later
					pEntry->strEnvironment = m_pobjUADMEnvironment->strEnvironment; // set the environment from the environment object
					pEntry->strCountry = resolveCountry(atoi(dbRow->Column(0)->Printable().Data())); // set the country based on the region branch
					pEntry->intRegionBranchNo = atoi(dbRow->Column(0)->Printable().Data());
					pEntry->strRegionBranchNo = std::string(dbRow->Column(0)->Printable().Data()); // convert to string
					pEntry->strRegionBranchName = trim(std::string(dbRow->Column(1)->Printable().Data()));
					pEntry->strServer = ""; // dependend on region branch, will be filled later
					pEntry->strDatabase = ""; // dependend on region branch, will be filled later
					pEntry->strServiceName = ""; // dependend on region branch, will be filled later
					pEntry->intPortNo = 0; // dependend on region branch, will be filled later
					pEntry->strPortNo = ""; // dependend on region branch, will be filled later
					pvecEntries->push_back(pEntry);
					dbRow->Release();
				}
				dbResultSet->Close();
			}
			else
			{
				m_strLastErrorMessage = "Failed to read regions for application " + m_strAppName;
				m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::getAppEntries - " + m_strLastErrorMessage);
				return false;
			}
#endif
		}

		// SELECT_BRANCH_BY_APPLICATION_ID
		sqlQuery = "SELECT DISTINCT b.branchid as areaid, b.name as areaname FROM ppbranch b, ppcorappbranchreg c WHERE c.applicationid = " + std::to_string((long long)m_lngAppID) + " and c.branchid = b.branchid ORDER BY b.branchid";

		// Voodoo
		if (((m_enmUADMConnectionType == UADM_CONNECTION_TYPE_VOODOO_PRIMARY) || (m_enmUADMConnectionType == UADM_CONNECTION_TYPE_VOODOO_SECONDARY)) && (m_intVoodooConn >= 0))
		{
			short tmpID;
			char  tmpName[31];
			sqlStatus = SqlOpenCs(&sqlHandle, sqlQuery.c_str());
			if (sqlStatus == SQL_SUCCESS)
			{
				do  // loop result
				{
					// fetch mapping must be exact to the column sizes in the database
					sqlStatus = SqlFetch(sqlHandle, 1, &tmpID, &tmpName, NULL);
					if (sqlStatus == SQL_SUCCESS)
					{
						// store in entry vector
						UADM_APP_ENTRY* pEntry = new UADM_APP_ENTRY();
						pEntry->intEntryNo = (int)pvecEntries->size(); // set the entry number
						pEntry->strEntryName = ""; // set the entry name, will be filled later
						pEntry->strEnvironment = m_pobjUADMEnvironment->strEnvironment; // set the environment from the environment object
						pEntry->strCountry = resolveCountry(tmpID); // set the country based on the region branch
						pEntry->intRegionBranchNo = tmpID;
						pEntry->strRegionBranchNo = std::to_string((long long)tmpID); // convert to string
						pEntry->strRegionBranchName = trim(std::string(tmpName));
						pEntry->strServer = ""; // dependend on region branch, will be filled later
						pEntry->strDatabase = ""; // dependend on region branch, will be filled later
						pEntry->strServiceName = ""; // dependend on region branch, will be filled later
						pEntry->intPortNo = 0; // dependend on region branch, will be filled later
						pEntry->strPortNo = ""; // dependend on region branch, will be filled later
						pvecEntries->push_back(pEntry);
					}
				} while (sqlStatus == SQL_SUCCESS);
			}
			else
			{
				m_strLastErrorMessage = "Failed to read branches for application " + m_strAppName;
				m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::getAppEntries - " + m_strLastErrorMessage);
				return false;
			}
			SqlCloseCs(sqlHandle);
		}
		// Informix
		else if (((m_enmUADMConnectionType == UADM_CONNECTION_TYPE_INFORMIX_PRIMARY) || (m_enmUADMConnectionType == UADM_CONNECTION_TYPE_INFORMIX_SECONDARY)) && (m_pobjIFXConn != NULL))
		{
#ifndef EXLUDE_INFORMIX
			// execute SQL via informix connection
			ITQuery dbQuery(*m_pobjIFXConn);
			ITSet* dbResultSet = dbQuery.ExecToSet(ITString(sqlQuery.c_str()));
			ITRow* dbRow = NULL;
			if (dbResultSet != NULL)
			{
				while ((dbRow = (ITRow*)dbResultSet->Fetch()) != NULL)
				{
					// store in entry vector
					UADM_APP_ENTRY* pEntry = new UADM_APP_ENTRY();
					pEntry->intEntryNo = (int)pvecEntries->size(); // set the entry number
					pEntry->strEntryName = ""; // set the entry name, will be filled later
					pEntry->strEnvironment = m_pobjUADMEnvironment->strEnvironment; // set the environment from the environment object
					pEntry->strCountry = resolveCountry(atoi(dbRow->Column(0)->Printable().Data())); // set the country based on the region branch
					pEntry->intRegionBranchNo = atoi(dbRow->Column(0)->Printable().Data());
					pEntry->strRegionBranchNo = std::string(dbRow->Column(0)->Printable().Data()); // convert to string
					pEntry->strRegionBranchName = trim(std::string(dbRow->Column(1)->Printable().Data()));
					pEntry->strServer = ""; // dependend on region branch, will be filled later
					pEntry->strDatabase = ""; // dependend on region branch, will be filled later
					pEntry->strServiceName = ""; // dependend on region branch, will be filled later
					pEntry->intPortNo = 0; // dependend on region branch, will be filled later
					pEntry->strPortNo = ""; // dependend on region branch, will be filled later
					pvecEntries->push_back(pEntry);
					dbRow->Release();
				}
				dbResultSet->Close();
			}
			else
			{
				m_strLastErrorMessage = "Failed to read branches for application " + m_strAppName;
				m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::getAppEntries - " + m_strLastErrorMessage);
				return false;
			}
#endif
		}

		// debug info
		m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::getAppEntries - done with " + std::to_string((long long)pvecLanguages->size()) + " languages and " + std::to_string((long long)pvecEntries->size()) + " region/branch entries");
		return true;
	}

	bool CUADMLoginInterface::findAndSetLanguage(std::vector<UADM_APP_LANGUAGE*>* pvecLanguages, std::string strLanguage)
	{
		if ((pvecLanguages == NULL) || (strLanguage.empty() == true))
		{
			m_strLastErrorMessage = "Invalid parameters for finding uadm language";
			m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::findAndSetLanguage - " + m_strLastErrorMessage);
			return false;
		}
		// find the entry in the given vector
		std::vector<UADM_APP_LANGUAGE*>::iterator it;
		for (it = pvecLanguages->begin(); it != pvecLanguages->end(); ++it)
		{
			UADM_APP_LANGUAGE* pLang = *it;
			if ((pLang != NULL) && (toUpper(pLang->strLanguage) == toUpper(strLanguage)))
			{
				// found the entry, set it as current entry
				m_strLanguage = pLang->strLanguage;
				m_strLanguageName = pLang->strLangName;
				return true; // successfully found and set
			}
		}
		m_strLastErrorMessage = "Given uadm language '" + strLanguage + "' not found in loaded application data.";
		m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::findAndSetLanguage - " + m_strLastErrorMessage);
		return false; // not found
	}

	bool CUADMLoginInterface::findAndSetUADMEntry(std::vector<UADM_APP_ENTRY*>* pvecEntries, std::string strEnv, std::string strCountry, int intRegionBranch)
	{
		if ((pvecEntries == NULL) || (strEnv.empty() == true) || (intRegionBranch <= 0))
		{
			m_strLastErrorMessage = "Invalid parameters for finding uadm country and region/branch";
			m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::findAndSetUADMEntry - " + m_strLastErrorMessage);
			return false;
		}
		// find the entry in the given vector
		std::vector<UADM_APP_ENTRY*>::iterator it;
		for (it = pvecEntries->begin(); it != pvecEntries->end(); ++it)
		{
			UADM_APP_ENTRY* pEntry = *it;
			if ((pEntry != NULL) && (toUpper(pEntry->strEnvironment) == toUpper(strEnv)) && (pEntry->intRegionBranchNo == intRegionBranch))
			{
				if ((pEntry->strCountry.empty() == false) && (strCountry.empty() == false) && (toUpper(pEntry->strCountry) != toUpper(strCountry)))
				{
					continue; // country does not match, continue searching
				}
				// found the entry, set it as current entry
				m_pobjUADMEntry = pEntry;
				return true; // successfully found and set
			}
		}
		m_strLastErrorMessage = "Given uadm country '" + strCountry + "' and region / branch " + std::to_string((long long)intRegionBranch) + " not found in environment '" + strEnv + "'.";
		m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::findAndSetUADMEntry - " + m_strLastErrorMessage);
		return false; // not found
	}

	std::string CUADMLoginInterface::getEnvironment()
	{
		if (m_pobjUADMEntry != NULL)
		{
			return m_pobjUADMEntry->strEnvironment;
		}
		else if (m_pobjUADMEnvironment != NULL)
		{
			return m_pobjUADMEnvironment->strEnvironment;
		}
		return "";
	};

	bool CUADMLoginInterface::verifyLocalUser(std::string strAppName, int intBranchNo, std::string strUsername, std::string strPassword)
	{
		m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::verifyLocalUser - start");

		if ((m_enmUADMConnectionType == UADM_CONNECTION_TYPE_NONE) || (m_pobjUADMEnvironment == NULL) || ((m_intVoodooConn < 0) && (m_pobjIFXConn == NULL)) || (intBranchNo <= 0) || (strUsername.empty() == true))
		{
			m_strLastErrorMessage = "Invalid parameters for local user verification";
			m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::verifyLocalUser - " + m_strLastErrorMessage);
			return false; // no connection established or invalid parameters
		}
		bool ret = false;
		std::string strSQL;
		std::string strLocalPwd = "";
		long result = 0;
		// get application infos first
		if (verifyApplication(strAppName) == false)
		{
			m_strLastErrorMessage = "Application " + strAppName + " not found";
			m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::verifyLocalUser - " + m_strLastErrorMessage);
			return false; // application not found
		}
		// get the user data based on local username
		strSQL = "SELECT u.username, u.password, u.pwd_enc, up2.userparavalue as localpwd FROM ppcorappbruser up1, ppuserpara upn1, ppuser u ";
		// check as well for local password, might be not filled, so left join
		strSQL += "LEFT JOIN ppcorappbruser up2 on up2.userid=u.userid AND up2.applicationid=" + std::to_string((long long)m_lngAppID) + " AND up2.branchregionid=" + std::to_string((long long)intBranchNo) + " AND up2.userparaid=(SELECT upn2.userparaid FROM ppuserpara upn2 WHERE UPPER(upn2.name)='LOCALPASSWD') ";
		// continue with where clause
		strSQL += "WHERE up1.userid=u.userid AND up1.applicationid=" + std::to_string((long long)m_lngAppID) + " AND up1.branchregionid=" + std::to_string((long long)intBranchNo) + " AND up1.userparaid=upn1.userparaid AND  UPPER(upn1.name)='LOCALUSERNAME' AND  UPPER(up1.userparavalue)='" + toUpper(strUsername) + "' ";
		// Voodoo
		if (((m_enmUADMConnectionType == UADM_CONNECTION_TYPE_VOODOO_PRIMARY) || (m_enmUADMConnectionType == UADM_CONNECTION_TYPE_VOODOO_SECONDARY)) && (m_intVoodooConn >= 0))
		{
			char tmpUsername[21];
			char tmpPassword[9];
			char tmpPWDEnc[255];
			char tmpLocalPwd[255];
			// read mapping must be exact to the column sizes in the database
			result = SqlRead(strSQL.c_str(), &tmpUsername, &tmpPassword, &tmpPWDEnc, &tmpLocalPwd, NULL);
			if (result == 0)
			{
				// correct, take over the user data into members
				m_strUsername = trim(std::string(tmpUsername));
				if (m_blnAllowLocalLogin == true)
				{
					std::string tmpPwdOld = std::string(tmpPassword);
					std::string tmpPwdNew = trim(std::string(tmpPWDEnc));
					m_strEncPwd = tmpPwdNew;
					// if new one is not filled, take old
					if (m_strEncPwd.empty() == true)
					{
						m_strEncPwd = tmpPwdOld;
					}
					strLocalPwd = std::string(tmpLocalPwd);
				}
				ret = true;
			}
		}
		// Informix
		else if (((m_enmUADMConnectionType == UADM_CONNECTION_TYPE_INFORMIX_PRIMARY) || (m_enmUADMConnectionType == UADM_CONNECTION_TYPE_INFORMIX_SECONDARY)) && (m_pobjIFXConn != NULL))
		{
#ifndef EXLUDE_INFORMIX
			// execute SQL via informix connection
			ITQuery dbQuery(*m_pobjIFXConn);
			ITRow* dbRow = dbQuery.ExecOneRow(ITString(strSQL.c_str()));
			if (dbRow != NULL)
			{
				// correct, take over the user data into members
				m_strUsername = trim(std::string(dbRow->Column(0)->Printable().Data()));
				if (m_blnAllowLocalLogin == true)
				{
					// make sure that tmpPassword is not trimmed (fix length encryption), but tmpPWDEnc is trimmed (dynamic length encryption)
					std::string tmpPwdOld = std::string(dbRow->Column(1)->Printable().Data()); // fixed length, do not trim
					std::string tmpPwdNew = trim(std::string(dbRow->Column(2)->Printable().Data())); // dynamic length, do trim

					m_strEncPwd = tmpPwdNew;
					// if new one is not filled, take old
					if (m_strEncPwd.empty() == true)
					{
						m_strEncPwd = tmpPwdOld;
					}
					strLocalPwd = std::string(dbRow->Column(3)->Printable().Data());
					if (strLocalPwd.compare("null") == 0)
					{
						strLocalPwd = ""; // handle null value
					}
				}
				ret = true;
				dbRow->Release();
			}
#endif
		}
		if (ret == false)
		{
			m_strLastErrorMessage = "User verification failed for local user " + strUsername + " on branch " + std::to_string((long long)intBranchNo) + " for app " + strAppName;
			m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::verifyLocalUser - " + m_strLastErrorMessage);
			return false;
		}
		if (trim(strPassword).empty() == true)
		{
			m_strEncPwd = ""; // do not allow automatic login with empty password
			// but it still should return true if user is found
		}
		if (trim(strLocalPwd).empty() == true)
		{
			m_strEncPwd = ""; // do not allow automatic login with empty password
			// but it still should return true if user is found
		}
		if ((m_blnAllowLocalLogin == true) && (m_strEncPwd.empty() == false))
		{
			// need to encrypt the given password for comparison
			std::string encPwdOld = encryptOld(strPassword);
			if (encPwdOld.compare(strLocalPwd.substr(0, encPwdOld.length())) != 0)
			{
				m_strEncPwd = ""; // do not allow automatic login with empty password
				// but it still should return true if user is found
			}
		}

		// debug info
		m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::verifyLocalUser - done");
		return ret;
	}

	bool CUADMLoginInterface::verifyUser(std::string strUsername, std::string strPassword, bool pwdEncrypted)
	{
		// debug info
		m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::verifyUser - start");

		if ((m_enmUADMConnectionType == UADM_CONNECTION_TYPE_NONE) || (m_pobjUADMEnvironment == NULL) || ((m_intVoodooConn < 0) && (m_pobjIFXConn == NULL)) || (strUsername.empty() == true) || (strPassword.empty() == true))
		{
			m_strLastErrorMessage = "Invalid parameters for user verification";
			m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::verifyUser - " + m_strLastErrorMessage);
			return false; // no connection established or invalid parameters
		}
		bool ret = false;
		std::string strSQL;
		long result = 0;

		strSQL = "SELECT userid, username, password, pwd_enc, surname, forename, description, mailuser, emailuser, ntuser, activ, changepw, lastlogin, lastpwchange, CAST(to_char(sysdate,'%Y%m%d') as Integer) FROM ppuser WHERE UPPER(username)='" + toUpper(strUsername) + "'";
		// Voodoo
		if (((m_enmUADMConnectionType == UADM_CONNECTION_TYPE_VOODOO_PRIMARY) || (m_enmUADMConnectionType == UADM_CONNECTION_TYPE_VOODOO_SECONDARY)) && (m_intVoodooConn >= 0))
		{
			long tmpUserID = 0;
			char tmpUsername[21];
			char tmpPassword[9];
			char tmpPWDEnc[255];
			char tmpSurname[31];
			char tmpForename[21];
			char tmpDescription[255];
			char tmpMail[31];
			char tmpEMail[51];
			char tmpNTUser[51];
			short tmpActive = 0;
			short tmpChangePW = 0;
			long tmpLastLogin = 0;
			long tmpLastPWChange = 0;
			long tmpCurrentDate = 0;
			// read mapping must be exact to the column sizes in the database
			result = SqlRead(strSQL.c_str(), &tmpUserID, &tmpUsername, &tmpPassword, &tmpPWDEnc, &tmpSurname, &tmpForename, &tmpDescription, &tmpMail, &tmpEMail, &tmpNTUser, &tmpActive, &tmpChangePW, &tmpLastLogin, &tmpLastPWChange, &tmpCurrentDate, NULL);
			if (result == 0)
			{
				// user found, lets check the pwd (old and new encryption)
				std::string encPwdOld = strPassword;
				std::string encPwdNew = strPassword;
				if (pwdEncrypted == false)
				{
					// need to encrypt the given password for comparison
					encPwdOld = encryptOld(strPassword);
					encPwdNew = encryptNew(strPassword, strUsername);
				}
				// make sure that tmpPassword is not trimmed (fix length encryption), but tmpPWDEnc is trimmed (dynamic length encryption)
				std::string tmpPwdNew = trim(std::string(tmpPWDEnc));

				if (((encPwdOld.empty() == false) && (strlen(tmpPassword) > 0) && (encPwdOld.compare(std::string(tmpPassword)) == 0)) ||
					((encPwdNew.empty() == false) && (tmpPwdNew.empty() == false) && (encPwdNew.compare(tmpPwdNew) == 0)))
				{
					// correct, take over the user data into members
					m_lngUserID = tmpUserID;
					m_strUsername = trim(std::string(tmpUsername));
					m_strEncPwd = trim(std::string(tmpPassword));
					// only one pwd is filled, either old or new
					if (m_strEncPwd.empty() == true)
					{
						m_strEncPwd = tmpPwdNew;
					}
					m_strFirstname = trim(std::string(tmpForename));
					m_strLastname = trim(std::string(tmpSurname));
					m_strDescription = trim(std::string(tmpDescription));
					m_strMail = trim(std::string(tmpMail));
					m_strEMail = trim(std::string(tmpEMail));
					m_strDomainAccount = trim(std::string(tmpNTUser));
					m_blnIsActive = (tmpActive == 1);
					m_blnForcePasswordChange = (tmpChangePW == 1);
					m_lngLastPasswordChangeDate = tmpLastPWChange;
					m_lngLastLoginDate = tmpLastLogin;
					m_lngCurrentDate = tmpCurrentDate;
					ret = true;
				}
			}
		}
		// Informix
		else if (((m_enmUADMConnectionType == UADM_CONNECTION_TYPE_INFORMIX_PRIMARY) || (m_enmUADMConnectionType == UADM_CONNECTION_TYPE_INFORMIX_SECONDARY)) && (m_pobjIFXConn != NULL))
		{
#ifndef EXLUDE_INFORMIX
			// execute SQL via informix connection
			ITQuery dbQuery(*m_pobjIFXConn);
			ITRow* dbRow = dbQuery.ExecOneRow(ITString(strSQL.c_str()));
			if (dbRow != NULL)
			{
				// user found, lets check the pwd (old and new encryption)
				std::string encPwdOld = strPassword;
				std::string encPwdNew = strPassword;
				if (pwdEncrypted == false)
				{
					// need to encrypt the given password for comparison
					encPwdOld = encryptOld(strPassword);
					encPwdNew = encryptNew(strPassword, strUsername);
				}
				// make sure that tmpPassword is not trimmed (fix length encryption), but tmpPWDEnc is trimmed (dynamic length encryption)
				std::string tmpPwdOld = std::string(dbRow->Column(2)->Printable().Data()); // fixed length, do not trim
				std::string tmpPwdNew = trim(std::string(dbRow->Column(3)->Printable().Data())); // dynamic length, do trim

				if (((encPwdOld.empty() == false) && (tmpPwdOld.length() > 0) && (encPwdOld.compare(tmpPwdOld) == 0)) ||
					((encPwdNew.empty() == false) && (tmpPwdNew.empty() == false) && (encPwdNew.compare(tmpPwdNew) == 0)))
				{
					// correct, take over the user data into members
					m_lngUserID = atoi(dbRow->Column(0)->Printable().Data());
					m_strUsername = trim(std::string(dbRow->Column(1)->Printable().Data()));
					m_strEncPwd = tmpPwdOld;
					// only one pwd is filled, either old or new
					if (m_strEncPwd.empty() == true)
					{
						m_strEncPwd = tmpPwdNew;
					}
					m_strFirstname = trim(std::string(dbRow->Column(5)->Printable().Data()));
					m_strLastname = trim(std::string(dbRow->Column(4)->Printable().Data()));
					m_strDescription = trim(std::string(dbRow->Column(6)->Printable().Data()));
					m_strMail = trim(std::string(dbRow->Column(7)->Printable().Data()));
					m_strEMail = trim(std::string(dbRow->Column(8)->Printable().Data()));
					m_strDomainAccount = trim(std::string(dbRow->Column(9)->Printable().Data()));
					m_blnIsActive = (std::string(dbRow->Column(10)->Printable().Data()).compare("1") == 0);
					m_blnForcePasswordChange = (std::string(dbRow->Column(11)->Printable().Data()).compare("1") == 0);
					m_lngLastLoginDate = atoi(dbRow->Column(12)->Printable().Data());
					m_lngLastPasswordChangeDate = atoi(dbRow->Column(13)->Printable().Data());
					m_lngCurrentDate = atoi(dbRow->Column(14)->Printable().Data());
					ret = true;
				}
				dbRow->Release();
			}
#endif
		}
		if (ret == false)
		{
			m_strLastErrorMessage = "User verification failed for user " + strUsername;
			m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::verifyUser - " + m_strLastErrorMessage);
			return false;
		}
		// debug info
		m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::verifyUser - done");
		return ret;
	}

	bool CUADMLoginInterface::verifyAndLockUser(std::string strUsername)
	{
		if (m_blnLockUserAfterMaxRetries == true)
		{
			if ((m_enmUADMConnectionType == UADM_CONNECTION_TYPE_NONE) || (m_pobjUADMEnvironment == NULL) || ((m_intVoodooConn < 0) && (m_pobjIFXConn == NULL)) || (strUsername.empty() == true))
			{
				m_strLastErrorMessage = "Invalid parameters for locking user after too many login retries";
				m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::verifyAndLockUser - " + m_strLastErrorMessage);
				return false;
			}
			long         sqlStatus = 0;
			std::string  sqlQuery;
			// set the active flag to 0 to lock the user
			// prepare the SQL query
			sqlQuery = "UPDATE ppuser SET activ=0 WHERE UPPER(username)='" + toUpper(strUsername) + "'";
			sqlStatus = executeSQL(sqlQuery.c_str());
			if (sqlStatus == 0)
			{
				m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::verifyAndLockUser - User "+ strUsername + " locked after too many login retries");
			}
			else
			{
				m_strLastErrorMessage = "Failed to update active flag for user " + strUsername;
				m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::verifyAndLockUser - " + m_strLastErrorMessage);
				return false; // update failed
			}
		}
		return true;
	}

	bool CUADMLoginInterface::verifyPassword(std::string strPassword)
	{
		if (strPassword.empty() == true)
		{
			return false; // invalid parameters
		}
		bool ret = false;
		// need to encrypt the given password for comparison
		std::string encPwdOld = encryptOld(strPassword);
		std::string encPwdNew = encryptNew(strPassword, m_strUsername);
		// compare with stored encrypted password
		if (((encPwdOld.empty() == false) && (m_strEncPwd.empty() == false) && (encPwdOld.compare(m_strEncPwd) == 0)) ||
			((encPwdNew.empty() == false) && (m_strEncPwd.empty() == false) && (encPwdNew.compare(m_strEncPwd) == 0)))
		{
			ret = true; // password verified
		}
		return ret;
	}

	bool CUADMLoginInterface::getPreviousLoginData(std::string& prevUsername, std::string& prevCountry, std::string& prevRegionBranchNo, std::string& prevLanguage)
	{
		// debug info
		m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::getPreviousLoginData - start");

		if ((m_enmUADMConnectionType == UADM_CONNECTION_TYPE_NONE) || (m_pobjUADMEnvironment == NULL) || ((m_intVoodooConn < 0) && (m_pobjIFXConn == NULL)) || (m_strOSAccount.empty() == true) || (m_strAppName.empty() == true))
		{
			m_strLastErrorMessage = "Invalid parameters for previous login data retrieval";
			m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::getPreviousLoginData - " + m_strLastErrorMessage);
			return false; // no connection established or invalid parameters
		}
		bool ret = false;
		std::string strSQL;
		long result = 0;

		strSQL = "SELECT u.username, l.branchregionid, l.languageid, l.country FROM pploginprevious l, ppapplication a, ppuser u WHERE UPPER(a.name)='" + toUpper(m_strAppName) + "' and a.applicationid=l.applicationid and UPPER(l.globaluser)='" + toUpper(m_strOSAccount) + "' and l.userid=u.userid";

		// Voodoo
		if (((m_enmUADMConnectionType == UADM_CONNECTION_TYPE_VOODOO_PRIMARY) || (m_enmUADMConnectionType == UADM_CONNECTION_TYPE_VOODOO_SECONDARY)) && (m_intVoodooConn >= 0))
		{
			char tmpUsername[21];
			short tmpBranchRegionNo;
			char tmpLanguage[3];
			char tmpCountry[3];

			// read mapping must be exact to the column sizes in the database
			result = SqlRead(strSQL.c_str(), &tmpUsername, &tmpBranchRegionNo, &tmpLanguage, &tmpCountry, NULL);
			if (result == 0)
			{
				prevUsername = trim(std::string(tmpUsername));
				prevRegionBranchNo = trim(std::to_string((long long)tmpBranchRegionNo));
				prevLanguage = toUpper(trim(std::string(tmpLanguage)));
				prevCountry = trim(std::string(tmpCountry));
				ret = true;
			}
		}
		// Informix
		else if (((m_enmUADMConnectionType == UADM_CONNECTION_TYPE_INFORMIX_PRIMARY) || (m_enmUADMConnectionType == UADM_CONNECTION_TYPE_INFORMIX_SECONDARY)) && (m_pobjIFXConn != NULL))
		{
#ifndef EXLUDE_INFORMIX
			// execute SQL via informix connection
			ITQuery dbQuery(*m_pobjIFXConn);
			ITRow* dbRow = dbQuery.ExecOneRow(ITString(strSQL.c_str()));
			if (dbRow != NULL)
			{
				prevUsername = trim(std::string(dbRow->Column(0)->Printable().Data()));
				prevRegionBranchNo = trim(std::string(dbRow->Column(1)->Printable().Data()));
				prevLanguage = toUpper(trim(std::string(dbRow->Column(2)->Printable().Data())));
				prevCountry = trim(std::string(dbRow->Column(3)->Printable().Data()));
				dbRow->Release();
				ret = true;
			}
#endif
		}

		if (ret == false)
		{
			m_strLastErrorMessage = "No previous login data found for OS user " + m_strOSAccount + " and application " + m_strAppName;
			m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::getPreviousLoginData - " + m_strLastErrorMessage);
			return false;
		}
		// debug info
		m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::getPreviousLoginData - done");
		return ret;
	}

	bool CUADMLoginInterface::isPasswordChangeRequired()
	{
		bool ret = m_blnForcePasswordChange;

		// verify also the last change date to force a pwd change
		if (ret == false)
		{
			// if a pwd was never changed
			if (m_lngLastPasswordChangeDate == 0)
			{
				ret = true;
			}
			else if ((m_lngLastPasswordChangeDate > 0) && (m_lngCurrentDate > 0))
			{
				// if a pwd was older than max validity (default 90) days
				// calculate first the long <yyyymmdd> as days, this is a simple calculation with 365 days per year and 30 days per month, as average of days per month in a year is 30.42
				long lngLastChangeDays = (m_lngLastPasswordChangeDate / 10000) * 365 + ((m_lngLastPasswordChangeDate % 10000) / 100) * 30 + (m_lngLastPasswordChangeDate % 100);
				long lngCurrentDays = (m_lngCurrentDate / 10000) * 365 + ((m_lngCurrentDate % 10000) / 100) * 30 + (m_lngCurrentDate % 100);
				long diffDays = lngCurrentDays - lngLastChangeDays;
				if (diffDays > m_intMaxPasswordValidityDays)
				{
					ret = true;
				}
			}
		}
		return ret;
	}

	bool CUADMLoginInterface::verifyLastPasswordChange()
	{
		// verify the last change date to allow a pwd change only if the last change was not today or in the future
		if ((m_lngLastPasswordChangeDate > 0) && (m_lngCurrentDate > 0) && (m_blnForcePasswordChange == false))
		{
			if (m_lngLastPasswordChangeDate >= m_lngCurrentDate)
			{
				return false;
			}
		}
		return true;
	}

	bool CUADMLoginInterface::verifyApplication(std::string strAppName)
	{
		// debug info
		m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::verifyApplication - start");

		if ((m_enmUADMConnectionType == UADM_CONNECTION_TYPE_NONE) || (m_pobjUADMEnvironment == NULL) || ((m_intVoodooConn < 0) && (m_pobjIFXConn == NULL)) || (strAppName.empty() == true))
		{
			m_strLastErrorMessage = "Invalid parameters for application verification";
			m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::verifyApplication - " + m_strLastErrorMessage);
			return false; // no connection established or invalid parameters
		}
		bool ret = false;
		std::string strSQL;
		long result = 0;

		// make application name case insensitive to upper case
		strSQL = "SELECT applicationid, name, description FROM ppapplication WHERE UPPER(name)='" + toUpper(strAppName) + "'";

		// Voodoo
		if (((m_enmUADMConnectionType == UADM_CONNECTION_TYPE_VOODOO_PRIMARY) || (m_enmUADMConnectionType == UADM_CONNECTION_TYPE_VOODOO_SECONDARY)) && (m_intVoodooConn >= 0))
		{
			long tmpAppID = 0;
			char tmpAppName[31];
			char tmpAppDesc[255];

			// read mapping must be exact to the column sizes in the database
			result = SqlRead(strSQL.c_str(), &tmpAppID, &tmpAppName, &tmpAppDesc, NULL);
			if (result == 0)
			{
				// correct, take over the user data into members
				m_lngAppID = tmpAppID;
				m_strAppName = trim(std::string(tmpAppName));
				m_strAppDesc = trim(std::string(tmpAppDesc));
				ret = true;
			}
		}
		// Informix
		else if (((m_enmUADMConnectionType == UADM_CONNECTION_TYPE_INFORMIX_PRIMARY) || (m_enmUADMConnectionType == UADM_CONNECTION_TYPE_INFORMIX_SECONDARY)) && (m_pobjIFXConn != NULL))
		{
#ifndef EXLUDE_INFORMIX
			// execute SQL via informix connection
			ITQuery dbQuery(*m_pobjIFXConn);
			ITRow* dbRow = dbQuery.ExecOneRow(ITString(strSQL.c_str()));
			if (dbRow != NULL)
			{
				// read the three entries
				m_lngAppID = atoi(dbRow->Column(0)->Printable().Data());
				m_strAppName = trim(std::string(dbRow->Column(1)->Printable().Data()));
				m_strAppDesc = trim(std::string(dbRow->Column(2)->Printable().Data()));
				dbRow->Release();
				ret = true;
			}
#endif
		}

		if (ret == false)
		{
			m_strLastErrorMessage = "Application verification failed for application " + strAppName;
			m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::verifyApplication - " + m_strLastErrorMessage);
			return false;
		}
		// debug info
		m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::verifyApplication - done");
		return ret;
	}

	bool CUADMLoginInterface::getApplicationRights(std::vector<UADM_APP_PERMISSION*>* pvecPermissions, std::vector<UADM_USER_ROLE*>* pvecUserRoles, int intRegionBranch, bool blnRegionBranchOnly)
	{
		// debug info
		m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::getApplicationRights - start");

		if ((m_enmUADMConnectionType == UADM_CONNECTION_TYPE_NONE) || (m_pobjUADMEnvironment == NULL) || ((m_intVoodooConn < 0) && (m_pobjIFXConn == NULL)) ||
			(m_lngUserID <= 0) || (m_lngAppID <= 0) || (pvecPermissions == NULL))
		{
			m_strLastErrorMessage = "Invalid parameters for application rights retrieval";
			m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::getApplicationRights - " + m_strLastErrorMessage);
			return false;
		}
		long         sqlStatus = 0;
		int          sqlHandle;
		std::string  sqlQuery;

		// get at first all groups of this user and app combination
		std::string strGroupIDs = "";
		sqlQuery = "SELECT groupid FROM ppcorappusergroup ug WHERE ug.applicationid=" + std::to_string((long long)m_lngAppID) + " AND ug.userid=" + std::to_string((long long)m_lngUserID);

		// Voodoo
		if (((m_enmUADMConnectionType == UADM_CONNECTION_TYPE_VOODOO_PRIMARY) || (m_enmUADMConnectionType == UADM_CONNECTION_TYPE_VOODOO_SECONDARY)) && (m_intVoodooConn >= 0))
		{
			short tmpGroupID;
			sqlStatus = SqlOpenCs(&sqlHandle, sqlQuery.c_str());
			if (sqlStatus == SQL_SUCCESS)
			{
				do  // loop result
				{
					// fetch mapping must be exact to the column sizes in the database
					sqlStatus = SqlFetch(sqlHandle, 1, &tmpGroupID, NULL);
					if (sqlStatus == SQL_SUCCESS)
					{
						if (strGroupIDs.empty() == false)
						{
							strGroupIDs += ", ";
						}
						strGroupIDs += std::to_string((long long)tmpGroupID);
					}
				}
				while (sqlStatus == SQL_SUCCESS);
				SqlCloseCs(sqlHandle);
			}
			else
			{
				SqlCloseCs(sqlHandle);
				m_strLastErrorMessage = "Failed to read groups for user " + m_strUsername + " and application " + m_strAppName;
				m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::getApplicationRights - " + m_strLastErrorMessage);
				return false;
			}
		}
		// Informix
		else if (((m_enmUADMConnectionType == UADM_CONNECTION_TYPE_INFORMIX_PRIMARY) || (m_enmUADMConnectionType == UADM_CONNECTION_TYPE_INFORMIX_SECONDARY)) && (m_pobjIFXConn != NULL))
		{
#ifndef EXLUDE_INFORMIX
			// execute SQL via informix connection
			ITQuery dbQuery(*m_pobjIFXConn);
			ITSet* dbResultSet = dbQuery.ExecToSet(ITString(sqlQuery.c_str()));
			ITRow* dbRow = NULL;
			if (dbResultSet != NULL)
			{
				while ((dbRow = (ITRow*)dbResultSet->Fetch()) != NULL)
				{
					if (strGroupIDs.empty() == false)
					{
						strGroupIDs += ", ";
					}
					strGroupIDs += trim(std::string(dbRow->Column(0)->Printable().Data()));
					dbRow->Release();
				}
				dbResultSet->Close();
			}
			else
			{
				m_strLastErrorMessage = "Failed to read groups for user " + m_strUsername + " and application " + m_strAppName;
				m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::getApplicationRights - " + m_strLastErrorMessage);
				return false;
			}
#endif
		}
		// now read the groupids and names as user roles for applications use, if there are any and if the user roles vector is provided
		if ((strGroupIDs.empty() == false) && (pvecUserRoles != NULL))
		{
			sqlQuery = "SELECT groupid, name FROM ppgroup WHERE groupid IN (" + strGroupIDs + ") ORDER BY groupid ";
			// Voodoo
			if (((m_enmUADMConnectionType == UADM_CONNECTION_TYPE_VOODOO_PRIMARY) || (m_enmUADMConnectionType == UADM_CONNECTION_TYPE_VOODOO_SECONDARY)) && (m_intVoodooConn >= 0))
			{
				short tmpGroupID;
				char tmpGroupName[31];
				sqlStatus = SqlOpenCs(&sqlHandle, sqlQuery.c_str());
				if (sqlStatus == SQL_SUCCESS)
				{
					do  // loop result
					{
						// fetch mapping must be exact to the column sizes in the database
						sqlStatus = SqlFetch(sqlHandle, 1, &tmpGroupID, &tmpGroupName, NULL);
						if (sqlStatus == SQL_SUCCESS)
						{
							UADM_USER_ROLE* pRole = new UADM_USER_ROLE();
							pRole->intUserID = (int)m_lngUserID;
							pRole->intRoleID = (int)tmpGroupID;
							pRole->strRoleName = trim(std::string(tmpGroupName));
							pvecUserRoles->push_back(pRole);
						}
					} while (sqlStatus == SQL_SUCCESS);
					SqlCloseCs(sqlHandle);
				}
				else
				{
					SqlCloseCs(sqlHandle);
					m_strLastErrorMessage = "Failed to read group names (roles) for user " + m_strUsername + " and application " + m_strAppName;
					m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::getApplicationRights - " + m_strLastErrorMessage);
					return false;
				}
			}
			// Informix
			else if (((m_enmUADMConnectionType == UADM_CONNECTION_TYPE_INFORMIX_PRIMARY) || (m_enmUADMConnectionType == UADM_CONNECTION_TYPE_INFORMIX_SECONDARY)) && (m_pobjIFXConn != NULL))
			{
#ifndef EXLUDE_INFORMIX
				// execute SQL via informix connection
				ITQuery dbQuery(*m_pobjIFXConn);
				ITSet* dbResultSet = dbQuery.ExecToSet(ITString(sqlQuery.c_str()));
				ITRow* dbRow = NULL;
				if (dbResultSet != NULL)
				{
					while ((dbRow = (ITRow*)dbResultSet->Fetch()) != NULL)
					{
						UADM_USER_ROLE* pRole = new UADM_USER_ROLE();
						pRole->intUserID = (int)m_lngUserID;
						pRole->intRoleID = atoi(std::string(dbRow->Column(0)->Printable().Data()).c_str());
						pRole->strRoleName = trim(std::string(dbRow->Column(1)->Printable().Data()));
						pvecUserRoles->push_back(pRole);
						dbRow->Release();
					}
					dbResultSet->Close();
				}
				else
				{
					m_strLastErrorMessage = "Failed to read group names (roles) for user " + m_strUsername + " and application " + m_strAppName;
					m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::getApplicationRights - " + m_strLastErrorMessage);
					return false;
				}
#endif
			}
		}
		// debug info
		m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::getApplicationRights - reading groups done");

		// now load all permissions for the app for each module either for the user or the groups where the user is granted to
		sqlQuery = "SELECT ar.userid, ar.groupid, ar.rightname, ar.moduleid, ar.applicationid, ar.branchregionid FROM ppappright ar WHERE ar.applicationid=" + std::to_string((long long)m_lngAppID);
		if ((blnRegionBranchOnly == true) && (intRegionBranch > 0))
		{
			sqlQuery += " AND ar.branchregionid=" + std::to_string((long long)intRegionBranch);
		}
		sqlQuery += " AND (ar.userid=" + std::to_string((long long)m_lngUserID);
		if (strGroupIDs.empty() == false)
		{
			sqlQuery += " or ar.groupid in(" + strGroupIDs + ") ";
		}
		sqlQuery += ") ORDER BY ar.moduleid, ar.rightname";

		// Voodoo
		if (((m_enmUADMConnectionType == UADM_CONNECTION_TYPE_VOODOO_PRIMARY) || (m_enmUADMConnectionType == UADM_CONNECTION_TYPE_VOODOO_SECONDARY)) && (m_intVoodooConn >= 0))
		{
			short tmpUserID = 0;
			short tmpGroupID = 0;
			char tmpRight[21];
			short tmpModuleID = 0;
			short tmpAppID = 0;
			long tmpRegionBranch = 0;
			sqlStatus = SqlOpenCs(&sqlHandle, sqlQuery.c_str());
			// debug info
			m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::getApplicationRights - sql executed, reading data...");
			if (sqlStatus == SQL_SUCCESS)
			{
				do  // loop result
				{
					// fetch mapping must be exact to the column sizes in the database
					sqlStatus = SqlFetch(sqlHandle, 1, &tmpUserID, &tmpGroupID, &tmpRight, &tmpModuleID, &tmpAppID, &tmpRegionBranch, NULL);
					if (sqlStatus == SQL_SUCCESS)
					{
						// store in vector
						UADM_APP_PERMISSION* pPerm = new UADM_APP_PERMISSION();
						pPerm->intUserID = (int)tmpUserID;
						pPerm->intGroupID = (int)tmpGroupID;
						pPerm->strRight = trim(std::string(tmpRight));
						pPerm->intModuleID = (int)tmpModuleID;
						pPerm->intAppID = (int)tmpAppID;
						pPerm->intRegionBranch = (int)tmpRegionBranch;
						pvecPermissions->push_back(pPerm);
					}
				} 
				while (sqlStatus == SQL_SUCCESS);
				SqlCloseCs(sqlHandle);
			}
			else
			{
				SqlCloseCs(sqlHandle);
				m_strLastErrorMessage = "Failed to read permissions for user " + m_strUsername + " and application " + m_strAppName;
				m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::getApplicationRights - " + m_strLastErrorMessage);
				return false;
			}
		}
		// Informix
		else if (((m_enmUADMConnectionType == UADM_CONNECTION_TYPE_INFORMIX_PRIMARY) || (m_enmUADMConnectionType == UADM_CONNECTION_TYPE_INFORMIX_SECONDARY)) && (m_pobjIFXConn != NULL))
		{
#ifndef EXLUDE_INFORMIX
			// execute SQL via informix connection
			ITQuery dbQuery(*m_pobjIFXConn);
			ITSet* dbResultSet = dbQuery.ExecToSet(ITString(sqlQuery.c_str()));
			ITRow* dbRow = NULL;
			if (dbResultSet != NULL)
			{
				while ((dbRow = (ITRow*)dbResultSet->Fetch()) != NULL)
				{
					// store in vector
					UADM_APP_PERMISSION* pPerm = new UADM_APP_PERMISSION();
					pPerm->intUserID = atoi(dbRow->Column(0)->Printable().Data());
					pPerm->intGroupID = atoi(dbRow->Column(1)->Printable().Data());
					pPerm->strRight = trim(std::string(dbRow->Column(2)->Printable().Data()));
					pPerm->intModuleID = atoi(dbRow->Column(3)->Printable().Data());
					pPerm->intAppID = atoi(dbRow->Column(4)->Printable().Data());
					pPerm->intRegionBranch = atoi(dbRow->Column(5)->Printable().Data());
					pvecPermissions->push_back(pPerm);
					dbRow->Release();
				}
				dbResultSet->Close();
			}
			else
			{
				m_strLastErrorMessage = "Failed to read permissions for user " + m_strUsername + " and application " + m_strAppName;
				m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::getApplicationRights - " + m_strLastErrorMessage);
				return false;
			}
#endif
		}

		// debug info
		m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::getApplicationRights - done");
		return true;
	}

	bool CUADMLoginInterface::getUserMappings(std::vector<UADM_USER_MAPPING*>* pvecUserMappings, int intRegionBranch, int intUserID, int intLocalUserID)
	{
		// debug info
		m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::getUserMappings - start");

		if ((m_enmUADMConnectionType == UADM_CONNECTION_TYPE_NONE) || (m_pobjUADMEnvironment == NULL) || ((m_intVoodooConn < 0) && (m_pobjIFXConn == NULL)) || 
			(m_lngAppID <= 0) || (pvecUserMappings == NULL) || (m_pobjUADMEntry == NULL))
		{
			m_strLastErrorMessage = "no database connection for uadm users retrieval";
			m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::getUserMappings - " + m_strLastErrorMessage);
			return false;
		}
		long         sqlStatus = 0;
		int          sqlHandle;
		std::string  sqlQuery;
		//  get country from uadm entry and cut the first two chars and make upper case
		std::string strCountry = toUpper(m_pobjUADMEntry->strCountry.substr(0, 2));

		// get all uadm users and its parameters
		sqlQuery = "SELECT u.userid, u.username, up.branchregionid, up.userparaid, upn.name, up.userparavalue FROM ppuser u"
			" LEFT JOIN ppcorappbruser up ON up.userid=u.userid AND up.applicationid=" + std::to_string((long long)m_lngAppID);
		if (intRegionBranch > 0)
		{
			// scan only for branch
			sqlQuery += " AND up.branchregionid=" + std::to_string((long long)intRegionBranch);
		}
		sqlQuery += " LEFT JOIN ppuserpara upn ON up.userparaid = upn.userparaid";
		if (intLocalUserID < 0)
		{
			// scan only for local user id parameter
			sqlQuery += " AND UPPER(upn.name)='LOCALUSERID'";
		}

		if (intUserID > 0)
		{
			// scan only for uadm user id
			sqlQuery += " WHERE u.userid=" + std::to_string((long long)intUserID);
		}
		else if (intLocalUserID > 0)
		{
			// scan only for local user id
			sqlQuery += " WHERE UPPER(upn.name)='LOCALUSERID' AND up.userparavalue='" + std::to_string((long long)intLocalUserID) + "'";
		}
		else
		{
			// scan only for country
			sqlQuery += " WHERE u.country='" + strCountry + "'";
		}
		sqlQuery += " ORDER BY u.userid, up.branchregionid, up.userparaid";

		// Voodoo
		if (((m_enmUADMConnectionType == UADM_CONNECTION_TYPE_VOODOO_PRIMARY) || (m_enmUADMConnectionType == UADM_CONNECTION_TYPE_VOODOO_SECONDARY)) && (m_intVoodooConn >= 0))
		{
			long tmpUserID = 0;
			char tmpUsername[21];
			short tmpRegionBranchID = 0;
			short tmpUserParaID = 0;
			char tmpUserParaName[101];
			char tmpUserParaValue[255];
			sqlStatus = SqlOpenCs(&sqlHandle, sqlQuery.c_str());

			// debug info
			m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::getUserMappings - sql executed, reading all uadm users and mappings...");
			if (sqlStatus == SQL_SUCCESS)
			{
				do  // loop result
				{
					// fetch mapping must be exact to the column sizes in the database
					sqlStatus = SqlFetch(sqlHandle, 1, &tmpUserID, &tmpUsername, &tmpRegionBranchID, &tmpUserParaID, &tmpUserParaName, &tmpUserParaValue, NULL);
					if (sqlStatus == SQL_SUCCESS)
					{
						// store in vector
						UADM_USER_MAPPING* pUserMap = new UADM_USER_MAPPING();
						pUserMap->intUserID = (int)tmpUserID;
						pUserMap->strUsername = trim(std::string(tmpUsername));
						pUserMap->intRegionBranchID = (int)tmpRegionBranchID;
						pUserMap->intUserParaID = (int)tmpUserParaID;
						pUserMap->strUserParaName = trim(std::string(tmpUserParaName));
						pUserMap->strUserParaValue = trim(std::string(tmpUserParaValue));
						// verify if object is already in vector (function can be called to enhance the list over multiple calls)
						bool blnExists = false;
						for (size_t i = 0; i < pvecUserMappings->size(); i++)
						{
							UADM_USER_MAPPING* pExistMap = pvecUserMappings->at(i);
							if ((pExistMap->intUserID == pUserMap->intUserID) &&
								(pExistMap->intRegionBranchID == pUserMap->intRegionBranchID) &&
								(pExistMap->intUserParaID == pUserMap->intUserParaID))
							{
								blnExists = true;
								break;
							}
						}
						if (blnExists == false)
						{
							pvecUserMappings->push_back(pUserMap);
						}
						else
						{
							// not needed, delete again
							delete pUserMap;
						}
					}
				} 
				while (sqlStatus == SQL_SUCCESS);
				SqlCloseCs(sqlHandle);
			}
			else
			{
				SqlCloseCs(sqlHandle);
				m_strLastErrorMessage = "Failed to read all uadm users and mappings";
				m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::getUserMappings - " + m_strLastErrorMessage);
				return false;
			}
		}
		// Informix
		else if (((m_enmUADMConnectionType == UADM_CONNECTION_TYPE_INFORMIX_PRIMARY) || (m_enmUADMConnectionType == UADM_CONNECTION_TYPE_INFORMIX_SECONDARY)) && (m_pobjIFXConn != NULL))
		{
#ifndef EXLUDE_INFORMIX
			// execute SQL via informix connection
			ITQuery dbQuery(*m_pobjIFXConn);
			ITSet* dbResultSet = dbQuery.ExecToSet(ITString(sqlQuery.c_str()));
			ITRow* dbRow = NULL;
			if (dbResultSet != NULL)
			{
				while ((dbRow = (ITRow*)dbResultSet->Fetch()) != NULL)
				{
					// store in vector
					UADM_USER_MAPPING* pUserMap = new UADM_USER_MAPPING();
					pUserMap->intUserID = atoi(dbRow->Column(0)->Printable().Data());
					pUserMap->strUsername = trim(std::string(dbRow->Column(1)->Printable().Data()));
					pUserMap->intRegionBranchID = atoi(dbRow->Column(2)->Printable().Data());
					pUserMap->intUserParaID = atoi(dbRow->Column(3)->Printable().Data());
					pUserMap->strUserParaName = trim(std::string(dbRow->Column(4)->Printable().Data()));
					pUserMap->strUserParaValue = trim(std::string(dbRow->Column(5)->Printable().Data()));
					if ((pUserMap->strUserParaValue.compare("NULL") == 0) || (pUserMap->strUserParaValue.compare("null") == 0))
					{
						pUserMap->strUserParaValue = "";
					}
					// verify if object is already in vector (function can be called to enhance the list over multiple calls)
					bool blnExists = false;
					for (size_t i = 0; i < pvecUserMappings->size(); i++)
					{
						UADM_USER_MAPPING* pExistMap = pvecUserMappings->at(i);
						if ((pExistMap->intUserID == pUserMap->intUserID) &&
							(pExistMap->intRegionBranchID == pUserMap->intRegionBranchID) &&
							(pExistMap->intUserParaID == pUserMap->intUserParaID))
						{
							blnExists = true;
							break;
						}
					}
					if (blnExists == false)
					{
						pvecUserMappings->push_back(pUserMap);
					}
					else
					{
						// not needed, delete again
						delete pUserMap;
					}
					dbRow->Release();
				}
				dbResultSet->Close();
			}
			else
			{
				m_strLastErrorMessage = "Failed to read all uadm users and mappings";;
				m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::getUserMappings - " + m_strLastErrorMessage);
				return false;
			}
#endif
		}

		// debug info
		m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::getUserMappings - done");
		return true;
	}

	bool CUADMLoginInterface::saveUserMapping(int intUserID, int intRegionBranch, std::string strUserParaName, std::string& strUserParaValue)
	{
		// debug info
		m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::saveUserMapping - start");
		if ((m_enmUADMConnectionType == UADM_CONNECTION_TYPE_NONE) || (m_pobjUADMEnvironment == NULL) || ((m_intVoodooConn < 0) && (m_pobjIFXConn == NULL)) ||
			(m_lngAppID <= 0) || (intUserID <= 0) || (intRegionBranch <= 0) || (strUserParaName.empty() == true) || (strUserParaValue.empty() == true))
		{
			m_strLastErrorMessage = "Invalid parameters for saving UserMapping";
			m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::saveUserMapping - " + m_strLastErrorMessage);
			return false;
		}
		long         sqlStatus = 0;
		std::string  sqlQuery;
		std::string strAppID = std::to_string((long long)m_lngAppID);
		std::string strUserID = std::to_string((long long)intUserID);
		std::string strBranchID = std::to_string((long long)intRegionBranch);
		// get userparaid from name
		std::string strUserParaID = "";
		sqlQuery = "SELECT CAST(userparaid AS CHAR(254)) FROM ppuserpara WHERE UPPER(name)='" + toUpper(strUserParaName) + "'";
		if (readSQLValue(sqlQuery, strUserParaID) != 0)
		{
			m_strLastErrorMessage = "Failed to read userparaid for name " + strUserParaName;
			m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::saveUserMapping - " + m_strLastErrorMessage);
			return false;
		}
		strUserParaID = trim(strUserParaID);
		// now verify if user para value should be filled automatically for parameter name TerminalID
		if ((toUpper(strUserParaValue) == "*") && (toUpper(strUserParaName) == "TERMINALID"))
		{
			// generate branch as 2 digits with leading zeros
			std::string strBranchID2Digits = strBranchID;
			while (strBranchID2Digits.length() < 2)
			{
				strBranchID2Digits = "0" + strBranchID2Digits;
			}
			
			// get next value for terminal id for this branch
			sqlQuery = "SELECT CAST(NVL(MAX(userparavalue),'" + strBranchID2Digits+ "00') AS CHAR(254)) FROM ppcorappbruser WHERE userparaid=" + strUserParaID + " AND applicationid=" + strAppID + " AND branchregionid=" + strBranchID + " AND userparavalue like '" + strBranchID2Digits + "%'";
			std::string strNextVal = "";
			if (readSQLValue(sqlQuery, strNextVal) != 0)
			{
				m_strLastErrorMessage = "Failed to read next value for terminalid";
				m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::saveUserMapping - " + m_strLastErrorMessage);
				return false;
			}
			strUserParaValue = trim(strNextVal);
			// increment by one with rule of last two digits allowing from 00 to ZZ (0-9, A-Z)
			// this formula is taken over from zaapunix to support same terminalid generation in UADM now
			for (int i = 3; i >= 2; i--)
			{
				if (strUserParaValue[i] >= '0' && strUserParaValue[i] <= '9' || strUserParaValue[i] >= 'A' && strUserParaValue[i] <= 'Z')
				{
					if (strUserParaValue[i] == '9')
					{
						strUserParaValue[i] = 'A';
						break;
					}
					else if (strUserParaValue[i] == 'Z')
					{
						strUserParaValue[i] = '0';
					}
					else
					{
						strUserParaValue[i]++;
						break;
					}
				}
				else
				{
					m_strLastErrorMessage = "Failed to generate next value for terminalid, no more values available for branch " + strBranchID2Digits;
					m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::saveUserMapping - " + m_strLastErrorMessage);
					return false;
				}
			}
		}
		// check if entry already exists
		sqlQuery = "SELECT CAST(CAST(COUNT(*) AS INTEGER) AS CHAR(254)) FROM ppcorappbruser WHERE applicationid=" + strAppID + " AND userid=" + strUserID + " AND branchregionid=" + strBranchID + " AND userparaid=" + strUserParaID;
		std::string strCount = "";
		if (readSQLValue(sqlQuery, strCount) != 0)
		{
			m_strLastErrorMessage = "Failed to read existing user mapping for user " + strUserID;
			m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::saveUserMapping - " + m_strLastErrorMessage);
			return false;
		}
		strCount = trim(strCount);
		if (strCount == "0")
		{
			// insert new entry
			sqlQuery = "INSERT INTO ppcorappbruser (applicationid, userid, branchregionid, userparaid, userparavalue) VALUES ("
				+ strAppID + ", " + strUserID + ", " + strBranchID + ", " + strUserParaID + ", '" + trim(strUserParaValue) + "')";
		}
		else
		{
			// update existing entry
			sqlQuery = "UPDATE ppcorappbruser SET userparavalue='" + trim(strUserParaValue) + "' WHERE applicationid=" + strAppID + " AND userid=" + strUserID + " AND branchregionid=" + strBranchID + " AND userparaid=" + strUserParaID;
		}
		sqlStatus = executeSQL(sqlQuery.c_str());
		if (sqlStatus != 0)
		{
			m_strLastErrorMessage = "Failed to save usermapping for user " + strUserID + " on branch " + strBranchID + " with parameter " + strUserParaName;
			m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::saveUserMapping - " + m_strLastErrorMessage);
			return false; // update failed
		}
		// debug info
		m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::saveUserMapping - done");
		return true;
	}

	bool CUADMLoginInterface::getBranchUsers(std::vector<UADM_BRANCH_USER*>* pvecBranchUsers, int regionBranchNo)
	{
		// debug info
		m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::getBranchUsers - start");
		if ((m_enmUADMConnectionType == UADM_CONNECTION_TYPE_NONE) || (m_pobjUADMEnvironment == NULL) || ((m_intVoodooConn < 0) && (m_pobjIFXConn == NULL)) || 
			(m_lngAppID <= 0) || (pvecBranchUsers == NULL) || (regionBranchNo <= 0))
		{
			m_strLastErrorMessage = "Invalid parameters for branch users retrieval";
			m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::getBranchUsers - " + m_strLastErrorMessage);
			return false;
		}
		long         sqlStatus = 0;
		int          sqlHandle;
		std::string  sqlQuery;
		std::string strAppID = std::to_string((long long)m_lngAppID);
		std::string strBranchID = std::to_string((long long)regionBranchNo);
		// get all users for this branch/region and app
		sqlQuery = "SELECT DISTINCT u.userid, u.username, u.forename, u.surname, u.emailuser, up1.userparavalue AS termid, up2.userparavalue AS agentid ";
		sqlQuery += "FROM ppuser u LEFT JOIN ppcorappbruser up1 ON up1.userid=u.userid AND up1.applicationid=" + strAppID + " AND up1.branchregionid=" + strBranchID + " and up1.userparaid=2 ";
		sqlQuery += "LEFT JOIN ppcorappbruser up2 ON up2.userid=u.userid AND up2.applicationid=" + strAppID + " AND up2.branchregionid=" + strBranchID + " and up2.userparaid=3, ppcorappusergroup cup ";
		sqlQuery += "WHERE u.userid=cup.userid AND cup.applicationid=" + strAppID + " AND cup.groupid IN (SELECT DISTINCT ar.groupid FROM ppappright ar WHERE ar.applicationid=" + strAppID + " AND ar.branchregionid=" + strBranchID + ") ";
		sqlQuery += "ORDER BY u.userid";

		// Voodoo
		if (((m_enmUADMConnectionType == UADM_CONNECTION_TYPE_VOODOO_PRIMARY) || (m_enmUADMConnectionType == UADM_CONNECTION_TYPE_VOODOO_SECONDARY)) && (m_intVoodooConn >= 0))
		{
			long tmpUserID = 0;
			char tmpUsername[21];
			char tmpForename[21];
			char tmpSurname[31];
			char tmpEmailUser[51];
			char tmpTermID[255];
			char tmpAgentID[255];
			sqlStatus = SqlOpenCs(&sqlHandle, sqlQuery.c_str());
			// debug info
			m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::getBranchUsers - sql executed, reading data...");
			if (sqlStatus == SQL_SUCCESS)
			{
				do  // loop result
				{
					// fetch mapping must be exact to the column sizes in the database
					sqlStatus = SqlFetch(sqlHandle, 1, &tmpUserID, &tmpUsername, &tmpForename, &tmpSurname, &tmpEmailUser, &tmpTermID, &tmpAgentID, NULL);
					if (sqlStatus == SQL_SUCCESS)
					{
						// store in vector
						UADM_BRANCH_USER* pBranchUser = new UADM_BRANCH_USER();
						pBranchUser->intRegionBranchNo = regionBranchNo;
						pBranchUser->intUserID = (int)tmpUserID;
						pBranchUser->strUsername = trim(std::string(tmpUsername));
						pBranchUser->strFirstname = trim(std::string(tmpForename));
						pBranchUser->strLastname = trim(std::string(tmpSurname));
						pBranchUser->strEMail = trim(std::string(tmpEmailUser));
						pBranchUser->strTerminalID = trim(std::string(tmpTermID));
						pBranchUser->strAgentID = trim(std::string(tmpAgentID));
						pvecBranchUsers->push_back(pBranchUser);
					}
				} 
				while (sqlStatus == SQL_SUCCESS);
				SqlCloseCs(sqlHandle);
			}
			else
			{
				SqlCloseCs(sqlHandle);
				m_strLastErrorMessage = "Failed to read branch users for region/branch " + std::to_string((long long)regionBranchNo);
				m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::getBranchUsers - " + m_strLastErrorMessage);
				return false;
			}
		}
		// Informix
		else if (((m_enmUADMConnectionType == UADM_CONNECTION_TYPE_INFORMIX_PRIMARY) || (m_enmUADMConnectionType == UADM_CONNECTION_TYPE_INFORMIX_SECONDARY)) && (m_pobjIFXConn != NULL))
		{
#ifndef EXLUDE_INFORMIX
			// execute SQL via informix connection
			ITQuery dbQuery(*m_pobjIFXConn);
			ITSet* dbResultSet = dbQuery.ExecToSet(ITString(sqlQuery.c_str()));
			ITRow* dbRow = NULL;
			if (dbResultSet != NULL)
			{
				while ((dbRow = (ITRow*)dbResultSet->Fetch()) != NULL)
				{
					// store in vector
					UADM_BRANCH_USER* pBranchUser = new UADM_BRANCH_USER();
					pBranchUser->intRegionBranchNo = regionBranchNo;
					pBranchUser->intUserID = atoi(dbRow->Column(0)->Printable().Data());
					pBranchUser->strUsername = trim(std::string(dbRow->Column(1)->Printable().Data()));
					pBranchUser->strFirstname = trim(std::string(dbRow->Column(2)->Printable().Data()));
					pBranchUser->strLastname = trim(std::string(dbRow->Column(3)->Printable().Data()));
					pBranchUser->strEMail = trim(std::string(dbRow->Column(4)->Printable().Data()));
					pBranchUser->strTerminalID = trim(std::string(dbRow->Column(5)->Printable().Data()));
					pBranchUser->strAgentID = trim(std::string(dbRow->Column(6)->Printable().Data()));
					pvecBranchUsers->push_back(pBranchUser);
					dbRow->Release();
				}
				dbResultSet->Close();
			}
			else
			{
				m_strLastErrorMessage = "Failed to read branch users for region/branch " + std::to_string((long long)regionBranchNo);
				m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::getBranchUsers - " + m_strLastErrorMessage);
				return false;
			}
#endif
		}
		// debug info
		m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::getBranchUsers - done");
		return true;
	}

	bool CUADMLoginInterface::getApplicationParameters(std::vector<UADM_APP_PARAMETER*>* pvecParameters)
	{
		// debug info
		m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::getApplicationParameters - start");

		if ((m_enmUADMConnectionType == UADM_CONNECTION_TYPE_NONE) || (m_pobjUADMEnvironment == NULL) || ((m_intVoodooConn < 0) && (m_pobjIFXConn == NULL)) || (m_lngAppID <= 0) || (pvecParameters == NULL))
		{
			m_strLastErrorMessage = "Invalid parameters for application parameters retrieval";
			m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::getApplicationParameters - " + m_strLastErrorMessage);
			return false;
		}
		long         sqlStatus = 0;
		int          sqlHandle;
		std::string  sqlQuery;

		// get all parameters for this application
		sqlQuery = "SELECT applicationid, para1, para2, parabez, paravalue FROM pplogpara lp WHERE lp.applicationid=" + std::to_string((long long)m_lngAppID) + " ORDER BY para1, para2, parabez";

		// Voodoo
		if (((m_enmUADMConnectionType == UADM_CONNECTION_TYPE_VOODOO_PRIMARY) || (m_enmUADMConnectionType == UADM_CONNECTION_TYPE_VOODOO_SECONDARY)) && (m_intVoodooConn >= 0))
		{
			short tmpAppID = 0;
			char tmpPara1[31];
			char tmpPara2[31];
			char tmpParaBez[31];
			char tmpParaValue[255];

			sqlStatus = SqlOpenCs(&sqlHandle, sqlQuery.c_str());
			// debug info
			m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::getApplicationParameters - sql executed, reading data...");
			if (sqlStatus == SQL_SUCCESS)
			{
				do  // loop result
				{
					// fetch mapping must be exact to the column sizes in the database
					sqlStatus = SqlFetch(sqlHandle, 1, &tmpAppID, &tmpPara1, &tmpPara2, &tmpParaBez, &tmpParaValue, NULL);
					if (sqlStatus == SQL_SUCCESS)
					{
						UADM_APP_PARAMETER* pPara = new UADM_APP_PARAMETER();
						pPara->intAppID = (int)tmpAppID;
						pPara->strPara1 = trim(std::string(tmpPara1));
						pPara->strPara2 = trim(std::string(tmpPara2));
						pPara->strParaName = trim(std::string(tmpParaBez));
						pPara->strParaValue = trim(std::string(tmpParaValue));
						pvecParameters->push_back(pPara);
					}
				}
				while (sqlStatus == SQL_SUCCESS);
				SqlCloseCs(sqlHandle);
			}
			else
			{
				SqlCloseCs(sqlHandle);
				m_strLastErrorMessage = "Failed to read parameters for application " + m_strAppName;
				m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::getApplicationParameters - " + m_strLastErrorMessage);
				return false;
			}
		}
		// Informix
		else if (((m_enmUADMConnectionType == UADM_CONNECTION_TYPE_INFORMIX_PRIMARY) || (m_enmUADMConnectionType == UADM_CONNECTION_TYPE_INFORMIX_SECONDARY)) && (m_pobjIFXConn != NULL))
		{
#ifndef EXLUDE_INFORMIX
			// execute SQL via informix connection
			ITQuery dbQuery(*m_pobjIFXConn);
			ITSet* dbResultSet = dbQuery.ExecToSet(ITString(sqlQuery.c_str()));
			ITRow* dbRow = NULL;
			if (dbResultSet != NULL)
			{
				while ((dbRow = (ITRow*)dbResultSet->Fetch()) != NULL)
				{
					UADM_APP_PARAMETER* pPara = new UADM_APP_PARAMETER();
					pPara->intAppID = atoi(dbRow->Column(0)->Printable().Data());
					pPara->strPara1 = trim(std::string(dbRow->Column(1)->Printable().Data()));
					pPara->strPara2 = trim(std::string(dbRow->Column(2)->Printable().Data()));
					pPara->strParaName = trim(std::string(dbRow->Column(3)->Printable().Data()));
					pPara->strParaValue = trim(std::string(dbRow->Column(4)->Printable().Data()));
					pvecParameters->push_back(pPara);
					dbRow->Release();
				}
				dbResultSet->Close();
			}
			else
			{
				m_strLastErrorMessage = "Failed to read parameters for application " + m_strAppName;
				m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::getApplicationParameters - " + m_strLastErrorMessage);
				return false;
			}
#endif
		}

		// debug info
		m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::getApplicationParameters - done");
		return true;
	}

	bool CUADMLoginInterface::updateLastLoginDate()
	{
		// debug info
		m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::updateLastLoginDate - start");
		// update the last login date to now
		if ((m_enmUADMConnectionType == UADM_CONNECTION_TYPE_NONE) || (m_pobjUADMEnvironment == NULL) || ((m_intVoodooConn < 0) && (m_pobjIFXConn == NULL)) || (m_lngUserID <= 0) || (m_strUsername.empty() == true))
		{
			m_strLastErrorMessage = "Invalid parameters for last login date update";
			m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::updateLastLoginDate - " + m_strLastErrorMessage);
			return false;
		}
		long         sqlStatus = 0;
		std::string  sqlQuery;
		// set the last login to the format as long yyyymmdd
		// prepare the SQL query
		sqlQuery = "UPDATE ppuser SET lastlogin=TO_CHAR(sysdate,'%Y%m%d') WHERE userid=" + std::to_string((long long)m_lngUserID) + " AND UPPER(username)='" + toUpper(m_strUsername) + "'";
		sqlStatus = executeSQL(sqlQuery.c_str());
		if (sqlStatus != 0)
		{
			m_strLastErrorMessage = "Failed to update last login date for user " + m_strUsername;
			m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::updateLastLoginDate - " + m_strLastErrorMessage);
			return false; // update failed
		}
		// debug info
		m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::updateLastLoginDate - done");
		return true;
	}

	bool CUADMLoginInterface::saveLastLoginData(long lngAppID, std::string strOSAccount, long lngUserID, int intRegionBranchNo, std::string strCountry, std::string strLanguage)
	{
		// debug info
		m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::saveLastLoginData - start");
		// save the login information in the pploginprevious table in database
		if ((m_enmUADMConnectionType == UADM_CONNECTION_TYPE_NONE) || ((m_intVoodooConn < 0) && (m_pobjIFXConn == NULL)) || (strOSAccount.empty() == true) || (lngUserID <= 0) || (lngAppID <= 0))
		{
			m_strLastErrorMessage = "Invalid parameters for saving last login data";
			m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::saveLastLoginData - " + m_strLastErrorMessage);
			return false; // no connection established or invalid parameters
		}

		long         sqlStatus = 0;
		std::string  sqlQuery;

		// lowercase the language
		strLanguage = toLower(strLanguage);

		// check existance, will be managed by global user / os user account
		std::string tmpCnt = "";

		sqlQuery = "SELECT CAST(CAST(COUNT(*) AS INTEGER) AS CHAR(254)) FROM pploginprevious WHERE UPPER(globaluser)='" + toUpper(strOSAccount) + "' AND applicationid=" + std::to_string((long long)lngAppID);
		sqlStatus = readSQLValue(sqlQuery.c_str(), tmpCnt);
		if (sqlStatus == 0)
		{
			if (atoi(tmpCnt.c_str()) > 1)
			{
				// too many entries found, remove them all
				sqlQuery = "DELETE FROM pploginprevious WHERE UPPER(globaluser)='" + toUpper(strOSAccount) + "' AND applicationid=" + std::to_string((long long)lngAppID);
				sqlStatus = executeSQL(sqlQuery.c_str());
				if (sqlStatus != 0)
				{
					m_strLastErrorMessage = "Failed to cleanup last login data for OS user " + strOSAccount + " and application " + std::to_string((long long)lngAppID);
					m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::saveLastLoginData - " + m_strLastErrorMessage);
					return false; // update or insert failed
				}
				tmpCnt = "0"; // reset to 0 to insert new record
			}
			if (atoi(tmpCnt.c_str()) == 0)
			{
				// insert new record if not exists
				sqlQuery = "INSERT INTO pploginprevious (userid, applicationid, globaluser, languageid, branchregionid, country) VALUES (" + std::to_string((long long)lngUserID) + ", " + std::to_string((long long)lngAppID) + ", '" + strOSAccount + "', '" + strLanguage + "', " + std::to_string((long long)intRegionBranchNo) + ", '" + strCountry + "')";
				sqlStatus = executeSQL(sqlQuery.c_str());
				if (sqlStatus != 0)
				{
					m_strLastErrorMessage = "Failed to insert last login data for OS user " + strOSAccount + " and application " + std::to_string((long long)lngAppID);
					m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::saveLastLoginData - " + m_strLastErrorMessage);
					return false; // update or insert failed
				}
			}
			else if (atoi(tmpCnt.c_str()) == 1)
			{
				// update existing entry
				sqlQuery = "UPDATE pploginprevious SET userid = " + std::to_string((long long)lngUserID) + ", languageid='" + strLanguage + "' ,country='" + strCountry + "' ,branchregionid = " + std::to_string((long long)intRegionBranchNo) + " WHERE UPPER(globaluser)='" + toUpper(strOSAccount) + "' AND applicationid = " + std::to_string((long long)lngAppID);
				sqlStatus = executeSQL(sqlQuery.c_str());
				if (sqlStatus != 0)
				{
					m_strLastErrorMessage = "Failed to update last login data for OS user " + strOSAccount + " and application " + std::to_string((long long)lngAppID);
					m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::saveLastLoginData - " + m_strLastErrorMessage);
					return false; // update or insert failed
				}
			}
		}
		// debug info
		m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::saveLastLoginData - done");
		return true;
	}

	bool CUADMLoginInterface::saveLastLoginData()
	{
		// save the login information in the pploginprevious table in database
		if ((m_enmUADMConnectionType == UADM_CONNECTION_TYPE_NONE) || (m_pobjUADMEnvironment == NULL) || (m_pobjUADMEntry == NULL))
		{
			m_strLastErrorMessage = "Invalid parameters for handing over to save last login data";
			m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::saveLastLoginData - " + m_strLastErrorMessage);
			return false; // no connection established or invalid parameters
		}
		return saveLastLoginData(m_lngAppID, m_strOSAccount, m_lngUserID, m_pobjUADMEntry->intRegionBranchNo, m_pobjUADMEntry->strCountry, m_strLanguage);
	}

	bool CUADMLoginInterface::verifyNewPassword(const std::string& strNewPassword)
	{
		// verify the new password against the last 5 passwords of the user
		if ((m_enmUADMConnectionType == UADM_CONNECTION_TYPE_NONE) || (m_pobjUADMEnvironment == NULL) || ((m_intVoodooConn < 0) && (m_pobjIFXConn == NULL)) || (m_lngUserID <= 0) || (m_strUsername.empty() == true) || (strNewPassword.empty() == true))
		{
			m_strLastErrorMessage = "Invalid parameters for new password verification";
			m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::verifyNewPassword - " + m_strLastErrorMessage);
			return false; // no connection established or invalid parameters
		}

		if (m_enmUADMEncryptionType == UADM_PASSWORD_ENCRYPTION_TYPE_PHOENIX)
		{
			// old rules:
			// min length = 6
			// max length = 8
			const int MIN_LENGTH = 6;
			const int MAX_LENGTH = 8;
			if ((strNewPassword.length() < MIN_LENGTH) || (strNewPassword.length() > MAX_LENGTH))
			{
				m_strLastErrorMessage = "Password must be between " + std::to_string((long long)MIN_LENGTH) + " and " + std::to_string((long long)MAX_LENGTH) + " characters long";
				m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::verifyNewPassword - " + m_strLastErrorMessage);
				return false;
			}
		}
		else if (m_enmUADMEncryptionType == UADM_PASSWORD_ENCRYPTION_TYPE_AES)
		{
			// new rules:
			// min length = 8
			// max length = 64
			const int MIN_LENGTH = 8;
			const int MAX_LENGTH = 64;
			if ((strNewPassword.length() < MIN_LENGTH) || (strNewPassword.length() > MAX_LENGTH))
			{
				m_strLastErrorMessage = "Password must be between " + std::to_string((long long)MIN_LENGTH) + " and " + std::to_string((long long)MAX_LENGTH) + " characters long";
				m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::verifyNewPassword - " + m_strLastErrorMessage);
				return false;
			}
		}
		else
		{
			// not valid encryption type
			m_strLastErrorMessage = "No valid encryption type configured for password verification";
			m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::verifyNewPassword - " + m_strLastErrorMessage);
			return false;
		}
		// common rules:
		// only a-z A-Z 0-9 and !"§$%&/()=+#,.-;:_ {[]}
		// min one upper case, min one digit

		// Allowed characters
		std::string allowed = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!()=+#,.-;:_ {}[]";
		bool hasUpper = false;
		bool hasLower = false;
		bool hasDigit = false;
		bool hasSpecial = false;
		int charTypes = 0;
		for (size_t i = 0; i < strNewPassword.length(); i++)
		{
			char ch = strNewPassword.at(i);
			if (allowed.find(ch) == std::string::npos)
			{
				m_strLastErrorMessage = "Password contains invalid character: " + std::string(1, ch);
				m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::verifyNewPassword - " + m_strLastErrorMessage);
				return false;
			}
			if ((isupper(static_cast<unsigned char>(ch))) && (hasUpper == false))
			{
				charTypes++;
				hasUpper = true;
			}
			if ((islower(static_cast<unsigned char>(ch))) && (hasLower == false))
			{
				charTypes++;
				hasLower = true;
			}
			if ((ispunct(static_cast<unsigned char>(ch))) && (hasSpecial == false))
			{
				charTypes++;
				hasSpecial = true;
			}
			if ((isdigit(static_cast<unsigned char>(ch))) && (hasDigit == false))
			{
				charTypes++;
				hasDigit = true;
			}
		}
		// min two different character types
		if (charTypes < 2)
		{
			m_strLastErrorMessage = "Password must contain at least two different character types (uppercase, lowercase, digit, special)";
			m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::verifyNewPassword - " + m_strLastErrorMessage);
			return false;
		}
		return true;
	}

	bool CUADMLoginInterface::verifyAgainstHistoricPasswords(std::string strPassword, bool& blnExists)
	{
		// verify the new password against the last 5 passwords of the user
		if ((m_enmUADMConnectionType == UADM_CONNECTION_TYPE_NONE) || (m_pobjUADMEnvironment == NULL) || ((m_intVoodooConn < 0) && (m_pobjIFXConn == NULL)) || (m_lngUserID <= 0) || (m_strUsername.empty() == true) || (strPassword.empty() == true))
		{
			m_strLastErrorMessage = "Invalid parameters for historic password verification";
			m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::verifyAgainstHistoricPasswords - " + m_strLastErrorMessage);
			return false; // no connection established or invalid parameters
		}
		blnExists = false;
		long        sqlStatus = 0;
		int         sqlHandle;
		std::string sqlQuery;
		std::string strEncPwdOld = encryptOld(strPassword);
		std::string strEncPwdNew = encryptNew(strPassword, m_strUsername);
		sqlQuery = "SELECT FIRST " + std::to_string((long long)m_intPasswordHistoryCount) + " password, pwd_enc FROM pppasswordprevious WHERE userid = " + std::to_string((long long)m_lngUserID) + " ORDER BY datevalidfrom DESC";

		// Voodoo
		if (((m_enmUADMConnectionType == UADM_CONNECTION_TYPE_VOODOO_PRIMARY) || (m_enmUADMConnectionType == UADM_CONNECTION_TYPE_VOODOO_SECONDARY)) && (m_intVoodooConn >= 0))
		{
			sqlStatus = SqlOpenCs(&sqlHandle, sqlQuery.c_str());
			if (sqlStatus == SQL_SUCCESS)
			{
				do  // loop result
				{
					char tmpPasswordOld[9];
					char tmpPasswordNew[255];	
					sqlStatus = SqlFetch(sqlHandle, 1, &tmpPasswordOld, &tmpPasswordNew, NULL);
					if (sqlStatus == SQL_SUCCESS)
					{
						// trim the new password field because it is dynamic length
						std::string tmpPwdNewEnc = trim(std::string(tmpPasswordNew));
						if (((strEncPwdOld.empty() == false) && (strlen(tmpPasswordOld) > 0) && (strEncPwdOld.compare(std::string(tmpPasswordOld)) == 0)) ||
							((strEncPwdNew.empty() == false) && (tmpPwdNewEnc.empty() == false) && (strEncPwdNew.compare(tmpPwdNewEnc) == 0)))
						{
							blnExists = true; // password already exists in the history
						}
					}
				} 
				while (sqlStatus == SQL_SUCCESS);
				SqlCloseCs(sqlHandle);
				return true; // sql is ok and passwords checked
			}
		}
		// Informix
		else if (((m_enmUADMConnectionType == UADM_CONNECTION_TYPE_INFORMIX_PRIMARY) || (m_enmUADMConnectionType == UADM_CONNECTION_TYPE_INFORMIX_SECONDARY)) && (m_pobjIFXConn != NULL))
		{
#ifndef EXLUDE_INFORMIX
			// execute SQL via informix connection
			ITQuery dbQuery(*m_pobjIFXConn);
			ITSet* dbResultSet = dbQuery.ExecToSet(ITString(sqlQuery.c_str()));
			ITRow* dbRow = NULL;
			if (dbResultSet != NULL)
			{
				while ((dbRow = (ITRow*)dbResultSet->Fetch()) != NULL)
				{
					std::string tmpPwdOld = std::string(dbRow->Column(0)->Printable().Data()); // fixed length, no trim needed
					std::string tmpPwdNew = trim(std::string(dbRow->Column(1)->Printable().Data())); // must be trimmed because it is dynamic length
					if (((strEncPwdOld.empty() == false) && (tmpPwdOld.length() > 0) && (strEncPwdOld.compare(tmpPwdOld) == 0)) ||
						((strEncPwdNew.empty() == false) && (tmpPwdNew.empty() == false) && (strEncPwdNew.compare(tmpPwdNew) == 0)))
					{
						blnExists = true; // password already exists in the history
					}
					dbRow->Release();
				}
				dbResultSet->Close();
				return true; // sql is ok and passwords checked
			}
#endif	
		}

		m_strLastErrorMessage = "Failed to read historic passwords for user " + m_strUsername;
		m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::verifyAgainstHistoricPasswords - " + m_strLastErrorMessage);
		return false; // error
	}

	bool CUADMLoginInterface::saveNewPassword(std::string strPassword)
	{
		// save the new password encrypted, support old and new encryption type dependent on uadm parameter
		if ((m_enmUADMConnectionType == UADM_CONNECTION_TYPE_NONE) || (m_pobjUADMEnvironment == NULL) || ((m_intVoodooConn < 0) && (m_pobjIFXConn == NULL)) || (m_lngUserID <= 0) || (m_strUsername.empty() == true) || (strPassword.empty() == true))
		{
			m_strLastErrorMessage = "Invalid parameters for saving new password";
			m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::saveNewPassword - " + m_strLastErrorMessage);
			return false; // no connection established or invalid parameters
		}
		long         sqlStatus = 0;
		std::string  sqlQuery;

		if (m_enmUADMEncryptionType == UADM_PASSWORD_ENCRYPTION_TYPE_PHOENIX)
		{
			// use the old encryption and remove the new encrypted pwd
			std::string  strEncryptedPassword = encryptOld(strPassword);
			if (strEncryptedPassword.empty() == true)
			{
				m_strLastErrorMessage = "Failed to encrypt new password with old encryption type";
				m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::saveNewPassword - " + m_strLastErrorMessage);
				return false; // encryption failed
			}

			// prepare the SQL query
			sqlQuery = "UPDATE ppuser SET password ='" + strEncryptedPassword + "', pwd_enc = '', changepw=0, lastpwchange=TO_CHAR(sysdate,'%Y%m%d') WHERE userid = " + std::to_string((long long)m_lngUserID) + " AND username = '" + m_strUsername + "'";
			sqlStatus = executeSQL(sqlQuery.c_str());
			if (sqlStatus != 0)
			{
				m_strLastErrorMessage = "Failed to update new password for user " + m_strUsername + " with old encryption type";
				m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::saveNewPassword - " + m_strLastErrorMessage);
				return false; // update failed
			}
			// save the password in the history table
			sqlQuery = "INSERT INTO pppasswordprevious (userid, password, pwd_enc, datevalidfrom) VALUES (" + std::to_string((long long)m_lngUserID) + ", '" + strEncryptedPassword + "', '', TO_CHAR(sysdate,'%Y%m%d'))";
			sqlStatus = executeSQL(sqlQuery.c_str());
			if (sqlStatus != 0)
			{
				m_strLastErrorMessage = "Failed to insert new password into history for user " + m_strUsername + " with old encryption type";
				m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::saveNewPassword - " + m_strLastErrorMessage);
				//return false; // insert failed but ignore this error, as the password is already changed
			}
		}
		else if (m_enmUADMEncryptionType == UADM_PASSWORD_ENCRYPTION_TYPE_AES)
		{
			// use the new encryption and remove the old encrypted pwd
			std::string  strEncryptedPassword = encryptNew(strPassword, m_strUsername);
			if (strEncryptedPassword.empty() == true)
			{
				m_strLastErrorMessage = "Failed to encrypt new password with new encryption type";
				m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::saveNewPassword - " + m_strLastErrorMessage);
				return false; // encryption failed
			}

			// prepare the SQL query
			sqlQuery = "UPDATE ppuser SET password = '', pwd_enc='" + strEncryptedPassword + "', changepw=0, lastpwchange=TO_CHAR(sysdate,'%Y%m%d') WHERE userid = " + std::to_string((long long)m_lngUserID) + " AND username = '" + m_strUsername + "'";
			sqlStatus = executeSQL(sqlQuery.c_str());
			if (sqlStatus != 0)
			{
				m_strLastErrorMessage = "Failed to update new password for user " + m_strUsername + " with new encryption type";
				m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::saveNewPassword - " + m_strLastErrorMessage);
				return false; // update failed
			}
			// save the password in the history table
			sqlQuery = "INSERT INTO pppasswordprevious (userid, password, pwd_enc, datevalidfrom) VALUES (" + std::to_string((long long)m_lngUserID) + ", '', '" + strEncryptedPassword + "', TO_CHAR(sysdate,'%Y%m%d'))";
			sqlStatus = executeSQL(sqlQuery.c_str());
			if (sqlStatus != 0)
			{
				m_strLastErrorMessage = "Failed to insert new password into history for user " + m_strUsername + " with new encryption type";
				m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::saveNewPassword - " + m_strLastErrorMessage);
				//return false; // insert failed but ignore this error, as the password is already changed
			}
		}
		return true;
	}

	std::string CUADMLoginInterface::resolveCountry(int intRegionBranchNo)
	{
		// this function should be replaced by a database table in future releases
		// to be compatible with older compilers its a simple if else structure, but no map
		std::string ret = "";
		if (intRegionBranchNo == 1000)
		{
			return "WW"; // worldwide
		}
		if ((intRegionBranchNo == 2000) || (intRegionBranchNo == 2001) || (intRegionBranchNo == 2007) || (intRegionBranchNo == 2100) || (intRegionBranchNo == 2105) || (intRegionBranchNo == 2200))
		{
			return "DE"; // default country for region DE
		}
		if ((intRegionBranchNo == 5) || (intRegionBranchNo == 6) || (intRegionBranchNo == 8) || (intRegionBranchNo == 9) || (intRegionBranchNo == 21) || (intRegionBranchNo == 27) || (intRegionBranchNo == 31) || (intRegionBranchNo == 33))
		{
			return "DE"; // default country for branch DE
		}
		if ((intRegionBranchNo == 40) || (intRegionBranchNo == 44) || (intRegionBranchNo == 46) || (intRegionBranchNo == 51) || (intRegionBranchNo == 52) || (intRegionBranchNo == 54) || (intRegionBranchNo == 56) || (intRegionBranchNo == 57))
		{
			return "DE"; // default country for branch DE
		}
		if ((intRegionBranchNo == 58) || (intRegionBranchNo == 59) || (intRegionBranchNo == 70) || (intRegionBranchNo == 78) || (intRegionBranchNo == 81) || (intRegionBranchNo == 82) || (intRegionBranchNo == 99) || (intRegionBranchNo == 200) || (intRegionBranchNo == 297))
		{
			return "DE"; // default country for branch DE
		}
		if ((intRegionBranchNo == 2002) || (intRegionBranchNo == 2010))
		{
			return "CH"; // default country for region CH
		}
		if ((intRegionBranchNo == 12) || (intRegionBranchNo == 13) || (intRegionBranchNo == 15) || (intRegionBranchNo == 16) || (intRegionBranchNo == 19) || (intRegionBranchNo == 113) || (intRegionBranchNo == 202) || (intRegionBranchNo == 210))
		{
			return "CH"; // default country for branch CH
		}
		if ((intRegionBranchNo == 2003) || (intRegionBranchNo == 2014))
		{
			return "AT"; // default country for region AT
		}
		if ((intRegionBranchNo == 61) || (intRegionBranchNo == 62) || (intRegionBranchNo == 63) || (intRegionBranchNo == 64) || (intRegionBranchNo == 69) || (intRegionBranchNo == 83) || (intRegionBranchNo == 203) || (intRegionBranchNo == 214))
		{
			return "AT"; // default country for branch AT
		}
		if ((intRegionBranchNo == 2004) || (intRegionBranchNo == 71) || (intRegionBranchNo == 72) || (intRegionBranchNo == 73) || (intRegionBranchNo == 74) || (intRegionBranchNo == 77) || (intRegionBranchNo == 79) || (intRegionBranchNo == 204))
		{
			return "HR"; // default country for region/branch HR
		}
		if ((intRegionBranchNo == 2008) || (intRegionBranchNo == 22) || (intRegionBranchNo == 23) || (intRegionBranchNo == 24) || (intRegionBranchNo == 25) || (intRegionBranchNo == 26) || (intRegionBranchNo == 28) || (intRegionBranchNo == 29) || (intRegionBranchNo == 208))
		{
			return "BG"; // default country for region/branch BG
		}
		if ((intRegionBranchNo == 2005) || (intRegionBranchNo == 2011) || (intRegionBranchNo == 2012) || (intRegionBranchNo == 2013))
		{
			return "FR"; // default country for region FR
		}
		if ((intRegionBranchNo == 17) || (intRegionBranchNo == 18) || (intRegionBranchNo == 30) || (intRegionBranchNo == 32) || (intRegionBranchNo == 34) || (intRegionBranchNo == 35) || (intRegionBranchNo == 36) || (intRegionBranchNo == 37))
		{
			return "FR"; // default country for branch FR
		}
		if ((intRegionBranchNo == 38) || (intRegionBranchNo == 39) || (intRegionBranchNo == 41) || (intRegionBranchNo == 42) || (intRegionBranchNo == 43) || (intRegionBranchNo == 45) || (intRegionBranchNo == 47) || (intRegionBranchNo == 48))
		{
			return "FR"; // default country for branch FR
		}
		if ((intRegionBranchNo == 49) || (intRegionBranchNo == 50) || (intRegionBranchNo == 53) || (intRegionBranchNo == 55) || (intRegionBranchNo == 60) || (intRegionBranchNo == 65) || (intRegionBranchNo == 66) || (intRegionBranchNo == 67))
		{
			return "FR"; // default country for branch FR
		}
		if ((intRegionBranchNo == 68) || (intRegionBranchNo == 75) || (intRegionBranchNo == 76) || (intRegionBranchNo == 80) || (intRegionBranchNo == 86) || (intRegionBranchNo == 87) || (intRegionBranchNo == 90))
		{
			return "FR"; // default country for branch FR
		}
		if ((intRegionBranchNo == 205) || (intRegionBranchNo == 211) || (intRegionBranchNo == 212) || (intRegionBranchNo == 213))
		{
			return "FR"; // default country for branch FR
		}
		if ((intRegionBranchNo == 2009) || (intRegionBranchNo == 3) || (intRegionBranchNo == 84) || (intRegionBranchNo == 85) || (intRegionBranchNo == 209))
		{
			return "RS"; // default country for region/branch RS
		}
		return ret;
	}

	std::string CUADMLoginInterface::encryptPassword(std::string strPassword, std::string strUsername, int encryptionType)
	{
		if (encryptionType == UADM_PASSWORD_ENCRYPTION_TYPE_PHOENIX)
		{
			return encryptOld(strPassword);
		}
		else if (encryptionType == UADM_PASSWORD_ENCRYPTION_TYPE_AES)
		{
			return encryptNew(strPassword, strUsername);
		}
		return "";
	}

	std::string CUADMLoginInterface::encryptOld(std::string data)
	{
		// debug info
		m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::encryptOld - start");

		int clng = 8;
		if ((int)data.length() > clng)
		{
			// the old encryption method only supports max 8 chars
			return "";
		}
		int NOCHAR_1 = 127;
		int NOCHAR_2 = '"';
		int NOCHAR_3 = '\'';
		int perm[] = { 2, 7, 1, 5, 3, 0, 4, 6 };
		int PERM_ANZ = (sizeof(perm) / sizeof(int));
		char offs[] = { 0X1C, 0X08, 0X1B, 0X05, 0X17, 0X0D, 0X13, 0X09 };
		int OFFS_ANZ = (sizeof(offs) / sizeof(char));
		char crypt[9];
		int i;
		int j;
		while ((int)data.length() < clng)
		{
			data += " ";
		}
		const char* orig = data.c_str();

		for (i = 0; i < clng; i++)
		{
			j = perm[i % PERM_ANZ];
			if (orig[j] == NOCHAR_1 || orig[j] == NOCHAR_2 || orig[j] == NOCHAR_3)
			{
				crypt[i] = orig[j];
			}
			else
			{
				crypt[i] = (orig[j] ^ offs[i % OFFS_ANZ]) & (char)0X7F;
				if (crypt[i] == NOCHAR_1 || crypt[i] == NOCHAR_2 || crypt[i] == NOCHAR_3)
				{
					crypt[i] = orig[j];
				}
			}
		}
		for (i = 0; i < clng; i++)
		{
			if (isupper(crypt[i]) && crypt[i] & (char)0X01)
			{
				crypt[i] = (char)tolower(crypt[i]);
			}
			else if (islower(crypt[i]) && crypt[i] & (char)0X01)
			{
				crypt[i] = (char)toupper(crypt[i]);
			}
		}
		crypt[clng] = '\0';

		// debug info
		m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::encryptOld - done");
		return std::string(crypt);
	}

	std::string CUADMLoginInterface::decryptOld(std::string data)
	{
		if (data.length() != 8)
		{
			// the old decryption method only supports max 8 chars
			return "";
		}
		int i;
		int j;
		int clng = 8;
		int NOCHAR_1 = 127;
		int NOCHAR_2 = '"';
		int NOCHAR_3 = '\'';
		int perm[] = { 2, 7, 1, 5, 3, 0, 4, 6 };
		int PERM_ANZ = (sizeof(perm) / sizeof(int));
		char offs[] = { 0X1C, 0X08, 0X1B, 0X05, 0X17, 0X0D, 0X13, 0X09 };
		int OFFS_ANZ = (sizeof(offs) / sizeof(char));
		char* crypt = (char*)data.c_str();
		char orig[9];
		for (i = 0; i < clng; i++)
		{
			if (isupper(crypt[i]) && crypt[i] & (char)0X01)
			{
				crypt[i] = (char)tolower(crypt[i]);
			}
			else if (islower(crypt[i]) && crypt[i] & (char)0X01)
			{
				crypt[i] = (char)toupper(crypt[i]);
			}
		}
		for (i = 0; i < clng; i++)
		{
			j = perm[i % PERM_ANZ];
			if (crypt[i] == NOCHAR_1 || crypt[i] == NOCHAR_2 || crypt[i] == NOCHAR_3)
			{
				orig[j] = crypt[i];
			}
			else
			{
				orig[j] = (crypt[i] ^ offs[i % OFFS_ANZ]) & (char)0X7F;
				if (orig[j] == NOCHAR_1 || orig[j] == NOCHAR_2 || orig[j] == NOCHAR_3)
				{
					orig[j] = crypt[i];
				}
			}
		}
		orig[clng] = '\0';

		return std::string(orig); // return the decrypted password
	}

	std::string CUADMLoginInterface::encryptNew(std::string data, std::string key)
	{
		// debug info
		m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::encryptNew - start");
		std::string ret = "";
		try
		{
			// encrypt the password with the new encryption method
			if (data.length() > 255)
			{
				data = data.substr(0, 255); // limit to 255 chars
			}
			ret = AESEncrypt(data, key); // use username as key for AES encryption
		}
		catch (...)
		{
			// encryption failed!
			ret = "";
			m_strLastErrorMessage = "AES encryption failed!";
			m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::encryptNew - " + m_strLastErrorMessage);
		}
		// debug info
		m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::encryptNew - done");
		return ret;
	}

	std::string CUADMLoginInterface::decryptNew(std::string data, std::string key)
	{
		std::string ret = "";
		try
		{
			// decrypt the password with the new encryption method
			if (data.length() > 255)
			{
				data = data.substr(0, 255); // limit to 255 chars
			}
			ret = AESDecrypt(data, key); // use username as key for AES decryption
		}
		catch (...)
		{
			// decryption failed!
			ret = "";
			m_strLastErrorMessage = "AES decryption failed!";
			m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": CUADMLoginInterface::decryptNew - "+ m_strLastErrorMessage);
		}
		return ret;
	}

	std::string CUADMLoginInterface::trim(std::string data)
	{
		std::string ret = data;
		bool blnSpace = true;
		// remove leading and trailing spaces
		while ((ret.length() > 0) && (blnSpace == true))
		{
			blnSpace = false;
			// leading
			if (ret.at(0) == ' ')
			{
				blnSpace = true;
				ret = ret.substr(1);
			}
			// trailing
			else if (ret.at(ret.length() - 1) == ' ')
			{
				blnSpace = true;
				ret = ret.substr(0, ret.length() - 1);
			}
		}
		return ret;
	}

	std::string CUADMLoginInterface::toUpper(std::string data)
	{
		std::string ret = data;
		std::transform(ret.begin(), ret.end(), ret.begin(), ::toupper);
		return ret;
	}

	std::string CUADMLoginInterface::toLower(std::string data)
	{
		std::string ret = data;
		std::transform(ret.begin(), ret.end(), ret.begin(), ::tolower);
		return ret;
	}

	std::string CUADMLoginInterface::replaceAll(std::string strData, std::string strFrom, std::string strTo)
	{
		std::string ret = strData;
		size_t pos = ret.find(strFrom);
		while (pos != std::string::npos)
		{
			ret.replace(pos, strFrom.length(), strTo);
			pos = ret.find(strFrom, pos + strTo.length());
		}
		return ret;
	}

	std::string CUADMLoginInterface::replaceAllSpecialChars(std::string strData)
	{
		// supporting UTF-8 special characters for german umlauts
		std::string ret = strData;
		ret = replaceAll(ret, "Ã¼", "ü");
		ret = replaceAll(ret, "Ã¤", "ä");
		ret = replaceAll(ret, "Ã¶", "ö");
		ret = replaceAll(ret, "Ãœ", "Ü");
		ret = replaceAll(ret, "Ã„", "Ä");
		ret = replaceAll(ret, "Ã–", "Ö");
		return ret;
	}

	std::string CUADMLoginInterface::convertConnectionData(std::string conn, bool bService)
	{
		std::string ret = conn;
		// naming convention within PHOENIX are as follows:
		// connection to a server is always without _tcp or _shm (e.g. zdev21, ztst21, zqas21, ode21, ide21)
		// connection to a service (e.g. informix) is always with _tcp or _shm (e.g. zdev21_tcp, zdev21_shm, ztst21_tcp, zqas21_tcp, ode21_tcp, ide21_tcp)
		if (bService == false)
		{
			// remove any underscore and following characters (instance name, e.g. zdev21_tcp or zdev21_shm -> zdev21)
			size_t pos = ret.find_first_of('_');
			if (pos != std::string::npos)
			{
				ret = ret.substr(0, pos);
			}
		}
		else
		{
			// add _tcp if no _ is present (e.g. zdev21 -> zdev21_tcp)
			size_t pos = ret.find_first_of('_');
			if (pos == std::string::npos)
			{
				ret += "_tcp";
			}
		}
		return ret;
	}

	void CUADMLoginInterface::verifyEnvironment(std::string& strEnv, std::string& strName, std::string& strPrimaryServer, 
												std::string& strPrimaryDatabase, std::string& strSecondaryServer, 
												std::string& strSecondaryDatabase, std::string& strVoodooPipeName, std::string& strVoodooPipePort)
	{
		// dont like this function, but for backward compatibility we need it to set default values if not set
		if (strPrimaryServer.empty() == false)
		{
			// check server if contains dev, tst, qas to set corresponding environment if env = PRD set wrongly
			if ((strEnv == "PRD") || (strEnv.empty() == true))
			{
				std::string srv = toLower(strPrimaryServer);
				if (srv.find("dev") != std::string::npos)
				{
					strEnv = "DEV";
					strName = "Development";
				}
				else if (srv.find("tst") != std::string::npos)
				{
					strEnv = "TST";
					strName = "Test";
				}
				else if (srv.find("qas") != std::string::npos)
				{
					strEnv = "ACC";
					strName = "Acceptance";
				}
				else if (srv.find("ode") != std::string::npos)
				{
					strEnv = "PRD";
					strName = "Production";
				}
			}
		}
		else if (strEnv.empty() == false)
		{
			// set default server names if not set and env is set
			if (strEnv == "DEV")
			{
				if (strName.empty() == true)
				{
					strName = "Development";
				}
				strPrimaryServer = "zdev21_tcp"; // default dev server
				strPrimaryDatabase = "zpcua1"; // default dev database
				strSecondaryServer = "zdev21_tcp"; // default dev server
				strSecondaryDatabase = "zpcua2"; // default dev database
			}
			else if (strEnv == "TST")
			{
				if (strName.empty() == true)
				{
					strName = "Test";
				}
				strPrimaryServer = "ztst21_tcp"; // default dev server
				strPrimaryDatabase = "zpcua1"; // default dev database
				strSecondaryServer = "ztst21_tcp"; // default dev server
				strSecondaryDatabase = "zpcua2"; // default dev database
			}
			else if (strEnv == "ACC")
			{
				if (strName.empty() == true)
				{
					strName = "Acceptance";
				}
				strPrimaryServer = "zqas21_tcp"; // default dev server
				strPrimaryDatabase = "zpcua1"; // default dev database
				strSecondaryServer = "zqas21_tcp"; // default dev server
				strSecondaryDatabase = "zpcua2"; // default dev database
			}
			else if (strEnv == "PRD")
			{
				if (strName.empty() == true)
				{
					strName = "Production";
				}
				strPrimaryServer = "ode21_tcp"; // default dev server
				strPrimaryDatabase = "zpcua1"; // default dev database
				strSecondaryServer = "ide21_tcp"; // default dev server
				strSecondaryDatabase = "zpcua2"; // default dev database
			}
		}
		if (strVoodooPipeName.empty() == true)
		{
			strVoodooPipeName = "loguiserv"; // default voodoo pipe name
		}
		if (strVoodooPipePort.empty() == true)
		{
			strVoodooPipePort = "58001"; // default voodoo pipe port
		}
	}

	// debug info for date and time
	std::string CUADMLoginInterface::getSystemDateTimeString()
	{
		SYSTEMTIME st;
		GetLocalTime(&st);
		char buffer[100];
		sprintf_s(buffer, "%02d.%02d.%04d %02d:%02d:%02d", st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute, st.wSecond);
		return std::string(buffer);
	}

	int CUADMLoginInterface::executeSQL(std::string strSQL)
	{
		if ((m_enmUADMConnectionType == UADM_CONNECTION_TYPE_NONE) || (m_pobjUADMEnvironment == NULL) || ((m_pobjIFXConn == NULL) && (m_intVoodooConn < 0)))
		{
			// no server connection established
			m_strLastErrorMessage = "Server connection not established";
			return -1;
		}
		if (strSQL.empty() == true)
		{
			m_strLastErrorMessage = "No SQL statement provided";
			return -1;
		}
		if (((m_enmUADMConnectionType == UADM_CONNECTION_TYPE_VOODOO_PRIMARY) || (m_enmUADMConnectionType == UADM_CONNECTION_TYPE_VOODOO_SECONDARY)) && (m_intVoodooConn >= 0))
		{
			// execute SQL via voodoo connection
			long result = SqlExecuteImm(strSQL.c_str());
			if (result != 0)
			{
				m_strLastErrorMessage = "Failed to execute SQL statement via Voodoo connection: " + std::string(DynLastErrmld());
				return -1;
			}
		}
		else if (((m_enmUADMConnectionType == UADM_CONNECTION_TYPE_INFORMIX_PRIMARY) || (m_enmUADMConnectionType == UADM_CONNECTION_TYPE_INFORMIX_SECONDARY)) && (m_pobjIFXConn != NULL))
		{
#ifndef EXLUDE_INFORMIX
			// execute SQL via informix connection
			ITQuery dbQuery(*m_pobjIFXConn);
			ITBool result = dbQuery.ExecForStatus(ITString(strSQL.c_str()));
			if (result != TRUE)
			{
				if (dbQuery.Error() == TRUE)
				{
					m_strLastErrorMessage = "Failed to execute SQL statement via Informix connection: " + std::string(dbQuery.ErrorText().Data());
					return -1;
				}
			}
#endif
		}
		return 0; // ok
	}

	int CUADMLoginInterface::readSQLValue(std::string strSQL, std::string& strValue)
	{
		if ((m_enmUADMConnectionType == UADM_CONNECTION_TYPE_NONE) || (m_pobjUADMEnvironment == NULL) || ((m_pobjIFXConn == NULL) && (m_intVoodooConn < 0)))
		{
			// no server connection established
			m_strLastErrorMessage = "Server connection not established";
			return -1;
		}
		if (strSQL.empty() == true)
		{
			m_strLastErrorMessage = "No SQL statement provided";
			return -1;
		}
		if (((m_enmUADMConnectionType == UADM_CONNECTION_TYPE_VOODOO_PRIMARY) || (m_enmUADMConnectionType == UADM_CONNECTION_TYPE_VOODOO_SECONDARY)) && (m_intVoodooConn >= 0))
		{
			char tmpValue[255];
			long status = SqlRead(strSQL.c_str(), &tmpValue, NULL);
			if (status == 0)
			{
				strValue = std::string(tmpValue);
			}
			else if (status == 100)
			{
				return 1; // no error but no value
			}
			else
			{
				m_strLastErrorMessage = "Failed to read SQL value via Voodoo connection: " + std::string(DynLastErrmld());
				return -1; // error
			}
		}
		else if (((m_enmUADMConnectionType == UADM_CONNECTION_TYPE_INFORMIX_PRIMARY) || (m_enmUADMConnectionType == UADM_CONNECTION_TYPE_INFORMIX_SECONDARY)) && (m_pobjIFXConn != NULL))
		{
#ifndef EXLUDE_INFORMIX
			// execute SQL via informix connection
			ITQuery dbQuery(*m_pobjIFXConn);
			// set isolation to prevent locks
			ITRow* dbRow = dbQuery.ExecOneRow(ITString(strSQL.c_str()));
			if (dbRow != NULL)
			{
				// read the first column only as string
				ITValue* dbValue = dbRow->Column(0);
				if (dbValue->IsNull() == false)
				{
					strValue = std::string(dbValue->Printable().Data());
				}
				else
				{
					strValue = "";
				}
				dbRow->Release();
			}
			else
			{
				if (dbQuery.Error() == TRUE)
				{ 
					m_strLastErrorMessage = "Failed to execute SQL statement via Informix connection: " + std::string(dbQuery.ErrorText().Data());
					return -1;
				}
				return 1; // no error but no value
			}
#endif
		}
		return 0; // ok
	}

	void CUADMLoginInterface::addDebugInfo(std::string debugInfo)
	{
		if (m_pvecDebugInfos != NULL)
		{
			m_pvecDebugInfos->push_back(getSystemDateTimeString() + ": " + debugInfo);
		}
	}
}
/////////////////////////////////////////////////////////////////////////////
