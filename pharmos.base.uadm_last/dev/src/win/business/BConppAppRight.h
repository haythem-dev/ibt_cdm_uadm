/*----------------------------------------------------------------------------*/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                     */
/**   Date   : 15.06.2004                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once

#ifndef __BCONPPAPPRIGHT_H_____
#define __BCONPPAPPRIGHT_H_____

/*----------------------------------------------------------------------------*/
class BConppAppRight : public TVConBsnObject<BppAppRight>
{
	SCBMEB_DECLARE_CONBSN(BConppAppRight)

public:
										BConppAppRight ();
	virtual							   ~BConppAppRight ();

	virtual IPerst::ERet			load	   ();

	void		setAppid		(short sAppid);
	void		setModid		(short sModid);
	void		setUserid		(short sUserid);
	void		setBranchRegid	(short sBranchRegid);
	void		setGroupid		(short sGroupid);
	void		setBCorRights   (bool  bCorRight);
	void		setBCorGroupRights (bool bCorGroupRight);
	void		setBModRights	(bool bModRights);
	void		setRightCategory(short sCategory);
	
	const short	getRightCategory() const;
	const bool  getBModRights	() const;
	const bool  getBCorGroupRights() const;
	const bool  getBCorRights   () const;
	const short	getAppid		() const;
	const short	getGroupid		() const;
	const short	getUserid		() const;
	const short	getBranchRegid	() const;
	const short	getModid		() const;

private:
	short m_sAppid;
	short m_sModid;
	short m_sGroupid;
	short m_sUserid;
	short m_sBranchRegid;
	bool  m_bCorRight;
	bool  m_bCorGroupRight;
	bool  m_bModRights;
	short m_sCategory;
};

#endif
/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
