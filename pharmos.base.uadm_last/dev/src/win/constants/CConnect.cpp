/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH	                                         */
/**   Date   : 25.05.2004                                                     */
/**                                                                           */
/******************************************************************************/

/*----------------------------------------------------------------------------*/
#include "const.h"

#include <libsccpattern/libsccpattern.h>
#include <libscmlogin/libscmlogin.h>
#include "defines.h"

#include "CConnect.h"

using	scmod::ppBLoginData;

//---------------------------------------------------------------------------
// only default values (can be set in parameters of UADM) !!!
//---------------------------------------------------------------------------
CBString CConnect::TEXT_TABLE = "LGTEXTUADM";
CBString CConnect::VOODOO_SERVER    = "usradmuiserv";
CBString CConnect::VOODOO_SHAREDLIB = "uadmonlsrv.so";
unsigned long CConnect::VOODOO_PORT = 58002;

CBString CConnect::HOST				= "zpos1";
CBString CConnect::DATABASE			= "zpcua1";

CBString CConnect::HOST_BACKUP		= "zpps2"; 
CBString CConnect::DATABASE_BACKUP	= "zpcua2"; 

bool CConnect::TEST_CONNECTED = false;
	
//---------------------------------------------------------------------------
void CConnect::setTestHosts()
{
	CBString paraDesc;
	CBString paraVal;

	paraDesc = PARA_TEST_HOST;
	if(ppBLoginData::ex().getParaValue(paraDesc, paraVal))
		HOST		= paraVal; 
	
	paraDesc = PARA_TEST_HOST_BACKUP;
	if (ppBLoginData::ex().getParaValue(paraDesc, paraVal))
		HOST_BACKUP	= paraVal; 

	TEST_CONNECTED = true;
}

//---------------------------------------------------------------------------
void CConnect::setProdHosts()
{
	CBString paraDesc;
	CBString paraVal;

	paraDesc = PARA_HOST;
	if(ppBLoginData::ex().getParaValue(paraDesc, paraVal))
		HOST		= paraVal; 
	
	paraDesc = PARA_HOST_BACKUP;
	if (ppBLoginData::ex().getParaValue(paraDesc, paraVal))
		HOST_BACKUP	= paraVal; 
	
	TEST_CONNECTED = false;
}

//---------------------------------------------------------------------------
void CConnect::setConnPara()
{
	CBString paraDesc;
	CBString paraVal;

	paraDesc = PARA_TEXT_TABLE;
	if(ppBLoginData::ex().getParaValue(paraDesc, paraVal))
		TEXT_TABLE		= paraVal; 
	
	paraDesc = PARA_UISERV;
	if(ppBLoginData::ex().getParaValue(paraDesc, paraVal))
		VOODOO_SERVER		= paraVal; 
	
	paraDesc = PARA_ONL_SERV;
	if(ppBLoginData::ex().getParaValue(paraDesc, paraVal))
		VOODOO_SHAREDLIB	= paraVal; 
	
	paraDesc = PARA_VOODOO_PORT;
	if(ppBLoginData::ex().getParaValue(paraDesc, paraVal))
		VOODOO_PORT	= atoi(paraVal); 
		
	paraDesc = PARA_DB;
	if(ppBLoginData::ex().getParaValue(paraDesc, paraVal))
		DATABASE		= paraVal; 
	
	paraDesc = PARA_DB_BACKUP;
	if (ppBLoginData::ex().getParaValue(paraDesc, paraVal))
		DATABASE_BACKUP	= paraVal; 
}

//---------------------------------------------------------------------------
CConnect::CConnect()
{
}
