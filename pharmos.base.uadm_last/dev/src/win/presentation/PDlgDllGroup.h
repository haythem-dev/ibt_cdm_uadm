/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                     */
/**   Date   : 01.07.2004                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once
#include "afxwin.h"

#ifndef __PDLGDLLGROUP_H____
#define __PDLGDLLGROUP_H____

/*----------------------------------------------------------------------------*/
//  Forward Declaration
/*----------------------------------------------------------------------------*/
class PDlgGroupListCtrl;
class BppGroup;


class PDlgDllGroup : public CDialogMultiLang
{
public:	
	DECLARE_DYNCREATE(PDlgDllGroup)

					PDlgDllGroup();           
	virtual			~PDlgDllGroup();
	virtual BOOL	OnInitDialog();

protected:
	virtual void	DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	virtual BOOL	PreTranslateMessage(MSG           * pMsg );

private:
	// Generierte Nachrichtenzuordnungsfunktionen
	afx_msg void OnDblclkListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnItemchangedListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBtGrpNew();
	afx_msg void OnBtGrpWork();
	afx_msg void OnBtGrpDel();
	afx_msg void OnBtGrpRights();
	afx_msg void OnBtGrpExit();
		

	DECLARE_MESSAGE_MAP()

	void           SetCtrlState     (const int iItem);
	
	PDlgGroupListCtrl   * m_pli_Group;
	
	CButton m_bt_Grp_New;
	CButton m_bt_Grp_Work;
	CButton m_bt_Grp_Del;
	CButton m_bt_Grp_Rights;
	CButton m_bt_Grp_Exit;

	enum { IDD = IDD_DLG_DLL_GROUP };

};

#endif

