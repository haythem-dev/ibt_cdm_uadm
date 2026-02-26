/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH			                                   */
/**   Date   : 09.06.2004                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once

#ifndef __BPPGROUP_H____
#define __BPPGROUP_H____

/*----------------------------------------------------------------------------*/
class BppGroup : public VBsnObject
{
	SCBMEB_DECLARE_BSN(BppGroup)
public:
									BppGroup             ();
	                                BppGroup             (const BppGroup & r);
	virtual                        ~BppGroup             ();

	virtual BppGroup               * clone		        () const;

//ppCorAppUserGroup
	virtual IPerst::ERet		insertCor           ();
	virtual IPerst::ERet		deleteCor           ();
	virtual IPerst::ERet		deleteAllCorUser	();

//ppGroup
	virtual IPerst::ERet		insert           ();
	virtual IPerst::ERet		update           ();
	virtual IPerst::ERet		delGrp           ();
	virtual IPerst::ERet		selectID         ();
	virtual IPerst::ERet		SelForUpd        ();

	virtual IPerst::ERet		deleteGroup				 ();
	virtual IPerst::ERet		deleteAppRight           ();
	virtual IPerst::ERet		deleteAppCorUserGroup    ();
	

	void                            setGroupid          (long lGroupid);
	void                            setName		        (CBString strGroupname);
	void							setAppid			(short sAppid);
	void							setUserid			(short sUserid);
	
	const long						getGroupid          () const;
	const CBString					getGroupidStr		() const;
	const CBString                 getName		        () const;
	
protected:
	void                            cpyMember           (const BppGroup & r);
};

#endif

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
