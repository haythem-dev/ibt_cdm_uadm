/*----------------------------------------------------------------------------*/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                     */
/**   Date   : 14.06.2004                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once

#ifndef __BCONPPAPPMOD_H_____
#define __BCONPPAPPMOD_H_____

/*----------------------------------------------------------------------------*/
class BConppAppMod : public TVConBsnObject<BppAppMod>
{
	SCBMEB_DECLARE_CONBSN(BConppAppMod)

public:
										BConppAppMod ();
	virtual							   ~BConppAppMod ();

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
