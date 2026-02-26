/******************************************************************************/
/**                                                                           */
/**   Author : T.ARNOLD NTTDATA                                               */
/**   Date   : 25.11.2025                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once

#ifndef __BPPUSERPARA_H____
#define __BPPUSERPARA_H____

/*----------------------------------------------------------------------------*/
class BppUserPara : public VBsnObject
{
	SCBMEB_DECLARE_BSN(BppUserPara)
public:
	BppUserPara();
	BppUserPara(const BppUserPara& r);
	virtual ~BppUserPara();

	virtual BppUserPara* clone		        () const;

//ppUserPara
	virtual IPerst::ERet insertItem       ();
	virtual IPerst::ERet updateItem       ();
	virtual IPerst::ERet deleteItem       ();
	virtual IPerst::ERet selectID         ();
	virtual IPerst::ERet SelForUpd        ();	

	void                 setUserParaid    (short sUserParaid);
	void                 setName		  (CBString strUserParaname);
	void				 setDescription   (CBString strUserParadesc);
	void				 setUserCount     (long lUserCount);
	
	const short			 getUserParaid    () const;
	const CBString		 getUserParaidStr () const;
	const CBString       getName		  () const;
	const CBString       getDescription   () const;
	const long			 getUserCount     () const;
	const CBString       getUserCountStr  () const;
	
protected:
	void                 cpyMember        (const BppUserPara& r);
};

#endif

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
