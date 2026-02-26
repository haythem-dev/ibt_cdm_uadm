/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH                                         */
/**   Date   : 06.06.2005                                                     */
/**                                                                           */
/******************************************************************************/

#include "StdAfx.h"
#include "presentation.h"
#include <BBiDefault.h>
#include <BConnectOdbc.h>

#include <CConnect.h>

#include "PDlgBiDefault.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/**---------------------------------------------------------------------------*/
// PDlgUserDetail-Dialogfeld
/**---------------------------------------------------------------------------*/
IMPLEMENT_DYNAMIC(PDlgBiDefault, CDialogMultiLang)

/**---------------------------------------------------------------------------*/
PDlgBiDefault::PDlgBiDefault(const CBString& cbNtUser, 
				  const CBString& cbSurname, const CBString& cbForename, const CBString& cbEmail)
				: CDialogMultiLang(PDlgBiDefault::IDD ), m_cbNtUser(cbNtUser), 
				m_cbSurname(cbSurname), m_cbForename(cbForename), m_cbEmail(cbEmail)
{
	m_pobjBiDef = new BBiDefault;
			assert (m_pobjBiDef);
}

/**---------------------------------------------------------------------------*/
PDlgBiDefault::~PDlgBiDefault()
{
	if (m_pobjBiDef != NULL)
	{
		delete m_pobjBiDef;
		m_pobjBiDef = NULL;
	} 
}

/**---------------------------------------------------------------------------*/
void PDlgBiDefault::DoDataExchange(CDataExchange* pDX)
{
	CDialogMultiLang::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BT_BIDEFAULT_SAVE, m_bt_BiDef_Save);
	DDX_Control(pDX, IDC_BT_IBI_COUNTRYR, m_bt_Ibi_CountryR);
	DDX_Control(pDX, IDC_BT_BIDEFAULT_CANCEL, m_bt_BiDef_Exit);
	DDX_Control(pDX, IDC_ED_BIDEF_NTUSER, m_ed_NtUser);
	DDX_Control(pDX, IDC_ED_BIDEF_SURNAME, m_ed_Surname);
	DDX_Control(pDX, IDC_ED_BIDEF_FORENAME, m_ed_Forename);
	DDX_Control(pDX, IDC_ED_BIDEF_EMAIL,  m_ed_Email);
	DDX_Control(pDX, IDC_ED_BIDEF_REMARK, m_ed_Remark);
	DDX_Control(pDX, IDC_ED_BIDEF_BRANCH, m_ed_Branch);
	//DDX_Control(pDX, IDC_ED_BIDEF_COUNTRY, m_ed_Country);
	DDX_Control(pDX, IDC_ED_BIDEF_STYLE, m_ed_Style);
	DDX_Control(pDX, IDC_ED_BIDEF_FORMAT, m_ed_Format);
	DDX_Control(pDX, IDC_CB_BIDEF_COUNTRY, m_cb_Country);
}

/**---------------------------------------------------------------------------*/
BOOL PDlgBiDefault::OnInitDialog()
{
	BeginWaitCursor();

	CDialogMultiLang::OnInitDialog();

	m_ed_NtUser.SetWindowText(m_cbNtUser.c_str());
	m_ed_Surname.SetWindowText(m_cbSurname.c_str());
	m_ed_Forename.SetWindowText(m_cbForename.c_str());
	m_ed_Email.SetWindowText(m_cbEmail.c_str());

	CBString countries;
	CBString strParaCountries(PARA_BI_COUNTRIES);
	ppBLoginData::ex().getParaValue(strParaCountries, countries);
	size_t start = 0;
	size_t end	 = countries.find(";", start);
	while ( end != CBString::npos )
	{
		m_cb_Country.AddString(countries.substr((int)start,(int)end - (int)start));
		start = end + 1;
		end	 = countries.find(";", start);
	}
	m_cb_Country.SetCurSel(m_cb_Country.FindString(0, IBIUSER_STD_COUNTRY));
	m_ed_Style.SetWindowText(IBIUSER_STD_STYLE);
	m_ed_Format.SetWindowText(IBIUSER_STD_FORMAT);
	
	m_ed_Branch.SetFocus();

	EndWaitCursor();

	return FALSE;  // return TRUE unless you set the focus to a control
}

/**---------------------------------------------------------------------------*/
bool PDlgBiDefault::GetCtrlValues()
{
	//Daten von GUI
	CString csRemark;
	m_ed_Remark.GetWindowText(csRemark);

	CString csEmail;
	m_ed_Email.GetWindowText(csEmail);	

	CString csFormat;
	m_ed_Format.GetWindowText(csFormat);

	CString csStyle;
	m_ed_Style.GetWindowText(csStyle);

	CString csCountry;
	m_cb_Country.GetLBText(m_cb_Country.GetCurSel(),csCountry);

	CString csBranch;
	m_ed_Branch.GetWindowText(csBranch);


	//Längenprüfung 
	CBString cbEmail = csEmail.GetString();
	cbEmail.erasespace(CBString::END);
	int iLengthE = (int)cbEmail.length();

	CBString cbRemark = csRemark.GetString();
	cbRemark.erasespace(CBString::END);
	int iLengthR = (int)cbRemark.length();

	CBString cbCountry = csCountry.GetString();
	cbCountry.erasespace(CBString::END);
	cbCountry.upper();
	int iLengthC = (int)cbCountry.length();
	
	if ( iLengthR > 50 )
	{
		MsgBoxOK(IDS_ERR_REMARK, MB_ICONERROR); 
		m_ed_Remark.SetFocus();
		return false;
	}
	else if ( iLengthE > 50 )
	{
		MsgBoxOK(IDS_ERR_REMARK, MB_ICONERROR); 
		m_ed_Email.SetFocus();
		return false;
	}
	else if ( iLengthC > 3 )
	{
		MsgBoxOK(IDS_ERR_COUNTRYCODE, MB_ICONERROR); 
		m_cb_Country.SetFocus();
		return false;
	}
	else if ( csBranch.IsEmpty() )
	{
		MsgBoxOK(IDS_ERR_NO_BRANCH_INPUT, MB_ICONERROR); 
		m_ed_Branch.SetFocus();
		return false;
	}
	else
	{
		//BsnObj füllen 
		CBString str = csBranch.GetString();
		str.erasespace(CBString::END);
		short sBranchid = atoi(str);
		m_pobjBiDef->setBranchid(sBranchid);
		m_pobjBiDef->setNtUser(m_cbNtUser);
		m_pobjBiDef->setSurname(m_cbSurname);
		m_pobjBiDef->setForename(m_cbForename);
		m_pobjBiDef->setRemark(cbRemark);
		m_pobjBiDef->setEmail(cbEmail);
		m_pobjBiDef->setCountry(cbCountry);
		m_pobjBiDef->setFormat(csFormat.GetString());
		m_pobjBiDef->setStyle(csStyle.GetString());
	}
	return true;
}

/**---------------------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(PDlgBiDefault, CDialogMultiLang)
	ON_BN_CLICKED(IDC_BT_BIDEFAULT_SAVE, OnBtBiDefaultSave)
	ON_BN_CLICKED(IDC_BT_BIDEFAULT_CANCEL, OnBtBiDefaultExit)
	ON_BN_CLICKED(IDC_BT_IBI_COUNTRYR, OnBtIbiCountryR)
END_MESSAGE_MAP()

	
/**---------------------------------------------------------------------------*/
bool PDlgBiDefault::ConnectOdbc()
{
	if( BConnectOdbc::ex().IsOdbcOpen() )
		return true;
	
	CBString strConnVal, 
		     strUIDVal, 
			 strPwdVal; 

	if ( CConnect::TEST_CONNECTED )
	{
		CBString strConnPara(PARA_CONNECT_TEST),
					strUIDPara(PARA_UID_TEST),
					strPwdPara(PARA_PWD_TEST);
		
		ppBLoginData::ex().getParaValue(strConnPara, strConnVal);
		ppBLoginData::ex().getParaValue(strUIDPara , strUIDVal );
		ppBLoginData::ex().getParaValue(strPwdPara , strPwdVal );
	}
	else
	{
		CBString strConnPara(PARA_CONNECT),
					strUIDPara(PARA_UID),
					strPwdPara(PARA_PWD);
			
		ppBLoginData::ex().getParaValue(strConnPara, strConnVal);
		ppBLoginData::ex().getParaValue(strUIDPara , strUIDVal );
		ppBLoginData::ex().getParaValue(strPwdPara , strPwdVal );
	}

	BConnectOdbc::ex().setConnect(strConnVal, strUIDVal, strPwdVal);
	
	CBString strError;
	
	if (BConnectOdbc::ex().connect(strError) == false)
	{
		MsgBoxOK(strError, MB_ICONERROR);
		return false;
	}

	return true;
}


/**---------------------------------------------------------------------------*/
void PDlgBiDefault::OnBtBiDefaultSave()
{
	CString csMsg;
	
	if(GetCtrlValues()) //Daten von GUI eingelesen!
	{
		if ( ConnectOdbc() == false )
			return;

		int iCountIbiUser = 0;
		IPerst::ERet eRet = m_pobjBiDef->selcountIBIUser(&iCountIbiUser);

		if ( eRet != IPerst::RET_OK )
		{
			CBString strHlp;
			strHlp = m_pobjBiDef->getErrorMsg();
			strHlp.exchange("\n", "\r\n");

			csMsg.Format(CResString::ex().getStrTblText(IDS_MSG_ERR_SELCOUNT_IBIUSER), 
						strHlp.c_str());
			
			MsgBoxOK(csMsg.GetString());

			return;
		} 

		if ( iCountIbiUser > 0 )
		{		
			INT_PTR iRet = MsgBoxYesNo(IDS_UPDATE_IBIUSER);

			if(iRet == IDYES)
			{
				eRet = m_pobjBiDef->updateIBIUser();
				if ( eRet != IPerst::RET_OK )
				{
					CBString strHlp;
					strHlp = m_pobjBiDef->getErrorMsg();
					strHlp.exchange("\n", "\r\n");

					csMsg.Format(CResString::ex().getStrTblText(IDS_MSG_ERR_UPD_IBIUSER), 
								strHlp.c_str());
					
					MsgBoxOK(csMsg.GetString());

					return;
				}
				else
				{
					MsgBoxOK(IDS_MSG_BIDEF_OK);
				}
			}
			else
				return;
		}
		else
		{
			eRet = m_pobjBiDef->insertIBIUser();
			if ( eRet != IPerst::RET_OK )
			{
				CBString strHlp;
				strHlp = m_pobjBiDef->getErrorMsg();
				strHlp.exchange("\n", "\r\n");

				csMsg.Format(CResString::ex().getStrTblText(IDS_MSG_ERR_INS_IBIUSER), 
							strHlp.c_str());
				
				MsgBoxOK(csMsg.GetString());

				return;
			} 
			else
			{
				MsgBoxOK(IDS_MSG_BIDEF_OK);
			}
		}
	}
}

/**---------------------------------------------------------------------------*/
void PDlgBiDefault::OnBtIbiCountryR()
{
	CString csCountry;
	m_cb_Country.GetLBText(m_cb_Country.GetCurSel(),csCountry);

	CBString cbCountry = csCountry.GetString();
	cbCountry.erasespace(CBString::END);
	int iLengthC = (int)cbCountry.length();
	
	if ( iLengthC > 3 )
	{
		MsgBoxOK(IDS_ERR_COUNTRYCODE, MB_ICONERROR); 
		m_cb_Country.SetFocus();
		return;
	}
	else
	{
		//BsnObj füllen 
		m_pobjBiDef->setNtUser(m_cbNtUser);
		m_pobjBiDef->setCountry(cbCountry);
	}
	
	if ( ConnectOdbc() == false )
			return;
	
	CString csMsg;


	int iCountIbiUserCountryR = 0;
	IPerst::ERet eRet = m_pobjBiDef->selcountIBIUserCountryR(&iCountIbiUserCountryR);

	if ( eRet != IPerst::RET_OK )
	{
		CBString strHlp;
		strHlp = m_pobjBiDef->getErrorMsg();
		strHlp.exchange("\n", "\r\n");

		csMsg.Format(CResString::ex().getStrTblText(IDS_MSG_ERR_SELCOUNT_IBIUSERCOUNTRYR), 
					strHlp.c_str());
		
		MsgBoxOK(csMsg.GetString());

		return;
	} 

	if ( iCountIbiUserCountryR == 0 )
	{		
		eRet = m_pobjBiDef->insertIBIUserCountryRights();
		if ( eRet != IPerst::RET_OK )
		{
			CBString strHlp;
			strHlp = m_pobjBiDef->getErrorMsg();
			strHlp.exchange("\n", "\r\n");

			csMsg.Format(CResString::ex().getStrTblText(IDS_MSG_ERR_INS_IBIUSERCOUNTRYRIGHTS), 
						strHlp.c_str());
			
			MsgBoxOK(csMsg.GetString());

			return;
		} 
	}
	
	MsgBoxOK(IDS_MSG_BIDEF_OK);
}

/**---------------------------------------------------------------------------*/
void PDlgBiDefault::OnBtBiDefaultExit()
{
	CDialogMultiLang::OnCancel();
}

/**---------------------------------------------------------------------------*/
BOOL PDlgBiDefault::PreTranslateMessage(MSG  * pMsg )
{
	assert(pMsg != 0);

	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			if (GetFocus() == GetDlgItem(IDC_BT_BIDEFAULT_SAVE))
			{
				if (m_bt_BiDef_Save.IsWindowEnabled())
				{
					OnBtBiDefaultSave();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_BT_BIDEFAULT_CANCEL))
			{
				if (m_bt_BiDef_Exit.IsWindowEnabled())
				{
					OnBtBiDefaultExit();
					return TRUE;
				}
			}

			if (GetFocus() == GetDlgItem(IDC_BT_IBI_COUNTRYR))
			{
				if (m_bt_Ibi_CountryR.IsWindowEnabled())
				{
					OnBtIbiCountryR();
					return TRUE;
				}
			}
			return FALSE;
			
		case VK_ESCAPE:
			if (m_bt_BiDef_Exit.IsWindowEnabled())
			{
				OnBtBiDefaultExit();
				return TRUE;
			}
			return FALSE;


		default:
			break;
		}
	} 

	return CDialogMultiLang::PreTranslateMessage(pMsg);
}