/*----------------------------------------------------------------------------*/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                     */
/**   Date   : 05.07.2004                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once

#ifndef __BCONPPLANGUAGE_H_____
#define __BCONPPLANGUAGE_H_____

/*----------------------------------------------------------------------------*/
class BConppLanguage : public TVConBsnObject<BppLanguage>
{
	SCBMEB_DECLARE_CONBSN(BConppLanguage)

public:
										BConppLanguage ();
	virtual							   ~BConppLanguage ();

	virtual IPerst::ERet			load	   ();

	void		setAppid	(short sAppid);
	const short	getAppid	() const;

	void		setBAll		(bool bAllLang);
	const bool  getBAll		() const;

	void		setBList	(bool bLangList);
	const bool  getBList	() const;


private:
	short m_sAppid;
	bool  m_bAllLang;
	bool  m_bLangList;
	
};

#endif
/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
