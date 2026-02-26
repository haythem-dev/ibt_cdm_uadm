/*----------------------------------------------------------------------------*/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                     */
/**   Date   : 03.08.2004                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once

#ifndef __BCONPPPARA_H_____
#define __BCONPPPARA_H_____

/*----------------------------------------------------------------------------*/
class BConppPara : public TVConBsnObject<BppPara>
{
	SCBMEB_DECLARE_CONBSN(BConppPara)

public:
										BConppPara ();
	virtual							   ~BConppPara ();

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
