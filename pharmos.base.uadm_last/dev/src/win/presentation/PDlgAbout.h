/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH                                         */
/**   Date   : 27.05.2004                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once


#ifndef _PDLGABOUT_H___
#define _PDLGABOUT_H___

/*----------------------------------------------------------------------------*/
class PDlgAbout : public CDialogMultiLang
{
public:
					PDlgAbout		    ();
				   ~PDlgAbout		    ();

private:

	virtual BOOL	OnInitDialog		();
	virtual void	DoDataExchange		(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	
	afx_msg void	OnBnClickedBtOk();
	afx_msg void	OnClickedTxtUrl();
	afx_msg HBRUSH	OnCtlColor			(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL	OnSetCursor			(CWnd* pWnd, UINT nHitTest, UINT message);
	
	DECLARE_MESSAGE_MAP()

    void			GetBuildDate		(CString csFilename, CString& csDate);

	CFont		m_cfFont;
	HCURSOR		m_hCursor;
	COLORREF	m_crText;

	enum { IDD = IDD_DLG_ABOUT };

	CButton		m_bt_OK;
	CStatic     m_txt_Username;
	CStatic		m_txt_Version;
	CStatic		m_txt_Date;
	CStatic		m_txt_Copyright;
	CStatic		m_txt_Product;
	CStatic		m_txt_Doku;
	CStatic     m_txt_Hostname;
	CStatic     m_txt_Dbname;
	CListBox	m_li_Roles;
	CListCtrl   m_li_Dll;
	
};

#endif
/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
