/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH                                         */
/**   Date   : 07.07.2004                                                     */
/**                                                                           */
/******************************************************************************/

#include "StdAfx.h"
#include "presentation.h"
#include <BppText.h>
#include <BConppText.h>
#include <BTest.h>
#include "PDlgTextDetail.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/**---------------------------------------------------------------------------*/
// 
/**---------------------------------------------------------------------------*/
IMPLEMENT_DYNAMIC(PDlgTextDetail, CDialogMultiLang)

/**---------------------------------------------------------------------------*/
PDlgTextDetail::PDlgTextDetail(BppText * pBppText)
				: CDialogMultiLang(PDlgTextDetail::IDD ),
				m_pBppTextClone (pBppText->clone())
{
	m_pTest = new BTest;
		assert (m_pTest);

	pTextTestChange = pBppText->clone();
	
	assert(m_pBppTextClone);
	
	m_bTextNew = false;
	m_bDataChanged = false;
	m_bRefTextNew = false;
	m_bNewLang = false;
}

/**---------------------------------------------------------------------------*/
PDlgTextDetail::~PDlgTextDetail()
{
	if (m_pBppTextClone != NULL)
	{
		delete m_pBppTextClone;
		m_pBppTextClone = NULL;
	} 

	if (pTextTestChange != NULL)
	{
		delete pTextTestChange;
		pTextTestChange = NULL;
	}
	
	if (m_pTest != NULL)
	{
		delete m_pTest;
		m_pTest = NULL;
	}
}

/**---------------------------------------------------------------------------*/
void PDlgTextDetail::DoDataExchange(CDataExchange* pDX)
{
	CDialogMultiLang::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_ED_TXT_DTL_LANG, m_ed_Lang);
	DDX_Control(pDX, IDC_ED_TXT_DTL_REF, m_ed_Reftext);
	DDX_Control(pDX, IDC_ED_TXT_DTL_TEXT_EN, m_ed_Text_EN);
	DDX_Control(pDX, IDC_ED_TXT_DTL_TEXT, m_ed_Text);
	DDX_Control(pDX, IDC_BT_TXT_DTL_SAVE, m_bt_Txt_Save);
	DDX_Control(pDX, IDC_BT_TXT_DTL_EXIT, m_bt_Txt_Exit);
	DDX_Control(pDX, IDC_TXT_MESSAGE, m_txt_Message);
}

/**---------------------------------------------------------------------------*/
BOOL PDlgTextDetail::OnInitDialog()
{
	BeginWaitCursor();

	CDialogMultiLang::OnInitDialog();
	m_ed_Lang.SetWindowText(m_pBppTextClone->getLangid().c_str());
	
	if(m_bRefTextNew == false)
	{
		if (m_bNewLang == false)
		{
			CBString strTextEN;
			m_pBppTextClone->selTextEN(strTextEN);
			m_ed_Text_EN.SetWindowText(strTextEN.c_str());
			m_txt_Message.SetWindowText("");
		}
		else
		{
			m_txt_Message.SetWindowText(CResString::ex().getStrTblText(IDS_TEXT_EN));
		}
		m_ed_Reftext.SetWindowText(m_pBppTextClone->getReftext().c_str());
		m_ed_Reftext.EnableWindow(false);
		m_strReftextPrev = m_pBppTextClone->getReftext();
		m_ed_Text.SetFocus();						
	
		if(m_bTextNew == false)
		{
			m_ed_Text.SetWindowText(m_pBppTextClone->getText().c_str());
		}
	}
	else
	{
		m_ed_Reftext.SetFocus();
		m_txt_Message.SetWindowText(CResString::ex().getStrTblText(IDS_TEXT_EN));
	}

	EndWaitCursor();

	return FALSE;  // return TRUE unless you set the focus to a control
}

/**---------------------------------------------------------------------------*/
void PDlgTextDetail::setBNewLang(bool bNewLang)
{
	m_bNewLang = bNewLang;
}

/**---------------------------------------------------------------------------*/
const bool PDlgTextDetail::getBNewLang()
{
	return m_bNewLang;
}

/**---------------------------------------------------------------------------*/
void PDlgTextDetail::setBNewText(bool bNewText)
{
	m_bTextNew = bNewText;
}

/**---------------------------------------------------------------------------*/
const bool PDlgTextDetail::getBNewText()
{
	return m_bTextNew;
}
/**---------------------------------------------------------------------------*/
void PDlgTextDetail::setBNewRefText(bool bNewRefText)
{
	m_bRefTextNew = bNewRefText;
}

/**---------------------------------------------------------------------------*/
const bool PDlgTextDetail::getBNewRefText()
{
	return m_bRefTextNew;
}

/**---------------------------------------------------------------------------*/
const bool	PDlgTextDetail::getBDataChanged()
{
	return m_bDataChanged;
}


/**---------------------------------------------------------------------------*/
bool PDlgTextDetail::GetCtrlValues()
{
	//Daten von GUI
	CString csReftext;
	m_ed_Reftext.GetWindowText(csReftext);

	CString csText;
	m_ed_Text.GetWindowText(csText);

	//Längenprüfung 
	CBString strReftext = (LPCTSTR)csReftext;
	strReftext.erasespace(CBString::END);
	strReftext.upper();
	CBString strText = (LPCTSTR)csText;
	strText.erasespace(CBString::END);

	int iLengthRF = (int)strReftext.length();
	int iLengthT  = (int)strText.length();
	
	bool bRefTextExists;

	if ( iLengthRF > 254 || iLengthRF <= 0 )
	{
		MsgBoxOK(IDS_ERR_REFTEXT, MB_ICONERROR); //Kennungen nicht länger als 254 Zeichen oder leer
		m_ed_Reftext.SetFocus();
		return false;
	}
	else if ( iLengthT > 240 || iLengthT <= 0 )
	{
		MsgBoxOK(IDS_ERR_TEXT, MB_ICONERROR);
		m_ed_Text.SetFocus();
		return false;
	}
	else if (!(ReftextExists(strReftext, bRefTextExists)))
	{
		MsgBoxOK(IDS_ERR_SQL_COUNT_REFTEXT);//SQL-Abfrage auf Reftext FAULT!
		return false;
	}
	else if (bRefTextExists == true)
	{
		MsgBoxOK(IDS_ERR_REFTEXT_EXISTS, MB_ICONERROR);//Reftext schon vorhanden!
		m_ed_Reftext.SetFocus();
		return false;
	}	
	else
	{
		//BsnObj füllen
		m_pBppTextClone->setReftext(strReftext);
		m_pBppTextClone->setText(strText);
	}
	return true;
}

/**---------------------------------------------------------------------------*/
bool PDlgTextDetail::ReftextExists(CBString &strReftext, bool & bExists)
{
	if( m_bRefTextNew == false )
	{
		if ( m_strReftextPrev == strReftext )
		{
			bExists = false;
			return true;
		}
	}
	
	m_pTest->setReftext(strReftext);
	
	if(!(m_pTest->SelReftextExists(bExists)))
	{
		return false;
	}
	return true;
}

/**---------------------------------------------------------------------------*/
void PDlgTextDetail::getCloneObj(BppText * pTextChanged)
{
	*pTextChanged = *m_pBppTextClone;
}


/**---------------------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(PDlgTextDetail, CDialogMultiLang)
	ON_BN_CLICKED(IDC_BT_TXT_DTL_SAVE, OnBtTxtDtlSave)
	ON_BN_CLICKED(IDC_BT_TXT_DTL_EXIT, OnBtTxtDtlExit)
END_MESSAGE_MAP()

/**---------------------------------------------------------------------------*/
void PDlgTextDetail::OnBtTxtDtlSave()
{
	CBString strRight = RIGHT_WORK_DLL;//dllintern
	if (!(ppBLoginData::ex().isLegitimated(MODULE_DLL,strRight)))
	{
		MsgBoxOK(IDS_NO_RIGHT);
		return;
	}
	
	if(GetCtrlValues()) //Daten von GUI eingelesen!
	{
		/*if(*pTextTestChange == *m_pBppTextClone) //keine Änderung!
		{
			m_bDataChanged = false;
		}
		else
		{*/
			if(m_bRefTextNew == false) 
			{
				if (m_bTextNew == false)// UPDATE Text
				{
					if(*pTextTestChange == *m_pBppTextClone) //keine Änderung!
					{
						m_bDataChanged = false;
					}
					else
					{
						BppText * pTextUpdTest = new BppText;
						pTextUpdTest->setAppid(m_pBppTextClone->getAppid());
						pTextUpdTest->setModid(m_pBppTextClone->getModid());
						pTextUpdTest->setLangid(m_pBppTextClone->getLangid());
						pTextUpdTest->setReftext(m_pBppTextClone->getReftext());
						IPerst::ERet Ret = pTextUpdTest->SelForUpd();
						if (Ret != IPerst::RET_OK)
						{
							//kein Update --> Datensatz zum Updaten nicht gefunden!
							MsgBoxOK(IDS_NO_UPDATE_POSS);
						}
						else if (*pTextUpdTest != *pTextTestChange)
						{
							//kein Update --> Datensatz inzwischen extern verändert!
							MsgBoxOK(IDS_UPDATED_EXTERN);
						}
						else
						{
							IPerst::ERet eRet = m_pBppTextClone->update();
							if (eRet == IPerst::RET_OK)
							{
								m_bDataChanged = true;
							}
							else
							{
								MsgBoxOK(IDS_ERR_UPDATE);
							}
						}
						delete pTextUpdTest;
					}
				}
				else // INSERT Text
				{
					IPerst::ERet eRet = m_pBppTextClone->insert();
					if (eRet == IPerst::RET_OK)
					{
						m_bDataChanged = true;
					}
					else
					{
						MsgBoxOK(IDS_ERR_INSERT);
					}
				}
			}
			else //INSERT RefText
			{
				IPerst::ERet eRet = m_pBppTextClone->insert();
				if (eRet == IPerst::RET_OK)
				{
					m_bDataChanged = true;
				}
				else
				{
					MsgBoxOK(IDS_ERR_INSERT);
				}
			}
		
		CDialogMultiLang::OnOK();
	}
}

/**---------------------------------------------------------------------------*/
void PDlgTextDetail::OnBtTxtDtlExit()
{
	CDialogMultiLang::OnCancel();
}

/**---------------------------------------------------------------------------*/
BOOL PDlgTextDetail::PreTranslateMessage(MSG  * pMsg )
{
	assert(pMsg != 0);

	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			// Fokus steht in UsrListCtrl
			if (GetFocus() == GetDlgItem(IDC_BT_TXT_DTL_SAVE))
			{
				if (m_bt_Txt_Save.IsWindowEnabled())
				{
					OnBtTxtDtlSave();
					return TRUE;
				}
			}
			
			if (GetFocus() == GetDlgItem(IDC_BT_TXT_DTL_EXIT))
			{
				if (m_bt_Txt_Exit.IsWindowEnabled())
				{
					OnBtTxtDtlExit();
					return TRUE;
				}
			}
			return FALSE;
			
		case VK_ESCAPE:
			if (m_bt_Txt_Exit.IsWindowEnabled())
			{
				OnBtTxtDtlExit();
				return TRUE;
			}
			return FALSE;


		default:
			break;
		}
	} 

	return CDialogMultiLang::PreTranslateMessage(pMsg);
}

