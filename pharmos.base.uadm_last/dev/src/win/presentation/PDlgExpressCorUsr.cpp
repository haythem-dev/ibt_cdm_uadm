/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                     */
/**   Date   : 09.06.2005                                                     */
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
#include <BppGroup.h>
#include <BConppGroup.h>
#include "PDlgAppListCtrl.h"
#include "PDlgUserListCtrl.h"
#include "PDlgGroupListCtrl.h"
#include "presentation.h"
#include "PDlgPW.h"
#include "PDlgExpressCorUsr.h"

#include <CVersion.h>

#include <vector>
using std::vector;
class C_VecGrp : public vector <int>{};
class C_VecUsr : public vector <int>{};

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/**---------------------------------------------------------------------------*/
IMPLEMENT_DYNCREATE(PDlgExpressCorUsr, CDialogMultiLang)

/**---------------------------------------------------------------------------*/
PDlgExpressCorUsr::PDlgExpressCorUsr()
	: CDialogMultiLang(PDlgExpressCorUsr::IDD)
{

   m_pli_User = new PDlgUserListCtrl;
	   assert(m_pli_User);

    m_pli_Group = new PDlgGroupListCtrl;
	   assert(m_pli_Group);

   m_pli_App = new PDlgAppListCtrl;
	   assert(m_pli_App);

	m_sAppid = 0;

	m_pPDlgPW = new PDlgPW;
	assert(m_pPDlgPW);

	m_ConGrp = new C_VecGrp;
	m_ConUsr = new C_VecUsr;
}

/**---------------------------------------------------------------------------*/
PDlgExpressCorUsr::~PDlgExpressCorUsr()
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
	
	if (m_pli_User != NULL)
	{
		delete m_pli_User;
		m_pli_User = NULL;
	}	

	if (m_pPDlgPW != NULL)
	{
		delete m_pPDlgPW;
		m_pPDlgPW = NULL;
	}

	delete m_ConGrp;
	delete m_ConUsr;
}

/**---------------------------------------------------------------------------*/
void PDlgExpressCorUsr::DoDataExchange(CDataExchange* pDX)
{
	CDialogMultiLang::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LI_EXPCOR_APP,	  *m_pli_App);
	DDX_Control(pDX, IDC_LI_EXPCOR_USR, *m_pli_User);
	DDX_Control(pDX, IDC_LI_EXPCOR_GRP, *m_pli_Group);
	DDX_Control(pDX, IDC_BT_EXPCOR_SEL_APP, m_bt_Sel_App);
	DDX_Control(pDX, IDC_BT_EXPCOR_SAVE, m_bt_CorUsr);
	DDX_Control(pDX, IDC_BT_EXPCOR_EXIT, m_bt_Exit);
	DDX_Control(pDX, IDC_BT_EXPCOR_ALLUSR, m_bt_AllUsr);
	DDX_Control(pDX, IDC_BT_EXPCOR_ALLGRP, m_bt_AllGrp);
	DDX_Control(pDX, IDC_EXPCOR_CK_OVERWRITE, m_ck_OverWrite);

}

/**---------------------------------------------------------------------------*/
BOOL PDlgExpressCorUsr::OnInitDialog()
{
	BeginWaitCursor();

	CDialogMultiLang::OnInitDialog();

	m_pli_App->init();
	if (m_pli_App->load() == FALSE) 
		m_bt_Sel_App.EnableWindow(false);
	
	m_pli_User->init();

	m_pli_Group->init();

	m_ck_OverWrite.EnableWindow(false);
	
	m_pli_App->SetFocus();
		
	EndWaitCursor();

	return FALSE;  // return TRUE unless you set the focus to a control
}

/**---------------------------------------------------------------------------*/
void PDlgExpressCorUsr::OnBtSelApp()
{
	int iItem = -1;
	m_sAppid = (short) m_pli_App->getNextSelectedBsnObj(&iItem)->getAppid();
	
	m_pli_User->giveCon().setAppid(m_sAppid);
	m_pli_User->giveCon().setCorUsrGrp(false);
	m_pli_User->giveCon().setGroupid(0);
	
	if (m_pli_User->load() == FALSE) 
		m_bt_AllUsr.EnableWindow(false);
	else
	{
		m_pli_User->EnableWindow(true);
		m_bt_AllUsr.EnableWindow(true);
	}

	m_pli_Group->giveCon().setAppid(m_sAppid);
	m_pli_Group->giveCon().setBAll(false);
	
	if (m_pli_Group->load() == FALSE) 
		m_bt_AllGrp.EnableWindow(false);
	else
	{
		m_pli_Group->EnableWindow(true);
		m_pli_Group->SetFocus();
		m_bt_AllGrp.EnableWindow(true);
		m_ck_OverWrite.EnableWindow(true);
	}

}

/**---------------------------------------------------------------------------*/
void PDlgExpressCorUsr::OnBtAllUsr()
{
	m_pli_User->giveCon().setAppid(0);
	m_pli_User->giveCon().setCorUsrGrp(false);
	m_pli_User->giveCon().setGroupid(0);
	
	m_pli_User->load(); 

	m_pli_User->EnableWindow(true);
	m_pli_User->SetFocus();
}

/**---------------------------------------------------------------------------*/
void PDlgExpressCorUsr::OnBtAllGrp()
{
	m_pli_Group->giveCon().setBAll(true);
	m_pli_Group->load();
		
	m_pli_Group->EnableWindow(true);
	m_pli_Group->SetFocus();
}

/**---------------------------------------------------------------------------*/
void PDlgExpressCorUsr::OnBtExit()
{
	CDialogMultiLang::OnCancel();
}

/**---------------------------------------------------------------------------*/
void PDlgExpressCorUsr::FillContainer()
{
	BppGroup			* pGroup = 0;
	BppUser				* pUser  = 0;
	int					   iCnt;

	m_ConGrp->clear();
	m_ConUsr->clear();

	
	iCnt = -1;
	while ((pGroup = m_pli_Group->getNextSelectedBsnObj(&iCnt)) != 0)
	{
		m_ConGrp->push_back(pGroup->getGroupid());
	}

	iCnt = -1;
	while ((pUser = m_pli_User->getNextSelectedBsnObj(&iCnt)) != 0)
	{
		m_ConUsr->push_back(pUser->getUserid());
	}
}

/**---------------------------------------------------------------------------*/
void PDlgExpressCorUsr::OnBtCorUsr()
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
		INT_PTR iRet = MsgBoxYesNo ( CResString::ex().getStrTblText(IDS_OVERWRITE_CORUSR));
	
		if(iRet != IDYES)
			return;
	}

	BeginWaitCursor();

	FillContainer();

	BppGroup * pGroup = new BppGroup;
	
	size_t iSizeGrp = m_ConGrp->size();
	size_t iSizeUsr;

	IPerst::ERet eRet;

	bool bAction = false;
	
	pGroup->setAppid(m_sAppid);
	
	while ( iSizeGrp-- > 0 )
	{
		pGroup->setGroupid(m_ConGrp->at(iSizeGrp));
		
		if ( m_ck_OverWrite.GetCheck() > 0 )
		{
			eRet = pGroup->deleteAllCorUser();//delete
			if (eRet != IPerst::RET_OK) 
			{
				MsgBoxOK(IDS_ERR_DELETE);
			}
			else
				bAction = true;
		}

		iSizeUsr = m_ConUsr->size();
		
		while ( iSizeUsr-- > 0 )
		{
			pGroup->setUserid(m_ConUsr->at(iSizeUsr));
					
			eRet = pGroup->insertCor();//Insert
			if (eRet == IPerst::RET_ERROR) 
			{
				MsgBoxOK(IDS_ERR_INSERT);
				delete pGroup;
				EndWaitCursor();
				return;
			}
			bAction = true;
		}
	}
	
	if ( bAction == true)
		MsgBoxOK(CResString::ex().getStrTblText(IDS_EXP_CORUSR_OK));
	else
		MsgBoxOK(CResString::ex().getStrTblText(IDS_EXP_RI_NO_ACTION));

	delete pGroup;
	EndWaitCursor();
}

/**---------------------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(PDlgExpressCorUsr, CDialogMultiLang)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LI_EXPCOR_APP, OnItemchangedAppListCtrl)
	ON_NOTIFY(NM_DBLCLK, IDC_LI_EXPCOR_APP, OnDblclkAppListCtrl)
	ON_BN_CLICKED(IDC_BT_EXPCOR_ALLUSR, OnBtAllUsr)
	ON_BN_CLICKED(IDC_BT_EXPCOR_ALLGRP, OnBtAllGrp)
	ON_BN_CLICKED(IDC_BT_EXPCOR_SAVE, OnBtCorUsr)
	ON_BN_CLICKED(IDC_BT_EXPCOR_EXIT, OnBtExit)
	ON_BN_CLICKED(IDC_BT_EXPCOR_SEL_APP, OnBtSelApp)
END_MESSAGE_MAP()

/**---------------------------------------------------------------------------*/
void PDlgExpressCorUsr::OnDblclkAppListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE * pNMIA = (NMITEMACTIVATE *)pNMHDR;

	if (pNMIA->iItem != -1)
	{
		TRACE(__FUNCTION__ ": Item %d\n", pNMIA->iItem);
	
		if (m_bt_Sel_App.IsWindowEnabled())
			OnBtSelApp();
	}

	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgExpressCorUsr::OnItemchangedAppListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
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

	m_pli_Group->EnableWindow(false);
	m_pli_Group->clearAllBsnObj();

	m_bt_AllUsr.EnableWindow(false);
	m_bt_AllGrp.EnableWindow(false);
	m_ck_OverWrite.EnableWindow(false);

	m_ck_OverWrite.SetCheck(0);	
		
	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
BOOL PDlgExpressCorUsr::PreTranslateMessage(MSG* pMsg)
{
	assert(pMsg != 0);

	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			
			if (GetFocus() == GetDlgItem(IDC_LI_EXPCOR_APP))
			{
				if (m_bt_Sel_App.IsWindowEnabled())
				{
					OnBtSelApp();
					return TRUE;
				}
			}
			
			if (GetFocus() == GetDlgItem(IDC_BT_EXPCOR_SAVE))
			{
				if (m_bt_CorUsr.IsWindowEnabled())
				{
					OnBtCorUsr();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_BT_EXPCOR_EXIT))
			{
				if (m_bt_Exit.IsWindowEnabled())
				{
					OnBtExit();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_BT_EXPCOR_ALLUSR))
			{
				if (m_bt_AllUsr.IsWindowEnabled())
				{
					OnBtAllUsr();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_BT_EXPCOR_ALLGRP))
			{
				if (m_bt_AllGrp.IsWindowEnabled())
				{
					OnBtAllGrp();
					return TRUE;
				}
			}
			return FALSE;

		case VK_ESCAPE:
			if (m_bt_Exit.IsWindowEnabled())
			{
				OnBtExit();
				return TRUE;
			}
			return FALSE;


		default:
			break;
		}
		
		// Suche in User-Liste
		if (GetFocus() == GetDlgItem(IDC_LI_EXPCOR_USR))
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


