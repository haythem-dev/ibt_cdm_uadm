
// UADMLoginTesterDlg.h : header file
//

#pragma once


// CUADMLoginTesterDlg dialog
class CUADMLoginTesterDlg : public CDialogEx
{
// Construction
public:
	CUADMLoginTesterDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UADMLOGINTESTER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	CListCtrl m_lstPermissions;
	CListCtrl m_lstParameters;
	CListCtrl m_lstUserMappings;
	CListBox m_lstDebugInfo;
	CButton m_chkActive;
	CButton m_chkPWChange;
	CButton m_chkRegionBranchOnly;
	CButton m_chkUserMapping;
	CButton m_chkUserOnly;
	CButton m_chkUserAll;
	CButton m_chkPrefVoodoo;
	CButton m_chkTerminalID;
	CString m_strAppID;
	CString m_strTitle;

	void showDebugInfos(std::vector<std::string>* pDbg);

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnLoginWithDialog();
	afx_msg void OnLoginWithoutDialog();
	DECLARE_MESSAGE_MAP()
};
