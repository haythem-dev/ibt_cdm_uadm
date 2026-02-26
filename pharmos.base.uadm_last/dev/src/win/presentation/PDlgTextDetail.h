/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                     */
/**   Date   : 07.07.2004                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once
#include "afxwin.h"

#ifndef _PDLGTEXTDETAIL_H_____
#define _PDLGTEXTDETAIL_H_____

/*----------------------------------------------------------------------------*/
//  Forward Declaration
/*----------------------------------------------------------------------------*/
class BppText;
class BTest;

/*----------------------------------------------------------------------------*/
// 
/*----------------------------------------------------------------------------*/
class PDlgTextDetail : public CDialogMultiLang
{
	DECLARE_DYNAMIC(PDlgTextDetail)

public:
	PDlgTextDetail(BppText * pBppText); 
	virtual ~PDlgTextDetail();

	void		setBNewText(bool bNewText);
	const bool	getBNewText();
	void		setBNewLang(bool bNewLang);
	const bool	getBNewLang();
	void		setBNewRefText(bool bNewRefText);
	const bool	getBNewRefText();
	const bool	getBDataChanged();
	void		getCloneObj(BppText * pTextChanged);
	bool		ReftextExists(CBString &strReftext, bool & bExists);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	virtual BOOL	OnInitDialog       ();
	virtual BOOL	PreTranslateMessage(MSG  * pMsg );

	bool GetCtrlValues();


private:
	enum { IDD = IDD_DLG_DTL_TEXT };
	
	// Generierte Nachrichtenzuordnungsfunktionen
	afx_msg void OnBtTxtDtlSave();
	afx_msg void OnBtTxtDtlExit();
	
	
	DECLARE_MESSAGE_MAP()


	BppText   *	m_pBppTextClone;
	BppText	 *  pTextTestChange;
	bool		m_bTextNew;
	bool		m_bRefTextNew;
	bool		m_bDataChanged;
	bool		m_bNewLang;
	BTest	*	m_pTest;
	CBString   m_strReftextPrev;


	// #### Controls
	CEdit m_ed_Lang;
	CEdit m_ed_Reftext;
	CEdit m_ed_Text_EN;
	CEdit m_ed_Text;
	CButton m_bt_Txt_Save;
	CButton m_bt_Txt_Exit;
	CStatic m_txt_Message;
	
};

#endif
