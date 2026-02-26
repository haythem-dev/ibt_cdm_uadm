/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH                                         */
/**   Date   : 25.05.2004                                                     */
/**                                                                           */
/******************************************************************************/

#include "const.h"

#include "uadm_version.h"

#include "CVersion.h"

//---------------------------------------------------------------------------
const unsigned int CVersion::VERSION_MAJORNR   = C_VERSION_MAJORNR;
const unsigned int CVersion::VERSION_MINORNR   = C_VERSION_MINORNR;
const unsigned int CVersion::VERSION_BUILDNR   = C_VERSION_BUILDNR;
const unsigned int CVersion::VERSION_RESERVED  = C_VERSION_RESERVED;

const char * const CVersion::PROGRAM_NAME      = C_PROGRAM_NAME;
const char * const CVersion::VERSION_STRING    = C_VERSION_STRING;
const char * const CVersion::LIBVERSION_STRING = C_LIBVERSION_STRING;
const char * const CVersion::PRODUCT_NAME      = C_PRODUCT_NAME;
const char * const CVersion::FILE_NAME         = C_FILE_NAME;
const char * const CVersion::FILE_DESCRIPTION  = C_FILEDESCRIPTION;
const char * const CVersion::COMPANY_NAME      = C_COMPANYNAME;
const char * const CVersion::COPYRIGHT         = C_COPYRIGHT;
	
//---------------------------------------------------------------------------
CVersion::CVersion()
{
}
