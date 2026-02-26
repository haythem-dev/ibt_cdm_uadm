// CUADMChangePWDlg.cpp: implementation file
//

#include "pch.h"
#include "framework.h"
#include "UADMChangePWDlg.h"

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CUADMChangePWDlg 
namespace UADM
{

	CUADMChangePWDlg::CUADMChangePWDlg(CUADMLoginInterface* pInterface, HMODULE hModuleResource, HWND hParent)
	{
		m_hUADMResource = hModuleResource;
		m_hParent = hParent;
		m_hDlg = NULL;
		m_intRetryCount = 0;
		m_pobjInterface = pInterface;
		m_pvecEnvironments = NULL;
		resetInput();
	}

	void CUADMChangePWDlg::resetInput()
	{
		m_strUserName = "";
		m_strOldPassword = "";
		m_strNewPassword = "";
		m_strConfirmPassword = "";
		m_strEnvironment = "";
		m_intLanguage = 0; // default to english
	}

	INT_PTR CUADMChangePWDlg::DoModal()
	{
		// Pass 'this' as CUADMLoginDlg class instance as lParam
		return DialogBoxParam(m_hUADMResource, MAKEINTRESOURCE(IDD_UADM_CHANGEPW), m_hParent, CUADMChangePWDlg::DialogCallback, (LPARAM)this);
	}

	// static
	INT_PTR CALLBACK CUADMChangePWDlg::DialogCallback(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
	{
		CUADMChangePWDlg* pThis = nullptr;
		if (message == WM_INITDIALOG)
		{
			// Store the pointer to the class instance in GWLP_USERDATA
			pThis = reinterpret_cast<CUADMChangePWDlg*>(lParam);
			SetWindowLongPtr(hDlg, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
		}
		else
		{
			// Retrieve the pointer to the class instance
			pThis = reinterpret_cast<CUADMChangePWDlg*>(GetWindowLongPtr(hDlg, GWLP_USERDATA));
		}
		if (pThis == nullptr)
		{
			return FALSE; // If pThis is null, we cannot proceed
		}
		return pThis->MessageHandler(hDlg, message, wParam, lParam);
	}

	//////////////////////////////////////////////////////////////////////////////
	// Implementation
	BOOL CUADMChangePWDlg::MessageHandler(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_INITDIALOG:
			return OnInitDialog(hDlg); // Initialization successful  
		case WM_COMMAND:
			//IDC_BTN_UADM_CANCEL clicked
			if (LOWORD(wParam) == IDC_BTN_UADM_CANCEL)
			{
				OnCancel();
				return TRUE; // Handled
			}
			//IDC_BTN_UADM_OK clicked
			else if ((LOWORD(wParam) == IDC_BTN_UADM_OK) || (LOWORD(wParam) == IDOK))
			{
				OnOK();
				return TRUE; // Handled
			}
			// IDC_CMB_UADM_ENVIRONMENT changed
			else if ((LOWORD(wParam) == IDC_CMB_UADM_ENVIRONMENT) && (HIWORD(wParam) == CBN_SELCHANGE))
			{
				OnChangeUADMEnvironment();
				return TRUE; // Handled
			}
			// IDC_TXT_UADM_USERNAME changed
			else if ((LOWORD(wParam) == IDC_TXT_UADM_USERNAME) && (HIWORD(wParam) == EN_CHANGE))
			{
				OnChangeUADMUsername();
				return TRUE; // Handled
			}
			// IDC_TXT_UADM_OLDPASSWORD changed
			else if ((LOWORD(wParam) == IDC_TXT_UADM_OLDPASSWORD) && (HIWORD(wParam) == EN_CHANGE))
			{
				OnChangeUADMOldPassword();
				return TRUE; // Handled
			}
			// IDC_TXT_UADM_NEWPASSWORD changed
			else if ((LOWORD(wParam) == IDC_TXT_UADM_NEWPASSWORD) && (HIWORD(wParam) == EN_CHANGE))
			{
				OnChangeUADMNewPassword();
				return TRUE; // Handled
			}
			// IDC_TXT_UADM_CONFIRMPASSWORD changed
			else if ((LOWORD(wParam) == IDC_TXT_UADM_CONFIRMPASSWORD) && (HIWORD(wParam) == EN_CHANGE))
			{
				OnChangeUADMConfirmPassword();
				return TRUE; // Handled
			}
			break;
		case WM_CLOSE:
			EndDialog(hDlg, IDCANCEL);
			return TRUE;
		}
		return FALSE; // Default dialog processing  
	}

	int CUADMChangePWDlg::FindComboBoxIndex(HWND hComboBox, const std::string& strValue)
	{
		if (hComboBox == NULL || strValue.empty() == true)
		{
			return CB_ERR; // Invalid parameters
		}
		int count = (int)SendMessage(hComboBox, CB_GETCOUNT, 0, 0);
		for (int i = 0; i < count; ++i)
		{
			char buffer[256];
			SendMessage(hComboBox, CB_GETLBTEXT, i, (LPARAM)buffer);
			if (strcmp(buffer, strValue.c_str()) == 0)
			{
				return i; // Found the item
			}
		}
		return CB_ERR; // Item not found
	}

	BOOL CUADMChangePWDlg::OnInitDialog(HWND hDlg)
	{
		// store the dialog handle
		m_hDlg = hDlg;

		SetDlgItemText(hDlg, IDD_UADM_CHANGEPW, TranslateText(IDS_UADM_CHANGEPW_TITLE_EN).c_str());
		SetDlgItemText(hDlg, IDC_LBL_UADM_USERNAME, TranslateText(IDS_UADM_USERNAME_EN).c_str());
		SetDlgItemText(hDlg, IDC_LBL_UADM_OLDPASSWORD, TranslateText(IDS_UADM_OLDPASSWORD_EN).c_str());
		SetDlgItemText(hDlg, IDC_LBL_UADM_NEWPASSWORD, TranslateText(IDS_UADM_NEWPASSWORD_EN).c_str());
		SetDlgItemText(hDlg, IDC_LBL_UADM_CONFIRMPASSWORD, TranslateText(IDS_UADM_CONFIRMPASSWORD_EN).c_str());
		SetDlgItemText(hDlg, IDC_LBL_UADM_ENVIRONMENT, TranslateText(IDS_UADM_ENVIRONMENT_EN).c_str());
		SetDlgItemText(hDlg, IDC_BTN_UADM_OK, TranslateText(IDS_UADM_OK_EN).c_str());
		SetDlgItemText(hDlg, IDC_BTN_UADM_CANCEL, TranslateText(IDS_UADM_CANCEL_EN).c_str());

		SetDlgItemText(hDlg, IDC_TXT_UADM_USERNAME, m_strUserName.c_str());
		if (!m_strUserName.empty() == true)
		{
			EnableWindow(GetDlgItem(hDlg, IDC_TXT_UADM_USERNAME), FALSE);
		}
		fillEnvironments();
		OnChangeUADMEnvironment();
		if (m_strEnvironment.empty() == false)
		{
			EnableWindow(GetDlgItem(hDlg, IDC_CMB_UADM_ENVIRONMENT), FALSE);
		}
		OnOK();

		return TRUE;  // return TRUE unless you set the focus to a control
					  // EXCEPTION: OCX-Eigenschaftenseiten sollten FALSE zurückgeben
	}

	/////////////////////////////////////////////////////////////////////////////
	// Behandlungsroutinen für Nachrichten CUADMChangePWDlg 

	void CUADMChangePWDlg::OnOK()
	{
		if (m_strUserName.empty() == true)
		{
			setStatus();
			SetFocus(GetDlgItem(m_hDlg, IDC_TXT_UADM_USERNAME)); // set focus to username field
			return;
		}
		if (m_strOldPassword.empty() == true)
		{
			setStatus();
			SetFocus(GetDlgItem(m_hDlg, IDC_TXT_UADM_OLDPASSWORD)); // set focus to old password field
			return;
		}
		if (m_strNewPassword.empty() == true)
		{
			setStatus();
			SetFocus(GetDlgItem(m_hDlg, IDC_TXT_UADM_NEWPASSWORD)); // set focus to new password field
			return;
		}
		if (m_strConfirmPassword.empty() == true)
		{
			setStatus();
			SetFocus(GetDlgItem(m_hDlg, IDC_TXT_UADM_CONFIRMPASSWORD)); // set focus to confirm password field
			return;
		}
		if (m_strOldPassword == m_strNewPassword)
		{
			std::string strError = TranslateText(IDS_UADM_ERROR_NEW_PASSWORD_SAME_AS_OLD_EN);
			SetDlgItemText(m_hDlg, IDC_LBL_UADM_STATUS, ("Status: " + strError).c_str());
			MessageBox(m_hDlg, strError.c_str(), "Error", MB_ICONERROR | MB_OK);
			SetDlgItemText(m_hDlg, IDC_TXT_UADM_NEWPASSWORD, ""); // clear the new password field
			SetDlgItemText(m_hDlg, IDC_TXT_UADM_CONFIRMPASSWORD, ""); // clear the confirmed password field
			SetFocus(GetDlgItem(m_hDlg, IDC_TXT_UADM_NEWPASSWORD)); // set focus to new password field
			return;
		}
		if (m_strNewPassword != m_strConfirmPassword)
		{
			std::string strError = TranslateText(IDS_UADM_ERROR_CONFIRM_PASSWORD_WRONG_EN);
			SetDlgItemText(m_hDlg, IDC_LBL_UADM_STATUS, ("Status: " + strError).c_str());
			MessageBox(m_hDlg, strError.c_str(), "Error", MB_ICONERROR | MB_OK);
			SetFocus(GetDlgItem(m_hDlg, IDC_TXT_UADM_CONFIRMPASSWORD)); // set focus to confirm password field
			return;
		}
		if (m_strEnvironment.empty() == true)
		{
			setStatus();
			SetFocus(GetDlgItem(m_hDlg, IDC_CMB_UADM_ENVIRONMENT)); // set focus to environment field
			return;
		}
		if (m_pobjInterface == NULL)
		{
			std::string strError = TranslateText(IDS_UADM_ERROR_INTERFACE_NOT_SET_EN);
			SetDlgItemText(m_hDlg, IDC_LBL_UADM_STATUS, ("Status: " + strError).c_str());
			MessageBox(m_hDlg, strError.c_str(), "Error", MB_ICONERROR | MB_OK);
			return;
		}
		m_pobjInterface->addDebugInfo("CUADMChangePWDlg::OnOK - PasswordChange started");
		// get the environment details to connect via the UADM Login interface
		UADM_APP_ENVIRONMENT* pEnv = getEnvironment(m_strEnvironment);
		if (pEnv == NULL)
		{
			std::string strError = TranslateText(IDS_UADM_ERROR_ENVIRONMENT_NOT_FOUND_EN);
			SetDlgItemText(m_hDlg, IDC_LBL_UADM_STATUS, ("Status: " + strError).c_str());
			MessageBox(m_hDlg, strError.c_str(), "Error", MB_ICONERROR | MB_OK);
			SetFocus(GetDlgItem(m_hDlg, IDC_CMB_UADM_ENVIRONMENT)); // set focus to environment field
			return;
		}
		if (m_pobjInterface->isConnected(pEnv) == false)
		{
			// set and open the UADM Login interface
			m_pobjInterface->initialize(pEnv);
			if (m_pobjInterface->openConnection() == false)
			{
				std::string strError = TranslateText(IDS_UADM_ERROR_INTERFACE_OPEN_EN);
				SetDlgItemText(m_hDlg, IDC_LBL_UADM_STATUS, ("Status: " + strError).c_str());
				MessageBox(m_hDlg, strError.c_str(), "Error", MB_ICONERROR | MB_OK);
				return;
			}
		}
		// verify the user with old password
		if (m_pobjInterface->verifyUser(m_strUserName, m_strOldPassword, false) == false)
		{
			m_intRetryCount++; // increase the retry count
			if (m_intRetryCount < m_pobjInterface->getLoginMaxRetries())
			{
				std::string strError = TranslateText(IDS_UADM_ERROR_WRONG_USERNAME_PASSWORD_EN);
				SetDlgItemText(m_hDlg, IDC_LBL_UADM_STATUS, ("Status: " + strError).c_str());
				MessageBox(m_hDlg, strError.c_str(), "Error", MB_ICONERROR | MB_OK);
				//m_pobjInterface->closeConnection();
				SetDlgItemText(m_hDlg, IDC_TXT_UADM_OLDPASSWORD, ""); // clear the old password field
				SetFocus(GetDlgItem(m_hDlg, IDC_TXT_UADM_OLDPASSWORD)); // set focus to old password field
				return;
			}
			else
			{
				std::string strError = TranslateText(IDS_UADM_ERROR_TOO_MANY_RETRIES_EN);
				SetDlgItemText(m_hDlg, IDC_LBL_UADM_STATUS, ("Status: " + strError).c_str());
				MessageBox(m_hDlg, strError.c_str(), "Error", MB_ICONERROR | MB_OK);
				//m_pobjInterface->closeConnection();
				resetInput(); // remove entered values and return to UADM Login Manager as cancel
				EndDialog(m_hDlg, IDCANCEL);
				return;
			}
		}
		// check if user is active
		if (m_pobjInterface->isUserActive() == false)
		{
			std::string strError = TranslateText(IDS_UADM_ERROR_USER_INACTIVE_EN);
			SetDlgItemText(m_hDlg, IDC_LBL_UADM_STATUS, ("Status: " + strError).c_str());
			MessageBox(m_hDlg, strError.c_str(), "Error", MB_ICONERROR | MB_OK);
			//m_pobjInterface->closeConnection();
			resetInput(); // remove entered values and return to UADM Login Manager as cancel
			EndDialog(m_hDlg, IDCANCEL);
			return;
		}
		if (m_pobjInterface->verifyNewPassword(m_strNewPassword) == false)
		{
			std::string strError = TranslateText(IDS_UADM_ERROR_PASSWORD_DOES_NOT_FIT_NEW_RULES_EN);
			if (m_pobjInterface->getPasswordEncryptionType() == 1)
			{
				strError = TranslateText(IDS_UADM_ERROR_PASSWORD_DOES_NOT_FIT_OLD_RULES_EN);
			}
			SetDlgItemText(m_hDlg, IDC_LBL_UADM_STATUS, ("Status: " + strError).c_str());
			MessageBox(m_hDlg, strError.c_str(), "Error", MB_ICONERROR | MB_OK);
			SetFocus(GetDlgItem(m_hDlg, IDC_TXT_UADM_NEWPASSWORD)); // set focus to new password field
			return;
		}
		// verify if the last password change was too recent
		if (m_pobjInterface->verifyLastPasswordChange() == false)
		{
			std::string strError = TranslateText(IDS_UADM_ERROR_PASSWORD_ALREADY_CHANGED_EN);
			SetDlgItemText(m_hDlg, IDC_LBL_UADM_STATUS, ("Status: " + strError).c_str());
			MessageBox(m_hDlg, strError.c_str(), "Error", MB_ICONERROR | MB_OK);
			EndDialog(m_hDlg, IDCANCEL);
			return;
		}
		// verify against historic passwords
		bool blnPWUsed = false;
		if (m_pobjInterface->verifyAgainstHistoricPasswords(m_strNewPassword, blnPWUsed) == true)
		{
			if (blnPWUsed == true)
			{
				std::string strError = TranslateText(IDS_UADM_ERROR_PASSWORD_HISTORIC_EN);
				SetDlgItemText(m_hDlg, IDC_LBL_UADM_STATUS, ("Status: " + strError).c_str());
				MessageBox(m_hDlg, strError.c_str(), "Error", MB_ICONERROR | MB_OK);
				SetDlgItemText(m_hDlg, IDC_TXT_UADM_NEWPASSWORD, ""); // clear the new password field
				SetDlgItemText(m_hDlg, IDC_TXT_UADM_CONFIRMPASSWORD, ""); // clear the confirm password field
				SetFocus(GetDlgItem(m_hDlg, IDC_TXT_UADM_NEWPASSWORD)); // set focus to new password field
				return;
			}
		}

		// login successful, save the new password
		if (m_pobjInterface->saveNewPassword(m_strNewPassword) == false)
		{
			std::string strError = TranslateText(IDS_UADM_ERROR_CANNOT_SAVE_NEW_PASSWORD_EN);
			SetDlgItemText(m_hDlg, IDC_LBL_UADM_STATUS, ("Status: " + strError).c_str());
			MessageBox(m_hDlg, strError.c_str(), "Error", MB_ICONERROR | MB_OK);
			return;
		}
		std::string strStatus = TranslateText(IDS_UADM_STATUS_NEW_PASSWORD_SAVED_EN);
		SetDlgItemText(m_hDlg, IDC_LBL_UADM_STATUS, ("Status: " + strStatus).c_str());
		MessageBox(m_hDlg, strStatus.c_str(), "Status", MB_ICONINFORMATION | MB_OK);
		m_pobjInterface->addDebugInfo("CUADMChangePWDlg::OnOK - PasswordChange done");
		// jump back to the UADM Login Dialog
		EndDialog(m_hDlg, IDOK);
	}

	void CUADMChangePWDlg::OnCancel()
	{
		resetInput(); // remove entered values and return to UADM Login Manager
		EndDialog(m_hDlg, IDCANCEL);
	}

	void CUADMChangePWDlg::OnChangeUADMUsername()
	{
		char buffer[256];
		GetDlgItemText(m_hDlg, IDC_TXT_UADM_USERNAME, buffer, _countof(buffer));
		m_strUserName = buffer;
		setStatus(); // update the status label
	}

	void CUADMChangePWDlg::OnChangeUADMOldPassword()
	{
		char buffer[256];
		GetDlgItemText(m_hDlg, IDC_TXT_UADM_OLDPASSWORD, buffer, _countof(buffer));
		m_strOldPassword = buffer;
		setStatus();
	}

	void CUADMChangePWDlg::OnChangeUADMNewPassword()
	{
		char buffer[256];
		GetDlgItemText(m_hDlg, IDC_TXT_UADM_NEWPASSWORD, buffer, _countof(buffer));
		m_strNewPassword = buffer;
		setStatus();
	}

	void CUADMChangePWDlg::OnChangeUADMConfirmPassword()
	{
		char buffer[256];
		GetDlgItemText(m_hDlg, IDC_TXT_UADM_CONFIRMPASSWORD, buffer, _countof(buffer));
		m_strConfirmPassword = buffer;
		setStatus();
	}

	void CUADMChangePWDlg::fillEnvironments()
	{
		int intSel = 0; // reset to first item
		HWND hEnvironmentCombo = GetDlgItem(m_hDlg, IDC_CMB_UADM_ENVIRONMENT);
		if (hEnvironmentCombo == NULL)
		{
			return; // if the combo box is not available, do nothing
		}
		// reset the combo box
		SendMessage(hEnvironmentCombo, CB_RESETCONTENT, 0, 0);
		SendMessage(hEnvironmentCombo, CB_ADDSTRING, 0, (LPARAM)TranslateText(IDS_UADM_PLEASE_SELECT_EN).c_str());
		if (m_pvecEnvironments != NULL)
		{
			std::vector<UADM_APP_ENVIRONMENT*>::iterator it;
			for (it = m_pvecEnvironments->begin(); it != m_pvecEnvironments->end(); it++)
			{
				UADM_APP_ENVIRONMENT* pEnv = *it;
				if (pEnv != NULL)
				{
					std::string strEnv = pEnv->strEnvironment;
					std::string item = strEnv + " - " + pEnv->strEnvName;
					if (FindComboBoxIndex(hEnvironmentCombo, item.c_str()) == CB_ERR) // check if the item not yet exists
					{
						// add the environment only if it is not already in the list
						SendMessage(hEnvironmentCombo, CB_ADDSTRING, 0, (LPARAM)item.c_str());
						if (strEnv == m_strEnvironment)
						{
							// found the environment, set the index
							intSel = (int)SendMessage(hEnvironmentCombo, CB_GETCOUNT, 0, 0) - 1; // last item
						}
					}
				}
			}
		}
		if (intSel == 0)
		{
			// if no environment is selected and the list contains only one entry beside the "Please select" entry, select that one
			int cnt = (int)SendMessage(hEnvironmentCombo, CB_GETCOUNT, 0, 0);
			if (cnt == 2) // only "Please select" and one environment
			{
				intSel = 1; // select the first and only environment
			}
		}
		SendMessage(hEnvironmentCombo, CB_SETCURSEL, intSel, 0); // preselect the matching item or first item if not found
	}

	void CUADMChangePWDlg::OnChangeUADMEnvironment()
	{
		m_strEnvironment = "";
		HWND hEnvironmentCombo = GetDlgItem(m_hDlg, IDC_CMB_UADM_ENVIRONMENT);
		if (hEnvironmentCombo == NULL)
		{
			return; // if the combo box is not available, do nothing
		}
		// get the selected language from the combo box
		int sel = (int)SendMessage(hEnvironmentCombo, CB_GETCURSEL, 0, 0);

		if (sel != CB_ERR)
		{
			char buffer[256];
			SendMessage(hEnvironmentCombo, CB_GETLBTEXT, sel, (LPARAM)buffer);
			std::string strEnv = buffer;

			if (strEnv.empty() == false)
			{
				m_strEnvironment = strEnv.substr(0, strEnv.find(" - ")); // get the first characters before the separator (e.g., "DEV - Development", etc.)
			}
		}
		setStatus();
	}

	void CUADMChangePWDlg::setStatus()
	{
		if (m_strUserName.empty())
		{
			SetDlgItemText(m_hDlg, IDC_LBL_UADM_STATUS, TranslateText(IDS_UADM_STATUS_USERNAME_EN).c_str());
			return;
		}
		if (m_strOldPassword.empty())
		{
			SetDlgItemText(m_hDlg, IDC_LBL_UADM_STATUS, TranslateText(IDS_UADM_STATUS_OLDPASSWORD_EN).c_str());
			return;
		}
		if (m_strNewPassword.empty())
		{
			SetDlgItemText(m_hDlg, IDC_LBL_UADM_STATUS, TranslateText(IDS_UADM_STATUS_NEWPASSWORD_EN).c_str());
			return;
		}
		if (m_strConfirmPassword.empty())
		{
			SetDlgItemText(m_hDlg, IDC_LBL_UADM_STATUS, TranslateText(IDS_UADM_STATUS_CONFIRMPASSWORD_EN).c_str());
			return;
		}
		if (m_strEnvironment.empty())
		{
			SetDlgItemText(m_hDlg, IDC_LBL_UADM_STATUS, TranslateText(IDS_UADM_STATUS_ENVIRONMENT_EN).c_str());
			return;
		}
	}

	std::string CUADMChangePWDlg::TranslateText(int langIDText)
	{
		char buffer[256];
		std::string strText = ""; // default empty string
		// This function is used to translate the text based on the language ID
		if (m_intLanguage >= 0)
		{
			if (LoadString(m_hUADMResource, langIDText + m_intLanguage, buffer, 255))
			{
				strText = std::string(buffer);
			}
		}
		if (strText.empty() == true) // if translation failed
		{
			if (LoadString(m_hUADMResource, langIDText, buffer, 255)) // fallback to English
			{
				strText = std::string(buffer);
			}
		}
		return strText; // return empty string if translation fails
	}

	UADM_APP_ENVIRONMENT* CUADMChangePWDlg::getEnvironment(const std::string& strEnv)
	{
		UADM_APP_ENVIRONMENT* ret = NULL;

		std::vector<UADM_APP_ENVIRONMENT*>::iterator it;
		for (it = m_pvecEnvironments->begin(); it != m_pvecEnvironments->end(); it++)
		{
			UADM_APP_ENVIRONMENT* pEnv = *it;
			if (pEnv != NULL)
			{
				if (pEnv->strEnvironment == strEnv)
				{
					// found the environment, return the environment object
					ret = pEnv;
					break;
				}
			}
		}
		return ret;
	}
}
/////////////////////////////////////////////////////////////////////////////