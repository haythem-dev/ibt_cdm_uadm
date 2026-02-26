/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                     */
/**   Date   : 09.06.2004                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once
#include "afxwin.h"

#ifndef __PDLGGROUPLIST_H____
#define __PDLGGROUPLIST_H____

/*----------------------------------------------------------------------------*/
//  Forward Declaration
/*----------------------------------------------------------------------------*/
class PDlgGroupListCtrl;
class PDlgAppListCtrl;
class BppGroup; 


class PDlgGroupList : public CDialogMultiLang
{
public:	
	DECLARE_DYNCREATE(PDlgGroupList)

					PDlgGroupList();           
	virtual			~PDlgGroupList();
	virtual BOOL	OnInitDialog();

protected:
	virtual void	DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	virtual BOOL	PreTranslateMessage(MSG           * pMsg );

private:
	// Generierte Nachrichtenzuordnungsfunktionen
	afx_msg void OnDblclkListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnItemchangedListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnItemchangedAppListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblclkAppListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBtSelApp();
	afx_msg void OnBtSelGroup();
	afx_msg void OnBtGrpExit();
	afx_msg void OnBtGrpAllApp();
	afx_msg void OnBtNewGrp();

	DECLARE_MESSAGE_MAP()

	void           SetGrpCtrlState     (const int iItem);
	void           SetAppCtrlState     (const int iItem);
	
	PDlgGroupListCtrl  * m_pli_Group;
	PDlgAppListCtrl	   * m_pli_App;

	CButton				m_bt_SelGroup;
	CButton				m_bt_SelApp;
	CButton				m_bt_GrpExit;
	CButton				m_bt_Grp_All_App;
	CButton				m_bt_New_Grp;
		
	enum { IDD = IDD_DLG_GRP };

};

#endif

