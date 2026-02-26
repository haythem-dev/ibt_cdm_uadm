//////////////////////////////////////////////////////////////////////
//
//     SSqlStat.cpp:
//          Sammlung aller SQL-Statements, um diese am Ende auf die 
//			Alternativmaschine absetzen zu können.
//          Nur eine(!) Instanz sinnvoll, daher Singleton-Pattern
//
//////////////////////////////////////////////////////////////////////

#include "srv.h"
#include "SSqlStat.h"
		

/**---------------------------------------------------------------------------*/
VSingleton<SSqlStat>::ExemplarPtr VSingleton<SSqlStat>::m_objExemplar;
/**---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
// Konstruktor
//---------------------------------------------------------------------------
SSqlStat::SSqlStat()
{
	m_strSqlStat = "";
}


//---------------------------------------------------------------------------
// Destruktor
//---------------------------------------------------------------------------
SSqlStat::~SSqlStat()
{
}

//---------------------------------------------------------------------------
void SSqlStat::setStat (CBString & strStat)
{
	m_strSqlStat = strStat;
}

//---------------------------------------------------------------------------
const CBString SSqlStat::getStat ()
{
	return m_strSqlStat;
}

//---------------------------------------------------------------------------
VAccess::ERet SSqlStat::ExecuteSqlStat	()
{
	long lRet = SqlExecuteImm((char *)m_strSqlStat.c_str());

	switch (lRet)
	{
	case SQL_SUCCESS:
		return VAccess::RET_OK;

	case SQL_NOTFOUND:
		return VAccess::RET_NOTFOUND;

	default:
		SLog::ex().write(__FUNCTION__, VAccess::RET_ERROR, 
			        "on updating BACKUP (<%s>) = %ld",
					m_strSqlStat.c_str(),
					lRet);
		assert(0);
		return VAccess::RET_ERROR;
	}
}
