/*----------------------------------------------------------------------------*/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                     */
/**   Date   : 06.10.2006                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once

#ifndef __BCONUSERGROUPANALYSIS_H_____
#define __BCONUSERGROUPANALYSIS_H_____

/*----------------------------------------------------------------------------*/
class BConUserGroupAnalysis : public TVConBsnObject<BUserGroupAnalysis>
{
	SCBMEB_DECLARE_CONBSN(BConUserGroupAnalysis)

public:
										BConUserGroupAnalysis ();
	virtual							   ~BConUserGroupAnalysis();

	virtual IPerst::ERet			load	   ();

    void		setAppid(short sAppid);
    void		setUserid	(long sUserid);

	CBString	getRightname();

private:
    short m_sAppid;
    long  m_sUserid;
};

#endif
/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/