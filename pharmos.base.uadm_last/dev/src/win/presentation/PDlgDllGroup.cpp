/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                     */
/**   Date   : 01.07.2004                                                     */
/**                                                                           */
/******************************************************************************/

/*----------------------------------------------------------------------------*/
// Includes
/*----------------------------------------------------------------------------*/
#include "StdAfx.h"
#include <BppGroup.h>
#include <BConppGroup.h>
#include "PDlgGroupListCtrl.h"
#include "PDlgGroupDetail.h"
#include "PDlgAdmRights.h"
#include "presentation.h"
#include "PDlgDllGroup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(PDlgDllGroup, CDialogMultiLang)

/**---------------------------------------------------------------------------*/
PDlgDllGroup::PDlgDllGroup()
	: CDialogMultiLang(PDlgDllGroup::IDD)
{
	   m_pli_Group = new PDlgGroupListCtrl;
	   assert(m_pli_Group);
}

/**---------------------------------------------------------------------------*/
PDlgDllGroup::~PDlgDllGroup()
{
	if (m_pli_Group != NULL)
	{
		delete m_pli_Group;
		m_pli_Group = NULL;
	}
}

/**---------------------------------------------------------------------------*/
void PDlgDllGroup::DoDataExchange(CDataExchange* pDX)
{
	CDialogMultiLang::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LI_DLL_GRP  , *m_pli_Group);
	DDX_Control(pDX, IDC_BT_GRP_NEW, m_bt_Grp_New);
	DDX_Control(pDX, IDC_BT_GRP_WORK, m_bt_Grp_Work);
	DDX_Control(pDX, IDC_BT_GRP_DEL, m_bt_Grp_Del);
	DDX_Control(pDX, IDC_BT_GRP_RIGHTS, m_bt_Grp_Rights);
	DDX_Control(pDX, IDC_BT_GRP_DLL_EXIT, m_bt_Grp_Exit);
}

/**---------------------------------------------------------------------------*/
BOOL PDlgDllGroup::OnInitDialog()
{
	
	BeginWaitCursor();

	CDialogMultiLang::OnInitDialog();

	// ### AppListCtrl
	m_pli_Group->init();
	m_pli_Group->giveCon().setBAll(true);
	if (m_pli_Group->load() == FALSE) 
		SetCtrlState(-1);
	
	m_pli_Group->SetFocus();
	
	EndWaitCursor();

	return FALSE;  // return TRUE unless you set the focus to a control
}

/**---------------------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(PDlgDllGroup, CDialogMultiLang)
	ON_NOTIFY(NM_DBLCLK, IDC_LI_DLL_GRP, OnDblclkListCtrl)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LI_DLL_GRP, OnItemchangedListCtrl)
	ON_BN_CLICKED(IDC_BT_GRP_NEW, OnBtGrpNew)
	ON_BN_CLICKED(IDC_BT_GRP_WORK, OnBtGrpWork)
	ON_BN_CLICKED(IDC_BT_GRP_DEL, OnBtGrpDel)
	ON_BN_CLICKED(IDC_BT_GRP_RIGHTS, OnBtGrpRights)
	ON_BN_CLICKED(IDC_BT_GRP_DLL_EXIT, OnBtGrpExit)
END_MESSAGE_MAP()

/**---------------------------------------------------------------------------*/
void PDlgDllGroup::OnBtGrpNew()
{
	/*NASE - alle, die Lesen dürfen, dürfen auch Gruppen anlegen!
	CBString strRight = RIGHT_APPGRP_DLL;
	CBString strRight2 = RIGHT_WORK_RI;
	
	if (!(ppBLoginData::ex().isLegitimated(MODULE_DLL,strRight))
		&& !(ppBLoginData::ex().isLegitimated(MODULE_RIGHTS,strRight2)))
	{
		MsgBoxOK(IDS_NO_RIGHT);
		return;
	}*/
	
	BeginWaitCursor();

	BppGroup * pGroup = new BppGroup;
		assert(pGroup);
	
	PDlgGroupDetail dlgGrpDetail(pGroup);
	dlgGrpDetail.setBNewGrp(true);
	
	EndWaitCursor();

	INT_PTR iRet = dlgGrpDetail.DoModal();

	//clone zurückkopieren!
	if (dlgGrpDetail.getBDataChanged())
	{
		dlgGrpDetail.getCloneObj(pGroup);
		if (iRet == IDOK)
		{
			m_pli_Group->giveCon().addBsnObj(*pGroup);
			
			if (m_pli_Group->load(false) == FALSE) 
				SetCtrlState(-1);
		}
	}

	if(pGroup != NULL)
	{
		delete pGroup;
		pGroup = NULL;
	}
	
	BeginWaitCursor();
	
	m_pli_Group->SetFocus();
	
	EndWaitCursor();
}

/**---------------------------------------------------------------------------*/
void PDlgDllGroup::OnBtGrpWork()
{
	BeginWaitCursor();

	int       iItem   = -1;
	BppGroup * pGroup = m_pli_Group->getNextSelectedBsnObj(&iItem);

	if (iItem >= 0)
	{
	PDlgGroupDetail dlgGrpDetail(pGroup);
	dlgGrpDetail.setBNewGrp(false);
	
	EndWaitCursor();

	INT_PTR iRet = dlgGrpDetail.DoModal();
	
	//clone zurückkopieren!
	if (dlgGrpDetail.getBDataChanged())
	{
		dlgGrpDetail.getCloneObj(m_pli_Group->giveCon().getBsnObj(m_pli_Group->getIdx(iItem)));
		if (iRet == IDOK)
			m_pli_Group->refresh(iItem);
	}

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
void PDlgDllGroup::OnBtGrpDel()
{
	CBString strRight = RIGHT_DEL_DLL;
	if (!(ppBLoginData::ex().isLegitimated(MODULE_DLL,strRight)))
	{
		MsgBoxOK(IDS_NO_RIGHT);
		return;
	}
	
	BeginWaitCursor();

	int       iItem   = -1;
	BppGroup * pGroup = m_pli_Group->getNextSelectedBsnObj(&iItem);

	if (iItem >= 0)
	{
	//Sicherheitsabfrage
	CString csDeleteSure;
	csDeleteSure.Format(CResString::ex().getStrTblText(IDS_DELETE_SURE_ALL_APP), 
						pGroup->getName().c_str());
	INT_PTR iRet = MsgBoxYesNo(csDeleteSure);
		
	if(iRet == IDYES)
	{
		IPerst::ERet eRet = pGroup->delGrp();
		if (eRet == IPerst::RET_OK) //Fehler bei Delete auf DB
		{
			m_pli_Group->giveCon().clearBsnObj(m_pli_Group->getIdx(iItem));
			m_pli_Group->load(false);
		}
		else
		{
			MsgBoxOK(IDS_ERR_DELETE); 
		}
	}
	
	m_pli_Group->SetFocus();
	}
	else
	{
		MsgBoxOK(IDS_SELECT_ITEM);		
	}
	EndWaitCursor();
}

/**---------------------------------------------------------------------------*/
void PDlgDllGroup::OnBtGrpRights()
{
	CBString strRight = RIGHT_READ_RI;
	if (!(ppBLoginData::ex().isLegitimated(MODULE_RIGHTS,strRight)))
	{
		MsgBoxOK(IDS_NO_RIGHT);
		return;
	}
	
	bool bGroup = true;
	PDlgAdmRights dlgAdmRights;
	dlgAdmRights.setBGroup(bGroup);
	dlgAdmRights.DoModal();
}

/**---------------------------------------------------------------------------*/
void PDlgDllGroup::OnBtGrpExit()
{
	CDialogMultiLang::OnCancel();
}


/**---------------------------------------------------------------------------*/
void PDlgDllGroup::OnDblclkListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE * pNMIA = (NMITEMACTIVATE *)pNMHDR;

	if (pNMIA->iItem != -1)
	{
		TRACE(__FUNCTION__ ": Item %d\n", pNMIA->iItem);
	
		if (m_bt_Grp_Work.IsWindowEnabled())
			OnBtGrpWork();
	}

	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgDllGroup::OnItemchangedListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
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
void PDlgDllGroup::SetCtrlState(const int iItem)
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
	m_bt_Grp_Work.EnableWindow(bAnzeige);
	m_bt_Grp_Del.EnableWindow(bAnzeige);
}


/**---------------------------------------------------------------------------*/
BOOL PDlgDllGroup::PreTranslateMessage(MSG* pMsg)
{
	assert(pMsg != 0);

	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			// Fokus steht in GrpListCtrl
			if (GetFocus() == GetDlgItem(IDC_LI_DLL_GRP))
			{
				if (m_bt_Grp_Work.IsWindowEnabled())
				{
					OnBtGrpWork();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_BT_GRP_DLL_EXIT))
			{
				if (m_bt_Grp_Exit.IsWindowEnabled())
				{
					OnBtGrpExit();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_BT_GRP_NEW))
			{
				if (m_bt_Grp_New.IsWindowEnabled())
				{
					OnBtGrpNew();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_BT_GRP_WORK))
			{
				if (m_bt_Grp_Work.IsWindowEnabled())
				{
					OnBtGrpWork();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_BT_GRP_DEL))
			{
				if (m_bt_Grp_Del.IsWindowEnabled())
				{
					OnBtGrpDel();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_BT_GRP_RIGHTS))
			{
				if (m_bt_Grp_Rights.IsWindowEnabled())
				{
					OnBtGrpRights();
					return TRUE;
				}
			}
			return FALSE;

		case VK_ESCAPE:
			if (m_bt_Grp_Exit.IsWindowEnabled())
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


