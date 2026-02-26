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
#include <struct_usergroupanalysis.h>
#include <SUserGroupAnalysis.h>

#include "BUserGroupAnalysis.h"

//---------------------------------------------------------------------------
#define GETUSERGROUPANALYSISDATA		((SUserGroupAnalysis::SListProj &)giveData())

SCBMEB_DEFINE_BSN(BUserGroupAnalysis, SUserGroupAnalysis::SListProj)

//---------------------------------------------------------------------------
BUserGroupAnalysis::BUserGroupAnalysis()
{
}

//---------------------------------------------------------------------------
BUserGroupAnalysis::BUserGroupAnalysis(const BUserGroupAnalysis& r) : VBsnObject(r)
{
	cpyMember(r);
}

//---------------------------------------------------------------------------
BUserGroupAnalysis::~BUserGroupAnalysis()
{
}

/**---------------------------------------------------------------------------*/
BUserGroupAnalysis * BUserGroupAnalysis::clone() const
{
	return new BUserGroupAnalysis(*this);
}

/**---------------------------------------------------------------------------*/
void BUserGroupAnalysis::cpyMember(const BUserGroupAnalysis & r)
{
    setData((r.giveData()));
}

/**---------------------------------------------------------------------------*/
const CBString	BUserGroupAnalysis::getAppname() const
{
    m_strBufExch = GETUSERGROUPANALYSISDATA.stListProj.APPNAME;
    return m_strBufExch;
}


/**---------------------------------------------------------------------------*/
const CBString	BUserGroupAnalysis::getGroupname() const
{
    m_strBufExch = GETUSERGROUPANALYSISDATA.stListProj.GROUPNAME;
	return m_strBufExch;
}

/**---------------------------------------------------------------------------*/
const CBString	BUserGroupAnalysis::getUsername() const
{
	m_strBufExch = GETUSERGROUPANALYSISDATA.stListProj.USERNAME;
	return m_strBufExch;
}

/**---------------------------------------------------------------------------*/
const CBString	BUserGroupAnalysis::getSurname() const
{
	m_strBufExch = GETUSERGROUPANALYSISDATA.stListProj.SURNAME;
	return m_strBufExch;
}

/**---------------------------------------------------------------------------*/
const CBString	BUserGroupAnalysis::getForename() const
{
	m_strBufExch = GETUSERGROUPANALYSISDATA.stListProj.FORENAME;
	return m_strBufExch;
}

/**---------------------------------------------------------------------------*/
const CBString BUserGroupAnalysis::getUserCountry() const
{
	m_strBufExch = GETUSERGROUPANALYSISDATA.stListProj.USERCOUNTRY;
	return m_strBufExch;
}

/**---------------------------------------------------------------------------*/
const CBString BUserGroupAnalysis::getUserType() const
{
	m_strBufExch = GETUSERGROUPANALYSISDATA.stListProj.USERTYPE;
	return m_strBufExch;
}


/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/	
