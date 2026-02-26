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
#include <BppLanguage.h>
#include <BConppLanguage.h>
#include "PDlgAppListCtrl.h"
#include "PDlgLangListCtrl.h"
#include "PDlgLangDetail.h"
#include "presentation.h"
#include "PDlgDllLangText.h"

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
IMPLEMENT_DYNCREATE(PDlgDllLangText, CDialogMultiLang)

/**---------------------------------------------------------------------------*/
PDlgDllLangText::PDlgDllLangText()
	: CDialogMultiLang(PDlgDllLangText::IDD)
{

   m_pli_LangALL = new PDlgLangListCtrl;
	   assert(m_pli_LangALL);

    m_pli_LangCOR = new PDlgLangListCtrl;
	   assert(m_pli_LangCOR);

   m_pli_App = new PDlgAppListCtrl;
	   assert(m_pli_App);

	  m_bLoadNew = false;

}

/**---------------------------------------------------------------------------*/
PDlgDllLangText::~PDlgDllLangText()
{
	if (m_pli_LangALL != NULL)
	{
		delete m_pli_LangALL;
		m_pli_LangALL = NULL;
	}

	if (m_pli_App != NULL)
	{
		delete m_pli_App;
		m_pli_App = NULL;
	}
	
	if (m_pli_LangCOR != NULL)
	{
		delete m_pli_LangCOR;
		m_pli_LangCOR = NULL;
	}	
}

/**---------------------------------------------------------------------------*/
void PDlgDllLangText::DoDataExchange(CDataExchange* pDX)
{
	CDialogMultiLang::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LI_LANG_TEXT_APP,	  *m_pli_App);
	DDX_Control(pDX, IDC_LI_LANG_COR, *m_pli_LangCOR);
	DDX_Control(pDX, IDC_LI_LANG_ALL, *m_pli_LangALL);
	
	DDX_Control(pDX, IDC_BT_LANG_TEXT_SEL_APP, m_bt_SelApp);
	DDX_Control(pDX, IDC_BT_LANG_COR, m_bt_Cor_Lang);
	DDX_Control(pDX, IDC_BT_LANG_ERASE, m_bt_Erase_Lang);
	DDX_Control(pDX, IDC_BT_WORK_CORLANG, m_bt_Work_CORLang);
	DDX_Control(pDX, IDC_BT_LANG_NEW, m_bt_Lang_New);
	DDX_Control(pDX, IDC_BT_LANG_WORK, m_bt_Lang_Work);
	DDX_Control(pDX, IDC_BT_LANG_DEL, m_bt_Lang_Del);
	DDX_Control(pDX, IDC_BT_LANG_TEXT_EXIT, m_bt_Lang_Text_Exit);
}

/**---------------------------------------------------------------------------*/
BOOL PDlgDllLangText::OnInitDialog()
{
	BeginWaitCursor();

	CDialogMultiLang::OnInitDialog();

	m_pli_App->init();
	if (m_pli_App->load() == FALSE) 
		m_bt_SelApp.EnableWindow(false);
	
	m_pli_LangCOR->init();
	m_pli_LangALL->init();
	
	m_pli_App->SetFocus();
	
	//Controls
	//m_bt_Lang_New.EnableWindow(false);
	SetLangCORCtrlState(-1);
	SetLangALLCtrlState(-1);
		
	EndWaitCursor();

	return FALSE;  // return TRUE unless you set the focus to a control
}

/**---------------------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(PDlgDllLangText, CDialogMultiLang)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LI_LANG_TEXT_APP, OnItemchangedAppListCtrl)
	ON_NOTIFY(NM_DBLCLK, IDC_LI_LANG_TEXT_APP, OnDblclkAppListCtrl)
	ON_NOTIFY(NM_DBLCLK, IDC_LI_LANG_ALL, OnDblclkLangALLListCtrl)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LI_LANG_ALL, OnItemchangedLangALLListCtrl)
	ON_NOTIFY(NM_DBLCLK, IDC_LI_LANG_COR, OnDblclkLangCORListCtrl)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LI_LANG_COR, OnItemchangedLangCORListCtrl)
	
	ON_NOTIFY(NM_SETFOCUS, IDC_LI_LANG_COR, OnNMSetfocusLangCOR)
	ON_NOTIFY(NM_SETFOCUS, IDC_LI_LANG_ALL, OnNMSetfocusLangALL)
	ON_NOTIFY(NM_CLICK, IDC_LI_LANG_COR, OnNMClickLangCOR)
	ON_NOTIFY(NM_CLICK, IDC_LI_LANG_ALL, OnNMClickLangALL)
	ON_BN_CLICKED(IDC_BT_LANG_TEXT_SEL_APP, OnBtLangTextSelApp)
	ON_BN_CLICKED(IDC_BT_LANG_COR, OnBtLangCor)
	ON_BN_CLICKED(IDC_BT_LANG_ERASE, OnBtLangErase)
	ON_BN_CLICKED(IDC_BT_WORK_CORLANG, OnBtWorkCorLang)
	ON_BN_CLICKED(IDC_BT_LANG_NEW, OnBtLangNew)
	ON_BN_CLICKED(IDC_BT_LANG_WORK, OnBtLangWork)
	ON_BN_CLICKED(IDC_BT_LANG_DEL, OnBtLangDel)
	ON_BN_CLICKED(IDC_BT_LANG_TEXT_EXIT, OnBtLangTextExit)
END_MESSAGE_MAP()

/**---------------------------------------------------------------------------*/
void PDlgDllLangText::OnBtLangTextSelApp()
{
	BeginWaitCursor();
	
	int		  iItemApp = -1;
	m_sAppid = (short) m_pli_App->getNextSelectedBsnObj(&iItemApp)->getAppid();
	
	if(iItemApp >= 0)
	{
    //load
	m_pli_LangALL->giveCon().setAppid(m_sAppid);
	m_pli_LangALL->giveCon().setBAll(true);
	m_pli_LangALL->giveCon().setBList(false);
	
	if (m_pli_LangALL->load() == FALSE) 
		SetLangALLCtrlState(-1);
	else
		SetLangALLCtrlState(1);
	

	m_pli_LangCOR->giveCon().setAppid(m_sAppid);
	m_pli_LangCOR->giveCon().setBAll(false);
	m_pli_LangCOR->giveCon().setBList(false);

	if (m_pli_LangCOR->load() == FALSE) 
		SetLangCORCtrlState(-1);
	else
		SetLangCORCtrlState(1);

	
	//Enable-Ctrls
	m_bt_Lang_New.EnableWindow(true);
	
	m_pli_LangALL->EnableWindow(true);
	m_pli_LangCOR->EnableWindow(true);
	
	m_pli_LangALL->SetFocus();
	}
	else
	{
		MsgBoxOK(IDS_SELECT_ITEM);		
	}

	EndWaitCursor();
}

/**---------------------------------------------------------------------------*/
void PDlgDllLangText::OnBtLangCor()
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
	
	//zuordnen
	BeginWaitCursor();

	POSITION pos = m_pli_LangALL->GetFirstSelectedItemPosition();
	BppLanguage * pLang;

	C_VecItem * pVItems = new C_VecItem();
	int sCount = 0;
	int iItem = 0;

	while(pos) //Mehrfachauswahl möglich
	{
		sCount++;
		iItem =  m_pli_LangALL->GetNextSelectedItem(pos);
		pVItems->push_back(iItem);
	}
	
	while (sCount > 0)
	{
		iItem = pVItems->back();
		if (iItem >= 0)
		{
		pLang = m_pli_LangALL->giveCon().getBsnObj(m_pli_LangALL->getIdx(iItem));
		pLang->setAppid(m_sAppid);
		
		IPerst::ERet eRet = pLang->insertCorLang();
		if (eRet == IPerst::RET_OK) //Fehler bei Insert in DB
		{
			m_pli_LangCOR->giveCon().addBsnObj(*pLang);
			m_pli_LangALL->giveCon().clearBsnObj(m_pli_LangALL->getIdx(iItem));
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

	m_pli_LangALL->SetFocus();
	
	m_pli_LangCOR->load(m_bLoadNew );
	m_pli_LangALL->load(m_bLoadNew);
	
	delete pVItems;

	EndWaitCursor();
}

/**---------------------------------------------------------------------------*/
void PDlgDllLangText::OnBtLangErase()
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

	BppLanguage * pLang;
	
	int iItem = -1;
	POSITION pos = m_pli_LangCOR->GetFirstSelectedItemPosition();
	iItem =  m_pli_LangCOR->GetNextSelectedItem(pos);
	
	if (iItem >= 0)
	{
	pLang = m_pli_LangCOR->giveCon().getBsnObj(m_pli_LangCOR->getIdx(iItem));
	pLang->setAppid(m_sAppid);

	CString csDeleteSure;
	csDeleteSure.Format(CResString::ex().getStrTblText(IDS_DELETE_SURE), 
						pLang->getLangid().c_str());
	INT_PTR iRet = MsgBoxYesNo(csDeleteSure);
		
	if(iRet == IDYES)
	{
		IPerst::ERet eRet = pLang->deleteCorLang();
		if (eRet == IPerst::RET_OK) 
		{
			m_pli_LangALL->giveCon().addBsnObj(*pLang);
			m_pli_LangCOR->giveCon().clearBsnObj(m_pli_LangCOR->getIdx(iItem));
		}
		else
		{
			MsgBoxOK(IDS_ERR_DELETE);
		}
	
		m_pli_LangCOR->load(false);
		m_pli_LangALL->load(false);
	}
	
	m_pli_LangCOR->SetFocus();
	}
	else
	{
		MsgBoxOK(IDS_SELECT_ITEM);		
	}
	EndWaitCursor();
}

/**---------------------------------------------------------------------------*/
void PDlgDllLangText::OnBtWorkCorLang()
{
	BeginWaitCursor();

	int       iItem   = -1;
	BppLanguage * pLang = m_pli_LangCOR->getNextSelectedBsnObj(&iItem);

	if (iItem >= 0)
	{
	PDlgLangDetail dlgLangDetail(pLang);
	dlgLangDetail.setBNewLang(false);
	
	EndWaitCursor();

	INT_PTR iRet = dlgLangDetail.DoModal();
	
	//clone zurückkopieren!
	if (dlgLangDetail.getBDataChanged())
	{
		dlgLangDetail.getCloneObj(m_pli_LangCOR->giveCon().getBsnObj(m_pli_LangCOR->getIdx(iItem)));
		if (iRet == IDOK)
			m_pli_LangCOR->refresh(iItem);
	}

	BeginWaitCursor();
	
	m_pli_LangCOR->SetFocus();
	}
	else
	{
		MsgBoxOK(IDS_SELECT_ITEM);		
	}
	EndWaitCursor();
}

/**---------------------------------------------------------------------------*/
void PDlgDllLangText::OnBtLangNew()
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

	BppLanguage * pLang = new BppLanguage;
		assert(pLang);
	
	PDlgLangDetail dlgLangDetail(pLang);
	dlgLangDetail.setBNewLang(true);
	
	EndWaitCursor();

	INT_PTR iRet = dlgLangDetail.DoModal();

	//clone zurückkopieren!
	if (dlgLangDetail.getBDataChanged())
	{
		dlgLangDetail.getCloneObj(pLang);
		if (iRet == IDOK)
		{
			m_pli_LangALL->giveCon().addBsnObj(*pLang);
			
			if (m_pli_LangALL->load(false) == FALSE) 
				SetLangALLCtrlState(-1);
		}
	}

	if(pLang != NULL)
	{
		delete pLang;
		pLang = NULL;
	}
	
	BeginWaitCursor();
	
	m_pli_LangALL->SetFocus();
	
	EndWaitCursor();
}

/**---------------------------------------------------------------------------*/
void PDlgDllLangText::OnBtLangWork()
{
	BeginWaitCursor();

	int       iItem   = -1;
	BppLanguage * pLang = m_pli_LangALL->getNextSelectedBsnObj(&iItem);

	if (iItem >= 0)
	{
	PDlgLangDetail dlgLangDetail(pLang);
	dlgLangDetail.setBNewLang(false);
	
	EndWaitCursor();

	INT_PTR iRet = dlgLangDetail.DoModal();
	
	//clone zurückkopieren!
	if (dlgLangDetail.getBDataChanged())
	{
		dlgLangDetail.getCloneObj(m_pli_LangALL->giveCon().getBsnObj(m_pli_LangALL->getIdx(iItem)));
		if (iRet == IDOK)
			m_pli_LangALL->refresh(iItem);
	}

	BeginWaitCursor();
	
	m_pli_LangALL->SetFocus();
	}
	else
	{
		MsgBoxOK(IDS_SELECT_ITEM);		
	}
	EndWaitCursor();
}

/**---------------------------------------------------------------------------*/
void PDlgDllLangText::OnBtLangDel()
{
	CBString strRight = RIGHT_DEL_DLL;
	if (!(ppBLoginData::ex().isLegitimated(MODULE_DLL,strRight)))
	{
		MsgBoxOK(IDS_NO_RIGHT);
		return;
	}
	
	BeginWaitCursor();

	int       iItem   = -1;
	BppLanguage * pLang = m_pli_LangALL->getNextSelectedBsnObj(&iItem);

	if (iItem >= 0)
	{
	//Sicherheitsabfrage
	CString csDeleteSure;
	csDeleteSure.Format(CResString::ex().getStrTblText(IDS_DELETE_SURE_ALL_APP), 
						pLang->getLangid().c_str());
	INT_PTR iRet = MsgBoxYesNo(csDeleteSure);
		
	if(iRet == IDYES)
	{
		IPerst::ERet eRet = pLang->delLang();
		if (eRet == IPerst::RET_OK) //Fehler bei Delete auf DB
		{
			m_pli_LangALL->giveCon().clearBsnObj(m_pli_LangALL->getIdx(iItem));
			m_pli_LangALL->load(false);
		}
		else
		{
			MsgBoxOK(IDS_ERR_DELETE); 
		}
	}
	
	m_pli_LangALL->SetFocus();
	}
	else
	{
		MsgBoxOK(IDS_SELECT_ITEM);		
	}
	EndWaitCursor();
}

/**---------------------------------------------------------------------------*/
void PDlgDllLangText::OnBtLangTextExit()
{
	CDialogMultiLang::OnCancel();
}

/**---------------------------------------------------------------------------*/
void PDlgDllLangText::SetLangCORCtrlState(const int iItem)
{
	BOOL    bPrint,
		    bAnzeige;


	if (iItem == -1)					// nichts markiert
	{
		bPrint       = (m_pli_LangCOR->GetItemCount() > 0) ? TRUE : FALSE;		// ListCtrl nicht leer
		bAnzeige     = FALSE;

	}
	else								// Zeile markiert
	{
		bPrint   = TRUE;
		bAnzeige = TRUE;
	}

	// setzen der Controls
	m_bt_Erase_Lang.EnableWindow(bAnzeige);
	m_bt_Work_CORLang.EnableWindow(bAnzeige);
}

/**---------------------------------------------------------------------------*/
void PDlgDllLangText::SetLangALLCtrlState(const int iItem)
{
	BOOL    bPrint,
		    bAnzeige;


	if (iItem == -1)					// nichts markiert
	{
		bPrint       = (m_pli_LangALL->GetItemCount() > 0) ? TRUE : FALSE;		// ListCtrl nicht leer
		bAnzeige     = FALSE;

	}
	else								// Zeile markiert
	{
		bPrint   = TRUE;
		bAnzeige = TRUE;
	}

	// setzen der Controls
	m_bt_Cor_Lang.EnableWindow(bAnzeige);
	m_bt_Lang_Del.EnableWindow(bAnzeige);
	m_bt_Lang_Work.EnableWindow(bAnzeige);	
	m_bt_Lang_New.EnableWindow(bAnzeige);
	
}


/**---------------------------------------------------------------------------*/
void PDlgDllLangText::OnDblclkAppListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE * pNMIA = (NMITEMACTIVATE *)pNMHDR;

	if (pNMIA->iItem != -1)
	{
		TRACE(__FUNCTION__ ": Item %d\n", pNMIA->iItem);
	
		if (m_bt_SelApp.IsWindowEnabled())
			OnBtLangTextSelApp();
	}

	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgDllLangText::OnItemchangedAppListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
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

	SetLangCORCtrlState (-1);
	m_pli_LangCOR->EnableWindow(false);
	m_pli_LangCOR->clearAllBsnObj();
	
	SetLangALLCtrlState     (-1); 
	m_pli_LangALL->EnableWindow(false);
	m_pli_LangALL->clearAllBsnObj();
	
	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgDllLangText::OnDblclkLangCORListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE * pNMIA = (NMITEMACTIVATE *)pNMHDR;

	if (pNMIA->iItem != -1)
	{
		TRACE(__FUNCTION__ ": Item %d\n", pNMIA->iItem);
	
		if (m_bt_Work_CORLang.IsWindowEnabled())
			OnBtWorkCorLang();
	}

	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgDllLangText::OnItemchangedLangCORListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV   = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	static int   iSelect = -1;

	if (    ((pNMLV->uChanged & LVIF_TEXT) == LVIF_TEXT) 
		 && ( pNMLV->iItem                 == iSelect  ))
	{
		TRACE(__FUNCTION__ ": text changed on Item %d\n", iSelect);
		SetLangCORCtrlState(iSelect);
	}
		 
	if ((pNMLV->uChanged & LVIF_STATE) == LVIF_STATE) 
	{
		if ((pNMLV->uNewState & LVNI_SELECTED) == LVNI_SELECTED)			// wird selektiert
		{
			iSelect = pNMLV->iItem;
			TRACE(__FUNCTION__ ": state changed (LVNI_SELECTED) on Item %d\n", iSelect);

			SetLangCORCtrlState(iSelect);
		}
		else
		{
			if ((pNMLV->uOldState & LVNI_SELECTED) == LVNI_SELECTED)		// wird deselektiert
				SetLangCORCtrlState(-1);
		}
	}

	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgDllLangText::OnItemchangedLangALLListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV   = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	static int   iSelect = -1;

	if (    ((pNMLV->uChanged & LVIF_TEXT) == LVIF_TEXT) 
		 && ( pNMLV->iItem                 == iSelect  ))
	{
		TRACE(__FUNCTION__ ": text changed on Item %d\n", iSelect);
		SetLangALLCtrlState(iSelect);
	}
		 
	if ((pNMLV->uChanged & LVIF_STATE) == LVIF_STATE) 
	{
		if ((pNMLV->uNewState & LVNI_SELECTED) == LVNI_SELECTED)			// wird selektiert
		{
			iSelect = pNMLV->iItem;
			TRACE(__FUNCTION__ ": state changed (LVNI_SELECTED) on Item %d\n", iSelect);

			SetLangALLCtrlState(iSelect);
		}
		else
		{
			if ((pNMLV->uOldState & LVNI_SELECTED) == LVNI_SELECTED)		// wird deselektiert
				SetLangALLCtrlState(-1);
		}
	}

	*pResult = 0;
}
/**---------------------------------------------------------------------------*/
void PDlgDllLangText::OnDblclkLangALLListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE * pNMIA = (NMITEMACTIVATE *)pNMHDR;

	if (pNMIA->iItem != -1)
	{
		TRACE(__FUNCTION__ ": Item %d\n", pNMIA->iItem);
	
		if (m_bt_Lang_Work.IsWindowEnabled())
			OnBtLangWork();
	}

	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgDllLangText::OnNMSetfocusLangCOR(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE * pNMIA = (NMITEMACTIVATE *)pNMHDR;

	if (pNMIA->iItem == -1)
	{
		SetLangCORCtrlState	(-1);
	}
	else
	{
		SetLangCORCtrlState	(1);
	}
	SetLangALLCtrlState	   (-1);
	
	*pResult = 0;	
}

/**---------------------------------------------------------------------------*/
void PDlgDllLangText::OnNMSetfocusLangALL(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE * pNMIA = (NMITEMACTIVATE *)pNMHDR;

	if (pNMIA->iItem == -1)
	{
		SetLangALLCtrlState	(-1);
	}
	else
	{
		SetLangALLCtrlState	(1);
	}
	SetLangCORCtrlState	   (-1);
	
	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgDllLangText::OnNMClickLangCOR(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE * pNMIA = (NMITEMACTIVATE *)pNMHDR;

	if (pNMIA->iItem == -1)
	{
		SetLangCORCtrlState	(-1);
	}
	else
	{
		SetLangCORCtrlState	(1);
	}
	SetLangALLCtrlState	   (-1);
	
	*pResult = 0;	
}

/**---------------------------------------------------------------------------*/
void PDlgDllLangText::OnNMClickLangALL(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE * pNMIA = (NMITEMACTIVATE *)pNMHDR;

	if (pNMIA->iItem == -1)
	{
		SetLangALLCtrlState	(-1);
	}
	else
	{
		SetLangALLCtrlState	(1);
	}
	SetLangCORCtrlState	   (-1);
	m_bt_Lang_New.EnableWindow(true);
		
	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
BOOL PDlgDllLangText::PreTranslateMessage(MSG* pMsg)
{
	assert(pMsg != 0);

	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			
			if (GetFocus() == GetDlgItem(IDC_LI_LANG_TEXT_APP))
			{
				if (m_bt_SelApp.IsWindowEnabled())
				{
					OnBtLangTextSelApp();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_LI_LANG_COR))
			{
				if (m_bt_Work_CORLang.IsWindowEnabled())
				{
					OnBtWorkCorLang();
					return TRUE;
				}
			}
			
			if (GetFocus() == GetDlgItem(IDC_LI_LANG_ALL))
			{
				if (m_bt_Lang_Work.IsWindowEnabled())
				{
					OnBtLangWork();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_BT_LANG_TEXT_EXIT))
			{
				if (m_bt_Lang_Text_Exit.IsWindowEnabled())
				{
					OnBtLangTextExit();
					return TRUE;
				}
			}	
			if (GetFocus() == GetDlgItem(IDC_BT_LANG_COR))
			{
				if (m_bt_Cor_Lang.IsWindowEnabled())
				{
					OnBtLangCor();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_BT_LANG_ERASE))
			{
				if (m_bt_Erase_Lang.IsWindowEnabled())
				{
					OnBtLangErase();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_BT_WORK_CORLANG))
			{
				if (m_bt_Work_CORLang.IsWindowEnabled())
				{
					OnBtWorkCorLang();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_BT_LANG_NEW))
			{
				if (m_bt_Lang_New.IsWindowEnabled())
				{
					OnBtLangNew();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_BT_LANG_WORK))
			{
				if (m_bt_Lang_Work.IsWindowEnabled())
				{
					OnBtLangWork();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_BT_LANG_DEL))
			{
				if (m_bt_Lang_Del.IsWindowEnabled())
				{
					OnBtLangDel();
					return TRUE;
				}
			}	
			return FALSE;

		case VK_ESCAPE:
			if (m_bt_Lang_Text_Exit.IsWindowEnabled())
			{
				OnBtLangTextExit();
				return TRUE;
			}
			return FALSE;


		default:
			break;
		}
	} 

	return CDialogMultiLang::PreTranslateMessage(pMsg);
}

