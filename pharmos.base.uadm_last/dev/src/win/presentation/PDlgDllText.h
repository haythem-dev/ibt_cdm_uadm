/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                     */
/**   Date   : 05.07.2004                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once
#include "afxwin.h"

#ifndef __PDLGDLLTEXT_H____
#define __PDLGDLLTEXT_H____

/*----------------------------------------------------------------------------*/
//  Forward Declaration
/*----------------------------------------------------------------------------*/
class PDlgLangListCtrl;
class PDlgTextListCtrl;
class BppText; 



class PDlgDllText : public CDialogMultiLang
{
public:	
	DECLARE_DYNCREATE(PDlgDllText)

					PDlgDllText();           
	virtual			~PDlgDllText();
	virtual BOOL	OnInitDialog();

protected:
	virtual void	DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	virtual BOOL	PreTranslateMessage(MSG           * pMsg );

private:
	// Generierte Nachrichtenzuordnungsfunktionen
	afx_msg void OnDblclkLangListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnItemchangedLangListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnItemchangedTextCORListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblclkTextCORListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblclkTextALLListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnItemchangedTextALLListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	
	afx_msg void OnNMSetfocusTextCOR(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMSetfocusTextALL(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickTextCOR(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickTextALL(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnBtTxtSelLang();
	afx_msg void OnBtTxtCor();
	afx_msg void OnBtTxtErase();
	afx_msg void OnBtWorkTxtcor();
	afx_msg void OnBtTxtNew();
	//afx_msg void OnBtTxtWork();
	afx_msg void OnBtTxtDel();
	afx_msg void OnBtTexteExit();
	
	DECLARE_MESSAGE_MAP()

	void           SetTextCORCtrlState  (const int iItem);
	void           SetTextALLCtrlState  (const int iItem);
	
	PDlgTextListCtrl  * m_pli_TextCOR;
	PDlgLangListCtrl  * m_pli_Lang;
	PDlgTextListCtrl  * m_pli_TextALL;

	CButton m_bt_Txt_SelLang;
	CButton m_bt_Txt_Cor;
	CButton m_bt_Txt_Erase;
	CButton m_bt_Txt_WorkCor;
	CButton m_bt_Txt_New;
	CButton m_bt_Txt_Work;
	CButton m_bt_Txt_Del;
	CButton m_bt_Txt_Exit;
	
	CBString m_strLangid;
		
	enum { IDD = IDD_DLG_DLL_TEXTE };

};

#endif

