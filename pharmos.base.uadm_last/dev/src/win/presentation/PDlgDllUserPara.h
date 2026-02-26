/******************************************************************************/
/**                                                                           */
/**   Author : T.ARNOLD NTTDATA                                               */
/**   Date   : 25.11.2025                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once
#include "afxwin.h"

#ifndef __PDLGDLLUSERPARA_H____
#define __PDLGDLLUSERPARA_H____

/*----------------------------------------------------------------------------*/
//  Forward Declaration
/*----------------------------------------------------------------------------*/
class PDlgUserParaListCtrl;
class BppUserPara;


class PDlgDllUserPara : public CDialogMultiLang
{
public:	
	DECLARE_DYNCREATE(PDlgDllUserPara)

					PDlgDllUserPara();
	virtual			~PDlgDllUserPara();
	virtual BOOL	OnInitDialog();

protected:
	virtual void	DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	virtual BOOL	PreTranslateMessage(MSG           * pMsg );

private:
	// Generierte Nachrichtenzuordnungsfunktionen
	afx_msg void OnDblclkListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnItemchangedListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClickListCtrl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtUserParaNew();
	afx_msg void OnBtUserParaWork();
	afx_msg void OnBtUserParaDel();
	afx_msg void OnBtUserParaExit();
		

	DECLARE_MESSAGE_MAP()

	void           SetCtrlState     (const int iItem);
	
	PDlgUserParaListCtrl* m_pli_UserPara;
	
	CButton m_bt_UserPara_New;
	CButton m_bt_UserPara_Work;
	CButton m_bt_UserPara_Del;
	CButton m_bt_UserPara_Rights;
	CButton m_bt_UserPara_Exit;

	enum { IDD = IDD_DLG_DLL_USERPARAS };

};

#endif

