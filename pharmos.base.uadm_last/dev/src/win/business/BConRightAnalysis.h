/*----------------------------------------------------------------------------*/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                     */
/**   Date   : 30.08.2004                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once

#ifndef __BCONRIGHTANALYSIS_H_____
#define __BCONRIGHTANALYSIS_H_____

/*----------------------------------------------------------------------------*/
class BConRightAnalysis : public TVConBsnObject<BRightAnalysis>
{
	SCBMEB_DECLARE_CONBSN(BConRightAnalysis)

public:
										BConRightAnalysis ();
	virtual							   ~BConRightAnalysis ();

	virtual IPerst::ERet			load	   ();

	void		setAppid	(short sAppid);
	void		setUserid	(short sUserid);
	void		setBranchRegid	(short sBranchRegid);
	void		setUsrOnly	(bool bUsrOnly);
	void		setAppOnly	(bool bAppOnly);

private:
	short m_sAppid;
	short m_sUserid;
	short m_sBranchRegid;
	bool  m_bUsrOnly;
	bool  m_bAppOnly;
};

#endif
/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
