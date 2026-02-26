/*----------------------------------------------------------------------------*/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                     */
/**   Date   : 06.07.2004                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once

#ifndef __BCONPPTEXT_H_____
#define __BCONPPTEXT_H_____

/*----------------------------------------------------------------------------*/
class BConppText : public TVConBsnObject<BppText>
{
	SCBMEB_DECLARE_CONBSN(BConppText)

public:
										BConppText ();
	virtual							   ~BConppText ();

	virtual IPerst::ERet			load	   ();

	void		setAppid	(short sAppid);
	const short	getAppid	() const;

	void		setLangid	(CBString strLangid);
	const CBString getLangid () const;

	void		setModid	(short sModid);
	const short	getModid	() const;

	void		setBCor		(bool bCorText);
	const bool  getBCor		() const;

private:
	short m_sAppid;
	short m_sModid;
	bool  m_bCorText;
	CBString m_strLangid;
	
};

#endif
/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
