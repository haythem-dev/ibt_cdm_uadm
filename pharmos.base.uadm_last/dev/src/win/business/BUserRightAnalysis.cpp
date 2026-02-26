/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                     */
/**   Date   : 30.08.2004                                                     */
/**                                                                           */
/******************************************************************************/

/*----------------------------------------------------------------------------*/
// Includes
/*----------------------------------------------------------------------------*/
#include "bsn.h"		// precompiled header

#include <tbl_ppappmodule.h>
#include <tbl_ppapplication.h>
#include <tbl_ppappmodright.h>
#include <tbl_ppuser.h>
#include <struct_userrightanalysis.h>
#include <SUserRightAnalysis.h>

#include "BUserRightAnalysis.h"

//---------------------------------------------------------------------------
#define GETUSERRIGHTANALYSISDATA		((SUserRightAnalysis::SListProj &)giveData())

SCBMEB_DEFINE_BSN(BUserRightAnalysis, SUserRightAnalysis::SListProj)

//---------------------------------------------------------------------------
BUserRightAnalysis::BUserRightAnalysis()
{
}

//---------------------------------------------------------------------------
BUserRightAnalysis::BUserRightAnalysis(const BUserRightAnalysis& r) : VBsnObject(r)
{
	cpyMember(r);
}

//---------------------------------------------------------------------------
BUserRightAnalysis::~BUserRightAnalysis()
{
}

/**---------------------------------------------------------------------------*/
BUserRightAnalysis * BUserRightAnalysis::clone() const
{
	return new BUserRightAnalysis(*this);
}

/**---------------------------------------------------------------------------*/
void BUserRightAnalysis::cpyMember(const BUserRightAnalysis & r)
{
    setData((r.giveData()));
}

/**---------------------------------------------------------------------------*/
const CBString	BUserRightAnalysis::getAppname() const
{
    m_strBufExch = GETUSERRIGHTANALYSISDATA.stListProj.APPNAME;
    return m_strBufExch;
}
/**---------------------------------------------------------------------------*/
const CBString	BUserRightAnalysis::getModname() const
{
    m_strBufExch = GETUSERRIGHTANALYSISDATA.stListProj.MODNAME;
    return m_strBufExch;
}

/**---------------------------------------------------------------------------*/
const CBString	BUserRightAnalysis::getUsername() const
{
    m_strBufExch = GETUSERRIGHTANALYSISDATA.stListProj.USERNAME;
    return m_strBufExch;
}

/**---------------------------------------------------------------------------*/
const CBString	BUserRightAnalysis::getSurname() const
{
    m_strBufExch = GETUSERRIGHTANALYSISDATA.stListProj.SURNAME;
    return m_strBufExch;
}

/**---------------------------------------------------------------------------*/
const CBString	BUserRightAnalysis::getForename() const
{
    m_strBufExch = GETUSERRIGHTANALYSISDATA.stListProj.FORENAME;
    return m_strBufExch;
}

/**---------------------------------------------------------------------------*/
const CBString	BUserRightAnalysis::getAreaname() const
{
    m_strBufExch = GETUSERRIGHTANALYSISDATA.stListProj.AREANAME;
    return m_strBufExch;
}

/**---------------------------------------------------------------------------*/
const CBString BUserRightAnalysis::getRegionidStr() const
{
    long2scStr(m_strBufExch, GETUSERRIGHTANALYSISDATA.stListProj.REGIONID);
    return m_strBufExch;
}

/**---------------------------------------------------------------------------*/
const CBString	BUserRightAnalysis::getRightname() const
{
    m_strBufExch = GETUSERRIGHTANALYSISDATA.stListProj.RIGHTNAME;
    return m_strBufExch;
}

/**---------------------------------------------------------------------------*/
const CBString	BUserRightAnalysis::getRightDesc() const
{
    m_strBufExch = GETUSERRIGHTANALYSISDATA.stListProj.RIGHTDESC;
    return m_strBufExch;
}

/**---------------------------------------------------------------------------*/
const CBString	BUserRightAnalysis::getRightDesc_EN() const
{
    m_strBufExch = GETUSERRIGHTANALYSISDATA.stListProj.RIGHTDESC_EN;
    return m_strBufExch;
}

/**---------------------------------------------------------------------------*/
const CBString	BUserRightAnalysis::getRightDesc_FR() const
{
    m_strBufExch = GETUSERRIGHTANALYSISDATA.stListProj.RIGHTDESC_FR;
    return m_strBufExch;
}

/**---------------------------------------------------------------------------*/
const CBString	BUserRightAnalysis::getRightDesc_HR() const
{
    m_strBufExch = GETUSERRIGHTANALYSISDATA.stListProj.RIGHTDESC_HR;
    return m_strBufExch;
}

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/	
