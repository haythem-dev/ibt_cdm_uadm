/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                     */
/**   Date   : 07.07.2004                                                     */
/**                                                                           */
/******************************************************************************/

/*----------------------------------------------------------------------------*/
// Includes
/*----------------------------------------------------------------------------*/
#include "StdAfx.h"
#include <BppLanguage.h>
#include <BConppLanguage.h>
#include <BppText.h>
#include <BConppText.h>
#include "PDlgTextListCtrl.h"
#include "PDlgLangListCtrl.h"
#include "PDlgTextDetail.h"
#include "presentation.h"
#include "PDlgDllText.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*----------------------------------------------------------------------------*/
// 
/**---------------------------------------------------------------------------*/
IMPLEMENT_DYNCREATE(PDlgDllText, CDialogMultiLang)

/**---------------------------------------------------------------------------*/
PDlgDllText::PDlgDllText()
	: CDialogMultiLang(PDlgDllText::IDD)
{

   m_pli_TextALL = new PDlgTextListCtrl;
	   assert(m_pli_TextALL);

    m_pli_Lang = new PDlgLangListCtrl;
	   assert(m_pli_Lang);

   m_pli_TextCOR = new PDlgTextListCtrl;
	   assert(m_pli_TextCOR);

}

/**---------------------------------------------------------------------------*/
PDlgDllText::~PDlgDllText()
{
	if (m_pli_TextALL != NULL)
	{
		delete m_pli_TextALL;
		m_pli_TextALL = NULL;
	}

	if (m_pli_Lang != NULL)
	{
		delete m_pli_Lang;
		m_pli_Lang = NULL;
	}
	
	if (m_pli_TextCOR != NULL)
	{
		delete m_pli_TextCOR;
		m_pli_TextCOR = NULL;
	}	
}

/**---------------------------------------------------------------------------*/
void PDlgDllText::DoDataExchange(CDataExchange* pDX)
{
	CDialogMultiLang::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LI_TXT_LANG,*m_pli_Lang);
	DDX_Control(pDX, IDC_LI_TXT_COR, *m_pli_TextCOR);
	DDX_Control(pDX, IDC_LI_TXT_ALL, *m_pli_TextALL);

	DDX_Control(pDX, IDC_BT_TXT_SEL_LANG, m_bt_Txt_SelLang);
	DDX_Control(pDX, IDC_BT_TXT_COR, m_bt_Txt_Cor);
	DDX_Control(pDX, IDC_BT_TXT_ERASE, m_bt_Txt_Erase);
	DDX_Control(pDX, IDC_BT_WORK_TXTCOR, m_bt_Txt_WorkCor);
	DDX_Control(pDX, IDC_BT_TXT_NEW, m_bt_Txt_New);
	DDX_Control(pDX, IDC_BT_TXT_WORK, m_bt_Txt_Work);
	DDX_Control(pDX, IDC_BT_TXT_DEL, m_bt_Txt_Del);
	DDX_Control(pDX, IDC_BT_TEXTE_EXIT, m_bt_Txt_Exit);
}

/**---------------------------------------------------------------------------*/
BOOL PDlgDllText::OnInitDialog()
{
	BeginWaitCursor();

	CDialogMultiLang::OnInitDialog();

	m_pli_Lang->init();
	m_pli_Lang->giveCon().setBList(true);
	m_pli_Lang->giveCon().setBAll(true);
	if (m_pli_Lang->load() == FALSE) 
		m_bt_Txt_SelLang.EnableWindow(false);
	
	m_pli_TextCOR->init();
	m_pli_TextALL->init();
	
	m_pli_Lang->SetFocus();
	
	//Controls
	SetTextCORCtrlState(-1);
	SetTextALLCtrlState(-1);
			
	EndWaitCursor();

	return FALSE;  // return TRUE unless you set the focus to a control
}

/**---------------------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(PDlgDllText, CDialogMultiLang)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LI_TXT_LANG, OnItemchangedLangListCtrl)
	ON_NOTIFY(NM_DBLCLK, IDC_LI_TXT_LANG, OnDblclkLangListCtrl)
	ON_NOTIFY(NM_DBLCLK, IDC_LI_TXT_ALL, OnDblclkTextALLListCtrl)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LI_TXT_ALL, OnItemchangedTextALLListCtrl)
	ON_NOTIFY(NM_DBLCLK, IDC_LI_TXT_COR, OnDblclkTextCORListCtrl)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LI_TXT_COR, OnItemchangedTextCORListCtrl)
	
	ON_NOTIFY(NM_SETFOCUS, IDC_LI_TXT_COR, OnNMSetfocusTextCOR)
	ON_NOTIFY(NM_SETFOCUS, IDC_LI_TXT_ALL, OnNMSetfocusTextALL)
	ON_NOTIFY(NM_CLICK, IDC_LI_TXT_COR, OnNMClickTextCOR)
	ON_NOTIFY(NM_CLICK, IDC_LI_TXT_ALL, OnNMClickTextALL)
	ON_BN_CLICKED(IDC_BT_TXT_SEL_LANG, OnBtTxtSelLang)
	ON_BN_CLICKED(IDC_BT_TXT_COR, OnBtTxtCor)
	ON_BN_CLICKED(IDC_BT_TXT_ERASE, OnBtTxtErase)
	ON_BN_CLICKED(IDC_BT_WORK_TXTCOR, OnBtWorkTxtcor)
	ON_BN_CLICKED(IDC_BT_TXT_NEW, OnBtTxtNew)
	//ON_BN_CLICKED(IDC_BT_TXT_WORK, OnBtTxtWork)
	ON_BN_CLICKED(IDC_BT_TXT_DEL, OnBtTxtDel)
	ON_BN_CLICKED(IDC_BT_TEXTE_EXIT, OnBtTexteExit)
END_MESSAGE_MAP()



/**---------------------------------------------------------------------------*/
void PDlgDllText::OnBtTxtSelLang()
{
	BeginWaitCursor();
	
	int		  iItem = -1;
	m_strLangid = m_pli_Lang->getNextSelectedBsnObj(&iItem)->getLangid();
	
	if(iItem >= 0)
	{
    //load
	m_pli_TextALL->giveCon().setAppid(1);
	m_pli_TextALL->giveCon().setModid(1);
	m_pli_TextALL->giveCon().setBCor(false);
	m_pli_TextALL->giveCon().setLangid(m_strLangid);
	
	if (m_pli_TextALL->load() == FALSE) 
		SetTextALLCtrlState(-1);
	else
		SetTextALLCtrlState(1);
	

	m_pli_TextCOR->giveCon().setAppid(1);
	m_pli_TextCOR->giveCon().setModid(1);
	m_pli_TextCOR->giveCon().setBCor(true);
	m_pli_TextCOR->giveCon().setLangid(m_strLangid);

	if (m_pli_TextCOR->load() == FALSE) 
		SetTextCORCtrlState(-1);
	else
		SetTextCORCtrlState(1);

	
	//Enable-Ctrls
	m_bt_Txt_New.EnableWindow(true);
	m_pli_TextALL->EnableWindow(true);
	m_pli_TextCOR->EnableWindow(true);
	
	m_pli_TextALL->SetFocus();
	}
	else
	{
		MsgBoxOK(IDS_SELECT_ITEM);		
	}

	EndWaitCursor();
}

/**---------------------------------------------------------------------------*/
void PDlgDllText::OnBtTxtCor()
{
	CBString strRight = RIGHT_WORK_DLL;//dllintern
	if (!(ppBLoginData::ex().isLegitimated(MODULE_DLL,strRight)))
	{
		MsgBoxOK(IDS_NO_RIGHT);
		return;
	}
	
	//zuordnen
	BeginWaitCursor();

	BppText * pText;

	int iItem = -1;
	POSITION pos = m_pli_TextALL->GetFirstSelectedItemPosition();
	iItem =  m_pli_TextALL->GetNextSelectedItem(pos);
		
	if (iItem >= 0)
	{
		pText = m_pli_TextALL->giveCon().getBsnObj(m_pli_TextALL->getIdx(iItem));
		pText->setLangid(m_strLangid);
		pText->setModid(1);
		pText->setAppid(1);

		PDlgTextDetail dlgTextDetail(pText);
		dlgTextDetail.setBNewRefText(false);
		dlgTextDetail.setBNewText(true);
	
		EndWaitCursor();

		INT_PTR iRet = dlgTextDetail.DoModal();
	
		//clone zurückkopieren!
		if (dlgTextDetail.getBDataChanged())
		{
			dlgTextDetail.getCloneObj(pText);
			if (iRet == IDOK)
			{
				m_pli_TextCOR->giveCon().addBsnObj(*pText);
				m_pli_TextALL->giveCon().clearBsnObj(m_pli_TextALL->getIdx(iItem));
			}				
		}
	}
	else
	{
		MsgBoxOK(IDS_SELECT_ITEM);		
	}

	m_pli_TextALL->SetFocus();
	
	m_pli_TextCOR->load(false);
	m_pli_TextALL->load(false);

	EndWaitCursor();
}

/**---------------------------------------------------------------------------*/
void PDlgDllText::OnBtTxtErase()
{
	CBString strRight = RIGHT_DEL_DLL;
	if (!(ppBLoginData::ex().isLegitimated(MODULE_DLL,strRight)))
	{
		MsgBoxOK(IDS_NO_RIGHT);
		return;
	}
	
	// Zuordnung entfernen
	BeginWaitCursor();

	if ( m_strLangid != "en")
	{
	
		BppText * pText;
		
		int iItem = -1;
		POSITION pos = m_pli_TextCOR->GetFirstSelectedItemPosition();
		iItem =  m_pli_TextCOR->GetNextSelectedItem(pos);
		
		if (iItem >= 0)
		{
		pText = m_pli_TextCOR->giveCon().getBsnObj(m_pli_TextCOR->getIdx(iItem));
		pText->setLangid(m_strLangid);
		pText->setModid(1);
		pText->setAppid(1);

		CString csDeleteSure;
		csDeleteSure.Format(CResString::ex().getStrTblText(IDS_DELETE_SURE_ALL_APP), 
							pText->getReftext().c_str());
		INT_PTR iRet = MsgBoxYesNo(csDeleteSure);
			
		if(iRet == IDYES)
		{
			IPerst::ERet eRet = pText->delLangText();
			if (eRet == IPerst::RET_OK) 
			{
				m_pli_TextALL->giveCon().addBsnObj(*pText);
				m_pli_TextCOR->giveCon().clearBsnObj(m_pli_TextCOR->getIdx(iItem));
			}
			else
			{
				MsgBoxOK(IDS_ERR_DELETE);
			}
		
			m_pli_TextCOR->load(false);
			m_pli_TextALL->load(false);
		}
		
		m_pli_TextCOR->SetFocus();
		}
		else
		{
			MsgBoxOK(IDS_SELECT_ITEM);		
		}
	}
	else
	{
		MsgBoxOK(IDS_NO_COR_TXT_EN_DEL);
	}

	EndWaitCursor();
}

/**---------------------------------------------------------------------------*/
void PDlgDllText::OnBtWorkTxtcor()
{
	BeginWaitCursor();

	int       iItem   = -1;
	BppText * pText = m_pli_TextCOR->getNextSelectedBsnObj(&iItem);

	pText->setLangid(m_strLangid);
	pText->setModid(1);
	pText->setAppid(1);

	if (iItem >= 0)
	{
	PDlgTextDetail dlgTextDetail(pText);
	dlgTextDetail.setBNewRefText(false);
	dlgTextDetail.setBNewText(false);
	
	EndWaitCursor();

	INT_PTR iRet = dlgTextDetail.DoModal();
	
	//clone zurückkopieren!
	if (dlgTextDetail.getBDataChanged())
	{
		dlgTextDetail.getCloneObj(m_pli_TextCOR->giveCon().getBsnObj(m_pli_TextCOR->getIdx(iItem)));
		if (iRet == IDOK)
			m_pli_TextCOR->refresh(iItem);
	}

	BeginWaitCursor();
	
	m_pli_TextCOR->SetFocus();
	}
	else
	{
		MsgBoxOK(IDS_SELECT_ITEM);		
	}
	EndWaitCursor();
}

/**---------------------------------------------------------------------------*/
void PDlgDllText::OnBtTxtNew()
{
	CBString strRight = RIGHT_WORK_DLL;//dllintern
	if (!(ppBLoginData::ex().isLegitimated(MODULE_DLL,strRight)))
	{
		MsgBoxOK(IDS_NO_RIGHT);
		return;
	}
	
	BeginWaitCursor();

	BppText * pText = new BppText;
		assert(pText);

	pText->setLangid("en"); 
	pText->setModid(1);
	pText->setAppid(1);

	PDlgTextDetail dlgTextDetail(pText);
	dlgTextDetail.setBNewRefText(true);
	dlgTextDetail.setBNewText(true);
	
	EndWaitCursor();

	INT_PTR iRet = dlgTextDetail.DoModal();

	//clone zurückkopieren!
	if (dlgTextDetail.getBDataChanged())
	{
		dlgTextDetail.getCloneObj(pText);
		if (iRet == IDOK)
		{
			if ( m_strLangid == "en")
			{
				m_pli_TextCOR->giveCon().addBsnObj(*pText);
			
				if (m_pli_TextCOR->load(false) == FALSE) 
					SetTextCORCtrlState(-1);
			}
			else
			{
				m_pli_TextALL->giveCon().addBsnObj(*pText);
			
				if (m_pli_TextALL->load(false) == FALSE) 
					SetTextALLCtrlState(-1);
			}
		}
	}

	if(pText != NULL)
	{
		delete pText;
		pText = NULL;
	}
	
	BeginWaitCursor();
	
	m_pli_TextALL->SetFocus();
	
	EndWaitCursor();
}

/**---------------------------------------------------------------------------*/
/*void PDlgDllText::OnBtTxtWork()
{
	BeginWaitCursor();

	int       iItem   = -1;
	BppText * pText = m_pli_TextALL->getNextSelectedBsnObj(&iItem);

	pText->setLangid("en");
	pText->setModid(1);
	pText->setAppid(1);

	if (iItem >= 0)
	{
	PDlgTextDetail dlgTextDetail(pText);
	m_pli_TextALL->giveCon()->getBsnObj(m_pli_TextALL->getIdx(iItem));
	dlgTextDetail.setBNewRefText(false);
	dlgTextDetail.setBNewText(false);
	
	EndWaitCursor();

	INT_PTR iRet = dlgTextDetail.DoModal();
	
	//clone zurückkopieren!
	if (dlgTextDetail.getBDataChanged())
	{
		dlgTextDetail.getCloneObj(m_pli_TextALL->giveCon()->getBsnObj(m_pli_TextALL->getIdx(iItem)));
		if (iRet == IDOK)
			m_pli_TextALL->refresh(iItem);
	}

	BeginWaitCursor();
	
	m_pli_TextALL->SetFocus();
	}
	else
	{
		MsgBoxOK(IDS_SELECT_ITEM);		
	}
	EndWaitCursor();
}
*/
/**---------------------------------------------------------------------------*/
void PDlgDllText::OnBtTxtDel()
{
	CBString strRight = RIGHT_DEL_DLL;
	if (!(ppBLoginData::ex().isLegitimated(MODULE_DLL,strRight)))
	{
		MsgBoxOK(IDS_NO_RIGHT);
		return;
	}
	
	BeginWaitCursor();

	int       iItem   = -1;
	BppText * pText = m_pli_TextALL->getNextSelectedBsnObj(&iItem);

	if (iItem >= 0)
	{
	pText->setModid(1);
	pText->setAppid(1);
	
	//Sicherheitsabfrage
	CString csDeleteSure;
	csDeleteSure.Format(CResString::ex().getStrTblText(IDS_DELETE_SURE_ALL_APP), 
						pText->getReftext().c_str());
	INT_PTR iRet = MsgBoxYesNo(csDeleteSure);
		
	if(iRet == IDYES)
	{
		IPerst::ERet eRet = pText->delRefText();
		if (eRet == IPerst::RET_OK) //Fehler bei Delete auf DB
		{
			m_pli_TextALL->giveCon().clearBsnObj(m_pli_TextALL->getIdx(iItem));
			m_pli_TextALL->load(false);
		}
		else
		{
			MsgBoxOK(IDS_ERR_DELETE); 
		}
	}
	
	m_pli_TextALL->SetFocus();
	}
	else
	{
		MsgBoxOK(IDS_SELECT_ITEM);		
	}
	EndWaitCursor();
}

/**---------------------------------------------------------------------------*/
void PDlgDllText::OnBtTexteExit()
{
	CDialogMultiLang::OnCancel();
}

/**---------------------------------------------------------------------------*/
void PDlgDllText::SetTextCORCtrlState(const int iItem)
{
	BOOL    bPrint,
		    bAnzeige;


	if (iItem == -1)					// nichts markiert
	{
		bPrint       = (m_pli_TextCOR->GetItemCount() > 0) ? TRUE : FALSE;		// ListCtrl nicht leer
		bAnzeige     = FALSE;

	}
	else								// Zeile markiert
	{
		bPrint   = TRUE;
		bAnzeige = TRUE;
	}

	// setzen der Controls
	m_bt_Txt_Erase.EnableWindow(bAnzeige);
	m_bt_Txt_WorkCor.EnableWindow(bAnzeige);
}

/**---------------------------------------------------------------------------*/
void PDlgDllText::SetTextALLCtrlState(const int iItem)
{
	BOOL    bPrint,
		    bAnzeige;


	if (iItem == -1)					// nichts markiert
	{
		bPrint       = (m_pli_TextALL->GetItemCount() > 0) ? TRUE : FALSE;		// ListCtrl nicht leer
		bAnzeige     = FALSE;

	}
	else								// Zeile markiert
	{
		bPrint   = TRUE;
		bAnzeige = TRUE;
	}

	// setzen der Controls
	m_bt_Txt_Cor.EnableWindow(bAnzeige);
	m_bt_Txt_Del.EnableWindow(bAnzeige);
	m_bt_Txt_Work.EnableWindow(bAnzeige);	
	m_bt_Txt_New.EnableWindow(bAnzeige);
}


/**---------------------------------------------------------------------------*/
void PDlgDllText::OnDblclkLangListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE * pNMIA = (NMITEMACTIVATE *)pNMHDR;

	if (pNMIA->iItem != -1)
	{
		TRACE(__FUNCTION__ ": Item %d\n", pNMIA->iItem);
	
		if (m_bt_Txt_SelLang.IsWindowEnabled())
			OnBtTxtSelLang();
	}

	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgDllText::OnItemchangedLangListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
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

	SetTextCORCtrlState (-1);
	m_pli_TextCOR->EnableWindow(false);
	m_pli_TextCOR->clearAllBsnObj();
	
	SetTextALLCtrlState     (-1); 
	m_pli_TextALL->EnableWindow(false);
	m_pli_TextALL->clearAllBsnObj();
	
	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgDllText::OnDblclkTextCORListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE * pNMIA = (NMITEMACTIVATE *)pNMHDR;

	if (pNMIA->iItem != -1)
	{
		TRACE(__FUNCTION__ ": Item %d\n", pNMIA->iItem);
	
		if (m_bt_Txt_WorkCor.IsWindowEnabled())
			OnBtWorkTxtcor();
	}

	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgDllText::OnItemchangedTextCORListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV   = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	static int   iSelect = -1;

	if (    ((pNMLV->uChanged & LVIF_TEXT) == LVIF_TEXT) 
		 && ( pNMLV->iItem                 == iSelect  ))
	{
		TRACE(__FUNCTION__ ": text changed on Item %d\n", iSelect);
		SetTextCORCtrlState(iSelect);
	}
		 
	if ((pNMLV->uChanged & LVIF_STATE) == LVIF_STATE) 
	{
		if ((pNMLV->uNewState & LVNI_SELECTED) == LVNI_SELECTED)			// wird selektiert
		{
			iSelect = pNMLV->iItem;
			TRACE(__FUNCTION__ ": state changed (LVNI_SELECTED) on Item %d\n", iSelect);

			SetTextCORCtrlState(iSelect);
		}
		else
		{
			if ((pNMLV->uOldState & LVNI_SELECTED) == LVNI_SELECTED)		// wird deselektiert
				SetTextCORCtrlState(-1);
		}
	}

	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgDllText::OnItemchangedTextALLListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV   = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	static int   iSelect = -1;

	if (    ((pNMLV->uChanged & LVIF_TEXT) == LVIF_TEXT) 
		 && ( pNMLV->iItem                 == iSelect  ))
	{
		TRACE(__FUNCTION__ ": text changed on Item %d\n", iSelect);
		SetTextALLCtrlState(iSelect);
	}
		 
	if ((pNMLV->uChanged & LVIF_STATE) == LVIF_STATE) 
	{
		if ((pNMLV->uNewState & LVNI_SELECTED) == LVNI_SELECTED)			// wird selektiert
		{
			iSelect = pNMLV->iItem;
			TRACE(__FUNCTION__ ": state changed (LVNI_SELECTED) on Item %d\n", iSelect);

			SetTextALLCtrlState(iSelect);
		}
		else
		{
			if ((pNMLV->uOldState & LVNI_SELECTED) == LVNI_SELECTED)		// wird deselektiert
				SetTextALLCtrlState(-1);
		}
	}

	*pResult = 0;
}
/**---------------------------------------------------------------------------*/
void PDlgDllText::OnDblclkTextALLListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE * pNMIA = (NMITEMACTIVATE *)pNMHDR;

	if (pNMIA->iItem != -1)
	{
		TRACE(__FUNCTION__ ": Item %d\n", pNMIA->iItem);
	
		//if (m_bt_Txt_Work.IsWindowEnabled())
		//	OnBtTxtWork();
	}

	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgDllText::OnNMSetfocusTextCOR(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE * pNMIA = (NMITEMACTIVATE *)pNMHDR;

	if (pNMIA->iItem == -1)
	{
		SetTextCORCtrlState	(-1);
	}
	else
	{
		SetTextCORCtrlState	(1);
	}
	SetTextALLCtrlState	   (-1);
	
	*pResult = 0;	
}

/**---------------------------------------------------------------------------*/
void PDlgDllText::OnNMSetfocusTextALL(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE * pNMIA = (NMITEMACTIVATE *)pNMHDR;

	if (pNMIA->iItem == -1)
	{
		SetTextALLCtrlState	(-1);
	}
	else
	{
		SetTextALLCtrlState	(1);
	}
	SetTextCORCtrlState	   (-1);
	
	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgDllText::OnNMClickTextCOR(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE * pNMIA = (NMITEMACTIVATE *)pNMHDR;

	if (pNMIA->iItem == -1)
	{
		SetTextCORCtrlState	(-1);
	}
	else
	{
		SetTextCORCtrlState	(1);
	}
	SetTextALLCtrlState	   (-1);
	
	*pResult = 0;	
}

/**---------------------------------------------------------------------------*/
void PDlgDllText::OnNMClickTextALL(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE * pNMIA = (NMITEMACTIVATE *)pNMHDR;

	if (pNMIA->iItem == -1)
	{
		SetTextALLCtrlState	(-1);
	}
	else
	{
		SetTextALLCtrlState	(1);
	}
	SetTextCORCtrlState	   (-1);
	m_bt_Txt_New.EnableWindow(true);
	
	
	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
BOOL PDlgDllText::PreTranslateMessage(MSG* pMsg)
{
	assert(pMsg != 0);

	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			
			if (GetFocus() == GetDlgItem(IDC_LI_TXT_LANG))
			{
				if (m_bt_Txt_SelLang.IsWindowEnabled())
				{
					OnBtTxtSelLang();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_LI_TXT_COR))
			{
				if (m_bt_Txt_WorkCor.IsWindowEnabled())
				{
					OnBtWorkTxtcor();
					return TRUE;
				}
			}
			
		/*	if (GetFocus() == GetDlgItem(IDC_LI_TXT_ALL))
			{
				if (m_bt_Txt_Work.IsWindowEnabled())
				{
					OnBtTxtWork();
					return TRUE;
				}
			}
		*/	if (GetFocus() == GetDlgItem(IDC_BT_TEXTE_EXIT))
			{
				if (m_bt_Txt_Exit.IsWindowEnabled())
				{
					OnBtTexteExit();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_BT_TXT_COR))
			{
				if (m_bt_Txt_Cor.IsWindowEnabled())
				{
					OnBtTxtCor();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_BT_TXT_ERASE))
			{
				if (m_bt_Txt_Erase.IsWindowEnabled())
				{
					OnBtTxtErase();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_BT_WORK_TXTCOR))
			{
				if (m_bt_Txt_WorkCor.IsWindowEnabled())
				{
					OnBtWorkTxtcor();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_BT_TXT_NEW))
			{
				if (m_bt_Txt_New.IsWindowEnabled())
				{
					OnBtTxtNew();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_BT_TXT_DEL))
			{
				if (m_bt_Txt_Del.IsWindowEnabled())
				{
					OnBtTxtDel();
					return TRUE;
				}
			}
			return FALSE;

		case VK_ESCAPE:
			if (m_bt_Txt_Exit.IsWindowEnabled())
			{
				OnBtTexteExit();
				return TRUE;
			}
			return FALSE;


		default:
			break;
		}
	} 

	return CDialogMultiLang::PreTranslateMessage(pMsg);
}
