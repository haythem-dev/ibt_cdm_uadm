/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                     */
/**   Date   : 14.06.2004                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once
#include "afxwin.h"

#ifndef __PDLGADMRIGHTS_H____
#define __PDLGADMRIGHTS_H____

/*----------------------------------------------------------------------------*/
//  Forward Declaration
/*----------------------------------------------------------------------------*/
//ListCtrls
class PDlgGroupListCtrl;
class PDlgAppListCtrl;
class PDlgModListCtrl;
class PDlgUserListCtrl;
class PDlgBranchListCtrl;
class PDlgRegionListCtrl;
class PDlgRightListCtrl;
class PDlgPW;
//BsnObj


/*----------------------------------------------------------------------------*/
class PDlgAdmRights : public CDialogMultiLang
{
public:	
	DECLARE_DYNCREATE(PDlgAdmRights)

					PDlgAdmRights();           
	virtual			~PDlgAdmRights();
	virtual BOOL	OnInitDialog();

	void setBGroup (bool bGroupAdm);

protected:
	virtual void	DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	virtual BOOL	PreTranslateMessage(MSG  * pMsg );

private:
	// Generierte Nachrichtenzuordnungsfunktionen
	afx_msg void OnDblclkUsrListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnItemchangedUsrListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblclkAppListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnItemchangedAppListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblclkModListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnItemchangedModListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblclkRegListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnItemchangedRegListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblclkBranchListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnItemchangedBranchListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblclkCorRightListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnItemchangedCorRightListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblclkAllRightListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnItemchangedAllRightListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMSetfocusLiAdmAllrights(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMSetfocusLiAdmCorRights(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickLiAdmAllrights(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickLiAdmCorRights(NMHDR *pNMHDR, LRESULT *pResult);
	

	afx_msg void OnBtAdmApp();
	afx_msg void OnBtAdmMod();
	afx_msg void OnBtAdmBranch();
	afx_msg void OnBtAdmUsr();
	afx_msg void OnBtCorRight();
	afx_msg void OnBtEraseRight();
	afx_msg void OnBtAdmExit();
	afx_msg void OnBtReg();
	afx_msg void OnBtAllGrp();

	afx_msg void OnRightCategoryChanged();	

	DECLARE_MESSAGE_MAP()

	void           SetUsrCtrlState     (const int iItem);
	void           SetAppCtrlState     (const int iItem);
	void           SetBranchCtrlState  (const int iItem);
	void           SetRegCtrlState     (const int iItem);
	void           SetModCtrlState     (const int iItem);
	void           SetCorRightCtrlState(const int iItem);
	void           SetAllRightCtrlState(const int iItem);

	
	PDlgGroupListCtrl		* m_pli_Group;
	PDlgAppListCtrl			* m_pli_App;
	PDlgModListCtrl			* m_pli_Mod;
	PDlgUserListCtrl		* m_pli_User;
	PDlgBranchListCtrl		* m_pli_Branch;
	PDlgRegionListCtrl		* m_pli_Region;
	PDlgRightListCtrl		* m_pli_CorRights;
	PDlgRightListCtrl		* m_pli_AllRights;
	PDlgRightListCtrl		* m_pli_GroupRights;
	PDlgPW					* m_pPDlgPW;


	CButton m_bt_App;
	CButton m_bt_Mod;
	CButton m_bt_Branch;
	CButton m_bt_Usr;
	CStatic m_txt_UsrGrp;
	CButton m_bt_CorRight;
	CButton m_bt_EraseRight;
	CButton m_bt_AdmExit;
	CButton m_bt_Reg;
	CStatic m_txt_GroupRights;
	CButton m_bt_All_Grp;

	CComboBox	m_cb_Category;
	CStatic		m_txt_Category;

	bool m_bGroupAdm;
	bool m_bLoadNew;

	short m_sAppid;
	CBString m_Appname;
	short m_sModid;
	short m_sBranchRegid;
	short m_sUserid;
	short m_sGroupid;
	
	enum { IDD = IDD_DLG_ADM_USR_RIGHTS };
};

#endif


