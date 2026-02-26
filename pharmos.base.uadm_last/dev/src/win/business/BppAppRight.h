/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH			                                   */
/**   Date   : 15.06.2004                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once

#ifndef __BPPAPPRIGHT_H____
#define __BPPAPPRIGHT_H____

/*----------------------------------------------------------------------------*/
class BppAppRight : public VBsnObject
{
	SCBMEB_DECLARE_BSN(BppAppRight)
public:
									BppAppRight             ();
	                                BppAppRight             (const BppAppRight & r);
	virtual                        ~BppAppRight             ();

	virtual BppAppRight               * clone		        () const;

	virtual IPerst::ERet		insertAppRight      ();
	virtual IPerst::ERet		updateApplRight	    ();
	virtual IPerst::ERet		deleteAppRight      ();
	virtual IPerst::ERet		SelForUpd           ();

	virtual IPerst::ERet		delRight			();
	virtual IPerst::ERet		insertAppModRight   ();
	virtual IPerst::ERet		deleteAppModRight   ();
	virtual IPerst::ERet		updateAppModRight   ();
	virtual IPerst::ERet		updateApplModRight  ();
	virtual IPerst::ERet		delFromAppRight     ();
		
	void		setAppid		(short sAppid);
	void		setModid		(short sModid);
	void		setUserid		(short sUserid);
	void		setBranchRegid	(short sBranchRegid);
	void		setGroupid		(short sGroupid);
	void		setRightname	(CBString strRightname);
	void		setPrevRightName(CBString strPrevAppname);
	void		setRightDesc	(CBString strRightDesc);
	void		setRightDesc_EN	(CBString strRightDesc);
	void		setRightDesc_FR	(CBString strRightDesc);
	void		setRightDesc_HR	(CBString strRightDesc);
	void		setRightCategory(short sCategory);
		
	const CBString getRightname() const;
	const CBString getRightDesc() const;
	const CBString getRightDesc_EN() const;
	const CBString getRightDesc_FR() const;
	const CBString getRightDesc_HR() const;
	const short	   getRightCategory()const;
	
	const short	getAppid		() const;
	const short	getModid		() const;

	
protected:
	void                            cpyMember           (const BppAppRight & r);
	
private:
	CBString	m_strPrevRightname; // für update eines bestehenden Appname
};

#endif

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
