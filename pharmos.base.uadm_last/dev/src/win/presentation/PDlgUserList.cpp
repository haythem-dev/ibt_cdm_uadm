/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                     */
/**   Date   : 26.05.2004                                                     */
/**                                                                           */
/******************************************************************************/

/*----------------------------------------------------------------------------*/
// Includes
/*----------------------------------------------------------------------------*/
#include "StdAfx.h"
#include "BConnect.h"
#include "CConnect.h"
#include <BppUser.h>
#include <BConppUser.h>
#include <BppApp.h>
#include <BConppApp.h>
#include "PDlgUserDetail.h"
#include "PDlgUserListCtrl.h"
#include "PDlgAppListCtrl.h"
#include "presentation.h"
#include "PDlgUserList.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*----------------------------------------------------------------------------*/
// PViewUsrLst
/**---------------------------------------------------------------------------*/
IMPLEMENT_DYNCREATE(PDlgUserList, CDialogMultiLang)

/**---------------------------------------------------------------------------*/
PDlgUserList::PDlgUserList()
	: CDialogMultiLang(PDlgUserList::IDD)
{
	   m_pli_User = new PDlgUserListCtrl;
	   assert(m_pli_User);

	   m_pli_App = new PDlgAppListCtrl;
	   assert(m_pli_App);
}

/**---------------------------------------------------------------------------*/
PDlgUserList::~PDlgUserList()
{
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
}

/**---------------------------------------------------------------------------*/
void PDlgUserList::DoDataExchange(CDataExchange* pDX)
{
	CDialogMultiLang::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LST_USER  , *m_pli_User);
	DDX_Control(pDX, IDC_BT_USR_DETAIL, m_bt_Show_UsrDtl);
	DDX_Control(pDX, IDC_BT_USREXIT, m_bt_Cancel);
	DDX_Control(pDX, IDC_LI_APP_USR, *m_pli_App);
	DDX_Control(pDX, IDC_TXT_SELAPP, m_txt_SelApp);
	DDX_Control(pDX, IDC_BT_SELAPP, m_bt_SelApp);
	DDX_Control(pDX, IDC_BT_USRNEW, m_bt_UsrNew);
	DDX_Control(pDX, IDC_BT_USRDEL, m_bt_UsrDel);
	DDX_Control(pDX, IDC_BT_ALLAPP, m_bt_AppALL);
}

/**---------------------------------------------------------------------------*/
BOOL PDlgUserList::OnInitDialog()
{
	
	BeginWaitCursor();

	CDialogMultiLang::OnInitDialog();

	// ### AppListCtrl
	m_pli_App->init();
	if (m_pli_App->load() == FALSE) 
		SetAppCtrlState(-1);
	
	m_pli_User->init();

	m_pli_App->SetFocus();
	
	//Controls
	m_bt_AppALL.EnableWindow(true);
	m_bt_SelApp.EnableWindow(true);
	m_bt_Show_UsrDtl.EnableWindow(FALSE);
	m_bt_UsrDel.EnableWindow(FALSE);
	m_txt_SelApp.SetWindowText("");
	
	EndWaitCursor();

	return FALSE;  // return TRUE unless you set the focus to a control
}

/**---------------------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(PDlgUserList, CDialogMultiLang)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LI_APP_USR, OnItemchangedAppListCtrl)
	ON_NOTIFY(NM_DBLCLK, IDC_LI_APP_USR, OnDblclkAppListCtrl)
	ON_NOTIFY(NM_DBLCLK, IDC_LST_USER, OnDblclkListCtrl)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LST_USER, OnItemchangedListCtrl)
	ON_NOTIFY(NM_CLICK, IDC_LST_USER, OnNMClickLiUsr)
	ON_BN_CLICKED(IDC_BT_USREXIT, OnBtUsrExit)
	ON_BN_CLICKED(IDC_BT_USR_DETAIL, OnBtUsrDtl)
	ON_BN_CLICKED(IDC_BT_SELAPP, OnBtSelApp)
	ON_BN_CLICKED(IDC_BT_ALLAPP, OnBtAllApp)
	ON_BN_CLICKED(IDC_BT_USRNEW, OnBtUsrNew)
	ON_BN_CLICKED(IDC_BT_USRDEL, OnBtUsrDel)
END_MESSAGE_MAP()




// PDlgUserList-Meldungshandler
/**---------------------------------------------------------------------------*/
void PDlgUserList::OnNMClickLiUsr(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE * pNMIA = (NMITEMACTIVATE *)pNMHDR;

	if (pNMIA->iItem == -1)
	{
		SetUsrCtrlState	(-1);
	}
	else
	{
		SetUsrCtrlState	(1);
	}
	m_bt_UsrNew.EnableWindow(true);
	
	
	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgUserList::OnDblclkListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE * pNMIA = (NMITEMACTIVATE *)pNMHDR;

	if (pNMIA->iItem != -1)
	{
		TRACE(__FUNCTION__ ": Item %d\n", pNMIA->iItem);
	
		if (m_bt_Show_UsrDtl.IsWindowEnabled())
			OnBtUsrDtl();
	}

	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgUserList::OnBtUsrDtl()
{
	BeginWaitCursor();

	int       iItem   = -1;
	BppUser * pUser = m_pli_User->getNextSelectedBsnObj(&iItem);
	if (iItem >= 0)
	{
		pUser->setPHXPasswd(""); //--> sonst assert(bCmp == false) in UsrDtl nicht erfüllt 
		pUser->setAESPasswd(""); //--> sonst assert(bCmp == false) in UsrDtl nicht erfüllt
	
		CBString strError = "";
	
		PDlgUserDetail dlgUserDetail(pUser);
		dlgUserDetail.setBNewUser(false);

		//User nur ändern, wenn auf Main-Server!
		if (BConnect::ex().getServer() != CConnect::HOST)
		{
			MsgBoxOK(IDS_USER_ONLY_READ, MB_ICONINFORMATION);
			dlgUserDetail.SetSaveBtDisabled();
		}

		EndWaitCursor();

		if (dlgUserDetail.getBLocked() == true)
		{
			BConnect::ex().rollbackWork(strError);
		}
		else
		{
			INT_PTR iRet = dlgUserDetail.DoModal();
	
			//clone zurückkopieren!
			if (dlgUserDetail.getBDataChanged())
			{
				dlgUserDetail.getCloneObj(m_pli_User->giveCon().getBsnObj(m_pli_User->getIdx(iItem)));
				if (iRet == IDOK)
					m_pli_User->refresh(iItem);
			}
			else
			{
				BConnect::ex().rollbackWork(strError);
				if (BConnect::ex().getServer() == CConnect::HOST)
				{
					CBString strMsg;
					strMsg.format(": userid '%d' UNLOCKED without updating",
							pUser->getUserid());
					BConnect::ex().writeToLog(strMsg);
				}
			}
		}

		BeginWaitCursor();
	
		m_pli_User->SetFocus();
	}
	else
	{
		MsgBoxOK(IDS_SELECT_ITEM);		
	}
	EndWaitCursor();
}

/**---------------------------------------------------------------------------*/
void PDlgUserList::OnBtUsrNew()
{
	//User nur ändern, wenn auf Main-Server!
	if (BConnect::ex().getServer() != CConnect::HOST)
	{
		MsgBoxOK(IDS_USER_UPD_NOT_FROM_BACKUP);
		m_pli_User->SetFocus();
		EndWaitCursor();
		return;
	}
	
	CBString strRight = RIGHT_WORK_RI;
	if (!(ppBLoginData::ex().isLegitimated(MODULE_RIGHTS,strRight)))
	{
		MsgBoxOK(IDS_NO_RIGHT);
		return;
	}
	
	BeginWaitCursor();

	BppUser * pUser = new BppUser;
		assert(pUser);

	pUser->setUserid(0);
	
	PDlgUserDetail dlgUserDetail(pUser);
	dlgUserDetail.setBNewUser(true);

	EndWaitCursor();

	INT_PTR iRet = dlgUserDetail.DoModal();

	//clone zurückkopieren!
	if (dlgUserDetail.getBDataChanged())
	{
		dlgUserDetail.getCloneObj(pUser);
		if (iRet == IDOK)
		{
			m_pli_User->giveCon().addBsnObj(*pUser);
			
			/*Liste aller User laden, da neuer User noch nirgends zugeordnet!*/
			OnBtAllApp();
		}
	}

	if(pUser != NULL)
	{
		delete pUser;
		pUser = NULL;
	}
}

/**---------------------------------------------------------------------------*/
void PDlgUserList::OnBtUsrDel()
{
	CBString strRight = RIGHT_USRDEL_RI;
	if (!(ppBLoginData::ex().isLegitimated(MODULE_RIGHTS,strRight)))
	{
		MsgBoxOK(IDS_NO_RIGHT);
		return;
	}	
	
	BeginWaitCursor();

	int       iItem   = -1;
	BppUser * pUser = m_pli_User->getNextSelectedBsnObj(&iItem);
	if (iItem >= 0)
	{
	//Sicherheitsabfrage
	CString csDeleteSure;
	csDeleteSure.Format(CResString::ex().getStrTblText(IDS_DELETE_SURE_ALL_APP), 
						pUser->getUsername().c_str());
	INT_PTR iRet = MsgBoxYesNo(csDeleteSure);
		
	if(iRet == IDYES)
	{
		IPerst::ERet eRet = pUser->delObj();
		if (eRet == IPerst::RET_OK) //Fehler bei Delete auf DB
		{
			m_pli_User->giveCon().clearBsnObj(m_pli_User->getIdx(iItem));
			m_pli_User->load(false);
		}
		else
		{
			MsgBoxOK(IDS_ERR_DELETE); 
		}
	}
	
	m_pli_User->SetFocus();
	}
	else
	{
		MsgBoxOK(IDS_SELECT_ITEM);		
	}
	EndWaitCursor();
}

/**---------------------------------------------------------------------------*/
void PDlgUserList::OnDblclkAppListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE * pNMIA = (NMITEMACTIVATE *)pNMHDR;

	if (pNMIA->iItem != -1)
	{
		TRACE(__FUNCTION__ ": Item %d\n", pNMIA->iItem);
	
		if (m_bt_SelApp.IsWindowEnabled())
			OnBtSelApp();
	}

	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgUserList::OnBtSelApp()
{
	BeginWaitCursor();

	//ausgewählten Wert anblenden:
	m_txt_SelApp.SetWindowText(m_csAppName);

	int   iItem  = -1;
	
	//where-condition
	short sAppID = (short) m_pli_App->getNextSelectedBsnObj(&iItem)->getAppid();
	
	if (iItem >= 0)
	{
	m_pli_User->giveCon().setAppid(sAppID);
	m_pli_User->giveCon().setCorUsrGrp(false);
	m_pli_User->giveCon().setGroupid(0);
	
	if (m_pli_User->load() == FALSE) 
		SetUsrCtrlState(-1); 

	m_pli_User->EnableWindow(true);
	m_bt_UsrNew.EnableWindow(true);
	m_pli_User->SetFocus();

	SetAppCtrlState(-1);
	}
	else
	{
		MsgBoxOK(IDS_SELECT_ITEM);		
	}
	EndWaitCursor();
}

/**---------------------------------------------------------------------------*/
void PDlgUserList::OnBtAllApp()
{
	BeginWaitCursor();
	
	//where-condition
	short sAppid = 0;
	m_pli_User->giveCon().setAppid(sAppid);
	m_pli_User->giveCon().setCorUsrGrp(false);
	m_pli_User->giveCon().setGroupid(0);
	
	if (m_pli_User->load() == FALSE) 
		SetUsrCtrlState(-1);

	m_txt_SelApp.SetWindowText(CResString::ex().getStrTblText(IDS_ALL_APP));

	m_pli_User->EnableWindow(true);
	m_bt_UsrNew.EnableWindow(true);
	m_pli_User->SetFocus();

	SetAppCtrlState(-1);

	EndWaitCursor();
}

/**---------------------------------------------------------------------------*/
void PDlgUserList::OnItemchangedAppListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
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

	m_csAppName = m_pli_App->GetItemText(iSelect,0);

	SetUsrCtrlState (-1);
	m_pli_User->EnableWindow(false);
	m_pli_User->clearAllBsnObj();
	
	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgUserList::OnItemchangedListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
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
	
	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgUserList::SetUsrCtrlState(const int iItem)
{
	BOOL    bPrint,
		    bAnzeige;


	if (iItem == -1)					// nichts markiert
	{
		bPrint       = (m_pli_User->GetItemCount() > 0) ? TRUE : FALSE;		// ListCtrl nicht leer
		bAnzeige     = FALSE;

	}
	else								// Zeile markiert
	{
		bPrint   = TRUE;
		bAnzeige = TRUE;
	}

	// setzen der Controls
	m_bt_Show_UsrDtl.EnableWindow(bAnzeige);
	m_bt_UsrDel.EnableWindow(bAnzeige);
	//m_bt_UsrNew.EnableWindow(bAnzeige);
}
/**---------------------------------------------------------------------------*/
void PDlgUserList::SetAppCtrlState(const int iItem)
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
	m_bt_SelApp.EnableWindow(bAnzeige);
}

/**---------------------------------------------------------------------------*/
BOOL PDlgUserList::PreTranslateMessage(MSG* pMsg)
{
	assert(pMsg != 0);

	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			// Fokus steht in UsrListCtrl
			if (GetFocus() == GetDlgItem(IDC_LST_USER))
			{
				if (m_bt_Show_UsrDtl.IsWindowEnabled())
				{
					OnBtUsrDtl();
					return TRUE;
				}
			}
			// Fokus steht in AppListCtrl
			if (GetFocus() == GetDlgItem(IDC_LI_APP_USR))
			{
				if (m_bt_SelApp.IsWindowEnabled())
				{
					OnBtSelApp();
					return TRUE;
				}
			}

			if (GetFocus() == GetDlgItem(IDC_BT_USR_DETAIL))
			{
				if (m_bt_Show_UsrDtl.IsWindowEnabled())
				{
					OnBtUsrDtl();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_BT_USRNEW))
			{
				if (m_bt_UsrNew.IsWindowEnabled())
				{
					OnBtUsrNew();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_BT_USRDEL))
			{
				if (m_bt_UsrDel.IsWindowEnabled())
				{
					OnBtUsrDel();
					return TRUE;
				}
			}
			if (GetFocus() == GetDlgItem(IDC_BT_ALLAPP))
			{
				if (m_bt_AppALL.IsWindowEnabled())
				{
					OnBtAllApp();
					return TRUE;
				}
			}

			if (GetFocus() == GetDlgItem(IDC_BT_USREXIT))
			{
				if (m_bt_Cancel.IsWindowEnabled())
				{
					OnBtUsrExit();
					return TRUE;
				}
			}
			return FALSE;

		case VK_ESCAPE:
			if (m_bt_Cancel.IsWindowEnabled())
			{
				OnBtUsrExit();
				return TRUE;
			}
			return FALSE;


		default:
			break;
		}
	
		// Suche in User-Liste
		if (GetFocus() == GetDlgItem(IDC_LST_USER))
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
void PDlgUserList::OnBtUsrExit()
{
	CDialogMultiLang::OnCancel();
}
