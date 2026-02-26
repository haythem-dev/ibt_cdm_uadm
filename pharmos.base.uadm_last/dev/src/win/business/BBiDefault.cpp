/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                     */
/**   Date   : 06.06.2005                                                     */
/**                                                                           */
/******************************************************************************/

/*----------------------------------------------------------------------------*/
// Includes
/*----------------------------------------------------------------------------*/
#include "bsn.h"		// precompiled header

#include <tbl_ibiuser.h>
#include <struct_ibiuser.h>
#include <SBiDefault.h>

#include "BBiDefault.h"

//---------------------------------------------------------------------------
#define GETDATA		((SBiDefault::SListProj &)giveData())

SCBMEB_DEFINE_BSN(BBiDefault, SBiDefault::SListProj)

//---------------------------------------------------------------------------
BBiDefault::BBiDefault()
{
}

//---------------------------------------------------------------------------
BBiDefault::~BBiDefault()
{
}

/**---------------------------------------------------------------------------*/
IPerst::ERet BBiDefault::insertIBIUserCountryRights()
{
	SBiDefault objSBiDefault;

	VAccess::ERet eRet = objSBiDefault.insertIBIUserCountryRights(GETDATA);


	switch (eRet)
	{
	case VAccess::RET_OK:
		break;

	case VAccess::RET_NOTFOUND:
		m_strErrorMsg = objSBiDefault.getErrorMsg();
		m_iErrorCode  = objSBiDefault.getErrorCode();
		return IPerst::RET_WARNING;

	default:
		m_strErrorMsg = objSBiDefault.getErrorMsg();
		m_iErrorCode  = objSBiDefault.getErrorCode();
		assert(0);
		return IPerst::RET_ERROR;
	}

	return IPerst::RET_OK;
}

/**---------------------------------------------------------------------------*/
IPerst::ERet BBiDefault::selcountIBIUserCountryR     (int * piCountIbiUsrCountryR)
{
	SBiDefault objSBiDefault;
		
	VAccess::ERet eRet = objSBiDefault.selcountIBIUserCountryR(GETDATA, piCountIbiUsrCountryR);

	switch (eRet)
	{
	case VAccess::RET_ERROR:
		m_strErrorMsg = objSBiDefault.getErrorMsg();
		m_iErrorCode  = objSBiDefault.getErrorCode();
		return IPerst::RET_ERROR;

	default:
		return IPerst::RET_OK;
	}

	return IPerst::RET_OK;
}

/**---------------------------------------------------------------------------*/
IPerst::ERet BBiDefault::insertIBIUser()
{
	SBiDefault objSBiDefault;

	VAccess::ERet eRet = objSBiDefault.insertIBIUser(GETDATA);


	switch (eRet)
	{
	case VAccess::RET_OK:
		break;

	case VAccess::RET_NOTFOUND:
		m_strErrorMsg = objSBiDefault.getErrorMsg();
		m_iErrorCode  = objSBiDefault.getErrorCode();
		return IPerst::RET_WARNING;

	default:
		m_strErrorMsg = objSBiDefault.getErrorMsg();
		m_iErrorCode  = objSBiDefault.getErrorCode();
		assert(0);
		return IPerst::RET_ERROR;
	}

	return IPerst::RET_OK;
}

/**---------------------------------------------------------------------------*/
IPerst::ERet BBiDefault::selcountIBIUser(int * piCountIbiUser)
{
	SBiDefault objSBiDefault;
		
	VAccess::ERet eRet = objSBiDefault.selcountIBIUser(GETDATA, piCountIbiUser);

	switch (eRet)
	{
	case VAccess::RET_ERROR:
		m_strErrorMsg = objSBiDefault.getErrorMsg();
		m_iErrorCode  = objSBiDefault.getErrorCode();
		return IPerst::RET_ERROR;

	default:
		return IPerst::RET_OK;
	}

	return IPerst::RET_OK;
}


/**---------------------------------------------------------------------------*/
IPerst::ERet BBiDefault::updateIBIUser()
{
	SBiDefault objSBiDefault;
	
	VAccess::ERet eRet = objSBiDefault.updateIBIUser(GETDATA);

	switch (eRet)
	{
	case VAccess::RET_OK:
		break;

	case VAccess::RET_NOTFOUND:
		m_strErrorMsg = objSBiDefault.getErrorMsg();
		m_iErrorCode  = objSBiDefault.getErrorCode();
		return IPerst::RET_WARNING;

	default:
		m_strErrorMsg = objSBiDefault.getErrorMsg();
		m_iErrorCode  = objSBiDefault.getErrorCode();
		assert(0);
		return IPerst::RET_ERROR;
	}

	return IPerst::RET_OK;
}


/**---------------------------------------------------------------------------*/
void BBiDefault::setBranchid(const short sBranchid)
{
	GETDATA.stListProj.BRANCHID = sBranchid;
}

/**---------------------------------------------------------------------------*/
void BBiDefault::setNtUser(const CBString& cbNtUser)
{
	strcpy(GETDATA.stListCond.USERID, cbNtUser);
}

/**---------------------------------------------------------------------------*/
void BBiDefault::setSurname	        (const CBString& cbSurname)
{
	strcpy(GETDATA.stListCond.LASTNAME, cbSurname);
}

/**---------------------------------------------------------------------------*/
void BBiDefault::setForename (const CBString& cbForename)
{
	strcpy(GETDATA.stListProj.FIRSTNAME, cbForename);
}

/**---------------------------------------------------------------------------*/
void BBiDefault::setRemark	        (const CBString& cbRemark)
{
	strcpy(GETDATA.stListProj.REMARK, cbRemark);
}

/**---------------------------------------------------------------------------*/
void BBiDefault::setEmail	        (const CBString& cbEmail)
{
	strcpy(GETDATA.stListProj.EMAIL, cbEmail);
}

/**---------------------------------------------------------------------------*/
void BBiDefault::setCountry	        (const CBString& cbCountry)
{
	strcpy(GETDATA.stListProj.DEFAULTCOUNTRYCODE, cbCountry);
}

/**---------------------------------------------------------------------------*/
void BBiDefault::setFormat	        (const CBString& cbFormat)
{
	strcpy(GETDATA.stListProj.DEFAULTFORMAT, cbFormat);
}

/**---------------------------------------------------------------------------*/
void BBiDefault::setStyle	        (const CBString& cbStyle)
{
	strcpy(GETDATA.stListProj.DEFAULTSTYLE, cbStyle);
}

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/	
