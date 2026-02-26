/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH				                              */
/**   Date   : 25.05.2004                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once

#ifndef ___BSN_H__
#define ___BSN_H__

#define WINVER 0x0601
#define _WIN32_WINNT 0x0601     // Windows 7

/*----------------------------------------------------------------------------*/
// Includes
/*----------------------------------------------------------------------------*/
#include <afxdb.h>
#include <assert.h>
#include <libsccutil/libsccutil.h>
#include <libsccdate/libsccdate.h>
#include <libsccstring/libsccstring.h>
#include <libsccpattern/libsccpattern.h>
#include <libscsaccess/libscsaccess.h>
#include <libscsvoodoo/libscsvoodoo.h>
#include <libscsaccvdoo/libscsaccvdoo.h>
#include <libscsaccodbc/libscsaccodbc.h>
#include <libscbmeb/libscbmeb.h>

using sccmn::VSingleton;
using scbsn::IPerst;
using scbsn::VObject;
using sccmn::CBString;
using scsrv::VAccess;
using scsrv::CVdooConn;
using scsrv::COdbcConn;
using scsrv::VAccVdooCurs;
using scsrv::VAccOdbcCurs;
using scsrv::VData;
using scbsn::VBsnObject;
using scbsn::TVConBsnObject;

#include <CmnUtil.h>
#include "BSqlStat.h"

#endif

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
