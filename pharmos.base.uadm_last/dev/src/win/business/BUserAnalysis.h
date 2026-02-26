/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH			                                   */
/**   Date   : 06.10.2006                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once

#ifndef __BUSERANALYSIS_H____
#define __BUSERANALYSIS_H____

/*----------------------------------------------------------------------------*/
class BUserAnalysis : public VBsnObject
{
	SCBMEB_DECLARE_BSN(BUserAnalysis)
public:
									BUserAnalysis             ();
	                                BUserAnalysis             (const BUserAnalysis & r);
	virtual                        ~BUserAnalysis             ();

	virtual BUserAnalysis               * clone		        () const;

	const CBString					getAppname			() const;
	const CBString                  getModname		    () const;
	const CBString					getUsername			() const;
	const CBString                  getSurname		    () const;
	const CBString					getForename			() const;
	const CBString                  getAreaname		    () const;
	const CBString					getRightname		() const;

	const CBString					getRightDesc		() const;
	const CBString					getRightDesc_EN		() const;
	const CBString					getRightDesc_FR		() const;
	const CBString					getRightDesc_HR		() const;


protected:
	void                            cpyMember           (const BUserAnalysis & r);
};

#endif

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
