/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH			                                   */
/**   Date   : 16.08.2004                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once

#ifndef __BPPPREV_H____
#define __BPPPREV_H____

/*----------------------------------------------------------------------------*/
class BppPrev : public VBsnObject
{
	SCBMEB_DECLARE_BSN(BppPrev)
public:
									BppPrev             ();
	                                BppPrev             (const BppPrev & r);
	virtual                        ~BppPrev             ();

	virtual BppPrev               * clone		        () const;

	virtual IPerst::ERet		deletePrev	        ();
			
	void                            setGlobUser         (CBString strGlobUser);
	void							setAppid			(short sAppid);

	const short						getAppid			() const;
	const CBString                 getGlobUser 	    () const;
	const CBString                 getUsername 	    () const;
	const CBString                 getLanguageID 	    () const;
	const short						getBrRegID			() const;
	const CBString                 getBrRegIDstr 	    () const;
	
protected:
	void                            cpyMember           (const BppPrev & r);
};

#endif

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
