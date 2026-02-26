/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                     */
/**   Date   : 28.06.2004                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once
#include "afxwin.h"

#ifndef __PDLGDLLAPPMODRI_H____
#define __PDLGDLLAPPMODRI_H____

/*----------------------------------------------------------------------------*/
//  Forward Declaration
/*----------------------------------------------------------------------------*/
class PDlgAppListCtrl;
class PDlgModListCtrl;
class PDlgRightListCtrl;

class PDlgDllAppModRi : public CDialogMultiLang
{
public:	
	DECLARE_DYNCREATE(PDlgDllAppModRi)

					PDlgDllAppModRi();           
	virtual			~PDlgDllAppModRi();
	virtual BOOL	OnInitDialog();

protected:
	virtual void	DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	virtual BOOL	PreTranslateMessage(MSG   * pMsg );

private:
	// Generierte Nachrichtenzuordnungsfunktionen
	afx_msg void OnDblclkModListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnItemchangedModListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnItemchangedAppListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblclkAppListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnItemchangedRightListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblclkRightListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickLiMod(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickLiRight(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnBtDllAppSel();
	afx_msg void OnBtDllAppNew();
	afx_msg void OnBtDllAppWork();
	afx_msg void OnBtDllAppDel();
	afx_msg void OnBtDllModSel();
	afx_msg void OnBtDllModNew();
	afx_msg void OnBtDllModWork();
	afx_msg void OnBtDllModDel();
	afx_msg void OnBtDllRiNew();
	afx_msg void OnBtDllRiWork();
	afx_msg void OnBtDllRiDel();
	afx_msg void OnBtDllAppExit();
	afx_msg void OnRightCategoryChanged();


	DECLARE_MESSAGE_MAP()

	CButton m_bt_Exit;
	CButton m_bt_App_Sel;
	CButton m_bt_App_New;
	CButton m_bt_App_Work;
	CButton m_bt_App_Del;
	CButton m_bt_Mod_Sel;
	CButton m_bt_Mod_New;
	CButton m_bt_Mod_Work;
	CButton m_bt_Mod_Del;
	CButton m_bt_Ri_New;
	CButton m_bt_Ri_Work;
	CButton m_bt_Ri_Del;
	CComboBox	m_cb_Category;
	CStatic		m_txt_Category;

	void           SetAppCtrlState     (const int iItem);
	void		   SetModCtrlState	   (const int iItem);
	void		   SetRiCtrlState	   (const int iItem);

	bool		   DelApplRight		   (CBString& strAppname );

	PDlgAppListCtrl		* m_pli_App;
	PDlgModListCtrl		* m_pli_Mod;
	PDlgRightListCtrl	* m_pli_Right;

	short m_sAppid;
	short m_sModid;
	CBString m_Appname;
		
	enum { IDD = IDD_DLG_DLL_APP_MOD_RIGHT };

};

#endif


