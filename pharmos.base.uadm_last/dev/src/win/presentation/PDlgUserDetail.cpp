// PDlgUserDetail.cpp : Implementierungsdatei
/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH                                         */
/**   Date   : 26.05.2004                                                     */
/**                                                                           */
/******************************************************************************/

#include "StdAfx.h"
#include "presentation.h"
#include "BConnect.h"
#include "CConnect.h"
#include <BppUser.h>
#include <BConppUser.h>
#include <BTest.h>
#include "PDlgBiDefault.h"
#include "PDlgUserDetail.h"
#include <libscsvoodoo/libscsvoodoo.h>
#include "UADMLoginManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/**---------------------------------------------------------------------------*/
// PDlgUserDetail-Dialogfeld
/**---------------------------------------------------------------------------*/
IMPLEMENT_DYNAMIC(PDlgUserDetail, CDialogMultiLang)

/**---------------------------------------------------------------------------*/
PDlgUserDetail::PDlgUserDetail(BppUser * pBppUser)
				: CDialogMultiLang(PDlgUserDetail::IDD ),
				m_pBppUserClone (pBppUser->clone())
{
	m_pTest = new BTest;
		assert (m_pTest);

	m_bLocked = false;

	pUserTestChange = NULL;
	
	assert(m_pBppUserClone);

	bool bCmp = (*m_pBppUserClone == *pBppUser);
	assert(bCmp == true);

	if (m_pBppUserClone->getUserid() != 0)
	{
		IPerst::ERet eRet;
		if (BConnect::ex().getServer() != CConnect::HOST)
		{
			eRet = m_pBppUserClone->load(false);
		}
		else
		{
			eRet = m_pBppUserClone->load(true); 
		}

		if (eRet == IPerst::RET_WARNING)
		{
			MsgBoxOK(IDS_USER_LOCKED, MB_ICONINFORMATION);
			m_bDataChanged = false;
			m_bLocked = true;
			return;
		}

		pUserTestChange = m_pBppUserClone->clone();

		bCmp = (*m_pBppUserClone == *pBppUser);
		assert(bCmp == false);

			// test start ...
			BppUser * pTmp = new BppUser;

			*pTmp          = *pBppUser;

			bCmp = (*pTmp            == *pBppUser);
			assert(bCmp == true);

			bCmp = (*m_pBppUserClone == *pBppUser);
			assert(bCmp == false);

			delete pTmp;
			/* ... end test */		
	}
	
	m_bUserNew = false;
	m_bDataChanged = false;
	m_bSaveBtEnabled = true;
	m_bBiDefaultWritten = false;

}

/**---------------------------------------------------------------------------*/
PDlgUserDetail::~PDlgUserDetail()
{
	if (m_pBppUserClone != NULL)
	{
		delete m_pBppUserClone;
		m_pBppUserClone = NULL;
	} 

	if (pUserTestChange != NULL)
	{
		delete pUserTestChange;
		pUserTestChange = NULL;
	}
	
	if (m_pTest != NULL)
	{
		delete m_pTest;
		m_pTest = NULL;
	}
}

/**---------------------------------------------------------------------------*/
void PDlgUserDetail::DoDataExchange(CDataExchange* pDX)
{
	CDialogMultiLang::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BT_SAVE, m_bt_Save);
	DDX_Control(pDX, IDC_BT_CANCEL, m_bt_Cancel);
	DDX_Control(pDX, IDC_BT_BI_DEFAULT, m_bt_BI_Default);
	DDX_Control(pDX, IDC_ED_UNAME, m_ed_Username);
	DDX_Control(pDX, IDC_CHECK_ACTIV, m_ck_Activ);
	DDX_Control(pDX, IDC_CHECK_EXTERNAL, m_ck_External);
	DDX_Control(pDX, IDC_CHECK_CHANGEPW, m_ck_ChangePW);
	DDX_Control(pDX, IDC_ED_PASSWORD, m_ed_Password);
	DDX_Control(pDX, IDC_ED_SURNAME, m_ed_Surname);
	DDX_Control(pDX, IDC_EDFORENAME, m_ed_Forename);
	DDX_Control(pDX, IDC_ED_DESCRIPTION, m_ed_Desc);
	DDX_Control(pDX, IDC_ED_MAIL, m_ed_Mail);
	DDX_Control(pDX, IDC_ED_EMAIL, m_ed_Email);
	DDX_Control(pDX, IDC_ED_NTUSER, m_ed_NtUser);
	DDX_Control(pDX, IDC_ED_REFERENCE, m_ed_Ref);
	DDX_Control(pDX, IDC_ED_LASTLOG, m_ed_LastLog);
	DDX_Control(pDX, IDC_ED_LASTPWCHANGE, m_ed_LastPwChange);
	DDX_Control(pDX, IDC_CB_USERTYPE, m_cmb_UserType);
	DDX_Control(pDX, IDC_CB_USERCOUNTRY, m_cmb_Country);
}

/**---------------------------------------------------------------------------*/
BOOL PDlgUserDetail::OnInitDialog()
{
	BeginWaitCursor();

	CDialogMultiLang::OnInitDialog();

	InitUsertypes();
	InitCountries();

	if(m_bUserNew == false)
	{
		SetCtrlValues();
	}
	else
	{
		m_ck_Activ.SetCheck(1);
		m_ck_ChangePW.SetCheck(1);
	}
	m_bt_Save.EnableWindow(m_bSaveBtEnabled);
	m_ed_Username.SetFocus();

	EndWaitCursor();

	return FALSE;  // return TRUE unless you set the focus to a control
}

/**---------------------------------------------------------------------------*/
void PDlgUserDetail::setBNewUser(bool bNewUser)
{
	m_bUserNew = bNewUser;
}

/**---------------------------------------------------------------------------*/
const bool PDlgUserDetail::getBNewUser()
{
	return m_bUserNew;
}
/**---------------------------------------------------------------------------*/
const bool PDlgUserDetail::getBLocked()
{
	return m_bLocked;
}

/**---------------------------------------------------------------------------*/
const bool	PDlgUserDetail::getBDataChanged()
{
	return m_bDataChanged;
}

/**---------------------------------------------------------------------------*/
void PDlgUserDetail::SetCtrlValues()
{
	char  cLastLog [12];
	long lLastLog = m_pBppUserClone->getLastLogin();
	SCCdtDateLongToCharTTMMJJJJ(lLastLog,cLastLog);
	m_ed_LastLog.SetWindowText(cLastLog);
	m_ed_Username	.SetWindowText(m_pBppUserClone->getUsername().c_str());
	m_ed_Password	.SetWindowText("********"); // never copy a pwd into frontend
	m_ed_Surname	.SetWindowText(m_pBppUserClone->getSurname().c_str());
	m_ed_Forename	.SetWindowText(m_pBppUserClone->getForename().c_str());
	m_ed_Desc		.SetWindowText(m_pBppUserClone->getDescription().c_str());
	m_ed_Mail		.SetWindowText(m_pBppUserClone->getMailuser().c_str());
	m_ed_Email		.SetWindowText(m_pBppUserClone->getEmailuser().c_str());
	m_ck_Activ		.SetCheck(m_pBppUserClone->getActiv());
	m_ck_ChangePW	.SetCheck(m_pBppUserClone->getChangepw());
	m_ck_External	.SetCheck(m_pBppUserClone->getExternal());
	m_ed_Ref		.SetWindowText(m_pBppUserClone->getReference().c_str());
	m_ed_NtUser		.SetWindowText(m_pBppUserClone->getNtUser().c_str());
	m_cmb_UserType.SetCurSel(-1);
	short sUserTypeID = m_pBppUserClone->getUserTypeID();
	for (int i = 0; i < m_cmb_UserType.GetCount(); i++)
	{
		if ( (short)m_cmb_UserType.GetItemData(i) == sUserTypeID )
		{
			m_cmb_UserType.SetCurSel(i);
			break;
		}
	}
	m_cmb_Country.SetCurSel(-1);
	CBString strCountry = m_pBppUserClone->getUserCountry();
	for (int j = 0; j < m_cmb_Country.GetCount(); j++)
	{
		CString strCountryItem;
		m_cmb_Country.GetLBText(j, strCountryItem);
		if ( strCountry == (LPCTSTR)strCountryItem.Left(2) )
		{
			m_cmb_Country.SetCurSel(j);
			break;
		}
	}

	char  cLastPwChange [12];
	long lLastPwChange = m_pBppUserClone->getLastChangePw();
	SCCdtDateLongToCharTTMMJJJJ(lLastPwChange,cLastPwChange);
	m_ed_LastPwChange.SetWindowText(cLastPwChange);
	
	m_strUnamePrev = m_pBppUserClone->getUsername();

	OnCkExternal();
}

/**---------------------------------------------------------------------------*/
void PDlgUserDetail::InitUsertypes()
{
	long sqlStatus;
	int  sqlHandle;
	CString sqlQuery;
	CString lastError;

	m_cmb_UserType.ResetContent();

	sqlQuery = "SELECT usertypeid, name FROM ppusertype ORDER BY usertypeid";
	sqlStatus = SqlOpenCs(&sqlHandle, sqlQuery);
	if (sqlStatus != SQL_SUCCESS)
	{
		lastError = DynErrmld();
		MsgBoxOK(lastError);
		return;
	}
	short usertypeid;
	char name[151];
	sqlStatus = SqlFetch(sqlHandle, 1, &usertypeid, &name, NULL);
	if (sqlStatus == SQL_NOTFOUND)
	{
		SqlCloseCs(sqlHandle);
		return;
	}
	do
	{
		CString strTmp;
		strTmp.Format("%s", CString(name).Trim());
		int nIndex = m_cmb_UserType.AddString(strTmp);
		m_cmb_UserType.SetItemData(nIndex, usertypeid);
		// get next
		sqlStatus = SqlFetch(sqlHandle, 1, &usertypeid, &name, NULL);
	} while (sqlStatus != SQL_NOTFOUND);

	SqlCloseCs(sqlHandle);
}

/**---------------------------------------------------------------------------*/
void PDlgUserDetail::InitCountries()
{
	m_cmb_Country.ResetContent();

	m_cmb_Country.AddString("DE - Germany");
	m_cmb_Country.AddString("AT - Austria");
	m_cmb_Country.AddString("CH - Switzerland");
	m_cmb_Country.AddString("FR - France");
	m_cmb_Country.AddString("BG - Bulgaria");
	m_cmb_Country.AddString("HR - Croatia");
}

/**---------------------------------------------------------------------------*/
bool PDlgUserDetail::isPwComplex(CBString &strPw)
{
	unsigned int i = 0;
	bool digitFound = false;
	bool upperFound = false;

	while ( (digitFound == false || upperFound == false ) &&
			i < strPw.length() )
	{
		if ( digitFound == false && isdigit(strPw.at(i)) != 0 )
			digitFound = true;

		if ( upperFound == false && isupper(strPw.at(i)) != 0 )
			upperFound = true;

		++i;
	}
	
	return digitFound == true && upperFound == true;
}

/**---------------------------------------------------------------------------*/
bool PDlgUserDetail::isNtUserValid(CBString &strNtUser)
{
	if ( strNtUser.empty() )
		return true;

	size_t i = strNtUser.find("\\"); // 1 "\" must occure
	
	if ( i == CBString::npos )
		return false;

	if ( strNtUser.find("\\", i+1) != CBString::npos )	// if more than 1 "\" --> error
		return false;

	return true;
}

/**---------------------------------------------------------------------------*/
bool PDlgUserDetail::GetCtrlValues()
{
	//Daten von GUI
	CString csUserName;
	m_ed_Username.GetWindowText(csUserName);
	CString csPW;
	m_ed_Password.GetWindowText(csPW);
	CString csSurname;
	m_ed_Surname.GetWindowText(csSurname);
	CBString cbSurname = csSurname.Left(30).GetString();
	cbSurname.firstupper();
	CString csForename;
	m_ed_Forename.GetWindowText(csForename);
	CString csDesc;
	m_ed_Desc.GetWindowText(csDesc);
	CString csMail;
	m_ed_Mail.GetWindowText(csMail);
	CString csEmail;
	m_ed_Email.GetWindowText(csEmail);
	CString csNtUser;
	m_ed_NtUser.GetWindowText(csNtUser);
	CBString strNtUser = csNtUser.Left(50).GetString();
	strNtUser.lower();
	strNtUser.erasespace(CBString::END);
	CString csRef;
	m_ed_Ref.GetWindowText(csRef);
	CBString strRef = csRef.Left(40).GetString();
	strRef.lower();
	strRef.erasespace(CBString::END);

	short sExternal = m_ck_External.GetCheck();
	short sActiv = m_ck_Activ.GetCheck();
	short sChangePW = m_ck_ChangePW.GetCheck();

	short sUserTypeID = 0;
	CBString strUserTypeName = "";
	int iSelUserType = m_cmb_UserType.GetCurSel();
	if (iSelUserType != CB_ERR)
	{
		CString strUserTypeNameItem;
		sUserTypeID = (short)m_cmb_UserType.GetItemData(iSelUserType);
		m_cmb_UserType.GetLBText(iSelUserType, strUserTypeNameItem);
		strUserTypeName = strUserTypeNameItem.Left(150).GetString();
		// add spaces until length 150
		strUserTypeName.resize(150, ' ');
	}
	CBString strCountry = "";
	int iSelCountry = m_cmb_Country.GetCurSel();
	if (iSelCountry != CB_ERR)
	{
		CString strCountryItem;
		m_cmb_Country.GetLBText(iSelCountry, strCountryItem);
		strCountry = (LPCTSTR)strCountryItem.Left(2);
	}

	//Längenprüfung
	CBString strUserName = (LPCTSTR)csUserName;
	strUserName.erasespace(CBString::END);
	CBString strPW = (LPCTSTR)csPW;
	strPW.erasespace(CBString::END);
	int iLengthN = (int)strUserName.length();
	int iLengthPW = (int)strPW.length();

	bool bUnameExists;
	bool bPWChanged = false;

	if ( iLengthN > 20 || iLengthN <= 0)
	{
		MsgBoxOK(IDS_ERR_UNAME, MB_ICONERROR); //Kennungen nicht länger als 20 Zeichen oder leer
		m_ed_Username.SetFocus();
		return false;
	}
	if (isNtUserValid(strNtUser) == false)
	{
		MsgBoxOK(IDS_ERR_NTUSER, MB_ICONERROR); //NtUser ungültig
		m_ed_NtUser.SetFocus();
		return false;
	}
	if ((strPW.compare("********") != 0) || (m_bUserNew == true)) // Passwort wurde geändert oder neuer User
	{
		if (iLengthPW > 8)
		{
			MsgBoxOK(IDS_ERR_PW, MB_ICONERROR); //Passwort nicht länger als 8 Zeichen
			m_ed_Password.SetFocus();
			return false;
		}
		else if (iLengthPW < 8)
		{
			MsgBoxOK(IDS_ERR_PW_TOO_SHORT, MB_ICONERROR); //Passwort mind. 8 Zeichen
			m_ed_Password.SetFocus();
			return false;
		}
		else if (isPwComplex(strPW) == false)
		{
			MsgBoxOK(IDS_ERR_PW_NOT_COMPLEX, MB_ICONERROR); //Passwort hat nicht mind. 1 Ziffer + 1 Großbuchstaben
			m_ed_Password.SetFocus();
			return false;
		}
		bPWChanged = true;
		if (sChangePW == 0)
		{
			MsgBoxOK(IDS_ERR_PW_CHANGEFLAG, MB_ICONERROR); //Passwortänderungsflag muss gesetzt sein, wenn Passwort geändert wird!
			m_ck_ChangePW.SetFocus();
			return false;
		}
	}
	if (!(UserNameExists(strUserName, bUnameExists)))
	{
		MsgBoxOK(IDS_ERR_SQL_COUNT_UNAME);//SQL-Abfrage auf Username FAULT!
		return false;
	}
	if (bUnameExists == true)
	{
		MsgBoxOK(IDS_ERR_UNAME_EXISTS, MB_ICONERROR);//Username schon vorhanden!
		m_ed_Username.SetFocus();
		return false;
	}	
	if (sExternal == 1 && strRef.empty())
	{
		MsgBoxOK(IDS_EXTERNAL_IDF, MB_ICONERROR);//für externe User muss reference eingegeben werden!
		m_ed_Ref.SetFocus();
		return false;
	}
	if (sUserTypeID <= 0)
	{
		MsgBoxOK(IDS_ERR_USERTYPE, MB_ICONERROR); //Usertype muss ausgewählt werden!
		m_cmb_UserType.SetFocus();
		return false;
	}
	if (strCountry.empty())
	{
		MsgBoxOK(IDS_ERR_USERCOUNTRY, MB_ICONERROR); //Country muss ausgewählt werden!
		m_cmb_Country.SetFocus();
		return false;
	}
	else
	{
		//BsnObj füllen
		m_pBppUserClone->setDescription((LPCTSTR)csDesc.Left(254));
		m_pBppUserClone->setActiv(sActiv);
		m_pBppUserClone->setExternal(sExternal);
		m_pBppUserClone->setChangePW(sChangePW);
		m_pBppUserClone->setEmailuser((LPCTSTR)csEmail.Left(50));
		m_pBppUserClone->setForename((LPCTSTR)csForename.Left(20));
		m_pBppUserClone->setMailuser((LPCTSTR)csMail.Left(30));
		m_pBppUserClone->setSurname(cbSurname);
		m_pBppUserClone->setUsername(strUserName);
		m_pBppUserClone->setNtUser((LPCTSTR)strNtUser);
		m_pBppUserClone->setReference((LPCTSTR)strRef);
		m_pBppUserClone->setUserTypeID(sUserTypeID);
		m_pBppUserClone->setUserCountry(strCountry);
		m_pBppUserClone->setUserTypeName(strUserTypeName);
		m_pBppUserClone->setUserCountryName(strCountry);
		CBString oldPHXPW = m_pBppUserClone->getPHXPasswd();
		CBString oldAESPW = m_pBppUserClone->getAESPasswd();
		if (bPWChanged == true)
		{
			if (oldPHXPW.compare(oldAESPW) != 0)
			{
				bPWChanged = true;
			}
			//Encrypt new pwd with PHX and AES type and set it
			UADM::CUADMLoginManager mgr;
			m_pBppUserClone->setPHXPasswd(mgr.encryptPassword(strPW.c_str(), strUserName.c_str(), 1).c_str());
			m_pBppUserClone->setAESPasswd(mgr.encryptPassword(strPW.c_str(), strUserName.c_str(), 2).c_str());
		}
	}
	return true;
}

/**---------------------------------------------------------------------------*/
bool PDlgUserDetail::UserNameExists(CBString &strUserName, bool & bExists)
{
	if( m_bUserNew == false )
	{
		if ( m_strUnamePrev == strUserName )
		{
			bExists = false;
			return true;
		}
	}
	
	m_pTest->setUsername(strUserName);
	
	if(!(m_pTest->SelUNameExists(bExists)))
	{
		return false;
	}
	return true;
}

/**---------------------------------------------------------------------------*/
void PDlgUserDetail::OnBtCancel()
{
	CDialogMultiLang::OnCancel();
}

/**---------------------------------------------------------------------------*/
void PDlgUserDetail::getCloneObj(BppUser * pUserChanged)
{
	*pUserChanged = *m_pBppUserClone;
}

/**---------------------------------------------------------------------------*/
void PDlgUserDetail::OnOK()
{
	OnBtSave();	
}

/**---------------------------------------------------------------------------*/
void PDlgUserDetail::SetSaveBtDisabled  ()
{
	m_bSaveBtEnabled = false;
}

/**---------------------------------------------------------------------------*/
void PDlgUserDetail::OnBtSave()
{
	CBString strRight;
	
	if(m_bUserNew == false) // UPDATE
		strRight = RIGHT_WORK_RI;
	else //INSERT --> nur eingeschränkter Benutzerkreis !
		strRight = RIGHT_USRDEL_RI;
		
	if (!(ppBLoginData::ex().isLegitimated(MODULE_RIGHTS,strRight)))
	{
		MsgBoxOK(IDS_NO_RIGHT);
		return;
	}
	
	if(GetCtrlValues()) //Daten von GUI eingelesen!
	{
		if(m_bUserNew == false) // UPDATE
		{
			if(*pUserTestChange == *m_pBppUserClone) //keine Änderung!
			{
				m_bDataChanged = false;
			}
			else
			{
				BppUser * pUserUpdTest = new BppUser;
				pUserUpdTest->setUserid(m_pBppUserClone->getUserid());
				IPerst::ERet Ret = pUserUpdTest->SelForUpd();
				if (Ret != IPerst::RET_OK)
				{
					//kein Update --> Datensatz zum Updaten nicht gefunden!
					MsgBoxOK(IDS_NO_UPDATE_POSS);
				}
				else if (*pUserUpdTest != *pUserTestChange)
				{
					//kein Update --> Datensatz inzwischen extern verändert!
					MsgBoxOK(IDS_UPDATED_EXTERN);
				}
				else
				{
					IPerst::ERet eRet = m_pBppUserClone->update();
					if (eRet == IPerst::RET_OK)
					{
						m_bDataChanged = true;
					}
					else
					{
						MsgBoxOK(IDS_ERR_UPDATE);
					}
				}
				delete pUserUpdTest;
			}
		}
		else //INSERT
		{
			IPerst::ERet eRet = m_pBppUserClone->insert();
			if (eRet == IPerst::RET_OK)
			{
				eRet = m_pBppUserClone->selectID();
				if (eRet != IPerst::RET_OK) //Fehler beim Insert
				{
					MsgBoxOK(IDS_ERR_INSERT);
				}
				m_bDataChanged = true;

				// remind for bidefault-input, if new user and ntuser and surname are set!
				if ( !m_pBppUserClone->getNtUser().empty() && m_bBiDefaultWritten == false ) 
				{
					if ( MsgBoxYesNo(IDS_INPUT_FOR_BIDEFAULT) == IDYES )
					{
						PDlgBiDefault * dlgBiDef = new PDlgBiDefault(m_pBppUserClone->getNtUser(),
														m_pBppUserClone->getSurname(),
														m_pBppUserClone->getForename(),
														m_pBppUserClone->getEmailuser());
			
						dlgBiDef->DoModal();

						delete dlgBiDef;
					}
				}
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
void PDlgUserDetail::OnBIDefault()
{
	CBString strRight = RIGHT_WORK_RI;
	if (!(ppBLoginData::ex().isLegitimated(MODULE_RIGHTS,strRight)))
	{
		MsgBoxOK(IDS_NO_RIGHT);
		return;
	}

	if(GetCtrlValues()) //Daten von GUI eingelesen!
	{
		/*if( *pUserTestChange != *m_pBppUserClone) //Änderung!
		{
			MsgBoxOK(IDS_MSG_SAVE_FIRST);
			return;
		}*/
		
		if ( m_pBppUserClone->getNtUser().empty() )
		{
			MsgBoxOK(IDS_MSG_NTUSER_SURNAME_EMPTY);
			m_ed_NtUser.SetFocus();
			return;
		}
		else
		{
			PDlgBiDefault * dlgBiDef = new PDlgBiDefault(m_pBppUserClone->getNtUser(),
														m_pBppUserClone->getSurname(),
														m_pBppUserClone->getForename(),
														m_pBppUserClone->getEmailuser());
			
			dlgBiDef->DoModal();

			m_bBiDefaultWritten = true;

			delete dlgBiDef;
		}
	}
}

/**---------------------------------------------------------------------------*/
void PDlgUserDetail::OnCkExternal()
{
	if (m_ck_External.GetCheck() == 0)
	{
		m_ed_Ref.SetWindowText("");
		m_ed_Ref.EnableWindow(false);
	}
	else
	{
		m_ed_Ref.EnableWindow(true);
	}
}

/**---------------------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(PDlgUserDetail, CDialogMultiLang)
	ON_BN_CLICKED(IDC_BT_CANCEL, OnBtCancel)
	ON_BN_CLICKED(IDC_BT_SAVE, OnBtSave)
	ON_BN_CLICKED(IDC_BT_BI_DEFAULT, OnBIDefault)
	ON_BN_CLICKED(IDC_CHECK_EXTERNAL, OnCkExternal)
END_MESSAGE_MAP()
