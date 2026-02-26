/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                     */
/**   Date   : 09.06.2004                                                     */
/**                                                                           */
/******************************************************************************/

/*----------------------------------------------------------------------------*/
// Includes
/*----------------------------------------------------------------------------*/
#include "StdAfx.h"
#include <BppGroup.h>
#include <BConppGroup.h>
#include <BppApp.h>
#include <BConppApp.h>
#include "PDlgCorGrpUsr.h"
#include "PDlgGroupListCtrl.h"
#include "PDlgAppListCtrl.h"
#include "presentation.h"
#include "PDlgDllGroup.h"
#include "PDlgGroupList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*----------------------------------------------------------------------------*/
// PViewUsrLst
/**---------------------------------------------------------------------------*/
IMPLEMENT_DYNCREATE(PDlgGroupList, CDialogMultiLang)

/**---------------------------------------------------------------------------*/
PDlgGroupList::PDlgGroupList()
	: CDialogMultiLang(PDlgGroupList::IDD)
{
	   m_pli_Group = new PDlgGroupListCtrl;
	   assert(m_pli_Group);

	   m_pli_App = new PDlgAppListCtrl;
	   assert(m_pli_App);
}

/**---------------------------------------------------------------------------*/
PDlgGroupList::~PDlgGroupList()
{
	if (m_pli_Group != NULL)
	{
		delete m_pli_Group;
		m_pli_Group = NULL;
	}

	if (m_pli_App != NULL)
	{
		delete m_pli_App;
		m_pli_App = NULL;
	}
}

/**---------------------------------------------------------------------------*/
void PDlgGroupList::DoDataExchange(CDataExchange* pDX)
{
	CDialogMultiLang::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LI_GROUP  , *m_pli_Group);
	DDX_Control(pDX, IDC_LI_APP_GRP, *m_pli_App);
	DDX_Control(pDX, IDC_BT_SEL_GROUP, m_bt_SelGroup);
	DDX_Control(pDX, IDC_BT_SEL_APP, m_bt_SelApp);
	DDX_Control(pDX, IDC_BT_GRP_EXIT, m_bt_GrpExit);
	DDX_Control(pDX, IDC_BT_GRP_ALL_APP, m_bt_Grp_All_App);
	DDX_Control(pDX, IDC_BT_NEW_GROUP, m_bt_New_Grp);
}

/**---------------------------------------------------------------------------*/
BOOL PDlgGroupList::OnInitDialog()
{
	
	BeginWaitCursor();

	CDialogMultiLang::OnInitDialog();

	// ### AppListCtrl
	m_pli_App->init();
	if (m_pli_App->load() == FALSE) 
		SetAppCtrlState(-1);
	
	m_pli_Group->init();

	m_pli_App->SetFocus();
	
	//Controls
	m_bt_SelGroup.EnableWindow(false);
	
	EndWaitCursor();

	return FALSE;  // return TRUE unless you set the focus to a control
}

/**---------------------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(PDlgGroupList, CDialogMultiLang)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LI_APP_GRP, OnItemchangedAppListCtrl)
	ON_NOTIFY(NM_DBLCLK, IDC_LI_APP_GRP, OnDblclkAppListCtrl)
	ON_NOTIFY(NM_DBLCLK, IDC_LI_GROUP, OnDblclkListCtrl)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LI_GROUP, OnItemchangedListCtrl)
	ON_BN_CLICKED(IDC_BT_SEL_APP, OnBtSelApp)
	ON_BN_CLICKED(IDC_BT_SEL_GROUP, OnBtSelGroup)
	ON_BN_CLICKED(IDC_BT_GRP_EXIT, OnBtGrpExit)
	ON_BN_CLICKED(IDC_BT_GRP_ALL_APP, OnBtGrpAllApp)
	ON_BN_CLICKED(IDC_BT_NEW_GROUP, OnBtNewGrp)
END_MESSAGE_MAP()


// PDlgUserList-Meldungshandler
/**---------------------------------------------------------------------------*/
void PDlgGroupList::OnDblclkListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE * pNMIA = (NMITEMACTIVATE *)pNMHDR;

	if (pNMIA->iItem != -1)
	{
		TRACE(__FUNCTION__ ": Item %d\n", pNMIA->iItem);
	
		if (m_bt_SelGroup.IsWindowEnabled())
			OnBtSelGroup();
	}

	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgGroupList::OnBtSelGroup()
{
	BeginWaitCursor();

	int       iItem   = -1;
	BppGroup * pGroup = m_pli_Group->getNextSelectedBsnObj(&iItem);

	if (iItem >= 0)
	{
	PDlgCorGrpUsr dlgCorGrpUsr(pGroup);
	
	int		  iItemApp = -1;
	short sAppID = (short) m_pli_App->getNextSelectedBsnObj(&iItemApp)->getAppid();
	dlgCorGrpUsr.setAppid(sAppID);
	iItemApp = -1;
	dlgCorGrpUsr.setAppname(m_pli_App->getNextSelectedBsnObj(&iItemApp)->getAppname());
	
	EndWaitCursor();

	INT_PTR iRet = dlgCorGrpUsr.DoModal();
	
	BeginWaitCursor();
	
	m_pli_Group->SetFocus();
	}
	else
	{
		MsgBoxOK(IDS_SELECT_ITEM);		
	}
	EndWaitCursor();

}

/**---------------------------------------------------------------------------*/
void PDlgGroupList::OnDblclkAppListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE * pNMIA = (NMITEMACTIVATE *)pNMHDR;

	if (pNMIA->iItem != -1)
	{
		TRACE(__FUNCTION__ ": Item %d\n", pNMIA->iItem);
	
		if (m_bt_SelApp.IsWindowEnabled())
			OnBtSelApp();
	}

	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgGroupList::OnBtSelApp()
{
	BeginWaitCursor();

	int   iItem  = -1;
	short sAppID = (short) m_pli_App->getNextSelectedBsnObj(&iItem)->getAppid();
	if (iItem >= 0)
	{
	m_pli_Group->giveCon().setAppid(sAppID);
	m_pli_Group->giveCon().setBAll(false);
	if (m_pli_Group->load() == FALSE) 
		SetGrpCtrlState(-1); 

	m_pli_Group->EnableWindow(true);
	m_pli_Group->SetFocus();

	SetAppCtrlState(-1);
	}
	else
	{
		MsgBoxOK(IDS_SELECT_ITEM);		
	}
	EndWaitCursor();
}

/**---------------------------------------------------------------------------*/
void PDlgGroupList::OnBtGrpAllApp()
{
	BeginWaitCursor();

	/*int   iItem  = -1;
	short sAppID = (short) m_pli_App->getNextSelectedBsnObj(&iItem)->getAppid();
	if (iItem >= 0)
	{
	m_pli_Group->giveCon().setAppid(sAppID);*/
	m_pli_Group->giveCon().setBAll(true);
	if (m_pli_Group->load() == FALSE) 
		SetGrpCtrlState(-1); 

	m_pli_Group->EnableWindow(true);
	m_pli_Group->SetFocus();

	SetAppCtrlState(-1);
	/*}
	else
	{
		MsgBoxOK(IDS_SELECT_ITEM);		
	}*/

	EndWaitCursor();
}


/**---------------------------------------------------------------------------*/
void PDlgGroupList::OnBtNewGrp()
{
	/*NASE - alle, die Lesen dürfen, dürfen auch Gruppen anlegen!
	CBString strRight = RIGHT_WORK_RI;
	if (ppBLoginData::ex().isLegitimated(MODULE_RIGHTS,strRight))
	{*/
		PDlgDllGroup dlgGroups;
		dlgGroups.DoModal();
		OnBtGrpAllApp();
	/*}
	else
	{
		MsgBoxOK(IDS_NO_RIGHT);
	}*/
}

/**---------------------------------------------------------------------------*/
void PDlgGroupList::OnItemchangedAppListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
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

	SetGrpCtrlState (-1);
	m_pli_Group->EnableWindow(false);
	m_pli_Group->clearAllBsnObj();

	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgGroupList::OnItemchangedListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV   = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	static int   iSelect = -1;

	if (    ((pNMLV->uChanged & LVIF_TEXT) == LVIF_TEXT) 
		 && ( pNMLV->iItem                 == iSelect  ))
	{
		TRACE(__FUNCTION__ ": text changed on Item %d\n", iSelect);
		SetGrpCtrlState(iSelect);
	}
		 
	if ((pNMLV->uChanged & LVIF_STATE) == LVIF_STATE) 
	{
		if ((pNMLV->uNewState & LVNI_SELECTED) == LVNI_SELECTED)			// wird selektiert
		{
			iSelect = pNMLV->iItem;
			TRACE(__FUNCTION__ ": state changed (LVNI_SELECTED) on Item %d\n", iSelect);

			SetGrpCtrlState(iSelect);
		}
		else
		{
			if ((pNMLV->uOldState & LVNI_SELECTED) == LVNI_SELECTED)		// wird deselektiert
				SetGrpCtrlState(-1);
		}
	}
	
	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgGroupList::SetGrpCtrlState(const int iItem)
{
	BOOL    bPrint,
		    bAnzeige;


	if (iItem == -1)					// nichts markiert
	{
		bPrint       = (m_pli_Group->GetItemCount() > 0) ? TRUE : FALSE;		// ListCtrl nicht leer
		bAnzeige     = FALSE;

	}
	else								// Zeile markiert
	{
		bPrint   = TRUE;
		bAnzeige = TRUE;
	}

	// setzen der Controls
	m_bt_SelGroup.EnableWindow(bAnzeige);
}
/**---------------------------------------------------------------------------*/
void PDlgGroupList::SetAppCtrlState(const int iItem)
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
	m_bt_SelApp.EnableWindow(bAnzeige);
}

/**---------------------------------------------------------------------------*/
void PDlgGroupList::OnBtGrpExit()
{
	CDialogMultiLang::OnCancel();
}

/**---------------------------------------------------------------------------*/
BOOL PDlgGroupList::PreTranslateMessage(MSG* pMsg)
{
	assert(pMsg != 0);

	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			// Fokus steht in GroupListCtrl
			if (GetFocus() == GetDlgItem(IDC_LI_GROUP))
			{
				if (m_bt_SelGroup.IsWindowEnabled())
				{
					OnBtSelGroup();
					return TRUE;
				}
			}
			// Fokus steht in AppListCtrl
			if (GetFocus() == GetDlgItem(IDC_LI_APP_GRP))
			{
				if (m_bt_SelApp.IsWindowEnabled())
				{
					OnBtSelApp();
					return TRUE;
				}
			}

			if (GetFocus() == GetDlgItem(IDC_BT_GRP_ALL_APP))
			{
				if (m_bt_Grp_All_App.IsWindowEnabled())
				{
					OnBtGrpAllApp();
					return TRUE;
				}
			}

			if (GetFocus() == GetDlgItem(IDC_BT_SEL_GROUP))
			{
				if (m_bt_SelGroup.IsWindowEnabled())
				{
					OnBtSelGroup();
					return TRUE;
				}
			}

			if (GetFocus() == GetDlgItem(IDC_BT_GRP_EXIT))
			{
				if (m_bt_GrpExit.IsWindowEnabled())
				{
					OnBtGrpExit();
					return TRUE;
				}
			}

			if (GetFocus() == GetDlgItem(IDC_BT_NEW_GROUP))
			{
				if (m_bt_New_Grp.IsWindowEnabled())
				{
					OnBtNewGrp();
					return TRUE;
				}
			}

			
			return FALSE;

		case VK_ESCAPE:
			if (m_bt_GrpExit.IsWindowEnabled())
			{
				OnBtGrpExit();
				return TRUE;
			}
			return FALSE;


		default:
			break;
		}
	} 

	return CDialogMultiLang::PreTranslateMessage(pMsg);
}



