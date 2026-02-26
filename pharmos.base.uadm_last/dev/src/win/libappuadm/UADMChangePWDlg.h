#if !defined(UADM_CHANGEPWDLG_H)
#define UADM_CHANGEPWDLG_H

#if _MSC_VER > 1000
#pragma once
#endif

#include "libappuadm_resource.h"		// main symbols
#include <string>
#include <vector>
#include "UADMLoginManager.h"
#include "UADMLoginInterface.h"

/////////////////////////////////////////////////////////////////////////////
// Dialog CUADMChangePWDlg 
namespace UADM
{
	class CUADMChangePWDlg
	{
		// Construktion
	public:

		CUADMChangePWDlg(CUADMLoginInterface* pInterface = NULL, HMODULE hUADMResource = NULL, HWND hParent = NULL);   // standard constructor
		INT_PTR DoModal();
		void setEnvironments(std::vector<UADM_APP_ENVIRONMENT*>* pEnvironments, const std::string& strCurrEnv) { m_pvecEnvironments = pEnvironments; m_strEnvironment = strCurrEnv; };
		void setLanguage(int intLanguage) { m_intLanguage = intLanguage; };
		void setUserName(const std::string& strUserName) { m_strUserName = strUserName; };

	protected:

		void OnOK();
		void OnCancel();
		BOOL OnInitDialog(HWND hDlg);
		void OnChangeUADMEnvironment();
		void OnChangeUADMUsername();
		void OnChangeUADMOldPassword();
		void OnChangeUADMNewPassword();
		void OnChangeUADMConfirmPassword();

		static INT_PTR CALLBACK DialogCallback(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
		BOOL MessageHandler(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
		int  FindComboBoxIndex(HWND hDlg, const std::string& strValue);

	private:

		void resetInput();
		void fillEnvironments();
		void setStatus();
		std::string TranslateText(int intTextID);
		UADM_APP_ENVIRONMENT* getEnvironment(const std::string& strEnv);

		HMODULE m_hUADMResource;
		HWND  m_hParent;
		HWND  m_hDlg;
		int   m_intRetryCount;
		std::string m_strUserName;
		std::string m_strOldPassword;
		std::string m_strNewPassword;
		std::string m_strConfirmPassword;
		std::string m_strEnvironment;
		int m_intLanguage;

		std::vector<UADM_APP_ENVIRONMENT*>* m_pvecEnvironments;

		CUADMLoginInterface* m_pobjInterface;
	};
};
#endif // UADM_CHANGEPWDLG_H

