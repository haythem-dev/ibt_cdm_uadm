/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                     */
/**   Date   : 30.06.2004                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once
#include "afxwin.h"

#ifndef __PDLGDLLBRREG_H____
#define __PDLGDLLBRREG_H____

/*----------------------------------------------------------------------------*/
//  Forward Declaration
/*----------------------------------------------------------------------------*/
class PDlgAppListCtrl;
class PDlgBranchListCtrl;
class PDlgRegionListCtrl;

class PDlgDllBrReg : public CDialogMultiLang
{
public:	
	DECLARE_DYNCREATE(PDlgDllBrReg)

					PDlgDllBrReg();           
	virtual			~PDlgDllBrReg();
	virtual BOOL	OnInitDialog();

protected:
	virtual void	DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	virtual BOOL	PreTranslateMessage(MSG   * pMsg );

private:
	// Generierte Nachrichtenzuordnungsfunktionen
	afx_msg void OnDblclkRegALLListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblclkRegCORListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblclkBrCORListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnItemchangedRegALLListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnItemchangedRegCORListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnItemchangedAppListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblclkAppListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnItemchangedBrALLListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblclkBrALLListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnItemchangedBrCORListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMSetfocusCorReg(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMSetfocusAllReg(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMSetfocusCorBr(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMSetfocusAllBr(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickCorReg(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickAllReg(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickCorBr(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickAllBr(NMHDR *pNMHDR, LRESULT *pResult);
	
	afx_msg void OnBtBrregApp();
	afx_msg void OnBtBrregCorReg();
	afx_msg void OnBtBrregEraseReg();
	afx_msg void OnBtBrregNewReg();
	afx_msg void OnBtBrregWorkReg();
	afx_msg void OnBtBrregDelReg();
	afx_msg void OnBtBrRegCor();
	afx_msg void OnBtBrregCorBr();
	afx_msg void OnBtBrregEraseBr();
	afx_msg void OnBtBrregNewBr();
	afx_msg void OnBtBrregWorkBr();
	afx_msg void OnBtBrregDelBr();
	afx_msg void OnBtBrregExit();
	afx_msg void OnBtWorkCorReg();
	afx_msg void OnBtWorkCorBr();

	
	DECLARE_MESSAGE_MAP()
	
	void		   SetBranchCtrlState	   (const int iItem);
	void		   SetRegionCtrlState	   (const int iItem);
	void		   SetCORBranchCtrlState	(const int iItem);
	void		   SetCORRegionCtrlState	(const int iItem);

	PDlgAppListCtrl		* m_pli_App;
	PDlgBranchListCtrl	* m_pli_BranchALL;
	PDlgRegionListCtrl	* m_pli_RegionALL;
	PDlgBranchListCtrl	* m_pli_BranchCOR;
	PDlgRegionListCtrl	* m_pli_RegionCOR;


	short m_sAppid;
	bool m_bLoadNew;

	CButton m_bt_Sel_App;

	CButton m_bt_CorReg;
    CButton m_bt_EraseReg;
	CButton m_bt_Reg_New;
	CButton m_bt_Reg_Work;
	CButton m_bt_Reg_Del;
	CButton m_bt_Work_CorReg;

	CButton m_bt_Cor_RegBr;

	CButton m_bt_Br_Cor;
	CButton m_bt_Br_Erase;
	CButton m_bt_Br_New;
	CButton m_bt_Br_Work;
	CButton m_bt_Br_Del;
	CButton m_bt_Work_CorBr;

	CButton m_bt_BrReg_Exit;
		
	enum { IDD = IDD_DLG_DLL_BRANCH_REG };
public:
	
	
};

#endif
