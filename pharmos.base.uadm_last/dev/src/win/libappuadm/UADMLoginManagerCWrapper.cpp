// CUADMLoginManager.cpp: implementation file
//

#include "pch.h"
#include "framework.h"
#include "UADMLoginManager.h"
#include "UADMLoginInterface.h"
#include "UADMLoginDlg.h"
#include "UADMLoginManagerCWrapper.h"
#include <unordered_set>

/////////////////////////////////////////////////////////////////////////////
// UADM Login Manager 
namespace UADM
{
	void escapeQuotes(std::string& str)
	{
		size_t pos = 0;
		while ((pos = str.find("\"", pos)) != std::string::npos)
		{
			str.replace(pos, 1, "\\\"");
			pos += 2; 
		}
	}

	void escapeBackslashes(std::string& str)
	{
		size_t pos = 0;
		while ((pos = str.find("\\", pos)) != std::string::npos)
		{
			str.replace(pos, 1, "\\\\");
			pos += 2;
		}
	}

	void filterEscapeSequence(std::string& str)
	{
		std::vector<std::string> escapeSequencesIn =
		{
			"\?",   // Question mark
			"\a",   // Alert (bell)
			"\b",   // Backspace
			"\f",   // Form feed
			"\n",   // Newline
			"\r",   // Carriage return
			"\t",   // Horizontal tab
			"\v",   // Vertical tab
			"\\x",   // Vertical tab
		};

		for (size_t i = 0; i < escapeSequencesIn.size(); ++i)
		{
			size_t pos = 0;
			while ((pos = str.find(escapeSequencesIn[i], pos)) != std::string::npos)
			{
				str.replace(pos, escapeSequencesIn[i].length(), ""); // Replace target with an empty string
			}
		}
	}

	void replaceAllEscapeSequences(std::string& str)
	{
		std::vector<std::string> escapeSequencesIn =
		{
			"\a",   // Alert (bell)
			"\b",   // Backspace
			"\f",   // Form feed
			"\n",   // Newline
			"\r",   // Carriage return
			"\t",   // Horizontal tab
			"\v",   // Vertical tab

			"\?",   // Question mark
			"\'",  // Single quote
			"\"",  // Double quote
			"\\",  // Double quote
		};

		for (size_t i = 0; i < escapeSequencesIn.size(); ++i) 
		{
			size_t pos = 0;
			while ((pos = str.find(escapeSequencesIn[i], pos)) != std::string::npos) 
			{
				str.replace(pos, escapeSequencesIn[i].length(), "\\" + escapeSequencesIn[i]);
				pos += escapeSequencesIn[i].length() + 1; // Move past the replacement
			}
		}
	}

	const char* getDynamicString(std::string& str)
	{
		const char* cstr = str.c_str();

		size_t length = strlen(cstr) + 1; // +1 for '\0'

		char* dynamicStr = (char*)malloc(length);

		if (dynamicStr)
		{
			strcpy(dynamicStr, cstr);
		}

		return dynamicStr;
	}

	void createErrorWithCodeJsonResponse(int returnCode, std::string& str, const std::string message)
	{
		str = "{";
		str += "\"returnCode\":" + std::to_string(returnCode) + ",";
		str += "\"errorMessage\":\"" + message + "\"";
		str += "}";
	}

	void createErrorJsonResponse(std::string& str, const std::string message, int errorCode = UADM_JSON_RESPONSE_ERROR)
	{
		str = "{";
		str += "\"returnCode\":" + std::to_string(errorCode) + ",";
		str += "\"errorMessage\":\"" + message + "\"";
		str += "}";
	}

	size_t startDataJsonArrayResponse(std::string& str)
	{
		str = "{";
		str += "\"returnCode\":0,";
		str += "\"errorMessage\":\"OK\",";
		str += "\"data\":[";

		// Start of data array
		return str.length() + 1;
	}

	void endDataJsonArrayResponse(std::string& str)
	{
		str += "]";
		str += "}";
	}

	size_t startDataJsonBoolResponse(std::string& str)
	{
		str = "{";
		str += "\"returnCode\":0,";
		str += "\"errorMessage\":\"OK\",";
		str += "\"data\":";

		// Start of data array
		return str.length() + 1;
	}

	void endDataJsonBoolResponse(std::string& str)
	{
		str += "}";
	}

	size_t startDataJsonObjectResponse(std::string& str)
	{
		str = "{";
		str += "\"returnCode\":0,";
		str += "\"errorMessage\":\"OK\",";
		str += "\"data\":{";

		// Start of data array
		return str.length() + 1;
	}

	void endDataJsonObjectResponse(std::string& str)
	{
		str += "}";
		str += "}";
	}

	//----------------------------------------------------------------------------
	// C-style interface for other programming languages
	//----------------------------------------------------------------------------

	LIBAPPUADM_API CUADMLoginManager* CreateCUADMLoginManager()
	{
		return new CUADMLoginManager();
	}

	LIBAPPUADM_API void DestroyCUADMLoginManager(CUADMLoginManager* mgr)
	{
		delete mgr;
	}

	LIBAPPUADM_API const char* CUADMLoginManager_Initialize(UADM::CUADMLoginManager* mgr, const char* strEnv, const char* strName, const char* strPrimaryServer, const char* strPrimaryDatabase, const char* strSecondaryServer, const char* strSecondaryDatabase, const char* strVoodooPipeName, const char* strVoodooPipePort, const char* strAppName, const char* strAppVersion, bool useVoodoo) 
	{
		std::string jsonTemplate = "";

		try
		{
			if (!mgr)
			{
				createErrorJsonResponse(jsonTemplate, "Manager is null", UADM_JSON_RESPONSE_MANAGER_IS_NULL);
				return getDynamicString(jsonTemplate);
			}

			UADM::CUADMLoginInterface* loginInterface = mgr->getLoginInterface();
			if (!loginInterface)
			{
				createErrorJsonResponse(jsonTemplate, "Login interface is null", UADM_JSON_RESPONSE_LOGININTERFACE_IS_NULL);
				return getDynamicString(jsonTemplate);
			}

			// Set connection type preference
			loginInterface->setConnectionTypePreference(useVoodoo);

			if (mgr->initialize(strEnv, strName, strPrimaryServer, strPrimaryDatabase, strSecondaryServer, strSecondaryDatabase, strVoodooPipeName, strVoodooPipePort, strAppName, "") == false)
			{
				createErrorJsonResponse(jsonTemplate, "Could not initialize", UADM_JSON_RESPONSE_COULD_NOT_INIITIALIZE);
				return getDynamicString(jsonTemplate);
			}

			// Fetch environments
			std::vector<UADM_APP_ENVIRONMENT*> environments = mgr->getVecEnvironments();
			if (environments.empty())
			{
				createErrorJsonResponse(jsonTemplate, "No environments found", UADM_JSON_RESPONSE_NO_ENVIRONMENT_FOUND);
				return getDynamicString(jsonTemplate);
			}

			size_t size = startDataJsonArrayResponse(jsonTemplate);

			for (size_t environmentIndex = 0; environmentIndex < environments.size(); ++environmentIndex)
			{
				const UADM_APP_ENVIRONMENT* currentEnvironment = environments[environmentIndex];
				if (!currentEnvironment)
					continue;

				if (jsonTemplate.size() > size)
					jsonTemplate += ",";

				bool isPipePortNumeric = !currentEnvironment->strVoodooPipePort.empty() && std::all_of(currentEnvironment->strVoodooPipePort.begin(), currentEnvironment->strVoodooPipePort.end(), ::isdigit);

				jsonTemplate += "{";

				jsonTemplate += "\"IsDefault\":" + std::string(currentEnvironment->strEnvironment == "DEV" ? "true" : "false") + ",";
				jsonTemplate += "\"Application\":\"" + std::string(strAppName) + "\",";
				jsonTemplate += "\"Environment\":\"" + currentEnvironment->strEnvironment + "\",";
				jsonTemplate += "\"EnvironmentName\":\"" + currentEnvironment->strEnvName + "\",";
				jsonTemplate += "\"PrimaryServer\":\"" + currentEnvironment->strPrimaryServer + "\",";
				jsonTemplate += "\"PrimaryDatabase\":\"" + currentEnvironment->strPrimaryDatabase + "\",";
				jsonTemplate += "\"SecondaryServer\":\"" + currentEnvironment->strSecondaryServer + "\",";
				jsonTemplate += "\"SecondaryDatabase\":\"" + currentEnvironment->strSecondaryDatabase + "\",";
				jsonTemplate += "\"VoodooPipeName\":\"" + currentEnvironment->strVoodooPipeName + "\",";
				if (isPipePortNumeric)
					jsonTemplate += "\"VoodooPipePort\":" + currentEnvironment->strVoodooPipePort;
				else
					jsonTemplate += "\"VoodooPipePort\":\"" + currentEnvironment->strVoodooPipePort + "\"";

				jsonTemplate += "}";
			}

			endDataJsonArrayResponse(jsonTemplate);

			return getDynamicString(jsonTemplate);
		}
		catch (const std::exception& ex)
		{
			createErrorJsonResponse(jsonTemplate, ex.what());
			return getDynamicString(jsonTemplate);
		}
	}

	LIBAPPUADM_API const char* CUADMLoginManager_InitializeCompact(UADM::CUADMLoginManager* mgr, const char* strEnv, const char* strName, const char* strAppName, bool useVoodoo)
	{
		return CUADMLoginManager_Initialize(mgr, strEnv, strName, "", "", "", "", "", "", strAppName, "", useVoodoo);
	}

	LIBAPPUADM_API bool CUADMLoginManager_InitializeApp(UADM::CUADMLoginManager* mgr, const char* strAppName, bool useVoodo)
	{
		if (!mgr) 
			return false;

		mgr->setAppName(strAppName);

		return true;	
	}

	LIBAPPUADM_API const char* CUADMLoginManager_VerifyUser(UADM::CUADMLoginManager* mgr, const char* username, const char* password, const bool isPasswordEncrypted)
	{
		/*
		Pseudocode:
			1. Validate mgr not null; if null return nullptr.
			2. Validate username/password (treat null as empty string for safety).
			3. Get login interface via mgr->getLoginInterface(); if null return nullptr.
			4. Call verifyUser(username, password) on interface.
			5. If verification succeeds:
				 - Fetch user data (user id, username, active flag, force password change, last password change date, last login date).
				 - Build JSON with IsAuthenticated:true and the collected fields.
			   Else:
				 - Build JSON with IsAuthenticated:false and empty / default fields.
			6. Allocate dynamic C string via getDynamicString and return.
			7. Caller must release via CUADMLoginManager_ReleaseJsonString.
		*/

		std::string jsonTemplate = "";

		if (!mgr)
		{
			createErrorJsonResponse(jsonTemplate, "Manager is null", UADM_JSON_RESPONSE_MANAGER_IS_NULL);
			return getDynamicString(jsonTemplate);
		}

		UADM::CUADMLoginInterface* loginInterface = mgr->getLoginInterface();
		if (!loginInterface)
		{
			createErrorJsonResponse(jsonTemplate, "Login interface is null", UADM_JSON_RESPONSE_LOGININTERFACE_IS_NULL);
			return getDynamicString(jsonTemplate);
		}

		const char* safeUser = username ? username : "";
		const char* safePwd  = password ? password : "";

		bool authenticated = loginInterface->verifyUser(safeUser, safePwd, isPasswordEncrypted);

		// Collect data only if authenticated
		long userId = authenticated ? loginInterface->getUserID() : 0;
		std::string resolvedUsername = authenticated ? loginInterface->getUsername() : safeUser;
		std::string email = authenticated ? loginInterface->getEMail() : "";
		std::string firstName = loginInterface->getFirstname();
		std::string lastName = loginInterface->getLastname();
		std::string domainAccount = authenticated ? loginInterface->getDomainAccount() : "";
		std::string description = authenticated ? loginInterface->getDescription() : "";
		std::string osAccount = authenticated ? loginInterface->getOSAccount() : "";

		filterEscapeSequence(description);
		escapeBackslashes(description);
		escapeQuotes(description);

		escapeBackslashes(domainAccount);

		bool isActive = authenticated ? loginInterface->isUserActive() : false;
		bool forcePwdChange = authenticated ? loginInterface->isPasswordChangeRequired() : false;

		long lastPwdChange = authenticated ? loginInterface->getLastPasswordChangeDate() : 0;
		long lastLoginDate = authenticated ? loginInterface->getLastLoginDate() : 0;

		// Build JSON (simple concatenation; assumes no special chars needing escaping in strings)
		size_t size = startDataJsonObjectResponse(jsonTemplate);

		jsonTemplate += "\"UserId\":" + std::to_string(userId) + ",";
		jsonTemplate += "\"Username\":\"" + resolvedUsername + "\",";
		jsonTemplate += "\"Email\":\"" + email + "\",";
		jsonTemplate += "\"FirstName\":\"" + firstName + "\",";
		jsonTemplate += "\"LastName\":\"" + lastName + "\",";
		jsonTemplate += "\"DomainAccount\":\"" + domainAccount + "\",";
		jsonTemplate += "\"Description\":\"" + description + "\",";
		jsonTemplate += "\"OSAccount\":\"" + osAccount + "\",";
		jsonTemplate += "\"IsActive\":" + std::string(isActive ? "true" : "false") + ",";
		jsonTemplate += "\"IsPasswordChangeRequired\":" + std::string(forcePwdChange ? "true" : "false") + ",";
		jsonTemplate += "\"LastPasswordChangeDate\":" + std::to_string(lastPwdChange) + ",";
		jsonTemplate += "\"LastLoginDate\":" + std::to_string(lastLoginDate) + ",";
		jsonTemplate += "\"IsAuthenticated\":" + std::string(authenticated ? "true" : "false");

		endDataJsonObjectResponse(jsonTemplate);

		return UADM::getDynamicString(jsonTemplate);
	}

	LIBAPPUADM_API const char* CUADMLoginManager_ChangeUserPassword(UADM::CUADMLoginManager* mgr, const char* username, const char* oldPassword, const char* newPassword, const bool isPasswordEncrypted)
	{
		std::string jsonTemplate = "";

		if (!mgr)
		{
			createErrorJsonResponse(jsonTemplate, "Manager is null", UADM_JSON_RESPONSE_MANAGER_IS_NULL);
			return getDynamicString(jsonTemplate);
		}

		UADM::CUADMLoginInterface* loginInterface = mgr->getLoginInterface();
		if (!loginInterface)
		{
			createErrorJsonResponse(jsonTemplate, "Login interface is null", UADM_JSON_RESPONSE_LOGININTERFACE_IS_NULL);
			return getDynamicString(jsonTemplate);
		}

		const char* safeUser = username ? username : "";
		const char* safePwd = oldPassword ? oldPassword : "";

		bool authenticated = loginInterface->verifyUser(safeUser, safePwd, isPasswordEncrypted);

		// check if user could be verified	
		if (!authenticated)
		{
			createErrorJsonResponse(jsonTemplate, "Could not verify user " + std::string(safeUser), UADM_JSON_RESPONSE_COULD_NOT_VERIFY_USER);
			return getDynamicString(jsonTemplate);
		}

		// check if user is active
		if (loginInterface->isUserActive() == false)
		{
			createErrorJsonResponse(jsonTemplate, "User is not active", UADM_JSON_RESPONSE_USER_IS_NOT_ACTIVATED);
			return getDynamicString(jsonTemplate);
		}

		// now login was successful, so change the password	
		safePwd = newPassword ? newPassword : "";

		if (loginInterface->verifyNewPassword(safePwd) == false)
		{
			createErrorJsonResponse(jsonTemplate, loginInterface->getLastErrorMessage(), UADM_JSON_RESPONSE_PASSWORD_NOT_VALID);
			return getDynamicString(jsonTemplate);
		}

		// verify if the last password change was too recent
		if (loginInterface->verifyLastPasswordChange() == false)
		{
			createErrorJsonResponse(jsonTemplate, "Last password change was too recent", UADM_JSON_RESPONSE_LAST_PASSWORD_VERIFY_FAILED);
			return getDynamicString(jsonTemplate);
		}

		bool blnPWUsed = false;
		if (loginInterface->verifyAgainstHistoricPasswords(safePwd, blnPWUsed) == true)
		{
			if (blnPWUsed == true)
			{
				createErrorJsonResponse(jsonTemplate, "New password matches historic password", UADM_JSON_RESPONSE_HISTORY_VERIFY_FAILED);
				return getDynamicString(jsonTemplate);
			}
		}

		// login successful, save the new password
		if (loginInterface->saveNewPassword(safePwd) == false)
		{
			createErrorJsonResponse(jsonTemplate, "Could not save new password", UADM_JSON_RESPONSE_SAVE_PASSWORD_FAILED);
			return getDynamicString(jsonTemplate);
		}

		authenticated = loginInterface->verifyUser(safeUser, safePwd, isPasswordEncrypted);

		// Collect data only if authenticated
		long userId = authenticated ? loginInterface->getUserID() : 0;
		std::string resolvedUsername = authenticated ? loginInterface->getUsername() : safeUser;
		std::string email = authenticated ? loginInterface->getEMail() : "";
		std::string firstName = loginInterface->getFirstname();
		std::string lastName = loginInterface->getLastname();
		std::string domainAccount = authenticated ? loginInterface->getDomainAccount() : "";
		std::string description = authenticated ? loginInterface->getDescription() : "";
		std::string osAccount = authenticated ? loginInterface->getOSAccount() : "";

		filterEscapeSequence(description);
		escapeBackslashes(description);
		escapeQuotes(description);

		escapeBackslashes(domainAccount);

		bool isActive = authenticated ? loginInterface->isUserActive() : false;
		bool forcePwdChange = authenticated ? loginInterface->isPasswordChangeRequired() : false;

		long lastPwdChange = authenticated ? loginInterface->getLastPasswordChangeDate() : 0;
		long lastLoginDate = authenticated ? loginInterface->getLastLoginDate() : 0;

		// Build JSON (simple concatenation; assumes no special chars needing escaping in strings)
		size_t size = startDataJsonObjectResponse(jsonTemplate);

		jsonTemplate += "\"UserId\":" + std::to_string(userId) + ",";
		jsonTemplate += "\"Username\":\"" + resolvedUsername + "\",";
		jsonTemplate += "\"Email\":\"" + email + "\",";
		jsonTemplate += "\"FirstName\":\"" + firstName + "\",";
		jsonTemplate += "\"LastName\":\"" + lastName + "\",";
		jsonTemplate += "\"DomainAccount\":\"" + domainAccount + "\",";
		jsonTemplate += "\"Description\":\"" + description + "\",";
		jsonTemplate += "\"OSAccount\":\"" + osAccount + "\",";
		jsonTemplate += "\"IsActive\":" + std::string(isActive ? "true" : "false") + ",";
		jsonTemplate += "\"IsPasswordChangeRequired\":" + std::string(forcePwdChange ? "true" : "false") + ",";
		jsonTemplate += "\"LastPasswordChangeDate\":" + std::to_string(lastPwdChange) + ",";
		jsonTemplate += "\"LastLoginDate\":" + std::to_string(lastLoginDate) + ",";
		jsonTemplate += "\"IsAuthenticated\":" + std::string(authenticated ? "true" : "false");

		endDataJsonObjectResponse(jsonTemplate);

		return UADM::getDynamicString(jsonTemplate);
	}

	LIBAPPUADM_API const char* CUADMLoginManager_GetPreviousLoginData(UADM::CUADMLoginManager* mgr, const char* osAccount)
	{
		std::string jsonTemplate = "";

		if (!mgr)
		{
			createErrorJsonResponse(jsonTemplate, "Manager is null", UADM_JSON_RESPONSE_MANAGER_IS_NULL);
			return getDynamicString(jsonTemplate);
		}

		UADM::CUADMLoginInterface* loginInterface = mgr->getLoginInterface();
		if (!loginInterface)
		{
			createErrorJsonResponse(jsonTemplate, "Login interface is null", UADM_JSON_RESPONSE_LOGININTERFACE_IS_NULL);
			return getDynamicString(jsonTemplate);
		}

		loginInterface->setOSAccount(osAccount ? osAccount : "");

		std::string prevUsername = "";
		std::string prevRegionBranch = "";
		std::string prevLanguage = "";
		std::string prevCountry = "";
		bool authenticated = loginInterface->getPreviousLoginData(prevUsername, prevCountry, prevRegionBranch, prevLanguage);

		bool isActive = authenticated ? loginInterface->isUserActive() : false;
		bool forcePwdChange = authenticated ? loginInterface->isPasswordChangeRequired() : false;

		long lastPwdChange = authenticated ? loginInterface->getLastPasswordChangeDate() : 0;
		long lastLoginDate = authenticated ? loginInterface->getLastLoginDate() : 0;

		// Build JSON (simple concatenation; assumes no special chars needing escaping in strings)
		size_t size = startDataJsonObjectResponse(jsonTemplate);

		jsonTemplate += "\"OsAccount\":\"" + std::string(osAccount ? osAccount : "") + "\",";
		jsonTemplate += "\"Username\":\"" + prevUsername + "\",";
		jsonTemplate += "\"BranchRegion\":\"" + prevRegionBranch + "\",";
		jsonTemplate += "\"Language\":\"" + prevLanguage + "\",";
		jsonTemplate += "\"Country\":\"" + prevCountry + "\",";
		jsonTemplate += "\"IsAuthenticated\":" + std::string(authenticated ? "true" : "false");

		endDataJsonObjectResponse(jsonTemplate);

		return UADM::getDynamicString(jsonTemplate);
	}

	LIBAPPUADM_API const char* CUADMLoginManager_SetPreviousLoginData(UADM::CUADMLoginManager* mgr, const char* osAccount, long lngUserID, int branchRegion, const char* country, const char* language)
	{
		std::string jsonTemplate = "";

		if (!mgr)
		{
			createErrorJsonResponse(jsonTemplate, "Manager is null", UADM_JSON_RESPONSE_MANAGER_IS_NULL);
			return getDynamicString(jsonTemplate);
		}

		UADM::CUADMLoginInterface* loginInterface = mgr->getLoginInterface();
		if (!loginInterface)
		{
			createErrorJsonResponse(jsonTemplate, "Login interface is null", UADM_JSON_RESPONSE_LOGININTERFACE_IS_NULL);
			return getDynamicString(jsonTemplate);
		}

		if (loginInterface->saveLastLoginData(loginInterface->getAppID(), osAccount, lngUserID, branchRegion, country, language) == false)
		{
			createErrorJsonResponse(jsonTemplate, loginInterface->getLastErrorMessage());
			return getDynamicString(jsonTemplate);
		}

		// Build JSON (simple concatenation; assumes no special chars needing escaping in strings)
		size_t size = startDataJsonBoolResponse(jsonTemplate);

		jsonTemplate += "true";

		endDataJsonBoolResponse(jsonTemplate);

		return UADM::getDynamicString(jsonTemplate);
	}

	LIBAPPUADM_API const char* CUADMLoginManager_UpdateLastLoginDate(UADM::CUADMLoginManager* mgr, const long lngUserID, const char* username)
	{
		std::string jsonTemplate = "";

		if (!mgr)
		{
			createErrorJsonResponse(jsonTemplate, "Manager is null", UADM_JSON_RESPONSE_MANAGER_IS_NULL);
			return getDynamicString(jsonTemplate);
		}

		UADM::CUADMLoginInterface* loginInterface = mgr->getLoginInterface();
		if (!loginInterface)
		{
			createErrorJsonResponse(jsonTemplate, "Login interface is null", UADM_JSON_RESPONSE_LOGININTERFACE_IS_NULL);
			return getDynamicString(jsonTemplate);
		}

		const char* safeUser = username ? username : "";

		loginInterface->setUserID(lngUserID);
		loginInterface->setUsername(safeUser);

		size_t size = startDataJsonBoolResponse(jsonTemplate);

		std::string returnValue = "false";
		if (loginInterface->updateLastLoginDate() == true)
			returnValue = "true";

		jsonTemplate += returnValue;

		endDataJsonBoolResponse(jsonTemplate);

		return UADM::getDynamicString(jsonTemplate);
	}

	LIBAPPUADM_API const char* CUADMLoginManager_GetAppCountries(UADM::CUADMLoginManager* mgr)
	{
		std::string jsonTemplate = "";

		if (!mgr)
		{
			createErrorJsonResponse(jsonTemplate, "Manager is null", UADM_JSON_RESPONSE_MANAGER_IS_NULL);
			return getDynamicString(jsonTemplate);
		}

		std::unordered_set<std::string> processedValues;

		size_t size = startDataJsonArrayResponse(jsonTemplate);

		for (UADM_APP_ENTRY* lang : mgr->getVecEntries())
		{
			if (processedValues.find(lang->strCountry) != processedValues.end())
				continue; // already processed

			// Mark the entry as processed
			processedValues.insert(lang->strCountry);

			if (jsonTemplate.length() > size)
				jsonTemplate += ",";

			jsonTemplate += "{";
			jsonTemplate += "\"Key\":\"" + lang->strCountry + "\",";
			jsonTemplate += "\"Name\":\"" + lang->strCountry + "\"";
			jsonTemplate += "}";
		}

		endDataJsonArrayResponse(jsonTemplate);

		// Return the dynamically allocated JSON string
		return getDynamicString(jsonTemplate);
	}

	LIBAPPUADM_API const char* CUADMLoginManager_GetAppItems(UADM::CUADMLoginManager* mgr)
	{
		std::string jsonTemplate = "";

		if (!mgr)
		{
			createErrorJsonResponse(jsonTemplate, "Manager is null", UADM_JSON_RESPONSE_MANAGER_IS_NULL);
			return getDynamicString(jsonTemplate);
		}

		// Try to load (or refresh) entries via interface
		std::vector<UADM_APP_ENTRY*> entries;

		// Fallback to manager's stored entries
		for (UADM_APP_ENTRY* e : mgr->getVecEntries())
		{
			if (e) entries.push_back(e);
		}

		size_t size = startDataJsonArrayResponse(jsonTemplate);

		for (size_t i = 0; i < entries.size(); ++i)
		{
			UADM_APP_ENTRY* item = entries[i];
			if (!item) 
				continue;

			if (jsonTemplate.length() > size)
				jsonTemplate += ",";

			jsonTemplate += "{";
			jsonTemplate += "  \"intEntryNo\":" + std::to_string(item->intEntryNo) + ",";
			jsonTemplate += "  \"intKey\":" + std::to_string(item->intRegionBranchNo) + ",";
			jsonTemplate += "  \"strEntryName\":\"" + item->strEntryName + "\",";
			jsonTemplate += "  \"strEnvironment\":\"" + item->strEnvironment + "\",";
			jsonTemplate += "  \"strCountry\":\"" + item->strCountry + "\",";
			jsonTemplate += "  \"intRegionBranchNo\":" + std::to_string(item->intRegionBranchNo) + ",";
			jsonTemplate += "  \"strRegionBranchNo\":\"" + item->strRegionBranchNo + "\",";
			jsonTemplate += "  \"strRegionBranchName\":\"" + item->strRegionBranchName + "\",";
			jsonTemplate += "  \"strServer\":\"" + item->strServer + "\",";
			jsonTemplate += "  \"strDatabase\":\"" + item->strDatabase + "\",";
			jsonTemplate += "  \"intPortNo\":" + std::to_string(item->intPortNo);
			jsonTemplate += "}";
		}

		endDataJsonArrayResponse(jsonTemplate);

		return getDynamicString(jsonTemplate);
	}

	LIBAPPUADM_API const char* CUADMLoginManager_GetAppItemsByCountry(UADM::CUADMLoginManager* mgr, const char* country)
	{
		std::string jsonTemplate = "";

		if (!mgr)
		{
			createErrorJsonResponse(jsonTemplate, "Manager is null", UADM_JSON_RESPONSE_MANAGER_IS_NULL);
			return getDynamicString(jsonTemplate);
		}

		const std::string filterCountry = country ? country : "";

		// Try to load entries via interface (preferred)
		std::vector<UADM_APP_ENTRY*> entries;
		std::vector<UADM_APP_LANGUAGE*> dummyLangs;

		bool loadedFromInterface = false;

		if (CUADMLoginInterface* iface = mgr->getLoginInterface())
		{
			if (iface->getAppEntries(mgr->getAppName(), &dummyLangs, &entries))
			{
				if (!entries.empty())
					loadedFromInterface = true;
			}
		}

		// Fallback to manager stored entries
		if (!loadedFromInterface)
		{
			for (UADM_APP_ENTRY* e : mgr->getVecEntries())
			{
				if (e) 
					entries.push_back(e);
			}
		}

		size_t size = startDataJsonArrayResponse(jsonTemplate);

		// Start of data array
		for (UADM_APP_ENTRY* item : entries)
		{
			if (!item) continue;
			if (!filterCountry.empty() && item->strCountry != filterCountry)
				continue; // skip non-matching countries

			if (jsonTemplate.length() > size)
				jsonTemplate += ",";

			jsonTemplate += "{";
			jsonTemplate += "\"intEntryNo\":" + std::to_string(item->intEntryNo) + ",";
			jsonTemplate += "\"intKey\":" + std::to_string(item->intRegionBranchNo) + ",";
			jsonTemplate += "\"strEntryName\":\"" + item->strEntryName + "\",";
			jsonTemplate += "\"strEnvironment\":\"" + item->strEnvironment + "\",";
			jsonTemplate += "\"strCountry\":\"" + item->strCountry + "\",";
			jsonTemplate += "\"intRegionBranchNo\":" + std::to_string(item->intRegionBranchNo) + ",";
			jsonTemplate += "\"strRegionBranchNo\":\"" + item->strRegionBranchNo + "\",";
			jsonTemplate += "\"strRegionBranchName\":\"" + item->strRegionBranchName + "\",";
			jsonTemplate += "\"strServer\":\"" + item->strServer + "\",";
			jsonTemplate += "\"strDatabase\":\"" + item->strDatabase + "\",";
			jsonTemplate += "\"intPortNo\":" + std::to_string(item->intPortNo);
			jsonTemplate += "}";
		}

		endDataJsonArrayResponse(jsonTemplate);

		return getDynamicString(jsonTemplate);
	}

	LIBAPPUADM_API const char* CUADMLoginManager_GetAppLanguages(UADM::CUADMLoginManager* mgr)
	{
		std::string jsonTemplate = "";

		if (!mgr)
		{
			createErrorJsonResponse(jsonTemplate, "Manager is null", UADM_JSON_RESPONSE_MANAGER_IS_NULL);
			return getDynamicString(jsonTemplate);
		}

		// Try to obtain (or refresh) languages through the interface
		std::vector<UADM_APP_LANGUAGE*> languages;
		std::vector<UADM_APP_ENTRY*> dummyEntries; // not needed here, but interface call requires it

		bool loadedFromInterface = false;

		if (CUADMLoginInterface* iface = mgr->getLoginInterface())
		{
			if (iface->getAppEntries(mgr->getAppName(), &languages, &dummyEntries))
			{
				if (!languages.empty())
					loadedFromInterface = true;
			}
		}

		// Fallback to manager's stored languages if interface did not provide any
		if (!loadedFromInterface)
		{
			for (UADM_APP_LANGUAGE* l : mgr->getVecLanguages())
			{
				if (l) languages.push_back(l);
			}
		}

		startDataJsonArrayResponse(jsonTemplate);

		// Start of data array
		for (UADM_APP_LANGUAGE* lang : languages)
		{
			if (!lang) continue;

			if (jsonTemplate.length() > 1) // maintain comma insertion logic
				jsonTemplate += ",";

			jsonTemplate += "{";
			jsonTemplate += "\"Key\":\"" + lang->strLanguage + "\",";
			jsonTemplate += "\"Name\":\"" + lang->strLangName + "\",";
			jsonTemplate += "\"Filename\":\"" + lang->strLangFile + "\"";
			jsonTemplate += "}";
		}

		endDataJsonArrayResponse(jsonTemplate);

		return getDynamicString(jsonTemplate);
	}

	LIBAPPUADM_API const char* CUADMLoginManager_GetAllUserPermissionsByUserID(UADM::CUADMLoginManager* mgr, const long lngUserID, const int branchRegion, const bool blnRegionBranchOnly)
	{
		std::string jsonTemplate = "";

		if (!mgr)
		{
			createErrorJsonResponse(jsonTemplate, "Manager is null", UADM_JSON_RESPONSE_MANAGER_IS_NULL);
			return getDynamicString(jsonTemplate);
		}

		UADM::CUADMLoginInterface* loginInterface = mgr->getLoginInterface();
		if (!loginInterface)
		{
			createErrorJsonResponse(jsonTemplate, "Login interface is null", UADM_JSON_RESPONSE_LOGININTERFACE_IS_NULL);
			return getDynamicString(jsonTemplate);
		}

		loginInterface->setUserID(lngUserID);

		// handle permissions
		std::vector<UADM_APP_PERMISSION*> vecPermissions;
		if (loginInterface->getApplicationRights(&vecPermissions, NULL, branchRegion, blnRegionBranchOnly) == false)
		{
			createErrorJsonResponse(jsonTemplate, loginInterface->getLastErrorMessage());
			return getDynamicString(jsonTemplate);
		}

		size_t size = startDataJsonArrayResponse (jsonTemplate);

		for (UADM_APP_PERMISSION* perm : vecPermissions)
		{
			if (!perm) continue;

			if (jsonTemplate.length() > size)
				jsonTemplate += ",";

			jsonTemplate += "{";
			jsonTemplate += "  \"UserId\":" + std::to_string(perm->intUserID) + ",";
			jsonTemplate += "  \"GroupId\":" + std::to_string(perm->intGroupID) + ",";
			jsonTemplate += "  \"Right\":\"" + perm->strRight + "\",";
			jsonTemplate += "  \"ModuleId\":" + std::to_string(perm->intModuleID) + ",";
			jsonTemplate += "  \"AppId\":" + std::to_string(perm->intAppID) + ",";
			jsonTemplate += "  \"RegionBranch\":" + std::to_string(perm->intRegionBranch);
			jsonTemplate += "}";
		}
		
		// cleanup permissions vector (delete allocated permissions) to prevent memory leaks (since interface contract requires caller to clean up)
		while (vecPermissions.size() > 0)
		{
			UADM_APP_PERMISSION* perm = vecPermissions.back();
			vecPermissions.pop_back();
			delete perm;
		}

		endDataJsonArrayResponse(jsonTemplate);

		return getDynamicString(jsonTemplate);
	}

	LIBAPPUADM_API const char* CUADMLoginManager_GetAppParameters(UADM::CUADMLoginManager* mgr, const char* para1, const char* para2, const char* parameterName)
	{
		std::string jsonTemplate = "";

		if (!mgr)
		{
			createErrorJsonResponse(jsonTemplate, "Manager is null", UADM_JSON_RESPONSE_MANAGER_IS_NULL);
			return getDynamicString(jsonTemplate);
		}

		UADM::CUADMLoginInterface* loginInterface = mgr->getLoginInterface();
		if (!loginInterface)
		{
			createErrorJsonResponse(jsonTemplate, "Login interface is null", UADM_JSON_RESPONSE_LOGININTERFACE_IS_NULL);
			return getDynamicString(jsonTemplate);
		}

		std::vector<UADM_APP_PARAMETER*> vecParameters;
		if (loginInterface->getApplicationParameters(&vecParameters) == false)
		{
			createErrorJsonResponse(jsonTemplate, loginInterface->getLastErrorMessage());
			return getDynamicString(jsonTemplate);
		}

		size_t size = startDataJsonArrayResponse(jsonTemplate);

		// Build JSON array using the existing field names and order (do not change structure)
		for (UADM_APP_PARAMETER* param : vecParameters)
		{
			if (!param) 
				continue;

			if ((para1 != nullptr) && (param->strPara1 != para1))
				continue; // skip non-matching parameters

			if ((para2 != nullptr) && (param->strPara2 != para2))
				continue; // skip non-matching parameters

			if ((parameterName != nullptr) && (param->strParaName != parameterName))
				continue; // skip non-matching parameters

			if (jsonTemplate.length() > size)
				jsonTemplate += ",";

			jsonTemplate += "{";
			jsonTemplate += "  \"AppId\":" + std::to_string(param->intAppID) + ",";
			jsonTemplate += "  \"Para1\":\"" + param->strPara1 + "\",";
			jsonTemplate += "  \"Para2\":\"" + param->strPara2 + "\",";
			jsonTemplate += "  \"ParaName\":\"" + param->strParaName + "\",";
			jsonTemplate += "  \"ParaValue\":\"" + param->strParaValue + "\",";
			jsonTemplate += "}";
		}

		endDataJsonArrayResponse(jsonTemplate);

		return getDynamicString(jsonTemplate);
	}

	LIBAPPUADM_API const char* CUADMLoginManager_GetAllAppParameters(UADM::CUADMLoginManager* mgr)
	{
		return CUADMLoginManager_GetAppParameters(mgr, nullptr, nullptr, nullptr);
	}

	LIBAPPUADM_API const char* CUADMLoginManager_VerifyUserPermission(UADM::CUADMLoginManager* mgr, const long lngUserID, const int branchRegion, const char* permission)
	{
		std::string jsonTemplate = "";

		if (!mgr)
		{
			createErrorJsonResponse(jsonTemplate, "Manager is null", UADM_JSON_RESPONSE_MANAGER_IS_NULL);
			return getDynamicString(jsonTemplate);
		}

		UADM::CUADMLoginInterface* loginInterface = mgr->getLoginInterface();
		if (!loginInterface)
		{
			createErrorJsonResponse(jsonTemplate, "Login interface is null", UADM_JSON_RESPONSE_LOGININTERFACE_IS_NULL);
			return getDynamicString(jsonTemplate);
		}

		loginInterface->setUserID(lngUserID);

		// handle permissions
		std::vector<UADM_APP_PERMISSION*> vecPermissions;
		if (loginInterface->getApplicationRights(&vecPermissions, NULL, branchRegion, true) == false)
		{		
			createErrorJsonResponse(jsonTemplate, loginInterface->getLastErrorMessage());
			return getDynamicString(jsonTemplate);	
		}

		size_t size = startDataJsonBoolResponse(jsonTemplate);

		std::string returnValue = "false";

		for (UADM_APP_PERMISSION* perm : vecPermissions)
		{
			if (!perm)
				continue;

			if (perm->strRight == permission)
			{
				returnValue = "true";
				break;
			}
		}

		jsonTemplate += returnValue;

		// cleanup permissions vector (delete allocated permissions) to prevent memory leaks (since interface contract requires caller to clean up)
		while (vecPermissions.size() > 0)
		{
			UADM_APP_PERMISSION* perm = vecPermissions.back();
			vecPermissions.pop_back();
			delete perm;
		}

		endDataJsonBoolResponse(jsonTemplate);

		return getDynamicString(jsonTemplate);
	}

	LIBAPPUADM_API const char* CUADMLoginManager_GetUserPermissionInfo(UADM::CUADMLoginManager* mgr, const long lngUserID, const int branchRegion, const char* permission)
	{
		std::string jsonTemplate = "";

		if (!mgr)
		{
			createErrorJsonResponse(jsonTemplate, "Manager is null", UADM_JSON_RESPONSE_MANAGER_IS_NULL);
			return getDynamicString(jsonTemplate);
		}

		UADM::CUADMLoginInterface* loginInterface = mgr->getLoginInterface();
		if (!loginInterface)
		{
			createErrorJsonResponse(jsonTemplate, "Login interface is null", UADM_JSON_RESPONSE_LOGININTERFACE_IS_NULL);
			return getDynamicString(jsonTemplate);
		}

		loginInterface->setUserID(lngUserID);

		size_t size = startDataJsonObjectResponse(jsonTemplate);

		// handle permissions
		std::vector<UADM_APP_PERMISSION*> vecPermissions;
		if (loginInterface->getApplicationRights(&vecPermissions, NULL, branchRegion, true) == false)
		{
			createErrorJsonResponse(jsonTemplate, loginInterface->getLastErrorMessage());
			return getDynamicString(jsonTemplate);
		}

		bool found = false;

		for (UADM_APP_PERMISSION* perm : vecPermissions)
		{
			if (!perm)
				continue;

			if (perm->strRight == permission)
			{
				found = true;

				jsonTemplate += "  \"UserId\":" + std::to_string(perm->intUserID) + ",";
				jsonTemplate += "  \"GroupId\":" + std::to_string(perm->intGroupID) + ",";
				jsonTemplate += "  \"Right\":\"" + perm->strRight + "\",";
				jsonTemplate += "  \"ModuleId\":" + std::to_string(perm->intModuleID) + ",";
				jsonTemplate += "  \"AppId\":" + std::to_string(perm->intAppID) + ",";
				jsonTemplate += "  \"RegionBranch\":" + std::to_string(perm->intRegionBranch);

				break;
			}
		}

		if (found == false)
			jsonTemplate += "null";

		// cleanup permissions vector (delete allocated permissions) to prevent memory leaks (since interface contract requires caller to clean up)
		while (vecPermissions.size() > 0)
		{
			UADM_APP_PERMISSION* perm = vecPermissions.back();
			vecPermissions.pop_back();
			delete perm;
		}

		endDataJsonObjectResponse(jsonTemplate);

		return getDynamicString(jsonTemplate);
	}

	LIBAPPUADM_API const bool CUADMLoginManager_ReleaseJsonString(UADM::CUADMLoginManager* mgr, char* ptr)
	{
		if (!mgr)
			return false;
		if (!ptr)
			return false;

		free(ptr); // Use free for memory allocated with malloc

		return true;
	}
}


/////////////////////////////////////////////////////////////////////////////