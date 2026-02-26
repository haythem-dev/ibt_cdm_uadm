// CUADMLoginDlg.cpp: implementation file
//

#include "pch.h"
#include "framework.h"
#include "UADMLoginDlg.h"
#include "UADMChangePWDlg.h"
#include "uadm_version.h"

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CUADMLoginDlg 
namespace UADM
{

	CUADMLoginDlg::CUADMLoginDlg(CUADMLoginInterface* pInterface, HMODULE hModuleResource, HWND hParent)
	{
		m_hUADMResource = hModuleResource;
		m_hParent = hParent;
		m_hDlg = NULL;
		m_pobjInterface = pInterface;
		m_intRetryCount = 0;
		m_strAppKey = "";
		m_strAppName = "";
		m_strAppVersion = "";
		m_pvecLanguages = NULL;
		m_pvecEnvironments = NULL;
		m_pvecEntries = NULL;
		m_bmpBackground = NULL;
		m_fontAppBold = NULL;
		m_fontUADMBold = NULL;
		resetInput();
	}
	CUADMLoginDlg::~CUADMLoginDlg()
	{
		if (m_bmpBackground != NULL)
		{
			DeleteObject(m_bmpBackground);
		}
		if (m_fontAppBold != NULL)
		{
			DeleteObject(m_fontAppBold);
		}
		if (m_fontUADMBold != NULL)
		{
			DeleteObject(m_fontUADMBold);
		}
	}

	void CUADMLoginDlg::resetInput()
	{
		m_strUserName = "";
		m_strPassword = "";
		m_strCountry = "";
		m_strRegionBranchNo = "";
		m_strEnvironment = "";
		m_intLanguage = 0;
		m_strLanguage = "";
		m_intEntryIndex = -1;
	}

	INT_PTR CUADMLoginDlg::DoModal()
	{
		// Pass 'this' as CUADMLoginDlg class instance as lParam
		return DialogBoxParam(m_hUADMResource, MAKEINTRESOURCE(IDD_UADM_LOGIN), m_hParent, CUADMLoginDlg::DialogCallback, (LPARAM)this);
	}

	// static
	INT_PTR CALLBACK CUADMLoginDlg::DialogCallback(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
	{
		CUADMLoginDlg* pThis = nullptr;
		if (message == WM_INITDIALOG)
		{
			// Store the pointer to the class instance in GWLP_USERDATA
			pThis = reinterpret_cast<CUADMLoginDlg*>(lParam);
			SetWindowLongPtr(hDlg, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
		}
		else
		{
			// Retrieve the pointer to the class instance
			pThis = reinterpret_cast<CUADMLoginDlg*>(GetWindowLongPtr(hDlg, GWLP_USERDATA));
		}
		if (pThis == nullptr)
		{
			return FALSE; // If pThis is null, we cannot proceed
		}
		return pThis->MessageHandler(hDlg, message, wParam, lParam);
	}

	//////////////////////////////////////////////////////////////////////////////
	// Implementation
	BOOL CUADMLoginDlg::MessageHandler(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_INITDIALOG:
			return OnInitDialog(hDlg); // Initialization successful  
		case WM_PAINT:
			OnPaint(); // Handle the paint message
			return TRUE; // Handled
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
			//IDC_BTN_UADM_CHANGE_PWD clicked
			else if (LOWORD(wParam) == IDC_BTN_UADM_CHANGE_PWD)
			{
				OnChangePasswordDialog();
				return TRUE; // Handled
			}
			// IDC_TXT_UADM_USERNAME changed
			else if ((LOWORD(wParam) == IDC_TXT_UADM_USERNAME) && (HIWORD(wParam) == EN_CHANGE))
			{
				OnChangeUADMUsername();
				return TRUE; // Handled
			}
			// IDC_TXT_UADM_PASSWORD changed
			else if ((LOWORD(wParam) == IDC_TXT_UADM_PASSWORD) && (HIWORD(wParam) == EN_CHANGE))
			{
				OnChangeUADMPassword();
				return TRUE; // Handled
			}
			// IDC_CMB_UADM_LANGUAGE changed
			else if ((LOWORD(wParam) == IDC_CMB_UADM_LANGUAGE) && (HIWORD(wParam) == CBN_SELCHANGE))
			{
				OnChangeUADMLanguage();
				return TRUE; // Handled
			}
			// IDC_CMB_UADM_ENVIRONMENT changed
			else if ((LOWORD(wParam) == IDC_CMB_UADM_ENVIRONMENT) && (HIWORD(wParam) == CBN_SELCHANGE))
			{
				OnChangeUADMEnvironment();
				return TRUE; // Handled
			}
			// IDC_CMB_UADM_COUNTRY changed
			else if ((LOWORD(wParam) == IDC_CMB_UADM_COUNTRY) && (HIWORD(wParam) == CBN_SELCHANGE))
			{
				OnChangeUADMCountry();
				return TRUE; // Handled
			}
			// IDC_CMB_UADM_REGIONBRANCH changed
			else if ((LOWORD(wParam) == IDC_CMB_UADM_REGIONBRANCH) && (HIWORD(wParam) == CBN_SELCHANGE))
			{
				OnChangeUADMRegionBranch();
				return TRUE; // Handled
			}
			// if return key pressed on any field

			break;
		case WM_CLOSE:
			EndDialog(hDlg, IDCANCEL);
			return TRUE;
		}
		return FALSE; // Default dialog processing  
	}

	int CUADMLoginDlg::FindComboBoxIndex(HWND hComboBox, const std::string& strValue)
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

	BOOL CUADMLoginDlg::OnInitDialog(HWND hDlg)
	{
		// store the dialog handle
		m_hDlg = hDlg;
		// Load the background bitmap
		bool loaded = false;
		if (!m_strBackgroundImageFile.empty())
		{
			HBITMAP hBitmap = (HBITMAP)LoadImage(NULL, m_strBackgroundImageFile.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			if (hBitmap != NULL)
			{
				// get the size of the bitmap
				BITMAP bm = { 0 };
				GetObject(hBitmap, (int)sizeof bm, &bm);
				if ((bm.bmWidth == 500) && (bm.bmHeight == 278))
				{
					m_bmpBackground = hBitmap;
					loaded = true; // successfully loaded the bitmap with correct size
				}
			}
		}
		if (loaded == false)
		{
			// default background bitmap
			m_bmpBackground = LoadBitmap(m_hUADMResource, MAKEINTRESOURCE(IDB_UADM_LOGIN));
		}
		// Set the app and version with a bold font
		LOGFONT lf = { 0 };
		HWND hwndAppVersion = GetDlgItem(hDlg, IDC_LBL_UADM_APPVERSION); 
		HFONT hfntAppVersion = (HFONT)SendMessage(hwndAppVersion, WM_GETFONT, 0, 0);
		HDC hAppDC = GetDC(hwndAppVersion);
		GetObject(hfntAppVersion, sizeof(lf), &lf);
		lf.lfWeight = FW_BOLD;
		lf.lfHeight = -MulDiv(10, GetDeviceCaps(hAppDC, LOGPIXELSY), 72); // 10 points
		m_fontAppBold = CreateFontIndirect(&lf);
		// Set bold font to static control
		SendMessage(hwndAppVersion, WM_SETFONT, (WPARAM)m_fontAppBold, TRUE);
		// verify that the application name and version text fits into the static control
		std::string strText = m_strAppName + " Version " + m_strAppVersion;
		SIZE textSize = { 0 };
		GetTextExtentPoint32(hAppDC, strText.c_str(), (int)strText.length(), &textSize);
		RECT rect;
		GetClientRect(hwndAppVersion, &rect);
		if (textSize.cx > (rect.right - rect.left))
		{
			// reduce the font size to fit into the static control
			int newFontSize = (int)(9.5 * ((double)(rect.right - rect.left) / (double)textSize.cx));
			lf.lfHeight = -MulDiv(newFontSize, GetDeviceCaps(hAppDC, LOGPIXELSY), 72); // new font size in points
			DeleteObject(m_fontAppBold);
			m_fontAppBold = CreateFontIndirect(&lf);
			// Set bold font to static control
			SendMessage(hwndAppVersion, WM_SETFONT, (WPARAM)m_fontAppBold, TRUE);
		}
		ReleaseDC(hwndAppVersion, hAppDC);

		SetDlgItemText(hDlg, IDC_LBL_UADM_APPVERSION, strText.c_str());

		// Set the uadm with a small bold font (will be paint in the OnPaint() function)
		HWND hwndUADMVersion = GetDlgItem(hDlg, IDC_LBL_UADM_VERSION);
		HFONT hfntUADMVersion = (HFONT)SendMessage(hwndUADMVersion, WM_GETFONT, 0, 0);
		GetObject(hfntUADMVersion, sizeof(lf), &lf);
		lf.lfWeight = FW_BOLD;
		lf.lfHeight = -MulDiv(6, GetDeviceCaps(GetDC(hwndUADMVersion), LOGPIXELSY), 72); // 10 points
		m_fontUADMBold = CreateFontIndirect(&lf);

		// get the win user account
		// get OS account name and set it in the interface
		char csUser[256];
		DWORD dwSize = 256;
		GetUserName(csUser, &dwSize);
		m_pobjInterface->setOSAccount(csUser);

		// default to english language
		m_intLanguage = 0;
		m_strLanguage = "EN";
		SetWindowText(m_hDlg, (TranslateText(IDS_UADM_LOGIN_TITLE_EN) + " - " + m_strAppName).c_str());
		//SetDlgItemText(m_hDlg, IDD_UADM_LOGIN, (TranslateText(IDS_UADM_LOGIN_TITLE_EN) + " - " + m_strAppName).c_str());
		SetDlgItemText(hDlg, IDC_LBL_UADM_USERNAME, TranslateText(IDS_UADM_USERNAME_EN).c_str());
		SetDlgItemText(hDlg, IDC_LBL_UADM_PASSWORD, TranslateText(IDS_UADM_PASSWORD_EN).c_str());
		SetDlgItemText(hDlg, IDC_LBL_UADM_COUNTRY, TranslateText(IDS_UADM_COUNTRY_EN).c_str());
		SetDlgItemText(hDlg, IDC_LBL_UADM_REGIONBRANCH, TranslateText(IDS_UADM_REGIONBRANCH_EN).c_str());
		SetDlgItemText(hDlg, IDC_LBL_UADM_ENVIRONMENT, TranslateText(IDS_UADM_ENVIRONMENT_EN).c_str());
		SetDlgItemText(hDlg, IDC_LBL_UADM_LANGUAGE, TranslateText(IDS_UADM_LANGUAGE_EN).c_str());
		SetDlgItemText(hDlg, IDC_BTN_UADM_CHANGE_PWD, TranslateText(IDS_UADM_CHANGEPW_EN).c_str());
		SetDlgItemText(hDlg, IDC_BTN_UADM_OK, TranslateText(IDS_UADM_OK_EN).c_str());
		SetDlgItemText(hDlg, IDC_BTN_UADM_CANCEL, TranslateText(IDS_UADM_CANCEL_EN).c_str());

		ReadLastSuccessfulLogin();
		SetDlgItemText(hDlg, IDC_TXT_UADM_USERNAME, m_strUserName.c_str());
		fillUADMLanguages();
		OnChangeUADMLanguage();
		fillUADMEnvironments();
		OnChangeUADMEnvironment();
		fillUADMCountries();
		OnChangeUADMCountry();
		fillUADMRegionBranches();
		OnChangeUADMRegionBranch();
		OnOK();

		return FALSE;  // return TRUE unless you set the focus to a control
					  // EXCEPTION: OCX-Eigenschaftenseiten sollten FALSE zurückgeben
	}

	void CUADMLoginDlg::OnPaint()
	{
		if (IsWindowVisible(m_hDlg) == TRUE)
		{
			PAINTSTRUCT ps;

			HDC hDC = BeginPaint(m_hDlg, &ps);
			// get dialog client size
			RECT rect;
			GetClientRect(m_hDlg, &rect);
			// get bitmap size
			BITMAP bm = { 0 };
			GetObject(m_bmpBackground, (int)sizeof bm, &bm);
			SIZE cz;
			cz.cx = bm.bmWidth;
			cz.cy = bm.bmHeight;
			HBITMAP bmpM;
			// create window dc with the size of the dialog client
			HDC hMainDC;
			hMainDC = CreateCompatibleDC(hDC);
			bmpM = CreateCompatibleBitmap(hDC, rect.right-rect.left, rect.bottom-rect.top);
			HBITMAP pbmpM = (HBITMAP)SelectObject(hMainDC, bmpM);
			HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
			FillRect(hMainDC, &rect, hBrush); // fill with white
			// create bitmap dc with the loaded bitmap
			HDC hMemDC;
			hMemDC = CreateCompatibleDC(hDC);
			HBITMAP pbmp = (HBITMAP)SelectObject(hMemDC, m_bmpBackground);
			// Stretch the bitmap to the window dc
			SetStretchBltMode(hMainDC, COLORONCOLOR);
			StretchBlt(hMainDC, 0, 0, rect.right - rect.left, rect.bottom - rect.top, hMemDC, 0, 0, cz.cx, cz.cy, SRCCOPY);
			// draw the UADM text in the bottom right corner
			HFONT poldFont = (HFONT)SelectObject(hMainDC , m_fontUADMBold);
			SetTextColor(hMainDC, RGB(0, 146, 123)); // Set text color to phoenix green  
			SetBkMode(hMainDC, TRANSPARENT); // Set background mode to transparent
			SetTextAlign(hMainDC, TA_LEFT | TA_TOP); // Align text to the left and top
			std::string strText = std::string(C_PROGRAM_NAME) + " V" + std::string(UADM_VERSION);
			int enctype = m_pobjInterface->getEncryptionType();
			if (enctype == UADM_PASSWORD_ENCRYPTION_TYPE_PHOENIX)
			{
				strText += " 1";
			}
			else if (enctype == UADM_PASSWORD_ENCRYPTION_TYPE_AES)
			{
				strText += " 2";
			}
			else
			{
				strText += " 0";
			}
			int confconntype = m_pobjInterface->getConnectionTypePreference();
			if (confconntype == 1)
			{
				strText += "V"; // voodoo connection
			}
			else
			{
				strText += "I"; // informix connection
			}
			std::string realconntype = m_pobjInterface->getLastUADMConnectionType();
			if (realconntype.at(0) == 'V')
			{
				strText += "V"; // voodoo connection
			}
			else if (realconntype.at(0) == 'I')
			{
				strText += "I"; // informix connection
			}
			else
			{
				strText += "N"; // no connection
			}

			// Get the size of the text to center it
			SIZE textSize;
			GetTextExtentPoint32(hMainDC, strText.c_str(), (int)strText.length(), &textSize);
			// Calculate the position to center the text in the bottom right corner
			int x = 15; // 10 pixels from the right edge
			int y = rect.bottom - rect.top - textSize.cy - 25; // 10 pixels from the bottom edge
			// fill the background with white to make the text readable
			RECT fillRect = { x - 2, y, x + textSize.cx + 4, y + textSize.cy };
			FillRect(hMainDC, &fillRect, hBrush); // fill with white
			// Draw the text
			TextOut(hMainDC, x, y, strText.c_str(), (int)strText.length());
			// Restore the old font
			SelectObject(hMainDC, poldFont);
			DeleteObject(hBrush); // delete the brush

			// paint the window dc into the dialog dc
			BitBlt(hDC, 0, 0, rect.right - rect.left, rect.bottom - rect.top, hMainDC, 0, 0, SRCCOPY);
			// release the graphics resources
			SelectObject(hMainDC, pbmpM);
			DeleteDC(hMainDC);
			SelectObject(hMemDC, pbmp);
			DeleteDC(hMemDC);
			DeleteObject(bmpM);

			EndPaint(m_hDlg, &ps);
		}
	}

	/////////////////////////////////////////////////////////////////////////////
	// Behandlungsroutinen für Nachrichten CUADMLoginDlg 

	void CUADMLoginDlg::OnChangePasswordDialog()
	{
		CUADMChangePWDlg dlg(m_pobjInterface, m_hUADMResource, m_hDlg);
		dlg.setEnvironments(m_pvecEnvironments, m_strEnvironment);
		dlg.setLanguage(m_intLanguage);
		dlg.setUserName(m_strUserName);
		if (dlg.DoModal() == IDOK)
		{
			SetDlgItemText(m_hDlg, IDC_TXT_UADM_PASSWORD, "");
			setStatus();
			SetFocus(GetDlgItem(m_hDlg, IDC_TXT_UADM_PASSWORD));
		}
	}

	void CUADMLoginDlg::OnOK()
	{
		if (m_strUserName.empty() == true)
		{
			setStatus();
			SetFocus(GetDlgItem(m_hDlg, IDC_TXT_UADM_USERNAME));
			return;
		}
		if (m_strPassword.empty() == true)
		{
			setStatus();
			SetFocus(GetDlgItem(m_hDlg, IDC_TXT_UADM_PASSWORD));
			return;
		}
		if (m_strEnvironment.empty() == true)
		{
			setStatus();
			SetFocus(GetDlgItem(m_hDlg, IDC_CMB_UADM_ENVIRONMENT));
			return;
		}
		if (m_strCountry.empty() == true)
		{
			setStatus();
			SetFocus(GetDlgItem(m_hDlg, IDC_CMB_UADM_COUNTRY));
			return;
		}
		if (m_strRegionBranchNo.empty() == true)
		{
			setStatus();
			SetFocus(GetDlgItem(m_hDlg, IDC_CMB_UADM_REGIONBRANCH));
			return;
		}
		if (m_pobjInterface == NULL)
		{
			std::string strError = TranslateText(IDS_UADM_ERROR_INTERFACE_NOT_SET_EN);
			SetDlgItemText(m_hDlg, IDC_LBL_UADM_STATUS, ("Status: " + strError).c_str());
			MessageBox(m_hDlg, strError.c_str(), "Error", MB_ICONERROR | MB_OK);
			return;
		}
		m_pobjInterface->addDebugInfo("CUADMLoginDlg::OnOK - Login started");
		// get the environment details to connect via the UADM Login interface
		UADM_APP_ENVIRONMENT* pEnv = getEnvironment(m_strEnvironment);
		if (pEnv == NULL)
		{
			std::string strError = TranslateText(IDS_UADM_ERROR_ENVIRONMENT_NOT_FOUND_EN);
			SetDlgItemText(m_hDlg, IDC_LBL_UADM_STATUS, ("Status: " + strError).c_str());
			MessageBox(m_hDlg, strError.c_str(), "Error", MB_ICONERROR | MB_OK);
			SetFocus(GetDlgItem(m_hDlg, IDC_CMB_UADM_ENVIRONMENT));
			return;
		}
		if (m_pobjInterface->isConnected(pEnv) == false)
		{
			// set and (re-)open the UADM Login interface
			m_pobjInterface->initialize(pEnv);
			if (m_pobjInterface->openConnection() == false)
			{
				std::string strError = TranslateText(IDS_UADM_ERROR_INTERFACE_OPEN_EN);
				SetDlgItemText(m_hDlg, IDC_LBL_UADM_STATUS, ("Status: " + strError).c_str());
				MessageBox(m_hDlg, strError.c_str(), "Error", MB_ICONERROR | MB_OK);
				return;
			}
		}
		// verify in case of usermapping is enabled that user has given local user and password for specific branch
		bool blnEnc = false;
		if (m_blnUserMapping == true)
		{
			if (m_pobjInterface->verifyLocalUser(m_strAppName, atoi(m_strRegionBranchNo.c_str()), m_strUserName, m_strPassword) == true)
			{
				// only in case local user was found and verified, get the mapped uadm user
				int intLocalBranch = atoi(m_strRegionBranchNo.c_str());
				std::string strLocalUser = m_strUserName;
				std::string strLocalPwd = m_strPassword;
				// get the mapped uadm user for the given regionbranchno and local user
				m_strUserName = m_pobjInterface->getUsername();
				if (m_strUserName.compare(strLocalUser) != 0)
				{
					SetDlgItemText(m_hDlg, IDC_TXT_UADM_USERNAME, m_strUserName.c_str());
					SetDlgItemText(m_hDlg, IDC_TXT_UADM_PASSWORD, "");
					// inform user about the mapping
					std::string strInfo = TranslateText(IDS_UADM_STATUS_USER_MAPPING_EN) + "\n\n" + strLocalUser + " (" + std::to_string((long long)intLocalBranch) + ") -> " + m_strUserName;
					MessageBox(m_hDlg, strInfo.c_str(), "Info", MB_ICONINFORMATION | MB_OK);
					// allow or not allow local login
					m_strPassword = "";
					if (m_pobjInterface->IsLocalLoginAllowed() == true)
					{
						m_strPassword = m_pobjInterface->getEncPwd(); // get the encrypted password
						blnEnc = true;
					}
					// might be that verification of pwd failed due to empty local user or wrong local pwd
					if (m_strPassword.empty() == true)
					{
						setStatus();
						SetFocus(GetDlgItem(m_hDlg, IDC_TXT_UADM_PASSWORD));
						return;
					}
				}
			}
		}
		// verify the user
		if (m_pobjInterface->verifyUser(m_strUserName, m_strPassword, blnEnc) == false)
		{
			m_intRetryCount++; // increase the retry count
			if (m_intRetryCount < m_pobjInterface->getLoginMaxRetries())
			{
				std::string strError = TranslateText(IDS_UADM_ERROR_WRONG_USERNAME_PASSWORD_EN);
				SetDlgItemText(m_hDlg, IDC_LBL_UADM_STATUS, ("Status: " + strError).c_str());
				MessageBox(m_hDlg, strError.c_str(), "Error", MB_ICONERROR | MB_OK);
				SetDlgItemText(m_hDlg, IDC_TXT_UADM_PASSWORD, "");
				SetFocus(GetDlgItem(m_hDlg, IDC_TXT_UADM_PASSWORD)); // set focus to password field
				return;
			}
			else
			{
				m_pobjInterface->verifyAndLockUser(m_strUserName);
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
		// check if user needs to change the password
		if (m_pobjInterface->isPasswordChangeRequired() == true)
		{
			std::string strInfo = TranslateText(IDS_UADM_STATUS_PASSWORD_CHANGE_REQUIRED_EN);
			SetDlgItemText(m_hDlg, IDC_LBL_UADM_STATUS, ("Status: " + strInfo).c_str());
			OnChangePasswordDialog(); // open the change password dialog
			//m_pobjInterface->closeConnection();
			SetDlgItemText(m_hDlg, IDC_TXT_UADM_PASSWORD, "");
			SetFocus(GetDlgItem(m_hDlg, IDC_TXT_UADM_PASSWORD)); // set focus to password field
			return;
		}
		UADM_APP_ENTRY* pEntry = getEntry(m_strEnvironment, m_strCountry, m_strRegionBranchNo);
		if (pEntry == NULL)
		{
			std::string strError = TranslateText(IDS_UADM_ERROR_ENTRY_NOT_FOUND_EN);
			SetDlgItemText(m_hDlg, IDC_LBL_UADM_STATUS, ("Status: " + strError).c_str());
			MessageBox(m_hDlg, strError.c_str(), "Error", MB_ICONERROR | MB_OK);
			return;
		}
		// login successful, save the last successful login to interface and registry
		// get OS account name and set it in the interface, as this might be lost after pwd change
		char csUser[256];
		DWORD dwSize = 256;
		GetUserName(csUser, &dwSize);
		m_pobjInterface->setOSAccount(csUser);
		m_pobjInterface->setUADMEntry(pEntry);
		m_pobjInterface->findAndSetLanguage(m_pvecLanguages, m_strLanguage);
		SaveLastSuccessfulLogin(pEntry);
		m_pobjInterface->updateLastLoginDate();
		m_pobjInterface->addDebugInfo("CUADMLoginDlg::OnOK - Login done");
		// jump back to the UADM Login Manager
		EndDialog(m_hDlg, IDOK);
	}

	void CUADMLoginDlg::OnCancel()
	{
		resetInput(); // remove entered values and return to UADM Login Manager
		m_pobjInterface->addDebugInfo("CUADMLoginDlg::OnCancel - Aborted Login");
		EndDialog(m_hDlg, IDCANCEL);
	}

	void CUADMLoginDlg::OnChangeUADMUsername()
	{
		char buffer[256];
		GetDlgItemText(m_hDlg, IDC_TXT_UADM_USERNAME, buffer, _countof(buffer));
		m_strUserName = buffer;
		setStatus(); // update the status label
	}

	void CUADMLoginDlg::OnChangeUADMPassword()
	{
		char buffer[256];
		GetDlgItemText(m_hDlg, IDC_TXT_UADM_PASSWORD, buffer, _countof(buffer));
		m_strPassword = buffer;
		setStatus(); // update the status label
	}
	
	void CUADMLoginDlg::fillUADMLanguages()
	{
		m_intLanguage = 0;
		HWND hLanguageCombo = GetDlgItem(m_hDlg, IDC_CMB_UADM_LANGUAGE);
		if (hLanguageCombo == NULL)
		{
			return; // if the combo box is not available, do nothing
		}
		// reset the combo box
		SendMessage(hLanguageCombo, CB_RESETCONTENT, 0, 0);
		// fill the combo box with the available languages
		if (m_pvecLanguages != NULL) 
		{
			std::vector<UADM_APP_LANGUAGE*>::iterator it;
			for (it = m_pvecLanguages->begin(); it != m_pvecLanguages->end(); it++)
			{
				UADM_APP_LANGUAGE* pLang = *it;
				if (pLang != NULL)
				{
					std::string strLang = pLang->strLanguage;
					std::string item = strLang + " - " + pLang->strLangName;
					SendMessage(hLanguageCombo, CB_ADDSTRING, 0, (LPARAM)item.c_str());
					if (strLang.compare(m_strLanguage) == 0)
					{
						// found the language, set the index
						int cnt = (int)SendMessage(hLanguageCombo, CB_GETCOUNT, 0, 0);
						m_intLanguage = cnt - 1; // last item
					}
				}
			}
		}
		int cnt = (int)SendMessage(hLanguageCombo, CB_GETCOUNT, 0, 0);
		if (cnt == 0)
		{
			// default it at least for english, if no language is provided
			SendMessage(hLanguageCombo, CB_ADDSTRING, 0, (LPARAM)"EN - English");
			m_intLanguage = 0;
			m_strLanguage = "EN";
		}
		SendMessage(hLanguageCombo, CB_SETCURSEL, m_intLanguage, 0); // preselect the first item	
	}

    void CUADMLoginDlg::OnChangeUADMLanguage()  
    {
		HWND hLanguageCombo = GetDlgItem(m_hDlg, IDC_CMB_UADM_LANGUAGE);
		if (hLanguageCombo == NULL)
		{
			return; // if the combo box is not available, do nothing
		}
		// get the selected language from the combo box
        int sel = (int)SendMessage(hLanguageCombo, CB_GETCURSEL, 0, 0);

		if (sel != CB_ERR)
		{
			m_intLanguage = 0;
			char buffer[256];
			SendMessage(hLanguageCombo, CB_GETLBTEXT, sel, (LPARAM)buffer);
			std::string strLang = buffer;
			if (!strLang.empty())
			{
				m_strLanguage = strLang.substr(0, 2); // get the first two characters (e.g., "EN", "DE", etc.)
				// set the internal language index based on the selected language, so that the dialog can be translated immediately
				if (m_strLanguage.compare("DE") == 0)
				{
					m_intLanguage = 1; // German
				}
				else if (m_strLanguage.compare("FR") == 0)
				{
					m_intLanguage = 2; // French
				}
				else if (m_strLanguage.compare("BG") == 0)
				{
					m_intLanguage = 3; // Bulgarian
				}
				else if (m_strLanguage.compare("HR") == 0)
				{
					m_intLanguage = 4; // Croatian
				}
			}
		}
		SetWindowText(m_hDlg, (TranslateText(IDS_UADM_LOGIN_TITLE_EN) + " - " + m_strAppName).c_str());
		//SetDlgItemText(m_hDlg, IDD_UADM_LOGIN, (TranslateText(IDS_UADM_LOGIN_TITLE_EN) + " - " + m_strAppName).c_str());
		SetDlgItemText(m_hDlg, IDC_LBL_UADM_USERNAME, TranslateText(IDS_UADM_USERNAME_EN).c_str());
		SetDlgItemText(m_hDlg, IDC_LBL_UADM_PASSWORD, TranslateText(IDS_UADM_PASSWORD_EN).c_str());
		SetDlgItemText(m_hDlg, IDC_LBL_UADM_COUNTRY, TranslateText(IDS_UADM_COUNTRY_EN).c_str());
		SetDlgItemText(m_hDlg, IDC_LBL_UADM_REGIONBRANCH, TranslateText(IDS_UADM_REGIONBRANCH_EN).c_str());
		SetDlgItemText(m_hDlg, IDC_LBL_UADM_ENVIRONMENT, TranslateText(IDS_UADM_ENVIRONMENT_EN).c_str());
		SetDlgItemText(m_hDlg, IDC_LBL_UADM_LANGUAGE, TranslateText(IDS_UADM_LANGUAGE_EN).c_str());
		SetDlgItemText(m_hDlg, IDC_BTN_UADM_CHANGE_PWD, TranslateText(IDS_UADM_CHANGEPW_EN).c_str());
		SetDlgItemText(m_hDlg, IDC_BTN_UADM_OK, TranslateText(IDS_UADM_OK_EN).c_str());
		SetDlgItemText(m_hDlg, IDC_BTN_UADM_CANCEL, TranslateText(IDS_UADM_CANCEL_EN).c_str());

		fillUADMEnvironments();
		OnChangeUADMEnvironment();
		fillUADMCountries();
		OnChangeUADMCountry();
		fillUADMRegionBranches();
		OnChangeUADMRegionBranch();
		setStatus();
	}

	void CUADMLoginDlg::fillUADMEnvironments()
	{
		int intSel = 0; // reset to first item
		int intCnt = 0; // count the number of environments added (excluding "Please select")
		HWND hEnvironmentCombo = GetDlgItem(m_hDlg, IDC_CMB_UADM_ENVIRONMENT);
		if (hEnvironmentCombo == NULL)
		{
			return; // if the combo box is not available, do nothing
		}
		// reset the combo box
		SendMessage(hEnvironmentCombo, CB_RESETCONTENT, 0, 0);
		SendMessage(hEnvironmentCombo, CB_ADDSTRING, 0, (LPARAM)TranslateText(IDS_UADM_PLEASE_SELECT_EN).c_str());
		if (m_pvecEntries != NULL)
		{
			std::vector<UADM_APP_ENTRY*>::iterator it;
			for (it = m_pvecEntries->begin(); it != m_pvecEntries->end(); it++)
			{
				UADM_APP_ENTRY* pEntry = *it;
				if (pEntry != NULL)
				{
					std::string strEnv = pEntry->strEnvironment;
					std::string item = strEnv + " - " + getEnvironmentName(strEnv);
					if (FindComboBoxIndex(hEnvironmentCombo, item.c_str()) == CB_ERR) // check if the item not yet exists
					{
						// add the environment only if it is not already in the list
						SendMessage(hEnvironmentCombo, CB_ADDSTRING, 0, (LPARAM)item.c_str());
						intCnt++;
						if (strEnv.compare(m_strEnvironment) == 0)
						{
							// found the environment, set the index
							intSel = (int)SendMessage(hEnvironmentCombo, CB_GETCOUNT, 0, 0) -1; // last item
						}
					}
				}
			}
		}
		if ((intCnt == 0) && (m_pvecEnvironments != NULL))
		{
			// if no entries are provided, fill the environments from the environment list
			std::vector<UADM_APP_ENVIRONMENT*>::iterator it;
			for (it = m_pvecEnvironments->begin(); it != m_pvecEnvironments->end(); it++)
			{
				UADM_APP_ENVIRONMENT* pEnv = *it;
				if (pEnv != NULL)
				{
					std::string strEnv = pEnv->strEnvironment;
					std::string item = strEnv + " - " + pEnv->strEnvName;
					SendMessage(hEnvironmentCombo, CB_ADDSTRING, 0, (LPARAM)item.c_str());
					if (strEnv.compare(m_strEnvironment) == 0)
					{
						// found the environment, set the index
						intSel = (int)SendMessage(hEnvironmentCombo, CB_GETCOUNT, 0, 0) - 1; // last item
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

	void CUADMLoginDlg::OnChangeUADMEnvironment()
	{
		m_strEnvironment = "";
		HWND hEnvironmentCombo = GetDlgItem(m_hDlg, IDC_CMB_UADM_ENVIRONMENT);
		if (hEnvironmentCombo == NULL)
		{
			return; // if the combo box is not available, do nothing
		}
		// get the selected language from the combo box
		int sel = (int)SendMessage(hEnvironmentCombo, CB_GETCURSEL, 0, 0);

		if ((sel != CB_ERR) && (sel != 0)) // not "Please select"
		{
			char buffer[256];
			SendMessage(hEnvironmentCombo, CB_GETLBTEXT, sel, (LPARAM)buffer);
			std::string strEnv = buffer;
			if (strEnv.empty() == false)
			{
				m_strEnvironment = strEnv.substr(0, strEnv.find(" - ")); // get the first characters before the separator (e.g., "DEV - Development", etc.)
			}
		}
		fillUADMCountries(); // fill the countries based on the selected environment
		OnChangeUADMCountry();
		fillUADMRegionBranches();
		OnChangeUADMRegionBranch();
		setStatus();
	}

	void CUADMLoginDlg::fillUADMCountries()
	{
		int intSel = 0; // reset to first item
		HWND hCountryCombo = GetDlgItem(m_hDlg, IDC_CMB_UADM_COUNTRY);
		if (hCountryCombo == NULL)
		{
			return; // if the combo box is not available, do nothing
		}
		// reset the combo box
		SendMessage(hCountryCombo, CB_RESETCONTENT, 0, 0);
		SendMessage(hCountryCombo, CB_ADDSTRING, 0, (LPARAM)TranslateText(IDS_UADM_PLEASE_SELECT_EN).c_str());
		if (m_pvecEntries != NULL)
		{
			std::vector<UADM_APP_ENTRY*>::iterator it;
			for (it = m_pvecEntries->begin(); it != m_pvecEntries->end(); it++)
			{
				UADM_APP_ENTRY* pEntry = *it;
				if (pEntry != NULL)
				{
					std::string strEnv = pEntry->strEnvironment;
					std::string strCountry = pEntry->strCountry;
					if (strEnv.compare(m_strEnvironment) == 0)
					{
						std::string item = strCountry + " - " + getCountryName(strCountry.substr(0, 2));
						if (FindComboBoxIndex(hCountryCombo, item.c_str()) == CB_ERR) // check if the item not yet exists
						{
							SendMessage(hCountryCombo, CB_ADDSTRING, 0, (LPARAM)item.c_str());
							if (strCountry.compare(m_strCountry) == 0)
							{
								// found the country, set the index
								intSel = (int)SendMessage(hCountryCombo, CB_GETCOUNT, 0, 0) - 1; // last item
							}
						}
					}
				}
			}
		}
		if (intSel == 0)
		{
			// if no country is selected and the list contains only one entry beside the "Please select" entry, select that one
			int cnt = (int)SendMessage(hCountryCombo, CB_GETCOUNT, 0, 0);
			if (cnt == 2) // only "Please select" and one country
			{
				intSel = 1; // select the first and only country
			}
		}
		SendMessage(hCountryCombo, CB_SETCURSEL, intSel, 0); // preselect the matching item or first item if not found
	}

	void CUADMLoginDlg::OnChangeUADMCountry()
	{
		m_strCountry = "";
		HWND hCountryCombo = GetDlgItem(m_hDlg, IDC_CMB_UADM_COUNTRY);
		if (hCountryCombo == NULL)
		{
			return; // if the combo box is not available, do nothing
		}
		// get the selected language from the combo box
		int sel = (int)SendMessage(hCountryCombo, CB_GETCURSEL, 0, 0);

		if ((sel != CB_ERR) && (sel != 0)) // not "Please select"
		{
			char buffer[256];
			SendMessage(hCountryCombo, CB_GETLBTEXT, sel, (LPARAM)buffer);
			std::string strCountry = buffer;
			if (!strCountry.empty() == true)
			{
				m_strCountry = strCountry.substr(0, strCountry.find(" - ")); // get the first characters before the separator (e.g., "DE - Deutschland", etc.)
			}
		}
		fillUADMRegionBranches();
		OnChangeUADMRegionBranch();
		setStatus();
	}

	void CUADMLoginDlg::fillUADMRegionBranches()
	{
		int intSel = 0; // reset to first item
		HWND hRegionBranchCombo = GetDlgItem(m_hDlg, IDC_CMB_UADM_REGIONBRANCH);
		if (hRegionBranchCombo == NULL)
		{
			return; // if the combo box is not available, do nothing
		}
		// reset the combo box
		SendMessage(hRegionBranchCombo, CB_RESETCONTENT, 0, 0);
		SendMessage(hRegionBranchCombo, CB_ADDSTRING, 0, (LPARAM)TranslateText(IDS_UADM_PLEASE_SELECT_EN).c_str());
		if (m_pvecEntries != NULL)
		{
			std::vector<UADM_APP_ENTRY*>::iterator it;
			for (it = m_pvecEntries->begin(); it != m_pvecEntries->end(); it++)
			{
				UADM_APP_ENTRY* pEntry = *it;
				if (pEntry != NULL)
				{
					std::string strEnv = pEntry->strEnvironment;
					std::string strCountry = pEntry->strCountry;
					std::string strRegionBranchNo = pEntry->strRegionBranchNo;
					if ((strEnv.compare(m_strEnvironment) == 0) && (strCountry.compare(m_strCountry) == 0))
					{
						std::string item = strRegionBranchNo + " - " + pEntry->strRegionBranchName;
						if (FindComboBoxIndex(hRegionBranchCombo, item.c_str()) == CB_ERR) // check if the item not yet exists
						{
							SendMessage(hRegionBranchCombo, CB_ADDSTRING, 0, (LPARAM)item.c_str());
							if (strRegionBranchNo.compare(m_strRegionBranchNo) == 0)
							{
								// found the regionbranch, set the index
								intSel = (int)SendMessage(hRegionBranchCombo, CB_GETCOUNT, 0, 0) - 1; // last item
							}
						}
					}
				}
			}
		}
		if (intSel == 0)
		{
			// if no regionbranch is selected and the list contains only one entry beside the "Please select" entry, select that one
			int cnt = (int)SendMessage(hRegionBranchCombo, CB_GETCOUNT, 0, 0);
			if (cnt == 2) // only "Please select" and one regionbranch
			{
				intSel = 1; // select the first and only regionbranch
			}
		}
		SendMessage(hRegionBranchCombo, CB_SETCURSEL, intSel, 0); // preselect the matching item or first item if not found
	}

	void CUADMLoginDlg::OnChangeUADMRegionBranch()
	{
		m_strRegionBranchNo = "";
		HWND hRegionBranchCombo = GetDlgItem(m_hDlg, IDC_CMB_UADM_REGIONBRANCH);
		if (hRegionBranchCombo == NULL)
		{
			return; // if the combo box is not available, do nothing
		}
		// get the selected language from the combo box
		int sel = (int)SendMessage(hRegionBranchCombo, CB_GETCURSEL, 0, 0);

		if ((sel != CB_ERR) && (sel != 0)) // not "Please select"
		{
			char buffer[256];
			SendMessage(hRegionBranchCombo, CB_GETLBTEXT, sel, (LPARAM)buffer);
			std::string strRegionBranch = buffer;
			if (!strRegionBranch.empty() == true)
			{
				m_strRegionBranchNo = strRegionBranch.substr(0, strRegionBranch.find(" - ")); // get the first characters before the separator (e.g., "05 - Fürth", etc.)
			}
		}
		setStatus();
	}

	void CUADMLoginDlg::ReadLastSuccessfulLogin()
	{
		if (m_strAppKey.empty() == false)
		{
			// read the last successful login from registry if app key is not empty
			m_strUserName = readRegistry("Last_Username");
			m_strEnvironment = readRegistry("Last_Environment");
			m_strCountry = readRegistry("Last_Country");
			m_strRegionBranchNo = readRegistry("Last_RegionBranch");
			m_strLanguage = readRegistry("Last_Language");
			std::string strServer = readRegistry("Last_Server");
			std::string strDatabase = readRegistry("Last_Database");
			// compatibility to other apps
			if (m_strUserName.empty() == true)
			{
				m_strUserName = readRegistry("User"); // compatible to other apps
			}
			if (m_strUserName.empty() == true)
			{
				m_strUserName = readRegistry("user"); // compatible to other apps
			}
			if (m_strLanguage.empty() == true)
			{
				m_strLanguage = readRegistry("Language"); // compatible to other apps
			}
			if (m_strLanguage.empty() == true)
			{
				m_strLanguage = readRegistry("language"); // compatible to other apps
			}

			// scan all entries if server and database was found to identify environment, country and branch
			if ((m_pvecEntries != NULL) && ((m_strEnvironment.empty() == true) || (m_strCountry.empty() == true)) && (strServer.empty() == false) && (strDatabase.empty() == false))
			{
				std::vector<UADM_APP_ENTRY*>::iterator it;
				for (it = m_pvecEntries->begin(); it != m_pvecEntries->end(); it++)
				{
					UADM_APP_ENTRY* pEntry = *it;
					if (pEntry != NULL)
					{
						std::string strSrv = pEntry->strServer;
						std::string strDab = pEntry->strDatabase;
						if ((strServer.compare(strSrv) == 0) && (strDatabase.compare(strDab) == 0))
						{
							m_strEnvironment = pEntry->strEnvironment;
							m_strCountry = pEntry->strCountry;
							if (m_strRegionBranchNo.empty() == true)
							{
								m_strRegionBranchNo = pEntry->strRegionBranchNo; // only if not already set
							}
							break;
						}
					}
				}
			}
			// compatibility to other apps
			if (m_strRegionBranchNo.empty() == true)
			{
				std::string entryName = readRegistry("BranchName"); // compatible to other apps
				if (entryName.empty() == true)
				{
					entryName = readRegistry("branch"); // compatible to other apps
				}
				if ((m_pvecEntries != NULL) && (entryName.empty() == false))
				{
					std::vector<UADM_APP_ENTRY*>::iterator it;
					for (it = m_pvecEntries->begin(); it != m_pvecEntries->end(); it++)
					{
						UADM_APP_ENTRY* pEntry = *it;
						if (pEntry != NULL)
						{
							if (entryName.compare(pEntry->strEntryName) == 0)
							{
								m_strRegionBranchNo = pEntry->strRegionBranchNo;
								m_strEnvironment = pEntry->strEnvironment;
								m_strCountry = pEntry->strCountry;
								break;
							}
						}
					}
				}
			}
		}
		else
		{
			// read the last successful login from pploginprevious with global user and connection is established
			if (m_pobjInterface != NULL)
			{
				std::string prevUsername = "";
				std::string prevRegionBranchNo = "";
				std::string prevLanguage = "";
				std::string prevCountry = "";
				if (m_pobjInterface->getPreviousLoginData(prevUsername, prevCountry, prevRegionBranchNo, prevLanguage) == true)
				{
					m_strUserName = prevUsername;
					m_strEnvironment = m_pobjInterface->getEnvironment();
					m_strRegionBranchNo = prevRegionBranchNo;
					m_strLanguage = prevLanguage;
					m_strCountry = prevCountry;
					if ((m_strRegionBranchNo.empty() == false) && (m_strCountry.empty() == true))
					{
						// if only the regionbranch is provided, resolve the country via the interface
						m_strCountry = m_pobjInterface->resolveCountry(atoi(prevRegionBranchNo.c_str()));
					}
				}
			}
		}
	}

	void CUADMLoginDlg::setStatus()
	{
		if (m_strUserName.empty() == true)
		{
			SetDlgItemText(m_hDlg, IDC_LBL_UADM_STATUS, TranslateText(IDS_UADM_STATUS_USERNAME_EN).c_str());
			return;
		}
		if (m_strPassword.empty() == true)
		{
			SetDlgItemText(m_hDlg, IDC_LBL_UADM_STATUS, TranslateText(IDS_UADM_STATUS_PASSWORD_EN).c_str());
			return;
		}
		if (m_strEnvironment.empty() == true)
		{
			SetDlgItemText(m_hDlg, IDC_LBL_UADM_STATUS, TranslateText(IDS_UADM_STATUS_ENVIRONMENT_EN).c_str());
			return;
		}
		if (m_strCountry.empty() == true)
		{
			SetDlgItemText(m_hDlg, IDC_LBL_UADM_STATUS, TranslateText(IDS_UADM_STATUS_COUNTRY_EN).c_str());
			return;
		}
		if (m_strRegionBranchNo.empty() == true)
		{
			SetDlgItemText(m_hDlg, IDC_LBL_UADM_STATUS, TranslateText(IDS_UADM_STATUS_REGIONBRANCH_EN).c_str());
			return;
		}
	}

	void CUADMLoginDlg::SaveLastSuccessfulLogin(UADM_APP_ENTRY* pEntry)
	{
		if (pEntry == NULL)
		{
			return; // no entry provided, nothing to save
		}
		if (m_strAppKey.empty() == false)
		{
			// save the last successful login to registry
			writeRegistry("Last_Username", m_strUserName);
			writeRegistry("Last_Environment", pEntry->strEnvironment);
			writeRegistry("Last_Country", pEntry->strCountry);
			writeRegistry("Last_RegionBranch", pEntry->strRegionBranchNo);
			writeRegistry("Last_Language", m_strLanguage);
			writeRegistry("Last_Server", pEntry->strServer);
			writeRegistry("Last_Database", pEntry->strDatabase);
		}
	}

	std::string CUADMLoginDlg::TranslateText(int langIDText)
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

	std::string CUADMLoginDlg::getEnvironmentName(const std::string& strEnv)
	{
		std::string ret = "??";

		std::vector<UADM_APP_ENVIRONMENT*>::iterator it;
		for (it = m_pvecEnvironments->begin(); it != m_pvecEnvironments->end(); it++)
		{
			UADM_APP_ENVIRONMENT* pEnv = *it;
			if (pEnv != NULL)
			{
				if (pEnv->strEnvironment == strEnv)
				{
					// found the environment, return the name
					ret = pEnv->strEnvName;
					break;
				}
			}
		}
		return ret;
	}

	UADM_APP_ENVIRONMENT* CUADMLoginDlg::getEnvironment(const std::string& strEnv)
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

	UADM_APP_ENTRY* CUADMLoginDlg::getEntry(const std::string& strEnv, const std::string& strCountry, const std::string& strRegionBranchNo)
	{
		UADM_APP_ENTRY* ret = NULL;

		std::vector<UADM_APP_ENTRY*>::iterator it;
		for (it = m_pvecEntries->begin(); it != m_pvecEntries->end(); it++)
		{
			UADM_APP_ENTRY* pEntry = *it;
			if (pEntry != NULL)
			{
				if ((pEntry->strEnvironment == strEnv) && 
					(pEntry->strCountry == strCountry) && 
					(pEntry->strRegionBranchNo == strRegionBranchNo))
				{
					// found the environment, return the entry
					ret = pEntry;
					break;
				}
			}
		}
		return ret;
	}

	std::string CUADMLoginDlg::getCountryName(const std::string& strCountry)
	{
		std::string ret = "??";
		if (strCountry == "DE")
		{
			ret = TranslateText(IDS_UADM_COUNTRY_GERMANY_EN);
		}
		else if (strCountry == "AT")
		{
			ret = TranslateText(IDS_UADM_COUNTRY_AUSTRIA_EN);
		}
		else if (strCountry == "FR")
		{
			ret = TranslateText(IDS_UADM_COUNTRY_FRANCE_EN);
		}
		else if (strCountry == "CH")
		{
			ret = TranslateText(IDS_UADM_COUNTRY_SWITZERLAND_EN);
		}
		else if (strCountry == "BG")
		{
			ret = TranslateText(IDS_UADM_COUNTRY_BULGARIA_EN);
		}
		else if (strCountry == "HR")
		{
			ret = TranslateText(IDS_UADM_COUNTRY_CROATIA_EN);
		}
		else if (strCountry == "RS")
		{
			ret = TranslateText(IDS_UADM_COUNTRY_SERBIA_EN);
		}
		else if (strCountry == "WW")
		{
			ret = TranslateText(IDS_UADM_COUNTRY_GLOBAL_EN);
		}
		return ret;
	}

	std::string CUADMLoginDlg::readRegistry(const std::string& strRegistryKey)
	{
		std::string ret = "";

		long		lngResult = 0;
		HKEY		hKey = NULL;

		lngResult = RegOpenKeyEx(HKEY_CURRENT_USER, m_strAppKey.c_str(), 0, KEY_READ, &hKey);

		if ((hKey != NULL) && (lngResult == ERROR_SUCCESS))
		{
			DWORD dwType;
			TCHAR szValue[256];
			DWORD dwSize = sizeof(szValue) * sizeof(TCHAR);

			lngResult = RegQueryValueEx(hKey, strRegistryKey.c_str(), NULL, &dwType, (LPBYTE)szValue, &dwSize);

			if ((lngResult == ERROR_SUCCESS) && (dwType == REG_SZ))
			{
				ret = std::string(szValue);
			}
		}
		RegCloseKey(hKey);

		return ret;
	}

	bool CUADMLoginDlg::writeRegistry(const std::string& strRegistryKey, const std::string& strValue)
	{
		long		lnResult;
		HKEY		hKey = NULL;

		// try to open existing registry key
		lnResult = RegOpenKeyEx(HKEY_CURRENT_USER, m_strAppKey.c_str(), 0, KEY_WRITE, &hKey);

		if (!hKey || lnResult != ERROR_SUCCESS)
		{
			// if the key does not exist, create it
			lnResult = RegCreateKeyEx(HKEY_CURRENT_USER, m_strAppKey.c_str(), 0, (LPSTR)"", 0, KEY_WRITE, NULL, &hKey, NULL);
		}
		// fails, so return false
		if (!hKey || lnResult != ERROR_SUCCESS)
		{
			RegCloseKey(hKey);
			return false;
		}

		// set the value in the registry
		lnResult = RegSetValueEx(hKey, strRegistryKey.c_str(), 0, REG_SZ, (LPBYTE)strValue.c_str(), (DWORD)strValue.length());
		RegCloseKey(hKey);

		return true;
	}
}
/////////////////////////////////////////////////////////////////////////////