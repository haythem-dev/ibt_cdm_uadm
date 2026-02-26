/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                     */
/**   Date   : 05.07.2004                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once
#include "afxwin.h"

#ifndef __PDLGDLLLANGTEXT_H____
#define __PDLGDLLLANGTEXT_H____

/*----------------------------------------------------------------------------*/
//  Forward Declaration
/*----------------------------------------------------------------------------*/
class PDlgLangListCtrl;
class PDlgAppListCtrl;
class BppLang; 



class PDlgDllLangText : public CDialogMultiLang
{
public:	
	DECLARE_DYNCREATE(PDlgDllLangText)

					PDlgDllLangText();           
	virtual			~PDlgDllLangText();
	virtual BOOL	OnInitDialog();

protected:
	virtual void	DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	virtual BOOL	PreTranslateMessage(MSG           * pMsg );

private:
	// Generierte Nachrichtenzuordnungsfunktionen
	afx_msg void OnDblclkLangCORListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnItemchangedLangCORListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnItemchangedAppListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblclkAppListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblclkLangALLListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnItemchangedLangALLListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	
	afx_msg void OnNMSetfocusLangCOR(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMSetfocusLangALL(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickLangCOR(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickLangALL(NMHDR *pNMHDR, LRESULT *pResult);
	
	DECLARE_MESSAGE_MAP()

	afx_msg void OnBtLangTextSelApp();
	afx_msg void OnBtLangCor();
	afx_msg void OnBtLangErase();
	afx_msg void OnBtWorkCorLang();
	afx_msg void OnBtLangNew();
	afx_msg void OnBtLangWork();
	afx_msg void OnBtLangDel();
	afx_msg void OnBtLangTextExit();

	void           SetLangCORCtrlState  (const int iItem);
	void           SetLangALLCtrlState  (const int iItem);
	
	PDlgLangListCtrl  * m_pli_LangCOR;
	PDlgLangListCtrl  * m_pli_LangALL;
	PDlgAppListCtrl	   * m_pli_App;

	CButton m_bt_SelApp;
	CButton m_bt_Cor_Lang;
	CButton m_bt_Erase_Lang;
	CButton m_bt_Work_CORLang;
	CButton m_bt_Lang_New;
	CButton m_bt_Lang_Work;
	CButton m_bt_Lang_Del;
	CButton m_bt_Lang_Text_Exit;

	short m_sAppid;
	bool m_bLoadNew;
		
	enum { IDD = IDD_DLG_DLL_LANG_TEXT };

};

#endif
