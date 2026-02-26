/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                     */
/**   Date   : 04.08.2004                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once
#include "afxwin.h"

#ifndef _PDLGPARADETAIL_H_____
#define _PDLGPARADETAIL_H_____

/*----------------------------------------------------------------------------*/
//  Forward Declaration
/*----------------------------------------------------------------------------*/
class BppPara;
class BTest;

/*----------------------------------------------------------------------------*/
// PDlgParaDetail-Dialogfeld
/*----------------------------------------------------------------------------*/
class PDlgParaDetail : public CDialogMultiLang
{
	DECLARE_DYNAMIC(PDlgParaDetail)

public:
	PDlgParaDetail(BppPara * pBppPara); 
	virtual ~PDlgParaDetail();

	void		setBNewPara(bool bNewPara);
	const bool	getBNewPara();
	const bool	getBDataChanged();
	void		getCloneObj(BppPara * pParaChanged);
	void		setAppname(CBString strAppname);
	void		setAppid (short sAppid);
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	virtual BOOL	OnInitDialog       ();
	virtual BOOL	PreTranslateMessage(MSG  * pMsg );

	bool GetCtrlValues();
	bool ParaExists(CBString & strPara1, CBString & strPara2, CBString& strParaBez, bool & bExists);

	
private:
	enum { IDD = IDD_DLG_DTL_PARA };
	
	// Generierte Nachrichtenzuordnungsfunktionen
	afx_msg void OnBtDtlParaSave();
	afx_msg void OnBtDtlParaExit();

	
	DECLARE_MESSAGE_MAP()


	BppPara   *	m_pBppParaClone;
	BppPara	*   pParaTestChange;
	BTest	*		m_pTest;
	bool			m_bParaNew;
	bool			m_bDataChanged;
	CBString		m_strAppname;
	short			m_sAppid;
	
	// #### Controls
	CEdit m_ed_Para1;
	CEdit m_ed_Para2;
	CEdit m_ed_ParaBez;
	CEdit m_ed_ParaValue;
	CEdit m_ed_Para_App;
	CButton m_bt_Para_Save;
	CButton m_bt_Para_Exit;
	
};

#endif
