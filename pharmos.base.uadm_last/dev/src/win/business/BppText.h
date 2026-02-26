/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH			                                   */
/**   Date   : 06.07.2004                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once

#ifndef __BPPTEXT_H____
#define __BPPTEXT_H____

/*----------------------------------------------------------------------------*/
class BppText : public VBsnObject
{
	SCBMEB_DECLARE_BSN(BppText)
public:
									BppText             ();
	                                BppText             (const BppText & r);
	virtual                        ~BppText             ();

	virtual BppText	                * clone		        () const;
	virtual IPerst::ERet		update              ();
	virtual IPerst::ERet		insert              ();
	virtual IPerst::ERet		delRefText          ();
	virtual IPerst::ERet		delLangText         ();
	virtual IPerst::ERet		selTextEN           (CBString & strTextEN);
	virtual IPerst::ERet		SelForUpd           ();

	
	void                            setLangid           (CBString strLangid);
	void                            setReftext          (CBString strReftext);
	void							setAppid			(short sAppid);
	void							setModid			(short sModid);
	void                            setText	            (CBString strText);
	
	const CBString					getReftext			() const;
	const CBString                 getText             () const;
	const CBString                 getLangid           () const;
	const short						getAppid			() const;
	const short						getModid			() const;
	
	
protected:
	void                            cpyMember           (const BppText & r);
};

#endif

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
