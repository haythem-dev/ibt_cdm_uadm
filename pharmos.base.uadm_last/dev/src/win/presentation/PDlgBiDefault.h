/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                     */
/**   Date   : 06.06.2005                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once
#include "afxwin.h"

#ifndef __PDLGBIDEFAULT_H__
#define __PDLGBIDEFAULT_H__

/*----------------------------------------------------------------------------*/
//  Forward Declaration
/*----------------------------------------------------------------------------*/
class BBiDefault;

/*----------------------------------------------------------------------------*/
class PDlgBiDefault : public CDialogMultiLang
{
	DECLARE_DYNAMIC(PDlgBiDefault)

public:
	PDlgBiDefault(const CBString& cbNtUser, 
				  const CBString& cbSurname, const CBString& cbForename, const CBString& cbEmail); 
	virtual ~PDlgBiDefault();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	virtual BOOL	OnInitDialog       ();
	virtual BOOL	PreTranslateMessage(MSG  * pMsg );

	bool GetCtrlValues();
	
private:
	enum { IDD = IDD_DLG_BI_DEFAULT };
	
	// Generierte Nachrichtenzuordnungsfunktionen
	afx_msg void OnBtBiDefaultSave();
	afx_msg void OnBtBiDefaultExit();
	afx_msg void OnBtIbiCountryR();
	
	DECLARE_MESSAGE_MAP()

	CBString   m_cbNtUser;
	CBString   m_cbSurname;
	CBString   m_cbForename;
	CBString   m_cbEmail;

	// #### Controls
	CButton m_bt_BiDef_Save;
	CButton m_bt_BiDef_Exit;
	CButton m_bt_Ibi_CountryR;
	CComboBox m_cb_Country;
	CEdit m_ed_NtUser;
	CEdit m_ed_Surname;
	CEdit m_ed_Forename;
	CEdit m_ed_Email;
	CEdit m_ed_Remark;
	CEdit m_ed_Branch;
	//CEdit m_ed_Country;
	CEdit m_ed_Style;
	CEdit m_ed_Format;

	BBiDefault  *	m_pobjBiDef;	
	
	bool ConnectOdbc();
};

#endif