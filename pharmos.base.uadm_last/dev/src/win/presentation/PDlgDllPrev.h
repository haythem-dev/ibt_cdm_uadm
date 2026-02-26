/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                     */
/**   Date   : 16.08.2004                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once
#include "afxwin.h"

#ifndef __PDLGDLLPREV_H____
#define __PDLGDLLPREV_H____

/*----------------------------------------------------------------------------*/
//  Forward Declaration
/*----------------------------------------------------------------------------*/
class PDlgPrevListCtrl;
class PDlgAppListCtrl;
class BppPrev; 


class PDlgDllPrev : public CDialogMultiLang
{
public:	
	DECLARE_DYNCREATE(PDlgDllPrev)

					PDlgDllPrev();           
	virtual			~PDlgDllPrev();
	virtual BOOL	OnInitDialog();

protected:
	virtual void	DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	virtual BOOL	PreTranslateMessage(MSG           * pMsg );

private:
	// Generierte Nachrichtenzuordnungsfunktionen
	afx_msg void OnItemchangedListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnItemchangedAppListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblclkAppListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBtPrevApp();
	afx_msg void OnBtPrevExit();
	afx_msg void OnBtPrevDel();
	

	DECLARE_MESSAGE_MAP()

	CButton m_bt_App_Prev_Sel;
	CButton m_bt_Prev_Exit;
	CButton m_bt_Prev_Del;

	void           SetPrevCtrlState     (const int iItem);
	void           SetAppCtrlState     (const int iItem);
	
	PDlgPrevListCtrl  * m_pli_Prev;
	PDlgAppListCtrl	   * m_pli_App;

	short m_sAppid;


		
	enum { IDD = IDD_DLG_PREVIOUS };
};

#endif

