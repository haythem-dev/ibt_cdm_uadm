/******************************************************************************/
/**                                                                           */
/**   Author : T.ARNOLD NTTDATA                                               */
/**   Date   : 25.11.2025                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once
#include "afxwin.h"

#ifndef __PDLGDLLUSERTYPE_H____
#define __PDLGDLLUSERTYPE_H____

/*----------------------------------------------------------------------------*/
//  Forward Declaration
/*----------------------------------------------------------------------------*/
class PDlgUsertypeListCtrl;
class BppUsertype;


class PDlgDllUsertype : public CDialogMultiLang
{
public:	
	DECLARE_DYNCREATE(PDlgDllUsertype)

					PDlgDllUsertype();
	virtual			~PDlgDllUsertype();
	virtual BOOL	OnInitDialog();

protected:
	virtual void	DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	virtual BOOL	PreTranslateMessage(MSG           * pMsg );

private:
	// Generierte Nachrichtenzuordnungsfunktionen
	afx_msg void OnDblclkListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnItemchangedListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClickListCtrl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtUsertypeNew();
	afx_msg void OnBtUsertypeWork();
	afx_msg void OnBtUsertypeDel();
	afx_msg void OnBtUsertypeExit();
		

	DECLARE_MESSAGE_MAP()

	void           SetCtrlState     (const int iItem);
	
	PDlgUsertypeListCtrl* m_pli_Usertype;
	
	CButton m_bt_Usertype_New;
	CButton m_bt_Usertype_Work;
	CButton m_bt_Usertype_Del;
	CButton m_bt_Usertype_Rights;
	CButton m_bt_Usertype_Exit;

	enum { IDD = IDD_DLG_DLL_USERTYPES };

};

#endif

