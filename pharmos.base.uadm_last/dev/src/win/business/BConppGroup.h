/*----------------------------------------------------------------------------*/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                     */
/**   Date   : 09.06.2004                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once

#ifndef __BCONPPGROUP_H_____
#define __BCONPPGROUP_H_____

/*----------------------------------------------------------------------------*/
class BConppGroup : public TVConBsnObject<BppGroup>
{
	SCBMEB_DECLARE_CONBSN(BConppGroup)

public:
										BConppGroup ();
	virtual							   ~BConppGroup ();

	virtual IPerst::ERet			load	   ();

	void		setAppid	(short sAppid);
	const short	getAppid	() const;

	void		setBAll		(bool bAllGrp);
	const bool  getBAll		() const;

private:
	short m_sAppid;
	bool  m_bAllGrp;
	
};

#endif
/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
