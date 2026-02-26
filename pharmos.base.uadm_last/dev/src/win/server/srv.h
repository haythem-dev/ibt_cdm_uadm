/******************************************************************************/
/**                                                                           */
/**   File   : srv.h                                                          */
/**   Author : T. Hörath PRZ GmbH	                                         */
/**   Date   : 25.05.2004                                                     */
/**                                                                           */
/******************************************************************************/
#ifndef ___SRV_H____
#define ___SRV_H____


#pragma once

#define WINVER 0x0601
#define _WIN32_WINNT 0x0601     // Windows 7

/*----------------------------------------------------------------------------*/
// Includes
/*----------------------------------------------------------------------------*/
#include <afxdb.h>
#include <assert.h>
#include <stdarg.h>

#include <libsccutil/libsccutil.h>
#include <libsccstring/libsccstring.h>
#include <libsccpattern/libsccpattern.h>
#include <libscsaccess/libscsaccess.h>
#include <libscsvoodoo/libscsvoodoo.h>
#include <libscsaccvdoo/libscsaccvdoo.h>
#include <libscsaccodbc/libscsaccodbc.h>

using sccmn::VSingleton;
using sccmn::CBString;
using scsrv::VAccess;
using scsrv::VAccVdooCurs;
using scsrv::VAccOdbcCurs;
using scsrv::VData;
using scsrv::CVdooConn;
using scsrv::COdbcConn;

#include <CmnUtil.h>

#include "SSqlStat.h"
#include "SLog.h"



#endif

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
