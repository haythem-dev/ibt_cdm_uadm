/******************************************************************************/
/**                                                                           */
/**   Author : T.ARNOLD NTTDATA                                               */
/**   Date   : 25.11.2025                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once

#ifndef __BPPUSERTYPE_H____
#define __BPPUSERTYPE_H____

/*----------------------------------------------------------------------------*/
class BppUsertype : public VBsnObject
{
	SCBMEB_DECLARE_BSN(BppUsertype)
public:
	BppUsertype();
	BppUsertype(const BppUsertype& r);
	virtual ~BppUsertype();

	virtual BppUsertype* clone		        () const;

//ppUsertype
	virtual IPerst::ERet insertItem       ();
	virtual IPerst::ERet updateItem       ();
	virtual IPerst::ERet deleteItem       ();
	virtual IPerst::ERet selectID         ();
	virtual IPerst::ERet SelForUpd        ();	

	void                 setUsertypeid    (short sUsertypeid);
	void                 setName		  (CBString strUsertypename);
	void				 setDescription   (CBString strUsertypedesc);	
	void				 setUserCount     (long lUserCount);
	
	const short			 getUsertypeid    () const;
	const CBString		 getUsertypeidStr () const;
	const CBString       getName		  () const;
	const CBString       getDescription   () const;
	const long			 getUserCount     () const;
	const CBString       getUserCountStr  () const;
	
protected:
	void                 cpyMember        (const BppUsertype& r);
};

#endif

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
