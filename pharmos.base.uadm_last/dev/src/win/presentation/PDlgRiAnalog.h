/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                     */
/**   Date   : 21.04.2005                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once
#include "afxwin.h"

#ifndef __PDLGRIANALOG_H____
#define __PDLGRIANALOG_H____

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
class PDlgPW;

//BsnCon
class BConppGroup;
class BConppUser;
class BConppBranchReg;
class BConppAppRight;
//Vector
class C_VecArea;
class C_VecUsrGrp;
class C_VecMod;

/*----------------------------------------------------------------------------*/
class PDlgRiAnalog : public CDialogMultiLang
{
public:	
	DECLARE_DYNCREATE(PDlgRiAnalog)

					PDlgRiAnalog();           
	virtual			~PDlgRiAnalog();
	virtual BOOL	OnInitDialog();

protected:
	virtual void	DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	virtual BOOL	PreTranslateMessage(MSG  * pMsg );

private:
	// Generierte Nachrichtenzuordnungsfunktionen
	afx_msg void OnDblclkAppListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnItemchangedAppListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblclkModListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnItemchangedModListCtrl(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnBtAnalApp();
	afx_msg void OnBtAnalMod();
	afx_msg void OnBtAnalSave();
	afx_msg void OnBtAnalExit();
	afx_msg void OnBtAllGrpUsr();
	afx_msg void OnCbRegBr();
	afx_msg void OnCbUsrGrp();
	afx_msg void OnBtAllMod();

	DECLARE_MESSAGE_MAP()

	void           SetGrpUsrCtrlState  (const bool bShow);
	void           SetAppCtrlState     (const int iItem);
	void           SetModCtrlState     (const int iItem);

	void		   FillContainer();
	void		   GetBrRegComboValue();
	void		   GetUsrGrpComboValue();
	
	PDlgAppListCtrl			* m_pli_App;
	PDlgModListCtrl			* m_pli_Mod;
	PDlgUserListCtrl		* m_pli_User;
	PDlgGroupListCtrl		* m_pli_Group;
	PDlgBranchListCtrl		* m_pli_Branch;
	PDlgRegionListCtrl		* m_pli_Region;
	PDlgPW					* m_pPDlgPW; 


	CButton m_bt_App;
	CButton m_bt_Mod;
	CButton m_bt_AllGrpUsr;

	CButton m_ck_AllMod;
	
	CButton m_bt_AnalExit;
	CButton m_bt_AnalSave;

	CComboBox m_cb_BrReg;
	CComboBox m_cb_UsrGrp;

	bool m_bGroupDlg;
	bool m_bRegDlg;

	bool m_bDtlSelected;
	
	short m_sAppid;
	short m_sBrRegid;
	long  m_lUserid;
	long  m_lGroupid;
	
	// selected values in listctrls
	C_VecArea   * m_ConArea;
	C_VecUsrGrp * m_ConUsrGrp;
	C_VecMod    * m_ConMod;

	// for combo-boxes
	BConppBranchReg * m_pConBranch;
	BConppBranchReg * m_pConReg;
	BConppUser		* m_pConUser;
	BConppGroup		* m_pConGroup;
	
	// to read rights which should be given to another
	BConppAppRight  * m_pConRight;


	enum { IDD = IDD_DLG_RI_ANALOG };
};

#endif
