/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                     */
/**   Date   : 30.06.2004                                                     */
/**                                                                           */
/******************************************************************************/

/*----------------------------------------------------------------------------*/
// Includes
/*----------------------------------------------------------------------------*/
#include "StdAfx.h"
#include <BppApp.h>
#include <BConppApp.h>
#include <BppBranchReg.h>
#include <BConppBranchReg.h>
#include "PDlgAppListCtrl.h"
#include "PDlgBranchListCtrl.h"
#include "PDlgRegionListCtrl.h"
#include "PDlgBrRegDetail.h"
#include "PDlgCorBrReg.h"
#include "presentation.h"
#include "PDlgDllBrReg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include <vector>
using std::vector;
class C_VecItem : public vector <int>{};

/*----------------------------------------------------------------------------*/
// 
/**---------------------------------------------------------------------------*/
IMPLEMENT_DYNCREATE(PDlgDllBrReg, CDialogMultiLang)

/**---------------------------------------------------------------------------*/
PDlgDllBrReg::PDlgDllBrReg()
	: CDialogMultiLang(PDlgDllBrReg::IDD)
{

   m_pli_RegionALL = new PDlgRegionListCtrl;
	   assert(m_pli_RegionALL);

    m_pli_BranchALL = new PDlgBranchListCtrl;
	   assert(m_pli_BranchALL);

	  m_pli_RegionCOR = new PDlgRegionListCtrl;
	   assert(m_pli_RegionCOR);

    m_pli_BranchCOR = new PDlgBranchListCtrl;
	   assert(m_pli_BranchCOR);

   m_pli_App = new PDlgAppListCtrl;
	   assert(m_pli_App);

	   m_bLoadNew = false;
	
}

/**---------------------------------------------------------------------------*/
PDlgDllBrReg::~PDlgDllBrReg()
{
	if (m_pli_RegionALL != NULL)
	{
		delete m_pli_RegionALL;
		m_pli_RegionALL = NULL;
	}

	if (m_pli_App != NULL)
	{
		delete m_pli_App;
		m_pli_App = NULL;
	}
	
	if (m_pli_BranchALL != NULL)
	{
		delete m_pli_BranchALL;
		m_pli_BranchALL = NULL;
	}

	if (m_pli_BranchCOR != NULL)
	{
		delete m_pli_BranchCOR;
		m_pli_BranchCOR = NULL;
	}
	
	if (m_pli_RegionCOR != NULL)
	{
		delete m_pli_RegionCOR;
		m_pli_RegionCOR = NULL;
	}
	
}

/**---------------------------------------------------------------------------*/
void PDlgDllBrReg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMultiLang::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LI_BREG_APP,	  *m_pli_App);
	DDX_Control(pDX, IDC_LI_BRREG_ALL_REG,*m_pli_RegionALL);
	DDX_Control(pDX, IDC_LI_BREG_COR_REG, *m_pli_RegionCOR);
	DDX_Control(pDX, IDC_LI_BRREG_ALL_BR, *m_pli_BranchALL);
	DDX_Control(pDX, IDC_LI_BRREG_COR_BR, *m_pli_BranchCOR);
	DDX_Control(pDX, IDC_BT_BRREG_APP, m_bt_Sel_App);
	DDX_Control(pDX, IDC_BT_BRREG_COR_REG, m_bt_CorReg);
	DDX_Control(pDX, IDC_BT_BRREG_ERASE_REG, m_bt_EraseReg);
	DDX_Control(pDX, IDC_BT_BRREG_NEW_REG, m_bt_Reg_New);
	DDX_Control(pDX, IDC_BT_BRREG_WORK_REG, m_bt_Reg_Work);
	DDX_Control(pDX, IDC_BT_BRREG_DEL_REG, m_bt_Reg_Del);
	DDX_Control(pDX, IDC_BT_BRREG_COR, m_bt_Cor_RegBr);
	DDX_Control(pDX, IDC_BT_BRREG_COR_BR, m_bt_Br_Cor);
	DDX_Control(pDX, IDC_BT_BRREG_ERASE_BR, m_bt_Br_Erase);
	DDX_Control(pDX, IDC_BT_BRREG_NEW_BR, m_bt_Br_New);
	DDX_Control(pDX, IDC_BT_BRREG_WORK_BR, m_bt_Br_Work);
	DDX_Control(pDX, IDC_BT_BRREG_DEL_BR, m_bt_Br_Del);
	DDX_Control(pDX, IDC_BT_BRREG_EXIT, m_bt_BrReg_Exit);
	DDX_Control(pDX, IDC_BT_WORK_COR_REG, m_bt_Work_CorReg);
	DDX_Control(pDX, IDC_BT_WORK_COR_BR, m_bt_Work_CorBr);
}

/**---------------------------------------------------------------------------*/
BOOL PDlgDllBrReg::OnInitDialog()
{
	BeginWaitCursor();

	CDialogMultiLang::OnInitDialog();

	m_pli_App->init();
	if (m_pli_App->load() == FALSE) 
		m_bt_Sel_App.EnableWindow(false);
	
	m_pli_RegionALL->init();
	m_pli_RegionCOR->init();
	m_pli_BranchALL->init();
	m_pli_BranchCOR->init();

	m_pli_App->SetFocus();
	
	//Controls
	SetBranchCtrlState(-1);
	SetRegionCtrlState(-1);
	SetCORBranchCtrlState(-1);
	SetCORRegionCtrlState(-1);
		
	EndWaitCursor();

	return FALSE;  // return TRUE unless you set the focus to a control
}

/**---------------------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(PDlgDllBrReg, CDialogMultiLang)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LI_BREG_APP, OnItemchangedAppListCtrl)
	ON_NOTIFY(NM_DBLCLK, IDC_LI_BREG_APP, OnDblclkAppListCtrl)
	ON_NOTIFY(NM_DBLCLK, IDC_LI_BRREG_ALL_REG, OnDblclkRegALLListCtrl)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LI_BRREG_ALL_REG, OnItemchangedRegALLListCtrl)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LI_BREG_COR_REG, OnItemchangedRegCORListCtrl)
	ON_NOTIFY(NM_DBLCLK, IDC_LI_BRREG_ALL_BR, OnDblclkBrALLListCtrl)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LI_BRREG_ALL_BR, OnItemchangedBrALLListCtrl)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LI_BRREG_COR_BR, OnItemchangedBrCORListCtrl)
	ON_NOTIFY(NM_DBLCLK, IDC_LI_BRREG_COR_BR, OnDblclkBrCORListCtrl)
	ON_NOTIFY(NM_DBLCLK, IDC_LI_BREG_COR_REG, OnDblclkRegCORListCtrl)
	ON_BN_CLICKED(IDC_BT_BRREG_APP, OnBtBrregApp)
	ON_BN_CLICKED(IDC_BT_BRREG_COR_REG, OnBtBrregCorReg)
	ON_BN_CLICKED(IDC_BT_BRREG_ERASE_REG, OnBtBrregEraseReg)
	ON_BN_CLICKED(IDC_BT_BRREG_NEW_REG, OnBtBrregNewReg)
	ON_BN_CLICKED(IDC_BT_BRREG_WORK_REG, OnBtBrregWorkReg)
	ON_BN_CLICKED(IDC_BT_BRREG_DEL_REG, OnBtBrregDelReg)
	ON_BN_CLICKED(IDC_BT_BRREG_COR, OnBtBrRegCor)
	ON_BN_CLICKED(IDC_BT_BRREG_COR_BR, OnBtBrregCorBr)
	ON_BN_CLICKED(IDC_BT_BRREG_ERASE_BR, OnBtBrregEraseBr)
	ON_BN_CLICKED(IDC_BT_BRREG_NEW_BR, OnBtBrregNewBr)
	ON_BN_CLICKED(IDC_BT_BRREG_WORK_BR, OnBtBrregWorkBr)
	ON_BN_CLICKED(IDC_BT_BRREG_DEL_BR, OnBtBrregDelBr)
	ON_BN_CLICKED(IDC_BT_BRREG_EXIT, OnBtBrregExit)
	ON_NOTIFY(NM_SETFOCUS, IDC_LI_BREG_COR_REG, OnNMSetfocusCorReg)
	ON_NOTIFY(NM_SETFOCUS, IDC_LI_BRREG_ALL_REG, OnNMSetfocusAllReg)
	ON_NOTIFY(NM_SETFOCUS, IDC_LI_BRREG_COR_BR, OnNMSetfocusCorBr)
	ON_NOTIFY(NM_SETFOCUS, IDC_LI_BRREG_ALL_BR, OnNMSetfocusAllBr)
	ON_NOTIFY(NM_CLICK, IDC_LI_BREG_COR_REG, OnNMClickCorReg)
	ON_NOTIFY(NM_CLICK, IDC_LI_BRREG_ALL_REG, OnNMClickAllReg)
	ON_NOTIFY(NM_CLICK, IDC_LI_BRREG_COR_BR, OnNMClickCorBr)
	ON_NOTIFY(NM_CLICK, IDC_LI_BRREG_ALL_BR, OnNMClickAllBr)
	ON_BN_CLICKED(IDC_BT_WORK_COR_REG, OnBtWorkCorReg)
	ON_BN_CLICKED(IDC_BT_WORK_COR_BR, OnBtWorkCorBr)
END_MESSAGE_MAP()


/**---------------------------------------------------------------------------*/
void PDlgDllBrReg::OnBtBrregApp()
{
	BeginWaitCursor();
	
	int		  iItemApp = -1;
	m_sAppid = (short) m_pli_App->getNextSelectedBsnObj(&iItemApp)->getAppid();
	
	if(iItemApp >= 0)
	{
    //load
	m_pli_RegionALL->giveCon().setAppid(m_sAppid);
	m_pli_RegionALL->giveCon().setBSelReg(true);
	m_pli_RegionALL->giveCon().setBSelCor(false);
	m_pli_RegionALL->giveCon().setBSelBranch(false);
	if (m_pli_RegionALL->load() == FALSE) 
		SetRegionCtrlState(-1);
	else
		SetRegionCtrlState(1);
	

	m_pli_RegionCOR->giveCon().setAppid(m_sAppid);
	m_pli_RegionCOR->giveCon().setBSelReg(true);
	m_pli_RegionCOR->giveCon().setBSelCor(true);
	m_pli_RegionCOR->giveCon().setBSelBranch(false);
	if (m_pli_RegionCOR->load() == FALSE) 
		SetCORRegionCtrlState(-1);
	else
		SetCORRegionCtrlState(1);

	m_pli_BranchALL->giveCon().setAppid(m_sAppid);
	m_pli_BranchALL->giveCon().setBSelReg(false);
	m_pli_BranchALL->giveCon().setBSelCor(false);
	m_pli_BranchALL->giveCon().setBSelBranch(true);
	if (m_pli_BranchALL->load() == FALSE) 
		SetBranchCtrlState(-1);
	
	m_pli_BranchCOR->giveCon().setAppid(m_sAppid);
	m_pli_BranchCOR->giveCon().setBSelReg(false);
	m_pli_BranchCOR->giveCon().setBSelCor(true);
	m_pli_BranchCOR->giveCon().setBSelBranch(true);
	if (m_pli_BranchCOR->load() == FALSE) 
		SetCORBranchCtrlState(-1);


	//Enable-Ctrls
	m_bt_Reg_New.EnableWindow(true);
	m_bt_Br_New.EnableWindow(true);

	m_pli_RegionALL->EnableWindow(true);
	m_pli_RegionCOR->EnableWindow(true);
	m_pli_BranchALL->EnableWindow(true);
	m_pli_BranchCOR->EnableWindow(true);
	
	m_pli_RegionALL->SetFocus();
	}
	else
	{
		MsgBoxOK(IDS_SELECT_ITEM);		
	}

	EndWaitCursor();
}

/**---------------------------------------------------------------------------*/
void PDlgDllBrReg::OnBtBrregCorReg()
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
	
	//Regionen zuordnen
	BeginWaitCursor();

	POSITION pos = m_pli_RegionALL->GetFirstSelectedItemPosition();
	BppBranchReg * pBrReg;

	C_VecItem * pVItems = new C_VecItem();
	int sCount = 0;
	int iItem = 0;

	while(pos) //Mehrfachauswahl möglich
	{
		sCount++;
		iItem =  m_pli_RegionALL->GetNextSelectedItem(pos);
		pVItems->push_back(iItem);
	}
	
	while (sCount > 0)
	{
		iItem = pVItems->back();

		if (iItem >= 0)
		{

		pBrReg = m_pli_RegionALL->giveCon().getBsnObj(m_pli_RegionALL->getIdx(iItem));
		pBrReg->setCorAppid(m_sAppid);
		pBrReg->setCorBranchid(0);
		pBrReg->setCorRegid(pBrReg->getRegid());
		
		
		IPerst::ERet eRet = pBrReg->insertAppBranchReg();
		if (eRet == IPerst::RET_OK) //Fehler bei Insert in DB
		{
			m_pli_RegionCOR->giveCon().addBsnObj(*pBrReg);
			m_pli_RegionALL->giveCon().clearBsnObj(m_pli_RegionALL->getIdx(iItem));
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

	m_pli_RegionCOR->load(m_bLoadNew );
	m_pli_RegionALL->load(m_bLoadNew );
	
	m_pli_RegionALL->SetFocus();

	delete pVItems;

	EndWaitCursor();
}

/**---------------------------------------------------------------------------*/
void PDlgDllBrReg::OnBtBrregEraseReg()
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
	
	// Zuordnung entfernen
	BeginWaitCursor();

	BppBranchReg * pBrReg;
	POSITION pos = m_pli_RegionCOR->GetFirstSelectedItemPosition();
	
	int iItem = -1;
	
	iItem =  m_pli_RegionCOR->GetNextSelectedItem(pos);

	if (iItem >= 0)
	{
		pBrReg = m_pli_RegionCOR->giveCon().getBsnObj(m_pli_RegionCOR->getIdx(iItem));
		pBrReg->setCorAppid(m_sAppid);
		pBrReg->setCorRegid(pBrReg->getRegid());
		
		CString csDeleteSure;
		csDeleteSure.Format(CResString::ex().getStrTblText(IDS_DELETE_SURE), 
							pBrReg->getRegname().c_str());
		INT_PTR iRet = MsgBoxYesNo(csDeleteSure); 
	
		if(iRet == IDYES)
		{
			IPerst::ERet eRet = pBrReg->deleteCORReg();
			if (eRet == IPerst::RET_OK) 
			{
				m_pli_RegionALL->giveCon().addBsnObj(*pBrReg);
				m_pli_RegionCOR->giveCon().clearBsnObj(m_pli_RegionCOR->getIdx(iItem));
			}
			else
			{
				MsgBoxOK(IDS_ERR_DELETE);
			}
	
			m_pli_RegionCOR->load(false);
			m_pli_RegionALL->load(false);

			//Filialen neu laden!
			m_pli_BranchALL->giveCon().setAppid(m_sAppid);
			m_pli_BranchALL->giveCon().setBSelReg(false);
			m_pli_BranchALL->giveCon().setBSelCor(false);
			m_pli_BranchALL->giveCon().setBSelBranch(true);
			if (m_pli_BranchALL->load() == FALSE) 
				SetBranchCtrlState(-1);
			else
				SetBranchCtrlState(1);

			m_pli_BranchCOR->giveCon().setAppid(m_sAppid);
			m_pli_BranchCOR->giveCon().setBSelReg(false);
			m_pli_BranchCOR->giveCon().setBSelCor(true);
			m_pli_BranchCOR->giveCon().setBSelBranch(true);
			m_pli_BranchCOR->load();
		}
	
		m_pli_RegionCOR->SetFocus();
	}
	else
	{
		MsgBoxOK(IDS_SELECT_ITEM);		
	}
		
	EndWaitCursor();
	
}

/**---------------------------------------------------------------------------*/
void PDlgDllBrReg::OnBtBrregNewReg()
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
	BppBranchReg * pBrReg = new BppBranchReg;
		assert(pBrReg);
	
	PDlgBrRegDetail dlgBrRegDetail(pBrReg);
	dlgBrRegDetail.setBNewBrReg(true);
	dlgBrRegDetail.setBBranch(false);

	EndWaitCursor();

	INT_PTR iRet = dlgBrRegDetail.DoModal();

	//clone zurückkopieren!
	if (dlgBrRegDetail.getBDataChanged())
	{
		dlgBrRegDetail.getCloneObj(pBrReg);
		if (iRet == IDOK)
		{
			m_pli_RegionALL->giveCon().addBsnObj(*pBrReg);
			
			if (m_pli_RegionALL->load(false) == FALSE) 
				SetRegionCtrlState(-1);
		}
	}

	if(pBrReg != NULL)
	{
		delete pBrReg;
		pBrReg = NULL;
	}
	
	BeginWaitCursor();
	
	m_pli_RegionALL->SetFocus();
	
	EndWaitCursor();
}

/**---------------------------------------------------------------------------*/
void PDlgDllBrReg::OnBtBrregWorkReg()
{
	BeginWaitCursor();

	int       iItem   = -1;
	BppBranchReg * pBrReg = m_pli_RegionALL->getNextSelectedBsnObj(&iItem);

	if (iItem >= 0)
	{
	PDlgBrRegDetail dlgBrRegDetail(pBrReg);
	dlgBrRegDetail.setBNewBrReg(false);
	dlgBrRegDetail.setBBranch(false);

	EndWaitCursor();

	INT_PTR iRet = dlgBrRegDetail.DoModal();
	
	//clone zurückkopieren!
	if (dlgBrRegDetail.getBDataChanged())
	{
		dlgBrRegDetail.getCloneObj(m_pli_RegionALL->giveCon().getBsnObj(m_pli_RegionALL->getIdx(iItem)));
		if (iRet == IDOK)
			m_pli_RegionALL->refresh(iItem);
	}

	BeginWaitCursor();
	
	m_pli_RegionALL->SetFocus();
	}
	else
	{
		MsgBoxOK(IDS_SELECT_ITEM);		
	}
	EndWaitCursor();
}

/**---------------------------------------------------------------------------*/
void PDlgDllBrReg::OnBtBrregDelReg()
{
	CBString strRight = RIGHT_DEL_DLL;
	if (!(ppBLoginData::ex().isLegitimated(MODULE_DLL,strRight)))
	{
		MsgBoxOK(IDS_NO_RIGHT);
		return;
	}
	
	BeginWaitCursor();

	int       iItem   = -1;
	BppBranchReg * pBrReg = m_pli_RegionALL->getNextSelectedBsnObj(&iItem);

	if (iItem >= 0)
	{
	//Sicherheitsabfrage
	CString csDeleteSure;
	csDeleteSure.Format(CResString::ex().getStrTblText(IDS_DELETE_SURE_ALL_APP), 
						pBrReg->getRegname().c_str());
	INT_PTR iRet = MsgBoxYesNo(csDeleteSure);
		
	if(iRet == IDYES)
	{
		IPerst::ERet eRet = pBrReg->delRegion();
		if (eRet == IPerst::RET_OK) //Fehler bei Delete auf DB
		{
			m_pli_RegionALL->giveCon().clearBsnObj(m_pli_RegionALL->getIdx(iItem));
			m_pli_RegionALL->load(false);
		}
		else
		{
			MsgBoxOK(IDS_ERR_DELETE); 
		}
	}
	
	m_pli_RegionALL->SetFocus();
	}
	else
	{
		MsgBoxOK(IDS_SELECT_ITEM);		
	}
	EndWaitCursor();
}

/**---------------------------------------------------------------------------*/
void PDlgDllBrReg::OnBtBrRegCor()
{
	//ZUORDNUNG VON FILIALEN ZU REGIONEN
	BeginWaitCursor();

	int       iItem   = -1;
	BppBranchReg * pBrReg = m_pli_RegionCOR->getNextSelectedBsnObj(&iItem);
	
	if (iItem >= 0)
	{

	pBrReg->setCorAppid(m_sAppid);
	pBrReg->setAppid(m_sAppid);
	pBrReg->setCorRegid(pBrReg->getRegid());

	PDlgCorBrReg dlgCorBrReg(pBrReg);
	
	int		  iItemApp = -1;
	CBString strAppname = m_pli_App->getNextSelectedBsnObj(&iItemApp)->getAppname();
	dlgCorBrReg.setAppname(strAppname);
	
	EndWaitCursor();

	INT_PTR iRet = dlgCorBrReg.DoModal();
	
	BeginWaitCursor();
	
	m_pli_RegionCOR->SetFocus();
	}
	else
	{
		MsgBoxOK(IDS_SELECT_ITEM);		
	}
	EndWaitCursor();
}

/**---------------------------------------------------------------------------*/
void PDlgDllBrReg::OnBtBrregCorBr()
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
	
	//Filialen zuordnen
	BeginWaitCursor();

	POSITION pos = m_pli_BranchALL->GetFirstSelectedItemPosition();
	BppBranchReg * pBrReg;

	C_VecItem * pVItems = new C_VecItem();
	int sCount = 0;
	int iItem = 0;

	while(pos) //Mehrfachauswahl möglich
	{
		sCount++;
		iItem =  m_pli_BranchALL->GetNextSelectedItem(pos);
		pVItems->push_back(iItem);
	}
	
	while (sCount > 0)
	{
		iItem = pVItems->back();
		
		if (iItem >= 0)
		{
		pBrReg = m_pli_BranchALL->giveCon().getBsnObj(m_pli_BranchALL->getIdx(iItem));
		pBrReg->setCorAppid(m_sAppid);
		pBrReg->setCorBranchid(pBrReg->getBranchid());
		pBrReg->setCorRegid(0);
		
		IPerst::ERet eRet = pBrReg->insertAppBranchReg();
		if (eRet == IPerst::RET_OK) //Fehler bei Insert in DB
		{
			m_pli_BranchCOR->giveCon().addBsnObj(*pBrReg);
			m_pli_BranchALL->giveCon().clearBsnObj(m_pli_BranchALL->getIdx(iItem));
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

	m_pli_BranchCOR->load(m_bLoadNew );
	m_pli_BranchALL->load(m_bLoadNew );
	
	m_pli_BranchALL->SetFocus();

	delete pVItems;

	EndWaitCursor();
}

/**---------------------------------------------------------------------------*/
void PDlgDllBrReg::OnBtBrregEraseBr()
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

// Zuordnung entfernen
	BeginWaitCursor();

	BppBranchReg * pBrReg;
	POSITION pos = m_pli_BranchCOR->GetFirstSelectedItemPosition();
	
	int iItem = -1;
	
	iItem =  m_pli_BranchCOR->GetNextSelectedItem(pos);

	if (iItem >= 0)
	{
		pBrReg = m_pli_BranchCOR->giveCon().getBsnObj(m_pli_BranchCOR->getIdx(iItem));
		pBrReg->setCorAppid(m_sAppid);
		pBrReg->setCorBranchid(pBrReg->getBranchid());
		pBrReg->setCorRegid(0);
		
		CString csDeleteSure;
		csDeleteSure.Format(CResString::ex().getStrTblText(IDS_DELETE_SURE), 
							pBrReg->getBranchname().c_str());
		INT_PTR iRet = MsgBoxYesNo(csDeleteSure); 
	
		if(iRet == IDYES)
		{
			IPerst::ERet eRet = pBrReg->deleteAppBranchReg();
			if (eRet == IPerst::RET_OK) 
			{
				m_pli_BranchALL->giveCon().addBsnObj(*pBrReg);
				m_pli_BranchCOR->giveCon().clearBsnObj(m_pli_BranchCOR->getIdx(iItem));
			}
			else
			{
				MsgBoxOK(IDS_ERR_DELETE);
			}
	
			m_pli_BranchCOR->load(false);
			m_pli_BranchALL->load(false);
		}
	
		m_pli_BranchCOR->SetFocus();
	}
	else
	{
		MsgBoxOK(IDS_SELECT_ITEM);		
	}
	
	EndWaitCursor();
	
}

/**---------------------------------------------------------------------------*/
void PDlgDllBrReg::OnBtBrregNewBr()
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
	BppBranchReg * pBrReg = new BppBranchReg;
		assert(pBrReg);
	
	PDlgBrRegDetail dlgBrRegDetail(pBrReg);
	dlgBrRegDetail.setBNewBrReg(true);
	dlgBrRegDetail.setBBranch(true);

	EndWaitCursor();

	INT_PTR iRet = dlgBrRegDetail.DoModal();

	//clone zurückkopieren!
	if (dlgBrRegDetail.getBDataChanged())
	{
		dlgBrRegDetail.getCloneObj(pBrReg);
		if (iRet == IDOK)
		{
			m_pli_BranchALL->giveCon().addBsnObj(*pBrReg);
			
			if (m_pli_BranchALL->load(false) == FALSE) 
				SetBranchCtrlState(-1);
		}
	}

	if(pBrReg != NULL)
	{
		delete pBrReg;
		pBrReg = NULL;
	}
	
	BeginWaitCursor();
	
	m_pli_BranchALL->SetFocus();
	
	EndWaitCursor();
}

/**---------------------------------------------------------------------------*/
void PDlgDllBrReg::OnBtBrregWorkBr()
{
	BeginWaitCursor();

	int       iItem   = -1;
	BppBranchReg * pBrReg = m_pli_BranchALL->getNextSelectedBsnObj(&iItem);

	if (iItem >= 0)
	{
	PDlgBrRegDetail dlgBrRegDetail(pBrReg);
	dlgBrRegDetail.setBNewBrReg(false);
	dlgBrRegDetail.setBBranch(true);

	EndWaitCursor();

	INT_PTR iRet = dlgBrRegDetail.DoModal();
	
	//clone zurückkopieren!
	if (dlgBrRegDetail.getBDataChanged())
	{
		dlgBrRegDetail.getCloneObj(m_pli_BranchALL->giveCon().getBsnObj(m_pli_BranchALL->getIdx(iItem)));
		if (iRet == IDOK)
			m_pli_BranchALL->refresh(iItem);
	}

	BeginWaitCursor();
	
	m_pli_BranchALL->SetFocus();
	}
	else
	{
		MsgBoxOK(IDS_SELECT_ITEM);		
	}
	EndWaitCursor();
}

/**---------------------------------------------------------------------------*/
void PDlgDllBrReg::OnBtBrregDelBr()
{
	CBString strRight = RIGHT_DEL_DLL;
	if (!(ppBLoginData::ex().isLegitimated(MODULE_DLL,strRight)))
	{
		MsgBoxOK(IDS_NO_RIGHT);
		return;
	}
	
	BeginWaitCursor();

	int       iItem   = -1;
	BppBranchReg * pBrReg = m_pli_BranchALL->getNextSelectedBsnObj(&iItem);
	
	if (iItem >= 0)
	{
	//Sicherheitsabfrage
	CString csDeleteSure;
	csDeleteSure.Format(CResString::ex().getStrTblText(IDS_DELETE_SURE_ALL_APP), 
						pBrReg->getBranchname().c_str());
	INT_PTR iRet = MsgBoxYesNo(csDeleteSure);
		
	if(iRet == IDYES)
	{
		IPerst::ERet eRet = pBrReg->delBranch();
		if (eRet == IPerst::RET_OK) //Fehler bei Delete auf DB
		{
			m_pli_BranchALL->giveCon().clearBsnObj(m_pli_BranchALL->getIdx(iItem));
			m_pli_BranchALL->load(false);
		}
		else
		{
			MsgBoxOK(IDS_ERR_DELETE); 
		}
	}
	
	m_pli_BranchALL->SetFocus();
	}
	else
	{
		MsgBoxOK(IDS_SELECT_ITEM);		
	}
	EndWaitCursor();
}

/**---------------------------------------------------------------------------*/
void PDlgDllBrReg::OnBtWorkCorReg()
{
	BeginWaitCursor();

	int       iItem   = -1;
	BppBranchReg * pBrReg = m_pli_RegionCOR->getNextSelectedBsnObj(&iItem);

	if (iItem >= 0)
	{
	PDlgBrRegDetail dlgBrRegDetail(pBrReg);
	dlgBrRegDetail.setBNewBrReg(false);
	dlgBrRegDetail.setBBranch(false);

	EndWaitCursor();

	INT_PTR iRet = dlgBrRegDetail.DoModal();
	
	//clone zurückkopieren!
	if (dlgBrRegDetail.getBDataChanged())
	{
		dlgBrRegDetail.getCloneObj(m_pli_RegionCOR->giveCon().getBsnObj(m_pli_RegionCOR->getIdx(iItem)));
		if (iRet == IDOK)
			m_pli_RegionCOR->refresh(iItem);
	}

	BeginWaitCursor();
	
	m_pli_RegionCOR->SetFocus();
	}
	else
	{
		MsgBoxOK(IDS_SELECT_ITEM);		
	}
	EndWaitCursor();}

/**---------------------------------------------------------------------------*/
void PDlgDllBrReg::OnBtWorkCorBr()
{
	BeginWaitCursor();

	int       iItem   = -1;
	BppBranchReg * pBrReg = m_pli_BranchCOR->getNextSelectedBsnObj(&iItem);

	if (iItem >= 0)
	{
	PDlgBrRegDetail dlgBrRegDetail(pBrReg);
	dlgBrRegDetail.setBNewBrReg(false);
	dlgBrRegDetail.setBBranch(true);

	EndWaitCursor();

	INT_PTR iRet = dlgBrRegDetail.DoModal();
	
	//clone zurückkopieren!
	if (dlgBrRegDetail.getBDataChanged())
	{
		dlgBrRegDetail.getCloneObj(m_pli_BranchCOR->giveCon().getBsnObj(m_pli_BranchCOR->getIdx(iItem)));
		if (iRet == IDOK)
			m_pli_BranchCOR->refresh(iItem);
	}

	BeginWaitCursor();
	
	m_pli_BranchCOR->SetFocus();
	}
	else
	{
		MsgBoxOK(IDS_SELECT_ITEM);		
	}
	EndWaitCursor();}

/**---------------------------------------------------------------------------*/
void PDlgDllBrReg::OnBtBrregExit()
{
	CDialogMultiLang::OnCancel();
}

/**---------------------------------------------------------------------------*/
void PDlgDllBrReg::SetRegionCtrlState(const int iItem)
{
	BOOL    bPrint,
		    bAnzeige;


	if (iItem == -1)					// nichts markiert
	{
		bPrint       = (m_pli_RegionALL->GetItemCount() > 0) ? TRUE : FALSE;		// ListCtrl nicht leer
		bAnzeige     = FALSE;

	}
	else								// Zeile markiert
	{
		bPrint   = TRUE;
		bAnzeige = TRUE;
	}

	// setzen der Controls
	m_bt_Reg_Work.EnableWindow(bAnzeige);
	m_bt_Reg_Del.EnableWindow(bAnzeige);
	m_bt_CorReg.EnableWindow(bAnzeige);
	m_bt_Reg_New.EnableWindow(bAnzeige);
}

/**---------------------------------------------------------------------------*/
void PDlgDllBrReg::SetBranchCtrlState(const int iItem)
{
	BOOL    bPrint,
		    bAnzeige;


	if (iItem == -1)					// nichts markiert
	{
		bPrint       = (m_pli_BranchALL->GetItemCount() > 0) ? TRUE : FALSE;		// ListCtrl nicht leer
		bAnzeige     = FALSE;

	}
	else								// Zeile markiert
	{
		bPrint   = TRUE;
		bAnzeige = TRUE;
	}

	// setzen der Controls
	m_bt_Br_Cor.EnableWindow(bAnzeige);
	m_bt_Br_Work.EnableWindow(bAnzeige);
	m_bt_Br_Del.EnableWindow(bAnzeige);
	m_bt_Br_New.EnableWindow(bAnzeige);
}

/**---------------------------------------------------------------------------*/
void PDlgDllBrReg::SetCORBranchCtrlState	(const int iItem)
{
	BOOL    bPrint,
		    bAnzeige;


	if (iItem == -1)					// nichts markiert
	{
		bPrint       = (m_pli_BranchCOR->GetItemCount() > 0) ? TRUE : FALSE;		// ListCtrl nicht leer
		bAnzeige     = FALSE;

	}
	else								// Zeile markiert
	{
		bPrint   = TRUE;
		bAnzeige = TRUE;
	}

	// setzen der Controls
	m_bt_Br_Erase.EnableWindow(bAnzeige);
	m_bt_Work_CorBr.EnableWindow(bAnzeige);
}

/**---------------------------------------------------------------------------*/
void PDlgDllBrReg::SetCORRegionCtrlState	(const int iItem)
{
	BOOL    bPrint,
		    bAnzeige;


	if (iItem == -1)					// nichts markiert
	{
		bPrint       = (m_pli_RegionCOR->GetItemCount() > 0) ? TRUE : FALSE;		// ListCtrl nicht leer
		bAnzeige     = FALSE;

	}
	else								// Zeile markiert
	{
		bPrint   = TRUE;
		bAnzeige = TRUE;
	}

	// setzen der Controls
	m_bt_EraseReg.EnableWindow(bAnzeige);
	m_bt_Cor_RegBr.EnableWindow(bAnzeige);
	m_bt_Work_CorReg.EnableWindow(bAnzeige);

}


/**---------------------------------------------------------------------------*/
void PDlgDllBrReg::OnDblclkAppListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE * pNMIA = (NMITEMACTIVATE *)pNMHDR;

	if (pNMIA->iItem != -1)
	{
		TRACE(__FUNCTION__ ": Item %d\n", pNMIA->iItem);
	
		if (m_bt_Sel_App.IsWindowEnabled())
			OnBtBrregApp();
	}

	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgDllBrReg::OnItemchangedAppListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
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

	SetRegionCtrlState (-1);
	m_pli_RegionALL->EnableWindow(false);
	m_pli_RegionALL->clearAllBsnObj();
	m_pli_RegionCOR->EnableWindow(false);
	m_pli_RegionCOR->clearAllBsnObj();

	SetBranchCtrlState     (-1); 
	m_pli_BranchALL->EnableWindow(false);
	m_pli_BranchALL->clearAllBsnObj();
	m_pli_BranchCOR->EnableWindow(false);
	m_pli_BranchCOR->clearAllBsnObj();
	
	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgDllBrReg::OnDblclkRegALLListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE * pNMIA = (NMITEMACTIVATE *)pNMHDR;

	if (pNMIA->iItem != -1)
	{
		TRACE(__FUNCTION__ ": Item %d\n", pNMIA->iItem);
	
		if (m_bt_Reg_Work.IsWindowEnabled())
			OnBtBrregWorkReg();
	}

	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgDllBrReg::OnItemchangedRegALLListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV   = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	static int   iSelect = -1;

	if (    ((pNMLV->uChanged & LVIF_TEXT) == LVIF_TEXT) 
		 && ( pNMLV->iItem                 == iSelect  ))
	{
		TRACE(__FUNCTION__ ": text changed on Item %d\n", iSelect);
		SetRegionCtrlState(iSelect);
	}
		 
	if ((pNMLV->uChanged & LVIF_STATE) == LVIF_STATE) 
	{
		if ((pNMLV->uNewState & LVNI_SELECTED) == LVNI_SELECTED)			// wird selektiert
		{
			iSelect = pNMLV->iItem;
			TRACE(__FUNCTION__ ": state changed (LVNI_SELECTED) on Item %d\n", iSelect);

			SetRegionCtrlState(iSelect);
		}
		else
		{
			if ((pNMLV->uOldState & LVNI_SELECTED) == LVNI_SELECTED)		// wird deselektiert
				SetRegionCtrlState(-1);
		}
	}

	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgDllBrReg::OnItemchangedRegCORListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV   = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	static int   iSelect = -1;

	if (    ((pNMLV->uChanged & LVIF_TEXT) == LVIF_TEXT) 
		 && ( pNMLV->iItem                 == iSelect  ))
	{
		TRACE(__FUNCTION__ ": text changed on Item %d\n", iSelect);
		SetCORRegionCtrlState(iSelect);
	}
		 
	if ((pNMLV->uChanged & LVIF_STATE) == LVIF_STATE) 
	{
		if ((pNMLV->uNewState & LVNI_SELECTED) == LVNI_SELECTED)			// wird selektiert
		{
			iSelect = pNMLV->iItem;
			TRACE(__FUNCTION__ ": state changed (LVNI_SELECTED) on Item %d\n", iSelect);

			SetCORRegionCtrlState(iSelect);
		}
		else
		{
			if ((pNMLV->uOldState & LVNI_SELECTED) == LVNI_SELECTED)		// wird deselektiert
				SetCORRegionCtrlState(-1);
		}
	}

	*pResult = 0;
}
/**---------------------------------------------------------------------------*/
void PDlgDllBrReg::OnDblclkRegCORListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE * pNMIA = (NMITEMACTIVATE *)pNMHDR;

	if (pNMIA->iItem != -1)
	{
		TRACE(__FUNCTION__ ": Item %d\n", pNMIA->iItem);
	
		if (m_bt_Work_CorReg.IsWindowEnabled())
			OnBtWorkCorReg();
	}

	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgDllBrReg::OnDblclkBrCORListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE * pNMIA = (NMITEMACTIVATE *)pNMHDR;

	if (pNMIA->iItem != -1)
	{
		TRACE(__FUNCTION__ ": Item %d\n", pNMIA->iItem);
	
		if (m_bt_Work_CorBr.IsWindowEnabled())
			OnBtWorkCorBr();
	}

	*pResult = 0;
}	

/**---------------------------------------------------------------------------*/
void PDlgDllBrReg::OnDblclkBrALLListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE * pNMIA = (NMITEMACTIVATE *)pNMHDR;

	if (pNMIA->iItem != -1)
	{
		TRACE(__FUNCTION__ ": Item %d\n", pNMIA->iItem);
	
		if (m_bt_Br_Work.IsWindowEnabled())
			OnBtBrregWorkBr();
	}

	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgDllBrReg::OnItemchangedBrALLListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
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

	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgDllBrReg::OnItemchangedBrCORListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV   = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	static int   iSelect = -1;

	if (    ((pNMLV->uChanged & LVIF_TEXT) == LVIF_TEXT) 
		 && ( pNMLV->iItem                 == iSelect  ))
	{
		TRACE(__FUNCTION__ ": text changed on Item %d\n", iSelect);
		SetCORBranchCtrlState(iSelect);
	}
		 
	if ((pNMLV->uChanged & LVIF_STATE) == LVIF_STATE) 
	{
		if ((pNMLV->uNewState & LVNI_SELECTED) == LVNI_SELECTED)			// wird selektiert
		{
			iSelect = pNMLV->iItem;
			TRACE(__FUNCTION__ ": state changed (LVNI_SELECTED) on Item %d\n", iSelect);

			SetCORBranchCtrlState(iSelect);
		}
		else
		{
			if ((pNMLV->uOldState & LVNI_SELECTED) == LVNI_SELECTED)		// wird deselektiert
				SetCORBranchCtrlState(-1);
		}
	}

	*pResult = 0;
}


/**---------------------------------------------------------------------------*/
void PDlgDllBrReg::OnNMSetfocusCorReg(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE * pNMIA = (NMITEMACTIVATE *)pNMHDR;

	if (pNMIA->iItem == -1)
	{
		SetCORRegionCtrlState	(-1);
	}
	else
	{
		SetCORRegionCtrlState	(1);
	}
	SetBranchCtrlState	   (-1);
	SetRegionCtrlState	   (-1);
	SetCORBranchCtrlState	(-1);
	
	*pResult = 0;	
}

/**---------------------------------------------------------------------------*/
void PDlgDllBrReg::OnNMSetfocusAllReg(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE * pNMIA = (NMITEMACTIVATE *)pNMHDR;

	if (pNMIA->iItem == -1)
	{
		SetRegionCtrlState	(-1);
	}
	else
	{
		SetRegionCtrlState	(1);
	}
	SetBranchCtrlState	   (-1);
	SetCORBranchCtrlState	(-1);
	SetCORRegionCtrlState	(-1);

	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgDllBrReg::OnNMSetfocusCorBr(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE * pNMIA = (NMITEMACTIVATE *)pNMHDR;

	if (pNMIA->iItem == -1)
	{
		SetCORBranchCtrlState	(-1);
	}
	else
	{
		SetCORBranchCtrlState	(1);
	}
	SetBranchCtrlState	   (-1);
	SetRegionCtrlState	   (-1);
	SetCORRegionCtrlState	(-1);

	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgDllBrReg::OnNMSetfocusAllBr(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE * pNMIA = (NMITEMACTIVATE *)pNMHDR;

	if (pNMIA->iItem == -1)
	{
		SetBranchCtrlState	(-1);
	}
	else
	{
		SetBranchCtrlState	(1);
	}
	SetRegionCtrlState	   (-1);
	SetCORBranchCtrlState	(-1);
	SetCORRegionCtrlState	(-1);

	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgDllBrReg::OnNMClickCorReg(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE * pNMIA = (NMITEMACTIVATE *)pNMHDR;

	if (pNMIA->iItem == -1)
	{
		SetCORRegionCtrlState	(-1);
	}
	else
	{
		SetCORRegionCtrlState	(1);
	}
	SetBranchCtrlState	   (-1);
	SetRegionCtrlState	   (-1);
	SetCORBranchCtrlState	(-1);
	
	*pResult = 0;	
}

/**---------------------------------------------------------------------------*/
void PDlgDllBrReg::OnNMClickAllReg(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE * pNMIA = (NMITEMACTIVATE *)pNMHDR;

	if (pNMIA->iItem == -1)
	{
		SetRegionCtrlState	(-1);
	}
	else
	{
		SetRegionCtrlState	(1);
	}
	SetBranchCtrlState	   (-1);
	SetCORBranchCtrlState	(-1);
	SetCORRegionCtrlState	(-1);
	m_bt_Reg_New.EnableWindow(true);

	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgDllBrReg::OnNMClickCorBr(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE * pNMIA = (NMITEMACTIVATE *)pNMHDR;

	if (pNMIA->iItem == -1)
	{
		SetCORBranchCtrlState	(-1);
	}
	else
	{
		SetCORBranchCtrlState	(1);
	}
	SetBranchCtrlState	   (-1);
	SetRegionCtrlState	   (-1);
	SetCORRegionCtrlState	(-1);

	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgDllBrReg::OnNMClickAllBr(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE * pNMIA = (NMITEMACTIVATE *)pNMHDR;

	if (pNMIA->iItem == -1)
	{
		SetBranchCtrlState	(-1);
	}
	else
	{
		SetBranchCtrlState	(1);
	}
	SetRegionCtrlState	   (-1);
	SetCORBranchCtrlState	(-1);
	SetCORRegionCtrlState	(-1);
	m_bt_Br_New.EnableWindow(true);

	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
BOOL PDlgDllBrReg::PreTranslateMessage(MSG* pMsg)
{
	assert(pMsg != 0);

	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			
			if (GetFocus() == GetDlgItem(IDC_LI_BREG_APP))
			{
				if (m_bt_Sel_App.IsWindowEnabled())
				{
					OnBtBrregApp();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_LI_BRREG_ALL_BR))
			{
				if (m_bt_Br_Work.IsWindowEnabled())
				{
					OnBtBrregWorkBr();
					return TRUE;
				}
			}
			
			if (GetFocus() == GetDlgItem(IDC_LI_BRREG_ALL_REG))
			{
				if (m_bt_Reg_Work.IsWindowEnabled())
				{
					OnBtBrregWorkReg();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_BT_BRREG_EXIT))
			{
				if (m_bt_BrReg_Exit.IsWindowEnabled())
				{
					OnBtBrregExit();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_BT_BRREG_COR_REG))
			{
				if (m_bt_CorReg.IsWindowEnabled())
				{
					OnBtBrregCorReg();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_BT_BRREG_ERASE_REG))
			{
				if (m_bt_EraseReg.IsWindowEnabled())
				{
					OnBtBrregEraseReg();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_BT_BRREG_NEW_REG))
			{
				if (m_bt_Reg_New.IsWindowEnabled())
				{
					OnBtBrregNewReg();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_BT_BRREG_WORK_REG))
			{
				if (m_bt_Reg_Work.IsWindowEnabled())
				{
					OnBtBrregWorkReg();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_BT_BRREG_DEL_REG))
			{
				if (m_bt_Reg_Del.IsWindowEnabled())
				{
					OnBtBrregDelReg();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_BT_BRREG_COR))
			{
				if (m_bt_Cor_RegBr.IsWindowEnabled())
				{
					OnBtBrRegCor();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_BT_BRREG_COR_BR))
			{
				if (m_bt_Br_Cor.IsWindowEnabled())
				{
					OnBtBrregCorBr();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_BT_BRREG_ERASE_BR))
			{
				if (m_bt_Br_Erase.IsWindowEnabled())
				{
					OnBtBrregEraseBr();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_BT_BRREG_NEW_BR))
			{
				if (m_bt_Br_New.IsWindowEnabled())
				{
					OnBtBrregNewBr();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_BT_BRREG_WORK_BR))
			{
				if (m_bt_Br_Work.IsWindowEnabled())
				{
					OnBtBrregWorkBr();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_BT_BRREG_DEL_BR))
			{
				if (m_bt_Br_Del.IsWindowEnabled())
				{
					OnBtBrregDelBr();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_BT_WORK_COR_REG))
			{
				if (m_bt_Work_CorReg.IsWindowEnabled())
				{
					OnBtWorkCorReg();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_BT_WORK_COR_BR))
			{
				if (m_bt_Work_CorBr.IsWindowEnabled())
				{
					OnBtWorkCorBr();
					return TRUE;
				}
			}
			return FALSE;

		case VK_ESCAPE:
			if (m_bt_BrReg_Exit.IsWindowEnabled())
			{
				OnBtBrregExit();
				return TRUE;
			}
			return FALSE;


		default:
			break;
		}
	} 

	return CDialogMultiLang::PreTranslateMessage(pMsg);
}


