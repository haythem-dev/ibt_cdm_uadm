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
#include <struct_rightanalysis.h>
#include <SRightAnalysis.h>

#include "BRightAnalysis.h"

//---------------------------------------------------------------------------
#define GETANALYSISDATA		((SRightAnalysis::SListProj &)giveData())

SCBMEB_DEFINE_BSN(BRightAnalysis, SRightAnalysis::SListProj)

//---------------------------------------------------------------------------
BRightAnalysis::BRightAnalysis()
{
}

//---------------------------------------------------------------------------
BRightAnalysis::BRightAnalysis(const BRightAnalysis& r) : VBsnObject(r)
{
	cpyMember(r);
}

//---------------------------------------------------------------------------
BRightAnalysis::~BRightAnalysis()
{
}

/**---------------------------------------------------------------------------*/
BRightAnalysis * BRightAnalysis::clone() const
{
	return new BRightAnalysis(*this);
}

/**---------------------------------------------------------------------------*/
void BRightAnalysis::cpyMember(const BRightAnalysis & r)
{
    setData((r.giveData()));
}

/**---------------------------------------------------------------------------*/
const CBString	BRightAnalysis::getAppname			() const
{
	m_strBufExch = GETANALYSISDATA.stListProj.APPNAME;
	return m_strBufExch;
}
/**---------------------------------------------------------------------------*/
const CBString	BRightAnalysis::getModname() const
{
	m_strBufExch = GETANALYSISDATA.stListProj.MODNAME;
	return m_strBufExch;
}

/**---------------------------------------------------------------------------*/
const CBString	BRightAnalysis::getUsername() const
{
	m_strBufExch = GETANALYSISDATA.stListProj.USERNAME;
	return m_strBufExch;
}

/**---------------------------------------------------------------------------*/
const CBString	BRightAnalysis::getSurname() const
{
	m_strBufExch = GETANALYSISDATA.stListProj.SURNAME;
	return m_strBufExch;
}

/**---------------------------------------------------------------------------*/
const CBString	BRightAnalysis::getForename() const
{
	m_strBufExch = GETANALYSISDATA.stListProj.FORENAME;
	return m_strBufExch;
}

/**---------------------------------------------------------------------------*/
const CBString	BRightAnalysis::getAreaname() const
{
	m_strBufExch = GETANALYSISDATA.stListProj.AREANAME;
	return m_strBufExch;
}

/**---------------------------------------------------------------------------*/
const CBString	BRightAnalysis::getRightname() const
{
	m_strBufExch = GETANALYSISDATA.stListProj.RIGHTNAME;
	return m_strBufExch;
}

/**---------------------------------------------------------------------------*/
const CBString	BRightAnalysis::getRightDesc() const
{
	m_strBufExch = GETANALYSISDATA.stListProj.RIGHTDESC;
	return m_strBufExch;
}

/**---------------------------------------------------------------------------*/
const CBString	BRightAnalysis::getRightDesc_EN() const
{
	m_strBufExch = GETANALYSISDATA.stListProj.RIGHTDESC_EN;
	return m_strBufExch;
}

/**---------------------------------------------------------------------------*/
const CBString	BRightAnalysis::getRightDesc_FR() const
{
	m_strBufExch = GETANALYSISDATA.stListProj.RIGHTDESC_FR;
	return m_strBufExch;
}

/**---------------------------------------------------------------------------*/
const CBString	BRightAnalysis::getRightDesc_HR() const
{
	m_strBufExch = GETANALYSISDATA.stListProj.RIGHTDESC_HR;
	return m_strBufExch;
}


/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/	
