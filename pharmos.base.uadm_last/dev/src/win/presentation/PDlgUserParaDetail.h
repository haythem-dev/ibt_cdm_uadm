/******************************************************************************/
/**                                                                           */
/**   Author : T.ARNOLD NTTDATA                                               */
/**   Date   : 25.11.2025                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once
#include "afxwin.h"

#ifndef _PDLGUSERPARADETAIL_H_____
#define _PDLGUSERPARADETAIL_H_____

/*----------------------------------------------------------------------------*/
//  Forward Declaration
/*----------------------------------------------------------------------------*/
class BppUserPara;
class BTest;

/*----------------------------------------------------------------------------*/
// 
/*----------------------------------------------------------------------------*/
class PDlgUserParaDetail : public CDialogMultiLang
{
	DECLARE_DYNAMIC(PDlgUserParaDetail)

public:
	PDlgUserParaDetail(BppUserPara* pBppUserPara);
	virtual ~PDlgUserParaDetail();

	void		setBNewUserPara(bool bNewGrp);
	const bool	getBNewUserPara();
	const bool	getBDataChanged();
	void		getCloneObj(BppUserPara * pUserParaChanged);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	virtual BOOL	OnInitDialog       ();
	virtual BOOL	PreTranslateMessage(MSG  * pMsg );

	bool GetCtrlValues();
	bool UserParaNameExists(CBString &strUserParaName, bool & bExists);

private:
	enum { IDD = IDD_DLG_USERPARAS_DTL };
	
	// Generierte Nachrichtenzuordnungsfunktionen
	afx_msg void OnBtUserParaSave();
	afx_msg void OnBtUserParaExit();
	
	DECLARE_MESSAGE_MAP()


	BppUserPara*  m_pBppUserParaClone;
	BppUserPara*  pUserParaTestChange;
	bool		  m_bUserParaNew;
	bool		  m_bDataChanged;
	BTest*   	  m_pTest;
	CBString      m_strUserParanamePrev;
	CBString      m_strUserParadescPrev;


	// #### Controls
	CEdit m_ed_UserParaid;
	CEdit m_ed_UserParaname;
	CEdit m_ed_UserParadesc;
	CButton m_bt_UserPara_Save;
	CButton m_bt_UserPara_Exit;
};

#endif

