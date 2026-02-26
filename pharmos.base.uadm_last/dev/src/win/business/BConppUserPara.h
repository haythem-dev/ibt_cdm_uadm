/******************************************************************************/
/**                                                                           */
/**   Author : T.ARNOLD NTTDATA                                               */
/**   Date   : 25.11.2025                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once

#ifndef __BCONPPUSERPARA_H_____
#define __BCONPPUSERPARA_H_____

/*----------------------------------------------------------------------------*/
class BConppUserPara : public TVConBsnObject<BppUserPara>
{
	SCBMEB_DECLARE_CONBSN(BConppUserPara)

public:
										BConppUserPara ();
	virtual							   ~BConppUserPara();

	virtual IPerst::ERet			load	   ();

	void		    setName	(CBString sName);
	const CBString	getName	() const;

	void		setBAll		(bool bAllUsertypes);
	const bool  getBAll		() const;

	void        setSortIndex(int iSortIndex);
	void        toggleSortOrder();

	int		    getSortIndex() const;
	bool	    getSortOrder() const;

private:
	CBString m_strName;
	bool  m_bAllUserParas;
	int   m_iSortIndex;
	bool  m_bSortOrder;
};

#endif
/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
