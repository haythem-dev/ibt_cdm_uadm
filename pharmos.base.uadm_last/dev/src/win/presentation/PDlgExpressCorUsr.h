/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                     */
/**   Date   : 09.06.2005                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once
#include "afxwin.h"

#ifndef __PDLGEXPRESSCORUSR_H__
#define __PDLGEXPRESSCORUSR_H__

/*----------------------------------------------------------------------------*/
//  Forward Declaration
/*----------------------------------------------------------------------------*/
class PDlgGroupListCtrl;
class PDlgAppListCtrl;
class PDlgUserListCtrl;
class PDlgPW;

//Vector
class C_VecGrp;
class C_VecUsr;


class PDlgExpressCorUsr : public CDialogMultiLang
{
public:	
	DECLARE_DYNCREATE(PDlgExpressCorUsr)

					PDlgExpressCorUsr();           
	virtual			~PDlgExpressCorUsr();
	virtual BOOL	OnInitDialog();

protected:
	virtual void	DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	virtual BOOL	PreTranslateMessage(MSG           * pMsg );

private:
	// Generierte Nachrichtenzuordnungsfunktionen
	afx_msg void OnItemchangedAppListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblclkAppListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBtSelApp();
	afx_msg void OnBtCorUsr();
	afx_msg void OnBtExit();
	afx_msg void OnBtAllUsr();
	afx_msg void OnBtAllGrp();

	void		   FillContainer();
	C_VecGrp	 * m_ConGrp;
	C_VecUsr	 * m_ConUsr;

	
	PDlgGroupListCtrl  * m_pli_Group;
	PDlgUserListCtrl   * m_pli_User;
	PDlgAppListCtrl	   * m_pli_App;
	PDlgPW			   * m_pPDlgPW;

	CButton m_bt_Sel_App;
	CButton m_bt_Sel_Group;
	CButton m_bt_CorUsr;
	CButton m_bt_Exit;
	CButton m_bt_AllUsr;
	CButton m_bt_AllGrp;
	CButton m_ck_OverWrite;
	
	short m_sAppid;
	
	enum { IDD = IDD_DLG_COR_USER_GROUP };
};

#endif
