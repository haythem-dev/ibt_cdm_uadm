/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                     */
/**   Date   : 30.08.2004                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once
#include "afxwin.h"

#ifndef __PDLGRIANALYSIS_H____
#define __PDLGRIANALYSIS_H____

/*----------------------------------------------------------------------------*/
//  Forward Declaration
/*----------------------------------------------------------------------------*/
class PDlgRiAnalysisListCtrl;
class PDlgAppListCtrl;
class PDlgUserListCtrl;
class BRightAnalysis; 
class PDlgBranchListCtrl;
class PDlgRegionListCtrl;



class PDlgRiAnalysis : public CDialogMultiLang
{
public:	
	DECLARE_DYNCREATE(PDlgRiAnalysis)

					PDlgRiAnalysis();           
	virtual			~PDlgRiAnalysis();
	virtual BOOL	OnInitDialog();

protected:
	virtual void	DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	virtual BOOL	PreTranslateMessage(MSG           * pMsg );

private:
	// Generierte Nachrichtenzuordnungsfunktionen
	afx_msg void OnDblclkUserListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnItemchangedUserListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnItemchangedAppListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblclkAppListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblclkBranchListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnItemchangedBranchListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnItemchangedRegListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblclkRegListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBtAnalySelApp();
	afx_msg void OnBtAnalySelUsr();
	afx_msg void OnBtAnalySelBr();
	afx_msg void OnBtAnalySelReg();
	afx_msg void OnBtAnalyPrint();
	afx_msg void OnBtAnalyExit();
	afx_msg void OnBtAnalySelRights();

	afx_msg void OnClickedAllBrCk();
	afx_msg void OnClickedAllRegCk();
	afx_msg void OnClickedAllUserCk();
	
	void           SetRiCtrlState  (const int iItem);

	void           PrintHead        (CPrint * pPrint);
	void           PrintRows        (CPrint * pPrint);
	
	PDlgRiAnalysisListCtrl  * m_pli_RiAnaly;
	PDlgUserListCtrl		* m_pli_User;
	PDlgAppListCtrl			* m_pli_App;
	PDlgBranchListCtrl		* m_pli_Branch;
	PDlgRegionListCtrl		* m_pli_Region;

	CButton m_ck_AllReg;
	CButton m_ck_AllBr;
	CButton m_ck_AllUser;
		
	CButton m_bt_Analy_App;
	CButton m_bt_Analy_User;
	CButton m_bt_Analy_Br;
	CButton m_bt_Analy_Reg;
	CButton m_bt_Analy_Print;
	CButton m_bt_Analy_Exit;
	CButton m_bt_Analy_SelRights;
	CStatic m_txt_Analy_Desc;

	short m_sAppid;
	short m_sUserid;
	CBString m_strAppname;
	CBString m_strUsername;

	bool  m_bAppOnly;
	bool  m_bUsrOnly;

	enum { IDD = IDD_DLG_RI_ANALYSIS };
};

#endif
