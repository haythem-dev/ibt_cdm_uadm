/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH			                                  */
/**   Date   : 25.05.2004                                                     */
/**                                                                           */
/******************************************************************************/

/*----------------------------------------------------------------------------*/
// Includes
/*----------------------------------------------------------------------------*/
#include "bsn.h"

#include <CConnect.h>

#include <SLog.h>
#include <libscmlogin/libscmlogin.h>

#include <sys/stat.h>
#include <windows.h>

#include "BConnect.h"

#define UADM4TEST_FILE "uadm4test.cfg"	//!< file to search for in exe´s directory; if exists (no content required), uadm is connected to test

/**---------------------------------------------------------------------------*/
VSingleton<BConnect>::ExemplarPtr VSingleton<BConnect>::m_objExemplar;

/**---------------------------------------------------------------------------*/
BConnect::BConnect() : m_bOpen  (false),
                       m_bChange(true)
{
	 m_objVdooConnMain = new CVdooConn;
	   assert(m_objVdooConnMain);
	   
	 m_objVdooConnBackup = new CVdooConn;
	   assert(m_objVdooConnBackup);

	 m_hSockMain = NO_HANDLE;
	 m_hSockBackup = NO_HANDLE;

	 m_bNoLoginData = false;
}

/**---------------------------------------------------------------------------*/
BConnect::~BConnect()
{
	if (m_objVdooConnMain != NULL)
	{
		delete m_objVdooConnMain;
		m_objVdooConnMain = NULL;
	}

	if (m_objVdooConnBackup != NULL)
	{
		delete m_objVdooConnBackup;
		m_objVdooConnBackup = NULL;
	}
}

//----------------------------------------------------------------------------
// check if config file exists, if so then connection is made to test server 
bool BConnect::isUadm4Test() const
{
	CBString file;

	char path [MAX_PATH + 1] = { 0 };
	::GetModuleFileName(NULL, path, MAX_PATH);

	char drive[_MAX_DRIVE];
	char dir  [_MAX_DIR  ];
	char fname[_MAX_FNAME];
	char ext  [_MAX_EXT  ];

	_splitpath(path, drive, dir, fname, ext);

	file = drive;
	file += dir;

	file += UADM4TEST_FILE;
	
	struct stat fileStatus;
	return (-1 != ::stat(file.c_str(), &fileStatus));
}

/**---------------------------------------------------------------------------*/
bool BConnect::connect(CBString& strErrorMsg)
{
	if (m_bOpen == true)
	{
		if (m_bChange == true)
			disconnect(strErrorMsg);
	}

	if ( isUadm4Test() )
		CConnect::setTestHosts();
	else
		CConnect::setProdHosts();

	CConnect::setConnPara();
	
	if (m_bOpen == false)
	{
		m_objVdooConnMain->setHost(CConnect::HOST);
		m_objVdooConnMain->setPort(CConnect::VOODOO_PORT);
		m_objVdooConnMain->setDatabase(CConnect::DATABASE);
		m_objVdooConnMain->setVdooServ(CConnect::VOODOO_SERVER);
		m_objVdooConnMain->setIsoLevel(CVdooConn::ISOLEV_DIRTY_READ);
		m_objVdooConnMain->setExplain (false);
		m_objVdooConnMain->setLockModeWait(CVdooConn::LOCKMODE_NOTWAIT);
		m_objVdooConnMain->setLibrary(CConnect::VOODOO_SHAREDLIB);
	
		// socket
		if (m_objVdooConnMain->connectHost(&m_hSockMain) == false)
		{
			strErrorMsg.format("main-socket connect failed (%s)", 
				CVdooConn::getErrorMsg().c_str());

			return false;
		}
		
		// server library (.so)
		if (m_objVdooConnMain->loadLib() == false)
		{
			strErrorMsg.format("load main-server library failed (%s)!", 
				CVdooConn::getErrorMsg().c_str());

			if (m_objVdooConnMain->disconnectHost() == false)
			{
				strErrorMsg += "\nsocket disconnect failed (";
				strErrorMsg += CVdooConn::getErrorMsg();
				strErrorMsg += ")";
			}

			return false;
		}
		
		// database
		if (m_objVdooConnMain->openDatabase() == false)
		{
			strErrorMsg.format("open main-database failed (%s)!", 
				CVdooConn::getErrorMsg().c_str());

			if (m_objVdooConnMain->disconnectHost() == false)
			{
				strErrorMsg += "\nsocket disconnect failed (";
				strErrorMsg += CVdooConn::getErrorMsg();
				strErrorMsg += ")";
			}

			return false;
		}
		
		
		else //Open Backup-Connection
		{
			m_objVdooConnBackup->setHost(CConnect::HOST_BACKUP);
			m_objVdooConnBackup->setPort(CConnect::VOODOO_PORT);
			m_objVdooConnBackup->setDatabase(CConnect::DATABASE_BACKUP);
			m_objVdooConnBackup->setVdooServ(CConnect::VOODOO_SERVER);
			m_objVdooConnBackup->setIsoLevel(CVdooConn::ISOLEV_DIRTY_READ);
			m_objVdooConnBackup->setExplain (false);
			m_objVdooConnBackup->setLockModeWait(CVdooConn::LOCKMODE_NOTWAIT);
			m_objVdooConnBackup->setLibrary(CConnect::VOODOO_SHAREDLIB);

			// socket
			if (m_objVdooConnBackup->connectHost(&m_hSockBackup) == false)
			{
				strErrorMsg.format("backup-socket connect failed (%s)", 
					CVdooConn::getErrorMsg().c_str());

				return false;
			}

			// server library (.so)
			if (m_objVdooConnBackup->loadLib() == false)
			{
				strErrorMsg.format("load backup-server library failed (%s)!", 
					CVdooConn::getErrorMsg().c_str());

				if (m_objVdooConnBackup->disconnectHost() == false)
				{
					strErrorMsg += "\nsocket disconnect failed (";
					strErrorMsg += CVdooConn::getErrorMsg();
					strErrorMsg += ")";
				}

				return false;
			}
		
			// database
			if (m_objVdooConnBackup->openDatabase() == false)
			{
				strErrorMsg.format("open backup-database failed (%s)!", 
					CVdooConn::getErrorMsg().c_str());

				if (m_objVdooConnBackup->disconnectHost() == false)
				{
					strErrorMsg += "\nsocket disconnect failed (";
					strErrorMsg += CVdooConn::getErrorMsg();
					strErrorMsg += ")";
				}

				return false;
			}
		}

		if (CVdooConn::getCountOpenConn() != 2)
		{
			return false;
		}

		if (m_objVdooConnMain->isHostConnected() == false || m_objVdooConnBackup->isHostConnected() == false)
		{
			return false;
		}
		
		if (m_objVdooConnMain->setActive() == false || CVdooConn::getActiveHandle() != m_hSockMain )
		{
			return false;
		}

		m_bChange = false;
		m_bOpen   = true;

		SLog::ex().setActivConn(m_objVdooConnMain);
	}

	return true;
}

/**---------------------------------------------------------------------------*/
void BConnect::setNoLoginData ( bool bNoLog )
{
	m_bNoLoginData = bNoLog;
}

/**---------------------------------------------------------------------------*/
bool BConnect::disconnect(CBString& strErrorMsg)
{
	if (m_bOpen == true)
	{
		if (m_objVdooConnMain->setActive() == false || CVdooConn::getActiveHandle() != m_hSockMain)
		{
			return false;
		}
		else if (m_objVdooConnMain->closeDatabase() == false)
		{
			strErrorMsg.format("close main-database failed (%s)",
				CVdooConn::getErrorMsg().c_str());
		}

		SLog::ex().setActivConn(m_objVdooConnMain);
		
		if (m_bNoLoginData)
		{
			SLog::ex().write(__FUNCTION__, VAccess::RET_OK, 
			                    ": user 'unknown' logged out");
		}
		else
		{
			SLog::ex().write(__FUNCTION__, VAccess::RET_OK, 
			                    ": user '%s' logged out",
								scmod::ppBLoginData::ex().GetUsername().c_str());
		}

		if (m_objVdooConnMain->freeLib() == false)				// unload server library
		{
			strErrorMsg += "\nunload main-server library failed (";
			strErrorMsg += CVdooConn::getErrorMsg();
			strErrorMsg += ")";
		}

		if (m_objVdooConnMain->disconnectHost() == false)		// baut Socket-Verbindung ab
		{
			strErrorMsg += "\nmain-socket disconnect failed (";
			strErrorMsg += CVdooConn::getErrorMsg();
			strErrorMsg += ")";

			return false;
		}
		
		if (m_objVdooConnBackup->setActive() == false || CVdooConn::getActiveHandle() != m_hSockBackup)
		{
			return false;
		}
		else
		{
			if (m_objVdooConnBackup->closeDatabase() == false)
			{
				strErrorMsg.format("close backup-database failed (%s)",
					CVdooConn::getErrorMsg().c_str());
			}

			SLog::ex().setActivConn(m_objVdooConnBackup);
			
			if (m_bNoLoginData)
			{
				SLog::ex().write(__FUNCTION__, VAccess::RET_OK, 
									": user 'unknown' logged out");
			}
			else
			{
				SLog::ex().write(__FUNCTION__, VAccess::RET_OK, 
									": user '%s' logged out",
									scmod::ppBLoginData::ex().GetUsername().c_str());
			}

			if (m_objVdooConnBackup->freeLib() == false)				// unload server library
			{
				strErrorMsg += "\nunload backup-server library failed (";
				strErrorMsg += CVdooConn::getErrorMsg();
				strErrorMsg += ")";
			}

			if (m_objVdooConnBackup->disconnectHost() == false)		// baut Socket-Verbindung ab
			{
				strErrorMsg += "\nsocket backup-disconnect failed (";
				strErrorMsg += CVdooConn::getErrorMsg();
				strErrorMsg += ")";

				return false;
			}
		}
	
		if (CVdooConn::getCountOpenConn() < 1)
		{
			m_bOpen   = false;
		}
		else
		{
			strErrorMsg = m_strNotAllDisconn;
			return false;
		}
		m_bChange = true;
	}

	strErrorMsg = "";

	return true;
}

/**---------------------------------------------------------------------------*/
const CBString BConnect::getCurDatabase() const
{
	if (CVdooConn::getActiveHandle() == m_hSockMain)
	{
		return m_objVdooConnMain->getDatabase();
	}
	else
	{
		return m_objVdooConnBackup->getDatabase();
	}
}

/**---------------------------------------------------------------------------*/
const CBString BConnect::getServer() const
{
	if (CVdooConn::getActiveHandle() == m_hSockMain)
	{
		return m_objVdooConnMain->getHost();
	}
	else
	{
		return m_objVdooConnBackup->getHost();
	}
}

/**---------------------------------------------------------------------------*/
bool BConnect::beginWork(CBString& strErrorMsg)
{
	bool b = false;
	if (CVdooConn::getActiveHandle() == m_hSockMain)
	{
		if (m_objVdooConnMain->beginWork() == false)
		{
			strErrorMsg = CVdooConn::getErrorMsg();
		}
		else
			b = true;
	}
	else
	{
		if (m_objVdooConnBackup->beginWork() == false)
		{
			strErrorMsg = CVdooConn::getErrorMsg();
		}
		else
			b = true;
	}
	return b;
}

/**---------------------------------------------------------------------------*/
bool BConnect::commitWork(CBString& strErrorMsg)
{
	bool b = false;
	if (CVdooConn::getActiveHandle() == m_hSockMain)
	{
		if (m_objVdooConnMain->commitWork() == false)
		{
			strErrorMsg = CVdooConn::getErrorMsg();
		}
		else
			b = true;
	}
	else
	{
		if (m_objVdooConnBackup->commitWork() == false)
		{
			strErrorMsg = CVdooConn::getErrorMsg();
		}
		else
			b = true;
	}
	return b;
}

/**---------------------------------------------------------------------------*/
bool BConnect::rollbackWork	  (CBString& strErrorMsg)
{
	bool b = false;
	if (CVdooConn::getActiveHandle() == m_hSockMain)
	{
		if (m_objVdooConnMain->rollbackWork() == false)
		{
			strErrorMsg = CVdooConn::getErrorMsg();
		}
		else
			b = true;
	}
	else
	{
		if (m_objVdooConnBackup->rollbackWork() == false)
		{
			strErrorMsg = CVdooConn::getErrorMsg();
		}
		else
			b = true;
	}
	return b;
}

/**---------------------------------------------------------------------------*/
bool BConnect::changeConnection  (CBString& strErrorMsg)
{
	HNMP hSockActual;
	hSockActual = CVdooConn::getActiveHandle();
	
	if ( hSockActual == m_hSockMain )
	{
		if (m_objVdooConnBackup->setActive() == false)
		{
			char temp[254];
			strcpy( temp, m_strNotActivated.c_str());
			strErrorMsg.format(temp,
				CVdooConn::getErrorMsg().c_str());
			return false;
		}
		SLog::ex().setActivConn(m_objVdooConnBackup);
		return true;
	}
	
	if ( hSockActual ==  m_hSockBackup )
	{
		if (m_objVdooConnMain->setActive() == false)
		{
			char temp[254];
			strcpy (temp, m_strNotActivated.c_str());
			strErrorMsg.format(temp,
				CVdooConn::getErrorMsg().c_str());
			return false;
		}
		SLog::ex().setActivConn(m_objVdooConnMain);
		return true;
	}

	strErrorMsg = m_strNoActivConnect;
	return false;
	
}

/**---------------------------------------------------------------------------*/
bool BConnect::activateMain	  (CBString& strErrorMsg)
{
	HNMP hSockActual;
	hSockActual = CVdooConn::getActiveHandle();
	if (hSockActual != m_hSockMain)
	{
		if (m_objVdooConnMain->setActive() == false)
		{
			char temp[254];
			strcpy( temp, m_strNotActivated.c_str());
			strErrorMsg.format(temp,
				CVdooConn::getErrorMsg().c_str());
			return false;
		}
	}
	SLog::ex().setActivConn(m_objVdooConnMain);
	return true;
}

/**---------------------------------------------------------------------------*/
bool BConnect::activateBackup	  (CBString& strErrorMsg)
{
	HNMP hSockActual;
	hSockActual = CVdooConn::getActiveHandle();
	if (hSockActual != m_hSockBackup)
	{
		if (m_objVdooConnBackup->setActive() == false)
		{
			char temp[254];
			strcpy( temp, m_strNotActivated.c_str());
			strErrorMsg.format(temp,
				CVdooConn::getErrorMsg().c_str());
			return false;
		}
	}
	SLog::ex().setActivConn(m_objVdooConnBackup);
	return true;
}

/**---------------------------------------------------------------------------*/
const int BConnect::getSockMain		  () const
{
	return m_hSockMain;
}

/**---------------------------------------------------------------------------*/
const int BConnect::getSockBackup	  () const
{
	return m_hSockBackup;
}

/**---------------------------------------------------------------------------*/
void BConnect::setNotAct	(CBString strMsg)
{
	m_strNotActivated = strMsg;
}

/**---------------------------------------------------------------------------*/
void BConnect::setNoActiv	(CBString strMsg)
{
	m_strNoActivConnect = strMsg;
}

/**---------------------------------------------------------------------------*/
void BConnect::setNotDis	(CBString strMsg)
{
	m_strNotAllDisconn = strMsg;
}

/**---------------------------------------------------------------------------*/
void BConnect::writeToLog  (CBString& strMsg)
{
	//Log-Eintrag
	SLog::ex().write(__FUNCTION__, VAccess::RET_OK, 
						strMsg.c_str());
		
}

/**---------------------------------------------------------------------------*/
void BConnect::setVZ(short sVz)
{
	SLog::ex().setVZ(sVz);
}

/**---------------------------------------------------------------------------*/
void BConnect::setUID(long  lUid)
{
	SLog::ex().setUID(lUid);
}

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
