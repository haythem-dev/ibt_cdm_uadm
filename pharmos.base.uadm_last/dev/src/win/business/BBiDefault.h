/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH			                                   */
/**   Date   : 06.06.2005                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once

#ifndef __BBIDEFAULT_H__
#define __BBIDEFAULT_H__

class SBiDefault;

/*----------------------------------------------------------------------------*/
class BBiDefault : public VBsnObject
{
	SCBMEB_DECLARE_BSN(BBiDefault)
public:
									BBiDefault             ();
	virtual                        ~BBiDefault             ();

	virtual IPerst::ERet			insertIBIUser	       ();
	virtual IPerst::ERet			updateIBIUser          ();
	virtual IPerst::ERet			selcountIBIUser        (int * piCountIbiUser);

	virtual IPerst::ERet			selcountIBIUserCountryR     (int * piCountIbiUsrCountryR);
	virtual IPerst::ERet			insertIBIUserCountryRights  ();
	

	void	setBranchid         (const short sBranchid);
	void    setNtUser	        (const CBString& cbNtUser);
	void    setSurname	        (const CBString& cbSurname);
	void    setForename	        (const CBString& cbForename);
	void    setRemark	        (const CBString& cbRemark);
	void    setEmail	        (const CBString& cbEmail);
	void    setCountry	        (const CBString& cbCountry);
	void    setFormat	        (const CBString& cbFormat);
	void    setStyle	        (const CBString& cbStyle);
};

#endif

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
