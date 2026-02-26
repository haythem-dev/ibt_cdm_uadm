/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH                                         */
/**   Date   : 04.08.2004                                                     */
/**                                                                           */
/******************************************************************************/

#include "StdAfx.h"
#include "presentation.h"
#include <BppPara.h>
#include <BConppPara.h>
#include <BTest.h>
#include "PDlgParaDetail.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/**---------------------------------------------------------------------------*/
// PDlgUserDetail-Dialogfeld
/**---------------------------------------------------------------------------*/
IMPLEMENT_DYNAMIC(PDlgParaDetail, CDialogMultiLang)

/**---------------------------------------------------------------------------*/
PDlgParaDetail::PDlgParaDetail(BppPara * pBppPara)
				: CDialogMultiLang(PDlgParaDetail::IDD ),
				m_pBppParaClone (pBppPara->clone())
{
	pParaTestChange = pBppPara->clone();

	m_pTest = new BTest;
		assert (m_pTest);
	
	assert(m_pBppParaClone);
	
	m_bParaNew = false;
	m_bDataChanged = false;

}

/**---------------------------------------------------------------------------*/
PDlgParaDetail::~PDlgParaDetail()
{
	if (m_pBppParaClone != NULL)
	{
		delete m_pBppParaClone;
		m_pBppParaClone = NULL;
	} 

	if (pParaTestChange != NULL)
	{
		delete pParaTestChange;
		pParaTestChange = NULL;
	}

	if (m_pTest != NULL)
	{
		delete m_pTest;
		m_pTest = NULL;
	}
}

/**---------------------------------------------------------------------------*/
void PDlgParaDetail::DoDataExchange(CDataExchange* pDX)
{
	CDialogMultiLang::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ED_PARA1, m_ed_Para1);
	DDX_Control(pDX, IDC_ED_PARA2, m_ed_Para2);
	DDX_Control(pDX, IDC_ED_PARABEZ, m_ed_ParaBez);
	DDX_Control(pDX, IDC_ED_PARAVAL, m_ed_ParaValue);
	DDX_Control(pDX, IDC_BT_DTL_PARA_SAVE, m_bt_Para_Save);
	DDX_Control(pDX, IDC_BT_DTL_PARA_EXIT, m_bt_Para_Exit);
	DDX_Control(pDX, IDC_ED_PARA_APP, m_ed_Para_App);
}

/**---------------------------------------------------------------------------*/
BOOL PDlgParaDetail::OnInitDialog()
{
	BeginWaitCursor();

	CDialogMultiLang::OnInitDialog();

	if(m_bParaNew == false)
	{
		m_ed_Para1.SetWindowText(m_pBppParaClone->getPara1().c_str());
		m_ed_Para2.SetWindowText(m_pBppParaClone->getPara2().c_str());
		m_ed_ParaBez.SetWindowText(m_pBppParaClone->getParabez().c_str());
		m_ed_ParaValue.SetWindowText(m_pBppParaClone->getParavalue().c_str());

		m_ed_Para1.EnableWindow(false);
		m_ed_Para2.EnableWindow(false);
		m_ed_ParaBez.EnableWindow(false);
		m_ed_ParaValue.SetFocus();
	}
	else
	{
		m_ed_Para1.SetWindowText("0");
		m_ed_Para2.SetWindowText("0");
		
		m_ed_ParaBez.SetFocus();
	}

	m_ed_Para_App.SetWindowText(m_strAppname.c_str());
	
	EndWaitCursor();

	return FALSE;  // return TRUE unless you set the focus to a control
}

/**---------------------------------------------------------------------------*/
void PDlgParaDetail::setAppname(CBString strAppname)
{
	m_strAppname = strAppname;
}

/**---------------------------------------------------------------------------*/
void PDlgParaDetail::setAppid (short sAppid)
{
	m_sAppid = sAppid;
}

/**---------------------------------------------------------------------------*/
void PDlgParaDetail::setBNewPara(bool bNewPara)
{
	m_bParaNew = bNewPara;
}

/**---------------------------------------------------------------------------*/
const bool PDlgParaDetail::getBNewPara()
{
	return m_bParaNew;
}

/**---------------------------------------------------------------------------*/
const bool	PDlgParaDetail::getBDataChanged()
{
	return m_bDataChanged;
}


/**---------------------------------------------------------------------------*/
bool PDlgParaDetail::GetCtrlValues()
{
	//Daten von GUI
	CString csPara1;
	m_ed_Para1.GetWindowText(csPara1);

	CString csPara2;
	m_ed_Para2.GetWindowText(csPara2);

	CString csParaBez;
	m_ed_ParaBez.GetWindowText(csParaBez);

	CString csParaVal;
	m_ed_ParaValue.GetWindowText(csParaVal);

	CBString strPara1 = (LPCTSTR)csPara1;
	strPara1.erasespace(CBString::END);
	int iLengthP1 = (int)strPara1.length();

	CBString strPara2 = (LPCTSTR)csPara2;
	strPara2.erasespace(CBString::END);
	int iLengthP2 = (int)strPara2.length();

	CBString strParaBez = (LPCTSTR)csParaBez;
	strParaBez.erasespace(CBString::END);
	strParaBez.upper();
	int iLengthBez = (int)strParaBez.length();

	CBString strParaVal = (LPCTSTR)csParaVal;
	strParaVal.erasespace(CBString::END);
	int iLengthVal = (int)strParaVal.length();

	bool bParaExists;

	
	
	if ( iLengthBez > 30 || iLengthBez <= 0)
	{
		MsgBoxOK(IDS_ERR_PARABEZ, MB_ICONERROR); //Bez nicht länger als 30 Zeichen oder leer
		m_ed_ParaBez.SetFocus();
		return false;
	}
	else if (iLengthVal <= 0 || iLengthVal > 254 )
	{
		MsgBoxOK(IDS_ERR_PARAVAL, MB_ICONERROR); //Value nicht länger als 254 Zeichen oder leer
		m_ed_ParaValue.SetFocus();
		return false;
	}
	else if (iLengthP1 <= 0 || strPara1 == "0")//wenn Para1 leer, dann darf auch in Para2 nichts stehen!
	{
		if ( iLengthP2 > 0 && strPara2 != "0" )
		{
			MsgBoxOK(IDS_ERR_PARA_1_2, MB_ICONERROR);
			m_ed_Para1.SetFocus();
			return false;
		}
	}
	if (!(ParaExists(strPara1, strPara1, strParaBez, bParaExists)))
	{
		MsgBoxOK(IDS_ERR_SQL_COUNT_PARA);//SQL-Abfrage auf Rightname FAULT!
		return false;
	}
	else if (bParaExists == true)
	{
		MsgBoxOK(IDS_ERR_PARA_EXISTS);//Parameter schon vorhanden!
		m_ed_ParaBez.SetFocus();
		return false;
	}	
	else
	{
		if (iLengthP1 <= 0)//kein Wert eingetragen
		{
			strPara1 = "0";
		}
		if (iLengthP2 <= 0)//kein Wert eingetragen
		{
			strPara2 = "0";
		}
		//BsnObj füllen
		m_pBppParaClone->setPara1(strPara1);
		m_pBppParaClone->setPara2(strPara2);
		m_pBppParaClone->setParabez(strParaBez);
		m_pBppParaClone->setParavalue(strParaVal);
	}
	return true;
}

/**---------------------------------------------------------------------------*/
void PDlgParaDetail::getCloneObj(BppPara * pParaChanged)
{
	*pParaChanged = *m_pBppParaClone;
}

/**---------------------------------------------------------------------------*/
bool PDlgParaDetail::ParaExists(CBString & strPara1, CBString & strPara2, CBString& strParaBez, bool & bExists)
{
	if(m_bParaNew == true)
	{
		m_pTest->setAppid(m_sAppid);
		m_pTest->setPara1(strPara1);
		m_pTest->setPara2(strPara2);
		m_pTest->setParaBez(strParaBez);
	
		if(!(m_pTest->SelParaExists(bExists)))
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
BEGIN_MESSAGE_MAP(PDlgParaDetail, CDialogMultiLang)
	ON_BN_CLICKED(IDC_BT_DTL_PARA_SAVE, OnBtDtlParaSave)
	ON_BN_CLICKED(IDC_BT_DTL_PARA_EXIT, OnBtDtlParaExit)
END_MESSAGE_MAP()

/**---------------------------------------------------------------------------*/
void PDlgParaDetail::OnBtDtlParaSave()
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
		if(*pParaTestChange == *m_pBppParaClone) //keine Änderung!
		{
			m_bDataChanged = false;
		}
		else
		{
			if(m_bParaNew == false) // UPDATE
			{
				BppPara * pParaUpdTest = new BppPara;
				pParaUpdTest->setAppid(m_pBppParaClone->getAppid());
				pParaUpdTest->setPara1(m_pBppParaClone->getPara1());
				pParaUpdTest->setPara2(m_pBppParaClone->getPara2());
				pParaUpdTest->setParabez(m_pBppParaClone->getParabez());
				IPerst::ERet Ret = pParaUpdTest->SelForUpd();
				if (Ret != IPerst::RET_OK)
				{
					//kein Update --> Datensatz zum Updaten nicht gefunden!
					MsgBoxOK(IDS_NO_UPDATE_POSS);
				}
				//else if (*pParaUpdTest != *pParaTestChange) //NASE only temporary
				else if (pParaUpdTest->getParavalue() != pParaTestChange->getParavalue())
				{
					//kein Update --> Datensatz inzwischen extern verändert!
					MsgBoxOK(IDS_UPDATED_EXTERN);
				}
				else
				{
					IPerst::ERet eRet = m_pBppParaClone->updatePara();
					if (eRet == IPerst::RET_OK)
					{
						m_bDataChanged = true;
					}
					else
					{
						MsgBoxOK(IDS_ERR_UPDATE);
					}
				}
				delete pParaUpdTest;
			}
			else //INSERT
			{
				IPerst::ERet eRet = m_pBppParaClone->insertPara();
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
void PDlgParaDetail::OnBtDtlParaExit()
{
	CDialogMultiLang::OnCancel();
}

/**---------------------------------------------------------------------------*/
BOOL PDlgParaDetail::PreTranslateMessage(MSG  * pMsg )
{
	assert(pMsg != 0);

	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			if (GetFocus() == GetDlgItem(IDC_BT_DTL_PARA_SAVE))
			{
				if (m_bt_Para_Save.IsWindowEnabled())
				{
					OnBtDtlParaSave();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_BT_DTL_PARA_EXIT))
			{
				if (m_bt_Para_Exit.IsWindowEnabled())
				{
					OnBtDtlParaExit();
					return TRUE;
				}
			}
			return FALSE;
			
		case VK_ESCAPE:
			if (m_bt_Para_Exit.IsWindowEnabled())
			{
				OnBtDtlParaExit();
				return TRUE;
			}
			return FALSE;


		default:
			break;
		}
	} 

	return CDialogMultiLang::PreTranslateMessage(pMsg);
}
