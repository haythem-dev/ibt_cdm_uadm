/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                     */
/**   Date   : 03.08.2004                                                     */
/**                                                                           */
/******************************************************************************/

/*----------------------------------------------------------------------------*/
// Includes
/*----------------------------------------------------------------------------*/
#include "StdAfx.h"
#include <BppApp.h>
#include <BConppApp.h>
#include <BppPara.h>
#include <BConppPara.h>
#include "PDlgAppListCtrl.h"
#include "PDlgParaListCtrl.h"
#include "presentation.h"
#include "PDlgParaDetail.h"
#include "PDlgDllPara.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*----------------------------------------------------------------------------*/
// 
/**---------------------------------------------------------------------------*/
IMPLEMENT_DYNCREATE(PDlgDllPara, CDialogMultiLang)

/**---------------------------------------------------------------------------*/
PDlgDllPara::PDlgDllPara()
	: CDialogMultiLang(PDlgDllPara::IDD)
{

   m_pli_Para = new PDlgParaListCtrl;
	   assert(m_pli_Para);

   m_pli_App = new PDlgAppListCtrl;
	   assert(m_pli_App);
	
}

/**---------------------------------------------------------------------------*/
PDlgDllPara::~PDlgDllPara()
{
	if (m_pli_App != NULL)
	{
		delete m_pli_App;
		m_pli_App = NULL;
	}
	
	if (m_pli_Para != NULL)
	{
		delete m_pli_Para;
		m_pli_Para = NULL;
	}
	
}

/**---------------------------------------------------------------------------*/
void PDlgDllPara::DoDataExchange(CDataExchange* pDX)
{
	CDialogMultiLang::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LI_PARA_APP,	*m_pli_App);
	DDX_Control(pDX, IDC_LI_PARA,		*m_pli_Para);
	DDX_Control(pDX, IDC_BT_PARA_APP,	m_bt_Para_App);
	DDX_Control(pDX, IDC_BT_PARA_NEW,	m_bt_Para_New);
	DDX_Control(pDX, IDC_BT_PARA_WORK,	m_bt_Para_Work);
	DDX_Control(pDX, IDC_BT_PARA_DEL,	m_bt_Para_Del);
	DDX_Control(pDX, IDC_BT_PARA_EXIT,	m_bt_Para_Exit);
}

/**---------------------------------------------------------------------------*/
BOOL PDlgDllPara::OnInitDialog()
{
	BeginWaitCursor();

	CDialogMultiLang::OnInitDialog();

	m_pli_App->init();
	if (m_pli_App->load() == FALSE) 
		SetAppCtrlState(-1);
	
	m_pli_Para->init();
	
	m_pli_App->SetFocus();
	
	//Controls
	m_pli_Para->EnableWindow(false);
	SetParaCtrlState(-1);
	
	EndWaitCursor();

	return FALSE;  // return TRUE unless you set the focus to a control
}

/**---------------------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(PDlgDllPara, CDialogMultiLang)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LI_PARA_APP, OnItemchangedAppListCtrl)
	ON_NOTIFY(NM_DBLCLK, IDC_LI_PARA_APP, OnDblclkAppListCtrl)
	ON_NOTIFY(NM_DBLCLK, IDC_LI_PARA, OnDblclkParaListCtrl)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LI_PARA, OnItemchangedParaListCtrl)
	ON_NOTIFY(NM_CLICK, IDC_LI_PARA, OnNMClickLiPara)
	ON_BN_CLICKED(IDC_BT_PARA_APP, OnBtParaApp)
	ON_BN_CLICKED(IDC_BT_PARA_NEW, OnBtParaNew)
	ON_BN_CLICKED(IDC_BT_PARA_WORK, OnBtParaWork)
	ON_BN_CLICKED(IDC_BT_PARA_DEL, OnBtParaDel)
	ON_BN_CLICKED(IDC_BT_PARA_EXIT, OnBtParaExit)
END_MESSAGE_MAP()

/**---------------------------------------------------------------------------*/
void PDlgDllPara::OnBtParaApp()
{
	BeginWaitCursor();
	
	int		  iItemApp = -1;
	m_sAppid = (short) m_pli_App->getNextSelectedBsnObj(&iItemApp)->getAppid();
	
	if (iItemApp >= 0)
	{
    //load
	m_pli_Para->giveCon().setAppid(m_sAppid);
	if (m_pli_Para->load() == FALSE) 
		SetParaCtrlState(-1);
	else
		SetParaCtrlState(1);

	//Enable-Ctrls
	m_bt_Para_New.EnableWindow(true);
	m_pli_Para->EnableWindow(true);
	m_pli_Para->SetFocus();
	}
	else
	{
		MsgBoxOK(IDS_SELECT_ITEM);		
	}
	EndWaitCursor();
}

/**---------------------------------------------------------------------------*/
void PDlgDllPara::OnBtParaNew()
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

	BppPara * pPara = new BppPara;
		assert(pPara);

	pPara->setAppid(m_sAppid);
	
	PDlgParaDetail dlgParaDetail(pPara);
	dlgParaDetail.setBNewPara(true);

	EndWaitCursor();
	int		  iItemApp = -1;
	dlgParaDetail.setAppname(m_pli_App->getNextSelectedBsnObj(&iItemApp)->getAppname());
	dlgParaDetail.setAppid(m_sAppid);
	
	INT_PTR iRet = dlgParaDetail.DoModal();

	//clone zurückkopieren!
	if (dlgParaDetail.getBDataChanged())
	{
		dlgParaDetail.getCloneObj(pPara);
		if (iRet == IDOK)
		{
			m_pli_Para->giveCon().addBsnObj(*pPara);
			
			if (m_pli_Para->load(false) == FALSE) 
				SetParaCtrlState(-1);
		}
	}

	if(pPara != NULL)
	{
		delete pPara;
		pPara = NULL;
	}
	
	BeginWaitCursor();
	
	m_pli_Para->SetFocus();
	
	EndWaitCursor();
}

/**---------------------------------------------------------------------------*/
void PDlgDllPara::OnBtParaWork()
{
	BeginWaitCursor();

	int       iItem   = -1;
	BppPara * pPara = m_pli_Para->getNextSelectedBsnObj(&iItem);
	if (iItem >= 0)
	{
	pPara->setAppid(m_sAppid);
	
	PDlgParaDetail dlgParaDetail(pPara);
	dlgParaDetail.setBNewPara(false);

	int	iItemApp = -1;
	dlgParaDetail.setAppname(m_pli_App->getNextSelectedBsnObj(&iItemApp)->getAppname());
	dlgParaDetail.setAppid(m_sAppid);
	
	EndWaitCursor();

	INT_PTR iRet = dlgParaDetail.DoModal();
	
	//clone zurückkopieren!
	if (dlgParaDetail.getBDataChanged())
	{
		dlgParaDetail.getCloneObj(m_pli_Para->giveCon().getBsnObj(m_pli_Para->getIdx(iItem)));
		if (iRet == IDOK)
			m_pli_Para->refresh(iItem);
	}

	BeginWaitCursor();
	
	m_pli_Para->SetFocus();
	}
	else
	{
		MsgBoxOK(IDS_SELECT_ITEM);		
	}
	EndWaitCursor();
}

/**---------------------------------------------------------------------------*/
void PDlgDllPara::OnBtParaDel()
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
	BppPara * pPara = m_pli_Para->getNextSelectedBsnObj(&iItem);
	if (iItem >= 0)
	{
	pPara->setAppid(m_sAppid);
	
	//Sicherheitsabfrage
	CString csDeleteSure;
	csDeleteSure.Format(CResString::ex().getStrTblText(IDS_DELETE_SURE), 
						pPara->getParabez().c_str());
	INT_PTR iRet = MsgBoxYesNo(csDeleteSure);
		
	if(iRet == IDYES)
	{
		IPerst::ERet eRet = pPara->deletePara();
		if (eRet == IPerst::RET_OK) 
		{
			m_pli_Para->giveCon().clearBsnObj(m_pli_Para->getIdx(iItem));
			m_pli_Para->load(false);
		}
		else //Fehler bei Delete auf DB
		{
			MsgBoxOK(IDS_ERR_DELETE); 
		}
	}
	
	m_pli_Para->SetFocus();
	}
	else
	{
		MsgBoxOK(IDS_SELECT_ITEM);		
	}
	EndWaitCursor();
}

/**---------------------------------------------------------------------------*/
void PDlgDllPara::OnBtParaExit()
{
	CDialogMultiLang::OnCancel();
}

/**---------------------------------------------------------------------------*/
void PDlgDllPara::OnNMClickLiPara(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE * pNMIA = (NMITEMACTIVATE *)pNMHDR;

	if (pNMIA->iItem == -1)
	{
		SetParaCtrlState	(-1);
	}
	else
	{
		SetParaCtrlState	(1);
	}
	m_bt_Para_New.EnableWindow(true);
	
	
	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgDllPara::SetAppCtrlState(const int iItem)
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
	m_bt_Para_App.EnableWindow(bAnzeige);
}

/**---------------------------------------------------------------------------*/
void PDlgDllPara::SetParaCtrlState(const int iItem)
{
	BOOL    bPrint,
		    bAnzeige;


	if (iItem == -1)					// nichts markiert
	{
		bPrint       = (m_pli_Para->GetItemCount() > 0) ? TRUE : FALSE;		// ListCtrl nicht leer
		bAnzeige     = FALSE;

	}
	else								// Zeile markiert
	{
		bPrint   = TRUE;
		bAnzeige = TRUE;
	}

	// setzen der Controls
	m_bt_Para_Work.EnableWindow(bAnzeige);
	m_bt_Para_Del.EnableWindow(bAnzeige);
	m_bt_Para_New.EnableWindow(bAnzeige);
}

/**---------------------------------------------------------------------------*/
void PDlgDllPara::OnDblclkAppListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE * pNMIA = (NMITEMACTIVATE *)pNMHDR;

	if (pNMIA->iItem != -1)
	{
		TRACE(__FUNCTION__ ": Item %d\n", pNMIA->iItem);
	
		if (m_bt_Para_App.IsWindowEnabled())
			OnBtParaApp();
	}

	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgDllPara::OnItemchangedAppListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
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

	SetParaCtrlState (-1);
	m_pli_Para->EnableWindow(false);
	m_pli_Para->clearAllBsnObj();

	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgDllPara::OnDblclkParaListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE * pNMIA = (NMITEMACTIVATE *)pNMHDR;

	if (pNMIA->iItem != -1)
	{
		TRACE(__FUNCTION__ ": Item %d\n", pNMIA->iItem);
	
		if (m_bt_Para_Work.IsWindowEnabled())
			OnBtParaWork();
	}

	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgDllPara::OnItemchangedParaListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV   = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	static int   iSelect = -1;

	if (    ((pNMLV->uChanged & LVIF_TEXT) == LVIF_TEXT) 
		 && ( pNMLV->iItem                 == iSelect  ))
	{
		TRACE(__FUNCTION__ ": text changed on Item %d\n", iSelect);
		SetParaCtrlState(iSelect);
	}
		 
	if ((pNMLV->uChanged & LVIF_STATE) == LVIF_STATE) 
	{
		if ((pNMLV->uNewState & LVNI_SELECTED) == LVNI_SELECTED)			// wird selektiert
		{
			iSelect = pNMLV->iItem;
			TRACE(__FUNCTION__ ": state changed (LVNI_SELECTED) on Item %d\n", iSelect);

			SetParaCtrlState(iSelect);
		}
		else
		{
			if ((pNMLV->uOldState & LVNI_SELECTED) == LVNI_SELECTED)		// wird deselektiert
				SetParaCtrlState(-1);
		}
	}

	*pResult = 0;
}


/**---------------------------------------------------------------------------*/
BOOL PDlgDllPara::PreTranslateMessage(MSG* pMsg)
{
	assert(pMsg != 0);

	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			// Fokus steht in ParaListCtrl
			if (GetFocus() == GetDlgItem(IDC_LI_PARA))
			{
				if (m_bt_Para_Work.IsWindowEnabled())
				{
					OnBtParaWork();
					return TRUE;
				}
			}
			// Fokus steht in AppListCtrl
			if (GetFocus() == GetDlgItem(IDC_LI_PARA_APP))
			{
				if (m_bt_Para_App.IsWindowEnabled())
				{
					OnBtParaApp();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_BT_PARA_NEW))
			{
				if (m_bt_Para_New.IsWindowEnabled())
				{
					OnBtParaNew();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_BT_PARA_WORK))
			{
				if (m_bt_Para_Work.IsWindowEnabled())
				{
					OnBtParaWork();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_BT_PARA_DEL))
			{
				if (m_bt_Para_Del.IsWindowEnabled())
				{
					OnBtParaDel();
					return TRUE;
				}
			}			
			if (GetFocus() == GetDlgItem(IDC_BT_PARA_EXIT))
			{
				if (m_bt_Para_Exit.IsWindowEnabled())
				{
					OnBtParaExit();
					return TRUE;
				}
			}
			return FALSE;

		case VK_ESCAPE:
			if (m_bt_Para_Exit.IsWindowEnabled())
			{
				OnBtParaExit();
				return TRUE;
			}
			return FALSE;


		default:
			break;
		}
	} 

	return CDialogMultiLang::PreTranslateMessage(pMsg);
}


