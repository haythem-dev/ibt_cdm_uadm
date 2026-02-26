#if !defined(UADM_LOGININTERFACE_H)
#define UADM_LOGININTERFACE_H

#if _MSC_VER > 1000
#pragma once
#endif

#include <string>
#include <vector>
#include "libscsvoodoo.h"
#include "UADMLoginManager.h"

#ifndef EXLUDE_INFORMIX
#include <it.h>
#endif

namespace UADM
{
	enum UADM_CONNECTION_TYPE
	{
		UADM_CONNECTION_TYPE_NONE = 0,
		UADM_CONNECTION_TYPE_VOODOO_PRIMARY,
		UADM_CONNECTION_TYPE_VOODOO_SECONDARY,
		UADM_CONNECTION_TYPE_INFORMIX_PRIMARY,
		UADM_CONNECTION_TYPE_INFORMIX_SECONDARY
	};

	enum UADM_PASSWORD_ENCRYPTION_TYPE
	{
		UADM_PASSWORD_ENCRYPTION_TYPE_NONE = 0,
		UADM_PASSWORD_ENCRYPTION_TYPE_PHOENIX,
		UADM_PASSWORD_ENCRYPTION_TYPE_AES
	};

	class CUADMLoginInterface
	{
	public:
		CUADMLoginInterface(std::vector<std::string>* pvecDebugInfos);
		~CUADMLoginInterface();

		void reset();
		void initialize(UADM_APP_ENVIRONMENT* pobjUADMEnvironment);
		void setConnectionTypePreference(int intPref);
		bool openConnection();
		bool closeConnection(bool blnReset);
		bool isConnected(UADM_APP_ENVIRONMENT* pobjUADMEnvironment);

		// uadm main methods
		bool verifyLocalUser(std::string strAppName, int intBranchNo, std::string strUsername, std::string strPassword);
		bool verifyUser(std::string strUsername, std::string strPassword, bool pwdEncrypted);
		bool verifyAndLockUser(std::string strUsername);
		void setUADMEntry(UADM_APP_ENTRY* pEntry) { m_pobjUADMEntry = pEntry; };
		bool findAndSetUADMEntry(std::vector<UADM_APP_ENTRY*>* pvecEntries, std::string strEnv, std::string strCountry, int intRegionBranch);
		bool findAndSetLanguage(std::vector<UADM_APP_LANGUAGE*>* pvecLanguages, std::string strLanguage);
		bool verifyApplication(std::string strAppName);
		bool getPreviousLoginData(std::string& prevUsername, std::string& prevCountry, std::string& prevRegionBranchNo, std::string& prevLanguage);
		bool getApplicationRights(std::vector<UADM_APP_PERMISSION*>* pvecPermissions, std::vector<UADM_USER_ROLE*>* pvecUserRoles, int intRegionBranch, bool blnRegionBranchOnly);
		bool getApplicationParameters(std::vector<UADM_APP_PARAMETER*>* pvecParameters);
		bool getUserMappings(std::vector<UADM_USER_MAPPING*>* pvecUserMappings, int intRegionBranch, int intUserID, int intLocalUserID);
		bool saveUserMapping(int intUserID, int intRegionBranch, std::string strUserParaName, std::string& strUserParaValue);
		bool verifyNewPassword(const std::string& strNewPassword);
		bool verifyLastPasswordChange();
		bool verifyAgainstHistoricPasswords(std::string strPassword, bool& blnExists);
		int  getPasswordEncryptionType() { return (int)m_enmUADMEncryptionType; };
		int  getLoginMaxRetries() { return m_intLoginMaxRetries; };
		bool updateLastLoginDate(); // saves the current date as last login date (in yyyymmdd format)
		bool saveLastLoginData(long lngAppID, std::string strOSAccount, long lngUserID, int intRegionBranchNo, std::string strCountry, std::string strLanguage);
		bool saveLastLoginData(); // saves the current login data (username, country, regionbranchno, language)
		bool saveNewPassword(std::string strPassword);
		bool getAppEntries(std::string m_strAppName, std::vector<UADM_APP_LANGUAGE*>* pvecLanguages, std::vector<UADM_APP_ENTRY*>* pvecEntries);
		void verifyEnvironment(std::string& strEnv, std::string& strName, std::string& strPrimaryServer, std::string& strPrimaryDatabase, std::string& strSecondaryServer, std::string& strSecondaryDatabase, std::string& strVoodooPipeName, std::string& strVoodooPipePort);
		bool IsLocalLoginAllowed() { return m_blnAllowLocalLogin; };
		bool getBranchUsers(std::vector<UADM_BRANCH_USER*>* pvecBranchUsers, int regionBranchNo);
		bool verifyPassword(std::string strPassword);

		// get user data
		long getUserID() { return m_lngUserID; };
		std::string getUsername() { return m_strUsername; };
		std::string getEncPwd() { return m_strEncPwd; };
		std::string getFirstname() { return m_strFirstname; };
		std::string getLastname() { return m_strLastname; };
		std::string getMail() { return m_strMail; };
		std::string getEMail() { return m_strEMail; };
		std::string getDomainAccount() { return m_strDomainAccount; };
		std::string getDescription() { return m_strDescription; };
		std::string getOSAccount() { return m_strOSAccount; };
		bool isUserActive() { return m_blnIsActive; };
		bool isPasswordChangeRequired();
		long getLastPasswordChangeDate() { return m_lngLastPasswordChangeDate; };
		long getLastLoginDate() { return m_lngLastLoginDate; };
		// get uadm target data
		std::string getServername() { if (m_pobjUADMEntry != NULL) return m_pobjUADMEntry->strServer; return ""; };
		std::string getDatabase() { if (m_pobjUADMEntry != NULL) return m_pobjUADMEntry->strDatabase; return ""; };
		std::string getCountry() { if (m_pobjUADMEntry != NULL) return m_pobjUADMEntry->strCountry; return ""; };
		std::string resolveCountry(int intRegionBranchNo);
		int getRegionBranchNoVal() { if (m_pobjUADMEntry != NULL) return m_pobjUADMEntry->intRegionBranchNo; return 0; };
		std::string getRegionBranchNo() { if (m_pobjUADMEntry != NULL) return m_pobjUADMEntry->strRegionBranchNo; return ""; };
		std::string getRegionBranchName() { if (m_pobjUADMEntry != NULL) return m_pobjUADMEntry->strRegionBranchName; return ""; };
		std::string getEnvironment();
		std::string getServiceName() { if (m_pobjUADMEntry != NULL) return m_pobjUADMEntry->strServiceName; return ""; };
		int getPortNoVal() { if (m_pobjUADMEntry != NULL) return m_pobjUADMEntry->intPortNo; return 0; };
		std::string getPortNo() { if (m_pobjUADMEntry != NULL) return m_pobjUADMEntry->strPortNo; return ""; };
		// get app data
		long getAppID() { return m_lngAppID; };
		std::string getAppName() { return m_strAppName; };
		std::string getAppDesc() { return m_strAppDesc; };
		std::string getLanguage() { return m_strLanguage; };
		std::string getLanguageName() { return m_strLanguageName; };
		// get internal uadm conn data
		std::string getLastErrorMessage() { return m_strLastErrorMessage; };
		void        resetLastErrorMessage() { m_strLastErrorMessage = ""; };
		std::string getLastUADMConnectionType();
		std::string getLastUADMServername() { return m_strUADMLastServername; };
		std::string getLastUADMDatabase() { return m_strUADMLastDatabase; };
		std::string getLastUADMPort() { return m_strUADMLastPortNo; };
		std::string getLastUADMEncryptionType();
		std::string getSystemDateTimeString(); // returns current system date/time as string in format "YYYY-MM-DD HH:MM:SS"
		void        addDebugInfo(std::string debugInfo);
		int         getEncryptionType() { return (int)m_enmUADMEncryptionType; };
		int         getConnectionTypePreference() { return (int)m_blnPreferVoodoo; };
		std::string encryptPassword(std::string strPassword, std::string strUsername, int encryptionType);

		void setUserID(long lngUserID) { m_lngUserID = lngUserID; };
		void setUsername(std::string strUsername) { m_strUsername = strUsername; };
		void setOSAccount(std::string strOSAccount) { m_strOSAccount = strOSAccount; };

		// helper methods
		static std::string trim(std::string data);
		static std::string toUpper(std::string data);
		static std::string toLower(std::string data);
		static std::string replaceAll(std::string strData, std::string strFrom, std::string strTo);
		static std::string replaceAllSpecialChars(std::string strData);

	private:
		// specific voodoo connection methods
		bool openVoodooConnection();
		// specific informix connection methods
		bool openInformixConnection();
		// decouple the sql execution from voodoo and informix connection
		int executeSQL(std::string strSQL); // for insert, update, delete statements
		int readSQLValue(std::string strSQL, std::string& strValue); // for single value select statements
		// general interface setup methods
		bool initSession();
		std::string convertConnectionData(std::string conn, bool bService);
		// PHX internal encryption/decryption methods
		std::string encryptOld(std::string data);
		std::string decryptOld(std::string data);
		// AES standard encryption/decryption methods
		std::string encryptNew(std::string data, std::string key);
		std::string decryptNew(std::string data, std::string key);


		HNMP			m_intVoodooConn;
#ifndef EXLUDE_INFORMIX
		ITConnection*	m_pobjIFXConn;
#else
		void* m_pobjIFXConn;
#endif
		bool			m_blnPreferVoodoo;
		int				m_intLoginMaxRetries;
		bool            m_blnLockUserAfterMaxRetries;
		int				m_intPasswordHistoryCount;
		int             m_intMaxPasswordValidityDays;
		bool			m_blnAllowLocalLogin;
		UADM_APP_ENVIRONMENT* m_pobjUADMEnvironment;
		UADM_CONNECTION_TYPE m_enmUADMConnectionType;
		UADM_PASSWORD_ENCRYPTION_TYPE m_enmUADMEncryptionType;
		UADM_APP_ENTRY* m_pobjUADMEntry;
		std::vector<std::string>* m_pvecDebugInfos;


		UADM_CONNECTION_TYPE m_enmUADMLastConnectionType;
		std::string m_strUADMLastServername;
		std::string m_strUADMLastDatabase;
		std::string m_strUADMLastPortNo;

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
		long m_lngCurrentDate;

		long m_lngAppID;
		std::string m_strAppName;
		std::string m_strAppDesc;
		std::string m_strLanguage;
		std::string m_strLanguageName;
		std::string m_strLastErrorMessage;
	};
}

#endif // UADM_LOGININTERFACE_H
