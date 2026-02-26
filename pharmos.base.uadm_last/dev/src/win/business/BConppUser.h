/*----------------------------------------------------------------------------*/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                     */
/**   Date   : 25.05.2004                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once

#ifndef __BCONPPUSER_H_____
#define __BCONPPUSER_H_____

/*----------------------------------------------------------------------------*/
class BConppUser : public TVConBsnObject<BppUser>
{
	SCBMEB_DECLARE_CONBSN(BConppUser)

public:
										BConppUser ();
	virtual							   ~BConppUser ();

	virtual IPerst::ERet			load	   ();

	void		setAppid	(short sAppid);
	const short	getAppid	() const;
	
	void		setGroupid  (short sGroupid);
	const short getGroupid  () const;

	void		setCorUsrGrp  (bool bCorUsrGrp);
	const bool  getCorUsrGrp  () const;

	void		setBranchRegid  (short sBranchRegid);
	

private:
	short m_sAppid;
	short m_sGroupid;
	short m_sBranchRegid;
	bool  m_bCorUsrGrp;
	
};

#endif
/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
