/*----------------------------------------------------------------------------*/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                     */
/**   Date   : 16.08.2004                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once

#ifndef __BCONPPPREV_H_____
#define __BCONPPPREV_H_____

/*----------------------------------------------------------------------------*/
class BConppPrev : public TVConBsnObject<BppPrev>
{
	SCBMEB_DECLARE_CONBSN(BConppPrev)

public:
										BConppPrev ();
	virtual							   ~BConppPrev ();

	virtual IPerst::ERet			load	   ();

	void		setAppid	(short sAppid);
	const short	getAppid	() const;

private:
	short m_sAppid;
	
};

#endif
/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
