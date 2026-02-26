/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                     */
/**   Date   : 05.07.2004                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once
#include "afxwin.h"

#ifndef _PDLGLANGDETAIL_H_____
#define _PDLGLANGDETAIL_H_____

/*----------------------------------------------------------------------------*/
//  Forward Declaration
/*----------------------------------------------------------------------------*/
class BppLanguage;
class BTest;

/*----------------------------------------------------------------------------*/
// 
/*----------------------------------------------------------------------------*/
class PDlgLangDetail : public CDialogMultiLang
{
	DECLARE_DYNAMIC(PDlgLangDetail)

public:
	PDlgLangDetail(BppLanguage * pBppLang); 
	virtual ~PDlgLangDetail();

	void		setBNewLang(bool bNewLang);
	const bool	getBNewLang();
	const bool	getBDataChanged();
	void		getCloneObj(BppLanguage * pLangChanged);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	virtual BOOL	OnInitDialog       ();
	virtual BOOL	PreTranslateMessage(MSG  * pMsg );

	bool GetCtrlValues();
	bool LangIDExists(CBString &strLangid, bool & bExists);

private:
	enum { IDD = IDD_DLG_DTL_LANG };
	
	// Generierte Nachrichtenzuordnungsfunktionen
	afx_msg void OnBtLangdtlSave();
	afx_msg void OnBtLangdtlExit();
	
	
	DECLARE_MESSAGE_MAP()


	BppLanguage   *	m_pBppLangClone;
	BppLanguage	 *  pLangTestChange;
	bool		m_bLangNew;
	bool		m_bDataChanged;
	BTest	*	m_pTest;
	CBString   m_strLangidPrev;
	CBString	m_strLangname;


	// #### Controls
	CEdit m_ed_Langid;
	CEdit m_ed_Langname;
	CButton m_bt_Lang_Save;
	CButton m_bt_Lang_Exit;

};

#endif
