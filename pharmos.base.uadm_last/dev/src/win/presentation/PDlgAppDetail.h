/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                     */
/**   Date   : 28.06.2004                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once
#include "afxwin.h"

#ifndef _PDLGAPPDETAIL_H_____
#define _PDLGAPPDETAIL_H_____

/*----------------------------------------------------------------------------*/
//  Forward Declaration
/*----------------------------------------------------------------------------*/
class BppApp;
class BTest;

/*----------------------------------------------------------------------------*/
// PDlgUserDetail-Dialogfeld
/*----------------------------------------------------------------------------*/
class PDlgAppDetail : public CDialogMultiLang
{
	DECLARE_DYNAMIC(PDlgAppDetail)

public:
	PDlgAppDetail(BppApp * pBppApp); 
	virtual ~PDlgAppDetail();

	void		setBNewApp(bool bNewApp);
	const bool	getBNewApp();
	const bool	getBDataChanged();
	void		getCloneObj(BppApp * pAppChanged);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	virtual BOOL	OnInitDialog       ();
	virtual BOOL	PreTranslateMessage(MSG  * pMsg );

	bool GetCtrlValues();
	bool InsertNewRight();
	bool UpdateApplRight();
	bool AppNameExists(CBString &strAppName, bool & bExists);

private:
	enum { IDD = IDD_DLG_APP_DTL };

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
	afx_msg void OnBtAppSave();
	afx_msg void OnBtAppExit();
	afx_msg void OnChangeLang();
	
	DECLARE_MESSAGE_MAP()


	BppApp   *	m_pBppAppClone;
	BppApp	 *  pAppTestChange;
	bool		m_bAppNew;
	bool		m_bDataChanged;
	BTest	*	m_pTest;
	CBString    m_strAppnamePrev;


	// #### Controls
	CButton m_bt_App_Save;
	CButton m_bt_App_Exit;
	CEdit m_ed_Appid;
	CEdit m_ed_Appname;
	CEdit m_ed_AppDesc;
	CComboBox m_cb_Language;
};

#endif

