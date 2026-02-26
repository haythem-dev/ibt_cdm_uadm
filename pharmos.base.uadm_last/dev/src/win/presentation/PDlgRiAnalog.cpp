/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                     */
/**   Date   : 21.04.2005                                                     */
/**                                                                           */
/******************************************************************************/

/*----------------------------------------------------------------------------*/
// Includes
/*----------------------------------------------------------------------------*/
#include "StdAfx.h"
#include "CVersion.h"

#include <vector>
using std::vector;
class C_VecArea   : public vector <int>{};
class C_VecUsrGrp : public vector <int>{};
class C_VecMod : public vector <int>{};


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
//Detail-Dlg
#include "PDlgPW.h"
#include "presentation.h"
#include "PDlgRiAnalog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(PDlgRiAnalog, CDialogMultiLang) 

/**---------------------------------------------------------------------------*/
PDlgRiAnalog::PDlgRiAnalog()
	: CDialogMultiLang(PDlgRiAnalog::IDD)
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

	m_pPDlgPW = new PDlgPW;
	assert(m_pPDlgPW);

	 m_ConArea = new C_VecArea;
	 m_ConUsrGrp = new C_VecUsrGrp;
	 m_ConMod = new C_VecMod;

	m_pConBranch = new BConppBranchReg;
	m_pConReg = new BConppBranchReg;
	m_pConUser = new BConppUser;
	m_pConGroup = new BConppGroup;
	m_pConRight = new BConppAppRight;

	m_bGroupDlg = false;
	m_bRegDlg = false;
}

/**---------------------------------------------------------------------------*/
PDlgRiAnalog::~PDlgRiAnalog()
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

	if (m_pPDlgPW != NULL)
	{
		delete m_pPDlgPW;
		m_pPDlgPW = NULL;
	}

	delete m_ConArea;
	delete m_ConMod;
	delete m_ConUsrGrp;

	delete m_pConBranch;
	delete m_pConReg;
	delete m_pConUser;
	delete m_pConGroup;
	delete m_pConRight;
}

/**---------------------------------------------------------------------------*/
void PDlgRiAnalog::DoDataExchange(CDataExchange* pDX)
{
	CDialogMultiLang::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LC_ANAL_GRP,		*m_pli_Group);
	DDX_Control(pDX, IDC_LC_ANAL_USR,		*m_pli_User);
	
	DDX_Control(pDX, IDC_LC_ANAL_APP,		*m_pli_App);
	DDX_Control(pDX, IDC_LC_ANAL_MOD,		*m_pli_Mod);
	DDX_Control(pDX, IDC_LC_ANAL_BR,		*m_pli_Branch);	
	DDX_Control(pDX, IDC_LC_ANAL_REG,		*m_pli_Region);
	
	DDX_Control(pDX, IDC_BT_ANAL_APP,		m_bt_App);
	DDX_Control(pDX, IDC_BT_ANAL_MOD,		m_bt_Mod);
	
	DDX_Control(pDX, IDC_BT_ANAL_EXIT,		m_bt_AnalExit);
	DDX_Control(pDX, IDC_BT_ANAL_SAVE,		m_bt_AnalSave);

	DDX_Control(pDX, IDC_BT_ALL_USRGRP,		m_bt_AllGrpUsr);
		
	DDX_Control(pDX, IDC_CK_ALL_MOD,		m_ck_AllMod);

	DDX_Control(pDX, IDC_CB_ANAL_REG_BR,	m_cb_BrReg);
	DDX_Control(pDX, IDC_CB_ANAL_USR_GRP,	m_cb_UsrGrp);

}

/**---------------------------------------------------------------------------*/
BOOL PDlgRiAnalog::OnInitDialog()
{
	BeginWaitCursor();

	CDialogMultiLang::OnInitDialog();

	//init
	m_pli_App->init();
	m_pli_Mod->init();
	m_pli_User->init();
	m_pli_Group->init();
	m_pli_Branch->init();
	m_pli_Region->init();
	
	//load
	if (m_pli_App->load() == FALSE) 
		SetAppCtrlState(-1);

	m_pli_App->SetFocus();

	//Enable-Ctrls
	SetGrpUsrCtrlState  (false);
	SetModCtrlState     (-1);
	
	m_pli_Mod->EnableWindow(false);
	m_ck_AllMod.EnableWindow(false);
	m_bDtlSelected = false;
	m_ck_AllMod.SetCheck(0);	
	
	EndWaitCursor();

	return FALSE;  // return TRUE unless you set the focus to a control
}

/**---------------------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(PDlgRiAnalog, CDialogMultiLang)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LC_ANAL_APP, OnItemchangedAppListCtrl)
	ON_NOTIFY(NM_DBLCLK, IDC_LC_ANAL_APP, OnDblclkAppListCtrl)
	ON_NOTIFY(NM_DBLCLK, IDC_LC_ANAL_MOD, OnDblclkModListCtrl) 
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LC_ANAL_MOD, OnItemchangedModListCtrl)
	ON_BN_CLICKED(IDC_BT_ANAL_APP, OnBtAnalApp)
	ON_BN_CLICKED(IDC_BT_ANAL_MOD, OnBtAnalMod)
	ON_BN_CLICKED(IDC_BT_ALL_USRGRP, OnBtAllGrpUsr)
	ON_BN_CLICKED(IDC_BT_ANAL_EXIT, OnBtAnalExit)
	ON_BN_CLICKED(IDC_BT_ANAL_SAVE, OnBtAnalSave)
	ON_BN_CLICKED(IDC_CK_ALL_MOD, OnBtAllMod)
	ON_CBN_SELCHANGE(IDC_CB_ANAL_REG_BR, OnCbRegBr)
	ON_CBN_SELCHANGE(IDC_CB_ANAL_USR_GRP, OnCbUsrGrp)
END_MESSAGE_MAP()
/**---------------------------------------------------------------------------*/
void PDlgRiAnalog::OnBtAllMod()
{
	if ( m_bDtlSelected == false )
		OnBtAnalMod();
}
/**---------------------------------------------------------------------------*/
void PDlgRiAnalog::OnCbRegBr()
{
	GetBrRegComboValue();

	if ( m_sBrRegid < 0 )//alle
	{
		m_pli_Branch->ShowWindow(SW_HIDE);
		m_pli_Region->ShowWindow(SW_HIDE);	
		
		if ( m_cb_UsrGrp.GetCurSel() >= 0 )
			SetGrpUsrCtrlState  (true); 

		return;
	}

	if ( m_bRegDlg == true )
	{
		m_pli_Region->giveCon().setBSelBranch(false);
		m_pli_Region->giveCon().setBSelReg(true);
		m_pli_Region->giveCon().setAppid(m_sAppid);
		m_pli_Region->giveCon().setBSelCor(true);
		m_pli_Region->load();
		m_pli_Branch->ShowWindow(SW_HIDE);
		m_pli_Region->EnableWindow(true);
		m_pli_Region->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pli_Branch->giveCon().setAppid(m_sAppid);
		m_pli_Branch->giveCon().setBSelBranch(true);
		m_pli_Branch->giveCon().setBSelReg(false);
		m_pli_Branch->giveCon().setBSelCor(true);
		m_pli_Branch->load();
		m_pli_Branch->ShowWindow(SW_SHOW);
		m_pli_Branch->EnableWindow(true);
		m_pli_Region->ShowWindow(SW_HIDE);
	}

	if ( m_cb_UsrGrp.GetCurSel() >= 0 )
		SetGrpUsrCtrlState  (true); 
}

/**---------------------------------------------------------------------------*/
void PDlgRiAnalog::OnCbUsrGrp()
{
	GetUsrGrpComboValue();
	
	if ( m_bGroupDlg == true ) 
	{
		m_pli_Group->giveCon().setAppid(m_sAppid);
		m_pli_Group->giveCon().setBAll(false);
		m_pli_Group->load();
		m_pli_User->ShowWindow(SW_HIDE);
		m_pli_Group->ShowWindow(SW_SHOW);
		m_pli_Group->EnableWindow(true);
	}
	else
	{
		m_pli_User->giveCon().setAppid(m_sAppid);
		m_pli_User->giveCon().setCorUsrGrp(false);
		m_pli_User->giveCon().setGroupid(0);
		m_pli_User->load();
		m_pli_User->ShowWindow(SW_SHOW);
		m_pli_Group->ShowWindow(SW_HIDE);
		m_pli_User->EnableWindow(true);
	}

	if ( m_cb_BrReg.GetCurSel() >= 0 )
        SetGrpUsrCtrlState  (true); 

	m_bt_AllGrpUsr.EnableWindow(true);
}

/**---------------------------------------------------------------------------*/
void PDlgRiAnalog::OnDblclkModListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE * pNMIA = (NMITEMACTIVATE *)pNMHDR;

	if (pNMIA->iItem != -1)
	{
		TRACE(__FUNCTION__ ": Item %d\n", pNMIA->iItem);
	
		if (m_bt_Mod.IsWindowEnabled())
			OnBtAnalMod();
	}

	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgRiAnalog::OnDblclkAppListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE * pNMIA = (NMITEMACTIVATE *)pNMHDR;

	if (pNMIA->iItem != -1)
	{
		TRACE(__FUNCTION__ ": Item %d\n", pNMIA->iItem);
	
		if (m_bt_App.IsWindowEnabled())
			OnBtAnalApp();
	}

	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgRiAnalog::OnItemchangedAppListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
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
	
	m_pli_Group->EnableWindow(false);
	m_pli_Group->clearAllBsnObj();
	m_pli_User->EnableWindow(false);
	m_pli_User->clearAllBsnObj();
	m_pli_Mod->EnableWindow(false);
	m_ck_AllMod.EnableWindow(false);
	m_bDtlSelected = false;
	m_pli_Mod->clearAllBsnObj();
	m_pli_Branch->EnableWindow(false);
	m_pli_Branch->clearAllBsnObj();
	m_pli_Region->EnableWindow(false);
	m_pli_Region->clearAllBsnObj();
	m_ck_AllMod.SetCheck(0);
	m_cb_BrReg.ResetContent();
	m_cb_UsrGrp.ResetContent();
	SetGrpUsrCtrlState  (false);
	m_pli_User->ShowWindow(SW_HIDE);
	m_pli_Group->ShowWindow(SW_HIDE);
	m_pli_Branch->ShowWindow(SW_HIDE);
	m_pli_Region->ShowWindow(SW_HIDE);
	
	*pResult = 0;
}


/**---------------------------------------------------------------------------*/
void PDlgRiAnalog::OnItemchangedModListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
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

	m_ck_AllMod.SetCheck(0);
		
	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgRiAnalog::SetAppCtrlState(const int iItem)
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
void PDlgRiAnalog::SetModCtrlState     (const int iItem)
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
	m_ck_AllMod.SetCheck(0);
}

/**---------------------------------------------------------------------------*/
void PDlgRiAnalog::SetGrpUsrCtrlState(const bool bShow)
{
	m_bt_AnalSave.EnableWindow(bShow);
	m_bt_AllGrpUsr.EnableWindow(bShow);
}

/**---------------------------------------------------------------------------*/
void PDlgRiAnalog::OnBtAnalApp()
{
	BeginWaitCursor();
	
	int		  iItemApp = -1;
	m_sAppid = (short) m_pli_App->getNextSelectedBsnObj(&iItemApp)->getAppid();
	
	if (iItemApp >= 0)
	{
		//load
		m_pli_Mod->giveCon().setAppid(m_sAppid);
		if (m_pli_Mod->load() == FALSE) 
			SetModCtrlState(-1);
		else
			SetModCtrlState(1);

		//Enable-Ctrls
		m_pli_Group->EnableWindow(false);
		m_pli_Group->clearAllBsnObj();
		m_pli_User->EnableWindow(false);
		m_pli_User->clearAllBsnObj();
		m_pli_Mod->EnableWindow(true);
		m_ck_AllMod.EnableWindow(true);
		m_pli_Branch->EnableWindow(false);
		m_pli_Branch->clearAllBsnObj();
		m_pli_Region->EnableWindow(false);
		m_pli_Region->clearAllBsnObj();
		m_ck_AllMod.SetCheck(0);
		m_cb_BrReg.ResetContent();
		m_cb_UsrGrp.ResetContent();
		
		m_pli_Mod->SetFocus();
	}
	else
	{
		MsgBoxOK(IDS_SELECT_ITEM);		
	}
	EndWaitCursor();
}

/**---------------------------------------------------------------------------*/
void PDlgRiAnalog::OnBtAnalMod()
{
	BeginWaitCursor();

	m_cb_BrReg.ResetContent();
	m_cb_UsrGrp.ResetContent();
	
	int		  iItemMod = -1;
	m_pli_Mod->getNextSelectedBsnObj(&iItemMod);

	if ( iItemMod < 0 && m_ck_AllMod.GetCheck() < 1 )
	{
		m_pli_Mod->SetFocus();
		MsgBoxOK(IDS_SELECT_ITEM);
		return;
	}
	else
	{
		//load Comboboxes
//Branch
		m_pConBranch->setAppid(m_sAppid);
		m_pConBranch->setBSelBranch(true);
		m_pConBranch->setBSelReg(false);
		m_pConBranch->setBSelCor(true);
		m_pConBranch->load();
		BppBranchReg* pBranch = 0;
		unsigned int uiCnt  = 0;
		while ((pBranch = m_pConBranch->getBsnObj(uiCnt++)) != 0)
		{
			m_cb_BrReg.AddString(pBranch->getBranchname().c_str());
		}

//Region
		m_pConReg->setBSelBranch(false);
		m_pConReg->setBSelReg(true);
		m_pConReg->setAppid(m_sAppid);
		m_pConReg->setBSelCor(true);
		m_pConReg->load();
		BppBranchReg* pReg = 0;
		uiCnt  = 0;
		while ((pReg = m_pConReg->getBsnObj(uiCnt++)) != 0)
		{
			m_cb_BrReg.AddString(pReg->getRegname().c_str());
		}
		m_cb_BrReg.AddString(CResString::ex().getStrTblText(IDS_ALL_APP));
		m_cb_BrReg.SetCurSel(-1);
		
//Group
		m_pConGroup->setAppid(m_sAppid);
		m_pConGroup->setBAll(false);
		m_pConGroup->load(); 
		BppGroup* pGroup = 0;
		uiCnt  = 0;
		while ((pGroup = m_pConGroup->getBsnObj(uiCnt++)) != 0)
		{
			m_cb_UsrGrp.AddString(pGroup->getName().c_str());
		}
//User			
		m_pConUser->setAppid(m_sAppid);
		m_pConUser->setCorUsrGrp(false);
		m_pConUser->setGroupid(0);
		m_pConUser->load();
		BppUser* pUser = 0;
		uiCnt  = 0;
		while ((pUser = m_pConUser->getBsnObj(uiCnt++)) != 0)
		{
			m_cb_UsrGrp.AddString(pUser->getUsername().c_str());
		}
		m_cb_UsrGrp.SetCurSel(-1);
	
		m_cb_BrReg.SetFocus();
	}
	m_bDtlSelected = true;
	EndWaitCursor();
}

/**---------------------------------------------------------------------------*/
void PDlgRiAnalog::OnBtAllGrpUsr()
{
	if ( m_bGroupDlg == false)
	{
		m_pli_User->giveCon().setGroupid(0);
		m_pli_User->giveCon().setCorUsrGrp(false);
		m_pli_User->giveCon().setAppid(0);
		
		if (m_pli_User->load() == FALSE) 
			SetGrpUsrCtrlState(false);
		else if ( m_cb_BrReg.GetCurSel() >= 0 ) 
			SetGrpUsrCtrlState(true);
	}
	else
	{	
		m_pli_Group->giveCon().setAppid(m_sAppid);
		m_pli_Group->giveCon().setBAll(true);
		
		if (m_pli_Group->load() == FALSE) 
			SetGrpUsrCtrlState(false);
		else if ( m_cb_BrReg.GetCurSel() >= 0 ) 
			SetGrpUsrCtrlState(true);
	}

	m_bt_AllGrpUsr.EnableWindow(false);
}

/**---------------------------------------------------------------------------*/
void PDlgRiAnalog::FillContainer()
{
	BppBranchReg		* pBrReg = 0;
	BppGroup			* pGroup = 0;
	BppUser				* pUser  = 0;
	BppAppMod			* pMod   = 0;
	int					   iCnt;

	m_ConMod->clear();
	m_ConArea->clear();
	m_ConUsrGrp->clear();

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
		iCnt = -1;
		while ((pMod = m_pli_Mod->getNextSelectedBsnObj(&iCnt)) != 0)
		{
			m_ConMod->push_back(pMod->getModuleid());
		}
	}

	iCnt = -1;
	if ( m_bRegDlg == false && m_sBrRegid > 0 )
	{
		while ((pBrReg = m_pli_Branch->getNextSelectedBsnObj(&iCnt)) != 0)
		{
			m_ConArea->push_back(pBrReg->getBranchid());
		}
	}
	else if ( m_bRegDlg == true && m_sBrRegid > 0 )
	{
		while ((pBrReg = m_pli_Region->getNextSelectedBsnObj(&iCnt)) != 0)
		{
			m_ConArea->push_back(pBrReg->getRegid());
		}
	}
	else // all Branches / Regions
	{
		iCnt = 0;
		while ((pBrReg = m_pConBranch->getBsnObj(iCnt++)) != 0)
		{
			m_ConArea->push_back(pBrReg->getBranchid());
		}

		iCnt = 0;
		while ((pBrReg = m_pConReg->getBsnObj(iCnt++)) != 0)
		{
			m_ConArea->push_back(pBrReg->getRegid());
		}
	}
	
	iCnt = -1;
	if ( m_bGroupDlg == true )
	{
		while ((pGroup = m_pli_Group->getNextSelectedBsnObj(&iCnt)) != 0)
		{
			m_ConUsrGrp->push_back(pGroup->getGroupid());
		}
	}
	else
	{
		while ((pUser = m_pli_User->getNextSelectedBsnObj(&iCnt)) != 0)
		{
			m_ConUsrGrp->push_back(pUser->getUserid());
		}
	}
}

/**---------------------------------------------------------------------------*/
void PDlgRiAnalog::GetBrRegComboValue()
{
	m_sBrRegid = -1;
	CString csAreaName;
	CBString cbAreaName, cbAll;

	m_cb_BrReg.GetLBText(m_cb_BrReg.GetCurSel(),csAreaName);
	cbAreaName = (LPCTSTR)csAreaName;

	cbAll = (LPCTSTR)CResString::ex().getStrTblText(IDS_ALL_APP);

	BppBranchReg	  * pBrReg = 0;
	
	unsigned int        uiCnt  = 0;
	
	if ( cbAll != cbAreaName )	// wenn alle, dann bleibt m_sBrRegid = -1
	{
		while ((pBrReg = m_pConBranch->getBsnObj(uiCnt++)) != 0)
		{
			if ( pBrReg->getBranchname() == cbAreaName )
			{
				m_sBrRegid = pBrReg->getBranchid();
				m_bRegDlg = false;
				break;
			}
		}

		if ( m_sBrRegid < 0 )
		{
			uiCnt = 0;
			while ((pBrReg = m_pConReg->getBsnObj(uiCnt++)) != 0)
			{
				if ( pBrReg->getRegname() == cbAreaName )
				{
					m_sBrRegid = pBrReg->getRegid();
					m_bRegDlg = true;
					break;
				}
			}
		}
	}
}

/**---------------------------------------------------------------------------*/
void PDlgRiAnalog::GetUsrGrpComboValue()
{
	m_lUserid = -1;
	m_lGroupid = -1;
	
	CString csGrpUsrName;
	CBString cbGrpUsrName;
	
	m_cb_UsrGrp.GetLBText(m_cb_UsrGrp.GetCurSel(),csGrpUsrName);
	cbGrpUsrName = (LPCTSTR)csGrpUsrName;
	
	BppUser			  * pUser  = 0;
	BppGroup		  * pGroup = 0;
	unsigned int        uiCnt  = 0;
		
	while ((pUser = m_pConUser->getBsnObj(uiCnt++)) != 0)
	{
		if ( pUser->getUsername() == csGrpUsrName )
		{
			m_lUserid = pUser->getUserid();
			m_lGroupid = 0;
			m_bGroupDlg = false;
			break;
		}
	}

	if ( m_lUserid < 0 )
	{
		uiCnt = 0;
		while ((pGroup = m_pConGroup->getBsnObj(uiCnt++)) != 0)
		{
			if ( pGroup->getName() == csGrpUsrName )
			{
				m_lGroupid = pGroup->getGroupid();
				m_lUserid = 0;
				m_bGroupDlg = true;
				break;
			}
		}
	}
}

/**---------------------------------------------------------------------------*/
void PDlgRiAnalog::OnBtAnalSave()
{
	/* NASE - nicht filialspezifisch! Analog gewollt ?*/
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
	
	INT_PTR iRet = MsgBoxYesNo ( CResString::ex().getStrTblText(IDS_OVERWRITE));
	if(iRet != IDYES)
		return;

	BeginWaitCursor();

	FillContainer();
	
	BppAppRight * pRight = new BppAppRight;
	BppAppRight * pRiRead;

	unsigned int  uiCnt;
					
	
	size_t iSizeMod = m_ConMod->size();
	size_t iSizeArea;
	size_t iSizeUsrGrp;

	IPerst::ERet eRet;

	bool bAction = false;
	
	pRight->setAppid(m_sAppid);
	m_pConRight->setAppid(m_sAppid);
	m_pConRight->setBCorGroupRights(false);
	m_pConRight->setBCorRights(true);
	m_pConRight->setBModRights(false);
		
	while ( iSizeMod-- > 0 )
	{
		pRight->setModid(m_ConMod->at(iSizeMod));
		m_pConRight->setModid(m_ConMod->at(iSizeMod));

		iSizeUsrGrp = m_ConUsrGrp->size();

		while ( iSizeUsrGrp-- > 0 )
		{
			if ( m_bGroupDlg == true )
			{
				pRight->setGroupid(m_ConUsrGrp->at(iSizeUsrGrp));
				pRight->setUserid(0);
				m_pConRight->setGroupid((short)m_lGroupid);
				m_pConRight->setUserid(0);
			}
			else
			{
				pRight->setGroupid(0);
				pRight->setUserid(m_ConUsrGrp->at(iSizeUsrGrp));
				m_pConRight->setGroupid(0);
				m_pConRight->setUserid((short)m_lUserid);
			}
			
			if ( m_sBrRegid > 0 ) 
			{
				iSizeArea = m_ConArea->size();

				while ( iSizeArea-- > 0 )
				{
					pRight->setBranchRegid(m_ConArea->at(iSizeArea));
					m_pConRight->setBranchRegid(m_sBrRegid);

					m_pConRight->load();// Rechte laden, die dann wieder vergeben werden!

					pRight->setRightname("");
					eRet = pRight->deleteAppRight();//delete
					if (eRet != IPerst::RET_OK) 
					{
						MsgBoxOK(IDS_ERR_DELETE);
					}
					else
						bAction = true;

					
					uiCnt  = 0;
					while ((pRiRead = m_pConRight->getBsnObj(uiCnt++)) != 0)
					{
						pRight->setRightname(pRiRead->getRightname());
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
			else // alle BrReg
			{
				iSizeArea = m_ConArea->size();

				while ( iSizeArea-- > 0 )
				{
					pRight->setBranchRegid(m_ConArea->at(iSizeArea));
					m_pConRight->setBranchRegid(m_ConArea->at(iSizeArea));

					m_pConRight->load();

					pRight->setRightname("");
					eRet = pRight->deleteAppRight();//delete
					if (eRet != IPerst::RET_OK) 
					{
						MsgBoxOK(IDS_ERR_DELETE);
					}
					else
						bAction = true;

					
					uiCnt  = 0;
					while ((pRiRead = m_pConRight->getBsnObj(uiCnt++)) != 0)
					{
						pRight->setRightname(pRiRead->getRightname());
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
	}

	CString csMsg;
		
	if ( bAction == true )
	{
		if ( m_bGroupDlg == true )
			csMsg = CResString::ex().getStrTblText(IDS_EXP_RI_OK);
		else
			csMsg = CResString::ex().getStrTblText(IDS_ANAL_RI_USR_OK);
	}
	else
		csMsg = CResString::ex().getStrTblText(IDS_EXP_RI_NO_ACTION);
	
	MsgBoxOK(csMsg);
	delete pRight;
	EndWaitCursor();
}


/**---------------------------------------------------------------------------*/
void PDlgRiAnalog::OnBtAnalExit()
{
	CDialogMultiLang::OnCancel();
}

/**---------------------------------------------------------------------------*/
BOOL PDlgRiAnalog::PreTranslateMessage(MSG* pMsg)
{
	assert(pMsg != 0);

	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			if (GetFocus() == GetDlgItem(IDC_LC_ANAL_APP))
			{
				if (m_bt_App.IsWindowEnabled())
				{
					OnBtAnalApp();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_LC_ANAL_MOD))
			{
				if (m_bt_Mod.IsWindowEnabled())
				{
					OnBtAnalMod();
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
			if (GetFocus() == GetDlgItem(IDC_BT_ANAL_SAVE))
			{
				if (m_bt_AnalSave.IsWindowEnabled())
				{
					OnBtAnalSave();
					return TRUE;
				}
			}	
			if (GetFocus() == GetDlgItem(IDC_BT_ANAL_EXIT))
			{
				if (m_bt_AnalExit.IsWindowEnabled())
				{
					OnBtAnalExit();
					return TRUE;
				}
			}
			return FALSE;

		case VK_ESCAPE:
			if (m_bt_AnalExit.IsWindowEnabled())
			{
				OnBtAnalExit();
				return TRUE;
			}
			return FALSE;


		default:
			break;
		}

		// Suche in User-Liste
		if (GetFocus() == GetDlgItem(IDC_LC_ANAL_USR))
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

