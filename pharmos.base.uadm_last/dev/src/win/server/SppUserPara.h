/******************************************************************************/
/**                                                                           */
/**   Author : T.ARNOLD NTTDATA                                               */
/**   Date   : 25.11.2025                                                     */
/**                                                                           */
/******************************************************************************/
#ifndef ____SPPUSERPARA_H___
#define ____SPPUSERPARA_H___

#pragma once

/*----------------------------------------------------------------------------*/
class SppUserPara : public VAccVdooCurs
{
public:
	class SListProj : public VData
	{
	public:
		T_ListProjppUserPara	stListProj;
		T_ListCondppUserPara    stListCond;

					    SListProj   ();
		virtual void	clear		();
		virtual bool	operator == (const VData & r) const;
	};

	
	// -----------------------------------------------------------

						SppUserPara  ();
	virtual				~SppUserPara ();

//	UserPara
	VAccess::ERet		insertSingle			(SListProj  & pSDataSingle );
	VAccess::ERet		updateSingle			(SListProj  & pSDataSingle );
	VAccess::ERet		deleteSingle			(SListProj  & pSDataSingle );
	VAccess::ERet		selectID	            (CBString &strUserParaname, short& sUserParaid);
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
