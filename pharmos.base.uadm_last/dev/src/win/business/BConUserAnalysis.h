/*----------------------------------------------------------------------------*/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                     */
/**   Date   : 06.10.2006                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once

#ifndef __BCONUSERANALYSIS_H_____
#define __BCONUSERANALYSIS_H_____

/*----------------------------------------------------------------------------*/
class BConUserAnalysis : public TVConBsnObject<BUserAnalysis>
{
	SCBMEB_DECLARE_CONBSN(BConUserAnalysis)

public:
										BConUserAnalysis ();
	virtual							   ~BConUserAnalysis ();

	virtual IPerst::ERet			load	   ();

	void		setAppid	(short sAppid);
	void		setModid	(short sModid);
	void		setAreaid	(short sAreaid);
	void		setRightname(CBString cbRightname);

	CBString	getRightname();

private:
	short m_sAppid;
	short m_sModid;
	short m_sAreaid;
	CBString m_cbRightname;
};

#endif
/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/