/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH                                         */
/**   Date   : 30.06.2004                                                     */
/**                                                                           */
/******************************************************************************/

#include "StdAfx.h"
#include "presentation.h"
#include <BppBranchReg.h>
#include <BConppBranchReg.h>
#include <BTest.h>
#include "PDlgBrRegDetail.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/**---------------------------------------------------------------------------*/
// PDlgUserDetail-Dialogfeld
/**---------------------------------------------------------------------------*/
IMPLEMENT_DYNAMIC(PDlgBrRegDetail, CDialogMultiLang)

/**---------------------------------------------------------------------------*/
PDlgBrRegDetail::PDlgBrRegDetail(BppBranchReg * pBppBrReg)
				: CDialogMultiLang(PDlgBrRegDetail::IDD ),
				m_pBppBrRegClone (pBppBrReg->clone())
{
	m_pTest = new BTest;
		assert (m_pTest);

	pBrRegTestChange = pBppBrReg->clone();
	
	assert(m_pBppBrRegClone);
	
	m_bBrRegNew = false;
	m_bDataChanged = false;
}

/**---------------------------------------------------------------------------*/
PDlgBrRegDetail::~PDlgBrRegDetail()
{
	if (m_pBppBrRegClone != NULL)
	{
		delete m_pBppBrRegClone;
		m_pBppBrRegClone = NULL;
	} 

	if (pBrRegTestChange != NULL)
	{
		delete pBrRegTestChange;
		pBrRegTestChange = NULL;
	}
	
	if (m_pTest != NULL)
	{
		delete m_pTest;
		m_pTest = NULL;
	}
}

/**---------------------------------------------------------------------------*/
void PDlgBrRegDetail::DoDataExchange(CDataExchange* pDX)
{
	CDialogMultiLang::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_TXT_IDVALUE, m_txt_IDValue);
	DDX_Control(pDX, IDC_ED_ID, m_ed_ID);
	DDX_Control(pDX, IDC_ED_NAME, m_ed_Name);
	DDX_Control(pDX, IDC_BT_BRREG_SAVE, m_bt_BrReg_Save);
	DDX_Control(pDX, IDC_BT_BRREG_EXIT, m_bt_BrReg_Exit);
}

/**---------------------------------------------------------------------------*/
BOOL PDlgBrRegDetail::OnInitDialog()
{
	BeginWaitCursor();

	CDialogMultiLang::OnInitDialog();

	if(m_bBranch == true)
	{
		this->SetWindowText(CResString::ex().getStrTblText(IDS_BRDTL_TITLE));
		m_txt_IDValue.SetWindowText(CResString::ex().getStrTblText(IDS_BRID_VALUE));
	}
	else
	{
		this->SetWindowText(CResString::ex().getStrTblText(IDS_REGDTL_TITLE));
		m_txt_IDValue.SetWindowText(CResString::ex().getStrTblText(IDS_REGID_VALUE));
	}
	
	if(m_bBrRegNew == false)
	{
		if(m_bBranch == true)
		{
			m_ed_ID.SetWindowText(m_pBppBrRegClone->getBranchidStr().c_str());
			m_ed_Name.SetWindowText(m_pBppBrRegClone->getBranchname().c_str());
		}
		else
		{
			m_ed_ID.SetWindowText(m_pBppBrRegClone->getRegidStr().c_str());
			m_ed_Name.SetWindowText(m_pBppBrRegClone->getRegname().c_str());
		}
		m_ed_ID.EnableWindow(false);
		m_ed_Name.SetFocus();
	}
	else
	{
		m_ed_ID.SetFocus();	
	}

	EndWaitCursor();

	return FALSE;  // return TRUE unless you set the focus to a control
}

/**---------------------------------------------------------------------------*/
void PDlgBrRegDetail::setBNewBrReg(bool bNewBrReg)
{
	m_bBrRegNew = bNewBrReg;
}

/**---------------------------------------------------------------------------*/
void PDlgBrRegDetail::setBBranch (bool bBranch)
{
	m_bBranch = bBranch;
}

/**---------------------------------------------------------------------------*/
const bool PDlgBrRegDetail::getBNewBrReg()
{
	return m_bBrRegNew;
}

/**---------------------------------------------------------------------------*/
const bool	PDlgBrRegDetail::getBDataChanged()
{
	return m_bDataChanged;
}

/**---------------------------------------------------------------------------*/
bool PDlgBrRegDetail::GetCtrlValues()
{
	//Daten von GUI
	CString csBrRegid;
	m_ed_ID.GetWindowText(csBrRegid);

	CString csBrRegname;
	m_ed_Name.GetWindowText(csBrRegname);
	CBString strName = (LPCTSTR)csBrRegname;
	strName.erasespace(CBString::END);
	int iLengthN = (int)strName.length();

	CBString str = (LPCTSTR)csBrRegid;
	str.erasespace(CBString::END);
	short sBrRegid = atoi(str);

	bool bBrRegidExists;

	if ( iLengthN > 30 || iLengthN <= 0 )
	{
		MsgBoxOK(IDS_ERR_BRREGNAME, MB_ICONERROR); //Name > 30
		m_ed_Name.SetFocus();
		return false;
	}
	if ( m_bBranch == true )
	{
		if ( sBrRegid <= 0 || sBrRegid > 999)
		{
			MsgBoxOK(IDS_ERR_BRANCHID); //branchid nicht leer oder > 999
			m_ed_ID.SetFocus();
			return false;
		}
	}
	else
	{
		if ( sBrRegid < 1000)
		{
			MsgBoxOK(IDS_ERR_REGID); //regid nicht < 1000
			m_ed_ID.SetFocus();
			return false;
		}
	}
	if (!(BrRegIDExists(sBrRegid, bBrRegidExists)))
	{
		MsgBoxOK(IDS_ERR_SQL_COUNT_BRREGID);//SQL-Abfrage auf BRREGID FAULT!
		return false;
	}
	else if (bBrRegidExists == true)
	{
		MsgBoxOK(IDS_ERR_BRREGID_EXISTS, MB_ICONERROR);//BrRegid schon vorhanden!
		m_ed_ID.SetFocus();
		return false;
	}	
	else
	{
		//BsnObj füllen
		if (m_bBranch == true)
		{
			m_pBppBrRegClone->setBranchid(sBrRegid);
			m_pBppBrRegClone->setBranchname(strName);
		}
		else
		{
			m_pBppBrRegClone->setRegid(sBrRegid);
			m_pBppBrRegClone->setRegname(strName);
		}
	}
	return true;
}

/**---------------------------------------------------------------------------*/
bool PDlgBrRegDetail::BrRegIDExists(short &sBrRegid, bool & bExists)
{
	
	if (m_bBrRegNew == true)
	{
		m_pTest->setBrRegid(sBrRegid);
	
		if(  m_bBranch == true )
		{
			if(!(m_pTest->SelBrIDExists(bExists)))
			{
				return false;
			}
			return true;
		}
		else
		{
			if(!(m_pTest->SelRegIDExists(bExists)))
			{
				return false;
			}
			return true;
		}
	}
	else
	{
		bExists = false;
		return true;
	}
}

/**---------------------------------------------------------------------------*/
void PDlgBrRegDetail::getCloneObj(BppBranchReg * pBrRegChanged)
{
	*pBrRegChanged = *m_pBppBrRegClone;
}


/**---------------------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(PDlgBrRegDetail, CDialogMultiLang)
	ON_BN_CLICKED(IDC_BT_BRREG_SAVE, OnBtBrregSave)
	ON_BN_CLICKED(IDC_BT_BRREG_EXIT, OnBtBrregExit)
END_MESSAGE_MAP()

/**---------------------------------------------------------------------------*/
void PDlgBrRegDetail::OnBtBrregSave()
{
	if(GetCtrlValues()) //Daten von GUI eingelesen!
	{
		if(*pBrRegTestChange == *m_pBppBrRegClone) //keine Änderung!
		{
			m_bDataChanged = false;
		}
		else
		{
			IPerst::ERet eRet;
			if(m_bBrRegNew == false) // UPDATE
			{
				CBString strRight = RIGHT_WORK_DLL;//dllintern
				if (!(ppBLoginData::ex().isLegitimated(MODULE_DLL,strRight)))
				{
					MsgBoxOK(IDS_NO_RIGHT);
					return;
				}
				
				BppBranchReg * pBrRegUpdTest = new BppBranchReg;
				if ( m_bBranch == true )
				{
					pBrRegUpdTest->setBranchid(m_pBppBrRegClone->getBranchid());
					IPerst::ERet Ret = pBrRegUpdTest->SelForUpdBr();
					if (Ret != IPerst::RET_OK)
					{
						//kein Update --> Datensatz zum Updaten nicht gefunden!
						MsgBoxOK(IDS_NO_UPDATE_POSS);
					}
					else if (*pBrRegUpdTest != *pBrRegTestChange)
					{
						//kein Update --> Datensatz inzwischen extern verändert!
						MsgBoxOK(IDS_UPDATED_EXTERN);
					}
					else
					{
						eRet = m_pBppBrRegClone->updateBranch();
						if (eRet == IPerst::RET_OK)
						{
							m_bDataChanged = true;
						}
						else
						{
							MsgBoxOK(IDS_ERR_UPDATE);
						}
					}
				}
				else
				{
					pBrRegUpdTest->setRegid(m_pBppBrRegClone->getRegid());
					IPerst::ERet Ret = pBrRegUpdTest->SelForUpdReg();
					if (Ret != IPerst::RET_OK)
					{
						//kein Update --> Datensatz zum Updaten nicht gefunden!
						MsgBoxOK(IDS_NO_UPDATE_POSS);
					}
					else if (*pBrRegUpdTest != *pBrRegTestChange)
					{
						//kein Update --> Datensatz inzwischen extern verändert!
						MsgBoxOK(IDS_UPDATED_EXTERN);
					}
					else
					{
						eRet = m_pBppBrRegClone->updateRegion();
						if (eRet == IPerst::RET_OK)
						{
							m_bDataChanged = true;
						}
						else
						{
							MsgBoxOK(IDS_ERR_UPDATE);
						}
					}
				}
				delete pBrRegUpdTest;
			}
			else //INSERT
			{
				if ( m_bBranch == true )
				{
					eRet = m_pBppBrRegClone->insertBranch();
				}
				else
				{
					eRet = m_pBppBrRegClone->insertRegion();
				}

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
void PDlgBrRegDetail::OnBtBrregExit()
{
	CDialogMultiLang::OnCancel();
}

/**---------------------------------------------------------------------------*/
BOOL PDlgBrRegDetail::PreTranslateMessage(MSG  * pMsg )
{
	assert(pMsg != 0);

	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			if (GetFocus() == GetDlgItem(IDC_BT_BRREG_SAVE))
			{
				if (m_bt_BrReg_Save.IsWindowEnabled())
				{
					OnBtBrregSave();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_BT_BRREG_EXIT))
			{
				if (m_bt_BrReg_Exit.IsWindowEnabled())
				{
					OnBtBrregExit();
					return TRUE;
				}
			}
			return FALSE;
			
		case VK_ESCAPE:
			if (m_bt_BrReg_Exit.IsWindowEnabled())
			{
				OnBtBrregExit();
				return TRUE;
			}
			return FALSE;


		default:
			break;
		}
	} 

	return CDialogMultiLang::PreTranslateMessage(pMsg);
}


