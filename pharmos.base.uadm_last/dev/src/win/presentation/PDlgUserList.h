/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                     */
/**   Date   : 26.05.2004                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once
#include "afxwin.h"

#ifndef __PDLGUSERLIST_H____
#define __PDLGUSERLIST_H____

/*----------------------------------------------------------------------------*/
//  Forward Declaration
/*----------------------------------------------------------------------------*/
class PDlgUserListCtrl;
class PDlgAppListCtrl;
class BppUser;


class PDlgUserList : public CDialogMultiLang
{
public:	
	DECLARE_DYNCREATE(PDlgUserList)

					PDlgUserList();           
	virtual			~PDlgUserList();
	virtual BOOL	OnInitDialog();

protected:
	virtual void	DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	virtual BOOL	PreTranslateMessage(MSG           * pMsg );

private:
	// Generierte Nachrichtenzuordnungsfunktionen
	afx_msg void OnDblclkListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBtUsrDtl();
	afx_msg void OnItemchangedListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBtUsrExit();
	afx_msg void OnItemchangedAppListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblclkAppListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBtSelApp();
	afx_msg void OnBtAllApp();
	afx_msg void OnBtUsrNew();
	afx_msg void OnBtUsrDel();
	afx_msg void OnNMClickLiUsr(NMHDR *pNMHDR, LRESULT *pResult);
		

	DECLARE_MESSAGE_MAP()

	void           SetUsrCtrlState     (const int iItem);
	void           SetAppCtrlState     (const int iItem);
	
	PDlgUserListCtrl   * m_pli_User;
	PDlgAppListCtrl	   * m_pli_App;
	CString				m_csAppName;
	
	CButton m_bt_Show_UsrDtl;
	CButton m_bt_Cancel;
	CStatic m_txt_SelApp;
	CButton m_bt_SelApp;
	CButton m_bt_UsrNew;
	CButton m_bt_UsrDel;
	CButton m_bt_AppALL;

	enum { IDD = IDD_DLG_USR_LST };
	
};

#endif
