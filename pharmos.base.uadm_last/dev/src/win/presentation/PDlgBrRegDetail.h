/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                     */
/**   Date   : 30.06.2004                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once
#include "afxwin.h"

#ifndef _PDLGBRREGDETAIL_H_____
#define _PDLGBRREGDETAIL_H_____

/*----------------------------------------------------------------------------*/
//  Forward Declaration
/*----------------------------------------------------------------------------*/
class BppBranchReg;
class BTest;

/*----------------------------------------------------------------------------*/
// PDlgUserDetail-Dialogfeld
/*----------------------------------------------------------------------------*/
class PDlgBrRegDetail : public CDialogMultiLang
{
	DECLARE_DYNAMIC(PDlgBrRegDetail)

public:
	PDlgBrRegDetail(BppBranchReg * pBppBrReg); 
	virtual ~PDlgBrRegDetail();

	void		setBNewBrReg(bool bNewBrReg);
	const bool	getBNewBrReg();
	const bool	getBDataChanged();
	void		getCloneObj(BppBranchReg * pBrRegChanged);
	void		setBBranch (bool bBranch);
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	virtual BOOL	OnInitDialog       ();
	virtual BOOL	PreTranslateMessage(MSG  * pMsg );

	bool GetCtrlValues();
	bool BrRegIDExists(short &sBrRegid, bool & bExists);

private:
	enum { IDD = IDD_DLG_BRANCH_REG_DTL };
	
	// Generierte Nachrichtenzuordnungsfunktionen
	afx_msg void OnBtBrregSave();
	afx_msg void OnBtBrregExit();
	
	DECLARE_MESSAGE_MAP()


	BppBranchReg   *	m_pBppBrRegClone;
	BppBranchReg	*   pBrRegTestChange;
	bool				m_bBrRegNew;
	bool				m_bDataChanged;
	BTest	*			m_pTest;
	bool				m_bBranch;
	
	// #### Controls
	CStatic m_txt_IDValue;
	CEdit m_ed_ID;
	CEdit m_ed_Name;
	CButton m_bt_BrReg_Save;
	CButton m_bt_BrReg_Exit;

};

#endif

