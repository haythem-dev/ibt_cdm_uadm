/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH                                         */
/**   Date   : 26.05.2004                                                     */
/**                                                                           */
/******************************************************************************/

#include "StdAfx.h"

#include <vector>
using std::vector;
class C_VecItem : public vector <int>{};

#include <CVersion.h>
#include "presentation.h"
#include <BppGroup.h>
#include <BConppGroup.h>
#include <BppUser.h>
#include <BConppUser.h>
#include <BppApp.h>
#include <BConppApp.h>
#include "PDlgUserListCtrl.h"
#include "PDlgPW.h"
#include "PDlgCorGrpUsr.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




/**---------------------------------------------------------------------------*/
// PDlgUserDetail-Dialogfeld
/**---------------------------------------------------------------------------*/
IMPLEMENT_DYNAMIC(PDlgCorGrpUsr, CDialogMultiLang)

/**---------------------------------------------------------------------------*/
PDlgCorGrpUsr::PDlgCorGrpUsr(BppGroup * pBppGroup)
				: CDialogMultiLang(PDlgCorGrpUsr::IDD )
{
	m_pBppGroup = pBppGroup;
		assert(m_pBppGroup);

	m_pli_CorUser = new PDlgUserListCtrl;
	   assert(m_pli_CorUser);

	m_pli_AllUser = new PDlgUserListCtrl;
	   assert(m_pli_AllUser);
	
	m_pPDlgPW = new PDlgPW;
	   assert(m_pPDlgPW);
	   
	m_bLoadNew = false;
	m_sAppid = 0;
}

/**---------------------------------------------------------------------------*/
PDlgCorGrpUsr::~PDlgCorGrpUsr()
{
	if (m_pli_CorUser != NULL)
	{
		delete m_pli_CorUser;
		m_pli_CorUser = NULL;
	}

	if (m_pli_AllUser != NULL)
	{
		delete m_pli_AllUser;
		m_pli_AllUser = NULL;
	}

	if (m_pPDlgPW != NULL)
	{
		delete m_pPDlgPW;
		m_pPDlgPW = NULL;
	}
}

/**---------------------------------------------------------------------------*/
void PDlgCorGrpUsr::DoDataExchange(CDataExchange* pDX)
{
	CDialogMultiLang::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LI_USR_COR  , *m_pli_CorUser);
	DDX_Control(pDX, IDC_LI_USR_ALL  , *m_pli_AllUser);
	DDX_Control(pDX, IDC_BT_EXIT_GRPCOR, m_bt_Exit_GrpCor);
	DDX_Control(pDX, IDC_BT_GRP_COR_USR, m_bt_Cor_User);
	DDX_Control(pDX, IDC_BT_GRP_COR_DEL, m_bt_Del_CorUsr);
	DDX_Control(pDX, IDC_TXT_GROUP, m_txt_Groupname);
	DDX_Control(pDX, IDC_ED_GRP_COR_USR_APP, m_ed_Appname);
	DDX_Control(pDX, IDC_CB_APP,		m_cb_App);
	DDX_Control(pDX, IDC_CB_GRP,		m_cb_Group);
	DDX_Control(pDX, IDC_BT_ANALOG,		m_bt_Analog);
}

/**---------------------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(PDlgCorGrpUsr, CDialogMultiLang)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LI_USR_ALL, OnItemchangedAllListCtrl)
	ON_NOTIFY(NM_DBLCLK, IDC_LI_USR_ALL, OnDblclkAllListCtrl)
	ON_NOTIFY(NM_DBLCLK, IDC_LI_USR_COR, OnDblclkCorListCtrl)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LI_USR_COR, OnItemchangedCorListCtrl)
	ON_BN_CLICKED(IDC_BT_EXIT_GRPCOR, OnBtExitGrpcor)
	ON_BN_CLICKED(IDC_BT_GRP_COR_USR, OnBtGrpCorUsr)
	ON_BN_CLICKED(IDC_BT_GRP_COR_DEL, OnBtGrpCorDel)
	ON_BN_CLICKED(IDC_BT_ANALOG, OnBtAnalog)
	ON_NOTIFY(NM_SETFOCUS, IDC_LI_USR_COR, OnNMSetfocusLiUsrCor)
	ON_NOTIFY(NM_SETFOCUS, IDC_LI_USR_ALL, OnNMSetfocusLiUsrAll)
	ON_NOTIFY(NM_CLICK, IDC_LI_USR_COR, OnNMClickLiUsrCor)
	ON_NOTIFY(NM_CLICK, IDC_LI_USR_ALL, OnNMClickLiUsrAll)
END_MESSAGE_MAP()

/**---------------------------------------------------------------------------*/
void PDlgCorGrpUsr::setAppid(short sAppid)
{
	m_sAppid = sAppid;
}
/**---------------------------------------------------------------------------*/
void PDlgCorGrpUsr::setAppname	(CBString strAppname)
{
	m_strAppname = strAppname;
}

/**---------------------------------------------------------------------------*/
BOOL PDlgCorGrpUsr::OnInitDialog()
{
	BeginWaitCursor();

	CDialogMultiLang::OnInitDialog();

	m_txt_Groupname.SetWindowText(m_pBppGroup->getName().c_str());
	m_ed_Appname.SetWindowText(m_strAppname);
	
	m_pBppGroup->setAppid(m_sAppid);

	short sGroupid = (short)m_pBppGroup->getGroupid();
	m_pli_CorUser->giveCon().setGroupid(sGroupid);
	m_pli_AllUser->giveCon().setGroupid(sGroupid);
	m_pli_CorUser->giveCon().setAppid(m_sAppid);
	m_pli_AllUser->giveCon().setAppid(m_sAppid);

	m_pli_CorUser->giveCon().setCorUsrGrp(true);
	m_pli_AllUser->giveCon().setCorUsrGrp(false);

	m_pli_AllUser->init();
	m_pli_CorUser->init();
	
	if (m_pli_AllUser->load() == FALSE) 
		SetAllCtrlState(-1);

	if (m_pli_CorUser->load() == FALSE) 
		SetCorCtrlState(-1);

	m_pli_AllUser->SetFocus();

	//ComboBox APP füllen
	m_ConApp.load();
	BppApp			  * pApp = 0;
	unsigned int        uiCnt  = 0;
	
	while ((pApp = m_ConApp.getBsnObj(uiCnt++)) != 0)
	{
		//if ( pApp->getAppname() != m_strAppname )
			m_cb_App.AddString(pApp->getAppname().c_str());
	}
	m_cb_App.SetCurSel(0);

	//ComboBox GROUP füllen
	m_ConGroup.setAppid(m_sAppid);
	m_ConGroup.setBAll(true);
	m_ConGroup.load();
	
	BppGroup* pGroup = 0;
	uiCnt  = 0;
	
	while ((pGroup = m_ConGroup.getBsnObj(uiCnt++)) != 0)
	{
		m_cb_Group.AddString(pGroup->getName().c_str());
	}
	int iCurs = m_cb_Group.FindString(0, m_pBppGroup->getName().c_str());
	m_cb_Group.SetCurSel(iCurs);
	

	m_bt_Cor_User.EnableWindow(true);
	m_bt_Del_CorUsr.EnableWindow(false);

	EndWaitCursor();

	return FALSE;  // return TRUE unless you set the focus to a control
}


/**---------------------------------------------------------------------------*/
void PDlgCorGrpUsr::OnBtExitGrpcor()
{
	CDialogMultiLang::OnCancel();
}

/**---------------------------------------------------------------------------*/
void PDlgCorGrpUsr::OnBtAnalog()
{
	/* NASE - nicht auf VZ einschränkbar !  gewollt ? */
	CBString strRight = RIGHT_WORK_RI;
	if (!(ppBLoginData::ex().isLegitimated(MODULE_RIGHTS,strRight)))
	{
		if (!(ppBLoginData::ex().isLegitimated(MODULE_RIGHTS,m_strAppname)))
		{
			MsgBoxOK(IDS_NO_RIGHT);
			return;
		}
	}

	//Appid + Groupid ermitteln
	CString csAppName, csGrpName;
	CBString cbAppName, cbGrpName;
	int iAppid, iGroupid;
	m_cb_App.GetLBText(m_cb_App.GetCurSel(),csAppName);
	cbAppName = (LPCTSTR)csAppName;
	m_cb_Group.GetLBText(m_cb_Group.GetCurSel(),csGrpName);
	cbGrpName = (LPCTSTR)csGrpName;

	CString csWarning;
	csWarning.Format(CResString::ex().getStrTblText(IDS_GROUP_ANALOG), csAppName, csGrpName);
	INT_PTR iRet = MsgBoxYesNo(csWarning);
		
	if(iRet == IDYES)
	{
		BppApp			  * pApp = 0;
		unsigned int        uiCnt  = 0;
		
		while ((pApp = m_ConApp.getBsnObj(uiCnt++)) != 0)
		{
			if ( pApp->getAppname() == cbAppName )
			{
				iAppid = pApp->getAppid();
				break;
			}
		}
		
		BppGroup* pGrp = 0;
		uiCnt  = 0;
		
		while ((pGrp = m_ConGroup.getBsnObj(uiCnt++)) != 0)
		{
			if ( pGrp->getName() == cbGrpName )
			{
				iGroupid = pGrp->getGroupid();
				break;
			}
		}

		//Benutzer in Gruppe für diese App lesen
		BConppUser objConUser;
		objConUser.setAppid(iAppid);
		objConUser.setGroupid(iGroupid);
		objConUser.setCorUsrGrp(true);
		
		if (objConUser.load() != IPerst::RET_OK ) 
		{
			MsgBoxOK(IDS_ERR_LOAD_GRP_ANALOG);
			return;
		}

		//diese Benutzer für aktuelle App einfügen ( Update / Insert )
		BppUser			  * pUser  = 0;
		BppGroup		  * pGroup = new BppGroup;
		uiCnt  = 0;

		pGroup->setAppid(m_sAppid);
		pGroup->setGroupid(m_pBppGroup->getGroupid());
		
		// alle bisherigen Zuordnungen löschen!
		if (pGroup->deleteAppCorUserGroup() != IPerst::RET_OK ) 
		{
			MsgBoxOK(IDS_ERR_DEL_GRP_ANALOG);
			return;
		}

		while ((pUser = objConUser.getBsnObj(uiCnt++)) != 0)
		{
			pGroup->setUserid((short)pUser->getUserid());
			if (pGroup->insertCor() != IPerst::RET_OK ) 
			{
				MsgBoxOK(IDS_ERR_INS_GRP_ANALOG);
			}
		}

		//beide ListCtrl neu laden
		m_pli_CorUser->load();
		m_pli_AllUser->load();

		delete pGroup;
	}
}

/**---------------------------------------------------------------------------*/
void PDlgCorGrpUsr::OnBtGrpCorUsr()
{	
	/* NASE - nicht auf VZ einschränkbar !   gewollt ?*/
	CBString strRight = RIGHT_WORK_RI;
	if (!(ppBLoginData::ex().isLegitimated(MODULE_RIGHTS,strRight)))
	{
		if (!(ppBLoginData::ex().isLegitimated(MODULE_RIGHTS,m_strAppname)))
		{
			MsgBoxOK(IDS_NO_RIGHT);
			return;
		}
	}

	//Bei UADM nach Passwort fragen!!!
	if (m_strAppname == CVersion::PROGRAM_NAME)
	{
		m_pPDlgPW->DoModal();
		if (m_pPDlgPW->PwOK() == false)
		{
			MsgBoxOK(IDS_NO_RIGHT);
			return;
		}
	}
	
	//User zuordnen
	BeginWaitCursor();

	POSITION pos = m_pli_AllUser->GetFirstSelectedItemPosition();
	BppUser * pUser;

	C_VecItem * pVItems = new C_VecItem();
	int sCount = 0;
	int iItem = 0;

	while(pos) //Mehrfachauswahl möglich
	{
		sCount++;
		iItem =  m_pli_AllUser->GetNextSelectedItem(pos);
		pVItems->push_back(iItem);
	}
	
	while (sCount > 0)
	{
		iItem = pVItems->back();
		if (iItem >= 0)
		{
		pUser = m_pli_AllUser->giveCon().getBsnObj(m_pli_AllUser->getIdx(iItem));
		short sUserid = (short)pUser->getUserid();
		m_pBppGroup->setUserid(sUserid);

		IPerst::ERet eRet = m_pBppGroup->insertCor();
		if (eRet == IPerst::RET_OK) //Fehler bei Insert in DB
		{
			m_pli_CorUser->giveCon().addBsnObj(*pUser);
			m_pli_AllUser->giveCon().clearBsnObj(m_pli_AllUser->getIdx(iItem));
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

	m_pli_AllUser->SetFocus();
	
	m_pli_CorUser->load(m_bLoadNew);
	m_pli_AllUser->load(m_bLoadNew);
	
	delete pVItems;

	EndWaitCursor();
}

/**---------------------------------------------------------------------------*/
void PDlgCorGrpUsr::OnBtGrpCorDel()
{
	/* NASE - nicht auf VZ einschränkbar !   gewollt ? */
	CBString strRight = RIGHT_WORK_RI;
	if (!(ppBLoginData::ex().isLegitimated(MODULE_RIGHTS,strRight)))
	{
		if (!(ppBLoginData::ex().isLegitimated(MODULE_RIGHTS,m_strAppname)))
		{
			MsgBoxOK(IDS_NO_RIGHT);
			return;
		}
	}
	
	// Zuordnung entfernen
	BeginWaitCursor();

	BppUser * pUser;
	POSITION pos = m_pli_CorUser->GetFirstSelectedItemPosition();
	

	int iItem = -1;

	iItem =  m_pli_CorUser->GetNextSelectedItem(pos);
	if (iItem >= 0)
	{
	pUser = m_pli_CorUser->giveCon().getBsnObj(m_pli_CorUser->getIdx(iItem));
	short sUserid = (short)pUser->getUserid();
	m_pBppGroup->setUserid(sUserid);

	CString csDeleteSure;
	csDeleteSure.Format(CResString::ex().getStrTblText(IDS_DELETE_SURE), 
						pUser->getUsername().c_str());
	INT_PTR iRet = MsgBoxYesNo(csDeleteSure);
		
	if(iRet == IDYES)
	{
		IPerst::ERet eRet = m_pBppGroup->deleteCor();
		if (eRet == IPerst::RET_OK) 
		{
			m_pli_AllUser->giveCon().addBsnObj(*pUser);
			m_pli_CorUser->giveCon().clearBsnObj(m_pli_CorUser->getIdx(iItem));
		}
		else
		{
			MsgBoxOK(IDS_ERR_DELETE);
		}
	
		m_pli_CorUser->load(false);
		m_pli_AllUser->load(false);
	}
	
	m_pli_CorUser->SetFocus();
	}
	else
	{
		MsgBoxOK(IDS_SELECT_ITEM);		
	}
	EndWaitCursor();
}


/**---------------------------------------------------------------------------*/
void PDlgCorGrpUsr::OnDblclkCorListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE * pNMIA = (NMITEMACTIVATE *)pNMHDR;

	if (pNMIA->iItem != -1)
	{
		TRACE(__FUNCTION__ ": Item %d\n", pNMIA->iItem);
	
		if (m_bt_Del_CorUsr.IsWindowEnabled())
			OnBtGrpCorDel();
	}

	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgCorGrpUsr::OnDblclkAllListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE * pNMIA = (NMITEMACTIVATE *)pNMHDR;

	if (pNMIA->iItem != -1)
	{
		TRACE(__FUNCTION__ ": Item %d\n", pNMIA->iItem);
	
		if (m_bt_Cor_User.IsWindowEnabled())
			OnBtGrpCorUsr();
	}

	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgCorGrpUsr::OnItemchangedAllListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV   = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	static int   iSelect = -1;

	if (    ((pNMLV->uChanged & LVIF_TEXT) == LVIF_TEXT) 
		 && ( pNMLV->iItem                 == iSelect  ))
	{
		TRACE(__FUNCTION__ ": text changed on Item %d\n", iSelect);
		SetAllCtrlState(iSelect);
	}
		 
	if ((pNMLV->uChanged & LVIF_STATE) == LVIF_STATE) 
	{
		if ((pNMLV->uNewState & LVNI_SELECTED) == LVNI_SELECTED)			// wird selektiert
		{
			iSelect = pNMLV->iItem;
			TRACE(__FUNCTION__ ": state changed (LVNI_SELECTED) on Item %d\n", iSelect);

			SetAllCtrlState(iSelect);
		}
		else
		{
			if ((pNMLV->uOldState & LVNI_SELECTED) == LVNI_SELECTED)		// wird deselektiert
				SetAllCtrlState(-1);
		}
	}

	m_bt_Del_CorUsr.EnableWindow(false);
	
	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgCorGrpUsr::OnItemchangedCorListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV   = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	static int   iSelect = -1;

	if (    ((pNMLV->uChanged & LVIF_TEXT) == LVIF_TEXT) 
		 && ( pNMLV->iItem                 == iSelect  ))
	{
		TRACE(__FUNCTION__ ": text changed on Item %d\n", iSelect);
		SetCorCtrlState(iSelect);
	}
		 
	if ((pNMLV->uChanged & LVIF_STATE) == LVIF_STATE) 
	{
		if ((pNMLV->uNewState & LVNI_SELECTED) == LVNI_SELECTED)			// wird selektiert
		{
			iSelect = pNMLV->iItem;
			TRACE(__FUNCTION__ ": state changed (LVNI_SELECTED) on Item %d\n", iSelect);

			SetCorCtrlState(iSelect);
		}
		else
		{
			if ((pNMLV->uOldState & LVNI_SELECTED) == LVNI_SELECTED)		// wird deselektiert
				SetCorCtrlState(-1);
		}
	}

	m_bt_Cor_User.EnableWindow(false);
	
	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgCorGrpUsr::SetCorCtrlState(const int iItem)
{
	BOOL    bPrint,
		    bAnzeige;


	if (iItem == -1)					// nichts markiert
	{
		bPrint       = (m_pli_CorUser->GetItemCount() > 0) ? TRUE : FALSE;		// ListCtrl nicht leer
		bAnzeige     = FALSE;

	}
	else								// Zeile markiert
	{
		bPrint   = TRUE;
		bAnzeige = TRUE;
	}

	// setzen der Controls
	m_bt_Del_CorUsr.EnableWindow(bAnzeige);
}

/**---------------------------------------------------------------------------*/
void PDlgCorGrpUsr::SetAllCtrlState(const int iItem)
{
	BOOL    bPrint,
		    bAnzeige;


	if (iItem == -1)					// nichts markiert
	{
		bPrint       = (m_pli_AllUser->GetItemCount() > 0) ? TRUE : FALSE;		// ListCtrl nicht leer
		bAnzeige     = FALSE;

	}
	else								// Zeile markiert
	{
		bPrint   = TRUE;
		bAnzeige = TRUE;
	}

	// setzen der Controls
	m_bt_Cor_User.EnableWindow(bAnzeige);
}

/**---------------------------------------------------------------------------*/
BOOL PDlgCorGrpUsr::PreTranslateMessage(MSG* pMsg)
{
	assert(pMsg != 0);

	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			
			if (GetFocus() == GetDlgItem(IDC_BT_GRP_COR_USR))
			{
				if (m_bt_Cor_User.IsWindowEnabled())
				{
					OnBtGrpCorUsr();
					return TRUE;
				}
			}

			if (GetFocus() == GetDlgItem(IDC_BT_ANALOG))
			{
				if (m_bt_Analog.IsWindowEnabled())
				{
					OnBtAnalog();
					return TRUE;
				}
			}
			
			if (GetFocus() == GetDlgItem(IDC_BT_GRP_COR_DEL))
			{
				if (m_bt_Del_CorUsr.IsWindowEnabled())
				{
					OnBtGrpCorDel();
					return TRUE;
				}
			}
			
			if (GetFocus() == GetDlgItem(IDC_BT_EXIT_GRPCOR))
			{
				if (m_bt_Exit_GrpCor.IsWindowEnabled())
				{
					OnBtExitGrpcor();
					return TRUE;
				}
			}
			return FALSE;

		case VK_ESCAPE:
			if (m_bt_Exit_GrpCor.IsWindowEnabled())
			{
				OnBtExitGrpcor();
				return TRUE;
			}
			return FALSE;


		default:
			break;
		}

		// Suche in User-Liste
		if (GetFocus() == GetDlgItem(IDC_LI_USR_COR))
		{
			if (pMsg->wParam >= CHAR_A && pMsg->wParam <= CHAR_Z)
			{
				CBString cbStr;
				cbStr.format("%c",char(pMsg->wParam));
				m_pli_CorUser->searchSurname(cbStr);
				return TRUE;
			}
		}

		// Suche in User-Liste
		if (GetFocus() == GetDlgItem(IDC_LI_USR_ALL))
		{
			if (pMsg->wParam >= CHAR_A && pMsg->wParam <= CHAR_Z)
			{
				CBString cbStr;
				cbStr.format("%c",char(pMsg->wParam));
				m_pli_AllUser->searchSurname(cbStr);
				return TRUE;
			}
		}
	} 

	return CDialogMultiLang::PreTranslateMessage(pMsg);
}

/**---------------------------------------------------------------------------*/
void PDlgCorGrpUsr::OnNMSetfocusLiUsrCor(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE * pNMIA = (NMITEMACTIVATE *)pNMHDR;

	if (pNMIA->iItem == -1)
	{
		m_bt_Del_CorUsr.EnableWindow(false);
	}
	else
	{
		m_bt_Del_CorUsr.EnableWindow(true);
	}
	m_bt_Cor_User.EnableWindow(false);
	
	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgCorGrpUsr::OnNMSetfocusLiUsrAll(NMHDR *pNMHDR, LRESULT *pResult)
{
	
	NMITEMACTIVATE * pNMIA = (NMITEMACTIVATE *)pNMHDR;

	if (pNMIA->iItem == -1)
	{
		m_bt_Cor_User.EnableWindow(false);
	}
	else
	{
		m_bt_Cor_User.EnableWindow(true);
	}
	m_bt_Del_CorUsr.EnableWindow(false);

	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgCorGrpUsr::OnNMClickLiUsrCor(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE * pNMIA = (NMITEMACTIVATE *)pNMHDR;

	if (pNMIA->iItem == -1)
	{
		m_bt_Del_CorUsr.EnableWindow(false);
	}
	else
	{
		m_bt_Del_CorUsr.EnableWindow(true);
	}
	m_bt_Cor_User.EnableWindow(false);
	
	*pResult = 0;
}

/**---------------------------------------------------------------------------*/
void PDlgCorGrpUsr::OnNMClickLiUsrAll(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE * pNMIA = (NMITEMACTIVATE *)pNMHDR;

	if (pNMIA->iItem == -1)
	{
		m_bt_Cor_User.EnableWindow(false);
	}
	else
	{
		m_bt_Cor_User.EnableWindow(true);
	}
	m_bt_Del_CorUsr.EnableWindow(false);
	
	*pResult = 0;
}
