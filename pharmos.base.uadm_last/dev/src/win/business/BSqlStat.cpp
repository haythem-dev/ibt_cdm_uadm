//////////////////////////////////////////////////////////////////////
//
//     BSqlStat.cpp:
//          Ausführung aller abgesetzten SQL-Statements auf die Backup-DB
//          Nur eine(!) Instanz sinnvoll, daher Singleton-Pattern
//
//////////////////////////////////////////////////////////////////////

#include "bsn.h"
#include <CConnect.h>
#include "BConnect.h"
#include "SSqlStat.h"

/**---------------------------------------------------------------------------*/
VSingleton<BSqlStat>::ExemplarPtr VSingleton<BSqlStat>::m_objExemplar;
/**---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
// Konstruktor
//---------------------------------------------------------------------------
BSqlStat::BSqlStat()
{
}


//---------------------------------------------------------------------------
// Destruktor
//---------------------------------------------------------------------------
BSqlStat::~BSqlStat()
{
}


//---------------------------------------------------------------------------
IPerst::ERet BSqlStat::UpdateBackupMachine()
{
	CBString strError = "";
	if (BConnect::ex().changeConnection(strError) == false)
	{
		DbgMsg(__FUNCTION__ "%s", strError.c_str());
	}
	
	VAccess::ERet eRet = SSqlStat::ex().ExecuteSqlStat();
	CBString strHost= "";
		switch (eRet)
		{
		case VAccess::RET_OK:
			break;

		case VAccess::RET_NOTFOUND:
			if (CVdooConn::getActiveHandle() == BConnect::ex().getSockMain())
				strHost = CConnect::HOST;
			else
				strHost = CConnect::HOST_BACKUP;

			DbgMsg(__FUNCTION__ ": Backup-Update not found on %s! SQL-Stat: %s ", 
				strHost.c_str(), SSqlStat::ex().getStat().c_str());
			return IPerst::RET_WARNING;
	
		default:
			assert(0);
			return IPerst::RET_ERROR;
		}
	
	
	if (BConnect::ex().changeConnection(strError) == false)
	{
		DbgMsg(__FUNCTION__ "%s", strError.c_str());
	}

	return IPerst::RET_OK;

}

