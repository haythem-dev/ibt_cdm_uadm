/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH                                         */
/**   Date   : 26.05.2004                                                     */
/**                                                                           */
/******************************************************************************/

#include "StdAfx.h"
#include "presentation.h"
#include <BppApp.h>
#include <BConppApp.h>
#include <BTest.h>
#include <BppAppRight.h>
#include <defines.h>

#include "PDlgAppDetail.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/**---------------------------------------------------------------------------*/
// PDlgUserDetail-Dialogfeld
/**---------------------------------------------------------------------------*/
IMPLEMENT_DYNAMIC(PDlgAppDetail, CDialogMultiLang)

/**---------------------------------------------------------------------------*/
PDlgAppDetail::PDlgAppDetail(BppApp * pBppApp)
				: CDialogMultiLang(PDlgAppDetail::IDD ),
				m_pBppAppClone (pBppApp->clone())
{
	m_pTest = new BTest;
		assert (m_pTest);

	pAppTestChange = pBppApp->clone();
	
	assert(m_pBppAppClone);
	
	m_bAppNew = false;
	m_bDataChanged = false;

}

/**---------------------------------------------------------------------------*/
PDlgAppDetail::~PDlgAppDetail()
{
	if (m_pBppAppClone != NULL)
	{
		delete m_pBppAppClone;
		m_pBppAppClone = NULL;
	} 

	if (pAppTestChange != NULL)
	{
		delete pAppTestChange;
		pAppTestChange = NULL;
	}
	
	if (m_pTest != NULL)
	{
		delete m_pTest;
		m_pTest = NULL;
	}
}

/**---------------------------------------------------------------------------*/
void PDlgAppDetail::DoDataExchange(CDataExchange* pDX)
{
	CDialogMultiLang::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BT_APP_SAVE, m_bt_App_Save);
	DDX_Control(pDX, IDC_BT_APP_EXIT, m_bt_App_Exit);
	DDX_Control(pDX, IDC_ED_APPID, m_ed_Appid);
	DDX_Control(pDX, IDC_ED_APPNAME, m_ed_Appname);
	DDX_Control(pDX, IDC_ED_APPDESC, m_ed_AppDesc);
	DDX_Control(pDX, IDC_CB_LANG_APPNAME, m_cb_Language);
}

/**---------------------------------------------------------------------------*/
BOOL PDlgAppDetail::OnInitDialog()
{
	BeginWaitCursor();

	CDialogMultiLang::OnInitDialog();

	if(m_bAppNew == false)
	{
		m_LangInitialized = true;

		m_ed_Appid.SetWindowText(m_pBppAppClone->getAppidStr().c_str());
		m_ed_Appname.SetWindowText(m_pBppAppClone->getAppname().c_str());
		m_ed_AppDesc.SetWindowText(m_pBppAppClone->getAppDesc().c_str());

		m_strAppnamePrev = m_pBppAppClone->getAppname();
	}
	else
		m_LangInitialized = false;
	
	m_ed_Appname.SetFocus();

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
void PDlgAppDetail::setBNewApp(bool bNewApp)
{
	m_bAppNew = bNewApp;
}

/**---------------------------------------------------------------------------*/
const bool PDlgAppDetail::getBNewApp()
{
	return m_bAppNew;
}

/**---------------------------------------------------------------------------*/
const bool	PDlgAppDetail::getBDataChanged()
{
	return m_bDataChanged;
}

/**---------------------------------------------------------------------------*/
bool PDlgAppDetail::GetCtrlValues()
{
	if ( saveLanguageString() == true )
	{
		//Daten von GUI
		CString csAppDesc;
		m_ed_AppDesc.GetWindowText(csAppDesc);

		m_pBppAppClone->setAppDesc((LPCTSTR)csAppDesc);

		return true;
	}
	else 
		return false;
}

/**---------------------------------------------------------------------------*/
bool PDlgAppDetail::AppNameExists(CBString &strAppName, bool & bExists)
{
	if( m_bAppNew == false )
	{
		if ( m_strAppnamePrev == strAppName )
		{
			bExists = false;
			return true;
		}
	}
	
	m_pTest->setAppname(strAppName);
	
	if(!(m_pTest->SelAppNameExists(bExists)))
	{
		return false;
	}
	return true;
}

/**---------------------------------------------------------------------------*/
void PDlgAppDetail::getCloneObj(BppApp * pAppChanged)
{
	*pAppChanged = *m_pBppAppClone;
}

/**---------------------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(PDlgAppDetail, CDialogMultiLang)
	ON_BN_CLICKED(IDC_BT_APP_SAVE, OnBtAppSave)
	ON_BN_CLICKED(IDC_BT_APP_EXIT, OnBtAppExit)
	ON_CBN_SELCHANGE(IDC_CB_LANG_APPNAME, OnChangeLang)
END_MESSAGE_MAP()

/*----------------------------------------------------------------------------*/
void PDlgAppDetail::OnChangeLang() 
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
void PDlgAppDetail::setLanguageString() 
{
	CBString strAppname;
	switch ( m_iLangSel )
	{
	case GERMAN:
		strAppname = m_pBppAppClone->getAppname();
		break;
	case ENGLISH:
		strAppname = m_pBppAppClone->getAppname_EN();
		break;
	case FRENCH:
		strAppname = m_pBppAppClone->getAppname_FR();
		break;
	case CROATIAN:
		strAppname = m_pBppAppClone->getAppname_HR();
		break;
	default:
		strAppname = "";
		break;	
	}
	
	m_ed_Appname.SetWindowText(strAppname.c_str());
}

/*----------------------------------------------------------------------------*/
bool PDlgAppDetail::saveLanguageString() 
{
	CString csAppName;
	m_ed_Appname.GetWindowText(csAppName);

	CBString strAppName = (LPCTSTR)csAppName;
	strAppName.erasespace(CBString::END);
	strAppName.upper();
	int iLengthN = (int)strAppName.length();

	if ( iLengthN > 30 )
	{
		MsgBoxOK(IDS_ERR_APPNAME, MB_ICONERROR); //Sprach-Kennungen nicht länger als 30 Zeichen
		m_ed_Appname.SetFocus();
		return false;
	}
	
	if ( m_iLangSel == GERMAN )
	{
		bool bAppnameExists;

		if ( iLengthN <= 0)
		{
			MsgBoxOK(IDS_ERR_APPNAME, MB_ICONERROR); //Kennungen nicht länger als 30 Zeichen oder leer
			m_ed_Appname.SetFocus();
			return false;
		}
			
		if ( !(AppNameExists(strAppName, bAppnameExists)) )
		{
			MsgBoxOK(IDS_ERR_SQL_COUNT_APPNAME);//SQL-Abfrage auf Appname FAULT!
			return false;
		}
		
		if (bAppnameExists == true)
		{
			MsgBoxOK(IDS_ERR_APPNAME_EXISTS, MB_ICONERROR);//Appname schon vorhanden!
			m_ed_Appname.SetFocus();
			return false;
		}
	}

	//BsnObj füllen
	switch ( m_iLangSel )
	{
	case GERMAN:
		m_pBppAppClone->setAppname(strAppName);
		break;
	case ENGLISH:
		m_pBppAppClone->setAppname_EN(strAppName);
		break;
	case FRENCH:
		m_pBppAppClone->setAppname_FR(strAppName);
		break;
	case CROATIAN:
		m_pBppAppClone->setAppname_HR(strAppName);
		break;
	default:
		break;	
	}

	if ( m_LangInitialized == false && strAppName.empty() == false )
	{
		m_pBppAppClone->setAppname(strAppName);
		m_pBppAppClone->setAppname_EN(strAppName);
		m_pBppAppClone->setAppname_FR(strAppName);
		m_pBppAppClone->setAppname_HR(strAppName);

		m_LangInitialized = true;
	}

	return true;
}

/**---------------------------------------------------------------------------*/
void PDlgAppDetail::OnBtAppSave()
{
	if(GetCtrlValues()) //Daten von GUI eingelesen!
	{
		if(*pAppTestChange == *m_pBppAppClone) //keine Änderung!
		{
			m_bDataChanged = false;
		}
		else
		{
			if(m_bAppNew == false) // UPDATE
			{
				CBString strRight = RIGHT_WORK_DLL;
				if (!(ppBLoginData::ex().isLegitimated(MODULE_DLL,strRight)))
				{
					//Daten von GUI
					CString csAppName;
					m_ed_Appname.GetWindowText(csAppName);
					strRight = (LPCTSTR)csAppName;
					strRight.erasespace(CBString::END);
					strRight.upper();
					if (!(ppBLoginData::ex().isLegitimated(MODULE_DLL,strRight)))
					{
						MsgBoxOK(IDS_NO_RIGHT);
						return;
					}
				}
				
				BppApp * pAppUpdTest = new BppApp;
				pAppUpdTest->setAppid(m_pBppAppClone->getAppid());
				IPerst::ERet Ret = pAppUpdTest->SelForUpd();
				if (Ret != IPerst::RET_OK)
				{
					//kein Update --> Datensatz zum Updaten nicht gefunden!
					MsgBoxOK(IDS_NO_UPDATE_POSS);
				}
				else if (*pAppUpdTest != *pAppTestChange)
				{
					//kein Update --> Datensatz inzwischen extern verändert!
					MsgBoxOK(IDS_UPDATED_EXTERN);
				}
				else
				{
					IPerst::ERet eRet = m_pBppAppClone->update();
					if (eRet == IPerst::RET_OK)
					{
						//Recht für diese Applikation -> rightname anpassen!
						if ( m_pBppAppClone->getAppname() != pAppUpdTest->getAppname() )
						{
							if (UpdateApplRight() != true)
							{
								MsgBoxOK(IDS_ERR_UPDATE_APPL_RIGHT);
							}	
						}
						m_bDataChanged = true;
					}
					else
					{
						MsgBoxOK(IDS_ERR_UPDATE);
					}
				}
				delete pAppUpdTest;
			}
			else //INSERT
			{
				//NASE alle sprachfelder mit deutsch setzen
				IPerst::ERet eRet = m_pBppAppClone->insert();
				if (eRet == IPerst::RET_OK)
				{
					eRet = m_pBppAppClone->selectID();
					if (eRet != IPerst::RET_OK) //Fehler beim Insert
					{
						MsgBoxOK(IDS_ERR_INSERT);
					}
					else //Recht für diese Applikation an Operator vergeben
					{
						if (InsertNewRight() != true)
						{
							MsgBoxOK(IDS_ERR_INSERT_NEW_RIGHT);
						}
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
bool PDlgAppDetail::InsertNewRight()
{
	//neuer AppName = Recht in UADM (ID = 3) in Modul (ID = 2)
	CString csAppName;
	m_ed_Appname.GetWindowText(csAppName);
	CBString strAppName = (LPCTSTR)csAppName;
	strAppName.erasespace(CBString::END);
	strAppName.upper();
	BppAppRight * pRight = new BppAppRight;
	
	//Recht für UADM - Modul2 definieren
	pRight->setRightname(strAppName);
	CBString strRightDesc = "";
	strRightDesc.format("Einstellungen für %s bearbeiten.",strAppName.c_str());
	pRight->setRightDesc(strRightDesc);
	pRight->setAppid(UADM_ID);
	pRight->setModid(MODULE_DLL);
	IPerst::ERet eRet = pRight->insertAppModRight();
	if (eRet != IPerst::RET_OK)
	{
		delete pRight;
		return false;
	}
	
	//Recht an Operator als Einzelrecht zuweisen
	pRight->setBranchRegid(GLOBAL_REG_ID); //immer für Global!
	pRight->setGroupid(0);
	pRight->setUserid((short)ppBLoginData::ex().GetUserID());

	eRet = pRight->insertAppRight();
	if (eRet != IPerst::RET_OK)
	{
		delete pRight;
		return false;
	}

	// auch für Module 1
	pRight->setModid(MODULE_RIGHTS);
	eRet = pRight->insertAppModRight();
	if (eRet != IPerst::RET_OK)
	{
		delete pRight;
		return false;
	}

	eRet = pRight->insertAppRight();
	if (eRet != IPerst::RET_OK)
	{
		delete pRight;
		return false;
	}

	delete pRight;
	return true;
}

/**---------------------------------------------------------------------------*/
bool PDlgAppDetail::UpdateApplRight()
{
	// AppName = Recht in UADM (ID = 3) in Modul (ID = 2)
	BppAppRight * pRight = new BppAppRight;
	
	//Recht für UADM - Modul2 in ppappmodright updaten
	pRight->setRightname(m_pBppAppClone->getAppname());
	CBString strRightDesc = "";
	strRightDesc.format("Einstellungen für %s bearbeiten.",m_pBppAppClone->getAppname().c_str());
	pRight->setRightDesc(strRightDesc);
	
	pRight->setAppid(UADM_ID);
	pRight->setModid(MODULE_DLL);
	pRight->setPrevRightName(m_strAppnamePrev);
	
	IPerst::ERet eRet = pRight->updateApplModRight();
	if (eRet != IPerst::RET_OK)
	{
		delete pRight;
		return false;
	}

	//Rechtname in ppappright updaten
	eRet = pRight->updateApplRight();
	if (eRet != IPerst::RET_OK)
	{
		delete pRight;
		return false;
	}

	
	// auch für Module 1
	pRight->setModid(MODULE_RIGHTS);
	eRet = pRight->updateApplModRight();
	if (eRet != IPerst::RET_OK)
	{
		delete pRight;
		return false;
	}

	eRet = pRight->updateApplRight();
	if (eRet != IPerst::RET_OK)
	{
		delete pRight;
		return false;
	}

	delete pRight;
	return true;
}

/**---------------------------------------------------------------------------*/
void PDlgAppDetail::OnBtAppExit()
{
	CDialogMultiLang::OnCancel();
}

/**---------------------------------------------------------------------------*/
BOOL PDlgAppDetail::PreTranslateMessage(MSG  * pMsg )
{
	assert(pMsg != 0);

	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			// Fokus steht in UsrListCtrl
			if (GetFocus() == GetDlgItem(IDC_BT_APP_SAVE))
			{
				if (m_bt_App_Save.IsWindowEnabled())
				{
					OnBtAppSave();
					return TRUE;
				}
			}
			// Fokus steht in AppListCtrl
			if (GetFocus() == GetDlgItem(IDC_BT_APP_EXIT))
			{
				if (m_bt_App_Exit.IsWindowEnabled())
				{
					OnBtAppExit();
					return TRUE;
				}
			}
			return FALSE;
			
		case VK_ESCAPE:
			if (m_bt_App_Exit.IsWindowEnabled())
			{
				OnBtAppExit();
				return TRUE;
			}
			return FALSE;


		default:
			break;
		}
	} 

	return CDialogMultiLang::PreTranslateMessage(pMsg);
}