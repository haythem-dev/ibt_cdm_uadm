#if !defined(AFX_CONSOVA2_H__6DE7F421_6ECB_11D2_92F8_0060973B18D8__INCLUDED_)
#define AFX_CONSOVA2_H__6DE7F421_6ECB_11D2_92F8_0060973B18D8__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ConSoVa2.h : header file
//

#include <phxlib/hSrvCon.h>

#include "FeldBreite.h"
#include "RwProfile.h"

/////////////////////////////////////////////////////////////////////////////
// CConSoVa2 

class CConSoVa2 : public CCmdTarget
{

public:
	CConSoVa2();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations

protected:
	virtual void DoThingsBeforeCloseDatabase();

public:
	void InitValues( int ProgramNumber, int MaxMenus, char *Database );

	void CloseConnect( ); // neu 27.03.2001
	bool TryDefaultConnect( CString ServerNickName, char *ServerProgram );
	int  TryDialogConnect( char *ServerProgram );

	CString GetServerHandler( );
	CString GetServerName();
	CString GetDbaseName();
	CString GetUserName( );
	char*   GetRefToAnwender( );
	bool    IsSyncronized();
	bool	IsConnected();
	bool	IsStandardEngine();
	bool    IsSuperSuperUser();
	bool    DarfRechtePflegen();
	bool    DarfSubSuperAnlegen( );
	bool    DarfToolbarAendernPflegen( );
	bool    DarfToolbarAendern( );
	bool    DarfMenuPflegen( short Region, int MenuNr );
	bool	DarfMenuAendern( short Region, int MenuNr );
	bool    ReadUserRights( CString& cErrmld );
	CStringArray* GetLoginRechte();
	CUIntArray*   GetLoginRegion();


// Implementation
public:
	~CConSoVa2();

private:
	bool TryConnect( CString ServerName, CString DbaseName, CString ServerProgram );
//	bool TryConnect( CString ServerName, CString DbaseName );
	bool FindServerName( CString xxxPipeSrv, CString& ServerName,
					     CString& errmld );

	bool GetPasswdInfo( );
	long InitSuperUserRights( CString& cErrmld );

private:
	ChSrvCon m_Db;
	CRwProfile m_RegistryProfile;
	bool m_bConnected;
	bool m_bSynchronized;
	CString m_DbaseName;
	CString m_DefaultDbaseName;
	CString m_ServerName;
	CString m_ServerHandler;
	CString m_UserName;
	CString m_Password;
	CString m_Ek;
	CString m_AllgRechte;
//	CString m_FilialName;
//	short   m_FilialNr;
	char    m_Anwender[L_ANWENDER+1];
	bool	m_bStandardEngine;
	bool    m_bSuperSuperUser;
	bool	m_bSubSuper;
	bool    m_bCustomToolbar;
    CStringArray	m_RegioRechte;
    CUIntArray		m_Region;
	int m_nThisProgramNumber;
	int m_nMaxMenus;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONSOVA2_H__6DE7F421_6ECB_11D2_92F8_0060973B18D8__INCLUDED_)
