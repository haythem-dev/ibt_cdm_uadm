/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH			                                   */
/**   Date   : 14.06.2004                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once

#ifndef __BPPBRANCHREG_H____
#define __BPPBRANCHREG_H____

/*----------------------------------------------------------------------------*/
class BppBranchReg : public VBsnObject
{
	SCBMEB_DECLARE_BSN(BppBranchReg)
public:
									BppBranchReg             ();
	                                BppBranchReg             (const BppBranchReg & r);
	virtual                        ~BppBranchReg             ();

	virtual BppBranchReg               * clone		        () const;

	virtual IPerst::ERet		insertBranch	    ();
	virtual IPerst::ERet		insertRegion	    ();
	virtual IPerst::ERet		updateBranch        ();
	virtual IPerst::ERet		updateRegion        ();
	virtual IPerst::ERet		SelForUpdBr         ();
	virtual IPerst::ERet		SelForUpdReg        ();

	virtual IPerst::ERet		deleteBranch	();
	virtual IPerst::ERet		deleteBrPrevious();
	virtual IPerst::ERet		deleteBrCorReg	();
	virtual IPerst::ERet		deleteBrAppRight();
	virtual IPerst::ERet		delBranch	    ();

	virtual IPerst::ERet		deleteRegion			();
	virtual IPerst::ERet		deleteRegPrevious	    ();
	virtual IPerst::ERet		deleteRegCorBranch	    ();
	virtual IPerst::ERet		deleteRegAppRight	    ();
	virtual IPerst::ERet		delRegion			    ();

	
//für CorAppBranchReg!
	void                            setCorBranchid      (short sCorBranchid);
	void                            setCorRegid			(short sCorRegid);
	void							setCorAppid			(short sCorAppid);
	const short						getCorBranchid      () const;
	const CBString					getCorBranchidStr	() const;
	const CBString                 getCorBranchname    () const;
	const short						getCorRegid	        () const;
	const CBString					getCorRegidStr		() const;
	const CBString                 getCorRegname	    () const;
	virtual IPerst::ERet		insertAppBranchReg  ();
	virtual IPerst::ERet		deleteAppBranchReg  ();
	virtual IPerst::ERet		deleteCORReg  ();

	
	virtual IPerst::ERet		delCorBrFromPrev	();
	virtual IPerst::ERet		delCorBrFromAppRight();

	virtual IPerst::ERet		delCorRegFromPrev	 ();
	virtual IPerst::ERet		delCorRegFromAppRight();
	
//--------------------	
	

	void                            setBranchid         (short sBranchid);
	void                            setRegid			(short sRegid);
	
	void                            setBranchname       (CBString strBranchname);
	void                            setRegname		    (CBString strRegname);
		
	void							setAppid			(short sAppid);
	const short						getAppid			() const;
	
	const short						getBranchid         () const;
	const CBString					getBranchidStr		() const;
	const CBString                 getBranchname	    () const;

	const short						getRegid	        () const;
	const CBString					getRegidStr			() const;
	const CBString                 getRegname		    () const;
	
protected:
	void                            cpyMember           (const BppBranchReg & r);
};

#endif

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
