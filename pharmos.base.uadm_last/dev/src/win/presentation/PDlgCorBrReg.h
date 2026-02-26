/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                     */
/**   Date   : 01.07.2004                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once
#include "afxwin.h"

#ifndef _PDLGCORBRREG_H_____
#define _PDLGCORBRREG_H_____

/*----------------------------------------------------------------------------*/
//  Forward Declaration
/*----------------------------------------------------------------------------*/
class BppBranchReg;
class PDlgBranchListCtrl;

/*----------------------------------------------------------------------------*/
// PDlgUserDetail-Dialogfeld
/*----------------------------------------------------------------------------*/
class PDlgCorBrReg : public CDialogMultiLang
{
	DECLARE_DYNAMIC(PDlgCorBrReg)

public:
	PDlgCorBrReg(BppBranchReg * pBppBrReg); 
	virtual ~PDlgCorBrReg();
	
	void	setAppname	(CBString strAppname);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	virtual BOOL	OnInitDialog       ();
	virtual BOOL	PreTranslateMessage(MSG           * pMsg );

private:
	enum { IDD = IDD_DLG_COR_BR_REG };

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
		
	afx_msg void OnBtBrCor();
	afx_msg void OnBtBrErase();
	afx_msg void OnBtBrcorExit();

	PDlgBranchListCtrl		* m_pli_BrCOR;
	PDlgBranchListCtrl		* m_pli_BrALL;
	BppBranchReg		    * m_pBppBrReg;

	CBString  m_strAppname;
	bool m_bLoadNew;
	
	// #### Controls
	CEdit m_ed_Appname;
	CEdit m_ed_Regname;
	CButton m_bt_Br_Cor;
	CButton m_bt_Br_Erase;
	CButton m_bt_BrCor_Exit;

};

#endif

