/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH                                         */
/**   Date   : 01.07.2004                                                     */
/**                                                                           */
/******************************************************************************/

#include "StdAfx.h"

#include <vector>
using std::vector;
class C_VecItem : public vector <int>{};

#include "presentation.h"
#include <BppBranchReg.h>
#include <BConppBranchReg.h>
#include "PDlgBranchListCtrl.h"
#include "PDlgCorBrReg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/**---------------------------------------------------------------------------*/
// 
/**---------------------------------------------------------------------------*/
IMPLEMENT_DYNAMIC(PDlgCorBrReg, CDialogMultiLang)

/**---------------------------------------------------------------------------*/
PDlgCorBrReg::PDlgCorBrReg(BppBranchReg * pBppBrReg)
				: CDialogMultiLang(PDlgCorBrReg::IDD )
{
	m_pBppBrReg = pBppBrReg;
		assert(m_pBppBrReg);

	m_pli_BrCOR = new PDlgBranchListCtrl;
	   assert(m_pli_BrCOR);

	m_pli_BrALL = new PDlgBranchListCtrl;
	   assert(m_pli_BrALL);

	   m_bLoadNew = false;

}

/**---------------------------------------------------------------------------*/
PDlgCorBrReg::~PDlgCorBrReg()
{
	if (m_pli_BrCOR != NULL)
	{
		delete m_pli_BrCOR;
		m_pli_BrCOR = NULL;
	}

	if (m_pli_BrALL != NULL)
	{
		delete m_pli_BrALL;
		m_pli_BrALL = NULL;
	}
}

/**---------------------------------------------------------------------------*/
void PDlgCorBrReg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMultiLang::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LI_BR_COR  , *m_pli_BrCOR);
	DDX_Control(pDX, IDC_LI_BR_ALL  , *m_pli_BrALL);
	DDX_Control(pDX, IDC_ED_APP, m_ed_Appname);
	DDX_Control(pDX, IDC_ED_REG, m_ed_Regname);
	DDX_Control(pDX, IDC_BT_BR_COR, m_bt_Br_Cor);
	DDX_Control(pDX, IDC_BT_BR_ERASE, m_bt_Br_Erase);
	DDX_Control(pDX, IDC_BT_BRCOR_EXIT, m_bt_BrCor_Exit);
}

/**---------------------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(PDlgCorBrReg, CDialogMultiLang)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LI_BR_ALL, OnItemchangedAllListCtrl)
	ON_NOTIFY(NM_DBLCLK, IDC_LI_BR_ALL, OnDblclkAllListCtrl)
	ON_NOTIFY(NM_DBLCLK, IDC_LI_BR_COR, OnDblclkCorListCtrl)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LI_BR_COR, OnItemchangedCorListCtrl)
	ON_NOTIFY(NM_SETFOCUS, IDC_LI_BR_COR, OnNMSetfocusLiUsrCor)
	ON_NOTIFY(NM_SETFOCUS, IDC_LI_BR_ALL, OnNMSetfocusLiUsrAll)
	ON_NOTIFY(NM_CLICK, IDC_LI_BR_COR, OnNMClickLiUsrCor)
	ON_NOTIFY(NM_CLICK, IDC_LI_BR_ALL, OnNMClickLiUsrAll)
	ON_BN_CLICKED(IDC_BT_BR_COR, OnBtBrCor)
	ON_BN_CLICKED(IDC_BT_BR_ERASE, OnBtBrErase)
	ON_BN_CLICKED(IDC_BT_BRCOR_EXIT, OnBtBrcorExit)
END_MESSAGE_MAP()

/**---------------------------------------------------------------------------*/
void PDlgCorBrReg::setAppname	(CBString strAppname)
{
	m_strAppname = strAppname;
}

/**---------------------------------------------------------------------------*/
BOOL PDlgCorBrReg::OnInitDialog()
{
	BeginWaitCursor();

	CDialogMultiLang::OnInitDialog();

	m_ed_Appname.SetWindowText(m_strAppname.c_str());
	m_ed_Regname.SetWindowText(m_pBppBrReg->getRegname().c_str());

	m_pli_BrCOR->giveCon().setAppid(m_pBppBrReg->getAppid());
	m_pli_BrALL->giveCon().setAppid(m_pBppBrReg->getAppid());
	m_pli_BrCOR->giveCon().setRegid(m_pBppBrReg->getRegid());
	m_pli_BrALL->giveCon().setRegid(m_pBppBrReg->getRegid());
	
	
	m_pli_BrCOR->giveCon().setBSelBranch(true);
	m_pli_BrCOR->giveCon().setBSelCor(true);
	m_pli_BrCOR->giveCon().setBSelReg(true);

	m_pli_BrALL->giveCon().setBSelBranch(false);
	m_pli_BrALL->giveCon().setBSelCor(false);
	m_pli_BrALL->giveCon().setBSelReg(false);

	m_pli_BrCOR->init();
	m_pli_BrALL->init();
	
	if (m_pli_BrALL->load() == FALSE) 
		SetAllCtrlState(-1);
	else
		m_bt_Br_Cor.EnableWindow(true);

	if (m_pli_BrCOR->load() == FALSE) 
		SetCorCtrlState(-1);

	m_pli_BrALL->SetFocus();
		
	m_bt_Br_Erase.EnableWindow(false);

	EndWaitCursor();

	return FALSE;  // return TRUE unless you set the focus to a control
}

/**---------------------------------------------------------------------------*/
void PDlgCorBrReg::OnBtBrCor()
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
	
	//zuordnen
	BeginWaitCursor();

	POSITION pos = m_pli_BrALL->GetFirstSelectedItemPosition();
	BppBranchReg * pBrReg;

	C_VecItem * pVItems = new C_VecItem();
	int sCount = 0;
	int iItem = 0;

	while(pos) //Mehrfachauswahl möglich
	{
		sCount++;
		iItem =  m_pli_BrALL->GetNextSelectedItem(pos);
		pVItems->push_back(iItem);
	}
	
	while (sCount > 0)
	{
		iItem = pVItems->back();
		if (iItem >= 0)
		{
		pBrReg = m_pli_BrALL->giveCon().getBsnObj(m_pli_BrALL->getIdx(iItem));
		short sBranchid = (short)pBrReg->getBranchid();
		m_pBppBrReg->setCorBranchid(sBranchid);

		IPerst::ERet eRet = m_pBppBrReg->insertAppBranchReg();
		if (eRet == IPerst::RET_OK) //Fehler bei Insert in DB
		{
			m_pli_BrCOR->giveCon().addBsnObj(*pBrReg);
			m_pli_BrALL->giveCon().clearBsnObj(m_pli_BrALL->getIdx(iItem));
		}
		else if (eRet == IPerst::RET_WARNING)
		{
			m_bLoadNew = true;
		}
		else
		{
			MsgBoxOK(IDS_ERR_INSERT);
			break;
		}
		}
		else
		{
			MsgBoxOK(IDS_SELECT_ITEM);		
		}
		pVItems->pop_back();
		sCount--;
	}

	m_pli_BrALL->SetFocus();
	
	m_pli_BrCOR->load(m_bLoadNew);
	m_pli_BrALL->load(m_bLoadNew );
	
	delete pVItems;

	EndWaitCursor();
}

/**---------------------------------------------------------------------------*/
void PDlgCorBrReg::OnBtBrErase()
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
	
	// Zuordnung entfernen
	BeginWaitCursor();

	BppBranchReg * pBrReg;
	POSITION pos = m_pli_BrCOR->GetFirstSelectedItemPosition();
	

	int iItem = -1;

	iItem =  m_pli_BrCOR->GetNextSelectedItem(pos);
	if (iItem >= 0)
	{
	pBrReg = m_pli_BrCOR->giveCon().getBsnObj(m_pli_BrCOR->getIdx(iItem));
	short sBranchid = (short)pBrReg->getBranchid();
	m_pBppBrReg->setCorBranchid(sBranchid);

	CString csDeleteSure;
	csDeleteSure.Format(CResString::ex().getStrTblText(IDS_DELETE_SURE), 
						pBrReg->getBranchname().c_str());
	INT_PTR iRet = MsgBoxYesNo(csDeleteSure);
		
	if(iRet == IDYES)
	{
		IPerst::ERet eRet = m_pBppBrReg->deleteAppBranchReg();
		if (eRet == IPerst::RET_OK) 
		{
			m_pli_BrALL->giveCon().addBsnObj(*pBrReg);
			m_pli_BrCOR->giveCon().clearBsnObj(m_pli_BrCOR->getIdx(iItem));
		}
		else
		{
			MsgBoxOK(IDS_ERR_DELETE);
		}
	
		m_pli_BrCOR->load(false);
		m_pli_BrALL->load(false);
	}
	
	m_pli_BrCOR->SetFocus();
	}
	else
	{
		MsgBoxOK(IDS_SELECT_ITEM);		
	}
	EndWaitCursor();
}

/**---------------------------------------------------------------------------*/
void PDlgCorBrReg::OnBtBrcorExit()
{
	CDialogMultiLang::OnCancel();
}

/**---------------------------------------------------------------------------*/
void PDlgCorBrReg::OnDblclkCorListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE * pNMIA = (NMITEMACTIVATE *)pNMHDR;

	if (pNMIA->iItem != -1)
	{
		TRACE(__FUNCTION__ ": Item %d\n", pNMIA->iItem);
	
		if (m_bt_Br_Erase.IsWindowEnabled())
			OnBtBrErase();
	}

	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgCorBrReg::OnDblclkAllListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE * pNMIA = (NMITEMACTIVATE *)pNMHDR;

	if (pNMIA->iItem != -1)
	{
		TRACE(__FUNCTION__ ": Item %d\n", pNMIA->iItem);
	
		if (m_bt_Br_Cor.IsWindowEnabled())
			OnBtBrCor();
	}

	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgCorBrReg::OnItemchangedAllListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV   = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	static int   iSelect = -1;

	if (    ((pNMLV->uChanged & LVIF_TEXT) == LVIF_TEXT) 
		 && ( pNMLV->iItem                 == iSelect  ))
	{
		TRACE(__FUNCTION__ ": text changed on Item %d\n", iSelect);
		SetAllCtrlState(iSelect);
	}
		 
	if ((pNMLV->uChanged & LVIF_STATE) == LVIF_STATE) 
	{
		if ((pNMLV->uNewState & LVNI_SELECTED) == LVNI_SELECTED)			// wird selektiert
		{
			iSelect = pNMLV->iItem;
			TRACE(__FUNCTION__ ": state changed (LVNI_SELECTED) on Item %d\n", iSelect);

			SetAllCtrlState(iSelect);
		}
		else
		{
			if ((pNMLV->uOldState & LVNI_SELECTED) == LVNI_SELECTED)		// wird deselektiert
				SetAllCtrlState(-1);
		}
	}

	m_bt_Br_Erase.EnableWindow(false);
	
	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgCorBrReg::OnItemchangedCorListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV   = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	static int   iSelect = -1;

	if (    ((pNMLV->uChanged & LVIF_TEXT) == LVIF_TEXT) 
		 && ( pNMLV->iItem                 == iSelect  ))
	{
		TRACE(__FUNCTION__ ": text changed on Item %d\n", iSelect);
		SetCorCtrlState(iSelect);
	}
		 
	if ((pNMLV->uChanged & LVIF_STATE) == LVIF_STATE) 
	{
		if ((pNMLV->uNewState & LVNI_SELECTED) == LVNI_SELECTED)			// wird selektiert
		{
			iSelect = pNMLV->iItem;
			TRACE(__FUNCTION__ ": state changed (LVNI_SELECTED) on Item %d\n", iSelect);

			SetCorCtrlState(iSelect);
		}
		else
		{
			if ((pNMLV->uOldState & LVNI_SELECTED) == LVNI_SELECTED)		// wird deselektiert
				SetCorCtrlState(-1);
		}
	}

	m_bt_Br_Cor.EnableWindow(false);
	
	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgCorBrReg::SetCorCtrlState(const int iItem)
{
	BOOL    bPrint,
		    bAnzeige;


	if (iItem == -1)					// nichts markiert
	{
		bPrint       = (m_pli_BrCOR->GetItemCount() > 0) ? TRUE : FALSE;		// ListCtrl nicht leer
		bAnzeige     = FALSE;

	}
	else								// Zeile markiert
	{
		bPrint   = TRUE;
		bAnzeige = TRUE;
	}

	// setzen der Controls
	m_bt_Br_Erase.EnableWindow(bAnzeige);
}

/**---------------------------------------------------------------------------*/
void PDlgCorBrReg::SetAllCtrlState(const int iItem)
{
	BOOL    bPrint,
		    bAnzeige;


	if (iItem == -1)					// nichts markiert
	{
		bPrint       = (m_pli_BrALL->GetItemCount() > 0) ? TRUE : FALSE;		// ListCtrl nicht leer
		bAnzeige     = FALSE;

	}
	else								// Zeile markiert
	{
		bPrint   = TRUE;
		bAnzeige = TRUE;
	}

	// setzen der Controls
	m_bt_Br_Cor.EnableWindow(bAnzeige);
}

/**---------------------------------------------------------------------------*/
BOOL PDlgCorBrReg::PreTranslateMessage(MSG* pMsg)
{
	assert(pMsg != 0);

	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			if (GetFocus() == GetDlgItem(IDC_BT_BRCOR_EXIT))
			{
				if (m_bt_BrCor_Exit.IsWindowEnabled())
				{
					OnBtBrcorExit();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_BT_BR_COR))
			{
				if (m_bt_Br_Cor.IsWindowEnabled())
				{
					OnBtBrCor();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_BT_BR_ERASE))
			{
				if (m_bt_Br_Erase.IsWindowEnabled())
				{
					OnBtBrErase();
					return TRUE;
				}
			}
			return FALSE;

		case VK_ESCAPE:
			if (m_bt_BrCor_Exit.IsWindowEnabled())
			{
				OnBtBrcorExit();
				return TRUE;
			}
			return FALSE;


		default:
			break;
		}
	} 

	return CDialogMultiLang::PreTranslateMessage(pMsg);
}

/**---------------------------------------------------------------------------*/
void PDlgCorBrReg::OnNMSetfocusLiUsrCor(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE * pNMIA = (NMITEMACTIVATE *)pNMHDR;

	if (pNMIA->iItem == -1)
	{
		m_bt_Br_Erase.EnableWindow(false);
	}
	else
	{
		m_bt_Br_Erase.EnableWindow(true);
	}
	m_bt_Br_Cor.EnableWindow(false);
	
	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgCorBrReg::OnNMSetfocusLiUsrAll(NMHDR *pNMHDR, LRESULT *pResult)
{
	
	NMITEMACTIVATE * pNMIA = (NMITEMACTIVATE *)pNMHDR;

	if (pNMIA->iItem == -1)
	{
		m_bt_Br_Cor.EnableWindow(false);
	}
	else
	{
		m_bt_Br_Cor.EnableWindow(true);
	}
	m_bt_Br_Erase.EnableWindow(false);

	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgCorBrReg::OnNMClickLiUsrCor(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE * pNMIA = (NMITEMACTIVATE *)pNMHDR;

	if (pNMIA->iItem == -1)
	{
		m_bt_Br_Erase.EnableWindow(false);
	}
	else
	{
		m_bt_Br_Erase.EnableWindow(true);
	}
	m_bt_Br_Cor.EnableWindow(false);
	
	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgCorBrReg::OnNMClickLiUsrAll(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE * pNMIA = (NMITEMACTIVATE *)pNMHDR;

	if (pNMIA->iItem == -1)
	{
		m_bt_Br_Cor.EnableWindow(false);
	}
	else
	{
		m_bt_Br_Cor.EnableWindow(true);
	}
	m_bt_Br_Erase.EnableWindow(false);
	
	*pResult = 0;
}


