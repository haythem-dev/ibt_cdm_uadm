/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH			                                  */
/**   Date   : 06.06.2005                                                     */
/**                                                                           */
/******************************************************************************/

/*----------------------------------------------------------------------------*/
// Includes
/*----------------------------------------------------------------------------*/
#include "bsn.h"

#include <SConnectOdbc.h>

#include "BConnectOdbc.h"

/**---------------------------------------------------------------------------*/
VSingleton<BConnectOdbc>::ExemplarPtr VSingleton<BConnectOdbc>::m_objExemplar;

/**---------------------------------------------------------------------------*/
BConnectOdbc::BConnectOdbc() : m_bOpen  (false)
{
	TSConnectOdbc::ex().setConnect("", "", ""); 
}

/**---------------------------------------------------------------------------*/
BConnectOdbc::~BConnectOdbc()
{
}

/**---------------------------------------------------------------------------*/
bool BConnectOdbc::connect(CBString& strErrorMsg)
{
	if (m_bOpen == true)
		disconnect(strErrorMsg);

	if (m_bOpen == false)
	{
		// ODBC connect
		if (TSConnectOdbc::ex().openConnect() == false)
		{
			strErrorMsg.format("open ODBC connect failed (%s)!", 
				               TSConnectOdbc::ex().getErrorMsg());
			return false;
		}

		m_bOpen   = true;
	}

	return true;
}

/**---------------------------------------------------------------------------*/
bool BConnectOdbc::disconnect(CBString& strErrorMsg)
{
	if (m_bOpen == true)
	{
		m_bOpen   = false;

		if (TSConnectOdbc::ex().closeConnect() == false)
		{
			strErrorMsg.format("close ODBC connect failed (%s)",
				               TSConnectOdbc::ex().getErrorMsg());
			return false;
		}
	}

	strErrorMsg = "";

	return true;
}

/**---------------------------------------------------------------------------*/
bool BConnectOdbc::setConnect(const CBString & strConnect,
						  const CBString & strUID     ,
						  const CBString & strPwd     )
{
	if (m_bOpen == true)
		return false;

	TSConnectOdbc::ex().setConnect(strConnect, strUID, strPwd);

	return true;
}

/**---------------------------------------------------------------------------*/
bool BConnectOdbc::IsOdbcOpen()
{
	return m_bOpen;
}



/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
