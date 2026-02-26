/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                     */
/**   Date   : 16.08.2004                                                     */
/**                                                                           */
/******************************************************************************/

/*----------------------------------------------------------------------------*/
// Includes
/*----------------------------------------------------------------------------*/
#include "StdAfx.h"
#include <BppPrev.h>
#include <BConppPrev.h>
#include <BppApp.h>
#include <BConppApp.h>
#include "PDlgPrevListCtrl.h"
#include "PDlgAppListCtrl.h"
//#include "presentation.h"
#include "PDlgDllPrev.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*----------------------------------------------------------------------------*/
// PViewUsrLst
/**---------------------------------------------------------------------------*/
IMPLEMENT_DYNCREATE(PDlgDllPrev, CDialogMultiLang)

/**---------------------------------------------------------------------------*/
PDlgDllPrev::PDlgDllPrev()
	: CDialogMultiLang(PDlgDllPrev::IDD)
{
	   m_pli_Prev = new PDlgPrevListCtrl;
	   assert(m_pli_Prev);

	   m_pli_App = new PDlgAppListCtrl;
	   assert(m_pli_App);
}

/**---------------------------------------------------------------------------*/
PDlgDllPrev::~PDlgDllPrev()
{
	if (m_pli_Prev != NULL)
	{
		delete m_pli_Prev;
		m_pli_Prev = NULL;
	}

	if (m_pli_App != NULL)
	{
		delete m_pli_App;
		m_pli_App = NULL;
	}
}

/**---------------------------------------------------------------------------*/
void PDlgDllPrev::DoDataExchange(CDataExchange* pDX)
{
	CDialogMultiLang::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LI_PREV  , *m_pli_Prev);
	DDX_Control(pDX, IDC_LI_APP_PREV, *m_pli_App);
	DDX_Control(pDX, IDC_BT_PREV_APP, m_bt_App_Prev_Sel);
	DDX_Control(pDX, IDC_BT_PREV_EXIT, m_bt_Prev_Exit);
	DDX_Control(pDX, IDC_BT_PREV_DEL, m_bt_Prev_Del);
}

/**---------------------------------------------------------------------------*/
BOOL PDlgDllPrev::OnInitDialog()
{
	
	BeginWaitCursor();

	CDialogMultiLang::OnInitDialog();

	// ### AppListCtrl
	m_pli_App->init();
	if (m_pli_App->load() == FALSE) 
		SetAppCtrlState(-1);
	
	m_pli_Prev->init();

	m_pli_App->SetFocus();
	
	//Controls
	m_bt_Prev_Del.EnableWindow(false);
	
	EndWaitCursor();

	return FALSE;  // return TRUE unless you set the focus to a control
}

/**---------------------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(PDlgDllPrev, CDialogMultiLang)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LI_APP_PREV, OnItemchangedAppListCtrl)
	ON_NOTIFY(NM_DBLCLK, IDC_LI_APP_PREV, OnDblclkAppListCtrl)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LI_PREV, OnItemchangedListCtrl)
	ON_BN_CLICKED(IDC_BT_PREV_APP, OnBtPrevApp)
	ON_BN_CLICKED(IDC_BT_PREV_EXIT, OnBtPrevExit)
	ON_BN_CLICKED(IDC_BT_PREV_DEL, OnBtPrevDel)
END_MESSAGE_MAP()



/**---------------------------------------------------------------------------*/
void PDlgDllPrev::OnBtPrevApp()
{
	BeginWaitCursor();

	int   iItem  = -1;
	m_sAppid = (short) m_pli_App->getNextSelectedBsnObj(&iItem)->getAppid();
	if (iItem >= 0)
	{
	m_pli_Prev->giveCon().setAppid(m_sAppid);
	if (m_pli_Prev->load() == FALSE) 
		SetPrevCtrlState(-1); 

	m_pli_Prev->EnableWindow(true);
	m_pli_Prev->SetFocus();

	SetAppCtrlState(-1);
	}
	else
	{
		MsgBoxOK(IDS_SELECT_ITEM);		
	}
	EndWaitCursor();
}

/**---------------------------------------------------------------------------*/
void PDlgDllPrev::OnBtPrevExit()
{
	CDialogMultiLang::OnCancel();	
}

/**---------------------------------------------------------------------------*/
void PDlgDllPrev::OnBtPrevDel()
{
	CBString strRight = RIGHT_WORK_DLL;
	if (!(ppBLoginData::ex().isLegitimated(MODULE_DLL,strRight)))
	{
		int       iItemTemp   = -1;
		m_pli_App->getNextSelectedBsnObj(&iItemTemp);
		strRight = m_pli_App->giveCon().getBsnObj(m_pli_App->getIdx(iItemTemp))->getAppname();
		strRight.upper();

		if (!(ppBLoginData::ex().isLegitimated(MODULE_DLL,strRight)))
		{
			MsgBoxOK(IDS_NO_RIGHT);
			return;
		}
	}
	
	BeginWaitCursor();

	int       iItem   = -1;
	BppPrev * pPrev = m_pli_Prev->getNextSelectedBsnObj(&iItem);

	if (iItem >= 0)
	{
	//Sicherheitsabfrage
	CString csDeleteSure;
	csDeleteSure.Format(CResString::ex().getStrTblText(IDS_DELETE_SURE), 
					pPrev->getGlobUser().c_str());
	INT_PTR iRet = MsgBoxYesNo(csDeleteSure);
		
	if(iRet == IDYES)
	{
		pPrev->setAppid(m_sAppid);
		IPerst::ERet eRet = pPrev->deletePrev();
		if (eRet == IPerst::RET_OK) //Fehler bei Delete auf DB
		{
			m_pli_Prev->giveCon().clearBsnObj(m_pli_Prev->getIdx(iItem));
			m_pli_Prev->load(false);
		}
		else
		{
			MsgBoxOK(IDS_ERR_DELETE); 
		}
	}
	
	m_pli_Prev->SetFocus();
	}
	else
	{
		MsgBoxOK(IDS_SELECT_ITEM);		
	}
	EndWaitCursor();
}

/**---------------------------------------------------------------------------*/
void PDlgDllPrev::OnDblclkAppListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE * pNMIA = (NMITEMACTIVATE *)pNMHDR;

	if (pNMIA->iItem != -1)
	{
		TRACE(__FUNCTION__ ": Item %d\n", pNMIA->iItem);
	
		if (m_bt_App_Prev_Sel.IsWindowEnabled())
			OnBtPrevApp();
	}

	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgDllPrev::OnItemchangedAppListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV   = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	static int   iSelect = -1;

	if (    ((pNMLV->uChanged & LVIF_TEXT) == LVIF_TEXT) 
		 && ( pNMLV->iItem                 == iSelect  ))
	{
		TRACE(__FUNCTION__ ": text changed on Item %d\n", iSelect);
		SetAppCtrlState(iSelect);
	}
		 
	if ((pNMLV->uChanged & LVIF_STATE) == LVIF_STATE) 
	{
		if ((pNMLV->uNewState & LVNI_SELECTED) == LVNI_SELECTED)			// wird selektiert
		{
			iSelect = pNMLV->iItem;
			TRACE(__FUNCTION__ ": state changed (LVNI_SELECTED) on Item %d\n", iSelect);

			SetAppCtrlState(iSelect);
		}
		else
		{
			if ((pNMLV->uOldState & LVNI_SELECTED) == LVNI_SELECTED)		// wird deselektiert
				SetAppCtrlState(-1);
		}
	}

	SetPrevCtrlState (-1);
	m_pli_Prev->EnableWindow(false);
	m_pli_Prev->clearAllBsnObj();

	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgDllPrev::OnItemchangedListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV   = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	static int   iSelect = -1;

	if (    ((pNMLV->uChanged & LVIF_TEXT) == LVIF_TEXT) 
		 && ( pNMLV->iItem                 == iSelect  ))
	{
		TRACE(__FUNCTION__ ": text changed on Item %d\n", iSelect);
		SetPrevCtrlState(iSelect);
	}
		 
	if ((pNMLV->uChanged & LVIF_STATE) == LVIF_STATE) 
	{
		if ((pNMLV->uNewState & LVNI_SELECTED) == LVNI_SELECTED)			// wird selektiert
		{
			iSelect = pNMLV->iItem;
			TRACE(__FUNCTION__ ": state changed (LVNI_SELECTED) on Item %d\n", iSelect);

			SetPrevCtrlState(iSelect);
		}
		else
		{
			if ((pNMLV->uOldState & LVNI_SELECTED) == LVNI_SELECTED)		// wird deselektiert
				SetPrevCtrlState(-1);
		}
	}
	
	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgDllPrev::SetPrevCtrlState(const int iItem)
{
	BOOL    bPrint,
		    bAnzeige;


	if (iItem == -1)					// nichts markiert
	{
		bPrint       = (m_pli_Prev->GetItemCount() > 0) ? TRUE : FALSE;		// ListCtrl nicht leer
		bAnzeige     = FALSE;

	}
	else								// Zeile markiert
	{
		bPrint   = TRUE;
		bAnzeige = TRUE;
	}

	// setzen der Controls
	m_bt_Prev_Del.EnableWindow(bAnzeige);
}
/**---------------------------------------------------------------------------*/
void PDlgDllPrev::SetAppCtrlState(const int iItem)
{
	BOOL    bPrint,
		    bAnzeige;


	if (iItem == -1)					// nichts markiert
	{
		bPrint       = (m_pli_App->GetItemCount() > 0) ? TRUE : FALSE;		// ListCtrl nicht leer
		bAnzeige     = FALSE;

	}
	else								// Zeile markiert
	{
		bPrint   = TRUE;
		bAnzeige = TRUE;
	}

	// setzen der Controls
	m_bt_App_Prev_Sel.EnableWindow(bAnzeige);
}



/**---------------------------------------------------------------------------*/
BOOL PDlgDllPrev::PreTranslateMessage(MSG* pMsg)
{
	assert(pMsg != 0);

	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:
				// Fokus steht in AppListCtrl
			if (GetFocus() == GetDlgItem(IDC_LI_APP_PREV))
			{
				if (m_bt_App_Prev_Sel.IsWindowEnabled())
				{
					OnBtPrevApp();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_BT_PREV_EXIT))
			{
				if (m_bt_Prev_Exit.IsWindowEnabled())
				{
					OnBtPrevExit();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_BT_PREV_DEL))
			{
				if (m_bt_Prev_Del.IsWindowEnabled())
				{
					OnBtPrevDel();
					return TRUE;
				}
			}
			
			return FALSE;

		case VK_ESCAPE:
			if (m_bt_Prev_Exit.IsWindowEnabled())
			{
				OnBtPrevExit();
				return TRUE;
			}
			return FALSE;


		default:
			break;
		}
	} 

	return CDialogMultiLang::PreTranslateMessage(pMsg);
}


