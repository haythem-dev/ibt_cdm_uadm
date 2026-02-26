/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                     */
/**   Date   : 28.06.2004                                                     */
/**                                                                           */
/******************************************************************************/

/*----------------------------------------------------------------------------*/
// Includes
/*----------------------------------------------------------------------------*/
#include "StdAfx.h"
#include <BppApp.h>
#include <BConppApp.h>
#include <BppAppMod.h>
#include <BConppAppMod.h>
#include <BppAppRight.h>
#include <BConppAppRight.h>
#include "PDlgAppListCtrl.h"
#include "PDlgModListCtrl.h"
#include "PDlgRightListCtrl.h"
#include "presentation.h"
#include "PDlgAppDetail.h"
#include "PDlgModDetail.h"
#include "PDlgRightDetail.h"
#include "PDlgDllAppModRi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*----------------------------------------------------------------------------*/
// 
/**---------------------------------------------------------------------------*/
IMPLEMENT_DYNCREATE(PDlgDllAppModRi, CDialogMultiLang)

/**---------------------------------------------------------------------------*/
PDlgDllAppModRi::PDlgDllAppModRi()
	: CDialogMultiLang(PDlgDllAppModRi::IDD)
{

   m_pli_Mod = new PDlgModListCtrl;
	   assert(m_pli_Mod);

    m_pli_Right = new PDlgRightListCtrl;
	   assert(m_pli_Right);

   m_pli_App = new PDlgAppListCtrl;
	   assert(m_pli_App);
	
}

/**---------------------------------------------------------------------------*/
PDlgDllAppModRi::~PDlgDllAppModRi()
{
	if (m_pli_Right != NULL)
	{
		delete m_pli_Right;
		m_pli_Right = NULL;
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
	
}

/**---------------------------------------------------------------------------*/
void PDlgDllAppModRi::DoDataExchange(CDataExchange* pDX)
{
	CDialogMultiLang::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LI_DLL_APP,	*m_pli_App);
	DDX_Control(pDX, IDC_LI_DLL_MOD,	*m_pli_Mod);
	DDX_Control(pDX, IDC_LI_DLL_RIGHTS, *m_pli_Right);
	DDX_Control(pDX, IDC_BT_DLL_APP_EXIT, m_bt_Exit);
	DDX_Control(pDX, IDC_BT_DLL_APP_SEL, m_bt_App_Sel);
	DDX_Control(pDX, IDC_BT_DLL_APP_NEW, m_bt_App_New);
	DDX_Control(pDX, IDC_BT_DLL_APP_WORK, m_bt_App_Work);
	DDX_Control(pDX, IDC_BT_DLL_APP_DEL, m_bt_App_Del);
	DDX_Control(pDX, IDC_BT_DLL_MOD_SEL, m_bt_Mod_Sel);
	DDX_Control(pDX, IDC_BT_DLL_MOD_NEW, m_bt_Mod_New);
	DDX_Control(pDX, IDC_BT_DLL_MOD_WORK, m_bt_Mod_Work);
	DDX_Control(pDX, IDC_BT_DLL_MOD_DEL, m_bt_Mod_Del);
	DDX_Control(pDX, IDC_BT_DLL_RI_NEW, m_bt_Ri_New);
	DDX_Control(pDX, IDC_BT_DLL_RI_WORK, m_bt_Ri_Work);
	DDX_Control(pDX, IDC_BT_DLL_RI_DEL, m_bt_Ri_Del);
	DDX_Control(pDX, IDC_CB_RIGHT_CATEGORY, m_cb_Category);
	DDX_Control(pDX, IDC_ST_RIGHT_CATEGORY, m_txt_Category);
}

/**---------------------------------------------------------------------------*/
BOOL PDlgDllAppModRi::OnInitDialog()
{
	BeginWaitCursor();

	CDialogMultiLang::OnInitDialog();

	m_pli_App->init();
	if (m_pli_App->load() == FALSE) 
		SetAppCtrlState(-1);
	
	m_pli_Mod->init();
	m_pli_Right->init();

	m_cb_Category.AddString(CResString::ex().getStrTblText(IDS_ALL_APP));
	m_cb_Category.AddString(CResString::ex().getStrTblText(IDS_MAIN_REPORT));
	m_cb_Category.AddString(CResString::ex().getStrTblText(IDS_VARIABLE_REPORT));
	m_cb_Category.SetCurSel(0);
	m_cb_Category.ShowWindow(SW_HIDE);
	m_txt_Category.ShowWindow(SW_HIDE);

	m_pli_App->SetFocus();
	
	//Controls
	m_pli_Right->EnableWindow(false);
	m_pli_Mod->EnableWindow(false);
	SetModCtrlState(-1);
	SetRiCtrlState(-1);
	
	EndWaitCursor();

	return FALSE;  // return TRUE unless you set the focus to a control
}

/**---------------------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(PDlgDllAppModRi, CDialogMultiLang)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LI_DLL_APP, OnItemchangedAppListCtrl)
	ON_NOTIFY(NM_DBLCLK, IDC_LI_DLL_APP, OnDblclkAppListCtrl)
	ON_NOTIFY(NM_DBLCLK, IDC_LI_DLL_MOD, OnDblclkModListCtrl)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LI_DLL_MOD, OnItemchangedModListCtrl)
	ON_NOTIFY(NM_DBLCLK, IDC_LI_DLL_RIGHTS, OnDblclkRightListCtrl)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LI_DLL_RIGHTS, OnItemchangedRightListCtrl)
	ON_NOTIFY(NM_CLICK, IDC_LI_DLL_MOD, OnNMClickLiMod)
	ON_NOTIFY(NM_CLICK, IDC_LI_DLL_RIGHTS, OnNMClickLiRight)
	ON_BN_CLICKED(IDC_BT_DLL_APP_SEL, OnBtDllAppSel)
	ON_BN_CLICKED(IDC_BT_DLL_APP_NEW, OnBtDllAppNew)
	ON_BN_CLICKED(IDC_BT_DLL_APP_WORK, OnBtDllAppWork)
	ON_BN_CLICKED(IDC_BT_DLL_APP_DEL, OnBtDllAppDel)
	ON_BN_CLICKED(IDC_BT_DLL_MOD_SEL, OnBtDllModSel)
	ON_BN_CLICKED(IDC_BT_DLL_MOD_NEW, OnBtDllModNew)
	ON_BN_CLICKED(IDC_BT_DLL_MOD_WORK, OnBtDllModWork)
	ON_BN_CLICKED(IDC_BT_DLL_MOD_DEL, OnBtDllModDel)
	ON_BN_CLICKED(IDC_BT_DLL_RI_NEW, OnBtDllRiNew)
	ON_BN_CLICKED(IDC_BT_DLL_RI_WORK, OnBtDllRiWork)
	ON_BN_CLICKED(IDC_BT_DLL_RI_DEL, OnBtDllRiDel)
	ON_BN_CLICKED(IDC_BT_DLL_APP_EXIT, OnBtDllAppExit)
	ON_CBN_SELCHANGE(IDC_CB_RIGHT_CATEGORY, OnRightCategoryChanged)
END_MESSAGE_MAP()

/*----------------------------------------------------------------------------*/
void PDlgDllAppModRi::OnRightCategoryChanged() 
{
	OnBtDllModSel();
}

/**---------------------------------------------------------------------------*/
void PDlgDllAppModRi::OnNMClickLiRight(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE * pNMIA = (NMITEMACTIVATE *)pNMHDR;

	if (pNMIA->iItem == -1)
	{
		SetRiCtrlState	(-1);
	}
	else
	{
		SetRiCtrlState	(1);
	}
	m_bt_Ri_New.EnableWindow(true);
	
	
	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgDllAppModRi::OnNMClickLiMod(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE * pNMIA = (NMITEMACTIVATE *)pNMHDR;

	if (pNMIA->iItem == -1)
	{
		SetModCtrlState	(-1);
	}
	else
	{
		SetModCtrlState	(1);
	}
	m_bt_Mod_New.EnableWindow(true);
	
	
	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgDllAppModRi::SetAppCtrlState(const int iItem)
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
	m_bt_App_Sel.EnableWindow(bAnzeige);
	m_bt_App_Work.EnableWindow(bAnzeige);
	m_bt_App_Del.EnableWindow(bAnzeige);
}

/**---------------------------------------------------------------------------*/
void PDlgDllAppModRi::SetModCtrlState(const int iItem)
{
	BOOL    bPrint,
		    bAnzeige;


	if (iItem == -1)					// nichts markiert
	{
		bPrint       = (m_pli_Mod->GetItemCount() > 0) ? TRUE : FALSE;		// ListCtrl nicht leer
		bAnzeige     = FALSE;

	}
	else								// Zeile markiert
	{
		bPrint   = TRUE;
		bAnzeige = TRUE;
	}

	// setzen der Controls
	m_bt_Mod_Sel.EnableWindow(bAnzeige);
	m_bt_Mod_Work.EnableWindow(bAnzeige);
	m_bt_Mod_Del.EnableWindow(bAnzeige);
	m_bt_Mod_New.EnableWindow(bAnzeige);
}

/**---------------------------------------------------------------------------*/
void PDlgDllAppModRi::SetRiCtrlState(const int iItem)
{
	BOOL    bPrint,
		    bAnzeige;


	if (iItem == -1)					// nichts markiert
	{
		bPrint       = (m_pli_Right->GetItemCount() > 0) ? TRUE : FALSE;		// ListCtrl nicht leer
		bAnzeige     = FALSE;

	}
	else								// Zeile markiert
	{
		bPrint   = TRUE;
		bAnzeige = TRUE;
	}

	// setzen der Controls
	m_bt_Ri_Work.EnableWindow(bAnzeige);
	m_bt_Ri_Del.EnableWindow(bAnzeige);
	m_bt_Ri_New.EnableWindow(bAnzeige);
}

/**---------------------------------------------------------------------------*/
void PDlgDllAppModRi::OnBtDllAppSel()
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
	m_bt_Mod_New.EnableWindow(true);
	m_pli_Mod->EnableWindow(true);
	SetRiCtrlState     (-1);
	m_pli_Right->EnableWindow(false);
	m_pli_Right->clearAllBsnObj();
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
void PDlgDllAppModRi::OnBtDllAppNew()
{
	CBString strRight = RIGHT_APPGRP_DLL;
	if (!(ppBLoginData::ex().isLegitimated(MODULE_DLL,strRight)))
	{
		MsgBoxOK(IDS_NO_RIGHT);
		return;
	}

	BeginWaitCursor();

	//int       iItem   = -1;
	BppApp * pApp = new BppApp;
		assert(pApp);
	
	PDlgAppDetail dlgAppDetail(pApp);
	dlgAppDetail.setBNewApp(true);

	EndWaitCursor();

	INT_PTR iRet = dlgAppDetail.DoModal();

	//clone zurückkopieren!
	if (dlgAppDetail.getBDataChanged())
	{
		dlgAppDetail.getCloneObj(pApp);
		if (iRet == IDOK)
		{
			m_pli_App->giveCon().addBsnObj(*pApp);
			
			if (m_pli_App->load(false) == FALSE) 
				SetAppCtrlState(-1);

			MsgBoxOK(IDS_NEW_APP, MB_ICONINFORMATION);
		}
	}

	if(pApp != NULL)
	{
		delete pApp;
		pApp = NULL;
	}

	
	BeginWaitCursor();
	
	m_pli_App->SetFocus();
	
	EndWaitCursor();
}

/**---------------------------------------------------------------------------*/
void PDlgDllAppModRi::OnBtDllAppWork()
{
	BeginWaitCursor();

	int       iItem   = -1;
	BppApp * pApp = m_pli_App->getNextSelectedBsnObj(&iItem);

	if (iItem >= 0)
	{
	PDlgAppDetail dlgAppDetail(pApp);
	dlgAppDetail.setBNewApp(false);

	EndWaitCursor();

	INT_PTR iRet = dlgAppDetail.DoModal();
	
	//clone zurückkopieren!
	if (dlgAppDetail.getBDataChanged())
	{
		dlgAppDetail.getCloneObj(m_pli_App->giveCon().getBsnObj(m_pli_App->getIdx(iItem)));
		if (iRet == IDOK)
			m_pli_App->refresh(iItem);
	}

	BeginWaitCursor();
	
	m_pli_App->SetFocus();
	}
	else
	{
		MsgBoxOK(IDS_SELECT_ITEM);		
	}
	EndWaitCursor();
}

/**---------------------------------------------------------------------------*/
void PDlgDllAppModRi::OnBtDllAppDel()
{
	CBString strRight = RIGHT_DEL_DLL;
	if (!(ppBLoginData::ex().isLegitimated(MODULE_DLL,strRight)))
	{
		MsgBoxOK(IDS_NO_RIGHT);
		return;
	}
	
	BeginWaitCursor();

	int       iItem   = -1;
	BppApp * pApp = m_pli_App->getNextSelectedBsnObj(&iItem);
	
	if (iItem >= 0)
	{
	//Sicherheitsabfrage
	CString csDeleteSure;
	csDeleteSure.Format(CResString::ex().getStrTblText(IDS_DELETE_SURE), 
						pApp->getAppname().c_str());
	INT_PTR iRet = MsgBoxYesNo(csDeleteSure);
		
	if(iRet == IDYES)
	{
		CBString strAppname = pApp->getAppname();
		IPerst::ERet eRet = pApp->delApp();
		if (eRet == IPerst::RET_OK) //Fehler bei Delete auf DB
		{
			m_pli_App->giveCon().clearBsnObj(m_pli_App->getIdx(iItem));
			m_pli_App->load(false);
			
			//auch zugehöriges Appl-Recht löschen!
			if (DelApplRight( strAppname ) != true)
			{
				MsgBoxOK(IDS_ERR_DEL_APPL_RIGHT);
			}
		}
		else
		{
			MsgBoxOK(IDS_ERR_DELETE); 
		}
	}
	
	m_pli_App->SetFocus();
	}
	else
	{
		MsgBoxOK(IDS_SELECT_ITEM);		
	}
	EndWaitCursor();
}

/**---------------------------------------------------------------------------*/
bool PDlgDllAppModRi::DelApplRight(CBString& strAppName)
{
	//AppName = Recht in UADM (ID = 3) in Modul (ID = 2) löschen
	BppAppRight * pRight = new BppAppRight;
	
	//Recht aus ppappmodright löschen
	pRight->setRightname(strAppName);
	pRight->setAppid(UADM_ID);
	pRight->setModid(MODULE_DLL);
	IPerst::ERet eRet = pRight->deleteAppModRight();
	if (eRet != IPerst::RET_OK)
	{
		delete pRight;
		return false;
	}

	//Recht aus ppappright löschen
	eRet = pRight->delFromAppRight();
	if (eRet != IPerst::RET_OK)
	{
		delete pRight;
		return false;
	}

	// Mod 1
	pRight->setModid(MODULE_RIGHTS);
	eRet = pRight->deleteAppModRight();
	if (eRet != IPerst::RET_OK)
	{
		delete pRight;
		return false;
	}

	//Recht aus ppappright löschen
	eRet = pRight->delFromAppRight();
	if (eRet != IPerst::RET_OK)
	{
		delete pRight;
		return false;
	}
	
	delete pRight;
	return true;
}

/**---------------------------------------------------------------------------*/
void PDlgDllAppModRi::OnBtDllModSel()
{
	BeginWaitCursor();
	
	int		  iItem = -1;
	m_sModid = (short) m_pli_Mod->getNextSelectedBsnObj(&iItem)->getModuleid();
	
	if (iItem >= 0)
	{
    //load
	if ( m_Appname.area(3) == BI_APPL )
	{
		m_cb_Category.ShowWindow(SW_SHOW);
		m_txt_Category.ShowWindow(SW_SHOW);
	}
		
	m_pli_Right->giveCon().setAppid(m_sAppid);
	m_pli_Right->giveCon().setModid(m_sModid);
	m_pli_Right->giveCon().setBCorGroupRights(false);
	m_pli_Right->giveCon().setBCorRights(false);
	m_pli_Right->giveCon().setBModRights(true);
	
	if ( m_cb_Category.IsWindowEnabled() )
		m_pli_Right->giveCon().setRightCategory(m_cb_Category.GetCurSel());
	else
		m_pli_Right->giveCon().setRightCategory(RIGHT_CATEGORY_NONE);

	if (m_pli_Right->load() == FALSE) 
		SetRiCtrlState(-1);
	else
		SetRiCtrlState(1);

	//Enable-Ctrls
	m_pli_Right->EnableWindow(true);
	m_bt_Ri_New.EnableWindow(true);
	
	m_pli_Right->SetFocus();
	}
	else
	{
		MsgBoxOK(IDS_SELECT_ITEM);		
	}
	EndWaitCursor();
}

/**---------------------------------------------------------------------------*/
void PDlgDllAppModRi::OnBtDllModNew()
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

	//int       iItem   = -1;
	BppAppMod * pMod = new BppAppMod;
		assert(pMod);

	pMod->setAppid(m_sAppid);
	
	PDlgModDetail dlgModDetail(pMod);
	dlgModDetail.setBNewMod(true);

	EndWaitCursor();
	int		  iItemApp = -1;
	dlgModDetail.setAppname(m_pli_App->getNextSelectedBsnObj(&iItemApp)->getAppname());
	dlgModDetail.setAppid(m_sAppid);

	INT_PTR iRet = dlgModDetail.DoModal();

	//clone zurückkopieren!
	if (dlgModDetail.getBDataChanged())
	{
		dlgModDetail.getCloneObj(pMod);
		if (iRet == IDOK)
		{
			m_pli_Mod->giveCon().addBsnObj(*pMod);
			
			if (m_pli_Mod->load(false) == FALSE) 
				SetModCtrlState(-1);
		}
	}

	if(pMod != NULL)
	{
		delete pMod;
		pMod = NULL;
	}

	
	BeginWaitCursor();
	
	m_pli_Mod->SetFocus();
	
	EndWaitCursor();
}

/**---------------------------------------------------------------------------*/
void PDlgDllAppModRi::OnBtDllModWork()
{
	BeginWaitCursor();

	int       iItem   = -1;
	BppAppMod * pMod = m_pli_Mod->getNextSelectedBsnObj(&iItem);
	pMod->setAppid(m_sAppid);
	if (iItem >= 0)
	{
	PDlgModDetail dlgModDetail(pMod);
	dlgModDetail.setBNewMod(false);

	int		  iItemApp = -1;
	dlgModDetail.setAppname(m_pli_App->getNextSelectedBsnObj(&iItemApp)->getAppname());
	dlgModDetail.setAppid(m_sAppid);

	EndWaitCursor();

	INT_PTR iRet = dlgModDetail.DoModal();
	
	//clone zurückkopieren!
	if (dlgModDetail.getBDataChanged())
	{
		dlgModDetail.getCloneObj(m_pli_Mod->giveCon().getBsnObj(m_pli_Mod->getIdx(iItem)));
		if (iRet == IDOK)
			m_pli_Mod->refresh(iItem);
	}

	BeginWaitCursor();
	
	m_pli_Mod->SetFocus();
	}
	else
	{
		MsgBoxOK(IDS_SELECT_ITEM);		
	}
	EndWaitCursor();
}

/**---------------------------------------------------------------------------*/
void PDlgDllAppModRi::OnBtDllModDel()
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
	BppAppMod * pMod = m_pli_Mod->getNextSelectedBsnObj(&iItem);
	pMod->setAppid(m_sAppid);
	if (iItem >= 0)
	{
	//Sicherheitsabfrage
	CString csDeleteSure;
	csDeleteSure.Format(CResString::ex().getStrTblText(IDS_DELETE_SURE), 
						pMod->getModname().c_str());
	INT_PTR iRet = MsgBoxYesNo(csDeleteSure);
		
	if(iRet == IDYES)
	{
		IPerst::ERet eRet = pMod->deleteModule();
		if (eRet == IPerst::RET_OK) //Fehler bei Delete auf DB
		{
			m_pli_Mod->giveCon().clearBsnObj(m_pli_Mod->getIdx(iItem));
			m_pli_Mod->load(false);
		}
		else
		{
			MsgBoxOK(IDS_ERR_DELETE); 
		}
	}
	
	m_pli_Mod->SetFocus();
	}
	else
	{
		MsgBoxOK(IDS_SELECT_ITEM);		
	}
	EndWaitCursor();
}

/**---------------------------------------------------------------------------*/
void PDlgDllAppModRi::OnBtDllRiNew()
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

	//int       iItem   = -1;
	BppAppRight * pRight = new BppAppRight;
		assert(pRight);

	pRight->setAppid(m_sAppid);
	pRight->setModid(m_sModid);
	
	PDlgRightDetail dlgRightDetail(pRight);
	dlgRightDetail.setBNewRi(true);

	EndWaitCursor();
	int		  iItemApp = -1;
	dlgRightDetail.setAppname(m_pli_App->getNextSelectedBsnObj(&iItemApp)->getAppname());
	int iItemMod = -1;
	dlgRightDetail.setModname(m_pli_Mod->getNextSelectedBsnObj(&iItemMod)->getModname());
	dlgRightDetail.setAppid(m_sAppid);
	dlgRightDetail.setModid(m_sModid);

	INT_PTR iRet = dlgRightDetail.DoModal();

	//clone zurückkopieren!
	if (dlgRightDetail.getBDataChanged())
	{
		dlgRightDetail.getCloneObj(pRight);
		if (iRet == IDOK)
		{
			m_pli_Right->giveCon().addBsnObj(*pRight);
			
			if (m_pli_Right->load(false) == FALSE) 
				SetRiCtrlState(-1);
		}
	}

	if(pRight != NULL)
	{
		delete pRight;
		pRight = NULL;
	}
	
	BeginWaitCursor();
	
	m_pli_Right->SetFocus();
	
	EndWaitCursor();
}

/**---------------------------------------------------------------------------*/
void PDlgDllAppModRi::OnBtDllRiWork()
{
	BeginWaitCursor();

	int       iItem   = -1;
	BppAppRight * pRight = m_pli_Right->getNextSelectedBsnObj(&iItem);
	if (iItem >= 0)
	{
	pRight->setAppid(m_sAppid);
	pRight->setModid(m_sModid);

	PDlgRightDetail dlgRightDetail(pRight);
	dlgRightDetail.setBNewRi(false);

	int	iItemApp = -1;
	dlgRightDetail.setAppname(m_pli_App->getNextSelectedBsnObj(&iItemApp)->getAppname());
	int iItemMod = -1;
	dlgRightDetail.setModname(m_pli_Mod->getNextSelectedBsnObj(&iItemMod)->getModname());
	dlgRightDetail.setAppid(m_sAppid);
	dlgRightDetail.setModid(m_sModid);

	EndWaitCursor();

	INT_PTR iRet = dlgRightDetail.DoModal();
	
	//clone zurückkopieren!
	if (dlgRightDetail.getBDataChanged())
	{
		dlgRightDetail.getCloneObj(m_pli_Right->giveCon().getBsnObj(m_pli_Right->getIdx(iItem)));
		if (iRet == IDOK)
			m_pli_Right->refresh(iItem);
	}

	BeginWaitCursor();
	
	m_pli_Right->SetFocus();
	}
	else
	{
		MsgBoxOK(IDS_SELECT_ITEM);		
	}
	EndWaitCursor();
}

/**---------------------------------------------------------------------------*/
void PDlgDllAppModRi::OnBtDllRiDel()
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
	BppAppRight * pRight = m_pli_Right->getNextSelectedBsnObj(&iItem);
	if (iItem >= 0)
	{
	pRight->setAppid(m_sAppid);
	pRight->setModid(m_sModid);

	//Sicherheitsabfrage
	CString csDeleteSure;
	csDeleteSure.Format(CResString::ex().getStrTblText(IDS_DELETE_SURE), 
						pRight->getRightname().c_str());
	INT_PTR iRet = MsgBoxYesNo(csDeleteSure);
		
	if(iRet == IDYES)
	{
		IPerst::ERet eRet = pRight->delRight();
		if (eRet == IPerst::RET_OK) //Fehler bei Delete auf DB
		{
			m_pli_Right->giveCon().clearBsnObj(m_pli_Right->getIdx(iItem));
			m_pli_Right->load(false);
		}
		else
		{
			MsgBoxOK(IDS_ERR_DELETE); 
		}
	}
	
	m_pli_Right->SetFocus();
	}
	else
	{
		MsgBoxOK(IDS_SELECT_ITEM);		
	}
	EndWaitCursor();
}

/**---------------------------------------------------------------------------*/
void PDlgDllAppModRi::OnBtDllAppExit()
{
	CDialogMultiLang::OnCancel();
}

/**---------------------------------------------------------------------------*/
void PDlgDllAppModRi::OnDblclkAppListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE * pNMIA = (NMITEMACTIVATE *)pNMHDR;

	if (pNMIA->iItem != -1)
	{
		TRACE(__FUNCTION__ ": Item %d\n", pNMIA->iItem);
	
		if (m_bt_App_Sel.IsWindowEnabled())
			OnBtDllAppSel();
	}

	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgDllAppModRi::OnItemchangedAppListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
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

	SetModCtrlState (-1);
	m_pli_Mod->EnableWindow(false);
	m_pli_Mod->clearAllBsnObj();

	SetRiCtrlState     (-1); 
	m_pli_Right->EnableWindow(false);
	m_pli_Right->clearAllBsnObj();
	m_cb_Category.ShowWindow(SW_HIDE);
	m_txt_Category.ShowWindow(SW_HIDE);

	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgDllAppModRi::OnDblclkModListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE * pNMIA = (NMITEMACTIVATE *)pNMHDR;

	if (pNMIA->iItem != -1)
	{
		TRACE(__FUNCTION__ ": Item %d\n", pNMIA->iItem);
	
		if (m_bt_Mod_Sel.IsWindowEnabled())
			OnBtDllModSel();
	}

	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgDllAppModRi::OnItemchangedModListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
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

	SetRiCtrlState     (-1); 
	m_pli_Right->EnableWindow(false);
	m_pli_Right->clearAllBsnObj();
	m_cb_Category.ShowWindow(SW_HIDE);
	m_txt_Category.ShowWindow(SW_HIDE);

	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgDllAppModRi::OnDblclkRightListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE * pNMIA = (NMITEMACTIVATE *)pNMHDR;

	if (pNMIA->iItem != -1)
	{
		TRACE(__FUNCTION__ ": Item %d\n", pNMIA->iItem);
	
		if (m_bt_Ri_Work.IsWindowEnabled())
			OnBtDllRiWork();
	}

	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgDllAppModRi::OnItemchangedRightListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV   = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	static int   iSelect = -1;

	if (    ((pNMLV->uChanged & LVIF_TEXT) == LVIF_TEXT) 
		 && ( pNMLV->iItem                 == iSelect  ))
	{
		TRACE(__FUNCTION__ ": text changed on Item %d\n", iSelect);
		SetRiCtrlState(iSelect);
	}
		 
	if ((pNMLV->uChanged & LVIF_STATE) == LVIF_STATE) 
	{
		if ((pNMLV->uNewState & LVNI_SELECTED) == LVNI_SELECTED)			// wird selektiert
		{
			iSelect = pNMLV->iItem;
			TRACE(__FUNCTION__ ": state changed (LVNI_SELECTED) on Item %d\n", iSelect);

			SetRiCtrlState(iSelect);
		}
		else
		{
			if ((pNMLV->uOldState & LVNI_SELECTED) == LVNI_SELECTED)		// wird deselektiert
				SetRiCtrlState(-1);
		}
	}

	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
BOOL PDlgDllAppModRi::PreTranslateMessage(MSG* pMsg)
{
	assert(pMsg != 0);

	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			// Fokus steht in ModListCtrl
			if (GetFocus() == GetDlgItem(IDC_LI_DLL_MOD))
			{
				if (m_bt_Mod_Sel.IsWindowEnabled())
				{
					OnBtDllModSel();
					return TRUE;
				}
			}
			// Fokus steht in AppListCtrl
			if (GetFocus() == GetDlgItem(IDC_LI_DLL_APP))
			{
				if (m_bt_App_Sel.IsWindowEnabled())
				{
					OnBtDllAppSel();
					return TRUE;
				}
			}
			// Fokus steht in RightListCtrl
			if (GetFocus() == GetDlgItem(IDC_LI_DLL_RIGHTS))
			{
				if (m_bt_Ri_Work.IsWindowEnabled())
				{
					OnBtDllRiWork();
					return TRUE;
				}
			}

			if (GetFocus() == GetDlgItem(IDC_BT_DLL_APP_NEW))
			{
				if (m_bt_App_New.IsWindowEnabled())
				{
					OnBtDllAppNew();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_BT_DLL_APP_WORK))
			{
				if (m_bt_App_Work.IsWindowEnabled())
				{
					OnBtDllAppWork();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_BT_DLL_APP_DEL))
			{
				if (m_bt_App_Del.IsWindowEnabled())
				{
					OnBtDllAppDel();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_BT_DLL_MOD_NEW))
			{
				if (m_bt_Mod_New.IsWindowEnabled())
				{
					OnBtDllModNew();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_BT_DLL_MOD_WORK))
			{
				if (m_bt_Mod_Work.IsWindowEnabled())
				{
					OnBtDllModWork();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_BT_DLL_MOD_DEL))
			{
				if (m_bt_Mod_Del.IsWindowEnabled())
				{
					OnBtDllModDel();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_BT_DLL_RI_NEW))
			{
				if (m_bt_Ri_New.IsWindowEnabled())
				{
					OnBtDllRiNew();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_BT_DLL_RI_WORK))
			{
				if (m_bt_Ri_Work.IsWindowEnabled())
				{
					OnBtDllRiWork();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_BT_DLL_RI_DEL))
			{
				if (m_bt_Ri_Del.IsWindowEnabled())
				{
					OnBtDllRiDel();
					return TRUE;
				}
			}
	
			if (GetFocus() == GetDlgItem(IDC_BT_DLL_APP_EXIT))
			{
				if (m_bt_Exit.IsWindowEnabled())
				{
					OnBtDllAppExit();
					return TRUE;
				}
			}
			return FALSE;

		case VK_ESCAPE:
			if (m_bt_Exit.IsWindowEnabled())
			{
				OnBtDllAppExit();
				return TRUE;
			}
			return FALSE;


		default:
			break;
		}
	} 

	return CDialogMultiLang::PreTranslateMessage(pMsg);
}