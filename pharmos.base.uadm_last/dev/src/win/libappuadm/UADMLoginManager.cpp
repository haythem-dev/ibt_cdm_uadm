// CUADMLoginManager.cpp: implementation file
//

#include "pch.h"
#include "framework.h"
#include "UADMLoginManager.h"
#include "UADMLoginInterface.h"
#include "UADMLoginDlg.h"
#include "uadm_version.h"

/////////////////////////////////////////////////////////////////////////////
// UADM Login Manager 
namespace UADM
{
	CUADMLoginManager::CUADMLoginManager()
	{
		// initialize member variables
		m_lngAppID = 0;
		m_strAppKey = "";
		m_strAppName = "";
		m_strAppDesc = "";
		m_strAppVersion = "";

		m_lngUserID = 0;
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

		m_strEnvironment = "";
		m_strServername = "";
		m_strDatabase = "";
		m_strServiceName = "";
		m_intPortNo = 0;
		m_strPortNo = "";
		m_strCountry = "";
		m_blnRegionBranchOnly = true;
		m_blnUserMapping = false;
		m_intRegionBranchNo = 0;
		m_strRegionBranchNo = "";
		m_strRegionBranchName = "";
		m_strLanguage = "";
		m_strLanguageName = "";

		m_strLastErrorMessage = "";
		m_pobjLoginInterface = new CUADMLoginInterface(&m_vecDebugInfos);
		// debug info
		m_vecDebugInfos.push_back(m_pobjLoginInterface->getSystemDateTimeString() + ": UADMLoginManager::constructor - Version " + UADM_VERSION);
	}

	CUADMLoginManager::~CUADMLoginManager()
	{
		// cleanup
		reset();
		if (m_pobjLoginInterface != NULL)
		{
			m_pobjLoginInterface->closeConnection(true);
			delete m_pobjLoginInterface;
			m_pobjLoginInterface = NULL;
		}
		m_vecDebugInfos.clear();
	}

	//////////////////////////////////////////////////////////////////////////////
	// Implementation
	void CUADMLoginManager::reset()
	{
		// debug info
		m_vecDebugInfos.push_back(m_pobjLoginInterface->getSystemDateTimeString() + ": UADMLoginManager::reset - delete all internal members and lists");
		if (m_pobjLoginInterface != NULL)
		{
			m_pobjLoginInterface->reset();
		}
		// clean up the vectors
		while (m_vecLanguages.size() > 0)
		{
			UADM_APP_LANGUAGE* pLang = *m_vecLanguages.begin();
			if (pLang != NULL)
			{
				delete pLang;
				pLang = NULL;
			}
			m_vecLanguages.erase(m_vecLanguages.begin());
		}
		m_vecLanguages.clear();

		while (m_vecEnvironments.size() > 0)
		{
			UADM_APP_ENVIRONMENT* pEnv = *m_vecEnvironments.begin();
			if (pEnv != NULL)
			{
				delete pEnv;
				pEnv = NULL;
			}
			m_vecEnvironments.erase(m_vecEnvironments.begin());
		}
		m_vecEnvironments.clear();

		while (m_vecEntries.size() > 0)
		{
			UADM_APP_ENTRY* pEntry = *m_vecEntries.begin();
			if (pEntry != NULL)
			{
				delete pEntry;
				pEntry = NULL;
			}
			m_vecEntries.erase(m_vecEntries.begin());
		}
		m_vecEntries.clear();

		while (m_vecCountries.size() > 0)
		{
			UADM_APP_COUNTRY* pCountry = *m_vecCountries.begin();
			if (pCountry != NULL)
			{
				delete pCountry;
				pCountry = NULL;
			}
			m_vecCountries.erase(m_vecCountries.begin());
		}
		m_vecCountries.clear();

		while (m_vecRegionBranches.size() > 0)
		{
			UADM_APP_REGIONBRANCH* pRegionBranch = *m_vecRegionBranches.begin();
			if (pRegionBranch != NULL)
			{
				delete pRegionBranch;
				pRegionBranch = NULL;
			}
			m_vecRegionBranches.erase(m_vecRegionBranches.begin());
		}
		m_vecRegionBranches.clear();

		while (m_vecPermissions.size() > 0)
		{
			UADM_APP_PERMISSION* pPerm = *m_vecPermissions.begin();
			if (pPerm != NULL)
			{
				delete pPerm;
				pPerm = NULL;
			}
			m_vecPermissions.erase(m_vecPermissions.begin());
		}
		m_vecPermissions.clear();

		while (m_vecParameters.size() > 0)
		{
			UADM_APP_PARAMETER* pPara = *m_vecParameters.begin();
			if (pPara != NULL)
			{
				delete pPara;
				pPara = NULL;
			}
			m_vecParameters.erase(m_vecParameters.begin());
		}
		m_vecParameters.clear();

		while (m_vecUserMappings.size() > 0)
		{
			UADM_USER_MAPPING* pMap = *m_vecUserMappings.begin();
			if (pMap != NULL)
			{
				delete pMap;
				pMap = NULL;
			}
			m_vecUserMappings.erase(m_vecUserMappings.begin());
		}
		m_vecUserMappings.clear();

		while (m_vecUserRoles.size() > 0)
		{
			UADM_USER_ROLE* pRole = *m_vecUserRoles.begin();
			if (pRole != NULL)
			{
				delete pRole;
				pRole = NULL;
			}
			m_vecUserRoles.erase(m_vecUserRoles.begin());
		}
		m_vecUserRoles.clear();
		// debug info
		m_vecDebugInfos.push_back(m_pobjLoginInterface->getSystemDateTimeString() + ": UADMLoginManager::reset - done");
	}

	void CUADMLoginManager::setConnectionTypePreference(int intPref)
	{
		if (m_pobjLoginInterface != NULL)
		{
			m_pobjLoginInterface->setConnectionTypePreference(intPref);
		}
	}

	bool CUADMLoginManager::initialize(std::string strINIFile, std::string strAppKey, std::string strAppName, std::string strAppVersion)
	{
		reset();
		// debug info
		m_vecDebugInfos.push_back(m_pobjLoginInterface->getSystemDateTimeString() + ": UADMLoginManager::initialize - start for app "+strAppName+" via INI file " + strINIFile);

		m_strAppKey = strAppKey;
		m_strAppName = strAppName;
		m_strAppVersion = strAppVersion;
		m_strLastErrorMessage = "";
		m_pobjLoginInterface->resetLastErrorMessage();

		// read the UADM settings from the INI file via stl framework to be compatible with windows and unix
		std::ifstream infile(strINIFile);
		if (infile.good() == false)
		{
			m_strLastErrorMessage = "INI file " + strINIFile + " not found or cannot be opened!";
			return false;
		}
		std::vector<std::string> lines;
		std::string line;
		while (std::getline(infile, line))
		{
			line.erase(std::remove(line.begin(), line.end(), '\r'), line.end()); // remove carriageReturn character
			line.erase(std::remove(line.begin(), line.end(), '\n'), line.end()); // remove newline character
			line = CUADMLoginInterface::replaceAllSpecialChars(line); // remove all tab characters
			lines.push_back(line);
		}
		infile.close();
		// now scan the lines for the UADM settings
		int cntLanguages = getLineNumber(lines, "UADM", "NumberOfLanguages");
		for (int cntL = 1; cntL <= cntLanguages; cntL++)
		{
			UADM_APP_LANGUAGE* pLang = new UADM_APP_LANGUAGE();
			pLang->strLanguage = getLineString(lines, "UADM", "Language" + std::to_string((long long)cntL));
			pLang->strLangName = getLineString(lines, "UADM", "LanguageName" + std::to_string((long long)cntL));
			pLang->strLangFile = getLineString(lines, "UADM", "LanguageFile" + std::to_string((long long)cntL));
			m_vecLanguages.push_back(pLang);
		}
		int cntEnvironments = getLineNumber(lines, "UADM", "NumberOfEnvironments");
		for (int cntE = 1; cntE <= cntEnvironments; cntE++)
		{
			UADM_APP_ENVIRONMENT* pEnv = new UADM_APP_ENVIRONMENT();
			pEnv->strEnvironment = getLineString(lines, "UADM", "Environment" + std::to_string((long long)cntE));
			pEnv->strEnvName = getLineString(lines, "UADM", "EnvironmentName" + std::to_string((long long)cntE));
			pEnv->strPrimaryServer = getLineString(lines, "UADM", "PrimaryServer" + std::to_string((long long)cntE));
			pEnv->strPrimaryDatabase = getLineString(lines, "UADM", "PrimaryDatabase" + std::to_string((long long)cntE));
			pEnv->strSecondaryServer = getLineString(lines, "UADM", "SecondaryServer" + std::to_string((long long)cntE));
			pEnv->strSecondaryDatabase = getLineString(lines, "UADM", "SecondaryDatabase" + std::to_string((long long)cntE));
			pEnv->strVoodooPipeName = getLineString(lines, "UADM", "VoodooPipeName" + std::to_string((long long)cntE));
			pEnv->strVoodooPipePort = getLineString(lines, "UADM", "VoodooPipePort" + std::to_string((long long)cntE));
			pEnv->intVoodooPipePort = atoi(pEnv->strVoodooPipePort.c_str()); // convert the voodoo pipe port to an integer
			m_vecEnvironments.push_back(pEnv);
		}
		// get the preferred connection type
		std::string conntype = getLineString(lines, "UADM", "ConnectionType");
		if (CUADMLoginInterface::toUpper(conntype) == "VOODOO")
		{
			m_pobjLoginInterface->setConnectionTypePreference(1); // voodoo preferred
		}
		else
		{
			// default is informix
			m_pobjLoginInterface->setConnectionTypePreference(0); // informix preferred
		}

		// now scan for the different entries for the specific app
		int cntEntries = getLineNumber(lines, m_strAppName, "NumberOfEntries");
		for (int cntN = 1; cntN <= cntEntries; cntN++)
		{
			UADM_APP_ENTRY* pEntry = new UADM_APP_ENTRY();
			pEntry->intEntryNo = cntN;
			pEntry->strEntryName = getLineString(lines, m_strAppName, "EntryName" + std::to_string((long long)cntN));
			pEntry->strEnvironment = getLineString(lines, m_strAppName, "Environment" + std::to_string((long long)cntN));
			pEntry->strCountry = getLineString(lines, m_strAppName, "Country" + std::to_string((long long)cntN));
			pEntry->strRegionBranchNo = getLineString(lines, m_strAppName, "RegionBranch" + std::to_string((long long)cntN));
			pEntry->intRegionBranchNo = atoi(pEntry->strRegionBranchNo.c_str()); // convert the regionbranch number to an integer
			pEntry->strRegionBranchName = getLineString(lines, m_strAppName, "RegionBranchName" + std::to_string((long long)cntN));
			pEntry->strServer = getLineString(lines, m_strAppName, "Server" + std::to_string((long long)cntN));
			pEntry->strDatabase = getLineString(lines, m_strAppName, "Database" + std::to_string((long long)cntN));
			pEntry->strServiceName = getLineString(lines, m_strAppName, "ServiceName" + std::to_string((long long)cntN));
			pEntry->strPortNo = getLineString(lines, m_strAppName, "Port" + std::to_string((long long)cntN));
			pEntry->intPortNo = atoi(pEntry->strPortNo.c_str()); // convert the port number to an integer
			m_vecEntries.push_back(pEntry);
		}

		m_vecDebugInfos.push_back(m_pobjLoginInterface->getSystemDateTimeString() + ": UADMLoginManager::initialize done - INI file with " + std::to_string((long long)cntLanguages) + " languages, " + std::to_string((long long)cntEnvironments) + " environments and " + std::to_string((long long)cntEntries) + " entries");
		// debug info
		return true;
	}

	bool CUADMLoginManager::initialize(std::string strEnv, std::string strName, std::string strPrimaryServer, std::string strPrimaryDatabase, 
									   std::string strSecondaryServer, std::string strSecondaryDatabase, std::string strVoodooPipeName, 
									   std::string strVoodooPipePort, std::string strAppName, std::string strAppVersion)
	{
		reset();
		// debug info
		m_vecDebugInfos.push_back(m_pobjLoginInterface->getSystemDateTimeString() + ": UADMLoginManager::initialize - start for app " + strAppName + " via config on " + strPrimaryServer);

		// set the application name and version
		m_strAppKey = ""; // no app key, no regstristry used in this case
		m_strAppName = strAppName;
		m_strAppVersion = strAppVersion;
		m_strLastErrorMessage = "";
		m_pobjLoginInterface->resetLastErrorMessage();

		// verify the given environment settings
		m_pobjLoginInterface->verifyEnvironment(strEnv, strName, strPrimaryServer, strPrimaryDatabase, strSecondaryServer, strSecondaryDatabase, strVoodooPipeName, strVoodooPipePort);
		// initialize the login interface with the given environment settings
		UADM_APP_ENVIRONMENT* pEnv = new UADM_APP_ENVIRONMENT();
		pEnv->strEnvironment = strEnv;
		pEnv->strEnvName = strName;
		pEnv->strPrimaryServer = strPrimaryServer;
		pEnv->strPrimaryDatabase = strPrimaryDatabase;
		pEnv->strSecondaryServer = strSecondaryServer;
		pEnv->strSecondaryDatabase = strSecondaryDatabase;
		pEnv->strVoodooPipeName = strVoodooPipeName;
		pEnv->strVoodooPipePort = strVoodooPipePort;
		pEnv->intVoodooPipePort = atoi(strVoodooPipePort.c_str());
		m_vecEnvironments.push_back(pEnv);

		// connect to uadm environment
		m_pobjLoginInterface->initialize(pEnv);
		if (m_pobjLoginInterface->openConnection() == false)
		{
			// connection to uadm environment failed, so return false
			m_strLastErrorMessage = m_pobjLoginInterface->getLastErrorMessage();
			return false;
		}

		// read the available entries for this app from this uadm environment
		if (m_pobjLoginInterface->getAppEntries(m_strAppName ,&m_vecLanguages, &m_vecEntries) == false)
		{
			// reading the entries failed, so return false
			m_strLastErrorMessage = m_pobjLoginInterface->getLastErrorMessage();
			return false;
		}
		// debug info
		m_vecDebugInfos.push_back(m_pobjLoginInterface->getSystemDateTimeString() + ": UADMLoginManager::initialize - done for app " + strAppName + " via config on " + strPrimaryServer);

		return true;
	}
	
	bool CUADMLoginManager::closeConnection()
	{
		bool ret = true;
		// debug info
		m_vecDebugInfos.push_back(m_pobjLoginInterface->getSystemDateTimeString() + ": UADMLoginManager::closeConnection - start");

		if (m_pobjLoginInterface != NULL)
		{
			// close connection either with data reset (user mapping disabled) or without data reset (user mapping enabled)
			ret = m_pobjLoginInterface->closeConnection(m_blnUserMapping == false);
		}
		// debug info
		m_vecDebugInfos.push_back(m_pobjLoginInterface->getSystemDateTimeString() + ": UADMLoginManager::closeConnection - done");

		return ret;
	}

	std::string CUADMLoginManager::getUsedUADMConnectionType()
	{
		std::string ret = "";
		if (m_pobjLoginInterface != NULL)
		{
			ret = m_pobjLoginInterface->getLastUADMConnectionType();
		}
		return ret;
	}

	std::string CUADMLoginManager::getUsedUADMServername()
	{
		std::string ret = "";
		if (m_pobjLoginInterface != NULL)
		{
			ret = m_pobjLoginInterface->getLastUADMServername();
		}
		return ret;
	}
	
	std::string CUADMLoginManager::getUsedUADMDatabase()
	{
		std::string ret = "";
		if (m_pobjLoginInterface != NULL)
		{
			ret = m_pobjLoginInterface->getLastUADMDatabase();
		}
		return ret;
	}

	std::string CUADMLoginManager::getUsedUADMPort()
	{
		std::string ret = "";
		if (m_pobjLoginInterface != NULL)
		{
			ret = m_pobjLoginInterface->getLastUADMPort();
		}
		return ret;
	}

	std::string CUADMLoginManager::getUsedEncryptionType()
	{
		std::string ret = "";
		if (m_pobjLoginInterface != NULL)
		{
			ret = m_pobjLoginInterface->getLastUADMEncryptionType();
		}
		return ret;
	}

	bool CUADMLoginManager::loginWithDialog(std::string strBackgroundImageFile, HWND hParent)
	{
		bool ret = false;
		// debug info
		m_vecDebugInfos.push_back(m_pobjLoginInterface->getSystemDateTimeString() + ": UADMLoginManager::loginWithDialog - start");

		m_strLastErrorMessage = "";
		m_pobjLoginInterface->resetLastErrorMessage();
		// initialize resource from DLL
		HMODULE hDLL = NULL;
		// Use the address of a function in this module to get the module handle
		GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
			reinterpret_cast<LPCSTR>(reinterpret_cast<void*>(UADMStaticAddress)), &hDLL);

		CUADMLoginDlg loginDlg(m_pobjLoginInterface, hDLL, hParent);
		// hand over parameters from ini
		loginDlg.setAppNameVersion(m_strAppKey.c_str(), m_strAppName.c_str(), m_strAppVersion.c_str());
		loginDlg.setBackgroundImage(strBackgroundImageFile.c_str());
		loginDlg.setAppLanguages(&m_vecLanguages);
		loginDlg.setAppEnvironments(&m_vecEnvironments);
		loginDlg.setAppEntries(&m_vecEntries);
		loginDlg.setUserMapping(m_blnUserMapping);
		// let decide the dialog the user interactions and login and open the connection via interface
		INT_PTR result = loginDlg.DoModal();
		if (result == IDOK)
		{
			// take over all values from interface
			m_lngUserID = m_pobjLoginInterface->getUserID();
			m_strUsername = m_pobjLoginInterface->getUsername();
			m_strEncPwd = m_pobjLoginInterface->getEncPwd();
			m_strFirstname = m_pobjLoginInterface->getFirstname();
			m_strLastname = m_pobjLoginInterface->getLastname();
			m_strMail = m_pobjLoginInterface->getMail();
			m_strEMail = m_pobjLoginInterface->getEMail();
			m_strDomainAccount = m_pobjLoginInterface->getDomainAccount();
			m_strDescription = m_pobjLoginInterface->getDescription();
			m_strOSAccount = m_pobjLoginInterface->getOSAccount();
			m_blnIsActive = m_pobjLoginInterface->isUserActive();
			m_blnForcePasswordChange = m_pobjLoginInterface->isPasswordChangeRequired();
			m_lngLastPasswordChangeDate = m_pobjLoginInterface->getLastPasswordChangeDate();
			m_lngLastLoginDate = m_pobjLoginInterface->getLastLoginDate();
			m_strLanguage = m_pobjLoginInterface->getLanguage();
			m_strLanguageName = m_pobjLoginInterface->getLanguageName();
			m_strServername = m_pobjLoginInterface->getServername();
			m_strDatabase = m_pobjLoginInterface->getDatabase();
			m_strServiceName = m_pobjLoginInterface->getServiceName();
			m_intPortNo = m_pobjLoginInterface->getPortNoVal();
			m_strPortNo = m_pobjLoginInterface->getPortNo();
			m_strEnvironment = m_pobjLoginInterface->getEnvironment();
			m_strCountry = m_pobjLoginInterface->getCountry();
			m_intRegionBranchNo = m_pobjLoginInterface->getRegionBranchNoVal();
			m_strRegionBranchNo = m_pobjLoginInterface->getRegionBranchNo();
			m_strRegionBranchName = m_pobjLoginInterface->getRegionBranchName();

			// read the application settings from the login interface and save the login info in case of successfull read
			if (m_pobjLoginInterface->verifyApplication(m_strAppName) == false)
			{
				// application cannot be found, so no permission, no parameters
				m_strLastErrorMessage = m_pobjLoginInterface->getLastErrorMessage();
				return false;
			}
			m_lngAppID = m_pobjLoginInterface->getAppID();
			m_strAppName = m_pobjLoginInterface->getAppName();
			m_strAppDesc = m_pobjLoginInterface->getAppDesc();

			if (m_pobjLoginInterface->getApplicationRights(&m_vecPermissions, &m_vecUserRoles, m_intRegionBranchNo, m_blnRegionBranchOnly) == true)
			{
				// permissions found, no error handling needed here, as no permissions means no rights
			}
			if (m_pobjLoginInterface->getApplicationParameters(&m_vecParameters) == true)
			{
				// params found, no error handling needed here, as no parameters means no setings
			}
			if (m_pobjLoginInterface->saveLastLoginData() == true)
			{
				// login previous info saved, no error handling needed here, as this is just for info
			}
			if (m_blnUserMapping == true)
			{
				// special to load branch specific user mappings (default only for this user and this branch)
				m_pobjLoginInterface->getUserMappings(&m_vecUserMappings, m_intRegionBranchNo, m_lngUserID, 0);
			}
			ret = true;
		}
		else if (result == IDCANCEL)
		{
			// user cancelled the login dialog
			m_strLastErrorMessage = "User cancelled the login dialog!";
		}
		else
		{
			// dialog creation failed
			m_strLastErrorMessage = "Login dialog creation failed!";
		}
		// close connection in all cases, user mappings can also be loaded afterwards with same login manager object
		closeConnection(); // close the connection to the database, as we have all information now
		// debug info
		m_vecDebugInfos.push_back(m_pobjLoginInterface->getSystemDateTimeString() + ": UADMLoginManager::loginWithDialog - done");

		return ret;
	}

	bool CUADMLoginManager::loginWithoutDialog(std::string strUsername, std::string strCryptPWD, std::string strCountry, int intRegionBranch, std::string strLanguage)
	{
		bool ret = false;
		// debug info
		m_vecDebugInfos.push_back(m_pobjLoginInterface->getSystemDateTimeString() + ": UADMLoginManager::loginWithoutDialog - start");

		// do the login without any interaction with the user, but only if LoginInterface is created before
		if (m_pobjLoginInterface == NULL)
		{
			m_strLastErrorMessage = "Login interface not initialized!";
			return false;
		}
		// get environment
		if (m_vecEnvironments.size() == 0)
		{
			m_strLastErrorMessage = "No environment defined, initialize method was not called before!";
			return false;
		}
		// the initialize method has to be called before to establish a connection
		UADM_APP_ENVIRONMENT* pEnv = m_vecEnvironments.at(0); // take the first environment in the vector
		if (m_pobjLoginInterface->isConnected(pEnv) == false)
		{
			// connection to uadm environment failed, so return false
			m_strLastErrorMessage = m_pobjLoginInterface->getLastErrorMessage();
			return false;
		}

		// verify user
		if (m_pobjLoginInterface->verifyUser(strUsername, strCryptPWD, true) == false)
		{
			// user verification failed, so return false
			m_strLastErrorMessage = m_pobjLoginInterface->getLastErrorMessage();
			return false;
		}
		// check that user is active
		if (m_pobjLoginInterface->isUserActive() == false)
		{
			m_strLastErrorMessage = "User " + strUsername + " is not active!";
			return false;
		}
		// take over all values from interface
		m_lngUserID = m_pobjLoginInterface->getUserID();
		m_strUsername = m_pobjLoginInterface->getUsername();
		m_strEncPwd = m_pobjLoginInterface->getEncPwd();
		m_strFirstname = m_pobjLoginInterface->getFirstname();
		m_strLastname = m_pobjLoginInterface->getLastname();
		m_strMail = m_pobjLoginInterface->getMail();
		m_strEMail = m_pobjLoginInterface->getEMail();
		m_strDomainAccount = m_pobjLoginInterface->getDomainAccount();
		m_strDescription = m_pobjLoginInterface->getDescription();
		m_strOSAccount = m_pobjLoginInterface->getOSAccount(); // not filled on noninteractive mode
		m_blnIsActive = m_pobjLoginInterface->isUserActive();
		m_blnForcePasswordChange = m_pobjLoginInterface->isPasswordChangeRequired();
		m_lngLastPasswordChangeDate = m_pobjLoginInterface->getLastPasswordChangeDate();
		m_lngLastLoginDate = m_pobjLoginInterface->getLastLoginDate();

		// verify application
		if (m_pobjLoginInterface->verifyApplication(m_strAppName) == false)
		{
			// application cannot be found, so no permission, no parameters
			m_strLastErrorMessage = m_pobjLoginInterface->getLastErrorMessage();
			return false;
		}
		m_lngAppID = m_pobjLoginInterface->getAppID();
		m_strAppName = m_pobjLoginInterface->getAppName();
		m_strAppDesc = m_pobjLoginInterface->getAppDesc();

		m_pobjLoginInterface->findAndSetLanguage(&m_vecLanguages, strLanguage);
		if (m_pobjLoginInterface->findAndSetUADMEntry(&m_vecEntries, pEnv->strEnvironment, strCountry, intRegionBranch) == false)
		{
			// entry cannot be found, so no permission, no parameters
			m_strLastErrorMessage = m_pobjLoginInterface->getLastErrorMessage();
			return false;
		}
		// take over the entry settings
		m_strLanguage = m_pobjLoginInterface->getLanguage();
		m_strLanguageName = m_pobjLoginInterface->getLanguageName();
		m_strServername = m_pobjLoginInterface->getServername();
		m_strDatabase = m_pobjLoginInterface->getDatabase();
		m_strServiceName = m_pobjLoginInterface->getServiceName();
		m_intPortNo = m_pobjLoginInterface->getPortNoVal();
		m_strPortNo = m_pobjLoginInterface->getPortNo();
		m_strEnvironment = m_pobjLoginInterface->getEnvironment();
		m_strCountry = m_pobjLoginInterface->getCountry();
		m_intRegionBranchNo = m_pobjLoginInterface->getRegionBranchNoVal();
		m_strRegionBranchNo = m_pobjLoginInterface->getRegionBranchNo();
		m_strRegionBranchName = m_pobjLoginInterface->getRegionBranchName();

		// read permissions and parameters
		if (m_pobjLoginInterface->getApplicationRights(&m_vecPermissions, &m_vecUserRoles, m_intRegionBranchNo, m_blnRegionBranchOnly) == true)
		{
			// permissions found, no error handling needed here, as no permissions means no rights
		}
		if (m_pobjLoginInterface->getApplicationParameters(&m_vecParameters) == true)
		{
			// params found, no error handling needed here, as no parameters means no setings
		}
		// noninteractive login are not updating last login and not saving last login data, this is only done in dialog login
		//m_pobjLoginInterface->updateLastLoginDate();
		//m_pobjLoginInterface->saveLastLoginData();
		// close connection in all cases, user mappings can also be loaded afterwards with same login manager object
		closeConnection(); // close the connection to the database, as we have all information now

		// debug info
		m_vecDebugInfos.push_back(m_pobjLoginInterface->getSystemDateTimeString() + ": UADMLoginManager::loginWithoutDialog - done");
		ret = true;

		return ret;
	}

	std::vector<UADM_APP_COUNTRY*>* CUADMLoginManager::getCountries()
	{
		// debug info
		m_vecDebugInfos.push_back(m_pobjLoginInterface->getSystemDateTimeString() + ": UADMLoginManager::getCountries - start");
		// (re-)create the country vector
		while (m_vecCountries.size() > 0)
		{
			UADM_APP_COUNTRY* pCountry = *m_vecCountries.begin();
			if (pCountry != NULL)
			{
				delete pCountry;
				pCountry = NULL;
			}
			m_vecCountries.erase(m_vecCountries.begin());
		}
		m_vecCountries.clear();

		// scan the entries for the different countries for the current environment
		std::vector<UADM_APP_ENTRY*>::iterator it;
		for (it = m_vecEntries.begin(); it != m_vecEntries.end(); it++)
		{
			UADM_APP_ENTRY* pEntry = *it;
			if (pEntry != NULL)
			{
				// check if entry is for current environment
				if (pEntry->strEnvironment.compare(m_strEnvironment) != 0)
				{
					continue; // not for current environment, so skip it
				}
			
				// check if country already exists in vector
				bool fnd = false;
				std::vector<UADM_APP_COUNTRY*>::iterator itc;
				for (itc = m_vecCountries.begin(); itc != m_vecCountries.end(); itc++)
				{
					UADM_APP_COUNTRY* pCountry = *itc;
					if (pCountry != NULL)
					{
						if (pCountry->strCountry.compare(pEntry->strCountry) == 0)
						{
							fnd = true; // country already exists
						}
					}
				}
				if (fnd == false)
				{
					UADM_APP_COUNTRY* pCountry = new UADM_APP_COUNTRY();
					pCountry->strCountry = pEntry->strCountry;
					pCountry->strCountryName = pEntry->strCountry; // no country name in entry, so take country as name for now
					m_vecCountries.push_back(pCountry);
				}
			}
		}
		// debug info
		m_vecDebugInfos.push_back(m_pobjLoginInterface->getSystemDateTimeString() + ": UADMLoginManager::getCountries - done");
		return &m_vecCountries;
	}

	std::vector<UADM_APP_REGIONBRANCH*>* CUADMLoginManager::getRegionBranches(bool countryOnly, bool branchOnly)
	{
		// debug info
		m_vecDebugInfos.push_back(m_pobjLoginInterface->getSystemDateTimeString() + ": UADMLoginManager::getRegionBranches - start");
		// (re-)create the regionbranch vector
		while (m_vecRegionBranches.size() > 0)
		{
			UADM_APP_REGIONBRANCH* pRegionBranch = *m_vecRegionBranches.begin();
			if (pRegionBranch != NULL)
			{
				delete pRegionBranch;
				pRegionBranch = NULL;
			}
			m_vecRegionBranches.erase(m_vecRegionBranches.begin());
		}
		m_vecRegionBranches.clear();

		// scan the entries for the current regionbranches for the current environment and country
		std::vector<UADM_APP_ENTRY*>::iterator it;
		for (it = m_vecEntries.begin(); it != m_vecEntries.end(); it++)
		{
			UADM_APP_ENTRY* pEntry = *it;
			if (pEntry != NULL)
			{
				// check if entry is for current environment
				if (pEntry->strEnvironment.compare(m_strEnvironment) != 0)
				{
					continue; // not for current environment, so skip it
				}
				if (countryOnly == true)
				{
					if (pEntry->strCountry.compare(m_strCountry) != 0)
					{
						continue; // not for current country, so skip it
					}
				}
				// branches are only 2 digits, so check if regionbranch is between 0 and 99 if branchOnly is true
				if ((branchOnly == false) || ((branchOnly == true) && (pEntry->intRegionBranchNo >= 0) && (pEntry->intRegionBranchNo <= 99)))
				{
					// check if regionbranch already exists in vector
					bool fnd = false;
					std::vector<UADM_APP_REGIONBRANCH*>::iterator itc;
					for (itc = m_vecRegionBranches.begin(); itc != m_vecRegionBranches.end(); itc++)
					{
						UADM_APP_REGIONBRANCH* pRegionBranch = *itc;
						if (pRegionBranch != NULL)
						{
							if (pRegionBranch->strRegionBranchNo.compare(pEntry->strRegionBranchNo) == 0)
							{
								fnd = true; // regionbranch already exists
							}
						}
					}
					if (fnd == false)
					{
						UADM_APP_REGIONBRANCH* pRegionBranch = new UADM_APP_REGIONBRANCH();
						pRegionBranch->intRegionBranchNo = pEntry->intRegionBranchNo;
						pRegionBranch->strRegionBranchNo = pEntry->strRegionBranchNo;
						pRegionBranch->strRegionBranchName = pEntry->strRegionBranchName;
						m_vecRegionBranches.push_back(pRegionBranch);
					}
				}
			}
		}
		// debug info
		m_vecDebugInfos.push_back(m_pobjLoginInterface->getSystemDateTimeString() + ": UADMLoginManager::getRegionBranches - done");
		return &m_vecRegionBranches;
	}

	bool CUADMLoginManager::verifyPermission(int moduleid, std::string permission)
	{
		bool ret = false;
		std::vector<UADM_APP_PERMISSION*>::iterator it = m_vecPermissions.begin();
		while ((it != m_vecPermissions.end()) && (ret == false))
		{
			UADM_APP_PERMISSION* pPerm = *it;
			if (pPerm != NULL)
			{
				if ((pPerm->intModuleID == moduleid) && (pPerm->strRight.compare(permission) == 0))
				{
					ret = true;
				}
			}
			it++;
		}
		return ret;
	}

	bool CUADMLoginManager::verifyParameterName(std::string para1, std::string para2, std::string paraName)
	{
		bool ret = false;
		std::vector<UADM_APP_PARAMETER*>::iterator it = m_vecParameters.begin();
		while ((it != m_vecParameters.end()) && (ret == false))
		{
			UADM_APP_PARAMETER* pPara = *it;
			if (pPara != NULL)
			{
				if ((pPara->strPara1.compare(para1) == 0) && (pPara->strPara2.compare(para2) == 0) && (pPara->strParaName.compare(paraName) == 0))
				{
					ret = true;
				}
			}
			it++;
		}
		return ret;
	}
	
	std::string CUADMLoginManager::getParameterValue(std::string para1, std::string para2, std::string paraName)
	{
		std::string ret = "";
		bool fnd = false;
		std::vector<UADM_APP_PARAMETER*>::iterator it = m_vecParameters.begin();
		while ((it != m_vecParameters.end()) && (fnd == false))
		{
			UADM_APP_PARAMETER* pPara = *it;
			if (pPara != NULL)
			{
				if ((pPara->strPara1.compare(para1) == 0) && (pPara->strPara2.compare(para2) == 0) && (pPara->strParaName.compare(paraName) == 0))
				{
					ret = pPara->strParaValue;
					fnd = true;
				}
			}
			it++;
		}
		return ret;
	}

	std::string CUADMLoginManager::encryptPassword(std::string strPassword, std::string strUsername, int encryptionType)
	{
		if (m_pobjLoginInterface == NULL)
		{
			return "";
		}
		return m_pobjLoginInterface->encryptPassword(strPassword, strUsername, encryptionType);
	}

	bool CUADMLoginManager::getUserMapping(bool allowLoad, int& userid, std::string& username, int& regionbranch, std::string& branchuserid, std::string& branchusername, std::string& branchterminalid, std::string& branchagentid)
	{
		// typical combinations:
		// 0: CustomerDispo load mappings: regionbranch + userid + branchuserid > all mappings for this branch with userid + username and branchuserid + branchusername (userid=0, regionbranch>0, branchuserid<0)
		// 1: CustomerDispo historical dispoitems: regionbranch + branchuserid -> userid + username (userid=0, regionbranch>0, branchuserid>0)
		// 2: CustomerDispo new dispoitems: userid -> username                                      (userid>0, regionbranch=0, username='')
		// 3: OrderEntry: userid>0 + regionbranch>0 -> terminalid or agentid                        (userid>0, regionbranch>0)
		
		int loop = 0;
		bool ret = false;
		bool fnd = false;
		bool map = false;
		if ((userid == 0) && (regionbranch > 0) && (atoi(branchuserid.c_str()) < 0))
		{
			// case 0
			map = true; // load all mappings for this branch
		}

		// loop only twice to get the user mappings if not found in first run
		while ((loop <= 1) && (fnd == false))
		{
			if (map == false)
			{
				std::vector<UADM_USER_MAPPING*>::iterator it = m_vecUserMappings.begin();
				while ((it != m_vecUserMappings.end()) && (fnd == false))
				{
					UADM_USER_MAPPING* pMap = *it;
					if (pMap != NULL)
					{
						// Case 1: check for combination regionbranch + branchuserid as userid is not given
						if ((userid == 0) && (regionbranch > 0) && (atoi(branchuserid.c_str()) > 0))
						{
							if ((userid == 0) && (pMap->intRegionBranchID == regionbranch) && (pMap->strUserParaName.compare("LocalUserID") == 0) && (pMap->strUserParaValue.compare(branchuserid) == 0))
							{
								// mapping found
								userid = pMap->intUserID;
								username = pMap->strUsername;
								ret = true;
								fnd = true;
							}
						}
						// Case 2: check for userid only as branch is not given
						else if ((userid > 0) && (username.empty() == true) && (regionbranch == 0))
						{
							if (pMap->intUserID == userid)
							{
								// mapping found
								username = pMap->strUsername;
								ret = true;
								fnd = true;
							}
						}
						// Case 3: check for userid + regionbranch
						else if ((userid > 0) && (regionbranch > 0))
						{
							if ((pMap->intUserID == userid) && (pMap->intRegionBranchID == regionbranch) && (pMap->strUserParaName.compare("LocalUserID") == 0))
							{
								// mapping found
								username = pMap->strUsername;
								branchuserid = pMap->strUserParaValue;
								allowLoad = false; // no need to load mappings again, but look for all values in existing mappings for this user
							}
							else if ((pMap->intUserID == userid) && (pMap->intRegionBranchID == regionbranch) && (pMap->strUserParaName.compare("LocalUsername") == 0))
							{
								// mapping found
								username = pMap->strUsername;
								branchusername = pMap->strUserParaValue;
								allowLoad = false; // no need to load mappings again, but look for all values in existing mappings for this user
							}
							else if ((pMap->intUserID == userid) && (pMap->intRegionBranchID == regionbranch) && (pMap->strUserParaName.compare("TerminalID") == 0))
							{
								// mapping found
								username = pMap->strUsername;
								branchterminalid = pMap->strUserParaValue;
								allowLoad = false; // no need to load mappings again, but look for all values in existing mappings for this user
							}
							else if ((pMap->intUserID == userid) && (pMap->intRegionBranchID == regionbranch) && (pMap->strUserParaName.compare("AgentID") == 0))
							{
								// mapping found
								username = pMap->strUsername;
								branchagentid = pMap->strUserParaValue;
								allowLoad = false; // no need to load mappings again, but look for all values in existing mappings for this user
							}
						}
					}
					it++;
				}
				// check if in case 3 at least one value was found
				if ((userid > 0) && (regionbranch > 0) && ((branchterminalid.empty() == false) || (branchagentid.empty() == false) || (branchuserid.empty() == false) || (branchusername.empty() == false)))
				{
					fnd = true; // at least one value found, so exit
					ret = true;
				}
			}
			if ((fnd == false) && (loop == 0))
			{
				if (allowLoad == false)
				{
					// do not allow loading user mappings, so exit the loop
					break;
				}
				// open connection first as login was already done before
				if (m_pobjLoginInterface->openConnection() == false)
				{
					// connection to uadm environment failed, so exit the loop
					break;
				}
				// add user mappings with the specific values given
				ret = m_pobjLoginInterface->getUserMappings(&m_vecUserMappings, regionbranch, userid, atoi(branchuserid.c_str()));
				// close connection afterwards
				m_pobjLoginInterface->closeConnection(false);
				if (map == true)
				{
					// in case of loading all mappings for this branch, we are done now
					fnd = true;
				}
			}
			loop++;
		}
		return ret;
	}

	bool CUADMLoginManager::setUserMapping(int userid, int regionbranch, std::string userparaname, std::string& userparavalue)
	{
		if (m_pobjLoginInterface == NULL)
		{
			return false;
		}
		// open connection first as login was already done before
		if (m_pobjLoginInterface->openConnection() == false)
		{
			// connection to uadm environment failed, so return false
			m_strLastErrorMessage = m_pobjLoginInterface->getLastErrorMessage();
			return false;
		}
		// allow to create or change user mapping values
		bool ret = m_pobjLoginInterface->saveUserMapping(userid, regionbranch, userparaname, userparavalue);
		// close connection afterwards
		m_pobjLoginInterface->closeConnection(false);

		return ret;
	}

	bool CUADMLoginManager::getBranchUsers(std::vector<UADM_BRANCH_USER*>* pvecUsers, int regionBranchNo)
	{
		if (m_pobjLoginInterface == NULL)
		{
			return false;
		}
		// open connection first as login was already done before
		if (m_pobjLoginInterface->openConnection() == false)
		{
			// connection to uadm environment failed, so return false
			m_strLastErrorMessage = m_pobjLoginInterface->getLastErrorMessage();
			return false;
		}
		// get all branch user details for the given regionbranch number
		bool ret = m_pobjLoginInterface->getBranchUsers(pvecUsers, regionBranchNo);
		// close connection afterwards
		m_pobjLoginInterface->closeConnection(false);

		return ret;
	}

	bool CUADMLoginManager::verifyPassword(std::string strPassword)
	{
		if (m_pobjLoginInterface == NULL)
		{
			return false;
		}
		return m_pobjLoginInterface->verifyPassword(strPassword);
	}

	int CUADMLoginManager::getLineNumber(std::vector<std::string> lines, std::string chapter, std::string para)
	{
		int ret = 0;

		std::string value = getLineString(lines, chapter, para);
		ret = atoi(value.c_str());

		return ret;
	}

	std::string CUADMLoginManager::getLineString(std::vector<std::string> lines, std::string chapter, std::string para)
	{
		std::string ret = "";

		std::string curChapter = "";
		bool blnFnd = false;
		int cntLine = 0;
		// make chapter and para case insensitive
		chapter = CUADMLoginInterface::toUpper(chapter);
		para = CUADMLoginInterface::toUpper(para);

		para = para + "=";

		while ((cntLine < (int)lines.size()) && (blnFnd == false))
		{
			std::string line = lines.at(cntLine);
			// if line starts with [ set the curChapter
			if (line.empty() == false)
			{
				if (line.front() == '[')
				{
					size_t start = line.find('[');
					size_t end = line.find(']');
					if (start != std::string::npos && end != std::string::npos && end > start + 1)
					{
						curChapter = line.substr(start + 1, end - start - 1);
						curChapter = CUADMLoginInterface::toUpper(curChapter); // make it case insensitive
						curChapter = CUADMLoginInterface::trim(curChapter); // remove leading and trailing spaces
					}
				}
				else
				{
					if (curChapter.compare(chapter) == 0)
					{
						std::string curPara = line.substr(0, para.length());
						curPara = CUADMLoginInterface::toUpper(curPara); // make it case insensitive
						curPara = CUADMLoginInterface::trim(curPara); // remove leading and trailing spaces

						if (curPara.compare(para) == 0)
						{
							ret = CUADMLoginInterface::trim(line.substr(para.length())); // get the value after the '=' and trim it
							blnFnd = true;
						}
					}
				}
			}
			cntLine++;
		}

		return ret;
	}
}

/////////////////////////////////////////////////////////////////////////////