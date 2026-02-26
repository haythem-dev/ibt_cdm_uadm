/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                     */
/**   Date   : 15.04.2005                                                     */
/**                                                                           */
/******************************************************************************/

/*----------------------------------------------------------------------------*/
// Includes
/*----------------------------------------------------------------------------*/
#include "StdAfx.h"
#include "CVersion.h"

#include <vector>
using std::vector;
class C_VecRights : public vector <CBString>{};
class C_VecArea   : public vector <int>{};
class C_VecUsrGrp : public vector <int>{};
class C_VecMod	  : public vector <int>{};


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
#include "PDlgExpRights.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(PDlgExpRights, CDialogMultiLang) 

/**---------------------------------------------------------------------------*/
PDlgExpRights::PDlgExpRights()
	: CDialogMultiLang(PDlgExpRights::IDD)
{
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

	m_pli_Rights = new PDlgRightListCtrl;
	assert(m_pli_Rights);


	m_pPDlgPW = new PDlgPW;
	assert(m_pPDlgPW);

	 m_ConRight = new C_VecRights;
	 m_ConArea = new C_VecArea;
	 m_ConUsrGrp = new C_VecUsrGrp;
	 m_ConMod = new C_VecMod;

	m_bLoadNew = false;	
	m_bGroupDlg = false;
}

/**---------------------------------------------------------------------------*/
PDlgExpRights::~PDlgExpRights()
{
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

	if (m_pli_Rights != NULL)
	{
		delete m_pli_Rights;
		m_pli_Rights = NULL;
	}

	if (m_pPDlgPW != NULL)
	{
		delete m_pPDlgPW;
		m_pPDlgPW = NULL;
	}

	delete m_ConRight;
	delete m_ConArea;
	delete m_ConUsrGrp;
	delete m_ConMod;
}

/**---------------------------------------------------------------------------*/
void PDlgExpRights::setBGroup (bool bGroupDlg)
{
	m_bGroupDlg = bGroupDlg;
}

/**---------------------------------------------------------------------------*/
void PDlgExpRights::DoDataExchange(CDataExchange* pDX)
{
	CDialogMultiLang::DoDataExchange(pDX);

	if(m_bGroupDlg == true)
	{
		DDX_Control(pDX, IDC_LC_EXP_GRPUSER,*m_pli_Group);
	}
	else
	{
		DDX_Control(pDX, IDC_LC_EXP_GRPUSER,*m_pli_User);
	}

	DDX_Control(pDX, IDC_LC_EXP_RI,			*m_pli_Rights);
	DDX_Control(pDX, IDC_LC_EXP_APP,		*m_pli_App);
	DDX_Control(pDX, IDC_LC_EXP_MOD,		*m_pli_Mod);
	DDX_Control(pDX, IDC_LC_EXP_BR,			*m_pli_Branch);	
	DDX_Control(pDX, IDC_LC_EXP_REG,		*m_pli_Region);
	DDX_Control(pDX, IDC_BT_EXP_APP,		m_bt_App);
	DDX_Control(pDX, IDC_BT_EXP_MOD,		m_bt_Mod);
	
	DDX_Control(pDX, IDC_ST_GRP_USR,		m_txt_UsrGrp);
	DDX_Control(pDX, IDC_BT_EXP_EXIT,		m_bt_ExpExit);
	DDX_Control(pDX, IDC_BT_EXP_SAVE_RIGHTS,m_bt_ExpSave);
	DDX_Control(pDX, IDC_BT_EXP_DEL_RIGHTS,	m_bt_ExpDelete);

	DDX_Control(pDX, IDC_BT_ALL_GRPUSR,		m_bt_AllGrpUsr);
		
	DDX_Control(pDX, IDC_CK_ALL_MOD,		m_ck_AllMod);
	DDX_Control(pDX, IDC_CK_ALL_RI,			m_ck_AllRi);
	DDX_Control(pDX, IDC_CK_ALL_BR,			m_ck_AllBr);
	DDX_Control(pDX, IDC_CK_ALL_REG,		m_ck_AllReg);
	DDX_Control(pDX, IDC_CK_ALL_USRGRP,		m_ck_AllUsrGrp);
	DDX_Control(pDX, IDC_CK_OVERWRITE,		m_ck_OverWrite);

	DDX_Control(pDX, IDC_CB_RIGHT_CATEGORY, m_cb_Category);
	DDX_Control(pDX, IDC_ST_RIGHT_CATEGORY, m_txt_Category);
}

/**---------------------------------------------------------------------------*/
BOOL PDlgExpRights::OnInitDialog()
{
	BeginWaitCursor();

	CDialogMultiLang::OnInitDialog();

	//init
	if(m_bGroupDlg == true)
	{
		m_pli_Group->init();
		m_txt_UsrGrp.SetWindowText(CResString::ex().getStrTblText(IDS_GROUP));
		m_bt_AllGrpUsr.SetWindowText(CResString::ex().getStrTblText(IDS_ALL_GRP));
	}
	else
	{
		m_pli_User->init();
		m_bt_AllGrpUsr.SetWindowText(CResString::ex().getStrTblText(IDS_ALL_USR));
		m_txt_UsrGrp.SetWindowText(CResString::ex().getStrTblText(IDS_USER));
	}

	m_pli_App->init();
	m_pli_Mod->init();
	m_pli_Branch->init();
	m_pli_Region->init();
	m_pli_Rights->init();
	
	//load
	if (m_pli_App->load() == FALSE) 
		SetAppCtrlState(-1);

	m_pli_App->SetFocus();

	//Enable-Ctrls
	SetGrpUsrCtrlState  (false);
	SetModCtrlState     (-1);
	
	if(m_bGroupDlg == true)
		m_pli_Group->EnableWindow(false);
	else
		m_pli_User->EnableWindow(false);

	m_pli_Mod->EnableWindow(false);
	m_pli_Branch->EnableWindow(false);
	m_pli_Region->EnableWindow(false);
	m_pli_Rights->EnableWindow(false);
	m_ck_AllRi.SetCheck(0);	
	m_ck_AllBr.SetCheck(0);
	m_ck_AllReg.SetCheck(0);
	m_ck_AllUsrGrp.SetCheck(0);

	m_cb_Category.AddString(CResString::ex().getStrTblText(IDS_ALL_APP));
	m_cb_Category.AddString(CResString::ex().getStrTblText(IDS_MAIN_REPORT));
	m_cb_Category.AddString(CResString::ex().getStrTblText(IDS_VARIABLE_REPORT));
	m_cb_Category.SetCurSel(0);
	m_cb_Category.ShowWindow(SW_HIDE);
	m_txt_Category.ShowWindow(SW_HIDE);
		
	EndWaitCursor();

	return FALSE;  // return TRUE unless you set the focus to a control
}

/**---------------------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(PDlgExpRights, CDialogMultiLang)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LC_EXP_APP, OnItemchangedAppListCtrl)
	ON_NOTIFY(NM_DBLCLK, IDC_LC_EXP_APP, OnDblclkAppListCtrl)
	ON_NOTIFY(NM_DBLCLK, IDC_LC_EXP_MOD, OnDblclkModListCtrl) 
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LC_EXP_MOD, OnItemchangedModListCtrl)
	ON_BN_CLICKED(IDC_CK_ALL_MOD, OnClickedAllModCk)
	ON_BN_CLICKED(IDC_BT_EXP_APP, OnBtExpApp)
	ON_BN_CLICKED(IDC_BT_EXP_MOD, OnBtExpMod)
	ON_BN_CLICKED(IDC_BT_ALL_GRPUSR, OnBtAllGrpUsr)
	ON_BN_CLICKED(IDC_BT_EXP_EXIT, OnBtExpExit)
	ON_BN_CLICKED(IDC_BT_EXP_SAVE_RIGHTS,OnBtSaveRights)
	ON_BN_CLICKED(IDC_BT_EXP_DEL_RIGHTS,OnBtDelRights)
	ON_CBN_SELCHANGE(IDC_CB_RIGHT_CATEGORY, OnRightCategoryChanged)
END_MESSAGE_MAP()

/*----------------------------------------------------------------------------*/
void PDlgExpRights::OnRightCategoryChanged() 
{
	OnBtExpMod();
}

/**---------------------------------------------------------------------------*/
void PDlgExpRights::OnClickedAllModCk()
{
	if ( m_ck_AllMod.GetCheck() == 1 )
	{
		//load Branch / Region
		m_pli_Branch->giveCon().setAppid(m_sAppid);
		m_pli_Branch->giveCon().setBSelBranch(true);
		m_pli_Branch->giveCon().setBSelReg(false);
		m_pli_Branch->giveCon().setBSelCor(true);

		m_pli_Branch->load();

		m_pli_Region->giveCon().setBSelBranch(false);
		m_pli_Region->giveCon().setBSelReg(true);
		m_pli_Region->giveCon().setAppid(m_sAppid);
		m_pli_Region->giveCon().setBSelCor(true);
		
		m_pli_Region->load();


		//load User / Group
		if ( m_bGroupDlg == true )
		{
			m_pli_Group->giveCon().setAppid(m_sAppid);
			m_pli_Group->giveCon().setBAll(false);

			m_pli_Group->load(); 

			m_pli_Group->EnableWindow(true);
		}
		else
		{
			m_pli_User->giveCon().setAppid(m_sAppid);
			m_pli_User->giveCon().setCorUsrGrp(false);
			m_pli_User->giveCon().setGroupid(0);
			
			m_pli_User->load();

			m_pli_User->EnableWindow(true);
		}
	
		//Enable-Ctrls
		SetGrpUsrCtrlState  (true);
		m_pli_Branch->EnableWindow(true);
		m_pli_Region->EnableWindow(true);
		m_ck_AllUsrGrp.EnableWindow(true);
		
		m_pli_Rights->EnableWindow(false);
		m_pli_Rights->clearAllBsnObj();
		m_ck_AllRi.SetCheck(0);	
		m_ck_AllRi.EnableWindow(false);	
		m_cb_Category.ShowWindow(SW_HIDE);
		m_txt_Category.ShowWindow(SW_HIDE);

		m_ck_AllBr.SetCheck(0);
		m_ck_AllReg.SetCheck(0);
		m_ck_AllUsrGrp.SetCheck(0);
	}
	else
	{
		if (m_bt_Mod.IsWindowEnabled())
			OnBtExpMod();

		m_ck_AllRi.SetCheck(0);	
		m_ck_AllRi.EnableWindow(true);	
	}
}

/**---------------------------------------------------------------------------*/
void PDlgExpRights::OnDblclkModListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE * pNMIA = (NMITEMACTIVATE *)pNMHDR;

	if (pNMIA->iItem != -1)
	{
		TRACE(__FUNCTION__ ": Item %d\n", pNMIA->iItem);
	
		if (m_bt_Mod.IsWindowEnabled())
			OnBtExpMod();
	}

	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgExpRights::OnDblclkAppListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE * pNMIA = (NMITEMACTIVATE *)pNMHDR;

	if (pNMIA->iItem != -1)
	{
		TRACE(__FUNCTION__ ": Item %d\n", pNMIA->iItem);
	
		if (m_bt_App.IsWindowEnabled())
			OnBtExpApp();
	}

	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgExpRights::OnItemchangedAppListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
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
	
	if(m_bGroupDlg == true)
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
	m_pli_Rights->EnableWindow(false);
	m_pli_Rights->clearAllBsnObj();
	m_cb_Category.ShowWindow(SW_HIDE);
	m_txt_Category.ShowWindow(SW_HIDE);
	
	m_ck_AllRi.SetCheck(0);
	m_ck_AllBr.SetCheck(0);
	m_ck_AllReg.SetCheck(0);
	m_ck_AllUsrGrp.SetCheck(0);
	SetGrpUsrCtrlState  (false);
	
	*pResult = 0;
}


/**---------------------------------------------------------------------------*/
void PDlgExpRights::OnItemchangedModListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
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

	if(m_bGroupDlg == true)
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
	m_pli_Rights->EnableWindow(false);
	m_pli_Rights->clearAllBsnObj();
	m_cb_Category.ShowWindow(SW_HIDE);
	m_txt_Category.ShowWindow(SW_HIDE);
	m_ck_AllRi.SetCheck(0);
	m_ck_AllBr.SetCheck(0);
	m_ck_AllReg.SetCheck(0);
	m_ck_AllUsrGrp.SetCheck(0);
	SetGrpUsrCtrlState  (false);
		
	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgExpRights::SetAppCtrlState(const int iItem)
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
void PDlgExpRights::SetModCtrlState     (const int iItem)
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
	
	m_ck_AllMod.EnableWindow(bAnzeige);
	m_ck_AllMod.SetCheck(0);
}

/**---------------------------------------------------------------------------*/
void PDlgExpRights::SetGrpUsrCtrlState(const bool bShow)
{
	m_bt_AllGrpUsr.EnableWindow(bShow);
	m_bt_ExpSave.EnableWindow(bShow);
	m_bt_ExpDelete.EnableWindow(bShow);
}

/**---------------------------------------------------------------------------*/
void PDlgExpRights::OnBtExpApp()
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
		if(m_bGroupDlg == true)
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
		m_pli_Rights->EnableWindow(false);
		m_pli_Rights->clearAllBsnObj();
		m_cb_Category.ShowWindow(SW_HIDE);
		m_txt_Category.ShowWindow(SW_HIDE);
		m_ck_AllRi.SetCheck(0);
		m_ck_AllBr.SetCheck(0);
		m_ck_AllReg.SetCheck(0);
		m_ck_AllUsrGrp.SetCheck(0);
		m_ck_AllUsrGrp.EnableWindow(true);
		
		m_pli_Mod->SetFocus();
	}
	else
	{
		MsgBoxOK(IDS_SELECT_ITEM);		
	}
	EndWaitCursor();
}

/**---------------------------------------------------------------------------*/
void PDlgExpRights::OnBtExpMod()
{
	BeginWaitCursor();
	
	int		  iItemMod = -1;
	m_sModid = (short) m_pli_Mod->getNextSelectedBsnObj(&iItemMod)->getModuleid();
	
	if (iItemMod >= 0)
	{
		if ( m_Appname.area(3) == BI_APPL )
		{
			m_cb_Category.ShowWindow(SW_SHOW);
			m_txt_Category.ShowWindow(SW_SHOW);
		}
			
		//load Rights
		m_pli_Rights->giveCon().setAppid(m_sAppid);
		m_pli_Rights->giveCon().setModid(m_sModid);
		m_pli_Rights->giveCon().setBCorGroupRights(false);
		m_pli_Rights->giveCon().setBCorRights(false);
		m_pli_Rights->giveCon().setBModRights(true);

		if ( m_cb_Category.IsWindowEnabled() )
			m_pli_Rights->giveCon().setRightCategory(m_cb_Category.GetCurSel());
		else
			m_pli_Rights->giveCon().setRightCategory(RIGHT_CATEGORY_NONE);

		m_pli_Rights->load();


		//load Branch / Region
		m_pli_Branch->giveCon().setAppid(m_sAppid);
		m_pli_Branch->giveCon().setBSelBranch(true);
		m_pli_Branch->giveCon().setBSelReg(false);
		m_pli_Branch->giveCon().setBSelCor(true);

		m_pli_Branch->load();

		m_pli_Region->giveCon().setBSelBranch(false);
		m_pli_Region->giveCon().setBSelReg(true);
		m_pli_Region->giveCon().setAppid(m_sAppid);
		m_pli_Region->giveCon().setBSelCor(true);
		
		m_pli_Region->load();


		//load User / Group
		if ( m_bGroupDlg == true )
		{
			m_pli_Group->giveCon().setAppid(m_sAppid);
			m_pli_Group->giveCon().setBAll(false);

			m_pli_Group->load(); 

			m_pli_Group->EnableWindow(true);
		}
		else
		{
			m_pli_User->giveCon().setAppid(m_sAppid);
			m_pli_User->giveCon().setCorUsrGrp(false);
			m_pli_User->giveCon().setGroupid(0);
			
			m_pli_User->load();

			m_pli_User->EnableWindow(true);
		}

		if(m_bGroupDlg == true)
			m_txt_UsrGrp.SetWindowText(CResString::ex().getStrTblText(IDS_GROUP));
		else
			m_txt_UsrGrp.SetWindowText(CResString::ex().getStrTblText(IDS_USER));
	
		//Enable-Ctrls
		SetGrpUsrCtrlState  (true);
		m_pli_Branch->EnableWindow(true);
		m_pli_Region->EnableWindow(true);
		m_pli_Rights->EnableWindow(true);
		
		m_ck_AllUsrGrp.EnableWindow(true);
		m_pli_Rights->SetFocus();
	}
	else
	{
		MsgBoxOK(IDS_SELECT_ITEM);		
	}
	EndWaitCursor();

	m_ck_AllMod.SetCheck(0);
}

/**---------------------------------------------------------------------------*/
void PDlgExpRights::OnBtAllGrpUsr()
{
	if ( m_bGroupDlg == false)
	{
		m_pli_User->giveCon().setGroupid(0);
		m_pli_User->giveCon().setCorUsrGrp(false);
		m_pli_User->giveCon().setAppid(0);
		
		if (m_pli_User->load() == FALSE) 
			SetGrpUsrCtrlState(false);
		else
			SetGrpUsrCtrlState(true);
	}
	else
	{	
		m_pli_Group->giveCon().setAppid(m_sAppid);
		m_pli_Group->giveCon().setBAll(true);
		
		if (m_pli_Group->load() == FALSE) 
			SetGrpUsrCtrlState(false);
		else
			SetGrpUsrCtrlState(true);
	}

	if(m_bGroupDlg == true)
		m_txt_UsrGrp.SetWindowText(CResString::ex().getStrTblText(IDS_EXISTING_GRP));
	else
		m_txt_UsrGrp.SetWindowText(CResString::ex().getStrTblText(IDS_EXISTING_USR));

	m_ck_AllUsrGrp.SetCheck(0);
	m_ck_AllUsrGrp.EnableWindow(false);
	m_bt_AllGrpUsr.EnableWindow(false);
}

/**---------------------------------------------------------------------------*/
void PDlgExpRights::FillContainer()
{
	BppAppMod           * pMod	 = 0;
	BppAppRight         * pRight = 0;
	BppBranchReg		* pBrReg = 0;
	BppGroup			* pGroup = 0;
	BppUser				* pUser  = 0;
	int					   iCnt;

	m_ConRight->clear();
	m_ConArea->clear();
	m_ConUsrGrp->clear();
	m_ConMod->clear();

	if ( m_ck_AllMod.GetCheck() > 0 )
	{
		iCnt = 0;
		while ((pMod = m_pli_Mod->giveCon().getBsnObj(iCnt++)) != 0)
		{
			m_ConMod->push_back(pMod->getModuleid());
		}
	}
	else
	{
		m_ConMod->push_back(m_sModid);
	}

	if ( m_ck_AllRi.GetCheck() > 0 )
	{
		iCnt = 0;
		while ((pRight = m_pli_Rights->giveCon().getBsnObj(iCnt++)) != 0)
		{
			m_ConRight->push_back(pRight->getRightname());
		}
	}
	else
	{
		iCnt = -1;
		while ((pRight = m_pli_Rights->getNextSelectedBsnObj(&iCnt)) != 0)
		{
			m_ConRight->push_back(pRight->getRightname());
		}
	}

	if ( m_ck_AllBr.GetCheck() > 0 )
	{
		iCnt = 0;
		while ((pBrReg = m_pli_Branch->giveCon().getBsnObj(iCnt++)) != 0)
		{
			if ( pBrReg->getBranchid() != VZ_HAGEDA )
				m_ConArea->push_back(pBrReg->getBranchid());
		}
	}
	else
	{
		iCnt = -1;
		while ((pBrReg = m_pli_Branch->getNextSelectedBsnObj(&iCnt)) != 0)
		{
			m_ConArea->push_back(pBrReg->getBranchid());
		}
	}

	if ( m_ck_AllReg.GetCheck() > 0 )
	{
		iCnt = 0;
		while ((pBrReg = m_pli_Region->giveCon().getBsnObj(iCnt++)) != 0)
		{
			m_ConArea->push_back(pBrReg->getRegid());
		}
	}
	else
	{
		iCnt = -1;
		while ((pBrReg = m_pli_Region->getNextSelectedBsnObj(&iCnt)) != 0)
		{
			m_ConArea->push_back(pBrReg->getRegid());
		}
	}

	if ( m_bGroupDlg == true )
	{
		if ( m_ck_AllUsrGrp.GetCheck() > 0 )
		{
			iCnt = 0;
			while ((pGroup = m_pli_Group->giveCon().getBsnObj(iCnt++)) != 0)
			{
				m_ConUsrGrp->push_back(pGroup->getGroupid());
			}
		}
		else
		{			
			iCnt = -1;
			while ((pGroup = m_pli_Group->getNextSelectedBsnObj(&iCnt)) != 0)
			{
				m_ConUsrGrp->push_back(pGroup->getGroupid());
			}
		}
	}
	else
	{
		if ( m_ck_AllUsrGrp.GetCheck() > 0 )
		{
			iCnt = 0;
			while ((pUser = m_pli_User->giveCon().getBsnObj(iCnt++)) != 0)
			{
				m_ConUsrGrp->push_back(pUser->getUserid());
			}
		}
		else
		{		
			iCnt = -1;
			while ((pUser = m_pli_User->getNextSelectedBsnObj(&iCnt)) != 0)
			{
				m_ConUsrGrp->push_back(pUser->getUserid());
			}
		}
	}
}

/**---------------------------------------------------------------------------*/
void PDlgExpRights::OnBtSaveRights()
{
	/* NASE - nicht filialspezifisch! Express gewollt ?*/
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

	//Bei UADM nach Passwort fragen!!!
	int iItemApp = -1;
	if (m_pli_App->getNextSelectedBsnObj(&iItemApp)->getAppname()== CVersion::PROGRAM_NAME)
	{
		m_pPDlgPW->DoModal();
		if (m_pPDlgPW->PwOK() == false)
		{
			MsgBoxOK(IDS_NO_RIGHT);
			return;
		}
	}
	
	if ( m_ck_OverWrite.GetCheck() > 0 )
	{
		INT_PTR iRet = MsgBoxYesNo ( CResString::ex().getStrTblText(IDS_OVERWRITE));
	
		if(iRet != IDYES)
			return;
	}

	BeginWaitCursor();

	FillContainer();

	BppAppRight * pRight = new BppAppRight;
	
	size_t iSizeMod = m_ConMod->size();;
	size_t iSizeRi;
	size_t iSizeArea;
	size_t iSizeUsrGrp;

	IPerst::ERet eRet;

	bool bAction = false;
	
	pRight->setAppid(m_sAppid);

	BppAppRight  *	pRightForMod	= 0;
	int				iCnt			= 0;
	
	while ( iSizeMod-- > 0 )
	{
		pRight->setModid(m_ConMod->at(iSizeMod));

		if ( m_ck_AllMod.GetCheck() > 0 )
		{
			m_ConRight->clear();

			m_pli_Rights->giveCon().setAppid(m_sAppid);
			m_pli_Rights->giveCon().setModid(m_ConMod->at(iSizeMod));
			m_pli_Rights->giveCon().setBCorGroupRights(false);
			m_pli_Rights->giveCon().setBCorRights(false);
			m_pli_Rights->giveCon().setBModRights(true);

			if ( m_cb_Category.IsWindowEnabled() )
				m_pli_Rights->giveCon().setRightCategory(m_cb_Category.GetCurSel());
			else
				m_pli_Rights->giveCon().setRightCategory(RIGHT_CATEGORY_NONE);

			m_pli_Rights->load();

			iCnt = 0;
			while ((pRightForMod = m_pli_Rights->giveCon().getBsnObj(iCnt++)) != 0)
			{
				m_ConRight->push_back(pRightForMod->getRightname());
			}

			m_pli_Rights->clearAllBsnObj();
		}

		iSizeArea = m_ConArea->size(); 

		while ( iSizeArea-- > 0 )
		{
			pRight->setBranchRegid(m_ConArea->at(iSizeArea));
			
			iSizeUsrGrp = m_ConUsrGrp->size();

			while ( iSizeUsrGrp-- > 0 )
			{
				if ( m_bGroupDlg == true )
				{
					pRight->setGroupid(m_ConUsrGrp->at(iSizeUsrGrp));
					pRight->setUserid(0);
				}
				else
				{
					pRight->setGroupid(0);
					pRight->setUserid(m_ConUsrGrp->at(iSizeUsrGrp));
				}
				
				iSizeRi = m_ConRight->size();
				
				if ( m_ck_OverWrite.GetCheck() > 0 )
				{
					pRight->setRightname("");

					eRet = pRight->deleteAppRight();//delete
					if (eRet != IPerst::RET_OK) 
					{
						MsgBoxOK(IDS_ERR_DELETE);
					}
					else
						bAction = true;
				}
				
				while ( iSizeRi-- > 0 )
				{
					pRight->setRightname(m_ConRight->at(iSizeRi));
					eRet = pRight->insertAppRight();//Insert
					if (eRet == IPerst::RET_ERROR) 
					{
						MsgBoxOK(IDS_ERR_INSERT);
						delete pRight;
						EndWaitCursor();
						return;
					}
					bAction = true;
				}
			}
		}
	}
	
	if ( bAction == true)
		MsgBoxOK(CResString::ex().getStrTblText(IDS_EXP_RI_OK));
	else
		MsgBoxOK(CResString::ex().getStrTblText(IDS_EXP_RI_NO_ACTION));

	delete pRight;
	EndWaitCursor();
}

/**---------------------------------------------------------------------------*/
void PDlgExpRights::OnBtDelRights()
{
	/* NASE - nicht filialspezifisch! Express gewollt ?*/
	CBString strRight = RIGHT_WORK_RI;
	if (!(ppBLoginData::ex().isLegitimated(MODULE_RIGHTS,strRight)))
	{
		int       iItemTemp   = -1;
		m_pli_App->getNextSelectedBsnObj(&iItemTemp);
		strRight = m_pli_App->giveCon().getBsnObj(m_pli_App->getIdx(iItemTemp))->getAppname(); 
		strRight.upper();

		if (!(ppBLoginData::ex().isLegitimated(MODULE_RIGHTS, strRight)))
		{
			MsgBoxOK(IDS_NO_RIGHT);
			return;
		}
	}

	//Bei UADM nach Passwort fragen!!!
	int iItemApp = -1;
	if (m_pli_App->getNextSelectedBsnObj(&iItemApp)->getAppname()== CVersion::PROGRAM_NAME)
	{
		m_pPDlgPW->DoModal();
		if (m_pPDlgPW->PwOK() == false)
		{
			MsgBoxOK(IDS_NO_RIGHT);
			return;
		}
	}
	
	INT_PTR iRet = MsgBoxYesNo ( CResString::ex().getStrTblText(IDS_DEL_EXPRESS));
	
	if(iRet != IDYES)
		return;
	
	BeginWaitCursor();

	FillContainer();

	BppAppRight * pRight = new BppAppRight;
	
	size_t iSizeMod = m_ConMod->size();;
	size_t iSizeRi;
	size_t iSizeArea;
	size_t iSizeUsrGrp;

	IPerst::ERet eRet;

	bool bAction = false;
	
	pRight->setAppid(m_sAppid);

	BppAppRight  *	pRightForMod	= 0;
	int				iCnt			= 0;
	
	while ( iSizeMod-- > 0 )
	{
		pRight->setModid(m_ConMod->at(iSizeMod));

		if ( m_ck_AllMod.GetCheck() > 0 )
		{
			m_ConRight->clear();

			m_pli_Rights->giveCon().setAppid(m_sAppid);
			m_pli_Rights->giveCon().setModid(m_ConMod->at(iSizeMod));
			m_pli_Rights->giveCon().setBCorGroupRights(false);
			m_pli_Rights->giveCon().setBCorRights(false);
			m_pli_Rights->giveCon().setBModRights(true);

			if ( m_cb_Category.IsWindowEnabled() )
				m_pli_Rights->giveCon().setRightCategory(m_cb_Category.GetCurSel());
			else
				m_pli_Rights->giveCon().setRightCategory(RIGHT_CATEGORY_NONE);

			m_pli_Rights->load();

			iCnt = 0;
			while ((pRightForMod = m_pli_Rights->giveCon().getBsnObj(iCnt++)) != 0)
			{
				m_ConRight->push_back(pRightForMod->getRightname());
			}

			m_pli_Rights->clearAllBsnObj();
		}

		iSizeArea = m_ConArea->size(); 

		while ( iSizeArea-- > 0 )
		{
			pRight->setBranchRegid(m_ConArea->at(iSizeArea));
			
			iSizeUsrGrp = m_ConUsrGrp->size();

			while ( iSizeUsrGrp-- > 0 )
			{
				if ( m_bGroupDlg == true )
				{
					pRight->setGroupid(m_ConUsrGrp->at(iSizeUsrGrp));
					pRight->setUserid(0);
				}
				else
				{
					pRight->setGroupid(0);
					pRight->setUserid(m_ConUsrGrp->at(iSizeUsrGrp));
				}
				
				iSizeRi = m_ConRight->size();
				
				while ( iSizeRi-- > 0 )
				{
					pRight->setRightname(m_ConRight->at(iSizeRi));
					eRet = pRight->deleteAppRight();
					if (eRet == IPerst::RET_ERROR) 
					{
						MsgBoxOK(IDS_ERR_DELETE);
						delete pRight;
						EndWaitCursor();
						return;
					}
					bAction = true;
				}
			}
		}
	}
	
	if ( bAction == true)
		MsgBoxOK(CResString::ex().getStrTblText(IDS_EXP_RI_OK));
	else
		MsgBoxOK(CResString::ex().getStrTblText(IDS_EXP_RI_NO_ACTION));

	delete pRight;
	EndWaitCursor();
}

/**---------------------------------------------------------------------------*/
void PDlgExpRights::OnBtExpExit()
{
	CDialogMultiLang::OnCancel();
}

/**---------------------------------------------------------------------------*/
BOOL PDlgExpRights::PreTranslateMessage(MSG* pMsg)
{
	assert(pMsg != 0);

	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			if (GetFocus() == GetDlgItem(IDC_LC_EXP_APP))
			{
				if (m_bt_App.IsWindowEnabled())
				{
					OnBtExpApp();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_LC_EXP_MOD))
			{
				if (m_bt_Mod.IsWindowEnabled())
				{
					OnBtExpMod();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_BT_ALL_GRPUSR))
			{
				if (m_bt_AllGrpUsr.IsWindowEnabled())
				{
					OnBtAllGrpUsr();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_BT_EXP_SAVE_RIGHTS))
			{
				if (m_bt_ExpSave.IsWindowEnabled())
				{
					OnBtSaveRights();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_BT_EXP_DEL_RIGHTS))
			{
				if (m_bt_ExpDelete.IsWindowEnabled())
				{
					OnBtDelRights();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_BT_EXP_EXIT))
			{
				if (m_bt_ExpExit.IsWindowEnabled())
				{
					OnBtExpExit();
					return TRUE;
				}
			}
			return FALSE;

		case VK_ESCAPE:
			if (m_bt_ExpExit.IsWindowEnabled())
			{
				OnBtExpExit();
				return TRUE;
			}
			return FALSE;


		default:
			break;
		}
					
		// Suche in User-Liste
		if (GetFocus() == GetDlgItem(IDC_LC_EXP_GRPUSER) && m_bGroupDlg == false)
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

