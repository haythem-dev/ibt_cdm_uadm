/******************************************************************************/
/**                                                                           */
/**   File   : uadm_version.h                                                 */
/**   Author : T. Hörath                                                      */
/**   Date   : 25.05.2004                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once

#ifndef ____VERSION_H___
#define ____VERSION_H___

#define C_VERSION_MAJORNR           1
#define C_VERSION_MINORNR           3
#define C_VERSION_BUILDNR           7
#define C_VERSION_RESERVED          11

#define C_PROGRAM_NAME              "UADM"

#define UADM_VERSION                "01.03.07.11"

#define C_LIBVERSION_STRING         "01.19.04.02"

#ifdef _DEBUG
#define __C_VERSION_STRING2__       UADM_VERSION " dbg"
#else
#define __C_VERSION_STRING2__       UADM_VERSION " rel"
#endif

#define C_VERSION_STRING            __C_VERSION_STRING2__

#define C_PRODUCT_NAME              "Global User Administration"
#define C_FILE_NAME                 "UsrAdmin.exe"
#define C_FILEDESCRIPTION           "MFC-Application Global User Admin"
#define C_COMPANYNAME               "PHOENIX - CCITD"
#define C_COPYRIGHT                 "Copyright © 2008 - 2026"

#endif

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
