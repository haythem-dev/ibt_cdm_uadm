/******************************************************************************/
/**                                                                           */
/**   Author : T.ARNOLD NTTDATA                                               */
/**   Date   : 25.11.2025                                                     */
/**                                                                           */
/******************************************************************************/
#ifndef ____SPPUSERTYPE_H___
#define ____SPPUSERTYPE_H___

#pragma once

/*----------------------------------------------------------------------------*/
class SppUsertype : public VAccVdooCurs
{
public:
	class SListProj : public VData
	{
	public:
		T_ListProjppUsertype	stListProj;
		T_ListCondppUsertype    stListCond;

					    SListProj   ();
		virtual void	clear		();
		virtual bool	operator == (const VData & r) const;
	};

	
	// -----------------------------------------------------------

						SppUsertype  ();
	virtual				~SppUsertype ();

//	Usertype
	VAccess::ERet		insertSingle			(SListProj  & pSDataSingle );
	VAccess::ERet		updateSingle			(SListProj  & pSDataSingle );
	VAccess::ERet		deleteSingle			(SListProj  & pSDataSingle );
	VAccess::ERet		selectID	            (CBString &strUsertypename, short& sUsertypeid);
	VAccess::ERet		SelTestForUpd			(SListProj  & pSDataSingle );	
		
protected:
	virtual VAccess::ERet		openCursor  (VData& pSDataCond);
	virtual VAccess::ERet		fetch       (VData& pSDataProj);
};


/*----------------------------------------------------------------------------*/

#endif

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
