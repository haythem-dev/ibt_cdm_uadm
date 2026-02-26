/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH                                         */
/**   Date   : 05.07.2004                                                     */
/**                                                                           */
/******************************************************************************/

#include "StdAfx.h"
#include "presentation.h"
#include <BppLanguage.h>
#include <BConppLanguage.h>
#include <BTest.h>
#include "BppText.h"
#include "PDlgTextDetail.h"
#include "PDlgLangDetail.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/**---------------------------------------------------------------------------*/
// 
/**---------------------------------------------------------------------------*/
IMPLEMENT_DYNAMIC(PDlgLangDetail, CDialogMultiLang)

/**---------------------------------------------------------------------------*/
PDlgLangDetail::PDlgLangDetail(BppLanguage * pBppLang)
				: CDialogMultiLang(PDlgLangDetail::IDD ),
				m_pBppLangClone (pBppLang->clone())
{
	m_pTest = new BTest;
		assert (m_pTest);

	pLangTestChange = pBppLang->clone();
	
	assert(m_pBppLangClone);
	
	m_bLangNew = false;
	m_bDataChanged = false;
}

/**---------------------------------------------------------------------------*/
PDlgLangDetail::~PDlgLangDetail()
{
	if (m_pBppLangClone != NULL)
	{
		delete m_pBppLangClone;
		m_pBppLangClone = NULL;
	} 

	if (pLangTestChange != NULL)
	{
		delete pLangTestChange;
		pLangTestChange = NULL;
	}
	
	if (m_pTest != NULL)
	{
		delete m_pTest;
		m_pTest = NULL;
	}
}

/**---------------------------------------------------------------------------*/
void PDlgLangDetail::DoDataExchange(CDataExchange* pDX)
{
	CDialogMultiLang::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_ED_LANG_ID, m_ed_Langid);
	DDX_Control(pDX, IDC_ED_LANG_NAME, m_ed_Langname);
	DDX_Control(pDX, IDC_BT_LANGDTL_SAVE, m_bt_Lang_Save);
	DDX_Control(pDX, IDC_BT_LANGDTL_EXIT, m_bt_Lang_Exit);
}

/**---------------------------------------------------------------------------*/
BOOL PDlgLangDetail::OnInitDialog()
{
	BeginWaitCursor();

	CDialogMultiLang::OnInitDialog();
	
	if(m_bLangNew == false)
	{
		m_ed_Langid.SetWindowText(m_pBppLangClone->getLangid().c_str());
		m_ed_Langname.SetWindowText(m_pBppLangClone->getLangname().c_str());
		m_ed_Langid.EnableWindow(false);
		m_ed_Langname.SetFocus();
		
		m_strLangidPrev = m_pBppLangClone->getLangid();
	}
	else
	{
		m_ed_Langid.SetFocus();	
	}

	EndWaitCursor();

	return FALSE;  // return TRUE unless you set the focus to a control
}

/**---------------------------------------------------------------------------*/
void PDlgLangDetail::setBNewLang(bool bNewLang)
{
	m_bLangNew = bNewLang;
}

/**---------------------------------------------------------------------------*/
const bool PDlgLangDetail::getBNewLang()
{
	return m_bLangNew;
}

/**---------------------------------------------------------------------------*/
const bool	PDlgLangDetail::getBDataChanged()
{
	return m_bDataChanged;
}


/**---------------------------------------------------------------------------*/
bool PDlgLangDetail::GetCtrlValues()
{
	//Daten von GUI
	CString csLangID;
	m_ed_Langid.GetWindowText(csLangID);

	CString csLangName;
	m_ed_Langname.GetWindowText(csLangName);

	//Längenprüfung 
	CBString strLangID = (LPCTSTR)csLangID;
	strLangID.erasespace(CBString::END);
	strLangID.lower();
	m_strLangname = (LPCTSTR)csLangName;
	m_strLangname.erasespace(CBString::END);

	int iLengthID = (int)strLangID.length();
	int iLengthN  = (int)m_strLangname.length();
	
	bool bLangidExists;

	if ( iLengthN > 30 || iLengthN <= 0 )
	{
		MsgBoxOK(IDS_ERR_LANGNAME, MB_ICONERROR); //Kennungen nicht länger als 30 Zeichen oder leer
		m_ed_Langname.SetFocus();
		return false;
	}
	else if ( iLengthID != 2 )
	{
		MsgBoxOK(IDS_ERR_LANGID, MB_ICONERROR); //ID genau 2 Zeichen
		m_ed_Langid.SetFocus();
		return false;
	}
	else if (!(LangIDExists(strLangID, bLangidExists)))
	{
		MsgBoxOK(IDS_ERR_SQL_COUNT_LANGID);//SQL-Abfrage auf Langid FAULT!
		return false;
	}
	else if (bLangidExists == true)
	{
		MsgBoxOK(IDS_ERR_LANGID_EXISTS, MB_ICONERROR);//Langid schon vorhanden!
		m_ed_Langid.SetFocus();
		return false;
	}	
	else
	{
		//BsnObj füllen
		m_pBppLangClone->setLangid(strLangID);
		m_pBppLangClone->setLangname(m_strLangname);
	}
	return true;
}

/**---------------------------------------------------------------------------*/
bool PDlgLangDetail::LangIDExists(CBString &strLangid, bool & bExists)
{
	if( m_bLangNew == false )
	{
		if ( m_strLangidPrev == strLangid )
		{
			bExists = false;
			return true;
		}
	}
	
	m_pTest->setLangid(strLangid);
	
	if(!(m_pTest->SelLangidExists(bExists)))
	{
		return false;
	}
	return true;
}

/**---------------------------------------------------------------------------*/
void PDlgLangDetail::getCloneObj(BppLanguage * pLangChanged)
{
	*pLangChanged = *m_pBppLangClone;
}


/**---------------------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(PDlgLangDetail, CDialogMultiLang)
	ON_BN_CLICKED(IDC_BT_LANGDTL_SAVE, OnBtLangdtlSave)
	ON_BN_CLICKED(IDC_BT_LANGDTL_EXIT, OnBtLangdtlExit)
END_MESSAGE_MAP()


/**---------------------------------------------------------------------------*/
void PDlgLangDetail::OnBtLangdtlSave()
{
	if(GetCtrlValues()) //Daten von GUI eingelesen!
	{
		if(*pLangTestChange == *m_pBppLangClone) //keine Änderung!
		{
			m_bDataChanged = false;
		}
		else
		{
			if(m_bLangNew == false) // UPDATE
			{
				CBString strRight = RIGHT_WORK_DLL;//dllintern
				if (!(ppBLoginData::ex().isLegitimated(MODULE_DLL,strRight)))
				{
					MsgBoxOK(IDS_NO_RIGHT);
					return;
				}
				
				BppLanguage * pLangUpdTest = new BppLanguage;
				pLangUpdTest->setLangid(m_pBppLangClone->getLangid());
				IPerst::ERet Ret = pLangUpdTest->SelForUpd();
				if (Ret != IPerst::RET_OK)
				{
					//kein Update --> Datensatz zum Updaten nicht gefunden!
					MsgBoxOK(IDS_NO_UPDATE_POSS);
				}
				else if (*pLangUpdTest != *pLangTestChange)
				{
					//kein Update --> Datensatz inzwischen extern verändert!
					MsgBoxOK(IDS_UPDATED_EXTERN);
				}
				else
				{
					IPerst::ERet eRet = m_pBppLangClone->update();
					if (eRet == IPerst::RET_OK)
					{
						m_bDataChanged = true;
					}
					else
					{
						MsgBoxOK(IDS_ERR_UPDATE);
					}
					delete pLangUpdTest;
				}
			}
			else //INSERT
			{
				//NEU
				bool bRefExists;
				MsgBoxOK(IDS_SET_REFTEXT_LANG);
				BppText *pText = new BppText;
				pText->setAppid(1);
				pText->setModid(1);
				pText->setLangid("en");
				m_strLangname.upper();
				pText->setReftext(m_strLangname);
				PDlgTextDetail dlgTextDetail(pText);
				dlgTextDetail.ReftextExists(m_strLangname,bRefExists);
				if (bRefExists == true)
				{
					m_ed_Langname.SetFocus();
					MsgBoxOK(IDS_CHANGE_LANGNAME);
					delete pText;
					return;
				}
				dlgTextDetail.setBNewRefText(false);
				dlgTextDetail.setBNewText(true);
				dlgTextDetail.setBNewLang(true);
				INT_PTR iRet = dlgTextDetail.DoModal();
				if (iRet == IDOK)
				{
					IPerst::ERet eRet = m_pBppLangClone->insert();
					if (eRet == IPerst::RET_OK)
					{
						m_bDataChanged = true;
					}
					else
					{
						MsgBoxOK(IDS_ERR_INSERT);
					}
				}
				else
				{
					MsgBoxOK(IDS_LANG_NOT_NEW);
				}
				delete pText;
			}
		}
		CDialogMultiLang::OnOK();
	}
}

/**---------------------------------------------------------------------------*/
void PDlgLangDetail::OnBtLangdtlExit()
{
	CDialogMultiLang::OnCancel();
}

/**---------------------------------------------------------------------------*/
BOOL PDlgLangDetail::PreTranslateMessage(MSG  * pMsg )
{
	assert(pMsg != 0);

	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			// Fokus steht in UsrListCtrl
			if (GetFocus() == GetDlgItem(IDC_BT_LANGDTL_SAVE))
			{
				if (m_bt_Lang_Save.IsWindowEnabled())
				{
					OnBtLangdtlSave();
					return TRUE;
				}
			}
			// Fokus steht in AppListCtrl
			if (GetFocus() == GetDlgItem(IDC_BT_LANGDTL_EXIT))
			{
				if (m_bt_Lang_Exit.IsWindowEnabled())
				{
					OnBtLangdtlExit();
					return TRUE;
				}
			}
			return FALSE;
			
		case VK_ESCAPE:
			if (m_bt_Lang_Exit.IsWindowEnabled())
			{
				OnBtLangdtlExit();
				return TRUE;
			}
			return FALSE;


		default:
			break;
		}
	} 

	return CDialogMultiLang::PreTranslateMessage(pMsg);
}

