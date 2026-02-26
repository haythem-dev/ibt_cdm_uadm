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
#include <BppUsertype.h>
#include <BConppUsertype.h>
#include "PDlgUsertypeListCtrl.h"
#include "PDlgUsertypeDetail.h"
#include "PDlgAdmRights.h"
#include "presentation.h"
#include "PDlgDllUsertype.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(PDlgDllUsertype, CDialogMultiLang)

/**---------------------------------------------------------------------------*/
PDlgDllUsertype::PDlgDllUsertype()
	: CDialogMultiLang(PDlgDllUsertype::IDD)
{
	   m_pli_Usertype = new PDlgUsertypeListCtrl;
	   assert(m_pli_Usertype);
}

/**---------------------------------------------------------------------------*/
PDlgDllUsertype::~PDlgDllUsertype()
{
	if (m_pli_Usertype != NULL)
	{
		delete m_pli_Usertype;
		m_pli_Usertype = NULL;
	}
}

/**---------------------------------------------------------------------------*/
void PDlgDllUsertype::DoDataExchange(CDataExchange* pDX)
{
	CDialogMultiLang::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LI_DLL_USERTYPE  , *m_pli_Usertype);
	DDX_Control(pDX, IDC_BT_USERTYPE_NEW, m_bt_Usertype_New);
	DDX_Control(pDX, IDC_BT_USERTYPE_WORK, m_bt_Usertype_Work);
	DDX_Control(pDX, IDC_BT_USERTYPE_DEL, m_bt_Usertype_Del);
	DDX_Control(pDX, IDC_BT_USERTYPE_DLL_EXIT, m_bt_Usertype_Exit);
}

/**---------------------------------------------------------------------------*/
BOOL PDlgDllUsertype::OnInitDialog()
{
	
	BeginWaitCursor();

	CDialogMultiLang::OnInitDialog();

	// ### AppListCtrl
	m_pli_Usertype->init();
	m_pli_Usertype->giveCon().setBAll(true);
	if (m_pli_Usertype->load() == FALSE) 
		SetCtrlState(-1);
	
	m_pli_Usertype->SetFocus();
	
	EndWaitCursor();

	return FALSE;  // return TRUE unless you set the focus to a control
}

/**---------------------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(PDlgDllUsertype, CDialogMultiLang)
	ON_NOTIFY(NM_DBLCLK, IDC_LI_DLL_USERTYPE, OnDblclkListCtrl)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LI_DLL_USERTYPE, OnItemchangedListCtrl)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LI_DLL_USERTYPE, OnClickListCtrl)
	ON_BN_CLICKED(IDC_BT_USERTYPE_NEW, OnBtUsertypeNew)
	ON_BN_CLICKED(IDC_BT_USERTYPE_WORK, OnBtUsertypeWork)
	ON_BN_CLICKED(IDC_BT_USERTYPE_DEL, OnBtUsertypeDel)
	ON_BN_CLICKED(IDC_BT_USERTYPE_DLL_EXIT, OnBtUsertypeExit)
END_MESSAGE_MAP()

/**---------------------------------------------------------------------------*/
void PDlgDllUsertype::OnBtUsertypeNew()
{
	BeginWaitCursor();

	BppUsertype * pUsertype = new BppUsertype;
		assert(pUsertype);
	
	PDlgUsertypeDetail dlgUsertypeDetail(pUsertype);
	dlgUsertypeDetail.setBNewUsertype(true);
	
	EndWaitCursor();

	INT_PTR iRet = dlgUsertypeDetail.DoModal();

	//clone zurückkopieren!
	if (dlgUsertypeDetail.getBDataChanged())
	{
		dlgUsertypeDetail.getCloneObj(pUsertype);
		if (iRet == IDOK)
		{
			m_pli_Usertype->giveCon().addBsnObj(*pUsertype);
			
			if (m_pli_Usertype->load(false) == FALSE) 
				SetCtrlState(-1);
		}
	}

	if(pUsertype != NULL)
	{
		delete pUsertype;
		pUsertype = NULL;
	}
	
	BeginWaitCursor();
	
	m_pli_Usertype->SetFocus();
	
	EndWaitCursor();
}

/**---------------------------------------------------------------------------*/
void PDlgDllUsertype::OnBtUsertypeWork()
{
	BeginWaitCursor();

	int       iItem   = -1;
	BppUsertype * pUsertype = m_pli_Usertype->getNextSelectedBsnObj(&iItem);

	if (iItem >= 0)
	{
	PDlgUsertypeDetail dlgUsertypeDetail(pUsertype);
	dlgUsertypeDetail.setBNewUsertype(false);
	
	EndWaitCursor();

	INT_PTR iRet = dlgUsertypeDetail.DoModal();
	
	//clone zurückkopieren!
	if (dlgUsertypeDetail.getBDataChanged())
	{
		dlgUsertypeDetail.getCloneObj(m_pli_Usertype->giveCon().getBsnObj(m_pli_Usertype->getIdx(iItem)));
		if (iRet == IDOK)
			m_pli_Usertype->refresh(iItem);
	}

	BeginWaitCursor();
	
	m_pli_Usertype->SetFocus();
	}
	else
	{
		MsgBoxOK(IDS_SELECT_ITEM);		
	}
	EndWaitCursor();
}

/**---------------------------------------------------------------------------*/
void PDlgDllUsertype::OnBtUsertypeDel()
{
	CBString strRight = RIGHT_DEL_DLL;
	if (!(ppBLoginData::ex().isLegitimated(MODULE_DLL,strRight)))
	{
		MsgBoxOK(IDS_NO_RIGHT);
		return;
	}
	
	BeginWaitCursor();

	int       iItem   = -1;
	BppUsertype * pUsertype = m_pli_Usertype->getNextSelectedBsnObj(&iItem);

	if (iItem >= 0)
	{
	//Sicherheitsabfrage
	CString csDeleteSure;
	csDeleteSure.Format(CResString::ex().getStrTblText(IDS_DELETE_SURE_ALL_APP), 
						pUsertype->getName().c_str());
	INT_PTR iRet = MsgBoxYesNo(csDeleteSure);
		
	if(iRet == IDYES)
	{
		IPerst::ERet eRet = pUsertype->deleteItem();
		if (eRet == IPerst::RET_OK) //Fehler bei Delete auf DB
		{
			m_pli_Usertype->giveCon().clearBsnObj(m_pli_Usertype->getIdx(iItem));
			m_pli_Usertype->load(false);
		}
		else
		{
			MsgBoxOK(IDS_ERR_DELETE); 
		}
	}
	
	m_pli_Usertype->SetFocus();
	}
	else
	{
		MsgBoxOK(IDS_SELECT_ITEM);		
	}
	EndWaitCursor();
}

/**---------------------------------------------------------------------------*/
void PDlgDllUsertype::OnBtUsertypeExit()
{
	CDialogMultiLang::OnCancel();
}


/**---------------------------------------------------------------------------*/
void PDlgDllUsertype::OnDblclkListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE * pNMIA = (NMITEMACTIVATE *)pNMHDR;

	if (pNMIA->iItem != -1)
	{
		TRACE(__FUNCTION__ ": Item %d\n", pNMIA->iItem);
	
		if (m_bt_Usertype_Work.IsWindowEnabled())
			OnBtUsertypeWork();
	}

	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgDllUsertype::OnClickListCtrl(NMHDR* pNMHDR, LRESULT* pResult)
{
	// if headerline clicked, use for sorting
	NMITEMACTIVATE* pNMIA = (NMITEMACTIVATE*)pNMHDR;
	if (pNMIA->iItem == -1)
	{
		int nCol = pNMIA->iSubItem;
		if ((nCol >= 0) && (nCol <= 3))
		{
			int nSortCol = m_pli_Usertype->giveCon().getSortIndex();
			if (nSortCol == nCol)
			{
				m_pli_Usertype->giveCon().toggleSortOrder();
			}
			else
			{
				m_pli_Usertype->giveCon().setSortIndex(nCol);
			}
			m_pli_Usertype->load();
		}
	}
}

/**---------------------------------------------------------------------------*/
void PDlgDllUsertype::OnItemchangedListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
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
void PDlgDllUsertype::SetCtrlState(const int iItem)
{
	BOOL    bPrint,
		    bAnzeige;


	if (iItem == -1)					// nichts markiert
	{
		bPrint       = (m_pli_Usertype->GetItemCount() > 0) ? TRUE : FALSE;		// ListCtrl nicht leer
		bAnzeige     = FALSE;

	}
	else								// Zeile markiert
	{
		bPrint   = TRUE;
		bAnzeige = TRUE;
	}

	// setzen der Controls
	m_bt_Usertype_Work.EnableWindow(bAnzeige);
	m_bt_Usertype_Del.EnableWindow(bAnzeige);
}


/**---------------------------------------------------------------------------*/
BOOL PDlgDllUsertype::PreTranslateMessage(MSG* pMsg)
{
	assert(pMsg != 0);

	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			// Fokus steht in UsertypeListCtrl
			if (GetFocus() == GetDlgItem(IDC_LI_DLL_USERTYPE))
			{
				if (m_bt_Usertype_Work.IsWindowEnabled())
				{
					OnBtUsertypeWork();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_BT_USERTYPE_DLL_EXIT))
			{
				if (m_bt_Usertype_Exit.IsWindowEnabled())
				{
					OnBtUsertypeExit();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_BT_USERTYPE_NEW))
			{
				if (m_bt_Usertype_New.IsWindowEnabled())
				{
					OnBtUsertypeNew();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_BT_USERTYPE_WORK))
			{
				if (m_bt_Usertype_Work.IsWindowEnabled())
				{
					OnBtUsertypeWork();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_BT_USERTYPE_DEL))
			{
				if (m_bt_Usertype_Del.IsWindowEnabled())
				{
					OnBtUsertypeDel();
					return TRUE;
				}
			}
			return FALSE;

		case VK_ESCAPE:
			if (m_bt_Usertype_Exit.IsWindowEnabled())
			{
				OnBtUsertypeExit();
				return TRUE;
			}
			return FALSE;


		default:
			break;
		}
	} 

	return CDialogMultiLang::PreTranslateMessage(pMsg);
}


