/******************************************************************************/
/**                                                                           */
/**   Author : T.ARNOLD NTTDATA                                               */
/**   Date   : 25.11.2025                                                     */
/**                                                                           */
/******************************************************************************/

#include "StdAfx.h"
#include "presentation.h"
#include <BppUserpara.h>
#include <tbl_ppuserpara.h>
#include <BConppUserPara.h>
#include <BTest.h>
#include "PDlgUserParaDetail.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/**---------------------------------------------------------------------------*/
// 
/**---------------------------------------------------------------------------*/
IMPLEMENT_DYNAMIC(PDlgUserParaDetail, CDialogMultiLang)

/**---------------------------------------------------------------------------*/
PDlgUserParaDetail::PDlgUserParaDetail(BppUserPara* pBppUserPara)
				: CDialogMultiLang(PDlgUserParaDetail::IDD ),
				m_pBppUserParaClone (pBppUserPara->clone())
{
	m_pTest = new BTest;
		assert (m_pTest);

	pUserParaTestChange = pBppUserPara->clone();
	
	assert(m_pBppUserParaClone);
	
	m_bUserParaNew = false;
	m_bDataChanged = false;
	m_strUserParanamePrev = "";
	m_strUserParadescPrev = "";
}

/**---------------------------------------------------------------------------*/
PDlgUserParaDetail::~PDlgUserParaDetail()
{
	if (m_pBppUserParaClone != NULL)
	{
		delete m_pBppUserParaClone;
		m_pBppUserParaClone = NULL;
	} 

	if (pUserParaTestChange != NULL)
	{
		delete pUserParaTestChange;
		pUserParaTestChange = NULL;
	}
	
	if (m_pTest != NULL)
	{
		delete m_pTest;
		m_pTest = NULL;
	}
}

/**---------------------------------------------------------------------------*/
void PDlgUserParaDetail::DoDataExchange(CDataExchange* pDX)
{
	CDialogMultiLang::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_ED_USERPARA_NAME, m_ed_UserParaname);
	DDX_Control(pDX, IDC_ED_USERPARA_DESC, m_ed_UserParadesc);
	DDX_Control(pDX, IDC_BT_USERPARA_SAVE, m_bt_UserPara_Save);
	DDX_Control(pDX, IDC_BT_USERPARA_DTL_EXIT, m_bt_UserPara_Exit);
	DDX_Control(pDX, IDC_ED_USERPARA_ID, m_ed_UserParaid);
}

/**---------------------------------------------------------------------------*/
BOOL PDlgUserParaDetail::OnInitDialog()
{
	BeginWaitCursor();

	CDialogMultiLang::OnInitDialog();
	
	if(m_bUserParaNew == false)
	{
		m_ed_UserParaid.SetWindowText(m_pBppUserParaClone->getUserParaidStr().c_str());
		m_ed_UserParaname.SetWindowText(m_pBppUserParaClone->getName().c_str());
		m_ed_UserParadesc.SetWindowText(m_pBppUserParaClone->getDescription().c_str());
		
		m_strUserParanamePrev = m_pBppUserParaClone->getName();
		m_strUserParadescPrev = m_pBppUserParaClone->getDescription();
	}

	m_ed_UserParaname.SetFocus();

	EndWaitCursor();

	return FALSE;  // return TRUE unless you set the focus to a control
}

/**---------------------------------------------------------------------------*/
void PDlgUserParaDetail::setBNewUserPara(bool bNewUserPara)
{
	m_bUserParaNew = bNewUserPara;
}

/**---------------------------------------------------------------------------*/
const bool PDlgUserParaDetail::getBNewUserPara()
{
	return m_bUserParaNew;
}

/**---------------------------------------------------------------------------*/
const bool	PDlgUserParaDetail::getBDataChanged()
{
	return m_bDataChanged;
}


/**---------------------------------------------------------------------------*/
bool PDlgUserParaDetail::GetCtrlValues()
{
	//Daten von GUI
	CString csUserParaName;
	m_ed_UserParaname.GetWindowText(csUserParaName);
	CString csUserParaDesc;
	m_ed_UserParadesc.GetWindowText(csUserParaDesc);

	//Längenprüfung 
	CBString strUserParaName = (LPCTSTR)csUserParaName;
	strUserParaName.erasespace(CBString::END);
	int iLengthN = (int)strUserParaName.length();
	CBString strUserParaDesc = (LPCTSTR)csUserParaDesc;
	strUserParaDesc.erasespace(CBString::END);
	int iLengthD = (int)strUserParaDesc.length();
	bool bUserParanameExists;

	if ( iLengthN > L_PPUSERPARA_NAME || iLengthN <= 0)
	{
		MsgBoxOK(IDS_ERR_USERPARANAME, MB_ICONERROR); //Kennungen nicht länger als 150 Zeichen oder leer
		m_ed_UserParaname.SetFocus();
		return false;
	}
	if (iLengthD > L_PPUSERPARA_DESC)
	{
		MsgBoxOK(IDS_ERR_USERPARADESC, MB_ICONERROR); //Kennungen nicht länger als 254 Zeichen
		m_ed_UserParadesc.SetFocus();
		return false;
	}

	else if (!(UserParaNameExists(strUserParaName, bUserParanameExists)))
	{
		MsgBoxOK(IDS_ERR_SQL_COUNT_USERPARANAME); //SQL-Abfrage auf UserParaname FAULT!
		return false;
	}
	else if (bUserParanameExists == true)
	{
		MsgBoxOK(IDS_ERR_USERPARANAME_EXISTS, MB_ICONERROR); //UserParaname schon vorhanden!
		m_ed_UserParaname.SetFocus();
		return false;
	}	
	else
	{
		//BsnObj füllen
		m_pBppUserParaClone->setName(strUserParaName);
		m_pBppUserParaClone->setDescription(strUserParaDesc);
	}
	return true;
}

/**---------------------------------------------------------------------------*/
bool PDlgUserParaDetail::UserParaNameExists(CBString &strUserParaName, bool & bExists)
{
	if( m_bUserParaNew == false )
	{
		if ( m_strUserParanamePrev == strUserParaName )
		{
			bExists = false;
			return true;
		}
	}
	
	m_pTest->setUserParaName(strUserParaName);
	
	if(!(m_pTest->SelUserParaNameExists(bExists)))
	{
		return false;
	}
	return true;
}

/**---------------------------------------------------------------------------*/
void PDlgUserParaDetail::getCloneObj(BppUserPara* pUserParaChanged)
{
	*pUserParaChanged = *m_pBppUserParaClone;
}


/**---------------------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(PDlgUserParaDetail, CDialogMultiLang)
	ON_BN_CLICKED(IDC_BT_USERPARA_SAVE, OnBtUserParaSave)
	ON_BN_CLICKED(IDC_BT_USERPARA_DTL_EXIT, OnBtUserParaExit)
END_MESSAGE_MAP()

/**---------------------------------------------------------------------------*/
void PDlgUserParaDetail::OnBtUserParaSave()
{
	if(GetCtrlValues()) //Daten von GUI eingelesen!
	{
		if(*pUserParaTestChange == *m_pBppUserParaClone) //keine Änderung!
		{
			m_bDataChanged = false;
		}
		else
		{
			if(m_bUserParaNew == false) // UPDATE
			{
				BppUserPara* pUserParaUpdTest = new BppUserPara;
				pUserParaUpdTest->setUserParaid(m_pBppUserParaClone->getUserParaid());
				IPerst::ERet Ret = pUserParaUpdTest->SelForUpd();
				if (Ret != IPerst::RET_OK)
				{
					//kein Update --> Datensatz zum Updaten nicht gefunden!
					MsgBoxOK(IDS_NO_UPDATE_POSS);
				}
				else if (*pUserParaUpdTest != *pUserParaTestChange)
				{
					//kein Update --> Datensatz inzwischen extern verändert!
					MsgBoxOK(IDS_UPDATED_EXTERN);
				}
				else
				{
					IPerst::ERet eRet = m_pBppUserParaClone->updateItem();
					if (eRet == IPerst::RET_OK)
					{
						m_bDataChanged = true;
					}
					else
					{
						MsgBoxOK(IDS_ERR_UPDATE);
					}
				}
				delete pUserParaUpdTest;
			}
			else //INSERT
			{
				IPerst::ERet eRet = m_pBppUserParaClone->insertItem();
				if (eRet == IPerst::RET_OK)
				{
					eRet = m_pBppUserParaClone->selectID();
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
void PDlgUserParaDetail::OnBtUserParaExit()
{
	CDialogMultiLang::OnCancel();
}

/**---------------------------------------------------------------------------*/
BOOL PDlgUserParaDetail::PreTranslateMessage(MSG  * pMsg )
{
	assert(pMsg != 0);

	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			// Fokus steht in UsrListCtrl
			if (GetFocus() == GetDlgItem(IDC_BT_USERPARA_SAVE))
			{
				if (m_bt_UserPara_Save.IsWindowEnabled())
				{
					OnBtUserParaSave();
					return TRUE;
				}
			}
			// Fokus steht in AppListCtrl
			if (GetFocus() == GetDlgItem(IDC_BT_USERPARA_DTL_EXIT))
			{
				if (m_bt_UserPara_Exit.IsWindowEnabled())
				{
					OnBtUserParaExit();
					return TRUE;
				}
			}
			return FALSE;
			
		case VK_ESCAPE:
			if (m_bt_UserPara_Exit.IsWindowEnabled())
			{
				OnBtUserParaExit();
				return TRUE;
			}
			return FALSE;


		default:
			break;
		}
	} 

	return CDialogMultiLang::PreTranslateMessage(pMsg);
}


