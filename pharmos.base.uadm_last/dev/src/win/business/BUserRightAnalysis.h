/******************************************************************************/
/**                                                                           */
/**   Author : Bhaskar                                                        */
/**   Date   : 30.08.2004                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once

#ifndef __BUSERRIGHTANALYSIS_H____
#define __BUSERRIGHTANALYSIS_H____

/*----------------------------------------------------------------------------*/
class BUserRightAnalysis : public VBsnObject
{
	SCBMEB_DECLARE_BSN(BUserRightAnalysis)
public:
									BUserRightAnalysis             ();
                                    BUserRightAnalysis(const BUserRightAnalysis & r);
	virtual                        ~BUserRightAnalysis();

	virtual BUserRightAnalysis    * clone		        () const;

    const CBString					getAppname() const;
    const CBString					getModname() const;
    const CBString					getUsername() const;
    const CBString					getSurname() const;
    const CBString					getForename() const;
    const CBString					getAreaname() const;
    const CBString					getRegionidStr() const;
    const CBString					getRightname() const;

    const CBString					getRightDesc() const;
    const CBString					getRightDesc_EN() const;
    const CBString					getRightDesc_FR() const;
    const CBString					getRightDesc_HR() const;

protected:
	void                            cpyMember           (const BUserRightAnalysis & r);
};

#endif

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
