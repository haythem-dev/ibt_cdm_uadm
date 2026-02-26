/*----------------------------------------------------------------------------*/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                     */
/**   Date   : 14.06.2004                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once

#ifndef __BCONPPBRANCHREG_H_____
#define __BCONPPBRANCHREG_H_____

/*----------------------------------------------------------------------------*/
class BConppBranchReg : public TVConBsnObject<BppBranchReg>
{
	SCBMEB_DECLARE_CONBSN(BConppBranchReg)

public:
										BConppBranchReg ();
	virtual							   ~BConppBranchReg ();

	virtual IPerst::ERet			load	   ();

	void		setAppid	(short sAppid);
	void		setUserid	(long lUserid);
	void		setRegid	(short sRegid);
	void		setBSelReg  (bool bReg);
	void		setBSelBranch(bool bBranch);
	void		setBSelCor  (bool bCor);

	const bool  getBSelCor  () const;
	const short	getAppid	() const;
	const bool  getBSelReg  () const;
	const bool  getBSelBranch() const;
	const short	getRegid	() const;

private:
	short m_sAppid;
	long  m_lUserid;
	short m_sRegid;
	bool  m_bReg;
	bool  m_bBranch;
	bool  m_bCor;
	
};

#endif
/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
