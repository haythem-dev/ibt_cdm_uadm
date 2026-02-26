/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                     */
/**   Date   : 29.06.2004                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once
#include "afxwin.h"

#ifndef _PDLGMODDETAIL_H_____
#define _PDLGMODDETAIL_H_____

/*----------------------------------------------------------------------------*/
//  Forward Declaration
/*----------------------------------------------------------------------------*/
class BppAppMod;
class BTest;

/*----------------------------------------------------------------------------*/
// PDlgUserDetail-Dialogfeld
/*----------------------------------------------------------------------------*/
class PDlgModDetail : public CDialogMultiLang
{
	DECLARE_DYNAMIC(PDlgModDetail)

public:
	PDlgModDetail(BppAppMod * pBppAppMod); 
	virtual ~PDlgModDetail();

	void		setBNewMod(bool bNewMod);
	const bool	getBNewMod();
	const bool	getBDataChanged();
	void		getCloneObj(BppAppMod * pModChanged);
	void		setAppname(CBString strAppname);
	void		setAppid (short sAppid);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	virtual BOOL	OnInitDialog       ();
	virtual BOOL	PreTranslateMessage(MSG  * pMsg );

	bool GetCtrlValues();
	bool ModIDExists(short &sModid, bool & bExists);

private:
	enum { IDD = IDD_DLG_MOD_DTL };

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
	afx_msg void OnBtModSave();
	afx_msg void OnBtModExit();
	afx_msg void OnChangeLang();
	
	DECLARE_MESSAGE_MAP()


	BppAppMod   *	m_pBppModClone;
	BppAppMod	*   pModTestChange;
	bool			m_bModNew;
	bool			m_bDataChanged;
	BTest	*		m_pTest;
	CBString		m_strAppname;
	short			m_sAppid;


	// #### Controls
	CButton m_bt_Mod_Save;
	CButton m_bt_Mod_Exit;
	CEdit m_ed_Mod_Appid;
	CEdit m_ed_Modid;
	CEdit m_ed_Modname;
	CComboBox m_cb_Language;
};

#endif
