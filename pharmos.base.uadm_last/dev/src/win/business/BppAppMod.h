/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH			                                   */
/**   Date   : 14.06.2004                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once

#ifndef __BPPAPPMOD_H____
#define __BPPAPPMOD_H____

/*----------------------------------------------------------------------------*/
class BppAppMod : public VBsnObject
{
	SCBMEB_DECLARE_BSN(BppAppMod)
public:
									BppAppMod             ();
	                                BppAppMod             (const BppAppMod & r);
	virtual                        ~BppAppMod             ();

	virtual BppAppMod               * clone		        () const;

	virtual IPerst::ERet		insertMod           ();
	virtual IPerst::ERet		deleteModule        ();
	virtual IPerst::ERet		updateMod           ();
	virtual IPerst::ERet		SelForUpd           ();

	virtual IPerst::ERet		deleteMod           ();
	virtual IPerst::ERet		deleteAppModRight   ();
	virtual IPerst::ERet		deleteppText        ();
	virtual IPerst::ERet		deleteAppRight      ();

			
	void                            setModuleid         (short sModid);
	void                            setName		        (CBString strModname);
	void                            setName_EN	        (CBString strModname);
	void                            setName_FR	        (CBString strModname);
	void                            setName_HR	        (CBString strModname);
	void							setAppid			(short sAppid);

	const short						getAppid			() const;
	const short						getModuleid         () const;
	const CBString					getModuleidStr		() const;
	const CBString                  getModname		    () const;
	const CBString                  getModname_EN	    () const;
	const CBString                  getModname_FR	    () const;
	const CBString                  getModname_HR	    () const;

	
protected:
	void                            cpyMember           (const BppAppMod & r);
};

#endif

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
