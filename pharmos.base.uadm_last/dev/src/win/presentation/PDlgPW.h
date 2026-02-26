/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH                                         */
/**   Date   : 18.08.2004                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once
#include "afxwin.h"


#ifndef _PDLGPW_H___
#define _PDLGPW_H___

/*----------------------------------------------------------------------------*/
class PDlgPW : public CDialogMultiLang
{
public:
					PDlgPW		    ();
				   ~PDlgPW		    ();

	bool PwOK();

private:

	virtual BOOL	OnInitDialog		();
	virtual void	DoDataExchange		(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	virtual BOOL	PreTranslateMessage(MSG           * pMsg );

	
	DECLARE_MESSAGE_MAP()

	afx_msg void OnBtPwOk();
	afx_msg void OnBtPwExit();

	CEdit m_ed_PW;
	CButton m_bt_PW_OK;
	CButton m_bt_PW_Exit;

    bool m_bPwOk;

	enum { IDD = IDD_DLG_PW };	

};

#endif
/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
