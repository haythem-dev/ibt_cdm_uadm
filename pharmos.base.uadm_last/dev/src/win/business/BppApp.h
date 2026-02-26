/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH			                                   */
/**   Date   : 01.06.2004                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once

#ifndef __BPPAPP_H____
#define __BPPAPP_H____

/*----------------------------------------------------------------------------*/
class BppApp : public VBsnObject
{
	SCBMEB_DECLARE_BSN(BppApp)
public:
									BppApp             ();
	                                BppApp             (const BppApp & r);
	virtual                        ~BppApp             ();

	virtual BppApp	               * clone		        () const;

	virtual IPerst::ERet		update              ();
	virtual IPerst::ERet		insert              ();
	virtual IPerst::ERet		delApp              ();
	virtual IPerst::ERet		selectID            ();
	virtual IPerst::ERet		SelForUpd           ();

	virtual IPerst::ERet		delPrevious		    ();
	virtual IPerst::ERet		delppApplication    ();
	virtual IPerst::ERet		delCorAppUserGroup  ();
	virtual IPerst::ERet		delCorAppLang		();
	virtual IPerst::ERet		delppText		    ();
	virtual IPerst::ERet		delAppModule	    ();
	virtual IPerst::ERet		delCorAppBranchReg  ();
	virtual IPerst::ERet		delCorAppModRight	();
	virtual IPerst::ERet		delCorAppRight		();
	
	
    //GET
	const long						getAppid	        () const;
	const CBString					getAppidStr			() const;
	const CBString                  getAppname		    () const;
	const CBString                  getAppname_EN	    () const;
	const CBString                  getAppname_FR	    () const;
	const CBString                  getAppname_HR	    () const;
	const CBString                  getAppDesc          () const;

	//SET
	void                            setAppid	        (long lAppid);
	void                            setAppname          (CBString strAppname);
	void                            setAppname_EN       (CBString strAppname);
	void                            setAppname_FR       (CBString strAppname);
	void                            setAppname_HR       (CBString strAppname);
	void                            setAppDesc	        (CBString strAppDesc);
	
protected:
	void                            cpyMember           (const BppApp & r);

};

#endif

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
