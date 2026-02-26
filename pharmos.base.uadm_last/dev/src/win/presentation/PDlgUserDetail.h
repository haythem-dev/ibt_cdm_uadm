/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                     */
/**   Date   : 26.05.2004                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once
#include "afxwin.h"

#ifndef _PDLGUSERDETAIL_H_____
#define _PDLGUSERDETAIL_H_____

/*----------------------------------------------------------------------------*/
//  Forward Declaration
/*----------------------------------------------------------------------------*/
class BppUser;
class BTest;

/*----------------------------------------------------------------------------*/
// PDlgUserDetail-Dialogfeld
/*----------------------------------------------------------------------------*/
class PDlgUserDetail : public CDialogMultiLang
{
	DECLARE_DYNAMIC(PDlgUserDetail)

public:
	PDlgUserDetail(BppUser * pBppUser); 
	virtual ~PDlgUserDetail();

	void		setBNewUser(bool bNewUser);
	const bool	getBNewUser();
	const bool	getBDataChanged();
	void		getCloneObj(BppUser * pUserChanged);
	const bool  getBLocked();
	void		SetSaveBtDisabled  ();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	virtual BOOL	OnInitDialog       ();

private:
	enum { IDD = IDD_DLG_USR_DTL };
	
	// Generierte Nachrichtenzuordnungsfunktionen
	afx_msg void OnBtCancel();
	afx_msg void OnCkExternal();
	afx_msg void OnBtSave();
	afx_msg void OnOK();
	afx_msg void OnBIDefault();
	
	DECLARE_MESSAGE_MAP()


	void            SetCtrlValues      ();
	bool			GetCtrlValues	   ();
	bool			UserNameExists	   (CBString &strUserName, bool & bExists);
	bool			isPwComplex		   (CBString &strPw);
	bool			isNtUserValid	   (CBString &strNtUser);
	void            InitUsertypes      ();
	void            InitCountries      ();

	BppUser  *	m_pBppUserClone;
	BppUser	 *  pUserTestChange;
	bool		m_bUserNew;
	bool		m_bDataChanged;
	bool		m_bLocked;
	CBString	m_strUnamePrev;
	BTest	*	m_pTest;
	bool		m_bSaveBtEnabled;
	bool		m_bBiDefaultWritten;


	// #### Controls
	CButton m_bt_Cancel;
	CButton m_bt_Save;
	CButton m_bt_BI_Default;

	CButton m_ck_Activ;
	CButton m_ck_ChangePW;
	CButton m_ck_External;

	CComboBox m_cmb_UserType;
	CComboBox m_cmb_Country;

	CEdit m_ed_Username;
	CEdit m_ed_Password;
	CEdit m_ed_Surname;
	CEdit m_ed_Forename;
	CEdit m_ed_Desc;
	CEdit m_ed_Mail;
	CEdit m_ed_Email;
	CEdit m_ed_NtUser;
	CEdit m_ed_Ref;
	CEdit m_ed_LastLog;
	CEdit m_ed_LastPwChange;
};

#endif
