/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH                                         */
/**   Date   : 29.06.2004                                                     */
/**                                                                           */
/******************************************************************************/

#include "StdAfx.h"
#include "presentation.h"
#include <BppAppRight.h>
#include <BConppAppRight.h>
#include <BTest.h>
#include "PDlgRightDetail.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/**---------------------------------------------------------------------------*/
// PDlgUserDetail-Dialogfeld
/**---------------------------------------------------------------------------*/
IMPLEMENT_DYNAMIC(PDlgRightDetail, CDialogMultiLang)

/**---------------------------------------------------------------------------*/
PDlgRightDetail::PDlgRightDetail(BppAppRight * pBppAppRight)
				: CDialogMultiLang(PDlgRightDetail::IDD ),
				m_pBppRiClone (pBppAppRight->clone())
{
	m_pTest = new BTest;
		assert (m_pTest);

	pRiTestChange = pBppAppRight->clone();
	
	assert(m_pBppRiClone);
	
	m_bRiNew = false;
	m_bDataChanged = false;

}

/**---------------------------------------------------------------------------*/
PDlgRightDetail::~PDlgRightDetail()
{
	if (m_pBppRiClone != NULL)
	{
		delete m_pBppRiClone;
		m_pBppRiClone = NULL;
	} 

	if (pRiTestChange != NULL)
	{
		delete pRiTestChange;
		pRiTestChange = NULL;
	}
	
	if (m_pTest != NULL)
	{
		delete m_pTest;
		m_pTest = NULL;
	}
}

/**---------------------------------------------------------------------------*/
void PDlgRightDetail::DoDataExchange(CDataExchange* pDX)
{
	CDialogMultiLang::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_ED_RI_APPNAME, m_ed_Ri_Appname);
	DDX_Control(pDX, IDC_ED_RI_MODNAME, m_ed_Ri_Modname);
	DDX_Control(pDX, IDC_ED_RINAME, m_ed_RiName);
	DDX_Control(pDX, IDC_ED_RIDESC, m_ed_RiDesc);
	DDX_Control(pDX, IDC_BT_RI_SAVE, m_bt_Ri_Save);
	DDX_Control(pDX, IDC_BT_RI_EXIT, m_bt_Ri_Exit);
	DDX_Control(pDX, IDC_CB_LANG_RIGHTDESC, m_cb_Language);
	DDX_Control(pDX, IDC_CB_RIGHT_CATEGORY, m_cb_Category);
	DDX_Control(pDX, IDC_ST_RIGHT_CATEGORY, m_txt_Category);
}

/**---------------------------------------------------------------------------*/
BOOL PDlgRightDetail::OnInitDialog()
{
	BeginWaitCursor();

	CDialogMultiLang::OnInitDialog();

	if(m_bRiNew == false)
	{
		m_ed_RiName.SetWindowText(m_pBppRiClone->getRightname().c_str());
		m_ed_RiDesc.SetWindowText(m_pBppRiClone->getRightDesc().c_str());
		m_ed_RiName.EnableWindow(false);
		m_ed_RiDesc.SetFocus();
		m_LangInitialized = true;
	}
	else
	{
		m_ed_RiName.SetFocus();
		m_LangInitialized = false;
	}

	m_ed_Ri_Appname.SetWindowText(m_strAppname.c_str());
	m_ed_Ri_Modname.SetWindowText(m_strModname.c_str());

	if ( m_strAppname.area(3) == BI_APPL )
	{
		m_cb_Category.AddString(CResString::ex().getStrTblText(IDS_MAIN_REPORT));
		m_cb_Category.AddString(CResString::ex().getStrTblText(IDS_VARIABLE_REPORT));
		m_cb_Category.SetCurSel(m_pBppRiClone->getRightCategory() - 1);
	}
	else
	{
		m_cb_Category.ShowWindow(SW_HIDE);
		m_txt_Category.ShowWindow(SW_HIDE);
	}

	m_cb_Language.AddString(CResString::ex().getStrTblText(IDS_GERMAN));
	m_cb_Language.AddString(CResString::ex().getStrTblText(IDS_ENGLISH));
	m_cb_Language.AddString(CResString::ex().getStrTblText(IDS_FRENCH));
	m_cb_Language.AddString(CResString::ex().getStrTblText(IDS_CROATIAN));
	m_cb_Language.SetCurSel(0);

	m_iLangSel = 0;

	EndWaitCursor();

	return FALSE;  // return TRUE unless you set the focus to a control
}

/**---------------------------------------------------------------------------*/
void PDlgRightDetail::setAppname(CBString strAppname)
{
	m_strAppname = strAppname;
}

/**---------------------------------------------------------------------------*/
void PDlgRightDetail::setModname(CBString strModname)
{
	m_strModname = strModname;
}
/**---------------------------------------------------------------------------*/
void PDlgRightDetail::setAppid (short sAppid)
{
	m_sAppid = sAppid;
}

/**---------------------------------------------------------------------------*/
void PDlgRightDetail::setModid (short sModid)
{
	m_sModid = sModid;
}

/**---------------------------------------------------------------------------*/
void PDlgRightDetail::setBNewRi(bool bNewRi)
{
	m_bRiNew = bNewRi;
}

/**---------------------------------------------------------------------------*/
const bool PDlgRightDetail::getBNewRi()
{
	return m_bRiNew;
}

/**---------------------------------------------------------------------------*/
const bool	PDlgRightDetail::getBDataChanged()
{
	return m_bDataChanged;
}


/**---------------------------------------------------------------------------*/
bool PDlgRightDetail::GetCtrlValues()
{
	//Daten von GUI
	CString csRiName;
	m_ed_RiName.GetWindowText(csRiName);

	CBString strRiName = (LPCTSTR)csRiName;
	strRiName.erasespace(CBString::END);
	strRiName.upper();
	int iLengthN = (int)strRiName.length();
	
	bool bRiExists;

	if ( iLengthN > 20 || iLengthN <= 0)
	{
		MsgBoxOK(IDS_ERR_RINAME, MB_ICONERROR); //Rechte nicht länger als 20 Zeichen oder leer
		m_ed_RiName.SetFocus();
		return false;
	}
	else if (!(RightExists(strRiName, bRiExists)))
	{
		MsgBoxOK(IDS_ERR_SQL_COUNT_RINAME);//SQL-Abfrage auf Rightname FAULT!
		return false;
	}
	else if (bRiExists == true)
	{
		MsgBoxOK(IDS_ERR_RINAME_EXISTS, MB_ICONERROR);//Rightname schon vorhanden!
		m_ed_RiName.SetFocus();
		return false;
	}	
	else
	{
		//BsnObj füllen
		m_pBppRiClone->setRightname(strRiName);

		if ( m_cb_Category.IsWindowEnabled() )
			m_pBppRiClone->setRightCategory(m_cb_Category.GetCurSel() + 1);
		else
			m_pBppRiClone->setRightCategory(RIGHT_CATEGORY_NONE);
		
		if ( saveLanguageString() == false ) 
			return false;
	}
	return true;
}

/**---------------------------------------------------------------------------*/
bool PDlgRightDetail::RightExists(CBString & strRiName, bool & bExists)
{
	if(m_bRiNew == true)
	{
		m_pTest->setAppid(m_sAppid);
		m_pTest->setModid(m_sModid);
		m_pTest->setRiname(strRiName);
	
		if(!(m_pTest->SelRiNameExists(bExists)))
		{
			return false;
		}
		return true;
	}
	else
	{
		bExists = false;
		return true;
	}
}

/**---------------------------------------------------------------------------*/
void PDlgRightDetail::getCloneObj(BppAppRight * pRiChanged)
{
	*pRiChanged = *m_pBppRiClone;
}

/**---------------------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(PDlgRightDetail, CDialogMultiLang)
	ON_BN_CLICKED(IDC_BT_RI_SAVE, OnBtRiSave)
	ON_BN_CLICKED(IDC_BT_RI_EXIT, OnBtRiExit)
	ON_CBN_SELCHANGE(IDC_CB_LANG_RIGHTDESC, OnChangeLang)
END_MESSAGE_MAP()

/*----------------------------------------------------------------------------*/
void PDlgRightDetail::OnChangeLang() 
{
	int iLangSel;
	iLangSel = m_cb_Language.GetCurSel();

	if ( iLangSel != m_iLangSel )
	{
		// aktuelle Sprache in BsnObj
		if ( saveLanguageString() == true )
		{
			m_iLangSel = iLangSel;
			// andere Sprache anblenden 
			setLanguageString();
		}
		else
			m_cb_Language.SetCurSel(m_iLangSel);
	}
}

/*----------------------------------------------------------------------------*/
void PDlgRightDetail::setLanguageString() 
{
	CBString strRightDesc;

	switch ( m_iLangSel )
	{
	case GERMAN:
		strRightDesc = m_pBppRiClone->getRightDesc();
		break;
	case ENGLISH:
		strRightDesc = m_pBppRiClone->getRightDesc_EN();
		break;
	case FRENCH:
		strRightDesc = m_pBppRiClone->getRightDesc_FR();
		break;
	case CROATIAN:
		strRightDesc = m_pBppRiClone->getRightDesc_HR();
		break;
	default:
		strRightDesc = "";
		break;	
	}
	
	m_ed_RiDesc.SetWindowText(strRightDesc.c_str());
}

/*----------------------------------------------------------------------------*/
bool PDlgRightDetail::saveLanguageString() 
{
	CString csRiDesc;
	m_ed_RiDesc.GetWindowText(csRiDesc);
	
	CBString strRiDesc = (LPCTSTR)csRiDesc;
	int iLengthD = (int)strRiDesc.length();
	if ( iLengthD > 250 )
	{
		MsgBoxOK(IDS_ERR_RIDESC, MB_ICONERROR); 
		m_ed_RiDesc.SetFocus();
		return false;
	}
	
	//BsnObj füllen
	switch ( m_iLangSel )
	{
	case GERMAN:
		m_pBppRiClone->setRightDesc(strRiDesc);
		break;
	case ENGLISH:
		m_pBppRiClone->setRightDesc_EN(strRiDesc);
		break;
	case FRENCH:
		m_pBppRiClone->setRightDesc_FR(strRiDesc);
		break;
	case CROATIAN:
		m_pBppRiClone->setRightDesc_HR(strRiDesc);
		break;
	default:
		break;	
	}

	if ( m_LangInitialized == false && strRiDesc.empty() == false )
	{
		m_pBppRiClone->setRightDesc(strRiDesc);
		m_pBppRiClone->setRightDesc_EN(strRiDesc);
		m_pBppRiClone->setRightDesc_FR(strRiDesc);
		m_pBppRiClone->setRightDesc_HR(strRiDesc);

		m_LangInitialized = true;
	}

	return true;
}

/**---------------------------------------------------------------------------*/
void PDlgRightDetail::OnBtRiSave()
{
	CBString strRight = RIGHT_WORK_DLL;
	if (!(ppBLoginData::ex().isLegitimated(MODULE_DLL,strRight)))
	{
		strRight = m_strAppname;
		strRight.upper();
		
		if (!(ppBLoginData::ex().isLegitimated(MODULE_DLL,strRight)))
		{
			MsgBoxOK(IDS_NO_RIGHT);
			return;
		}
	}
	
	if(GetCtrlValues()) //Daten von GUI eingelesen!
	{
		if(*pRiTestChange == *m_pBppRiClone) //keine Änderung!
		{
			m_bDataChanged = false;
		}
		else
		{
			if(m_bRiNew == false) // UPDATE
			{
				BppAppRight * pRiUpdTest = new BppAppRight;
				pRiUpdTest->setAppid(m_pBppRiClone->getAppid());
				pRiUpdTest->setModid(m_pBppRiClone->getModid());
				pRiUpdTest->setRightname(m_pBppRiClone->getRightname());
				IPerst::ERet Ret = pRiUpdTest->SelForUpd();
				if (Ret != IPerst::RET_OK)
				{
					//kein Update --> Datensatz zum Updaten nicht gefunden!
					MsgBoxOK(IDS_NO_UPDATE_POSS);
				}
				//else if (*pRiUpdTest != *pRiTestChange) //NASE only temporary
				else if (pRiUpdTest->getRightDesc() != pRiTestChange->getRightDesc()/*.erasespace(sccmn::CBString::END)*/ ||
						pRiUpdTest->getRightDesc_EN() != pRiTestChange->getRightDesc_EN() ||
						pRiUpdTest->getRightDesc_FR() != pRiTestChange->getRightDesc_FR() ||
						pRiUpdTest->getRightDesc_HR() != pRiTestChange->getRightDesc_HR() ||
						pRiUpdTest->getRightCategory() != pRiTestChange->getRightCategory() )
				{
					//kein Update --> Datensatz inzwischen extern verändert!
					MsgBoxOK(IDS_UPDATED_EXTERN);
				}
				else
				{
					IPerst::ERet eRet = m_pBppRiClone->updateAppModRight();
					if (eRet == IPerst::RET_OK)
					{
						m_bDataChanged = true;
					}
					else
					{
						MsgBoxOK(IDS_ERR_UPDATE);
					}
				}
				delete pRiUpdTest;
			}
			else //INSERT
			{
				//NASE alle sprachfelder mit deutsch setzen
				IPerst::ERet eRet = m_pBppRiClone->insertAppModRight();
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
		CDialogMultiLang::OnOK();
	}
}

/**---------------------------------------------------------------------------*/
void PDlgRightDetail::OnBtRiExit()
{
	CDialogMultiLang::OnCancel();
}

/**---------------------------------------------------------------------------*/
BOOL PDlgRightDetail::PreTranslateMessage(MSG  * pMsg )
{
	assert(pMsg != 0);

	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			if (GetFocus() == GetDlgItem(IDC_BT_RI_SAVE))
			{
				if (m_bt_Ri_Save.IsWindowEnabled())
				{
					OnBtRiSave();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_BT_RI_EXIT))
			{
				if (m_bt_Ri_Exit.IsWindowEnabled())
				{
					OnBtRiExit();
					return TRUE;
				}
			}
			return FALSE;
			
		case VK_ESCAPE:
			if (m_bt_Ri_Exit.IsWindowEnabled())
			{
				OnBtRiExit();
				return TRUE;
			}
			return FALSE;


		default:
			break;
		}
	} 

	return CDialogMultiLang::PreTranslateMessage(pMsg);
}
