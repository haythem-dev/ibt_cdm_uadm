#if !defined(UADM_LOGINMANAGER_H)
#define UADM_LOGINMANAGER_H

#if _MSC_VER > 1000
#pragma once
#endif

#include <string>
#include <vector>
#include <algorithm>
#include <wtypes.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef LIBAPPUADM_SHARED
#define LIBAPPUADM_API __declspec(dllexport)
#else
#define LIBAPPUADM_API
#endif

namespace UADM
{
	class CUADMLoginInterface;

	struct UADM_APP_LANGUAGE
	{
		std::string strLanguage;
		std::string strLangName;
		std::string strLangFile;
	};

	struct UADM_APP_COUNTRY
	{
		std::string strCountry;
		std::string strCountryName;
	};

	struct UADM_APP_REGIONBRANCH
	{
		int intRegionBranchNo;
		std::string strRegionBranchNo;
		std::string strRegionBranchName;
	};

	struct UADM_APP_ENVIRONMENT
	{
		std::string strEnvironment;
		std::string strEnvName;
		std::string strPrimaryServer;
		std::string	strPrimaryDatabase;
		std::string	strSecondaryServer;
		std::string	strSecondaryDatabase;
		std::string strVoodooPipeName;
		int intVoodooPipePort;
		std::string strVoodooPipePort;
	};

	struct UADM_APP_ENTRY
	{
		int intEntryNo;
		std::string strEntryName;
		std::string strEnvironment;
		std::string	strCountry;
		int intRegionBranchNo;
		std::string strRegionBranchNo;
		std::string strRegionBranchName;
		std::string strServer;
		std::string	strDatabase;
		std::string strServiceName;
		int	intPortNo;
		std::string strPortNo;
	};

	struct UADM_APP_PERMISSION
	{
		int intUserID;
		int intGroupID;
		std::string strRight;
		int intModuleID;
		int intAppID;
		int intRegionBranch;
	};

	struct UADM_APP_PARAMETER
	{
		int intAppID;
		std::string strPara1;
		std::string strPara2;
		std::string strParaName;
		std::string strParaValue;
	};

	struct UADM_USER_MAPPING
	{
		int intUserID;
		std::string strUsername;
		int intRegionBranchID;
		int intUserParaID;
		std::string strUserParaName;
		std::string strUserParaValue;
	};

	struct UADM_BRANCH_USER
	{
		int intRegionBranchNo;
		int intUserID;
		std::string strUsername;
		std::string strFirstname;
		std::string strLastname;
		std::string strEMail;
		std::string strTerminalID;
		std::string strAgentID;
	};

	struct UADM_USER_ROLE
	{
		int intUserID;
		int intRoleID;
		std::string strRoleName;
	};

	class CUADMLoginManager
	{
	public:
		LIBAPPUADM_API CUADMLoginManager();
		LIBAPPUADM_API ~CUADMLoginManager();
		// optional presettings before initialization
		LIBAPPUADM_API void setConnectionTypePreference(int intPref); // 0=informix, 1=voodoo
		LIBAPPUADM_API void setLoginMode(bool blnRegionBranchOnly) { m_blnRegionBranchOnly = blnRegionBranchOnly; };
		LIBAPPUADM_API void setLoginMode(bool blnRegionBranchOnly, bool blnUserMapping) { m_blnRegionBranchOnly = blnRegionBranchOnly; m_blnUserMapping = blnUserMapping; };

		// initialization with ini and registry
		LIBAPPUADM_API bool initialize(std::string strINIFile, std::string strAppKey, std::string strAppName, std::string strAppVersion);
		// initialization with predefined environment
		LIBAPPUADM_API bool initialize(std::string strEnv, std::string strName, std::string strPrimaryServer, std::string strPrimaryDatabase, 
									   std::string strSecondaryServer, std::string strSecondaryDatabase, 
									   std::string strVoodooPipeName, std::string strVoodooPipePort, 
									   std::string strAppName, std::string strAppVersion);
		LIBAPPUADM_API bool loginWithDialog(std::string strBackgroundImageFile, HWND hParent);
		LIBAPPUADM_API bool loginWithoutDialog(std::string strUsername, std::string strCryptPWD, std::string strCountry, int intRegionBranch, std::string strLanguage);
		LIBAPPUADM_API std::string getLastErrorMessage() { return m_strLastErrorMessage; };

		// get all specific UADM data
		LIBAPPUADM_API long getAppID() { return m_lngAppID; };
		LIBAPPUADM_API std::string getAppName() { return m_strAppName; };
		LIBAPPUADM_API std::string getAppDesc() { return m_strAppDesc; };
		LIBAPPUADM_API long getUserID() { return m_lngUserID; };
		LIBAPPUADM_API std::string getUsername() { return m_strUsername; };
		LIBAPPUADM_API std::string getEncryptedPwd() { return m_strEncPwd; };
		LIBAPPUADM_API std::string getFirstname() { return m_strFirstname; };
		LIBAPPUADM_API std::string getLastname() { return m_strLastname; };
		LIBAPPUADM_API std::string getMail() { return m_strMail; };
		LIBAPPUADM_API std::string getEMail() { return m_strEMail; };
		LIBAPPUADM_API std::string getDomainAccount() { return m_strDomainAccount; };
		LIBAPPUADM_API std::string getDescription() { return m_strDescription; };
		LIBAPPUADM_API std::string getOSAccount() { return m_strOSAccount; };
		LIBAPPUADM_API bool isUserActive() { return m_blnIsActive; };
		LIBAPPUADM_API bool isPasswordChangeRequired() { return m_blnForcePasswordChange; }
		LIBAPPUADM_API long getLastPasswordChangeDate() { return m_lngLastPasswordChangeDate; }
		LIBAPPUADM_API long getLastLoginDate() { return m_lngLastLoginDate; };
		LIBAPPUADM_API std::string getServername() { return m_strServername; };
		LIBAPPUADM_API std::string getDatabase() { return m_strDatabase; };
		LIBAPPUADM_API std::string getServiceName() { return m_strServiceName; };
		LIBAPPUADM_API int getPortNoVal() { return m_intPortNo; };
		LIBAPPUADM_API std::string getPortNo() { return m_strPortNo; };
		LIBAPPUADM_API std::string getCountry() { return m_strCountry; };
		LIBAPPUADM_API int getRegionBranchNoVal() { return m_intRegionBranchNo; };
		LIBAPPUADM_API std::string getRegionBranchNo() { return m_strRegionBranchNo; };
		LIBAPPUADM_API std::string getRegionBranchName() { return m_strRegionBranchName; };
		LIBAPPUADM_API std::string getEnvironment() { return m_strEnvironment; };
		LIBAPPUADM_API std::string getLanguage() { return m_strLanguage; };
		LIBAPPUADM_API std::string getLanguageName() { return m_strLanguageName; };

		LIBAPPUADM_API void setAppName(std::string value) { m_strAppName = value; };

		LIBAPPUADM_API std::vector<UADM_APP_LANGUAGE*> getVecLanguages() { return  m_vecLanguages; };
		LIBAPPUADM_API std::vector<UADM_APP_ENTRY*> getVecEntries() { return m_vecEntries; };
		LIBAPPUADM_API std::vector<UADM_APP_ENVIRONMENT*> getVecEnvironments() { return m_vecEnvironments; }
		LIBAPPUADM_API CUADMLoginInterface* getLoginInterface() { return  m_pobjLoginInterface; };

		LIBAPPUADM_API bool verifyPermission(int moduleid, std::string permission);
		LIBAPPUADM_API bool verifyParameterName(std::string para1, std::string para2, std::string paraName);
		LIBAPPUADM_API std::string getParameterValue(std::string para1, std::string para2, std::string paraName);
		LIBAPPUADM_API bool getUserMapping(bool allowLoad, int& userid, std::string& username, int& regionbranch, std::string& branchuserid, std::string& branchusername, std::string& branchterminalid, std::string& branchagentid);
		LIBAPPUADM_API bool setUserMapping(int userid, int regionbranch, std::string userparaname, std::string& userparavalue);
		LIBAPPUADM_API bool getBranchUsers(std::vector<UADM_BRANCH_USER*>* pvecUsers, int regionBranchNo);
		LIBAPPUADM_API bool verifyPassword(std::string strPassword);

		// make sure, that the real vectors are not modified
		LIBAPPUADM_API std::vector<UADM_APP_COUNTRY*>* getCountries();
		LIBAPPUADM_API std::vector<UADM_APP_REGIONBRANCH*>* getRegionBranches(bool countryOnly, bool branchOnly);
		LIBAPPUADM_API std::vector<UADM_APP_PERMISSION*>* getPermissions() { return &m_vecPermissions; };
		LIBAPPUADM_API std::vector<UADM_APP_PARAMETER*>* getParameters() { return &m_vecParameters; };
		LIBAPPUADM_API std::vector<UADM_USER_MAPPING*>* getUserMappings() { return &m_vecUserMappings; };
		LIBAPPUADM_API std::vector<UADM_APP_LANGUAGE*>* getLanguages() { return &m_vecLanguages; };
		LIBAPPUADM_API std::vector<UADM_USER_ROLE*>* getUserRoles() { return &m_vecUserRoles; };
		LIBAPPUADM_API std::vector<std::string>* getDebugInfos() { return &m_vecDebugInfos; };

		// get internals from connected UADM server
		LIBAPPUADM_API std::string getUsedUADMConnectionType();
		LIBAPPUADM_API std::string getUsedUADMServername();
		LIBAPPUADM_API std::string getUsedUADMDatabase();
		LIBAPPUADM_API std::string getUsedUADMPort();
		LIBAPPUADM_API std::string getUsedEncryptionType();
		LIBAPPUADM_API std::string encryptPassword(std::string strPassword, std::string strUsername, int encryptionType);

	private:

		void        reset();
		bool        closeConnection();
		int         getLineNumber(std::vector<std::string> lines, std::string chapter, std::string para);
		std::string getLineString(std::vector<std::string> lines, std::string chapter, std::string para);

		// handler for resource, important to show dialog from DLL
		static void UADMStaticAddress() {};

		// app data
		long m_lngAppID;
		std::string m_strAppKey;
		std::string m_strAppName;
		std::string m_strAppDesc;
		std::string m_strAppVersion;
		// user data
		long m_lngUserID;
		std::string m_strUsername;
		std::string m_strEncPwd;
		std::string m_strFirstname;
		std::string m_strLastname;
		std::string m_strMail;
		std::string m_strEMail;
		std::string m_strDomainAccount;
		std::string m_strDescription;
		std::string m_strOSAccount;
		bool m_blnIsActive;
		bool m_blnForcePasswordChange;
		long m_lngLastPasswordChangeDate;
		long m_lngLastLoginDate;
		// parameter data
		std::string m_strEnvironment;
		std::string m_strServername;
		std::string m_strDatabase;
		std::string m_strServiceName;
		int m_intPortNo;
		std::string m_strPortNo;
		std::string m_strCountry;
		bool m_blnRegionBranchOnly;
		bool m_blnUserMapping;
		int m_intRegionBranchNo;
		std::string m_strRegionBranchNo;
		std::string m_strRegionBranchName;
		std::string m_strLanguage;
		std::string m_strLanguageName;
		// configration items
		std::string m_strLastErrorMessage;
		CUADMLoginInterface* m_pobjLoginInterface;
		std::vector<UADM_APP_LANGUAGE*> m_vecLanguages;
		std::vector<UADM_APP_ENVIRONMENT*> m_vecEnvironments;
		std::vector<UADM_APP_ENTRY*> m_vecEntries;
		std::vector<UADM_APP_COUNTRY*> m_vecCountries;
		std::vector<UADM_APP_REGIONBRANCH*> m_vecRegionBranches;
		std::vector<UADM_APP_PERMISSION*> m_vecPermissions;
		std::vector<UADM_APP_PARAMETER*> m_vecParameters;
		std::vector<std::string> m_vecDebugInfos;
		std::vector<UADM_USER_MAPPING*> m_vecUserMappings;
		std::vector<UADM_USER_ROLE*> m_vecUserRoles;
	};
}
#ifdef __cplusplus
};
#endif /* __cplusplus */

#endif // UADM_LOGINMANAGER_H