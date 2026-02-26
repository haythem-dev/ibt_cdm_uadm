/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH			                                   */
/**   Date   : 30.08.2004                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once

#ifndef __BRIGHTANALYSIS_H____
#define __BRIGHTANALYSIS_H____

/*----------------------------------------------------------------------------*/
class BRightAnalysis : public VBsnObject
{
	SCBMEB_DECLARE_BSN(BRightAnalysis)
public:
									BRightAnalysis             ();
	                                BRightAnalysis             (const BRightAnalysis & r);
	virtual                        ~BRightAnalysis             ();

	virtual BRightAnalysis          * clone		        () const;

	const CBString					getAppname			() const;
	const CBString					getModname		    () const;
	const CBString					getUsername			() const;
	const CBString					getSurname		    () const;
	const CBString					getForename			() const;
	const CBString					getAreaname		    () const;
	const CBString					getRightname		() const;
	
	const CBString					getRightDesc		() const;
	const CBString					getRightDesc_EN		() const;
	const CBString					getRightDesc_FR		() const;
	const CBString					getRightDesc_HR		() const;

protected:
	void                            cpyMember           (const BRightAnalysis & r);
};

#endif

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
