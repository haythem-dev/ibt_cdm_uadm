/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH			                                   */
/**   Date   : 03.08.2004                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once

#ifndef __BPPPARA_H____
#define __BPPPARA_H____

/*----------------------------------------------------------------------------*/
class BppPara : public VBsnObject
{
	SCBMEB_DECLARE_BSN(BppPara)
public:
									BppPara             ();
	                                BppPara             (const BppPara & r);
	virtual                        ~BppPara             ();

	virtual BppPara               * clone		        () const;

	virtual IPerst::ERet		insertPara          ();
	virtual IPerst::ERet		deletePara	        ();
	virtual IPerst::ERet		updatePara          ();
	virtual IPerst::ERet		SelForUpd           ();
			
	void                            setPara1	        (CBString strPara1);
	void                            setPara2	        (CBString strPara2);
	void                            setParabez	        (CBString strParabez);
	void                            setParavalue        (CBString strParavalue);
	void							setAppid			(short sAppid);

	const short						getAppid			() const;
	const CBString                 getPara1		    () const;
	const CBString                 getPara2		    () const;
	const CBString                 getParabez		    () const;
	const CBString                 getParavalue	    () const;
	
protected:
	void                            cpyMember           (const BppPara & r);
};

#endif

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
