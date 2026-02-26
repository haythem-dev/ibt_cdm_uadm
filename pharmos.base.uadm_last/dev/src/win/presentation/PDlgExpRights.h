/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                     */
/**   Date   : 15.04.2005                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once
#include "afxwin.h"

#ifndef __PDLGEXPRIGHTS_H____
#define __PDLGEXPRIGHTS_H____

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

//Vector
class C_VecRights;
class C_VecArea;
class C_VecUsrGrp;
class C_VecMod;

/*----------------------------------------------------------------------------*/
class PDlgExpRights : public CDialogMultiLang
{
public:	
	DECLARE_DYNCREATE(PDlgExpRights)

					PDlgExpRights();           
	virtual			~PDlgExpRights();
	virtual BOOL	OnInitDialog();

	void setBGroup (bool bGroupDlg);

protected:
	virtual void	DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	virtual BOOL	PreTranslateMessage(MSG  * pMsg );

private:
	// Generierte Nachrichtenzuordnungsfunktionen
	afx_msg void OnDblclkAppListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnItemchangedAppListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblclkModListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnItemchangedModListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClickedAllModCk();

	afx_msg void OnBtExpApp();
	afx_msg void OnBtExpMod();
	afx_msg void OnBtSaveRights();
	afx_msg void OnBtDelRights();
	afx_msg void OnBtExpExit();

	afx_msg void OnBtAllGrpUsr();

	afx_msg void OnRightCategoryChanged();

	DECLARE_MESSAGE_MAP()

	void           SetGrpUsrCtrlState  (const bool bShow);
	void           SetAppCtrlState     (const int iItem);
	void           SetModCtrlState     (const int iItem);

	void		   FillContainer();
	
	PDlgGroupListCtrl		* m_pli_Group;
	PDlgAppListCtrl			* m_pli_App;
	PDlgModListCtrl			* m_pli_Mod;
	PDlgUserListCtrl		* m_pli_User;
	PDlgBranchListCtrl		* m_pli_Branch;
	PDlgRegionListCtrl		* m_pli_Region;
	PDlgRightListCtrl		* m_pli_Rights;
	PDlgPW					* m_pPDlgPW; 


	CButton m_bt_App;
	CButton m_bt_Mod;
	CButton m_bt_AllGrpUsr;

	CButton m_ck_AllMod;
	CButton m_ck_AllRi;
	CButton m_ck_AllBr;
	CButton m_ck_AllReg;
	CButton m_ck_AllUsrGrp;
	CButton m_ck_OverWrite;
	
	CStatic m_txt_UsrGrp;

	CButton m_bt_ExpExit;
	CButton m_bt_ExpSave;
	CButton m_bt_ExpDelete;

	CComboBox	m_cb_Category;
	CStatic		m_txt_Category;

	bool m_bGroupDlg;
	bool m_bLoadNew;

	short m_sAppid;
	short m_sModid;
	CBString m_Appname;

	C_VecRights * m_ConRight;
	C_VecArea   * m_ConArea;
	C_VecUsrGrp * m_ConUsrGrp;
	C_VecMod	* m_ConMod;

	enum { IDD = IDD_DLG_EXPRESS_RIGHTS };
};

#endif


