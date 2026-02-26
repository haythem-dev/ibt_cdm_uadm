
// UADMLoginTesterDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "UADMLoginTester.h"
#include "UADMLoginTesterDlg.h"
#include "afxdialogex.h"
#include "UADMLoginManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CUADMLoginTesterDlg dialog
CUADMLoginTesterDlg::CUADMLoginTesterDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_UADMLOGINTESTER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_strAppID = "";
}

void CUADMLoginTesterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PERMISSION, m_lstPermissions);
	DDX_Control(pDX, IDC_LIST_PARAMETER, m_lstParameters);
	DDX_Control(pDX, IDC_LIST_USERMAPPINGS, m_lstUserMappings);
	DDX_Control(pDX, IDC_LIST_DEBUGINFO, m_lstDebugInfo);
	DDX_Control(pDX, IDC_CHK_ACTIVE, m_chkActive);
	DDX_Control(pDX, IDC_CHK_PWCHANGE, m_chkPWChange);
	DDX_Control(pDX, IDC_CHK_REGIONBRANCHONLY, m_chkRegionBranchOnly);
	DDX_Control(pDX, IDC_CHK_USERMAPPING, m_chkUserMapping);
	DDX_Control(pDX, IDC_CHK_USERONLY, m_chkUserOnly);
	DDX_Control(pDX, IDC_CHK_USERALL, m_chkUserAll);
	DDX_Control(pDX, IDC_CHK_PREFVOODOO, m_chkPrefVoodoo);
	DDX_Control(pDX, IDC_CHK_TERMINALID, m_chkTerminalID);
}

BEGIN_MESSAGE_MAP(CUADMLoginTesterDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_LOGINWITHDLG, &CUADMLoginTesterDlg::OnLoginWithDialog)
	ON_BN_CLICKED(IDC_BTN_LOGINWODLG, &CUADMLoginTesterDlg::OnLoginWithoutDialog)
END_MESSAGE_MAP()


// CUADMLoginTesterDlg message handlers

BOOL CUADMLoginTesterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// Initialize the file list control
	m_lstPermissions.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_lstPermissions.InsertColumn(0, _T("UserID"), LVCFMT_LEFT, 60);
	m_lstPermissions.InsertColumn(1, _T("GroupID"), LVCFMT_LEFT, 60);
	m_lstPermissions.InsertColumn(2, _T("AppID"), LVCFMT_LEFT, 50);
	m_lstPermissions.InsertColumn(3, _T("AppName"), LVCFMT_LEFT, 75);
	m_lstPermissions.InsertColumn(4, _T("Module"), LVCFMT_RIGHT, 55);
	m_lstPermissions.InsertColumn(5, _T("Right"), LVCFMT_LEFT, 60);
	m_lstPermissions.InsertColumn(6, _T("Area"), LVCFMT_LEFT, 60);

	m_lstParameters.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_lstParameters.InsertColumn(0, _T("AppID"), LVCFMT_LEFT, 60);
	m_lstParameters.InsertColumn(1, _T("Para1"), LVCFMT_LEFT, 60);
	m_lstParameters.InsertColumn(2, _T("Para2"), LVCFMT_LEFT, 60);
	m_lstParameters.InsertColumn(3, _T("ParameterName"), LVCFMT_LEFT, 120);
	m_lstParameters.InsertColumn(4, _T("ParameterValue"), LVCFMT_LEFT, 120);

	m_lstUserMappings.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_lstUserMappings.InsertColumn(0, _T("UserID"), LVCFMT_LEFT, 50);
	m_lstUserMappings.InsertColumn(1, _T("Username"), LVCFMT_LEFT, 70);
	m_lstUserMappings.InsertColumn(2, _T("Branch"), LVCFMT_LEFT, 50);
	m_lstUserMappings.InsertColumn(3, _T("ParaID"), LVCFMT_LEFT, 50);
	m_lstUserMappings.InsertColumn(4, _T("ParaName"), LVCFMT_LEFT, 80);
	m_lstUserMappings.InsertColumn(5, _T("ParaValue"), LVCFMT_LEFT, 80);

	m_chkRegionBranchOnly.SetCheck(1);
	// define the INI File
	char appPath[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, appPath, sizeof(appPath));
	PathRemoveFileSpec(appPath);

	CString appName = "UADMLoginTester";
	CString iniFile(appPath);
	iniFile.Append("\\");
	iniFile.Append(appName);
	iniFile.Append(".ini");
	CString regKey = "Software\\" + appName;

	appName = "ORDER_ENTRY";
	iniFile = "C:\\dev\\iam\\pharmos.outbound.csc_core_applications\\dev\\src\\ae\\src\\ae\\DebugHipathNew\\KSC.ini";
	regKey = "Software\\PRZ\\Phosix";

	// Write the default entries on dialog
	GetDlgItem(IDC_TXT_APPVERSION)->SetWindowText("1.0.0");
	GetDlgItem(IDC_TXT_APPNAME)->SetWindowText(appName);
	GetDlgItem(IDC_TXT_INIFILE)->SetWindowText(iniFile);
	GetDlgItem(IDC_TXT_REGKEY)->SetWindowText(regKey);

	GetDlgItem(IDC_TXT_ENV)->SetWindowText("DEV");
	GetDlgItem(IDC_TXT_ENVNAME)->SetWindowText("Development");
	GetDlgItem(IDC_TXT_PRIMHOST)->SetWindowText("zdev21_tcp");
	GetDlgItem(IDC_TXT_PRIMDB)->SetWindowText("zpcua1");
	GetDlgItem(IDC_TXT_SECHOST)->SetWindowText("zdev21_tcp");
	GetDlgItem(IDC_TXT_SECDB)->SetWindowText("zpcua2");
	//m_chkPrefVoodoo.SetCheck(1);
	//GetDlgItem(IDC_TXT_VDSERVICE)->SetWindowText("loguiserv");
	//GetDlgItem(IDC_TXT_VDPORT)->SetWindowText("58001");

	GetDlgItem(IDC_TXT_PERMCOUNT)->SetWindowText("0 Entries");
	GetDlgItem(IDC_TXT_PARACOUNT)->SetWindowText("0 Entries");
	GetDlgItem(IDC_TXT_USERMAPCOUNT)->SetWindowText("0 Entries");

#ifdef COMPILE32
	m_strTitle = "UADM Login Tester - 32 Bit";
#else
	m_strTitle = "UADM Login Tester - 64 Bit";
#endif
	SetWindowText(m_strTitle);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CUADMLoginTesterDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CUADMLoginTesterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CUADMLoginTesterDlg::OnLoginWithDialog()
{
	// Clear the permissions list
	m_lstPermissions.DeleteAllItems();
	GetDlgItem(IDC_TXT_PERMCOUNT)->SetWindowText("0 Entries");
	// Clear the parameters list
	m_lstParameters.DeleteAllItems();
	GetDlgItem(IDC_TXT_PARACOUNT)->SetWindowText("0 Entries");
	// Clear the user mappings list
	m_lstUserMappings.DeleteAllItems();
	GetDlgItem(IDC_TXT_USERMAPCOUNT)->SetWindowText("0 Entries");
	// clear the debug info list
	m_lstDebugInfo.ResetContent();
	// clear app
	m_strAppID = "";

	// get name, inifile and regkey from dialog
	CString appName, appVersion, appDesc, iniFile, regKey;
	GetDlgItem(IDC_TXT_APPNAME)->GetWindowText(appName);
	GetDlgItem(IDC_TXT_APPVERSION)->GetWindowText(appVersion);
	GetDlgItem(IDC_TXT_INIFILE)->GetWindowText(iniFile);
	GetDlgItem(IDC_TXT_REGKEY)->GetWindowText(regKey);

	// get the uadm connection parameters from dialog
	CString env, envName, primHost, primDB, secHost, secDB, vdService, vdPort;
	GetDlgItem(IDC_TXT_ENV)->GetWindowText(env);
	GetDlgItem(IDC_TXT_ENVNAME)->GetWindowText(envName);
	GetDlgItem(IDC_TXT_PRIMHOST)->GetWindowText(primHost);
	GetDlgItem(IDC_TXT_PRIMDB)->GetWindowText(primDB);
	GetDlgItem(IDC_TXT_SECHOST)->GetWindowText(secHost);
	GetDlgItem(IDC_TXT_SECDB)->GetWindowText(secDB);
	GetDlgItem(IDC_TXT_VDSERVICE)->GetWindowText(vdService);
	GetDlgItem(IDC_TXT_VDPORT)->GetWindowText(vdPort);
	// Initialize the login manager
	UADM::CUADMLoginManager mgr;

	if (appName.IsEmpty() == true)
	{
		MessageBox("Application name is empty, cannot continue.");
		return;
	}
	// set the preferred connection type before initialization
	if (m_chkPrefVoodoo.GetCheck() == 1)
	{
		mgr.setConnectionTypePreference(1);
	}
	else
	{
		mgr.setConnectionTypePreference(0);
	}
	// first try to initialize with INI File and Reg Key, if given
	bool status = false;
	if (iniFile.IsEmpty() == false)
	{
		status = mgr.initialize(_T(iniFile.GetString()), regKey.GetString(), appName.GetString(), appVersion.GetString());
		if ( status == false)
		{
			CString s;
			s.Format("Cannot initialize the login manager with INI File '%s' and Registry Key '%s', %s", iniFile.GetString(), regKey.GetString(), mgr.getLastErrorMessage().c_str());
			MessageBox(s);
			showDebugInfos(mgr.getDebugInfos());
		}
	}
	// if not given or successful, try with given connection parameters
	if (status == false)
	{
		status = mgr.initialize(env.GetString(), envName.GetString(), primHost.GetString(), primDB.GetString(), secHost.GetString(), secDB.GetString(), vdService.GetString(), vdPort.GetString(), appName.GetString(), appVersion.GetString());
		if (status == false)
		{
			CString s;
			s.Format("Cannot initialize the login manager with environment '%s', %s", env.GetString(), mgr.getLastErrorMessage().c_str());
			MessageBox(s);
			showDebugInfos(mgr.getDebugInfos());
			return;
		}
	}
	// set the login mode for multiple region/branch checking
	mgr.setLoginMode(m_chkRegionBranchOnly.GetCheck() == 1, m_chkUserMapping.GetCheck() == 1);

	// set a background image for the login dialog
	char appPath[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, appPath, sizeof(appPath));
	PathRemoveFileSpec(appPath);
	CString strBack(appPath);
	strBack.Append("\\UADMLogin.bmp");

	HWND hwnd = m_hWnd;
	bool ret = mgr.loginWithDialog(strBack.GetString(), hwnd);

	if (ret == false)
	{
		CString s;
		s.Format("Login failed for User %s in environment %s for application %s in region/branch %d, %s", mgr.getUsername().c_str(), mgr.getEnvironment().c_str(), mgr.getAppName().c_str(), mgr.getRegionBranchNoVal(), mgr.getLastErrorMessage().c_str());
		MessageBox(s);
	}
	else
	{
		// only if login was successful, get the permissions, parameters and user mappings
		if (m_chkUserMapping.GetCheck() == 1)
		{
			// first check if terminal id should be generated
			if (m_chkTerminalID.GetCheck() == 1)
			{
				// first check if terminal ID is already there for this user
				int userid = mgr.getUserID();
				std::string username = ""; // not needed for this mode
				int branch = mgr.getRegionBranchNoVal();
				std::string branchuserid = "";  // not needed for this mode
				std::string branchusername = ""; // not needed for this mode
				std::string terminalid = ""; // to be filled by the function
				std::string dummy2 = ""; // not needed for this mode
				mgr.getUserMapping(false, userid, username, branch, branchuserid, branchusername, terminalid, dummy2);
				if (terminalid.empty() == true)
				{
					// generate a new terminal ID
					terminalid = "*"; // will force to generate a new terminal ID
					if (mgr.setUserMapping(userid,branch,"TerminalID", terminalid) == false)
					{
						CString s;
						s.Format("Generating terminal ID for user %s in environment %s for application %s in region/branch %d failed, %s", mgr.getUsername().c_str(), mgr.getEnvironment().c_str(), mgr.getAppName().c_str(), mgr.getRegionBranchNoVal(), mgr.getLastErrorMessage().c_str());
						MessageBox(s);
					}
				}
			}
			// now check if all user mappings should be loaded
			if ((m_chkUserOnly.GetCheck() == 1) && (m_chkUserAll.GetCheck() == 1))
			{
				int userid = 0;
				std::string username = "";
				int branch = mgr.getRegionBranchNoVal();
				std::string branchuserid = "-1"; // force to load all mappings
				std::string branchusername = "";
				std::string dummy1 = ""; // not needed for this mode
				std::string dummy2 = ""; // not needed for this mode
				if (mgr.getUserMapping(true, userid, username, branch, branchuserid, branchusername, dummy1, dummy2) == false)
				{
					CString s;
					s.Format("Getting user mappings in environment %s for application %s in region/branch %d, %s", mgr.getEnvironment().c_str(), mgr.getAppName().c_str(), mgr.getRegionBranchNoVal(), mgr.getLastErrorMessage().c_str());
					MessageBox(s);
				}
			}
		}
	}
	m_strAppID.Format("%d", mgr.getAppID());
	appName = mgr.getAppName().c_str();
	appDesc = mgr.getAppDesc().c_str();

	SetWindowText(m_strTitle + " - " + appName + "(" + appDesc + ") in " + mgr.getRegionBranchName().c_str() + " on " + mgr.getEnvironment().c_str());
	// Fill the dialog controls with the user information
	CString tmp;
	tmp.Format("%d", mgr.getUserID());
	GetDlgItem(IDC_TXT_USERID)->SetWindowText(tmp);
	GetDlgItem(IDC_TXT_USERNAME)->SetWindowText(mgr.getUsername().c_str());
	GetDlgItem(IDC_TXT_DOMAINNAME)->SetWindowText(mgr.getDomainAccount().c_str());
	GetDlgItem(IDC_TXT_FIRSTNAME)->SetWindowText(mgr.getFirstname().c_str());
	GetDlgItem(IDC_TXT_LASTNAME)->SetWindowText(mgr.getLastname().c_str());
	GetDlgItem(IDC_TXT_EMAIL)->SetWindowText(mgr.getEMail().c_str());
	tmp.Format("%ld", mgr.getLastLoginDate());
	GetDlgItem(IDC_TXT_LASTLOGIN)->SetWindowText(tmp);
	tmp.Format("%ld", mgr.getLastPasswordChangeDate());
	GetDlgItem(IDC_TXT_LASTPWCHANGE)->SetWindowText(tmp);

	if (mgr.isUserActive() == true)
	{
		m_chkActive.SetCheck(1);
	}
	if (mgr.isPasswordChangeRequired() == true)
	{
		m_chkPWChange.SetCheck(1);
	}
	// fill the uadm and connection information
	GetDlgItem(IDC_TXT_UADMENV)->SetWindowText(mgr.getEnvironment().c_str());
	GetDlgItem(IDC_TXT_UADMCONN)->SetWindowText(mgr.getUsedUADMConnectionType().c_str());
	GetDlgItem(IDC_TXT_UADMSERVER)->SetWindowText(mgr.getUsedUADMServername().c_str());
	GetDlgItem(IDC_TXT_UADMDB)->SetWindowText(mgr.getUsedUADMDatabase().c_str());
	GetDlgItem(IDC_TXT_UADMSERVICEPORT)->SetWindowText(mgr.getUsedUADMPort().c_str());
	GetDlgItem(IDC_TXT_COUNTRY)->SetWindowText(mgr.getCountry().c_str());
	GetDlgItem(IDC_TXT_REGIONBRANCH)->SetWindowText((mgr.getRegionBranchNo() + " - " + mgr.getRegionBranchName()).c_str());
	GetDlgItem(IDC_TXT_LANGUAGE)->SetWindowText((mgr.getLanguage() + " - " + mgr.getLanguageName()).c_str());
	GetDlgItem(IDC_TXT_SERVER)->SetWindowText(mgr.getServername().c_str());
	GetDlgItem(IDC_TXT_DATABASE)->SetWindowText(mgr.getDatabase().c_str());
	GetDlgItem(IDC_TXT_SERVICEPORT)->SetWindowText((mgr.getServiceName() + ":" + mgr.getPortNo()).c_str());
	GetDlgItem(IDC_TXT_ENCTYPE)->SetWindowText(mgr.getUsedEncryptionType().c_str());

	// Fill the permissions list
	std::vector<UADM::UADM_APP_PERMISSION*>* pVec = mgr.getPermissions();
	int cnt = 0;
	if (pVec != NULL)
	{
		cnt = (int)pVec->size();
		if (cnt > 0)
		{
			std::vector<UADM::UADM_APP_PERMISSION*>::iterator it;
			for (it = pVec->begin(); it != pVec->end(); it++)
			{
				UADM::UADM_APP_PERMISSION* pPerm = *it;
				if (pPerm != NULL)
				{
					CString strTmp;
					strTmp.Format("%d", pPerm->intUserID);
					int iItem = m_lstPermissions.InsertItem(m_lstPermissions.GetItemCount(), strTmp);
					strTmp.Format("%d", pPerm->intGroupID);
					m_lstPermissions.SetItemText(iItem, 1, strTmp);
					strTmp.Format("%d", pPerm->intAppID);
					m_lstPermissions.SetItemText(iItem, 2, strTmp);
					m_lstPermissions.SetItemText(iItem, 3, appName.MakeUpper());
					strTmp.Format("%d", pPerm->intModuleID);
					m_lstPermissions.SetItemText(iItem, 4, strTmp);
					m_lstPermissions.SetItemText(iItem, 5, pPerm->strRight.c_str());
					strTmp.Format("%d", pPerm->intRegionBranch);
					m_lstPermissions.SetItemText(iItem, 6, strTmp);
				}
			}
		}
	}
	if (cnt == 0)
	{
		int iItem = m_lstPermissions.InsertItem(m_lstPermissions.GetItemCount(), "-none-");
	}
	CString strTmp;
	strTmp.Format("%d Entries", cnt);
	GetDlgItem(IDC_TXT_PERMCOUNT)->SetWindowText(strTmp);
	// Fill the parameters list
	std::vector<UADM::UADM_APP_PARAMETER*>* pPars = mgr.getParameters();
	cnt = 0;
	if (pPars != NULL)
	{
		cnt = (int)pPars->size();
		if (cnt > 0)
		{
			std::vector<UADM::UADM_APP_PARAMETER*>::iterator it;
			for (it = pPars->begin(); it != pPars->end(); it++)
			{
				UADM::UADM_APP_PARAMETER* pPara = *it;
				if (pPara != NULL)
				{
					CString strTmp;
					strTmp.Format("%d", pPara->intAppID);
					int iItem = m_lstParameters.InsertItem(m_lstParameters.GetItemCount(), strTmp);
					m_lstParameters.SetItemText(iItem, 1, pPara->strPara1.c_str());
					m_lstParameters.SetItemText(iItem, 2, pPara->strPara2.c_str());
					m_lstParameters.SetItemText(iItem, 3, pPara->strParaName.c_str());
					m_lstParameters.SetItemText(iItem, 4, pPara->strParaValue.c_str());
				}
			}
		}
	}
	if (cnt == 0)
	{
		int iItem = m_lstParameters.InsertItem(m_lstParameters.GetItemCount(), "-none-");
	}
	strTmp.Format("%d Entries", cnt);
	GetDlgItem(IDC_TXT_PARACOUNT)->SetWindowText(strTmp);
	// Fill the user mappings list
	std::vector<UADM::UADM_USER_MAPPING*>* pMappings = mgr.getUserMappings();
	cnt = 0;
	if (pMappings != NULL)
	{
		cnt = (int)pMappings->size();
		if (cnt > 0)
		{
			std::vector<UADM::UADM_USER_MAPPING*>::iterator it;
			for (it = pMappings->begin(); it != pMappings->end(); it++)
			{
				UADM::UADM_USER_MAPPING* pMapping = *it;
				if (pMapping != NULL)
				{
					CString strTmp;
					strTmp.Format("%d", pMapping->intUserID);
					int iItem = m_lstUserMappings.InsertItem(m_lstUserMappings.GetItemCount(), strTmp);
					m_lstUserMappings.SetItemText(iItem, 1, pMapping->strUsername.c_str());
					strTmp.Format("%d", pMapping->intRegionBranchID);
					m_lstUserMappings.SetItemText(iItem, 2, strTmp);
					strTmp.Format("%d", pMapping->intUserParaID);
					m_lstUserMappings.SetItemText(iItem, 3, strTmp);
					m_lstUserMappings.SetItemText(iItem, 4, pMapping->strUserParaName.c_str());
					m_lstUserMappings.SetItemText(iItem, 5, pMapping->strUserParaValue.c_str());
				}
			}
		}
	}
	if (cnt == 0)
	{
		int iItem = m_lstUserMappings.InsertItem(m_lstUserMappings.GetItemCount(), "-none-");
	}
	strTmp.Format("%d Entries", cnt);
	GetDlgItem(IDC_TXT_USERMAPCOUNT)->SetWindowText(strTmp);

	// fill the debug info list
	showDebugInfos(mgr.getDebugInfos());
}

void CUADMLoginTesterDlg::OnLoginWithoutDialog()
{
	// Clear the permissions list
	m_lstPermissions.DeleteAllItems();
	GetDlgItem(IDC_TXT_PERMCOUNT)->SetWindowText("0 Entries");
	// Clear the parameters list
	m_lstParameters.DeleteAllItems();
	GetDlgItem(IDC_TXT_PARACOUNT)->SetWindowText("0 Entries");
	// clear the debug info list
	m_lstDebugInfo.ResetContent();
	// clear app
	m_strAppID = "";

	// get name, inifile 
	CString appName, appVersion;
	GetDlgItem(IDC_TXT_APPNAME)->GetWindowText(appName);
	GetDlgItem(IDC_TXT_APPVERSION)->GetWindowText(appVersion);

	// get the uadm connection parameters from dialog
	CString env, envName, primHost, primDB, secHost, secDB, vdService, vdPort;
	GetDlgItem(IDC_TXT_ENV)->GetWindowText(env);
	GetDlgItem(IDC_TXT_ENVNAME)->GetWindowText(envName);
	GetDlgItem(IDC_TXT_PRIMHOST)->GetWindowText(primHost);
	GetDlgItem(IDC_TXT_PRIMDB)->GetWindowText(primDB);
	GetDlgItem(IDC_TXT_SECHOST)->GetWindowText(secHost);
	GetDlgItem(IDC_TXT_SECDB)->GetWindowText(secDB);
	GetDlgItem(IDC_TXT_VDSERVICE)->GetWindowText(vdService);
	GetDlgItem(IDC_TXT_VDPORT)->GetWindowText(vdPort);

	// get the username password and region/branch from dialog
	CString username, password, regbranch;
	GetDlgItem(IDC_TXT_LOGINUSER)->GetWindowText(username);
	GetDlgItem(IDC_TXT_LOGINPASS)->GetWindowText(password);
	GetDlgItem(IDC_TXT_LOGINREGIONBRANCH)->GetWindowText(regbranch);

	// Initialize the login manager
	UADM::CUADMLoginManager mgr;

	if (appName.IsEmpty() == true)
	{
		MessageBox("Application name is empty, cannot continue.");
		return;
	}
	if ((username.IsEmpty() == true) && (password.IsEmpty() == true))
	{
		MessageBox("Username and Password are empty, cannot continue.");
		return;
	}
	bool status = false;
	status = mgr.initialize(env.GetString(), envName.GetString(), primHost.GetString(), primDB.GetString(), secHost.GetString(), secDB.GetString(), vdService.GetString(), vdPort.GetString(), appName.GetString(), appVersion.GetString());
	if (status == false)
	{
		CString s;
		s.Format("Cannot initialize the login manager with environment '%s', %s", env.GetString(), mgr.getLastErrorMessage().c_str());
		MessageBox(s);
		showDebugInfos(mgr.getDebugInfos());
		return;
	}
	mgr.setLoginMode(m_chkRegionBranchOnly.GetCheck() == 1);

	if (mgr.loginWithoutDialog(username.GetString(), password.GetString(), "", atoi(regbranch.GetString()), "DE") == false)
	{
		CString s;
		s.Format("Login failed for User %s in environment %s for application %s in region/branch %d, %s", mgr.getUsername().c_str(), mgr.getEnvironment().c_str(), mgr.getAppName().c_str(), mgr.getRegionBranchNoVal(), mgr.getLastErrorMessage().c_str());
		MessageBox(s);
	}
	m_strAppID.Format("%d", mgr.getAppID());
	appName = mgr.getAppName().c_str();

	SetWindowText(m_strTitle + " - " + appName + " in " + mgr.getRegionBranchName().c_str() + " on " + mgr.getEnvironment().c_str());
	// Fill the dialog controls with the user information
	CString tmp;
	tmp.Format("%d", mgr.getUserID());
	GetDlgItem(IDC_TXT_USERID)->SetWindowText(tmp);
	GetDlgItem(IDC_TXT_USERNAME)->SetWindowText(mgr.getUsername().c_str());
	GetDlgItem(IDC_TXT_DOMAINNAME)->SetWindowText(mgr.getDomainAccount().c_str());
	GetDlgItem(IDC_TXT_FIRSTNAME)->SetWindowText(mgr.getFirstname().c_str());
	GetDlgItem(IDC_TXT_LASTNAME)->SetWindowText(mgr.getLastname().c_str());
	GetDlgItem(IDC_TXT_EMAIL)->SetWindowText(mgr.getEMail().c_str());
	tmp.Format("%ld", mgr.getLastLoginDate());
	GetDlgItem(IDC_TXT_LASTLOGIN)->SetWindowText(tmp);
	tmp.Format("%ld", mgr.getLastPasswordChangeDate());
	GetDlgItem(IDC_TXT_LASTPWCHANGE)->SetWindowText(tmp);

	if (mgr.isUserActive() == true)
	{
		m_chkActive.SetCheck(1);
	}
	if (mgr.isPasswordChangeRequired() == true)
	{
		m_chkPWChange.SetCheck(1);
	}
	// fill the uadm and connection information
	GetDlgItem(IDC_TXT_UADMENV)->SetWindowText(mgr.getEnvironment().c_str());
	GetDlgItem(IDC_TXT_UADMCONN)->SetWindowText(mgr.getUsedUADMConnectionType().c_str());
	GetDlgItem(IDC_TXT_UADMSERVER)->SetWindowText(mgr.getUsedUADMServername().c_str());
	GetDlgItem(IDC_TXT_UADMDB)->SetWindowText(mgr.getUsedUADMDatabase().c_str());
	GetDlgItem(IDC_TXT_UADMSERVICEPORT)->SetWindowText(mgr.getUsedUADMPort().c_str());
	GetDlgItem(IDC_TXT_COUNTRY)->SetWindowText(mgr.getCountry().c_str());
	GetDlgItem(IDC_TXT_REGIONBRANCH)->SetWindowText((mgr.getRegionBranchNo() + " - " + mgr.getRegionBranchName()).c_str());
	GetDlgItem(IDC_TXT_LANGUAGE)->SetWindowText((mgr.getLanguage() + " - " + mgr.getLanguageName()).c_str());
	GetDlgItem(IDC_TXT_SERVER)->SetWindowText(mgr.getServername().c_str());
	GetDlgItem(IDC_TXT_DATABASE)->SetWindowText(mgr.getDatabase().c_str());
	GetDlgItem(IDC_TXT_SERVICEPORT)->SetWindowText((mgr.getServiceName() + ":" + mgr.getPortNo()).c_str());
	GetDlgItem(IDC_TXT_ENCTYPE)->SetWindowText(mgr.getUsedEncryptionType().c_str());

	// Fill the permissions and parameters lists
	std::vector<UADM::UADM_APP_PERMISSION*>* pVec = mgr.getPermissions();
	int cnt = 0;
	if (pVec != NULL)
	{
		cnt = (int)pVec->size();
		if (cnt > 0)
		{
			std::vector<UADM::UADM_APP_PERMISSION*>::iterator it;
			for (it = pVec->begin(); it != pVec->end(); it++)
			{
				UADM::UADM_APP_PERMISSION* pPerm = *it;
				if (pPerm != NULL)
				{
					CString strTmp;
					strTmp.Format("%d", pPerm->intUserID);
					int iItem = m_lstPermissions.InsertItem(m_lstPermissions.GetItemCount(), strTmp);
					strTmp.Format("%d", pPerm->intGroupID);
					m_lstPermissions.SetItemText(iItem, 1, strTmp);
					strTmp.Format("%d", pPerm->intAppID);
					m_lstPermissions.SetItemText(iItem, 2, strTmp);
					m_lstPermissions.SetItemText(iItem, 3, appName.MakeUpper());
					strTmp.Format("%d", pPerm->intModuleID);
					m_lstPermissions.SetItemText(iItem, 4, strTmp);
					m_lstPermissions.SetItemText(iItem, 5, pPerm->strRight.c_str());
					strTmp.Format("%d", pPerm->intRegionBranch);
					m_lstPermissions.SetItemText(iItem, 6, strTmp);
				}
			}
		}
	}
	if (cnt == 0)
	{
		int iItem = m_lstPermissions.InsertItem(m_lstPermissions.GetItemCount(), "-none-");
	}
	CString strTmp;
	strTmp.Format("%d Entries", cnt);
	GetDlgItem(IDC_TXT_PERMCOUNT)->SetWindowText(strTmp);

	std::vector<UADM::UADM_APP_PARAMETER*>* pPars = mgr.getParameters();
	cnt = 0;
	if (pPars != NULL)
	{
		cnt = (int)pPars->size();
		if (cnt > 0)
		{
			std::vector<UADM::UADM_APP_PARAMETER*>::iterator it;
			for (it = pPars->begin(); it != pPars->end(); it++)
			{
				UADM::UADM_APP_PARAMETER* pPara = *it;
				if (pPara != NULL)
				{
					CString strTmp;
					strTmp.Format("%d", pPara->intAppID);
					int iItem = m_lstParameters.InsertItem(m_lstParameters.GetItemCount(), strTmp);
					m_lstParameters.SetItemText(iItem, 1, pPara->strPara1.c_str());
					m_lstParameters.SetItemText(iItem, 2, pPara->strPara2.c_str());
					m_lstParameters.SetItemText(iItem, 3, pPara->strParaName.c_str());
					m_lstParameters.SetItemText(iItem, 4, pPara->strParaValue.c_str());
				}
			}
		}
	}
	if (cnt == 0)
	{
		int iItem = m_lstParameters.InsertItem(m_lstParameters.GetItemCount(), "-none-");
	}
	strTmp.Format("%d Entries", cnt);
	GetDlgItem(IDC_TXT_PARACOUNT)->SetWindowText(strTmp);

	showDebugInfos(mgr.getDebugInfos());
}


void CUADMLoginTesterDlg::showDebugInfos(std::vector<std::string>* pDbg)
{
	// Fill the debug info list and save to log file
	if (pDbg != NULL)
	{
		// set a background image for the login dialog
		char appPath[MAX_PATH] = { 0 };
		GetModuleFileName(NULL, appPath, sizeof(appPath));
		PathRemoveFileSpec(appPath);

		// create log file
		CString logFile(appPath);
		logFile.Append("\\UADMLoginTester.debug.log");
		CFile file;
		CFileException fe;
		if (file.Open(logFile, CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate | CFile::shareDenyWrite, &fe) == TRUE)
		{
			file.SeekToEnd();
		}
		int cnt = (int)pDbg->size();
		if (cnt > 0)
		{
			CDC* pDC = m_lstDebugInfo.GetDC();
			int dx = 0;
			std::vector<std::string>::iterator it;
			for (it = pDbg->begin(); it != pDbg->end(); it++)
			{
				std::string s = *it;
				m_lstDebugInfo.AddString(s.c_str());
				CSize sz = pDC->GetTextExtent(s.c_str());
				if (sz.cx > dx) dx = sz.cx;
				s.append("\r\n");
				file.Write(s.c_str(), (UINT)s.length());
			}
			m_lstDebugInfo.ReleaseDC(pDC);
			if (m_lstDebugInfo.GetHorizontalExtent() < dx)
			{
				m_lstDebugInfo.SetHorizontalExtent(dx);
			}
		}
		file.Close();

	}
}