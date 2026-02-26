/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                     */
/**   Date   : 03.08.2004                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once
#include "afxwin.h"

#ifndef __PDLGDLLPARA_H____
#define __PDLGDLLPARA_H____

/*----------------------------------------------------------------------------*/
//  Forward Declaration
/*----------------------------------------------------------------------------*/
class PDlgAppListCtrl;
class PDlgParaListCtrl;


class PDlgDllPara : public CDialogMultiLang
{
public:	
	DECLARE_DYNCREATE(PDlgDllPara)

					PDlgDllPara();           
	virtual			~PDlgDllPara();
	virtual BOOL	OnInitDialog();

protected:
	virtual void	DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	virtual BOOL	PreTranslateMessage(MSG   * pMsg );

private:
	// Generierte Nachrichtenzuordnungsfunktionen
	afx_msg void OnDblclkParaListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnItemchangedParaListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnItemchangedAppListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblclkAppListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickLiPara(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnBtParaApp();
	afx_msg void OnBtParaNew();
	afx_msg void OnBtParaWork();
	afx_msg void OnBtParaDel();
	afx_msg void OnBtParaExit();


	DECLARE_MESSAGE_MAP()
	
	CButton m_bt_Para_App;
	CButton m_bt_Para_New;
	CButton m_bt_Para_Work;
	CButton m_bt_Para_Del;
	CButton m_bt_Para_Exit;

	void           SetAppCtrlState     (const int iItem);
	void		   SetParaCtrlState	   (const int iItem);
	
	PDlgAppListCtrl		* m_pli_App;
	PDlgParaListCtrl	* m_pli_Para;

	short m_sAppid;
			
	enum { IDD = IDD_DLG_DLL_PARA };
};

#endif
