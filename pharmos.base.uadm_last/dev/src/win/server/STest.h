/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH			                                   */
/**   Date   : 02.06.2004                                                     */
/**                                                                           */
/******************************************************************************/
#ifndef ____STEST_H___
#define ____STEST_H___

#pragma once

/*----------------------------------------------------------------------------*/
class STest
{
public:

									STest     ();
	virtual						   ~STest     ();

	VAccess::ERet	selectUNameExists(short & sCounter, CBString & strUsername );
	VAccess::ERet	selectAppNameExists(short & sCounter, CBString & strAppname );
	VAccess::ERet	selectModIDExists(short & sCounter, short sAppid, short sModid );
	VAccess::ERet	selectRiNameExists(short & sCounter, CBString & strRiName, short sAppid, short sModid );
	VAccess::ERet	selectBrIDExists(short & sCounter, short sBranchid );
	VAccess::ERet	selectRegIDExists(short & sCounter, short sRegid );
	VAccess::ERet	selectGrpNameExists(short & sCounter, CBString & strGrpname );
	VAccess::ERet	selectLangIDExists(short & sCounter, CBString & strLangid );
	VAccess::ERet	selectReftextExists(short & sCounter, CBString & strReftext );
	VAccess::ERet	selectParaExists(short & sCounter, CBString & strPara1, CBString& strPara2, CBString& strParabez, short sAppid );
	VAccess::ERet	selectUsertypeNameExists(short& sCounter, CBString& strUsertypeName);
	VAccess::ERet	selectUserParaNameExists(short& sCounter, CBString& strUserParaName);

};

#endif

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
