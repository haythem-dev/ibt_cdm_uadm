/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                     */
/**   Date   : 01.07.2004                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once
#include "afxwin.h"

#ifndef _PDLGGROUPDETAIL_H_____
#define _PDLGGROUPDETAIL_H_____

/*----------------------------------------------------------------------------*/
//  Forward Declaration
/*----------------------------------------------------------------------------*/
class BppGroup;
class BTest;

/*----------------------------------------------------------------------------*/
// 
/*----------------------------------------------------------------------------*/
class PDlgGroupDetail : public CDialogMultiLang
{
	DECLARE_DYNAMIC(PDlgGroupDetail)

public:
	PDlgGroupDetail(BppGroup * pBppGroup); 
	virtual ~PDlgGroupDetail();

	void		setBNewGrp(bool bNewGrp);
	const bool	getBNewGrp();
	const bool	getBDataChanged();
	void		getCloneObj(BppGroup * pGrpChanged);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	virtual BOOL	OnInitDialog       ();
	virtual BOOL	PreTranslateMessage(MSG  * pMsg );

	bool GetCtrlValues();
	bool GrpNameExists(CBString &strGrpName, bool & bExists);

private:
	enum { IDD = IDD_DLG_GROUP_DTL };
	
	// Generierte Nachrichtenzuordnungsfunktionen
	afx_msg void OnBtGrpSave();
	afx_msg void OnBtGrpExit();
	
	DECLARE_MESSAGE_MAP()


	BppGroup   *	m_pBppGrpClone;
	BppGroup	 *  pGrpTestChange;
	bool		m_bGrpNew;
	bool		m_bDataChanged;
	BTest	*	m_pTest;
	CBString   m_strGrpnamePrev;


	// #### Controls
	CEdit m_ed_Grpname;
	CButton m_bt_Grp_Save;
	CButton m_bt_Grp_Exit;
	CEdit m_ed_Grpid;

};

#endif

