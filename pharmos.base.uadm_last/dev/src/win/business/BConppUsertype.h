/******************************************************************************/
/**                                                                           */
/**   Author : T.ARNOLD NTTDATA                                               */
/**   Date   : 25.11.2025                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once

#ifndef __BCONPPUSERTYPE_H_____
#define __BCONPPUSERTYPE_H_____

/*----------------------------------------------------------------------------*/
class BConppUsertype : public TVConBsnObject<BppUsertype>
{
	SCBMEB_DECLARE_CONBSN(BConppUsertype)

public:
										BConppUsertype ();
	virtual							   ~BConppUsertype ();

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
	bool  m_bAllUsertypes;
	int   m_iSortIndex;
	bool  m_bSortOrder;
};

#endif
/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
