#if !defined(UADM_LOGINDLG_H)
#define UADM_LOGINDLG_H

#if _MSC_VER > 1000
#pragma once
#endif

#include "libappuadm_resource.h"		// main symbols
#include <string>
#include <vector>
#include "UADMLoginManager.h"
#include "UADMLoginInterface.h"

/////////////////////////////////////////////////////////////////////////////
// Dialog CUADMLoginDlg 
namespace UADM
{
	class CUADMLoginDlg
	{
		// Konstruktion
	public:
		CUADMLoginDlg(CUADMLoginInterface* pInterface = NULL, HMODULE hUADMResource = NULL, HWND hParent = NULL); 
		~CUADMLoginDlg();
		INT_PTR DoModal();
		void setAppNameVersion(std::string strAppKey, std::string strAppName, std::string strAppVersion) { m_strAppKey = strAppKey; m_strAppName = strAppName; m_strAppVersion = strAppVersion; };
		void setBackgroundImage(std::string strBackgroundImageFile) { m_strBackgroundImageFile = strBackgroundImageFile; };
		void setAppLanguages(std::vector<UADM_APP_LANGUAGE*>* pLanguages) { m_pvecLanguages = pLanguages; };
		void setAppEnvironments(std::vector<UADM_APP_ENVIRONMENT*>* pEnvironments) { m_pvecEnvironments = pEnvironments; };
		void setAppEntries(std::vector<UADM_APP_ENTRY*>* pEntries) { m_pvecEntries = pEntries; };
		void setUserMapping(bool blnUserMapping) { m_blnUserMapping = blnUserMapping; };
		int  getEntryIndex() { return m_intEntryIndex; };

	protected:

		void OnChangePasswordDialog();
		void OnOK();
		void OnCancel();
		BOOL OnInitDialog(HWND hDlg);
		void OnPaint();
		void OnChangeUADMLanguage();
		void OnChangeUADMEnvironment();
		void OnChangeUADMCountry();
		void OnChangeUADMRegionBranch();
		void OnChangeUADMUsername();
		void OnChangeUADMPassword();

		static INT_PTR CALLBACK DialogCallback(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
		BOOL MessageHandler(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
		int  FindComboBoxIndex(HWND hDlg, const std::string& strValue);

	private:

		void    ReadLastSuccessfulLogin();
		void    SaveLastSuccessfulLogin(UADM_APP_ENTRY* pEntry);
		std::string TranslateText(int intTextID);
		std::string getEnvironmentName(const std::string& strEnv);
		UADM_APP_ENVIRONMENT* getEnvironment(const std::string& strEnv);
		UADM_APP_ENTRY* getEntry(const std::string& strEnv, const std::string& strCountry, const std::string& strRegionBranchNo);
		std::string getCountryName(const std::string& strCountry);
		std::string readRegistry(const std::string& strRegistryKey);
		bool    writeRegistry(const std::string& strRegistryKey, const std::string& strValue);

		void resetInput();
		void fillUADMLanguages();
		void fillUADMEnvironments();
		void fillUADMCountries();
		void fillUADMRegionBranches();
		void setStatus();

		HMODULE m_hUADMResource;
		HWND  m_hParent;
		HWND  m_hDlg;
		int   m_intRetryCount;
		HFONT m_fontAppBold;
		HFONT m_fontUADMBold;
		HBITMAP m_bmpBackground;
		std::string m_strBackgroundImageFile;
		std::string m_strAppKey;
		std::string m_strAppName;
		std::string m_strAppVersion;

		std::string m_strUserName;
		std::string m_strPassword;
		std::string m_strCountry;
		std::string m_strRegionBranchNo;
		std::string m_strEnvironment;
		int m_intLanguage;
		std::string m_strLanguage;
		bool m_blnUserMapping;

		std::vector<UADM_APP_LANGUAGE*>* m_pvecLanguages;
		std::vector<UADM_APP_ENVIRONMENT*>* m_pvecEnvironments;

		int m_intEntryIndex;
		std::vector<UADM_APP_ENTRY*>* m_pvecEntries;

		CUADMLoginInterface* m_pobjInterface;
	};
};
#endif // UADM_LOGINDLG_H

