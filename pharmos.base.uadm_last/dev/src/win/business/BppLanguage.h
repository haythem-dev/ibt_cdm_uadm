/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH			                                   */
/**   Date   : 05.07.2004                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once

#ifndef __BPPLANGUAGE_H____
#define __BPPLANGUAGE_H____

/*----------------------------------------------------------------------------*/
class BppLanguage : public VBsnObject
{
	SCBMEB_DECLARE_BSN(BppLanguage)
public:
									BppLanguage             ();
	                                BppLanguage             (const BppLanguage & r);
	virtual                        ~BppLanguage             ();

	virtual BppLanguage             * clone		        () const;
	virtual IPerst::ERet		update              ();
	virtual IPerst::ERet		insert              ();
	virtual IPerst::ERet		delLang             ();
	virtual IPerst::ERet		SelForUpd           ();
	
//Deletes
	virtual IPerst::ERet		deleteSingle        ();
	virtual IPerst::ERet		deletePrevious      ();
	virtual IPerst::ERet		deleteAppCorLang    ();
	virtual IPerst::ERet		deleteppText        ();
	
//Correlation APP - LANG
	virtual IPerst::ERet		insertCorLang       ();
	virtual IPerst::ERet		deleteCorLang       ();
	virtual IPerst::ERet		deleteCorppText     ();


	void                            setLangid           (CBString strLangid);
	void                            setLangname         (CBString strLangname);
	void							setAppid			(short sAppid);
	
	const CBString					getLangid			() const;
	const CBString                 getLangname         () const;
	
	
protected:
	void                            cpyMember           (const BppLanguage & r);
};

#endif

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
