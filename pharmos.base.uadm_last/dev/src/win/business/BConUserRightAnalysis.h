/*----------------------------------------------------------------------------*/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                     */
/**   Date   : 06.10.2006                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once

#ifndef __BCONUSERRIGHTANALYSIS_H_____
#define __BCONUSERRIGHTANALYSIS_H_____

/*----------------------------------------------------------------------------*/
class BConUserRightAnalysis : public TVConBsnObject<BUserRightAnalysis>
{
	SCBMEB_DECLARE_CONBSN(BConUserRightAnalysis)

public:
										BConUserRightAnalysis ();
	virtual							   ~BConUserRightAnalysis();

	virtual IPerst::ERet			load	   ();

    void		setAppid(short sAppid);
    void		setUserid	(long sUserid);
    void		setBranchRegid(short sBrRegid);

	CBString	getRightname();

private:
	short m_sAppid;
    long  m_sUserId;
    short m_sBrRegid;
};

#endif
/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/