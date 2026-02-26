/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH                                         */
/**   Date   : 18.08.2004                                                     */
/**                                                                           */
/******************************************************************************/

/*----------------------------------------------------------------------------*/
#include "StdAfx.h"

#include "presentation.h"
#include "PDlgPW.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/*----------------------------------------------------------------------------*/
PDlgPW::PDlgPW() : CDialogMultiLang(PDlgPW::IDD)
{
	m_bPwOk = false;
}

/*----------------------------------------------------------------------------*/
// 
/*----------------------------------------------------------------------------*/
PDlgPW::~PDlgPW()
{

}

/*----------------------------------------------------------------------------*/
// 
/*----------------------------------------------------------------------------*/
void PDlgPW::DoDataExchange(CDataExchange* pDX)
{
	CDialogMultiLang::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ED_PW, m_ed_PW);
	DDX_Control(pDX, IDC_BT_PW_OK, m_bt_PW_OK);
	DDX_Control(pDX, IDC_BT_PW_EXIT, m_bt_PW_Exit);
}

/*----------------------------------------------------------------------------*/
BEGIN_MESSAGE_MAP(PDlgPW, CDialogMultiLang)
	ON_BN_CLICKED(IDC_BT_PW_OK, OnBtPwOk)
	ON_BN_CLICKED(IDC_BT_PW_EXIT, OnBtPwExit)
END_MESSAGE_MAP()



/*----------------------------------------------------------------------------*/
BOOL PDlgPW::OnInitDialog() 
{
	CDialogMultiLang::OnInitDialog();

	m_ed_PW.SetFocus();
	
	return TRUE;
}

/*----------------------------------------------------------------------------*/
void PDlgPW::OnBtPwOk()
{
	m_bPwOk = false;
	CString csPW;
	m_ed_PW.GetWindowText(csPW);
	CBString strPW = (LPCTSTR)csPW;
	strPW.erasespace(CBString::END);
	if (strPW != PW_UADM)
	{
		//PW falsch
		MsgBoxOK(IDS_CHECK_PW, MB_ICONERROR);
		return;
	}
	else
	{
		m_bPwOk = true;
		CDialogMultiLang::OnOK();
	}
}

/*----------------------------------------------------------------------------*/
bool PDlgPW::PwOK()
{
	return m_bPwOk;
}

/*----------------------------------------------------------------------------*/
void PDlgPW::OnBtPwExit()
{
	CDialogMultiLang::OnCancel();
}

/**---------------------------------------------------------------------------*/
BOOL PDlgPW::PreTranslateMessage(MSG* pMsg)
{
	assert(pMsg != 0);

	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:
				// Fokus steht in AppListCtrl
			if (GetFocus() == GetDlgItem(IDC_BT_PW_EXIT))
			{
				OnBtPwExit();
				return TRUE;
			}
			else
			{
				OnBtPwOk();
				return TRUE;
			}
			return FALSE;

		case VK_ESCAPE:
				OnBtPwExit();
				return TRUE;
			
		default:
			break;
		}
	} 

	return CDialogMultiLang::PreTranslateMessage(pMsg);
}
/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/


