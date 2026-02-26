/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                     */
/**   Date   : 05.07.2004                                                     */
/**                                                                           */
/******************************************************************************/

/*----------------------------------------------------------------------------*/
// Includes
/*----------------------------------------------------------------------------*/
#include "StdAfx.h"
#include <BppApp.h>
#include <BConppApp.h>
#include <BppUser.h>
#include <BConppUser.h>
#include <BRightAnalysis.h>
#include <BConRightAnalysis.h>
#include <BppBranchReg.h>
#include <BConppBranchReg.h>
#include "PDlgAppListCtrl.h"
#include "PDlgUserListCtrl.h"
#include "PDlgRiAnalysisListCtrl.h"
#include "PDlgBranchListCtrl.h"
#include "PDlgRegionListCtrl.h"
#include "presentation.h"
#include "PDlgRiAnalysis.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*----------------------------------------------------------------------------*/
// Defines 
/*----------------------------------------------------------------------------*/
#define VS_APPNAME 				 0
#define VS_MODNAME	 			(VS_APPNAME    + 250)
#define VS_USERNAME			    (VS_MODNAME    + 350)
#define VS_SURNAME	 			(VS_USERNAME   + 270)
#define VS_FORENAME				(VS_SURNAME    + 280)
#define VS_AREANAME             (VS_FORENAME   + 250)
#define VS_RIGHTNAME            (VS_AREANAME   + 300)
#define VS_RIGHTDESC            (VS_RIGHTNAME  + 420)
#define VS_COLENDE              (VS_RIGHTDESC  + 480)

#define VS_PRINTCOL_EINZUG           10

#define VS_PRINTLINE_WIDTH         2600

#if (VS_COLENDE > VS_PRINTLINE_WIDTH)
#error VS_COLENDE > VS_PRINTLINE_WIDTH
#endif


/*----------------------------------------------------------------------------*/
// 
/**---------------------------------------------------------------------------*/
IMPLEMENT_DYNCREATE(PDlgRiAnalysis, CDialogMultiLang)

/**---------------------------------------------------------------------------*/
PDlgRiAnalysis::PDlgRiAnalysis()
	: CDialogMultiLang(PDlgRiAnalysis::IDD)
{
   m_pli_User = new PDlgUserListCtrl;
	   assert(m_pli_User);

    m_pli_RiAnaly = new PDlgRiAnalysisListCtrl;
	   assert(m_pli_RiAnaly);

   m_pli_App = new PDlgAppListCtrl;
	   assert(m_pli_App);
	   
	m_pli_Branch = new PDlgBranchListCtrl;
		assert(m_pli_Branch);

	m_pli_Region = new PDlgRegionListCtrl;
		assert(m_pli_Region);

	m_bAppOnly = false;
	m_bUsrOnly = false;
	m_sAppid = 0;
	m_sUserid = 0;
	m_strAppname = "";
	m_strUsername = "";
}

/**---------------------------------------------------------------------------*/
PDlgRiAnalysis::~PDlgRiAnalysis()
{
	if (m_pli_RiAnaly != NULL)
	{
		delete m_pli_RiAnaly;
		m_pli_RiAnaly = NULL;
	}

	if (m_pli_App != NULL)
	{
		delete m_pli_App;
		m_pli_App = NULL;
	}
	
	if (m_pli_User != NULL)
	{
		delete m_pli_User;
		m_pli_User = NULL;
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
}

/**---------------------------------------------------------------------------*/
void PDlgRiAnalysis::DoDataExchange(CDataExchange* pDX)
{
	CDialogMultiLang::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LI_ANALY_APP,	*m_pli_App);
	DDX_Control(pDX, IDC_LI_ANALY_USER, *m_pli_User);
	DDX_Control(pDX, IDC_LI_ANALY_RI,	*m_pli_RiAnaly);
	DDX_Control(pDX, IDC_LC_ANALY_BR,	*m_pli_Branch);	
	DDX_Control(pDX, IDC_LC_ANALY_REG,	*m_pli_Region);
	
	DDX_Control(pDX, IDC_BT_ANALY_SEL_APP, m_bt_Analy_App);
	DDX_Control(pDX, IDC_BT_ANALY_SEL_USR, m_bt_Analy_User);
	DDX_Control(pDX, IDC_BT_ANALY_SEL_BR,  m_bt_Analy_Br);
	DDX_Control(pDX, IDC_BT_ANALY_SEL_REG, m_bt_Analy_Reg);

	DDX_Control(pDX, IDC_BT_ANALY_PRINT, m_bt_Analy_Print);
	DDX_Control(pDX, IDC_BT_ANALY_EXIT, m_bt_Analy_Exit);
	DDX_Control(pDX, IDC_ST_ANALY_DESC, m_txt_Analy_Desc);
	DDX_Control(pDX, IDC_BT_RI_ANALY_SEL_RIGHTS, m_bt_Analy_SelRights);

	DDX_Control(pDX, IDC_CK_ALL_REG,		m_ck_AllReg);
	DDX_Control(pDX, IDC_CK_ALL_BR,			m_ck_AllBr);
	DDX_Control(pDX, IDC_CK_ALL_USR,		m_ck_AllUser);
}

/**---------------------------------------------------------------------------*/
BOOL PDlgRiAnalysis::OnInitDialog()
{
	BeginWaitCursor();

	CDialogMultiLang::OnInitDialog();

	m_pli_App->init();
	
	if (m_pli_App->load() == FALSE) 
		m_bt_Analy_App.EnableWindow(false);
	
	m_bt_Analy_User.EnableWindow(false);
	m_bt_Analy_Br.EnableWindow(false);
	m_bt_Analy_Reg.EnableWindow(false);
	
	m_pli_User->init();
	m_pli_Branch->init();
	m_pli_Region->init();

	m_pli_User->EnableWindow(false);
	m_pli_Branch->EnableWindow(false);
	m_pli_Region->EnableWindow(false);

	m_ck_AllUser.SetCheck(1);
	m_ck_AllBr.SetCheck(1);
	m_ck_AllReg.SetCheck(1);

	m_pli_RiAnaly->init();
	m_pli_RiAnaly->EnableWindow(false);
	
	m_pli_App->SetFocus();

	m_txt_Analy_Desc.SetWindowText("");
	
	//Controls
	SetRiCtrlState(-1);
	m_bt_Analy_SelRights.EnableWindow(false);
		
	EndWaitCursor();

	return FALSE;  // return TRUE unless you set the focus to a control
}

/**---------------------------------------------------------------------------*/
void PDlgRiAnalysis::OnClickedAllBrCk()
{
	if ( m_sAppid == 0 )
		return;

	if ( m_ck_AllBr.GetCheck() > 0 )
	{
		m_ck_AllReg.SetCheck(1);
		if ( m_pli_Region->load() == TRUE )
		{
			m_pli_Region->EnableWindow(true);
			m_ck_AllReg.EnableWindow(true);
			m_bt_Analy_Reg.EnableWindow(true);
		}
		else
		{
			m_ck_AllReg.EnableWindow(false);
			m_bt_Analy_Reg.EnableWindow(false);
		}
	}
	else
	{
		m_pli_Region->EnableWindow(false);
		m_pli_Region->clearAllBsnObj();
		m_ck_AllReg.SetCheck(1);
		m_ck_AllReg.EnableWindow(false);
		m_bt_Analy_Reg.EnableWindow(false);
	}

	if (m_bt_Analy_Br.IsWindowEnabled())
		OnBtAnalySelBr();
}

/**---------------------------------------------------------------------------*/
void PDlgRiAnalysis::OnClickedAllRegCk()
{
	if ( m_sAppid == 0 )
		return;

	if ( m_ck_AllReg.GetCheck() > 0 )
	{
		m_ck_AllBr.SetCheck(1);
		if ( m_pli_Branch->load() == TRUE )
		{
			m_pli_Branch->EnableWindow(true);
			m_ck_AllBr.EnableWindow(true);
			m_bt_Analy_Br.EnableWindow(true);
		}
		else
		{
			m_ck_AllBr.EnableWindow(false);
			m_bt_Analy_Br.EnableWindow(false);
		}
	}
	else
	{
		m_pli_Branch->EnableWindow(false);
		m_pli_Branch->clearAllBsnObj();
		m_ck_AllBr.SetCheck(1);
		m_ck_AllBr.EnableWindow(false);
		m_bt_Analy_Br.EnableWindow(false);
	}

	if (m_bt_Analy_Reg.IsWindowEnabled())
		OnBtAnalySelReg();
}

/**---------------------------------------------------------------------------*/
void PDlgRiAnalysis::OnClickedAllUserCk()
{
	if (m_bt_Analy_User.IsWindowEnabled())
		OnBtAnalySelUsr(); // Gebiete des Users laden
}

/**---------------------------------------------------------------------------*/
void PDlgRiAnalysis::OnBtAnalySelApp()
{
	int iItem = -1;
	m_sAppid = (short) m_pli_App->getNextSelectedBsnObj(&iItem)->getAppid();
	m_strAppname = m_pli_App->getBsnObj(m_pli_App->getIdx(iItem))->getAppname();
	m_pli_User->giveCon().setAppid(m_sAppid);
	m_pli_User->giveCon().setCorUsrGrp(false);
	m_pli_User->giveCon().setGroupid(0);
	m_pli_User->giveCon().setBranchRegid (0);
	
	if (m_pli_User->load() == FALSE) 
	{
		m_bt_Analy_User.EnableWindow(false);
		m_bt_Analy_SelRights.EnableWindow(false);
		m_ck_AllUser.EnableWindow(false);
	}
	else
	{
		m_pli_User->EnableWindow(true);
		m_pli_User->SetFocus();
		m_bt_Analy_SelRights.EnableWindow(true);
		m_ck_AllUser.EnableWindow(true);
		m_bt_Analy_User.EnableWindow(true);		
	}

	//load Branch / Region
	m_pli_Branch->giveCon().setAppid(m_sAppid);
	m_pli_Branch->giveCon().setBSelBranch(true);
	m_pli_Branch->giveCon().setBSelReg(false);
	m_pli_Branch->giveCon().setBSelCor(true);
	m_pli_Branch->giveCon().setUserid(0);

	if ( m_pli_Branch->load() == TRUE )
	{
		m_pli_Branch->EnableWindow(true);
		m_ck_AllBr.EnableWindow(true);
		m_bt_Analy_Br.EnableWindow(true);	
	}
	else
	{
		m_ck_AllBr.EnableWindow(false);
		m_bt_Analy_Br.EnableWindow(false);
	}


	m_pli_Region->giveCon().setBSelBranch(false);
	m_pli_Region->giveCon().setBSelReg(true);
	m_pli_Region->giveCon().setAppid(m_sAppid);
	m_pli_Region->giveCon().setBSelCor(true);
	m_pli_Region->giveCon().setUserid(0);

	
	if ( m_pli_Region->load() == TRUE )
	{
		m_pli_Region->EnableWindow(true);
		m_ck_AllReg.EnableWindow(true);
		m_bt_Analy_Reg.EnableWindow(true);
	}
	else
	{
		m_ck_AllReg.EnableWindow(false);
		m_bt_Analy_Reg.EnableWindow(false);
	}

	m_ck_AllUser.SetCheck(1);
	m_ck_AllBr.SetCheck(1);
	m_ck_AllReg.SetCheck(1);
}

/**---------------------------------------------------------------------------*/
void PDlgRiAnalysis::OnBtAnalySelRights()
{
	if ( m_ck_AllUser.GetCheck() > 0 )
	{
		INT_PTR iRet = MsgBoxYesNo(IDS_ANALYSIS_ALL_USR);
		
		if ( iRet != IDYES )
			return;
	}

	if ( m_ck_AllUser.GetCheck()	> 0		&&
		 m_ck_AllBr.GetCheck()		> 0		&&
		 m_ck_AllReg.GetCheck()		> 0		)
	{
		// alle User, alle Gebiete
		m_bAppOnly = true;
		m_sUserid = 0;
		m_bUsrOnly = false;
		m_strUsername = (LPCTSTR) CResString::ex().getStrTblText(IDS_ALL_APP);

		m_pli_RiAnaly->giveCon().setAppOnly(m_bAppOnly);
		m_pli_RiAnaly->giveCon().setAppid(m_sAppid);
		m_pli_RiAnaly->giveCon().setUsrOnly(m_bUsrOnly);
		m_pli_RiAnaly->giveCon().setUserid(m_sUserid);
		m_pli_RiAnaly->giveCon().setBranchRegid(0);	
		
		if (m_pli_RiAnaly->load() == TRUE)
		{
			SetRiCtrlState(1);
			m_pli_RiAnaly->EnableWindow(true);
			m_pli_RiAnaly->setColumnColor(6, RGB(0,0,0), RGB(255,255,0));
			CString csDesc;
			int iItem = -1;
			csDesc.Format(CResString::ex().getStrTblText(IDS_ANALY_FOR_ALL_USR), 
							m_pli_App->getNextSelectedBsnObj(&iItem)->getAppname().c_str());
			m_txt_Analy_Desc.SetWindowText(csDesc);
		}
	}
	else if ( m_ck_AllUser.GetCheck()	< 1		&&
			  m_ck_AllBr.GetCheck()		> 0		&&
			  m_ck_AllReg.GetCheck()	> 0		)
	{
		// 1 User, alle Gebiete
		int iItem = -1;
		m_sUserid = (short)m_pli_User->getNextSelectedBsnObj(&iItem)->getUserid();
		m_strUsername = m_pli_User->getBsnObj(m_pli_User->getIdx(iItem))->getUsername();
		m_bUsrOnly = true;
		m_bAppOnly = true;
		
		m_pli_RiAnaly->giveCon().setAppOnly(m_bAppOnly);
		m_pli_RiAnaly->giveCon().setAppid(m_sAppid);
		m_pli_RiAnaly->giveCon().setUsrOnly(m_bUsrOnly);
		m_pli_RiAnaly->giveCon().setUserid(m_sUserid);
		m_pli_RiAnaly->giveCon().setBranchRegid(0);	

		
		if (m_pli_RiAnaly->load() == TRUE)
		{
			SetRiCtrlState(1);
			m_pli_RiAnaly->EnableWindow(true);
			m_pli_RiAnaly->setColumnColor(6, RGB(0,0,0), RGB(255,255,0));
			CString csDesc;
			csDesc.Format(CResString::ex().getStrTblText(IDS_ANALY_FOR_SPEC_USR), 
						m_strUsername.c_str());
			m_txt_Analy_Desc.SetWindowText(csDesc);
		}
	}
	else if ( m_ck_AllUser.GetCheck()	> 0		&&
			  (m_ck_AllBr.GetCheck()	< 1		||		  m_ck_AllReg.GetCheck()	< 1	)
			 )
	{
		//alle User, 1 Gebiet
		short sBranchRegid;
		int iItem = -1;
		if ( m_ck_AllBr.GetCheck() < 1 )
		{
			sBranchRegid = m_pli_Branch->getNextSelectedBsnObj(&iItem)->getBranchid();
		}
		else
		{
			sBranchRegid = m_pli_Region->getNextSelectedBsnObj(&iItem)->getRegid();
		}
		
		
		m_bAppOnly = true;
		m_sUserid = 0;
		m_bUsrOnly = false;
		m_strUsername = (LPCTSTR) CResString::ex().getStrTblText(IDS_ALL_APP);

		m_pli_RiAnaly->giveCon().setAppOnly(m_bAppOnly);
		m_pli_RiAnaly->giveCon().setAppid(m_sAppid);
		m_pli_RiAnaly->giveCon().setUsrOnly(m_bUsrOnly);
		m_pli_RiAnaly->giveCon().setUserid(m_sUserid);
		m_pli_RiAnaly->giveCon().setBranchRegid(sBranchRegid);

		if (m_pli_RiAnaly->load() == TRUE)
		{
			SetRiCtrlState(1);
			m_pli_RiAnaly->EnableWindow(true);
			m_pli_RiAnaly->setColumnColor(6, RGB(0,0,0), RGB(255,255,0));
			CString csDesc;
			int iItem = -1;
			csDesc.Format(CResString::ex().getStrTblText(IDS_ANALY_FOR_ALL_USR), 
							m_pli_App->getNextSelectedBsnObj(&iItem)->getAppname().c_str());
			m_txt_Analy_Desc.SetWindowText(csDesc);
		}
	}
	else
	{
		//1 User, 1 Gebiet
		short sBranchRegid;
		int iItem = -1;
		if ( m_ck_AllBr.GetCheck()	< 1 )
		{
			sBranchRegid = m_pli_Branch->getNextSelectedBsnObj(&iItem)->getBranchid();
		}
		else
		{
			sBranchRegid = m_pli_Region->getNextSelectedBsnObj(&iItem)->getRegid();
		}

		iItem = -1;
		m_sUserid = (short)m_pli_User->getNextSelectedBsnObj(&iItem)->getUserid();
		m_strUsername = m_pli_User->getBsnObj(m_pli_User->getIdx(iItem))->getUsername();
		m_bUsrOnly = true;
		m_bAppOnly = true;
		
		m_pli_RiAnaly->giveCon().setAppOnly(m_bAppOnly);
		m_pli_RiAnaly->giveCon().setAppid(m_sAppid);
		m_pli_RiAnaly->giveCon().setUsrOnly(m_bUsrOnly);
		m_pli_RiAnaly->giveCon().setUserid(m_sUserid);
		m_pli_RiAnaly->giveCon().setBranchRegid(sBranchRegid);

		if (m_pli_RiAnaly->load() == TRUE)
		{
			SetRiCtrlState(1);
			m_pli_RiAnaly->EnableWindow(true);
			m_pli_RiAnaly->setColumnColor(6, RGB(0,0,0), RGB(255,255,0));
			CString csDesc;
			csDesc.Format(CResString::ex().getStrTblText(IDS_ANALY_FOR_SPEC_USR), 
						m_strUsername.c_str());
			m_txt_Analy_Desc.SetWindowText(csDesc);
		}
	}
}

/**---------------------------------------------------------------------------*/
void PDlgRiAnalysis::OnBtAnalySelUsr()
{
	// alle Gebiete dieses Users anzeigen
	//load Branch / Region
	if ( m_ck_AllUser.GetCheck() > 0 )
	{
		m_pli_Region->giveCon().setUserid(0);
		m_pli_Branch->giveCon().setUserid(0);
	}
	else
	{
		long lUserid;
		int iItem = -1;
		lUserid = m_pli_User->getNextSelectedBsnObj(&iItem)->getUserid();
		m_pli_Branch->giveCon().setUserid (lUserid);
		m_pli_Region->giveCon().setUserid (lUserid);
	}

	if ( m_pli_Branch->load() == TRUE )
	{
		m_pli_Branch->EnableWindow(true);
		m_ck_AllBr.EnableWindow(true);
		m_bt_Analy_Br.EnableWindow(true);
	}
	else
	{
		m_ck_AllBr.EnableWindow(false);
		m_bt_Analy_Br.EnableWindow(false);
		m_ck_AllBr.SetCheck(1);
	}
		

	if ( m_pli_Region->load() == TRUE )
	{
		m_pli_Region->EnableWindow(true);
		m_ck_AllReg.EnableWindow(true);
		m_bt_Analy_Reg.EnableWindow(true);
	}
	else
	{
		m_bt_Analy_Reg.EnableWindow(false);
		m_ck_AllReg.EnableWindow(false);
		m_ck_AllReg.SetCheck(1);
	}
	
	
	SetRiCtrlState (-1);
	m_pli_RiAnaly->EnableWindow(false);
	m_pli_RiAnaly->clearAllBsnObj();

	m_txt_Analy_Desc.SetWindowText("");
}

/**---------------------------------------------------------------------------*/
void PDlgRiAnalysis::OnBtAnalySelBr()
{
	// alle User in diesem Gebiet anzeigen
	if ( m_ck_AllBr.GetCheck() > 0 )
		m_pli_User->giveCon().setBranchRegid (0);
	else
	{
		short sBranchRegid;
		int iItem = -1;
		sBranchRegid = m_pli_Branch->getNextSelectedBsnObj(&iItem)->getBranchid();
		m_pli_User->giveCon().setBranchRegid (sBranchRegid);
	}
	
	if (m_pli_User->load() == FALSE)
	{
		m_bt_Analy_SelRights.EnableWindow(false);
		m_bt_Analy_User.EnableWindow(false);
		m_ck_AllUser.EnableWindow(false);
	}
	else
	{
		m_pli_User->EnableWindow(true);
		m_bt_Analy_SelRights.EnableWindow(true);
		m_bt_Analy_User.EnableWindow(true);
		m_ck_AllUser.EnableWindow(true);
		m_ck_AllUser.SetCheck(1);
	}
	
	SetRiCtrlState (-1);
	m_pli_RiAnaly->EnableWindow(false);
	m_pli_RiAnaly->clearAllBsnObj();

	m_txt_Analy_Desc.SetWindowText("");
}

/**---------------------------------------------------------------------------*/
void PDlgRiAnalysis::OnBtAnalySelReg()
{
	// alle User in diesem Gebiet anzeigen
	if ( m_ck_AllReg.GetCheck() > 0 )
		m_pli_User->giveCon().setBranchRegid (0);
	else
	{
		short sBranchRegid;
		int iItem = -1;
		sBranchRegid = m_pli_Region->getNextSelectedBsnObj(&iItem)->getRegid();
		m_pli_User->giveCon().setBranchRegid (sBranchRegid);
	}

	if (m_pli_User->load() == FALSE)
	{
		m_bt_Analy_SelRights.EnableWindow(false);
		m_bt_Analy_User.EnableWindow(false);
		m_ck_AllUser.EnableWindow(false);
	}
	else
	{
		m_pli_User->EnableWindow(true);
		m_pli_User->SetFocus();
		m_bt_Analy_SelRights.EnableWindow(true);
		m_ck_AllUser.EnableWindow(true);
		m_ck_AllUser.SetCheck(1);
	}
	
	SetRiCtrlState (-1);
	m_pli_RiAnaly->EnableWindow(false);
	m_pli_RiAnaly->clearAllBsnObj();

	m_txt_Analy_Desc.SetWindowText("");
}

/**---------------------------------------------------------------------------*/
void PDlgRiAnalysis::OnBtAnalyPrint()
{
	if (MsgBoxYesNo(IDS_PRINT_MSG) == IDYES)
	{
		BeginWaitCursor();

		CPrint * pPrint = new CPrint; 
		assert(pPrint);

		EndWaitCursor();

		CString csTitle;
		csTitle.Format(CResString::ex().getStrTblText(IDS_PRINT_TITLE),m_strAppname.c_str(), m_strUsername.c_str());

		if (pPrint->init(GetFont(), 
						csTitle,
						CPrint::LANDSCAPE,
						true) == false)
		{
			delete pPrint;
			return;
		}

		BeginWaitCursor();

		PrintHead(pPrint);									// Kopf

		pPrint->setFooterPageLabel (CResString::ex().getStrTblText(IDS_PRINT_PAGE));
		pPrint->setFooterDateLabel (CResString::ex().getStrTblText(IDS_PRINT_DATE));
		pPrint->storeFooterStandard(0, VS_PRINTLINE_WIDTH);		// Fuß mit Seite und Datum

		PrintRows(pPrint);									// Daten

		delete pPrint;
		EndWaitCursor();
	}
}

/**---------------------------------------------------------------------------*/
void PDlgRiAnalysis::PrintHead        (CPrint * pPrint)
{
	CString csTitle;
	csTitle.Format(CResString::ex().getStrTblText(IDS_PRINT_TITLE),m_strAppname.c_str(), m_strUsername.c_str());

	pPrint->storeHeaderTextLine(csTitle, 0, VS_PRINTLINE_WIDTH, FW_BOLD  , TA_LEFT, true);
	pPrint->storeHeaderTextLine(" "    , 0, VS_PRINTLINE_WIDTH, FW_NORMAL, TA_LEFT, true);

	// Spaltentrennung
	pPrint->storeHeaderVerticLine(VS_MODNAME   - VS_PRINTCOL_EINZUG/2);
	pPrint->storeHeaderVerticLine(VS_USERNAME - VS_PRINTCOL_EINZUG/2);
	pPrint->storeHeaderVerticLine(VS_SURNAME   - VS_PRINTCOL_EINZUG/2);
	pPrint->storeHeaderVerticLine(VS_FORENAME - VS_PRINTCOL_EINZUG/2);
	pPrint->storeHeaderVerticLine(VS_AREANAME - VS_PRINTCOL_EINZUG/2);
	pPrint->storeHeaderVerticLine(VS_RIGHTNAME - VS_PRINTCOL_EINZUG/2);
	pPrint->storeHeaderVerticLine(VS_RIGHTDESC - VS_PRINTCOL_EINZUG/2);


	// Spaltentext
	pPrint->storeHeaderTextLine(CResString::ex().getStrTblText(IDS_APP), VS_APPNAME + 25 ,0,FW_BOLD  );
	pPrint->storeHeaderTextLine(CResString::ex().getStrTblText(IDS_MOD  ), VS_MODNAME + 25, 0, FW_BOLD, TA_LEFT );
	pPrint->storeHeaderTextLine(CResString::ex().getStrTblText(IDS_USERNAME), VS_USERNAME + 25, 0, FW_BOLD, TA_LEFT);
	pPrint->storeHeaderTextLine(CResString::ex().getStrTblText(IDS_SURNAME  ), VS_SURNAME + 25, 0, FW_BOLD, TA_LEFT);
	pPrint->storeHeaderTextLine(CResString::ex().getStrTblText(IDS_FORENAME), VS_FORENAME + 15, 0, FW_BOLD, TA_LEFT);
	pPrint->storeHeaderTextLine(CResString::ex().getStrTblText(IDS_VIEW_PREV_LI_AREA  ), VS_AREANAME + 25, 0, FW_BOLD, TA_LEFT );
	pPrint->storeHeaderTextLine(CResString::ex().getStrTblText(IDS_RIGHT  ), VS_RIGHTNAME + 25, 0, FW_BOLD, TA_LEFT);
	pPrint->storeHeaderTextLine(CResString::ex().getStrTblText(IDS_DESC  ), VS_RIGHTDESC + 25, 0, FW_BOLD, TA_LEFT, true);
	
	pPrint->storeHeaderVerticLine(VS_MODNAME   - VS_PRINTCOL_EINZUG/2);
	pPrint->storeHeaderVerticLine(VS_USERNAME - VS_PRINTCOL_EINZUG/2);
	pPrint->storeHeaderVerticLine(VS_SURNAME   - VS_PRINTCOL_EINZUG/2);
	pPrint->storeHeaderVerticLine(VS_FORENAME - VS_PRINTCOL_EINZUG/2);
	pPrint->storeHeaderVerticLine(VS_AREANAME - VS_PRINTCOL_EINZUG/2);
	pPrint->storeHeaderVerticLine(VS_RIGHTNAME - VS_PRINTCOL_EINZUG/2);
	pPrint->storeHeaderVerticLine(VS_RIGHTDESC - VS_PRINTCOL_EINZUG/2);

	pPrint->storeHeaderHorizLine(0, VS_PRINTLINE_WIDTH, true);
}

/**---------------------------------------------------------------------------*/
void PDlgRiAnalysis::PrintRows        (CPrint * pPrint)
{
	BConRightAnalysis* pCon       = &m_pli_RiAnaly->giveCon();
	BRightAnalysis   * pRiAnaly   = NULL;
	unsigned int	   uiIdx      = 0;
	CBString           csTxt;
	unsigned int	   length;


	while ((pRiAnaly = pCon->getBsnObj(uiIdx++)) != 0)
	{
		
		csTxt = pRiAnaly->getAppname();
		length = 15;
		if (csTxt.length() < length) 
			length = (unsigned int)csTxt.length();
		pPrint->printTextLine(csTxt.substr(0,length).c_str(), VS_APPNAME);

		csTxt = pRiAnaly->getModname();
		length = 20;
		if (csTxt.length() < length) 
			length = (unsigned int)csTxt.length();
		pPrint->printTextLine(csTxt.substr(0,length).c_str(), VS_MODNAME + VS_PRINTCOL_EINZUG);
		
		csTxt = pRiAnaly->getUsername();
		length = 15;
		if (csTxt.length() < length) 
			length = (unsigned int)csTxt.length();
		pPrint->printTextLine(csTxt.substr(0,length).c_str(), VS_USERNAME + VS_PRINTCOL_EINZUG);
		
		csTxt = pRiAnaly->getSurname();
		length = 15;
		if (csTxt.length() < length) 
			length = (unsigned int)csTxt.length();
		pPrint->printTextLine(csTxt.substr(0,length).c_str(), VS_SURNAME + VS_PRINTCOL_EINZUG);
		
		csTxt = pRiAnaly->getForename();
		length = 12;
		if (csTxt.length() < length) 
			length = (unsigned int)csTxt.length();
		pPrint->printTextLine(csTxt.substr(0,length).c_str(), VS_FORENAME + VS_PRINTCOL_EINZUG);
		
		csTxt = pRiAnaly->getAreaname();
		length = 15;
		if (csTxt.length() < length) 
			length = (unsigned int)csTxt.length();
		pPrint->printTextLine(csTxt.substr(0,length).c_str(), VS_AREANAME + VS_PRINTCOL_EINZUG);
		
		csTxt = pRiAnaly->getRightname();
		length = 20;
		if (csTxt.length() < length) 
			length = (unsigned int)csTxt.length();
		pPrint->printTextLine(csTxt.substr(0,length).c_str(), VS_RIGHTNAME + VS_PRINTCOL_EINZUG);
		
		csTxt = pRiAnaly->getRightDesc();
		length = 40;
		if (csTxt.length() < length) 
			length = (unsigned int)csTxt.length();
		pPrint->printTextLine(csTxt.substr(0,length).c_str(), VS_RIGHTDESC + VS_PRINTCOL_EINZUG);

		// Spaltentrennung
		pPrint->printVerticLine(VS_MODNAME   - VS_PRINTCOL_EINZUG/2);
		pPrint->printVerticLine(VS_USERNAME - VS_PRINTCOL_EINZUG/2);
		pPrint->printVerticLine(VS_SURNAME   - VS_PRINTCOL_EINZUG/2);
		pPrint->printVerticLine(VS_FORENAME - VS_PRINTCOL_EINZUG/2);
		pPrint->printVerticLine(VS_AREANAME - VS_PRINTCOL_EINZUG/2);
		pPrint->printVerticLine(VS_RIGHTNAME - VS_PRINTCOL_EINZUG/2);
		pPrint->printVerticLine(VS_RIGHTDESC - VS_PRINTCOL_EINZUG/2);

		pPrint->linefeed();
	}
}

/**---------------------------------------------------------------------------*/
void PDlgRiAnalysis::OnBtAnalyExit()
{
	CDialogMultiLang::OnCancel();
}

/**---------------------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(PDlgRiAnalysis, CDialogMultiLang)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LI_ANALY_APP, OnItemchangedAppListCtrl)
	ON_NOTIFY(NM_DBLCLK, IDC_LI_ANALY_APP, OnDblclkAppListCtrl)
	ON_NOTIFY(NM_DBLCLK, IDC_LI_ANALY_USER, OnDblclkUserListCtrl)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LI_ANALY_USER, OnItemchangedUserListCtrl)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LC_ANALY_BR, OnItemchangedBranchListCtrl)
	ON_NOTIFY(NM_DBLCLK, IDC_LC_ANALY_BR, OnDblclkBranchListCtrl)
	ON_NOTIFY(NM_DBLCLK, IDC_LC_ANALY_REG, OnDblclkRegListCtrl)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LC_ANALY_REG, OnItemchangedRegListCtrl)
	
	ON_BN_CLICKED(IDC_BT_ANALY_SEL_APP, OnBtAnalySelApp)
	ON_BN_CLICKED(IDC_BT_ANALY_SEL_USR, OnBtAnalySelUsr)
	ON_BN_CLICKED(IDC_BT_ANALY_PRINT, OnBtAnalyPrint)
	ON_BN_CLICKED(IDC_BT_ANALY_EXIT, OnBtAnalyExit)
	ON_BN_CLICKED(IDC_BT_RI_ANALY_SEL_RIGHTS, OnBtAnalySelRights)
	ON_BN_CLICKED(IDC_CK_ALL_BR, OnClickedAllBrCk)
	ON_BN_CLICKED(IDC_CK_ALL_REG, OnClickedAllRegCk)
	ON_BN_CLICKED(IDC_CK_ALL_USR, OnClickedAllUserCk)
END_MESSAGE_MAP()


/**---------------------------------------------------------------------------*/
void PDlgRiAnalysis::SetRiCtrlState(const int iItem)
{
	BOOL    bPrint,
		    bAnzeige;


	if (iItem == -1)					// nichts markiert
	{
		bPrint       = (m_pli_RiAnaly->GetItemCount() > 0) ? TRUE : FALSE;		// ListCtrl nicht leer
		bAnzeige     = FALSE;

	}
	else								// Zeile markiert
	{
		bPrint   = TRUE;
		bAnzeige = TRUE;
	}

	// setzen der Controls
	m_bt_Analy_Print.EnableWindow(bAnzeige);
}


/**---------------------------------------------------------------------------*/
void PDlgRiAnalysis::OnDblclkAppListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE * pNMIA = (NMITEMACTIVATE *)pNMHDR;

	if (pNMIA->iItem != -1)
	{
		TRACE(__FUNCTION__ ": Item %d\n", pNMIA->iItem);
	
		if (m_bt_Analy_App.IsWindowEnabled())
			OnBtAnalySelApp();
	}

	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgRiAnalysis::OnItemchangedAppListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV   = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	static int   iSelect = -1;

	if (    ((pNMLV->uChanged & LVIF_TEXT) == LVIF_TEXT) 
		 && ( pNMLV->iItem                 == iSelect  ))
	{
		TRACE(__FUNCTION__ ": text changed on Item %d\n", iSelect);
	}
		 
	if ((pNMLV->uChanged & LVIF_STATE) == LVIF_STATE) 
	{
		if ((pNMLV->uNewState & LVNI_SELECTED) == LVNI_SELECTED)			// wird selektiert
		{
			iSelect = pNMLV->iItem;
			TRACE(__FUNCTION__ ": state changed (LVNI_SELECTED) on Item %d\n", iSelect);
		}
	
	}
	
	m_pli_User->EnableWindow(false);
	m_pli_User->clearAllBsnObj();

	m_pli_Branch->EnableWindow(false);
	m_pli_Branch->clearAllBsnObj();

	m_pli_Region->EnableWindow(false);
	m_pli_Region->clearAllBsnObj();

	SetRiCtrlState (-1);
	m_bt_Analy_SelRights.EnableWindow(false);
	m_pli_RiAnaly->EnableWindow(false);
	m_pli_RiAnaly->clearAllBsnObj();

	m_txt_Analy_Desc.SetWindowText("");

	m_bt_Analy_User.EnableWindow(false);
	m_bt_Analy_Br.EnableWindow(false);
	m_bt_Analy_Reg.EnableWindow(false);
	
	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgRiAnalysis::OnDblclkUserListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE * pNMIA = (NMITEMACTIVATE *)pNMHDR;

	if (pNMIA->iItem != -1)
	{
		TRACE(__FUNCTION__ ": Item %d\n", pNMIA->iItem);
	
		if (m_bt_Analy_User.IsWindowEnabled())
			OnBtAnalySelUsr();
	}

	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgRiAnalysis::OnItemchangedUserListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV   = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	static int   iSelect = -1;

	if (    ((pNMLV->uChanged & LVIF_TEXT) == LVIF_TEXT) 
		 && ( pNMLV->iItem                 == iSelect  ))
	{
		TRACE(__FUNCTION__ ": text changed on Item %d\n", iSelect);
	}
		 
	if ((pNMLV->uChanged & LVIF_STATE) == LVIF_STATE) 
	{
		if ((pNMLV->uNewState & LVNI_SELECTED) == LVNI_SELECTED)			// wird selektiert
		{
			iSelect = pNMLV->iItem;
			TRACE(__FUNCTION__ ": state changed (LVNI_SELECTED) on Item %d\n", iSelect);

		}
	}

	if ( m_ck_AllUser.GetCheck() < 1 )
	{
		SetRiCtrlState (-1);
		m_pli_RiAnaly->EnableWindow(false);
		m_pli_RiAnaly->clearAllBsnObj();

		m_txt_Analy_Desc.SetWindowText("");
	}

	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgRiAnalysis::OnDblclkBranchListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE * pNMIA = (NMITEMACTIVATE *)pNMHDR;

	if (pNMIA->iItem != -1)
	{
		TRACE(__FUNCTION__ ": Item %d\n", pNMIA->iItem);
	
		if (m_bt_Analy_Br.IsWindowEnabled())
			OnBtAnalySelBr();
	}

	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgRiAnalysis::OnItemchangedBranchListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV   = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	static int   iSelect = -1;

	if (    ((pNMLV->uChanged & LVIF_TEXT) == LVIF_TEXT) 
		 && ( pNMLV->iItem                 == iSelect  ))
	{
		TRACE(__FUNCTION__ ": text changed on Item %d\n", iSelect);
	}
		 
	if ((pNMLV->uChanged & LVIF_STATE) == LVIF_STATE) 
	{
		if ((pNMLV->uNewState & LVNI_SELECTED) == LVNI_SELECTED)			// wird selektiert
		{
			iSelect = pNMLV->iItem;
			TRACE(__FUNCTION__ ": state changed (LVNI_SELECTED) on Item %d\n", iSelect);

		}
	}

	if ( m_ck_AllBr.GetCheck() < 1 )
	{
		SetRiCtrlState (-1);
	
		m_pli_RiAnaly->EnableWindow(false);
		m_pli_RiAnaly->clearAllBsnObj();

		m_txt_Analy_Desc.SetWindowText("");
	}

	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgRiAnalysis::OnDblclkRegListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE * pNMIA = (NMITEMACTIVATE *)pNMHDR;

	if (pNMIA->iItem != -1)
	{
		TRACE(__FUNCTION__ ": Item %d\n", pNMIA->iItem);
	
		if (m_bt_Analy_Reg.IsWindowEnabled())
			OnBtAnalySelReg();
	}

	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgRiAnalysis::OnItemchangedRegListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV   = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	static int   iSelect = -1;

	if (    ((pNMLV->uChanged & LVIF_TEXT) == LVIF_TEXT) 
		 && ( pNMLV->iItem                 == iSelect  ))
	{
		TRACE(__FUNCTION__ ": text changed on Item %d\n", iSelect);
	}
		 
	if ((pNMLV->uChanged & LVIF_STATE) == LVIF_STATE) 
	{
		if ((pNMLV->uNewState & LVNI_SELECTED) == LVNI_SELECTED)			// wird selektiert
		{
			iSelect = pNMLV->iItem;
			TRACE(__FUNCTION__ ": state changed (LVNI_SELECTED) on Item %d\n", iSelect);

		}
	}

	if ( m_ck_AllReg.GetCheck() < 1 )
	{
		SetRiCtrlState (-1);
		m_pli_RiAnaly->EnableWindow(false);
		m_pli_RiAnaly->clearAllBsnObj();

		m_txt_Analy_Desc.SetWindowText("");
	}

	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
BOOL PDlgRiAnalysis::PreTranslateMessage(MSG* pMsg)
{
	assert(pMsg != 0);

	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			
			if (GetFocus() == GetDlgItem(IDC_LI_ANALY_APP))
			{
				if (m_bt_Analy_App.IsWindowEnabled())
				{
					OnBtAnalySelApp();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_LI_ANALY_USER))
			{
				if (m_bt_Analy_User.IsWindowEnabled())
				{
					OnBtAnalySelUsr();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_LC_ANALY_BR))
			{
				if (m_bt_Analy_Br.IsWindowEnabled())
				{
					OnBtAnalySelBr();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_LC_ANALY_REG))
			{
				if (m_bt_Analy_Reg.IsWindowEnabled())
				{
					OnBtAnalySelReg();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_BT_ANALY_PRINT))
			{
				if (m_bt_Analy_Print.IsWindowEnabled())
				{
					OnBtAnalyPrint();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_BT_ANALY_EXIT))
			{
				if (m_bt_Analy_Exit.IsWindowEnabled())
				{
					OnBtAnalyExit();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_BT_RI_ANALY_SEL_RIGHTS))
			{
				if (m_bt_Analy_SelRights.IsWindowEnabled())
				{
					OnBtAnalySelRights();
					return TRUE;
				}
			}
			return FALSE;

		case VK_ESCAPE:
			if (m_bt_Analy_Exit.IsWindowEnabled())
			{
				OnBtAnalyExit();
				return TRUE;
			}
			return FALSE;


		default:
			break;
		}
		
		// Suche in User-Liste
		if (GetFocus() == GetDlgItem(IDC_LI_ANALY_USER))
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


