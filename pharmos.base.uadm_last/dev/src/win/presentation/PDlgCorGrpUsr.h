/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                     */
/**   Date   : 09.06.2004                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once
#include "afxwin.h"

#ifndef _PDLGCORGRPUSR_H_____
#define _PDLGCORGRPUSR_H_____

/*----------------------------------------------------------------------------*/
//  Forward Declaration
/*----------------------------------------------------------------------------*/
class BppGroup;
class PDlgUserListCtrl;
class PDlgPW;
class BConppApp;
class BConppGroup;

/*----------------------------------------------------------------------------*/
// PDlgUserDetail-Dialogfeld
/*----------------------------------------------------------------------------*/
class PDlgCorGrpUsr : public CDialogMultiLang
{
	DECLARE_DYNAMIC(PDlgCorGrpUsr)

public:
	PDlgCorGrpUsr(BppGroup * pBppGroup); 
	virtual ~PDlgCorGrpUsr();

	
	void	setAppid	(short sAppid);
	void	setAppname	(CBString strAppname);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	virtual BOOL	OnInitDialog       ();
	virtual BOOL	PreTranslateMessage(MSG           * pMsg );

private:
	enum { IDD = IDD_DLG_GRP_USER };

	void           SetCorCtrlState     (const int iItem);
	void           SetAllCtrlState     (const int iItem);
	
	// Generierte Nachrichtenzuordnungsfunktionen
	afx_msg void OnDblclkCorListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnItemchangedCorListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnItemchangedAllListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblclkAllListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMSetfocusLiUsrCor(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMSetfocusLiUsrAll(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickLiUsrCor(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickLiUsrAll(NMHDR *pNMHDR, LRESULT *pResult);
	
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBtExitGrpcor();
	afx_msg void OnBtGrpCorUsr();
	afx_msg void OnBtGrpCorDel();
	afx_msg void OnBtAnalog();


	PDlgUserListCtrl   * m_pli_CorUser;
	PDlgUserListCtrl   * m_pli_AllUser;
	BppGroup		   * m_pBppGroup;
	PDlgPW			   * m_pPDlgPW;

	short m_sAppid;
	CBString m_strAppname;
	bool m_bLoadNew;
	BConppApp m_ConApp;
	BConppGroup m_ConGroup;
	
	// #### Controls
	CButton m_bt_Exit_GrpCor;
	CButton m_bt_Cor_User;
	CButton m_bt_Del_CorUsr;
	CStatic m_txt_Groupname;
	CEdit m_ed_Appname;
	CComboBox m_cb_App;
	CComboBox m_cb_Group;
	CButton m_bt_Analog;
	
};

#endif
