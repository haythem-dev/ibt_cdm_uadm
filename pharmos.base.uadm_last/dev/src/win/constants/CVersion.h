/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH			                                  */
/**   Date   : 25.05.2004                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once

#ifndef __CVERSION_H___
#define __CVERSION_H___

/*----------------------------------------------------------------------------*/
class CVersion
{
public:
	static const unsigned int VERSION_MAJORNR;
	static const unsigned int VERSION_MINORNR;
	static const unsigned int VERSION_BUILDNR;
	static const unsigned int VERSION_RESERVED;

	static const char * const PROGRAM_NAME;
	static const char * const VERSION_STRING;
	static const char * const LIBVERSION_STRING;
	static const char * const PRODUCT_NAME;
	static const char * const FILE_NAME;
    static const char * const FILE_DESCRIPTION;
    static const char * const COMPANY_NAME;
	static const char * const COPYRIGHT;

private:
	CVersion();
};

#endif

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
