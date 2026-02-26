/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                     */
/**   Date   : 29.06.2004                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once
#include "afxwin.h"

#ifndef _PDLGRIGHTDETAIL_H_____
#define _PDLGRIGHTDETAIL_H_____

/*----------------------------------------------------------------------------*/
//  Forward Declaration
/*----------------------------------------------------------------------------*/
class BppAppRight;
class BTest;

/*----------------------------------------------------------------------------*/
// PDlgUserDetail-Dialogfeld
/*----------------------------------------------------------------------------*/
class PDlgRightDetail : public CDialogMultiLang
{
	DECLARE_DYNAMIC(PDlgRightDetail)

public:
	PDlgRightDetail(BppAppRight * pBppAppRight); 
	virtual ~PDlgRightDetail();

	void		setBNewRi(bool bNewRi);
	const bool	getBNewRi();
	const bool	getBDataChanged();
	void		getCloneObj(BppAppRight * pRiChanged);
	void		setAppname(CBString strAppname);
	void		setModname(CBString strModname);
	void		setAppid (short sAppid);
	void		setModid (short sModid);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	virtual BOOL	OnInitDialog       ();
	virtual BOOL	PreTranslateMessage(MSG  * pMsg );

	bool GetCtrlValues();
	bool RightExists(CBString & strRiName, bool & bExists);

private:
	enum { IDD = IDD_DLG_RIGHT_DTL };

	enum Language
	{
		GERMAN = 0,
		ENGLISH,
		FRENCH,
		CROATIAN
	};

	void setLanguageString();
	bool saveLanguageString();

	bool m_LangInitialized;

	int m_iLangSel;
	
	// Generierte Nachrichtenzuordnungsfunktionen
	afx_msg void OnBtRiSave();
	afx_msg void OnBtRiExit();
	afx_msg void OnChangeLang();

	
	DECLARE_MESSAGE_MAP()


	BppAppRight   *	m_pBppRiClone;
	BppAppRight	*   pRiTestChange;
	bool			m_bRiNew;
	bool			m_bDataChanged;
	BTest	*		m_pTest;
	CBString		m_strAppname;
	CBString		m_strModname;
	short			m_sAppid;
	short			m_sModid;

	// #### Controls
	CEdit m_ed_Ri_Appname;
	CEdit m_ed_Ri_Modname;
	CEdit m_ed_RiName;
	CEdit m_ed_RiDesc;
	CButton m_bt_Ri_Save;
	CButton m_bt_Ri_Exit;
	CComboBox m_cb_Language;
	CComboBox m_cb_Category;
	CStatic m_txt_Category;
};

#endif

