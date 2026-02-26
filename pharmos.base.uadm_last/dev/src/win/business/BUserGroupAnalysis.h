/******************************************************************************/
/**                                                                           */
/**   Author : Bhaskar                                                        */
/**   Date   : 30.08.2004                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once

#ifndef __BUSERGROUPANALYSIS_H____
#define __BUSERGROUPANALYSIS_H____

/*----------------------------------------------------------------------------*/
class BUserGroupAnalysis : public VBsnObject
{
	SCBMEB_DECLARE_BSN(BUserGroupAnalysis)
public:
									BUserGroupAnalysis             ();
                                    BUserGroupAnalysis(const BUserGroupAnalysis & r);
	virtual                        ~BUserGroupAnalysis();

	virtual BUserGroupAnalysis    * clone		        () const;

	const CBString					getAppname          () const;
	const CBString					getGroupname        () const;
	const CBString					getUsername			() const;
	const CBString					getSurname		    () const;
	const CBString					getForename			() const;
	const CBString                  getUserCountry      () const;
	const CBString                  getUserType         () const;

protected:
	void                            cpyMember           (const BUserGroupAnalysis & r);
};

#endif

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
