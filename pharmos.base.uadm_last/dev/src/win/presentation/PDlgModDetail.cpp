/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH                                         */
/**   Date   : 29.06.2004                                                     */
/**                                                                           */
/******************************************************************************/

#include "StdAfx.h"
#include "presentation.h"
#include <BppAppMod.h>
#include <BConppAppMod.h>
#include <BTest.h>
#include "PDlgModDetail.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/**---------------------------------------------------------------------------*/
// PDlgUserDetail-Dialogfeld
/**---------------------------------------------------------------------------*/
IMPLEMENT_DYNAMIC(PDlgModDetail, CDialogMultiLang)

/**---------------------------------------------------------------------------*/
PDlgModDetail::PDlgModDetail(BppAppMod * pBppAppMod)
				: CDialogMultiLang(PDlgModDetail::IDD ),
				m_pBppModClone (pBppAppMod->clone())
{
	m_pTest = new BTest;
		assert (m_pTest);

	pModTestChange = pBppAppMod->clone();
	
	assert(m_pBppModClone);
	
	m_bModNew = false;
	m_bDataChanged = false;

}

/**---------------------------------------------------------------------------*/
PDlgModDetail::~PDlgModDetail()
{
	if (m_pBppModClone != NULL)
	{
		delete m_pBppModClone;
		m_pBppModClone = NULL;
	} 

	if (pModTestChange != NULL)
	{
		delete pModTestChange;
		pModTestChange = NULL;
	}
	
	if (m_pTest != NULL)
	{
		delete m_pTest;
		m_pTest = NULL;
	}
}

/**---------------------------------------------------------------------------*/
void PDlgModDetail::DoDataExchange(CDataExchange* pDX)
{
	CDialogMultiLang::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BT_MOD_SAVE, m_bt_Mod_Save);
	DDX_Control(pDX, IDC_BT_MOD_EXIT, m_bt_Mod_Exit);
	DDX_Control(pDX, IDC_ED_MOD_APPID, m_ed_Mod_Appid);
	DDX_Control(pDX, IDC_ED_MODID, m_ed_Modid);
	DDX_Control(pDX, IDC_ED_MODNAME, m_ed_Modname);
	DDX_Control(pDX, IDC_CB_LANG_MODNAME, m_cb_Language);
}

/**---------------------------------------------------------------------------*/
BOOL PDlgModDetail::OnInitDialog()
{
	BeginWaitCursor();

	CDialogMultiLang::OnInitDialog();

	if(m_bModNew == false)
	{
		m_ed_Modid.SetWindowText(m_pBppModClone->getModuleidStr().c_str());
		m_ed_Modname.SetWindowText(m_pBppModClone->getModname().c_str());
		m_ed_Modid.EnableWindow(false);
		m_ed_Modname.SetFocus();
		m_LangInitialized = true;
	}
	else
	{
		m_ed_Modid.SetFocus();
		m_LangInitialized = false;
	}

	m_ed_Mod_Appid.SetWindowText(m_strAppname.c_str());

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
void PDlgModDetail::setAppname(CBString strAppname)
{
	m_strAppname = strAppname;
}

/**---------------------------------------------------------------------------*/
void PDlgModDetail::setBNewMod(bool bNewMod)
{
	m_bModNew = bNewMod;
}

/**---------------------------------------------------------------------------*/
const bool PDlgModDetail::getBNewMod()
{
	return m_bModNew;
}

/**---------------------------------------------------------------------------*/
const bool	PDlgModDetail::getBDataChanged()
{
	return m_bDataChanged;
}

/**---------------------------------------------------------------------------*/
void PDlgModDetail::setAppid (short sAppid)
{
	m_sAppid = sAppid;
}

/**---------------------------------------------------------------------------*/
bool PDlgModDetail::GetCtrlValues()
{
	//Daten von GUI
	CString csModid;
	m_ed_Modid.GetWindowText(csModid);

	CBString str = (LPCTSTR)csModid;
	str.erasespace(CBString::END);
	short sModid = atoi(str);

	bool bModidExists;

	if ( sModid <= 0)
	{
		MsgBoxOK(IDS_ERR_MODID, MB_ICONERROR); //Modid nicht leer
		m_ed_Modid.SetFocus();
		return false;
	}
	else if (!(ModIDExists(sModid, bModidExists)))
	{
		MsgBoxOK(IDS_ERR_SQL_COUNT_MODID);//SQL-Abfrage auf Modid FAULT!
		return false;
	}
	else if (bModidExists == true)
	{
		MsgBoxOK(IDS_ERR_MODID_EXISTS, MB_ICONERROR);//Modid schon vorhanden!
		m_ed_Modid.SetFocus();
		return false;
	}	
	else
	{
		//BsnObj füllen
		m_pBppModClone->setModuleid(sModid);
		
		if ( saveLanguageString() == false ) 
			return false;
	}
	return true;
}

/**---------------------------------------------------------------------------*/
bool PDlgModDetail::ModIDExists(short &sModid, bool & bExists)
{
	
	if (m_bModNew == true)
	{
		m_pTest->setAppid(m_sAppid);
		m_pTest->setModid(sModid);
	
		if(!(m_pTest->SelModIDExists(bExists)))
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
void PDlgModDetail::getCloneObj(BppAppMod * pModChanged)
{
	*pModChanged = *m_pBppModClone;
}

/**---------------------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(PDlgModDetail, CDialogMultiLang)
	ON_BN_CLICKED(IDC_BT_MOD_SAVE, OnBtModSave)
	ON_BN_CLICKED(IDC_BT_MOD_EXIT, OnBtModExit)
	ON_CBN_SELCHANGE(IDC_CB_LANG_MODNAME, OnChangeLang)
END_MESSAGE_MAP()

/*----------------------------------------------------------------------------*/
void PDlgModDetail::OnChangeLang() 
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
void PDlgModDetail::setLanguageString() 
{
	CBString strModname;

	switch ( m_iLangSel )
	{
	case GERMAN:
		strModname = m_pBppModClone->getModname();
		break;
	case ENGLISH:
		strModname = m_pBppModClone->getModname_EN();
		break;
	case FRENCH:
		strModname = m_pBppModClone->getModname_FR();
		break;
	case CROATIAN:
		strModname = m_pBppModClone->getModname_HR();
		break;
	default:
		strModname = "";
		break;	
	}
	
	m_ed_Modname.SetWindowText(strModname.c_str());
}

/*----------------------------------------------------------------------------*/
bool PDlgModDetail::saveLanguageString() 
{
	CString csModname;
	m_ed_Modname.GetWindowText(csModname);

	CBString strModName = (LPCTSTR)csModname;
	strModName.erasespace(CBString::END);
	
	//BsnObj füllen
	switch ( m_iLangSel )
	{
	case GERMAN:
		m_pBppModClone->setName(strModName);
		break;
	case ENGLISH:
		m_pBppModClone->setName_EN(strModName);
		break;
	case FRENCH:
		m_pBppModClone->setName_FR(strModName);
		break;
	case CROATIAN:
		m_pBppModClone->setName_HR(strModName);
		break;
	default:
		break;	
	}

	if ( m_LangInitialized == false && strModName.empty() == false )
	{
		m_pBppModClone->setName(strModName);
		m_pBppModClone->setName_EN(strModName);
		m_pBppModClone->setName_FR(strModName);
		m_pBppModClone->setName_HR(strModName);

		m_LangInitialized = true;
	}

	return true;
}

/**---------------------------------------------------------------------------*/
void PDlgModDetail::OnBtModSave()
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
		if(*pModTestChange == *m_pBppModClone) //keine Änderung!
		{
			m_bDataChanged = false;
		}
		else
		{
			if(m_bModNew == false) // UPDATE
			{
				BppAppMod * pModUpdTest = new BppAppMod;
				pModUpdTest->setAppid(m_pBppModClone->getAppid());
				pModUpdTest->setModuleid(m_pBppModClone->getModuleid());
				IPerst::ERet Ret = pModUpdTest->SelForUpd();
				if (Ret != IPerst::RET_OK)
				{
					//kein Update --> Datensatz zum Updaten nicht gefunden!
					MsgBoxOK(IDS_NO_UPDATE_POSS);
				}
				else if (*pModUpdTest != *pModTestChange)
				{
					//kein Update --> Datensatz inzwischen extern verändert!
					MsgBoxOK(IDS_UPDATED_EXTERN);
				}
				else
				{
					IPerst::ERet eRet = m_pBppModClone->updateMod();
					if (eRet == IPerst::RET_OK)
					{
						m_bDataChanged = true;
					}
					else
					{
						MsgBoxOK(IDS_ERR_UPDATE);
					}
				}
				delete pModUpdTest;
			}
			else //INSERT
			{
				//NASE alle sprachfelder mit deutsch setzen
				IPerst::ERet eRet = m_pBppModClone->insertMod();
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
void PDlgModDetail::OnBtModExit()
{
	CDialogMultiLang::OnCancel();
}

/**---------------------------------------------------------------------------*/
BOOL PDlgModDetail::PreTranslateMessage(MSG  * pMsg )
{
	assert(pMsg != 0);

	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			if (GetFocus() == GetDlgItem(IDC_BT_MOD_SAVE))
			{
				if (m_bt_Mod_Save.IsWindowEnabled())
				{
					OnBtModSave();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_BT_MOD_EXIT))
			{
				if (m_bt_Mod_Exit.IsWindowEnabled())
				{
					OnBtModExit();
					return TRUE;
				}
			}
			return FALSE;
			
		case VK_ESCAPE:
			if (m_bt_Mod_Exit.IsWindowEnabled())
			{
				OnBtModExit();
				return TRUE;
			}
			return FALSE;


		default:
			break;
		}
	} 

	return CDialogMultiLang::PreTranslateMessage(pMsg);
}
