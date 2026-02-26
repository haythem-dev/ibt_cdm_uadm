/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                     */
/**   Date   : 14.06.2004                                                     */
/**                                                                           */
/******************************************************************************/

/*----------------------------------------------------------------------------*/
// Includes
/*----------------------------------------------------------------------------*/
#include "StdAfx.h"
#include "CVersion.h"

#include <vector>
using std::vector;
class C_VecItem : public vector <int>{};

//BsnObj
#include <BppGroup.h>
#include <BConppGroup.h>
#include <BppApp.h>
#include <BConppApp.h>
#include <BppUser.h>
#include <BConppUser.h>
#include <BppAppMod.h>
#include <BConppAppMod.h>
#include <BppBranchReg.h>
#include <BConppBranchReg.h>
#include <BppAppRight.h>
#include <BConppAppRight.h>
//ListCtrl
#include "PDlgGroupListCtrl.h"
#include "PDlgUserListCtrl.h"
#include "PDlgAppListCtrl.h"
#include "PDlgModListCtrl.h"
#include "PDlgBranchListCtrl.h"
#include "PDlgRegionListCtrl.h"
#include "PDlgRightListCtrl.h"
//Detail-Dlg
#include "PDlgPW.h"
#include "presentation.h"
#include "PDlgAdmRights.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(PDlgAdmRights, CDialogMultiLang) 

/**---------------------------------------------------------------------------*/
PDlgAdmRights::PDlgAdmRights()
	: CDialogMultiLang(PDlgAdmRights::IDD)
{
		
	m_pli_GroupRights	= new PDlgRightListCtrl;
	assert (m_pli_GroupRights);
	
		m_pli_Group = new PDlgGroupListCtrl;
		assert(m_pli_Group);
	
		m_pli_User = new PDlgUserListCtrl;
		assert(m_pli_User);
	
	   m_pli_App = new PDlgAppListCtrl;
	   assert(m_pli_App);

	   m_pli_Mod = new PDlgModListCtrl;
	   assert(m_pli_Mod);

	   m_pli_Branch = new PDlgBranchListCtrl;
	   assert(m_pli_Branch);

	   m_pli_Region = new PDlgRegionListCtrl;
	   assert(m_pli_Region);

	   m_pli_CorRights = new PDlgRightListCtrl;
	   assert(m_pli_CorRights);

	   m_pli_AllRights = new PDlgRightListCtrl;
	   assert(m_pli_AllRights);

	   m_pPDlgPW = new PDlgPW;
	   assert(m_pPDlgPW);

	   m_bLoadNew = false;	   
}

/**---------------------------------------------------------------------------*/
PDlgAdmRights::~PDlgAdmRights()
{
	if (m_pli_GroupRights != NULL)
		{
			delete m_pli_GroupRights;
			m_pli_GroupRights = NULL;
		}
	
	if (m_pli_Group != NULL)
		{
			delete m_pli_Group;
			m_pli_Group = NULL;
		}

	if (m_pli_User != NULL)
		{
			delete m_pli_User;
			m_pli_User = NULL;
		}
		
	if (m_pli_App != NULL)
	{
		delete m_pli_App;
		m_pli_App = NULL;
	}

	if (m_pli_Mod != NULL)
	{
		delete m_pli_Mod;
		m_pli_Mod = NULL;
	}

	if (m_pli_Branch != NULL)
	{
		delete m_pli_Branch;
		m_pli_Branch = NULL;
	}

	if (m_pli_Region != NULL)
	{
		delete m_pli_Region;
		m_pli_Region = NULL;
	}	

	if (m_pli_CorRights != NULL)
	{
		delete m_pli_CorRights;
		m_pli_CorRights = NULL;
	}

	if (m_pli_AllRights != NULL)
	{
		delete m_pli_AllRights;
		m_pli_AllRights = NULL;
	}

	if (m_pPDlgPW != NULL)
	{
		delete m_pPDlgPW;
		m_pPDlgPW = NULL;
	}
}

/**---------------------------------------------------------------------------*/
void PDlgAdmRights::setBGroup (bool bGroupAdm)
{
	m_bGroupAdm = bGroupAdm;
}

/**---------------------------------------------------------------------------*/
void PDlgAdmRights::DoDataExchange(CDataExchange* pDX)
{
	CDialogMultiLang::DoDataExchange(pDX);

	if(m_bGroupAdm == true)
	{
		DDX_Control(pDX, IDC_LI_ADM_USR,		*m_pli_Group);
	}
	else
	{
		DDX_Control(pDX, IDC_LI_ADM_USR,		*m_pli_User);
	}

	DDX_Control(pDX, IDC_LI_ADM_GROUPRIGHTS,*m_pli_GroupRights);
	DDX_Control(pDX, IDC_LI_ADM_APP,		*m_pli_App);
	DDX_Control(pDX, IDC_LI_ADM_MODULE,		*m_pli_Mod);
	DDX_Control(pDX, IDC_LI_ADM_BRANCH,		*m_pli_Branch);	
	DDX_Control(pDX, IDC_LI_ADM_REG,		*m_pli_Region);
	DDX_Control(pDX, IDC_LI_ADM_COR_RIGHTS, *m_pli_CorRights);
	DDX_Control(pDX, IDC_LI_ADM_ALLRIGHTS,	*m_pli_AllRights);
	DDX_Control(pDX, IDC_BT_ADM_APP,		m_bt_App);
	DDX_Control(pDX, IDC_BT_ADM_MOD,		m_bt_Mod);
	DDX_Control(pDX, IDC_BT_ADM_BRANCH,		m_bt_Branch);
	DDX_Control(pDX, IDC_BT_ADM_USR,		m_bt_Usr);
	DDX_Control(pDX, IDC_TXT_USRGRP,		m_txt_UsrGrp);
	DDX_Control(pDX, IDC_BT_CORRIGHT,		m_bt_CorRight);
	DDX_Control(pDX, IDC_BT_ERASERIGHT,		m_bt_EraseRight);
	DDX_Control(pDX, IDC_BT_ADM_EXIT,		m_bt_AdmExit);
	DDX_Control(pDX, IDC_BT_REG,			m_bt_Reg);
	DDX_Control(pDX, IDC_TXT_GROUPRIGHTS,	m_txt_GroupRights);
	DDX_Control(pDX, IDC_BT_APP_RIGHT_ALL_GRP, m_bt_All_Grp);
	DDX_Control(pDX, IDC_CB_RIGHT_CATEGORY, m_cb_Category);
	DDX_Control(pDX, IDC_ST_RIGHT_CATEGORY, m_txt_Category);
}

/**---------------------------------------------------------------------------*/
BOOL PDlgAdmRights::OnInitDialog()
{
	
	BeginWaitCursor();

	CDialogMultiLang::OnInitDialog();

	//init
	if(m_bGroupAdm == true)
	{
		m_pli_Group->init();
		m_txt_UsrGrp.SetWindowText(CResString::ex().getStrTblText(IDS_GROUP));
		//CResString::ex().setWndText(m_txt_UsrGrp,IDS_GROUP);
		m_txt_GroupRights.SetWindowText("");
		m_bt_All_Grp.SetWindowText(CResString::ex().getStrTblText(IDS_ALL_GRP));
	}
	else
	{
		m_pli_User->init();
		m_bt_All_Grp.SetWindowText(CResString::ex().getStrTblText(IDS_ALL_USR));
		//m_bt_All_Grp.ShowWindow(SW_HIDE);
		//CResString::ex().setWndText(m_txt_UsrGrp,IDS_USER);
		m_txt_UsrGrp.SetWindowText(CResString::ex().getStrTblText(IDS_USER));
		CResString::ex().setWndText(m_txt_GroupRights,IDS_GROUPRIGHTS);
		m_pli_GroupRights->init();
	}
	m_pli_App->init();
	m_pli_Mod->init();
	m_pli_Branch->init();
	m_pli_Region->init();
	m_pli_CorRights->init();
	m_pli_AllRights->init();

	m_cb_Category.AddString(CResString::ex().getStrTblText(IDS_ALL_APP));
	m_cb_Category.AddString(CResString::ex().getStrTblText(IDS_MAIN_REPORT));
	m_cb_Category.AddString(CResString::ex().getStrTblText(IDS_VARIABLE_REPORT));
	m_cb_Category.SetCurSel(0);
	m_cb_Category.ShowWindow(SW_HIDE);
	m_txt_Category.ShowWindow(SW_HIDE);
		
	//load
	if (m_pli_App->load() == FALSE) 
		SetAppCtrlState(-1);

	m_pli_App->SetFocus();

	//Enable-Ctrls
	SetUsrCtrlState     (-1);
	SetBranchCtrlState  (-1);
	SetRegCtrlState     (-1);
	SetModCtrlState     (-1);
	SetCorRightCtrlState(-1);
	SetAllRightCtrlState(-1);

	
	if(m_bGroupAdm == true)
		m_pli_Group->EnableWindow(false);
	else
		m_pli_User->EnableWindow(false);
	m_pli_Mod->EnableWindow(false);
	m_pli_Branch->EnableWindow(false);
	m_pli_Region->EnableWindow(false);
	m_pli_CorRights->EnableWindow(false);
	m_pli_AllRights->EnableWindow(false);
	m_pli_GroupRights->EnableWindow(false);
	m_cb_Category.ShowWindow(SW_HIDE);
	m_txt_Category.ShowWindow(SW_HIDE);
		
	EndWaitCursor();

	return FALSE;  // return TRUE unless you set the focus to a control
}

/**---------------------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(PDlgAdmRights, CDialogMultiLang)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LI_ADM_APP, OnItemchangedAppListCtrl)
	ON_NOTIFY(NM_DBLCLK, IDC_LI_ADM_APP, OnDblclkAppListCtrl)
	ON_NOTIFY(NM_DBLCLK, IDC_LI_ADM_USR, OnDblclkUsrListCtrl) 
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LI_ADM_USR, OnItemchangedUsrListCtrl)
	ON_NOTIFY(NM_DBLCLK, IDC_LI_ADM_MODULE, OnDblclkModListCtrl) 
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LI_ADM_MODULE, OnItemchangedModListCtrl)
	ON_NOTIFY(NM_DBLCLK, IDC_LI_ADM_REG, OnDblclkRegListCtrl) 
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LI_ADM_REG, OnItemchangedRegListCtrl)
	ON_NOTIFY(NM_DBLCLK, IDC_LI_ADM_BRANCH, OnDblclkBranchListCtrl) 
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LI_ADM_BRANCH, OnItemchangedBranchListCtrl)
	ON_NOTIFY(NM_DBLCLK, IDC_LI_ADM_COR_RIGHTS, OnDblclkCorRightListCtrl) 
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LI_ADM_COR_RIGHTS, OnItemchangedCorRightListCtrl)
	ON_NOTIFY(NM_DBLCLK, IDC_LI_ADM_ALLRIGHTS, OnDblclkAllRightListCtrl) 
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LI_ADM_ALLRIGHTS, OnItemchangedAllRightListCtrl)
	ON_BN_CLICKED(IDC_BT_ADM_APP, OnBtAdmApp)
	ON_BN_CLICKED(IDC_BT_ADM_MOD, OnBtAdmMod)
	ON_BN_CLICKED(IDC_BT_ADM_BRANCH, OnBtAdmBranch)
	ON_BN_CLICKED(IDC_BT_ADM_USR, OnBtAdmUsr)
	ON_BN_CLICKED(IDC_BT_CORRIGHT, OnBtCorRight)
	ON_BN_CLICKED(IDC_BT_ERASERIGHT, OnBtEraseRight)
	ON_BN_CLICKED(IDC_BT_ADM_EXIT, OnBtAdmExit)
	ON_BN_CLICKED(IDC_BT_REG, OnBtReg)
	ON_NOTIFY(NM_SETFOCUS, IDC_LI_ADM_ALLRIGHTS, OnNMSetfocusLiAdmAllrights)
	ON_NOTIFY(NM_SETFOCUS, IDC_LI_ADM_COR_RIGHTS, OnNMSetfocusLiAdmCorRights)
	ON_NOTIFY(NM_CLICK, IDC_LI_ADM_ALLRIGHTS, OnNMClickLiAdmAllrights)
	ON_NOTIFY(NM_CLICK, IDC_LI_ADM_COR_RIGHTS, OnNMClickLiAdmCorRights)
	ON_BN_CLICKED(IDC_BT_APP_RIGHT_ALL_GRP, OnBtAllGrp)
	ON_CBN_SELCHANGE(IDC_CB_RIGHT_CATEGORY, OnRightCategoryChanged)
END_MESSAGE_MAP()


// PDlgUserList-Meldungshandler
/*----------------------------------------------------------------------------*/
void PDlgAdmRights::OnRightCategoryChanged() 
{
	OnBtAdmUsr();
}

/**---------------------------------------------------------------------------*/
void PDlgAdmRights::OnDblclkModListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE * pNMIA = (NMITEMACTIVATE *)pNMHDR;

	if (pNMIA->iItem != -1)
	{
		TRACE(__FUNCTION__ ": Item %d\n", pNMIA->iItem);
	
		if (m_bt_Mod.IsWindowEnabled())
			OnBtAdmMod();
	}

	*pResult = 0;
}
/**---------------------------------------------------------------------------*/
void PDlgAdmRights::OnDblclkRegListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE * pNMIA = (NMITEMACTIVATE *)pNMHDR;

	if (pNMIA->iItem != -1)
	{
		TRACE(__FUNCTION__ ": Item %d\n", pNMIA->iItem);
	
		if (m_bt_Reg.IsWindowEnabled())
			OnBtReg();
	}

	*pResult = 0;
}
/**---------------------------------------------------------------------------*/
void PDlgAdmRights::OnDblclkBranchListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE * pNMIA = (NMITEMACTIVATE *)pNMHDR;

	if (pNMIA->iItem != -1)
	{
		TRACE(__FUNCTION__ ": Item %d\n", pNMIA->iItem);
	
		if (m_bt_Branch.IsWindowEnabled())
			OnBtAdmBranch();
	}

	*pResult = 0;
}
/**---------------------------------------------------------------------------*/
void PDlgAdmRights::OnDblclkCorRightListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE * pNMIA = (NMITEMACTIVATE *)pNMHDR;

	if (pNMIA->iItem != -1)
	{
		TRACE(__FUNCTION__ ": Item %d\n", pNMIA->iItem);
	
		if (m_bt_EraseRight.IsWindowEnabled())
			OnBtEraseRight();
	}

	*pResult = 0;
}
/**---------------------------------------------------------------------------*/
void PDlgAdmRights::OnDblclkAllRightListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE * pNMIA = (NMITEMACTIVATE *)pNMHDR;

	if (pNMIA->iItem != -1)
	{
		TRACE(__FUNCTION__ ": Item %d\n", pNMIA->iItem);
	
		if (m_bt_CorRight.IsWindowEnabled())
			OnBtCorRight();
	}

	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgAdmRights::OnDblclkUsrListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE * pNMIA = (NMITEMACTIVATE *)pNMHDR;

	if (pNMIA->iItem != -1)
	{
		TRACE(__FUNCTION__ ": Item %d\n", pNMIA->iItem);
	
		if (m_bt_Usr.IsWindowEnabled())
			OnBtAdmUsr();
	}

	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgAdmRights::OnDblclkAppListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE * pNMIA = (NMITEMACTIVATE *)pNMHDR;

	if (pNMIA->iItem != -1)
	{
		TRACE(__FUNCTION__ ": Item %d\n", pNMIA->iItem);
	
		if (m_bt_App.IsWindowEnabled())
			OnBtAdmApp();
	}

	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgAdmRights::OnItemchangedAppListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
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

	SetModCtrlState		(-1);
	SetUsrCtrlState     (-1);
	SetBranchCtrlState  (-1);
	SetRegCtrlState     (-1);
	SetCorRightCtrlState(-1);
	SetAllRightCtrlState(-1);
	
	if(m_bGroupAdm == true)
	{
		m_pli_Group->EnableWindow(false);
		m_pli_Group->clearAllBsnObj();
	}
	else
	{
		m_pli_User->EnableWindow(false);
		m_pli_User->clearAllBsnObj();
	}
	m_pli_Mod->EnableWindow(false);
	m_pli_Mod->clearAllBsnObj();
	m_pli_Branch->EnableWindow(false);
	m_pli_Branch->clearAllBsnObj();
	m_pli_Region->EnableWindow(false);
	m_pli_Region->clearAllBsnObj();
	m_pli_CorRights->EnableWindow(false);
	m_pli_CorRights->clearAllBsnObj();
	m_pli_AllRights->EnableWindow(false);
	m_pli_AllRights->clearAllBsnObj();
	m_pli_GroupRights->clearAllBsnObj();
	m_pli_GroupRights->EnableWindow(false);
	m_cb_Category.ShowWindow(SW_HIDE);
	m_txt_Category.ShowWindow(SW_HIDE);

	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgAdmRights::OnItemchangedUsrListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV   = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	static int   iSelect = -1;

	if (    ((pNMLV->uChanged & LVIF_TEXT) == LVIF_TEXT) 
		 && ( pNMLV->iItem                 == iSelect  ))
	{
		TRACE(__FUNCTION__ ": text changed on Item %d\n", iSelect);
		SetUsrCtrlState(iSelect);
	}
		 
	if ((pNMLV->uChanged & LVIF_STATE) == LVIF_STATE) 
	{
		if ((pNMLV->uNewState & LVNI_SELECTED) == LVNI_SELECTED)			// wird selektiert
		{
			iSelect = pNMLV->iItem;
			TRACE(__FUNCTION__ ": state changed (LVNI_SELECTED) on Item %d\n", iSelect);

			SetUsrCtrlState(iSelect);
		}
		else
		{
			if ((pNMLV->uOldState & LVNI_SELECTED) == LVNI_SELECTED)		// wird deselektiert
				SetUsrCtrlState(-1);
		}
	}

	SetCorRightCtrlState(-1);
	SetAllRightCtrlState(-1);

	m_pli_CorRights->EnableWindow(false);
	m_pli_CorRights->clearAllBsnObj();
	m_pli_AllRights->EnableWindow(false);
	m_pli_AllRights->clearAllBsnObj();
	m_pli_GroupRights->clearAllBsnObj();
	m_pli_GroupRights->EnableWindow(false);
	m_cb_Category.ShowWindow(SW_HIDE);
	m_txt_Category.ShowWindow(SW_HIDE);
	
	*pResult = 0;
}
/**---------------------------------------------------------------------------*/
void PDlgAdmRights::OnItemchangedModListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV   = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	static int   iSelect = -1;

	if (    ((pNMLV->uChanged & LVIF_TEXT) == LVIF_TEXT) 
		 && ( pNMLV->iItem                 == iSelect  ))
	{
		TRACE(__FUNCTION__ ": text changed on Item %d\n", iSelect);
		SetModCtrlState(iSelect);
	}
		 
	if ((pNMLV->uChanged & LVIF_STATE) == LVIF_STATE) 
	{
		if ((pNMLV->uNewState & LVNI_SELECTED) == LVNI_SELECTED)			// wird selektiert
		{
			iSelect = pNMLV->iItem;
			TRACE(__FUNCTION__ ": state changed (LVNI_SELECTED) on Item %d\n", iSelect);

			SetModCtrlState(iSelect);
		}
		else
		{
			if ((pNMLV->uOldState & LVNI_SELECTED) == LVNI_SELECTED)		// wird deselektiert
				SetModCtrlState(-1);
		}
	}

	SetUsrCtrlState     (-1);
	SetBranchCtrlState  (-1);
	SetRegCtrlState     (-1);
	SetCorRightCtrlState(-1);
	SetAllRightCtrlState(-1);

	if(m_bGroupAdm == true)
	{
		m_pli_Group->EnableWindow(false);
		m_pli_Group->clearAllBsnObj();
	}
	else
	{
		m_pli_User->EnableWindow(false);
		m_pli_User->clearAllBsnObj();
	}
	m_pli_Branch->EnableWindow(false);
	m_pli_Branch->clearAllBsnObj();
	m_pli_Region->EnableWindow(false);
	m_pli_Region->clearAllBsnObj();
	m_pli_CorRights->EnableWindow(false);
	m_pli_CorRights->clearAllBsnObj();
	m_pli_AllRights->EnableWindow(false);
	m_pli_AllRights->clearAllBsnObj();
	m_pli_GroupRights->clearAllBsnObj();
	m_pli_GroupRights->EnableWindow(false);
	m_cb_Category.ShowWindow(SW_HIDE);
	m_txt_Category.ShowWindow(SW_HIDE);
	
	*pResult = 0;
}
/**---------------------------------------------------------------------------*/
void PDlgAdmRights::OnItemchangedRegListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV   = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	static int   iSelect = -1;

	if (    ((pNMLV->uChanged & LVIF_TEXT) == LVIF_TEXT) 
		 && ( pNMLV->iItem                 == iSelect  ))
	{
		TRACE(__FUNCTION__ ": text changed on Item %d\n", iSelect);
		SetRegCtrlState(iSelect);
	}
		 
	if ((pNMLV->uChanged & LVIF_STATE) == LVIF_STATE) 
	{
		if ((pNMLV->uNewState & LVNI_SELECTED) == LVNI_SELECTED)			// wird selektiert
		{
			iSelect = pNMLV->iItem;
			TRACE(__FUNCTION__ ": state changed (LVNI_SELECTED) on Item %d\n", iSelect);

			SetRegCtrlState(iSelect);
		}
		else
		{
			if ((pNMLV->uOldState & LVNI_SELECTED) == LVNI_SELECTED)		// wird deselektiert
				SetRegCtrlState(-1);
		}
	}
	
	SetUsrCtrlState     (-1);
	SetCorRightCtrlState(-1);
	SetAllRightCtrlState(-1);

	if(m_bGroupAdm == true)
	{
		m_pli_Group->EnableWindow(false);
		m_pli_Group->clearAllBsnObj();
	}
	else
	{
		m_pli_User->EnableWindow(false);
		m_pli_User->clearAllBsnObj();
	}
	m_pli_CorRights->EnableWindow(false);
	m_pli_CorRights->clearAllBsnObj();
	m_pli_AllRights->EnableWindow(false);
	m_pli_AllRights->clearAllBsnObj();
	m_pli_GroupRights->clearAllBsnObj();
	m_pli_GroupRights->EnableWindow(false);
	m_cb_Category.ShowWindow(SW_HIDE);
	m_txt_Category.ShowWindow(SW_HIDE);
	
	*pResult = 0;
}
/**---------------------------------------------------------------------------*/
void PDlgAdmRights::OnItemchangedBranchListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV   = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	static int   iSelect = -1;

	if (    ((pNMLV->uChanged & LVIF_TEXT) == LVIF_TEXT) 
		 && ( pNMLV->iItem                 == iSelect  ))
	{
		TRACE(__FUNCTION__ ": text changed on Item %d\n", iSelect);
		SetBranchCtrlState(iSelect);
	}
		 
	if ((pNMLV->uChanged & LVIF_STATE) == LVIF_STATE) 
	{
		if ((pNMLV->uNewState & LVNI_SELECTED) == LVNI_SELECTED)			// wird selektiert
		{
			iSelect = pNMLV->iItem;
			TRACE(__FUNCTION__ ": state changed (LVNI_SELECTED) on Item %d\n", iSelect);

			SetBranchCtrlState(iSelect);
		}
		else
		{
			if ((pNMLV->uOldState & LVNI_SELECTED) == LVNI_SELECTED)		// wird deselektiert
				SetBranchCtrlState(-1);
		}
	}

	SetUsrCtrlState     (-1);
	SetCorRightCtrlState(-1);
	SetAllRightCtrlState(-1);

	if(m_bGroupAdm == true)
	{
		m_pli_Group->EnableWindow(false);
		m_pli_Group->clearAllBsnObj();
	}
	else
	{
		m_pli_User->EnableWindow(false);
		m_pli_User->clearAllBsnObj();
	}
	m_pli_CorRights->EnableWindow(false);
	m_pli_CorRights->clearAllBsnObj();
	m_pli_AllRights->EnableWindow(false);
	m_pli_AllRights->clearAllBsnObj();
	m_pli_GroupRights->clearAllBsnObj();
	m_pli_GroupRights->EnableWindow(false);
	m_cb_Category.ShowWindow(SW_HIDE);
	m_txt_Category.ShowWindow(SW_HIDE);
	
	*pResult = 0;
}
/**---------------------------------------------------------------------------*/
void PDlgAdmRights::OnItemchangedCorRightListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV   = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	static int   iSelect = -1;

	if (    ((pNMLV->uChanged & LVIF_TEXT) == LVIF_TEXT) 
		 && ( pNMLV->iItem                 == iSelect  ))
	{
		TRACE(__FUNCTION__ ": text changed on Item %d\n", iSelect);
		SetCorRightCtrlState(iSelect);
	}
		 
	if ((pNMLV->uChanged & LVIF_STATE) == LVIF_STATE) 
	{
		if ((pNMLV->uNewState & LVNI_SELECTED) == LVNI_SELECTED)			// wird selektiert
		{
			iSelect = pNMLV->iItem;
			TRACE(__FUNCTION__ ": state changed (LVNI_SELECTED) on Item %d\n", iSelect);

			SetCorRightCtrlState(iSelect);
		}
		else
		{
			if ((pNMLV->uOldState & LVNI_SELECTED) == LVNI_SELECTED)		// wird deselektiert
				SetCorRightCtrlState(-1);
		}
	}
	
	*pResult = 0;
}
/**---------------------------------------------------------------------------*/
void PDlgAdmRights::OnItemchangedAllRightListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV   = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	static int   iSelect = -1;

	if (    ((pNMLV->uChanged & LVIF_TEXT) == LVIF_TEXT) 
		 && ( pNMLV->iItem                 == iSelect  ))
	{
		TRACE(__FUNCTION__ ": text changed on Item %d\n", iSelect);
		SetAllRightCtrlState(iSelect);
	}
		 
	if ((pNMLV->uChanged & LVIF_STATE) == LVIF_STATE) 
	{
		if ((pNMLV->uNewState & LVNI_SELECTED) == LVNI_SELECTED)			// wird selektiert
		{
			iSelect = pNMLV->iItem;
			TRACE(__FUNCTION__ ": state changed (LVNI_SELECTED) on Item %d\n", iSelect);

			SetAllRightCtrlState(iSelect);
		}
		else
		{
			if ((pNMLV->uOldState & LVNI_SELECTED) == LVNI_SELECTED)		// wird deselektiert
				SetAllRightCtrlState(-1);
		}
	}
	
	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgAdmRights::SetUsrCtrlState(const int iItem)
{
	BOOL    bAnzeige;

	if (iItem == -1)					// nichts markiert
	{
		bAnzeige     = FALSE;
	}
	else								// Zeile markiert
	{
		bAnzeige = TRUE;
	}
	m_bt_Usr.EnableWindow(bAnzeige);
	m_bt_All_Grp.EnableWindow(bAnzeige);

}

/**---------------------------------------------------------------------------*/
void PDlgAdmRights::SetAppCtrlState(const int iItem)
{
	BOOL    bAnzeige;

	if (iItem == -1)					// nichts markiert
	{
		bAnzeige     = FALSE;
	}
	else								// Zeile markiert
	{
		bAnzeige = TRUE;
	}
	m_bt_App.EnableWindow(bAnzeige);
}

/**---------------------------------------------------------------------------*/
void PDlgAdmRights::SetBranchCtrlState  (const int iItem)
{
	BOOL    bAnzeige;

	if (iItem == -1)					// nichts markiert
	{
		bAnzeige     = FALSE;
	}
	else								// Zeile markiert
	{
		bAnzeige = TRUE;
	}
	m_bt_Branch.EnableWindow(bAnzeige);
}

/**---------------------------------------------------------------------------*/
void PDlgAdmRights::SetRegCtrlState     (const int iItem)
{
	BOOL    bAnzeige;

	if (iItem == -1)					// nichts markiert
	{
		bAnzeige     = FALSE;
	}
	else								// Zeile markiert
	{
		bAnzeige = TRUE;
	}
	m_bt_Reg.EnableWindow(bAnzeige);
}

/**---------------------------------------------------------------------------*/
void PDlgAdmRights::SetModCtrlState     (const int iItem)
{
	BOOL    bAnzeige;

	if (iItem == -1)					// nichts markiert
	{
		bAnzeige = FALSE;			
	}
	else								// Zeile markiert
	{
		bAnzeige = TRUE;
	}
	m_bt_Mod.EnableWindow(bAnzeige);
}

/**---------------------------------------------------------------------------*/
void PDlgAdmRights::SetCorRightCtrlState(const int iItem)
{
	BOOL    bAnzeige;

	if (iItem == -1)					// nichts markiert
	{
		bAnzeige     = FALSE;
	}
	else								// Zeile markiert
	{
		bAnzeige = TRUE;
	}
	m_bt_EraseRight.EnableWindow(bAnzeige);
}

/**---------------------------------------------------------------------------*/
void PDlgAdmRights::SetAllRightCtrlState(const int iItem)
{
	BOOL    bAnzeige;

	if (iItem == -1)					// nichts markiert
	{
		bAnzeige     = FALSE;
	}
	else								// Zeile markiert
	{
		bAnzeige = TRUE;
	}
	m_bt_CorRight.EnableWindow(bAnzeige);
}

/**---------------------------------------------------------------------------*/
void PDlgAdmRights::OnBtAdmApp()
{
	BeginWaitCursor();
	
	int		  iItemApp = -1;
	m_sAppid = (short) m_pli_App->getNextSelectedBsnObj(&iItemApp)->getAppid();
	m_Appname= m_pli_App->getBsnObj(m_pli_App->getIdx(iItemApp))->getAppname();
		
	if (iItemApp >= 0)
	{
    //load
	m_pli_Mod->giveCon().setAppid(m_sAppid);
	if (m_pli_Mod->load() == FALSE) 
		SetModCtrlState(-1);
	else
		SetModCtrlState(1);

	//Enable-Ctrls
	SetUsrCtrlState     (-1);
	SetBranchCtrlState  (-1);
	SetRegCtrlState     (-1);
	SetCorRightCtrlState(-1);
	SetAllRightCtrlState(-1);
	
	if(m_bGroupAdm == true)
	{
		m_pli_Group->EnableWindow(false);
		m_pli_Group->clearAllBsnObj();
	}
	else
	{
		m_pli_User->EnableWindow(false);
		m_pli_User->clearAllBsnObj();
	}
	m_pli_Mod->EnableWindow(true);
	m_pli_Branch->EnableWindow(false);
	m_pli_Branch->clearAllBsnObj();
	m_pli_Region->EnableWindow(false);
	m_pli_Region->clearAllBsnObj();
	m_pli_CorRights->EnableWindow(false);
	m_pli_CorRights->clearAllBsnObj();
	m_pli_AllRights->EnableWindow(false);
	m_pli_AllRights->clearAllBsnObj();
	m_pli_GroupRights->clearAllBsnObj();
	m_pli_GroupRights->EnableWindow(false);
	m_cb_Category.ShowWindow(SW_HIDE);
	m_txt_Category.ShowWindow(SW_HIDE);

	m_pli_Mod->SetFocus();
	}
	else
	{
		MsgBoxOK(IDS_SELECT_ITEM);		
	}
	EndWaitCursor();
}

/**---------------------------------------------------------------------------*/
void PDlgAdmRights::OnBtAdmMod()
{
	BeginWaitCursor();
	
	int		  iItemMod = -1;
	m_sModid = (short) m_pli_Mod->getNextSelectedBsnObj(&iItemMod)->getModuleid();
	if (iItemMod >= 0)
	{
	//load
	m_pli_Branch->giveCon().setAppid(m_sAppid);
	m_pli_Branch->giveCon().setBSelBranch(true);
	m_pli_Branch->giveCon().setBSelReg(false);
	m_pli_Branch->giveCon().setBSelCor(true);

	if (m_pli_Branch->load() == FALSE) 
		SetBranchCtrlState(-1);
	else
		SetBranchCtrlState(1);

	m_pli_Region->giveCon().setBSelBranch(false);
	m_pli_Region->giveCon().setBSelReg(true);
	m_pli_Region->giveCon().setAppid(m_sAppid);
	m_pli_Region->giveCon().setBSelCor(true);
	if (m_pli_Region->load() == FALSE) 
		SetRegCtrlState(-1);
	else
		SetRegCtrlState(1);



	//Enable-Ctrls
	SetUsrCtrlState     (-1);
	SetCorRightCtrlState(-1);
	SetAllRightCtrlState(-1);
	if(m_bGroupAdm == true)
	{
		m_pli_Group->EnableWindow(false);
		m_pli_Group->clearAllBsnObj();
	}
	else
	{
		m_pli_User->EnableWindow(false);
		m_pli_User->clearAllBsnObj();
	}
	m_pli_Branch->EnableWindow(true);
	m_pli_Region->EnableWindow(true);
	m_pli_CorRights->EnableWindow(false);
	m_pli_CorRights->clearAllBsnObj();
	m_pli_AllRights->EnableWindow(false);
	m_pli_AllRights->clearAllBsnObj();
	m_pli_GroupRights->clearAllBsnObj();
	m_pli_GroupRights->EnableWindow(false);
	m_cb_Category.ShowWindow(SW_HIDE);
	m_txt_Category.ShowWindow(SW_HIDE);

	m_pli_Branch->SetFocus();
	}
	else
	{
		MsgBoxOK(IDS_SELECT_ITEM);		
	}
	EndWaitCursor();
}

/**---------------------------------------------------------------------------*/
void PDlgAdmRights::OnBtAdmBranch()
{
	BeginWaitCursor();
	
	int		  iItemBranch = -1;
	m_sBranchRegid = (short) m_pli_Branch->getNextSelectedBsnObj(&iItemBranch)->getBranchid();
	if (iItemBranch >= 0)
	{
	//load
	if ( m_bGroupAdm == false)
	{
		m_pli_User->giveCon().setGroupid(0);
		m_pli_User->giveCon().setCorUsrGrp(false);
		m_pli_User->giveCon().setAppid(m_sAppid);
		if (m_pli_User->load() == FALSE) 
			SetUsrCtrlState(-1);
		else
			SetUsrCtrlState(1);
	}
	else
	{
		m_pli_Group->giveCon().setAppid(m_sAppid);
		m_pli_Group->giveCon().setBAll(false);
		if (m_pli_Group->load() == FALSE) 
			SetUsrCtrlState(-1);
		else
			SetUsrCtrlState(1);
	}

	//Enable-Ctrls
	SetRegCtrlState     (-1);
	SetCorRightCtrlState(-1);
	SetAllRightCtrlState(-1);
	m_bt_All_Grp.EnableWindow(true);
	if(m_bGroupAdm == true)
	{
		m_pli_Group->EnableWindow(true);
		m_pli_Group->SetFocus();
	}
	else
	{
		m_pli_User->EnableWindow(true);
		m_pli_User->SetFocus();
	}
	m_pli_Region->EnableWindow(false);
	m_pli_Region->clearAllBsnObj();
	m_pli_CorRights->EnableWindow(false);
	m_pli_CorRights->clearAllBsnObj();
	m_pli_AllRights->EnableWindow(false);
	m_pli_AllRights->clearAllBsnObj();
	m_pli_GroupRights->clearAllBsnObj();
	m_pli_GroupRights->EnableWindow(false);
	m_cb_Category.ShowWindow(SW_HIDE);
	m_txt_Category.ShowWindow(SW_HIDE);
	}
	EndWaitCursor();

	if(m_bGroupAdm == true)
		m_txt_UsrGrp.SetWindowText(CResString::ex().getStrTblText(IDS_GROUP));
	else
		m_txt_UsrGrp.SetWindowText(CResString::ex().getStrTblText(IDS_USER));
}

/**---------------------------------------------------------------------------*/
void PDlgAdmRights::OnBtReg()
{
	BeginWaitCursor();
	
	int		  iItemReg = -1;
	m_sBranchRegid = (short) m_pli_Region->getNextSelectedBsnObj(&iItemReg)->getRegid();
	if (iItemReg >= 0)
	{
	//load
	if ( m_bGroupAdm == false)
	{
		m_pli_User->giveCon().setGroupid(0);
		m_pli_User->giveCon().setCorUsrGrp(false);
		m_pli_User->giveCon().setAppid(m_sAppid);
		if (m_pli_User->load() == FALSE) 
			SetUsrCtrlState(-1);
		else
			SetUsrCtrlState(1);
	}
	else
	{
		m_pli_Group->giveCon().setAppid(m_sAppid);
		m_pli_Group->giveCon().setBAll(false);
		if (m_pli_Group->load() == FALSE) 
			SetUsrCtrlState(-1);
		else
			SetUsrCtrlState(1);
	}

	//Enable-Ctrls
	SetBranchCtrlState  (-1);
	SetCorRightCtrlState(-1);
	SetAllRightCtrlState(-1);
	m_bt_All_Grp.EnableWindow(true);
	if(m_bGroupAdm == true)
	{
		m_pli_Group->EnableWindow(true);
		m_pli_Group->SetFocus();
	}
	else
	{
		m_pli_User->EnableWindow(true);
		m_pli_User->SetFocus();
	}
	m_pli_Branch->EnableWindow(false);
	m_pli_Branch->clearAllBsnObj();
	m_pli_CorRights->EnableWindow(false);
	m_pli_CorRights->clearAllBsnObj();
	m_pli_AllRights->EnableWindow(false);
	m_pli_AllRights->clearAllBsnObj();
	m_pli_GroupRights->clearAllBsnObj();
	m_pli_GroupRights->EnableWindow(false);
	m_cb_Category.ShowWindow(SW_HIDE);
	m_txt_Category.ShowWindow(SW_HIDE);
	}
	else
	{
		MsgBoxOK(IDS_SELECT_ITEM);		
	}

	if(m_bGroupAdm == true)
		m_txt_UsrGrp.SetWindowText(CResString::ex().getStrTblText(IDS_GROUP));
	else
		m_txt_UsrGrp.SetWindowText(CResString::ex().getStrTblText(IDS_USER));

	EndWaitCursor();
}

/**---------------------------------------------------------------------------*/
void PDlgAdmRights::OnBtAllGrp()
{
	if ( m_bGroupAdm == false)
	{
		m_pli_User->giveCon().setGroupid(0);
		m_pli_User->giveCon().setCorUsrGrp(false);
		m_pli_User->giveCon().setAppid(0);
		if (m_pli_User->load() == FALSE) 
			SetUsrCtrlState(-1);
		else
			SetUsrCtrlState(1);
	}
	else
	{	
		m_pli_Group->giveCon().setAppid(m_sAppid);
		m_pli_Group->giveCon().setBAll(true);
		if (m_pli_Group->load() == FALSE) 
			SetUsrCtrlState(-1);
		else
			SetUsrCtrlState(1);
	}

	if(m_bGroupAdm == true)
		m_txt_UsrGrp.SetWindowText(CResString::ex().getStrTblText(IDS_EXISTING_GRP));
	else
		m_txt_UsrGrp.SetWindowText(CResString::ex().getStrTblText(IDS_EXISTING_USR));
}

/**---------------------------------------------------------------------------*/
void PDlgAdmRights::OnBtAdmUsr()
{
	BeginWaitCursor();
	int		  iItemUsr = -1;
	
	if (m_bGroupAdm == true)
	{
		m_sGroupid = (short) m_pli_Group->getNextSelectedBsnObj(&iItemUsr)->getGroupid();
		m_sUserid = 0;
	}
	else
	{
		m_sUserid = (short) m_pli_User->getNextSelectedBsnObj(&iItemUsr)->getUserid();
		m_sGroupid = 0;

		if (iItemUsr >= 0)
		{
		if ( m_Appname.area(3) == BI_APPL )
		{
			m_cb_Category.ShowWindow(SW_SHOW);
			m_txt_Category.ShowWindow(SW_SHOW);
		}
		//Gruppen-Rechte eines Users anblenden:
		m_pli_GroupRights->giveCon().setBCorRights(false);
		m_pli_GroupRights->giveCon().setAppid(m_sAppid);
		m_pli_GroupRights->giveCon().setBranchRegid(m_sBranchRegid);
		m_pli_GroupRights->giveCon().setUserid(m_sUserid);
		m_pli_GroupRights->giveCon().setModid(m_sModid);
		m_pli_GroupRights->giveCon().setBCorGroupRights(true);
		m_pli_GroupRights->giveCon().setBModRights(false);
		
		if ( m_cb_Category.IsWindowEnabled() )
			m_pli_GroupRights->giveCon().setRightCategory(m_cb_Category.GetCurSel());
		else
			m_pli_GroupRights->giveCon().setRightCategory(RIGHT_CATEGORY_NONE);

		m_pli_GroupRights->load();
		m_pli_GroupRights->setColumnColor(VS_LC_COL_RIGHTNAME, RGB(0,0,0), RGB(220,220,220));//RGB(204,204,204));
		m_pli_GroupRights->setColumnColor(VS_LC_COL_RIGHTDESC, RGB(0,0,0), RGB(220,220,220));//RGB(204,204,204));
		m_pli_GroupRights->EnableWindow(true);
		}
	}
	
	if (iItemUsr >= 0)
	{
	//load
	if ( m_Appname.area(3) == BI_APPL )
	{
		m_cb_Category.ShowWindow(SW_SHOW);
		m_txt_Category.ShowWindow(SW_SHOW);
	}

	m_pli_CorRights->giveCon().setAppid(m_sAppid);
	m_pli_CorRights->giveCon().setBCorRights(true);
	m_pli_CorRights->giveCon().setBranchRegid(m_sBranchRegid);
	m_pli_CorRights->giveCon().setUserid(m_sUserid);
	m_pli_CorRights->giveCon().setModid(m_sModid);
	m_pli_CorRights->giveCon().setGroupid(m_sGroupid);
	m_pli_CorRights->giveCon().setBCorGroupRights(false);
	m_pli_CorRights->giveCon().setBModRights(false);

	if ( m_cb_Category.IsWindowEnabled() )
		m_pli_CorRights->giveCon().setRightCategory(m_cb_Category.GetCurSel());
	else
		m_pli_CorRights->giveCon().setRightCategory(RIGHT_CATEGORY_NONE);

	if (m_pli_CorRights->load() == FALSE) 
		SetCorRightCtrlState(-1);
	else
		SetCorRightCtrlState(1);

	m_pli_AllRights->giveCon().setAppid(m_sAppid);
	m_pli_AllRights->giveCon().setBCorRights(false);
	m_pli_AllRights->giveCon().setBranchRegid(m_sBranchRegid);
	m_pli_AllRights->giveCon().setUserid(m_sUserid);
	m_pli_AllRights->giveCon().setModid(m_sModid);
	m_pli_AllRights->giveCon().setGroupid(m_sGroupid);
	m_pli_AllRights->giveCon().setBCorGroupRights(false);
	m_pli_AllRights->giveCon().setBModRights(false);

	if ( m_cb_Category.IsWindowEnabled() )
		m_pli_AllRights->giveCon().setRightCategory(m_cb_Category.GetCurSel());
	else
		m_pli_AllRights->giveCon().setRightCategory(RIGHT_CATEGORY_NONE);

	if (m_pli_AllRights->load() == FALSE) 
		SetAllRightCtrlState(-1);
	else
		SetAllRightCtrlState(1);

	//Enable-Ctrls
	m_pli_CorRights->EnableWindow(true);
	m_pli_AllRights->EnableWindow(true);
	
	m_pli_CorRights->SetFocus();
	}
	else
	{
		MsgBoxOK(IDS_SELECT_ITEM);		
	}
	EndWaitCursor();
}

/**---------------------------------------------------------------------------*/
void PDlgAdmRights::OnBtCorRight()
{
	/* NASE - nicht filialspezifisch! */
	CBString strRight = RIGHT_WORK_RI;
	if (!(ppBLoginData::ex().isLegitimated(MODULE_RIGHTS,strRight)))
	{
		int       iItemTemp   = -1;
		m_pli_App->getNextSelectedBsnObj(&iItemTemp);
		strRight = m_pli_App->giveCon().getBsnObj(m_pli_App->getIdx(iItemTemp))->getAppname();
		strRight.upper();

		if (!(ppBLoginData::ex().isLegitimated(MODULE_RIGHTS,strRight)))
		{
			MsgBoxOK(IDS_NO_RIGHT);
			return;
		}
	}
	
	//Rechte zuordnen
	BeginWaitCursor();

	POSITION pos = m_pli_AllRights->GetFirstSelectedItemPosition();
	BppAppRight * pRight;

	C_VecItem * pVItems = new C_VecItem();
	int sCount = 0;
	int iItem = 0;

	while(pos) //Mehrfachauswahl möglich
	{
		sCount++;
		iItem =  m_pli_AllRights->GetNextSelectedItem(pos);
		pVItems->push_back(iItem);
	}

	//Bei UADM nach Passwort fragen!!! - nicht für eigenes APP-Recht
	int		  iItemApp = -1;
	CBString strAppname = m_pli_App->getNextSelectedBsnObj(&iItemApp)->getAppname();
	int		iItemRight = -1;
	CBString strRightname = m_pli_AllRights->getNextSelectedBsnObj(&iItemRight)->getRightname();
	
	if (ppBLoginData::ex().isLegitimated(m_sModid, strRightname) == false || sCount > 1 )
	{
		if (strAppname == CVersion::PROGRAM_NAME)
		{
			m_pPDlgPW->DoModal();
			if (m_pPDlgPW->PwOK() == false)
			{
				MsgBoxOK(IDS_NO_RIGHT);
				delete pVItems;
				return;
			}
		}
	}
	else
	{
		BppApp *pApp = new BppApp;
		pApp->setAppname(strRightname);
		IPerst::ERet Ret = pApp->selectID();
		delete pApp;
		if (Ret != IPerst::RET_OK)//Recht steht nicht für eine Applikation!
		{
			if (strAppname == CVersion::PROGRAM_NAME)
			{
				m_pPDlgPW->DoModal();
				if (m_pPDlgPW->PwOK() == false)
				{
					MsgBoxOK(IDS_NO_RIGHT);
					delete pVItems;
					return;
				}
			}
		}
	}
	
	while (sCount > 0)
	{
		iItem = pVItems->back();
		if (iItem >= 0)
		{
		pRight = m_pli_AllRights->giveCon().getBsnObj(m_pli_AllRights->getIdx(iItem));
		pRight->setAppid(m_sAppid);
		pRight->setBranchRegid(m_sBranchRegid);
		pRight->setGroupid(m_sGroupid);
		pRight->setModid(m_sModid);
		pRight->setUserid(m_sUserid);

		IPerst::ERet eRet = pRight->insertAppRight();
		if (eRet == IPerst::RET_OK) //Fehler bei Insert in DB
		{
			m_pli_CorRights->giveCon().addBsnObj(*pRight);
			m_pli_AllRights->giveCon().clearBsnObj(m_pli_AllRights->getIdx(iItem));
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

	m_pli_CorRights->load(m_bLoadNew);
	
	if (m_pli_AllRights->load() == FALSE) 
		SetAllRightCtrlState(-1);
	else
		m_pli_AllRights->SetFocus();
	
	delete pVItems;

	EndWaitCursor();
}

/**---------------------------------------------------------------------------*/
void PDlgAdmRights::OnBtEraseRight()
{
	/* NASE - nicht filialspezifisch! */
	CBString strRight = RIGHT_WORK_RI;
	if (!(ppBLoginData::ex().isLegitimated(MODULE_RIGHTS,strRight)))
	{
		int       iItemTemp   = -1;
		m_pli_App->getNextSelectedBsnObj(&iItemTemp);
		strRight = m_pli_App->giveCon().getBsnObj(m_pli_App->getIdx(iItemTemp))->getAppname(); 
		strRight.upper();

		if (!(ppBLoginData::ex().isLegitimated(MODULE_RIGHTS,strRight)))
		{
			MsgBoxOK(IDS_NO_RIGHT);
			return;
		}
	}
	
	// Zuordnung entfernen
	BeginWaitCursor();

	BppAppRight * pRight;
	POSITION pos = m_pli_CorRights->GetFirstSelectedItemPosition();
	
	//Bei UADM nach Passwort fragen!!! - nicht für eigenes APP-Recht
	int		  iItemApp = -1;
	CBString strAppname = m_pli_App->getNextSelectedBsnObj(&iItemApp)->getAppname();
	int		iItemRight = -1;
	CBString strRightname = m_pli_CorRights->getNextSelectedBsnObj(&iItemRight)->getRightname();
	
	if (ppBLoginData::ex().isLegitimated(m_sModid, strRightname) == false )
	{
		if (strAppname == CVersion::PROGRAM_NAME)
		{
			m_pPDlgPW->DoModal();
			if (m_pPDlgPW->PwOK() == false)
			{
				MsgBoxOK(IDS_NO_RIGHT);
				return;
			}
		}
	}
	else
	{
		BppApp *pApp = new BppApp;
		pApp->setAppname(strRightname);
		IPerst::ERet Ret = pApp->selectID();
		delete pApp;
		if (Ret != IPerst::RET_OK)//Recht steht nicht für eine Applikation!
		{
			if (strAppname == CVersion::PROGRAM_NAME)
			{
				m_pPDlgPW->DoModal();
				if (m_pPDlgPW->PwOK() == false)
				{
					MsgBoxOK(IDS_NO_RIGHT);
					return;
				}
			}
		}
	}	
	
	int iItem = -1;
	
	iItem =  m_pli_CorRights->GetNextSelectedItem(pos);

	if (iItem >= 0)
	{
		pRight = m_pli_CorRights->giveCon().getBsnObj(m_pli_CorRights->getIdx(iItem));
		pRight->setAppid(m_sAppid);
		pRight->setBranchRegid(m_sBranchRegid);
		pRight->setGroupid(m_sGroupid);
		pRight->setModid(m_sModid);
		pRight->setUserid(m_sUserid);
		
		CString csDeleteSure;
		csDeleteSure.Format(CResString::ex().getStrTblText(IDS_DELETE_SURE), 
						pRight->getRightname().c_str());
		INT_PTR iRet = MsgBoxYesNo(csDeleteSure); 
	
		if(iRet == IDYES)
		{
			IPerst::ERet eRet = pRight->deleteAppRight();
			if (eRet == IPerst::RET_OK) 
			{
				m_pli_AllRights->giveCon().addBsnObj(*pRight);
				m_pli_CorRights->giveCon().clearBsnObj(m_pli_CorRights->getIdx(iItem));
			}
			else
			{
				MsgBoxOK(IDS_ERR_DELETE);
			}

			m_pli_AllRights->load(false);

			if (m_pli_CorRights->load() == FALSE) 
				SetCorRightCtrlState(-1);
			else
				m_pli_CorRights->SetFocus();
		}
		else
			m_pli_CorRights->SetFocus();
	}
	else
	{
		MsgBoxOK(IDS_SELECT_ITEM);		
	}
		
	EndWaitCursor();
	
}

/**---------------------------------------------------------------------------*/
void PDlgAdmRights::OnBtAdmExit()
{
	CDialogMultiLang::OnCancel();
}

/**---------------------------------------------------------------------------*/
BOOL PDlgAdmRights::PreTranslateMessage(MSG* pMsg)
{
	assert(pMsg != 0);

	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			if (GetFocus() == GetDlgItem(IDC_LI_ADM_BRANCH))
			{
				if (m_bt_Branch.IsWindowEnabled())
				{
					OnBtAdmBranch();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_LI_ADM_REG))
			{
				if (m_bt_Reg.IsWindowEnabled())
				{
					OnBtReg();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_LI_ADM_COR_RIGHTS))
			{
				if (m_bt_EraseRight.IsWindowEnabled())
				{
					OnBtEraseRight();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_LI_ADM_ALLRIGHTS))
			{
				if (m_bt_CorRight.IsWindowEnabled())
				{
					OnBtCorRight();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_LI_ADM_USR))
			{
				if (m_bt_Usr.IsWindowEnabled())
				{
					OnBtAdmUsr();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_LI_ADM_APP))
			{
				if (m_bt_App.IsWindowEnabled())
				{
					OnBtAdmApp();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_LI_ADM_MODULE))
			{
				if (m_bt_Mod.IsWindowEnabled())
				{
					OnBtAdmMod();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_BT_ERASERIGHT))
			{
				if (m_bt_EraseRight.IsWindowEnabled())
				{
					OnBtEraseRight();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_BT_CORRIGHT))
			{
				if (m_bt_CorRight.IsWindowEnabled())
				{
					OnBtCorRight();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_BT_APP_RIGHT_ALL_GRP))
			{
				if (m_bt_All_Grp.IsWindowEnabled())
				{
					OnBtAllGrp();
					return TRUE;
				}
			}

			
			if (GetFocus() == GetDlgItem(IDC_BT_ADM_EXIT))
			{
				if (m_bt_AdmExit.IsWindowEnabled())
				{
					OnBtAdmExit();
					return TRUE;
				}
			}
			return FALSE;

		case VK_ESCAPE:
			if (m_bt_AdmExit.IsWindowEnabled())
			{
				OnBtAdmExit();
				return TRUE;
			}
			return FALSE;


		default:
			break;
		}
			
		// Suche in User-Liste
		if (GetFocus() == GetDlgItem(IDC_LI_ADM_USR) && m_bGroupAdm == false)
		{
			if (pMsg->wParam >= CHAR_A && pMsg->wParam <= CHAR_Z)
			{
				CBString cbStr;
				cbStr.format("%c",char(pMsg->wParam));
				m_pli_User->searchSurname(cbStr);
				return TRUE;
			}
		}
	} 

	return CDialogMultiLang::PreTranslateMessage(pMsg);
}

/**---------------------------------------------------------------------------*/
void PDlgAdmRights::OnNMSetfocusLiAdmAllrights(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE * pNMIA = (NMITEMACTIVATE *)pNMHDR;

	if (pNMIA->iItem == -1)
	{
		m_bt_CorRight.EnableWindow(false);
	}
	else
	{
		m_bt_CorRight.EnableWindow(true);
	}
	m_bt_EraseRight.EnableWindow(false);

	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgAdmRights::OnNMSetfocusLiAdmCorRights(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE * pNMIA = (NMITEMACTIVATE *)pNMHDR;

	if (pNMIA->iItem == -1)
	{
		m_bt_EraseRight.EnableWindow(false);
	}
	else
	{
		m_bt_EraseRight.EnableWindow(true);
	}
	m_bt_CorRight.EnableWindow(false);

	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgAdmRights::OnNMClickLiAdmAllrights(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE * pNMIA = (NMITEMACTIVATE *)pNMHDR;

	if (pNMIA->iItem == -1)
	{
		m_bt_CorRight.EnableWindow(false);
	}
	else
	{
		m_bt_CorRight.EnableWindow(true);
	}
	m_bt_EraseRight.EnableWindow(false);
	
	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgAdmRights::OnNMClickLiAdmCorRights(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE * pNMIA = (NMITEMACTIVATE *)pNMHDR;

	if (pNMIA->iItem == -1)
	{
		m_bt_EraseRight.EnableWindow(false);
	}
	else
	{
		m_bt_EraseRight.EnableWindow(true);
	}
	m_bt_CorRight.EnableWindow(false);
	
	*pResult = 0;
}


