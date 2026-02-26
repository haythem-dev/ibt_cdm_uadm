/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                     */
/**   Date   : 06.10.2006                                                     */
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
#include <struct_useranalysis.h>
#include <SUserAnalysis.h>

#include "BUserAnalysis.h"

//---------------------------------------------------------------------------
#define GETUSERANALYSISDATA		((SUserAnalysis::SListProj &)giveData())

SCBMEB_DEFINE_BSN(BUserAnalysis, SUserAnalysis::SListProj)

//---------------------------------------------------------------------------
BUserAnalysis::BUserAnalysis()
{
}

//---------------------------------------------------------------------------
BUserAnalysis::BUserAnalysis(const BUserAnalysis& r) : VBsnObject(r)
{
	cpyMember(r);
}

//---------------------------------------------------------------------------
BUserAnalysis::~BUserAnalysis()
{
}

/**---------------------------------------------------------------------------*/
BUserAnalysis * BUserAnalysis::clone() const
{
	return new BUserAnalysis(*this);
}

/**---------------------------------------------------------------------------*/
void BUserAnalysis::cpyMember(const BUserAnalysis & r)
{
    setData((r.giveData()));
}

/**---------------------------------------------------------------------------*/
const CBString	BUserAnalysis::getAppname			() const
{
	m_strBufExch = GETUSERANALYSISDATA.stListProj.APPNAME;
	return m_strBufExch;
}
/**---------------------------------------------------------------------------*/
const CBString	BUserAnalysis::getModname() const
{
	m_strBufExch = GETUSERANALYSISDATA.stListProj.MODNAME;
	return m_strBufExch;
}

/**---------------------------------------------------------------------------*/
const CBString	BUserAnalysis::getUsername() const
{
	m_strBufExch = GETUSERANALYSISDATA.stListProj.USERNAME;
	return m_strBufExch;
}

/**---------------------------------------------------------------------------*/
const CBString	BUserAnalysis::getSurname() const
{
	m_strBufExch = GETUSERANALYSISDATA.stListProj.SURNAME;
	return m_strBufExch;
}

/**---------------------------------------------------------------------------*/
const CBString	BUserAnalysis::getForename() const
{
	m_strBufExch = GETUSERANALYSISDATA.stListProj.FORENAME;
	return m_strBufExch;
}

/**---------------------------------------------------------------------------*/
const CBString	BUserAnalysis::getAreaname() const
{
	m_strBufExch = GETUSERANALYSISDATA.stListProj.AREANAME;
	return m_strBufExch;
}

/**---------------------------------------------------------------------------*/
const CBString	BUserAnalysis::getRightname() const
{
	m_strBufExch = GETUSERANALYSISDATA.stListCond.RIGHTNAME;
	return m_strBufExch;
}

/**---------------------------------------------------------------------------*/
const CBString	BUserAnalysis::getRightDesc() const
{
	m_strBufExch = GETUSERANALYSISDATA.stListProj.RIGHTDESC;
	return m_strBufExch;
}

/**---------------------------------------------------------------------------*/
const CBString	BUserAnalysis::getRightDesc_EN() const
{
	m_strBufExch = GETUSERANALYSISDATA.stListProj.RIGHTDESC_EN;
	return m_strBufExch;
}

/**---------------------------------------------------------------------------*/
const CBString	BUserAnalysis::getRightDesc_FR() const
{
	m_strBufExch = GETUSERANALYSISDATA.stListProj.RIGHTDESC_FR;
	return m_strBufExch;
}

/**---------------------------------------------------------------------------*/
const CBString	BUserAnalysis::getRightDesc_HR() const
{
	m_strBufExch = GETUSERANALYSISDATA.stListProj.RIGHTDESC_HR;
	return m_strBufExch;
}

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/	
