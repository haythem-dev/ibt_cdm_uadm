/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH                                         */
/**   Date   : 01.07.2004                                                     */
/**                                                                           */
/******************************************************************************/

#include "StdAfx.h"
#include "presentation.h"
#include <BppGroup.h>
#include <BConppGroup.h>
#include <BTest.h>
#include "PDlgGroupDetail.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/**---------------------------------------------------------------------------*/
// 
/**---------------------------------------------------------------------------*/
IMPLEMENT_DYNAMIC(PDlgGroupDetail, CDialogMultiLang)

/**---------------------------------------------------------------------------*/
PDlgGroupDetail::PDlgGroupDetail(BppGroup * pBppGroup)
				: CDialogMultiLang(PDlgGroupDetail::IDD ),
				m_pBppGrpClone (pBppGroup->clone())
{
	m_pTest = new BTest;
		assert (m_pTest);

	pGrpTestChange = pBppGroup->clone();
	
	assert(m_pBppGrpClone);
	
	m_bGrpNew = false;
	m_bDataChanged = false;
}

/**---------------------------------------------------------------------------*/
PDlgGroupDetail::~PDlgGroupDetail()
{
	if (m_pBppGrpClone != NULL)
	{
		delete m_pBppGrpClone;
		m_pBppGrpClone = NULL;
	} 

	if (pGrpTestChange != NULL)
	{
		delete pGrpTestChange;
		pGrpTestChange = NULL;
	}
	
	if (m_pTest != NULL)
	{
		delete m_pTest;
		m_pTest = NULL;
	}
}

/**---------------------------------------------------------------------------*/
void PDlgGroupDetail::DoDataExchange(CDataExchange* pDX)
{
	CDialogMultiLang::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_ED_GRP_NAME, m_ed_Grpname);
	DDX_Control(pDX, IDC_BT_GRP_SAVE, m_bt_Grp_Save);
	DDX_Control(pDX, IDC_BT_GRP_DTL_EXIT, m_bt_Grp_Exit);
	DDX_Control(pDX, IDC_ED_GRP_ID, m_ed_Grpid);
}

/**---------------------------------------------------------------------------*/
BOOL PDlgGroupDetail::OnInitDialog()
{
	BeginWaitCursor();

	CDialogMultiLang::OnInitDialog();
	
	if(m_bGrpNew == false)
	{
		m_ed_Grpid.SetWindowText(m_pBppGrpClone->getGroupidStr().c_str());
		m_ed_Grpname.SetWindowText(m_pBppGrpClone->getName().c_str());
		
		m_strGrpnamePrev = m_pBppGrpClone->getName();
	}

	m_ed_Grpname.SetFocus();

	EndWaitCursor();

	return FALSE;  // return TRUE unless you set the focus to a control
}

/**---------------------------------------------------------------------------*/
void PDlgGroupDetail::setBNewGrp(bool bNewGrp)
{
	m_bGrpNew = bNewGrp;
}

/**---------------------------------------------------------------------------*/
const bool PDlgGroupDetail::getBNewGrp()
{
	return m_bGrpNew;
}

/**---------------------------------------------------------------------------*/
const bool	PDlgGroupDetail::getBDataChanged()
{
	return m_bDataChanged;
}


/**---------------------------------------------------------------------------*/
bool PDlgGroupDetail::GetCtrlValues()
{
	//Daten von GUI
	CString csGrpName;
	m_ed_Grpname.GetWindowText(csGrpName);

	//Längenprüfung 
	CBString strGrpName = (LPCTSTR)csGrpName;
	strGrpName.erasespace(CBString::END);
	int iLengthN = (int)strGrpName.length();
	
	bool bGrpnameExists;

	if ( iLengthN > 30 || iLengthN <= 0)
	{
		MsgBoxOK(IDS_ERR_GRPNAME, MB_ICONERROR); //Kennungen nicht länger als 30 Zeichen oder leer
		m_ed_Grpname.SetFocus();
		return false;
	}
	else if (!(GrpNameExists(strGrpName, bGrpnameExists)))
	{
		MsgBoxOK(IDS_ERR_SQL_COUNT_GRPNAME);//SQL-Abfrage auf Grpname FAULT!
		return false;
	}
	else if (bGrpnameExists == true)
	{
		MsgBoxOK(IDS_ERR_GRPNAME_EXISTS, MB_ICONERROR);//Grpname schon vorhanden!
		m_ed_Grpname.SetFocus();
		return false;
	}	
	else
	{
		//BsnObj füllen
		m_pBppGrpClone->setName(strGrpName);
	}
	return true;
}

/**---------------------------------------------------------------------------*/
bool PDlgGroupDetail::GrpNameExists(CBString &strGrpName, bool & bExists)
{
	if( m_bGrpNew == false )
	{
		if ( m_strGrpnamePrev == strGrpName )
		{
			bExists = false;
			return true;
		}
	}
	
	m_pTest->setGrpname(strGrpName);
	
	if(!(m_pTest->SelGrpNameExists(bExists)))
	{
		return false;
	}
	return true;
}

/**---------------------------------------------------------------------------*/
void PDlgGroupDetail::getCloneObj(BppGroup * pGrpChanged)
{
	*pGrpChanged = *m_pBppGrpClone;
}


/**---------------------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(PDlgGroupDetail, CDialogMultiLang)
	ON_BN_CLICKED(IDC_BT_GRP_SAVE, OnBtGrpSave)
	ON_BN_CLICKED(IDC_BT_GRP_DTL_EXIT, OnBtGrpExit)
END_MESSAGE_MAP()

/**---------------------------------------------------------------------------*/
void PDlgGroupDetail::OnBtGrpSave()
{
	/*NASE - alle, die Lesen dürfen, dürfen auch Gruppen anlegen!
	CBString strRight = RIGHT_APPGRP_DLL;
	CBString strRight2 = RIGHT_WORK_RI;
	
	if (!(ppBLoginData::ex().isLegitimated(MODULE_DLL,strRight))
		&& !(ppBLoginData::ex().isLegitimated(MODULE_RIGHTS,strRight2)))
	{
		MsgBoxOK(IDS_NO_RIGHT);
		return;
	}*/

	if(GetCtrlValues()) //Daten von GUI eingelesen!
	{
		if(*pGrpTestChange == *m_pBppGrpClone) //keine Änderung!
		{
			m_bDataChanged = false;
		}
		else
		{
			if(m_bGrpNew == false) // UPDATE
			{
				BppGroup * pGrpUpdTest = new BppGroup;
				pGrpUpdTest->setGroupid(m_pBppGrpClone->getGroupid());
				IPerst::ERet Ret = pGrpUpdTest->SelForUpd();
				if (Ret != IPerst::RET_OK)
				{
					//kein Update --> Datensatz zum Updaten nicht gefunden!
					MsgBoxOK(IDS_NO_UPDATE_POSS);
				}
				else if (*pGrpUpdTest != *pGrpTestChange)
				{
					//kein Update --> Datensatz inzwischen extern verändert!
					MsgBoxOK(IDS_UPDATED_EXTERN);
				}
				else
				{
					IPerst::ERet eRet = m_pBppGrpClone->update();
					if (eRet == IPerst::RET_OK)
					{
						m_bDataChanged = true;
					}
					else
					{
						MsgBoxOK(IDS_ERR_UPDATE);
					}
				}
				delete pGrpUpdTest;
			}
			else //INSERT
			{
				IPerst::ERet eRet = m_pBppGrpClone->insert();
				if (eRet == IPerst::RET_OK)
				{
					eRet = m_pBppGrpClone->selectID();
					if (eRet != IPerst::RET_OK) //Fehler beim Insert
					{
						MsgBoxOK(IDS_ERR_INSERT);
					}
					m_bDataChanged = true;
				}
				else
				{
					MsgBoxOK(IDS_ERR_INSERT);
				}
			}
		}
		CDialogMultiLang::OnOK();
	}
}

/**---------------------------------------------------------------------------*/
void PDlgGroupDetail::OnBtGrpExit()
{
	CDialogMultiLang::OnCancel();
}

/**---------------------------------------------------------------------------*/
BOOL PDlgGroupDetail::PreTranslateMessage(MSG  * pMsg )
{
	assert(pMsg != 0);

	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			// Fokus steht in UsrListCtrl
			if (GetFocus() == GetDlgItem(IDC_BT_GRP_SAVE))
			{
				if (m_bt_Grp_Save.IsWindowEnabled())
				{
					OnBtGrpSave();
					return TRUE;
				}
			}
			// Fokus steht in AppListCtrl
			if (GetFocus() == GetDlgItem(IDC_BT_GRP_DTL_EXIT))
			{
				if (m_bt_Grp_Exit.IsWindowEnabled())
				{
					OnBtGrpExit();
					return TRUE;
				}
			}
			return FALSE;
			
		case VK_ESCAPE:
			if (m_bt_Grp_Exit.IsWindowEnabled())
			{
				OnBtGrpExit();
				return TRUE;
			}
			return FALSE;


		default:
			break;
		}
	} 

	return CDialogMultiLang::PreTranslateMessage(pMsg);
}


