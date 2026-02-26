/******************************************************************************/
/**                                                                           */
/**   Author : T.ARNOLD NTTDATA                                               */
/**   Date   : 25.11.2025                                                     */
/**                                                                           */
/******************************************************************************/

/*----------------------------------------------------------------------------*/
// Includes
/*----------------------------------------------------------------------------*/
#include "StdAfx.h"
#include <BppUserPara.h>
#include <BConppUserPara.h>
#include "PDlgUserParaListCtrl.h"
#include "PDlgUserParaDetail.h"
#include "PDlgAdmRights.h"
#include "presentation.h"
#include "PDlgDllUserPara.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(PDlgDllUserPara, CDialogMultiLang)

/**---------------------------------------------------------------------------*/
PDlgDllUserPara::PDlgDllUserPara()
	: CDialogMultiLang(PDlgDllUserPara::IDD)
{
	   m_pli_UserPara = new PDlgUserParaListCtrl;
	   assert(m_pli_UserPara);
}

/**---------------------------------------------------------------------------*/
PDlgDllUserPara::~PDlgDllUserPara()
{
	if (m_pli_UserPara != NULL)
	{
		delete m_pli_UserPara;
		m_pli_UserPara = NULL;
	}
}

/**---------------------------------------------------------------------------*/
void PDlgDllUserPara::DoDataExchange(CDataExchange* pDX)
{
	CDialogMultiLang::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LI_DLL_USERPARA  , *m_pli_UserPara);
	DDX_Control(pDX, IDC_BT_USERPARA_NEW, m_bt_UserPara_New);
	DDX_Control(pDX, IDC_BT_USERPARA_WORK, m_bt_UserPara_Work);
	DDX_Control(pDX, IDC_BT_USERPARA_DEL, m_bt_UserPara_Del);
	DDX_Control(pDX, IDC_BT_USERPARA_DLL_EXIT, m_bt_UserPara_Exit);
}

/**---------------------------------------------------------------------------*/
BOOL PDlgDllUserPara::OnInitDialog()
{
	
	BeginWaitCursor();

	CDialogMultiLang::OnInitDialog();

	// ### AppListCtrl
	m_pli_UserPara->init();
	m_pli_UserPara->giveCon().setBAll(true);
	if (m_pli_UserPara->load() == FALSE) 
		SetCtrlState(-1);
	
	m_pli_UserPara->SetFocus();
	
	EndWaitCursor();

	return FALSE;  // return TRUE unless you set the focus to a control
}

/**---------------------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(PDlgDllUserPara, CDialogMultiLang)
	ON_NOTIFY(NM_DBLCLK, IDC_LI_DLL_USERPARA, OnDblclkListCtrl)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LI_DLL_USERPARA, OnItemchangedListCtrl)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LI_DLL_USERPARA, OnClickListCtrl)
	ON_BN_CLICKED(IDC_BT_USERPARA_NEW, OnBtUserParaNew)
	ON_BN_CLICKED(IDC_BT_USERPARA_WORK, OnBtUserParaWork)
	ON_BN_CLICKED(IDC_BT_USERPARA_DEL, OnBtUserParaDel)
	ON_BN_CLICKED(IDC_BT_USERPARA_DLL_EXIT, OnBtUserParaExit)
END_MESSAGE_MAP()

/**---------------------------------------------------------------------------*/
void PDlgDllUserPara::OnBtUserParaNew()
{
	BeginWaitCursor();

	BppUserPara * pUserPara = new BppUserPara;
		assert(pUserPara);
	
	PDlgUserParaDetail dlgUserParaDetail(pUserPara);
	dlgUserParaDetail.setBNewUserPara(true);
	
	EndWaitCursor();

	INT_PTR iRet = dlgUserParaDetail.DoModal();

	//clone zurückkopieren!
	if (dlgUserParaDetail.getBDataChanged())
	{
		dlgUserParaDetail.getCloneObj(pUserPara);
		if (iRet == IDOK)
		{
			m_pli_UserPara->giveCon().addBsnObj(*pUserPara);
			
			if (m_pli_UserPara->load(false) == FALSE) 
				SetCtrlState(-1);
		}
	}

	if(pUserPara != NULL)
	{
		delete pUserPara;
		pUserPara = NULL;
	}
	
	BeginWaitCursor();
	
	m_pli_UserPara->SetFocus();
	
	EndWaitCursor();
}

/**---------------------------------------------------------------------------*/
void PDlgDllUserPara::OnBtUserParaWork()
{
	BeginWaitCursor();

	int       iItem   = -1;
	BppUserPara * pUserPara = m_pli_UserPara->getNextSelectedBsnObj(&iItem);

	if (iItem >= 0)
	{
	PDlgUserParaDetail dlgUserParaDetail(pUserPara);
	dlgUserParaDetail.setBNewUserPara(false);
	
	EndWaitCursor();

	INT_PTR iRet = dlgUserParaDetail.DoModal();
	
	//clone zurückkopieren!
	if (dlgUserParaDetail.getBDataChanged())
	{
		dlgUserParaDetail.getCloneObj(m_pli_UserPara->giveCon().getBsnObj(m_pli_UserPara->getIdx(iItem)));
		if (iRet == IDOK)
			m_pli_UserPara->refresh(iItem);
	}

	BeginWaitCursor();
	
	m_pli_UserPara->SetFocus();
	}
	else
	{
		MsgBoxOK(IDS_SELECT_ITEM);		
	}
	EndWaitCursor();
}

/**---------------------------------------------------------------------------*/
void PDlgDllUserPara::OnBtUserParaDel()
{
	CBString strRight = RIGHT_DEL_DLL;
	if (!(ppBLoginData::ex().isLegitimated(MODULE_DLL,strRight)))
	{
		MsgBoxOK(IDS_NO_RIGHT);
		return;
	}
	
	BeginWaitCursor();

	int       iItem   = -1;
	BppUserPara * pUserPara = m_pli_UserPara->getNextSelectedBsnObj(&iItem);

	if (iItem >= 0)
	{
	//Sicherheitsabfrage
	CString csDeleteSure;
	csDeleteSure.Format(CResString::ex().getStrTblText(IDS_DELETE_SURE_ALL_APP), 
						pUserPara->getName().c_str());
	INT_PTR iRet = MsgBoxYesNo(csDeleteSure);
		
	if(iRet == IDYES)
	{
		IPerst::ERet eRet = pUserPara->deleteItem();
		if (eRet == IPerst::RET_OK) //Fehler bei Delete auf DB
		{
			m_pli_UserPara->giveCon().clearBsnObj(m_pli_UserPara->getIdx(iItem));
			m_pli_UserPara->load(false);
		}
		else
		{
			MsgBoxOK(IDS_ERR_DELETE); 
		}
	}
	
	m_pli_UserPara->SetFocus();
	}
	else
	{
		MsgBoxOK(IDS_SELECT_ITEM);		
	}
	EndWaitCursor();
}

/**---------------------------------------------------------------------------*/
void PDlgDllUserPara::OnBtUserParaExit()
{
	CDialogMultiLang::OnCancel();
}


/**---------------------------------------------------------------------------*/
void PDlgDllUserPara::OnDblclkListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE * pNMIA = (NMITEMACTIVATE *)pNMHDR;

	if (pNMIA->iItem != -1)
	{
		TRACE(__FUNCTION__ ": Item %d\n", pNMIA->iItem);
	
		if (m_bt_UserPara_Work.IsWindowEnabled())
			OnBtUserParaWork();
	}

	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgDllUserPara::OnClickListCtrl(NMHDR* pNMHDR, LRESULT* pResult)
{
	// if headerline clicked, use for sorting
	NMITEMACTIVATE* pNMIA = (NMITEMACTIVATE*)pNMHDR;
	if (pNMIA->iItem == -1)
	{
		int nCol = pNMIA->iSubItem;
		if ((nCol >= 0) && (nCol <= 3))
		{
			int nSortCol = m_pli_UserPara->giveCon().getSortIndex();
			if (nSortCol == nCol)
			{
				m_pli_UserPara->giveCon().toggleSortOrder();
			}
			else
			{
				m_pli_UserPara->giveCon().setSortIndex(nCol);
			}
			m_pli_UserPara->load();
		}
	}
}

/**---------------------------------------------------------------------------*/
void PDlgDllUserPara::OnItemchangedListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV   = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	static int   iSelect = -1;

	if (    ((pNMLV->uChanged & LVIF_TEXT) == LVIF_TEXT) 
		 && ( pNMLV->iItem                 == iSelect  ))
	{
		TRACE(__FUNCTION__ ": text changed on Item %d\n", iSelect);
		SetCtrlState(iSelect);
	}
		 
	if ((pNMLV->uChanged & LVIF_STATE) == LVIF_STATE) 
	{
		if ((pNMLV->uNewState & LVNI_SELECTED) == LVNI_SELECTED)			// wird selektiert
		{
			iSelect = pNMLV->iItem;
			TRACE(__FUNCTION__ ": state changed (LVNI_SELECTED) on Item %d\n", iSelect);

			SetCtrlState(iSelect);
		}
		else
		{
			if ((pNMLV->uOldState & LVNI_SELECTED) == LVNI_SELECTED)		// wird deselektiert
				SetCtrlState(-1);
		}
	}
	
	*pResult = 0;
}


/**---------------------------------------------------------------------------*/
void PDlgDllUserPara::SetCtrlState(const int iItem)
{
	BOOL    bPrint,
		    bAnzeige;


	if (iItem == -1)					// nichts markiert
	{
		bPrint       = (m_pli_UserPara->GetItemCount() > 0) ? TRUE : FALSE;		// ListCtrl nicht leer
		bAnzeige     = FALSE;

	}
	else								// Zeile markiert
	{
		bPrint   = TRUE;
		bAnzeige = TRUE;
	}

	// setzen der Controls
	m_bt_UserPara_Work.EnableWindow(bAnzeige);
	m_bt_UserPara_Del.EnableWindow(bAnzeige);
}


/**---------------------------------------------------------------------------*/
BOOL PDlgDllUserPara::PreTranslateMessage(MSG* pMsg)
{
	assert(pMsg != 0);

	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			// Fokus steht in UserParaListCtrl
			if (GetFocus() == GetDlgItem(IDC_LI_DLL_USERPARA))
			{
				if (m_bt_UserPara_Work.IsWindowEnabled())
				{
					OnBtUserParaWork();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_BT_USERPARA_DLL_EXIT))
			{
				if (m_bt_UserPara_Exit.IsWindowEnabled())
				{
					OnBtUserParaExit();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_BT_USERPARA_NEW))
			{
				if (m_bt_UserPara_New.IsWindowEnabled())
				{
					OnBtUserParaNew();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_BT_USERPARA_WORK))
			{
				if (m_bt_UserPara_Work.IsWindowEnabled())
				{
					OnBtUserParaWork();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_BT_USERPARA_DEL))
			{
				if (m_bt_UserPara_Del.IsWindowEnabled())
				{
					OnBtUserParaDel();
					return TRUE;
				}
			}
			return FALSE;

		case VK_ESCAPE:
			if (m_bt_UserPara_Exit.IsWindowEnabled())
			{
				OnBtUserParaExit();
				return TRUE;
			}
			return FALSE;


		default:
			break;
		}
	} 

	return CDialogMultiLang::PreTranslateMessage(pMsg);
}


