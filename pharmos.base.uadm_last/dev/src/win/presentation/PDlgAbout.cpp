/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH                                         */
/**   Date   : 27.05.2004                                                     */
/**                                                                           */
/******************************************************************************/

/*----------------------------------------------------------------------------*/
#include "StdAfx.h"

#include "CVersion.h"

#include "BConnect.h"

#include "presentation.h"
#include "PDlgAbout.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include <math.h>


/*----------------------------------------------------------------------------*/
// Defines
/*----------------------------------------------------------------------------*/
#define DB_LC_COL_NAME				  0
#define DB_LC_COL_VERSION			  1

#define DB_LC_COLWIDTH_NAME			151
#define DB_LC_COLWIDTH_VERSION		168

/*----------------------------------------------------------------------------*/
// 
/*----------------------------------------------------------------------------*/
PDlgAbout::PDlgAbout() : CDialogMultiLang(PDlgAbout::IDD)
{
	m_hCursor = 0;
	m_crText  = 0;
}

/*----------------------------------------------------------------------------*/
// 
/*----------------------------------------------------------------------------*/
PDlgAbout::~PDlgAbout()
{
	m_cfFont.DeleteObject();
}

/*----------------------------------------------------------------------------*/
// 
/*----------------------------------------------------------------------------*/
void PDlgAbout::DoDataExchange(CDataExchange* pDX)
{
	CDialogMultiLang::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BT_OK	           , m_bt_OK);
	DDX_Control(pDX, IDC_LI_DLLS           , m_li_Dll);
	DDX_Control(pDX, IDC_LI_USERROLES      , m_li_Roles);
	DDX_Control(pDX, IDC_TXT_USERNAME	   , m_txt_Username);
	DDX_Control(pDX, IDC_TXT_VERSION       , m_txt_Version);
	DDX_Control(pDX, IDC_TXT_DATE          , m_txt_Date);
	DDX_Control(pDX, IDC_TXT_COPYRIGHT     , m_txt_Copyright);
	DDX_Control(pDX, IDC_GRP_APPLICATION   , m_txt_Product);
	DDX_Control(pDX, IDC_TXT_URL		   , m_txt_Doku);
	DDX_Control(pDX, IDC_TXT_HOSTNAME      , m_txt_Hostname);
	DDX_Control(pDX, IDC_TXT_DATABASE      , m_txt_Dbname);
}

/*----------------------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(PDlgAbout, CDialogMultiLang)
	ON_BN_CLICKED(IDC_BT_OK, OnBnClickedBtOk)
	ON_STN_CLICKED(IDC_TXT_URL, OnClickedTxtUrl)
	ON_WM_CTLCOLOR	()
	ON_WM_SETCURSOR	()
END_MESSAGE_MAP()


/*----------------------------------------------------------------------------*/
HBRUSH PDlgAbout::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogMultiLang::OnCtlColor(pDC, pWnd, nCtlColor);

	if (pWnd == &m_txt_Doku)
	{
		if (CTLCOLOR_STATIC == nCtlColor)
		{
			pDC->SelectObject(&m_cfFont);
			pDC->SetTextColor(m_crText );		// blau
		}
	}

	return hbr;
}

/*----------------------------------------------------------------------------*/
BOOL PDlgAbout::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (pWnd == &m_txt_Doku)
	{
		if (m_hCursor != 0)
		{
			::SetCursor(m_hCursor);
			return TRUE;
		}
	}

	return CDialogMultiLang::OnSetCursor(pWnd, nHitTest, message);
}

/*----------------------------------------------------------------------------*/
void PDlgAbout::GetBuildDate(CString csFilename, CString& csDate)
{
	// Versionsdatum ermitteln
	CFileStatus	objCFileStat;
	CTime		objCTime;

	if (CFile::GetStatus(csFilename, objCFileStat))
	{
		objCTime = objCFileStat.m_mtime;
		csDate.Format(CResString::ex().getStrTblText(IDS_APP_DATUMZEIT),
                      objCTime.GetDay(), 
                      objCTime.GetMonth(), 
                      objCTime.GetYear(),
                      objCTime.GetHour(),
                      objCTime.GetMinute());
	}
    else
    {
        TRACE(__FUNCTION__": file %s not found!\n", csFilename);
    }
}

/*----------------------------------------------------------------------------*/
void PDlgAbout::OnBnClickedBtOk()
{
	CDialogMultiLang::OnCancel();
}

/*----------------------------------------------------------------------------*/
void PDlgAbout::OnClickedTxtUrl()
{
	CString strLink;
	m_txt_Doku.GetWindowText(strLink);

	ShellExecute(NULL,				// hWnd
		         "open",			// operation
				 strLink,			// file
				 NULL,				// parameters
				 NULL,				// directory
				 SW_SHOWNORMAL);	// ShowCmd
}

/*----------------------------------------------------------------------------*/
BOOL PDlgAbout::OnInitDialog() 
{
	CDialogMultiLang::OnInitDialog();

    CString csCopyright,
            csBuildDate,
            csFilename;

    csFilename.Format("%s.exe", AfxGetApp()->m_pszExeName);

	GetModuleFileName(AfxGetApp()->m_hInstance, csFilename.GetBuffer(MAX_PATH), MAX_PATH);

	csCopyright.Format("%s %s", CVersion::COPYRIGHT, CVersion::COMPANY_NAME);
    GetBuildDate(csFilename, csBuildDate);

	csFilename.ReleaseBuffer();

	m_txt_Username.SetWindowText(ppBLoginData::ex().GetUsername().c_str());
	// add roles to listbox
	CStringList* pRoles = &((CpresentationApp*)AfxGetApp())->m_lstUserRoles;
	for (int i=0;i < pRoles->GetSize(); i++)
	{
		m_li_Roles.AddString(pRoles->GetAt(pRoles->FindIndex(i)));
	}

	m_txt_Product   .SetWindowText(CVersion::PRODUCT_NAME);
	m_txt_Version   .SetWindowText(CVersion::VERSION_STRING);
    m_txt_Date      .SetWindowText(csBuildDate);
    m_txt_Copyright .SetWindowText(csCopyright);
	m_txt_Hostname  .SetWindowText(BConnect  ::ex().getServer     ().c_str());
	m_txt_Dbname    .SetWindowText(BConnect  ::ex().getCurDatabase().c_str());

	// Font erstellen für Doku-Link und in m_cfFont speichern
	LOGFONT lfFont;
	m_txt_Doku.GetFont()->GetLogFont(&lfFont);
	lfFont.lfUnderline = TRUE;
	m_cfFont.CreateFontIndirect(&lfFont);
	
	m_hCursor = AfxGetApp()->LoadCursor(IDC_CURSOR_HAND);			// Cursor erstellen für Doku-Link
	m_crText = RGB(0, 0, 255);										// Farbe einstellen für Doku-Link
	//CResString::ex().setWndText(m_txt_Doku,IDS_DOKU_URL);
	m_txt_Doku.SetWindowText(CResString::ex().getStrTblText(IDS_DOKU_URL));
	
	// list control
	m_li_Dll.InsertColumn(DB_LC_COL_NAME, 
						  "Name", 
						  LVCFMT_LEFT, 
						  DB_LC_COLWIDTH_NAME);

	m_li_Dll.InsertColumn(DB_LC_COL_VERSION, 
		                  "Version", 
						  LVCFMT_LEFT, 
						  DB_LC_COLWIDTH_VERSION);

	m_li_Dll.SetBkColor    (::GetSysColor(COLOR_BTNFACE));
	m_li_Dll.SetTextBkColor(::GetSysColor(COLOR_BTNFACE));

	int iCntDlls = sizeof(((CpresentationApp *)AfxGetApp())->m_sDll) / sizeof(CpresentationApp::T_Dll);

	for (int i = 0; i < iCntDlls; i++)
	{
		m_li_Dll.InsertItem (i, "");
		m_li_Dll.SetItemText(i, DB_LC_COL_NAME   , ((CpresentationApp *)AfxGetApp())->m_sDll[i].m_spName     );
		m_li_Dll.SetItemText(i, DB_LC_COL_VERSION, ((CpresentationApp *)AfxGetApp())->m_sDll[i].m_fpVersion());
	}
	
	return TRUE;
}

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/





