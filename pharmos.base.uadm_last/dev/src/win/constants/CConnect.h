/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                     */
/**   Date   : 25.05.2004                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once

#ifndef ___CCONNECT_H_____
#define ___CCONNECT_H_____

#include <libsccstring/libsccstring.h>
using	sccmn::CBString;

/*----------------------------------------------------------------------------*/
class CConnect
{
public:
	static void setTestHosts();
	static void setProdHosts();
	static void setConnPara();
	
	static CBString  VOODOO_SERVER;
	static CBString  VOODOO_SHAREDLIB;
	static unsigned long VOODOO_PORT;
	
    static CBString  DATABASE;
	static CBString  DATABASE_BACKUP;

	static CBString  TEXT_TABLE;

	static CBString HOST;
	static CBString HOST_BACKUP;

	static bool TEST_CONNECTED;

private:
	CConnect();
};

#endif

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
