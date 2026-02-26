/******************************************************************************/
/**                                                                           */
/**   Author : T.ARNOLD NTTDATA                                               */
/**   Date   : 25.11.2025                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once

#include "afxwin.h"
#include <vector>

#ifndef _PDLGUSERMANAGER_H_____
#define _PDLGUSERMANAGER_H_____

class PDlgUserManager : public CDialogMultiLang
{
	DECLARE_DYNAMIC(PDlgUserManager)

public:
	PDlgUserManager(); 
	virtual ~PDlgUserManager();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

private:
	// afx_msg handlers
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnBtnClickedUsersRefresh();
	afx_msg void OnBtnClickedUsersReport();
	afx_msg void OnUsersList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnUserParamsList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnApplicationSelChange();
	afx_msg BOOL OnApplicationDescTipNotify(UINT id, NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnUserNew();
	afx_msg void OnBtnUserSave();
	afx_msg void OnBtnUserDelete();
	afx_msg void OnBtnPermitGroup();
	afx_msg void OnBtnRevokeGroup();
	afx_msg void OnBtnUserParamNew();
	afx_msg void OnBtnUserParamSave();
	afx_msg void OnBtnUserParamDelete();
	afx_msg void OnBtnUserManagerExit();

	DECLARE_MESSAGE_MAP()

	// internal structures
	struct COLUMN_TYPES
	{
		int Index;
		CString Columns;
		int Width;
		int Align;
	};
	// internal methods
	void InitListbox(CListCtrl* pListbox, COLUMN_TYPES* pColtypes);
	// static data for comboboxes
	void InitUsertypes();
	void InitCountries();
	void InitApplications();
	void InitBranches();
	void InitUserParams();
	// dynamic areas to reset
	void ResetUserList(bool filter);
	void ResetUserDetails(int state);
	void ResetAllGroups(bool filter);
	void ResetUserGroups(bool filter);
	void ResetUserParams(bool filter);
	void ResetUserParamDetails(int state);
	// dynamic areas to load
	void LoadUsers(bool bExport, std::ofstream* pFile);
	void LoadUserDetails(int userid);
	void LoadAllGroups(int userid, int applicationid);
	void LoadUserGroups(int userid, int applicationid);
	void LoadUserParams(int userid, int applicationid);
	void SetUsertype(short iUsertype);
	void SetCountry(CString sCountry);
	bool AllowedChars(CString sString, bool ext);

	CListCtrl m_lstUsers;
	CListCtrl m_lstAllGroups;
	CListCtrl m_lstUserGroups;
	CListCtrl m_lstUserParams;

	CComboBox m_cmbUserType;
	CComboBox m_cmbCountry;
	CComboBox m_cmbApplication;
	CComboBox m_cmbBranch;
	CComboBox m_cmbUserParam;

	CButton m_chkUserActive;
	CButton m_chkCryptPHX;
	CButton m_chkCryptAES;
	CButton m_chkPwdChange;
	CButton m_chkExternal;

	int m_intUserState;
	int m_intUserParamState;
	CString m_strOrgPasswordPHX;
	CString m_strOrgPasswordAES;
	CString m_strSeparator;

	CToolTipCtrl m_AppDescTip;
	std::vector<CString> m_vecAppDesc;

	enum { IDD = IDD_DLG_USER_MANAGER };
};

#endif
