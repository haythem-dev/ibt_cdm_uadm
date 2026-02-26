/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH			                                   */
/**   Date   : 14.06.2004                                                     */
/**                                                                           */
/******************************************************************************/
#ifndef ____SPPBRANCHREG_H___
#define ____SPPBRANCHREG_H___

#pragma once

/*----------------------------------------------------------------------------*/
class SppBranchReg : public VAccVdooCurs
{
public:
	class SListProj : public VData
	{
	public:
		T_ListProjppBranchReg	stListProj;
		T_ListProjppBranch		stListBranch;
		T_ListProjppRegion		stListRegion;
		T_ListCondppBranchReg   stListCond;

					    SListProj     ();
		virtual void	clear		();
		virtual bool	operator == (const VData & r) const;
	};
	
	// -----------------------------------------------------------

							SppBranchReg     ();
	virtual					~SppBranchReg     ();

//	ppBranch
	VAccess::ERet		insertBranch(SListProj  & pSDataSingle );
	VAccess::ERet		deleteBranch(SListProj  & pSDataSingle );
	VAccess::ERet		updateBranch(SListProj  & pSDataSingle );
	VAccess::ERet		SelTestForUpdBr(SListProj  & pSDataSingle );
	
	VAccess::ERet		deleteBrPrevious	(SListProj  & pSDataSingle );
	VAccess::ERet		deleteBrCorReg		(SListProj  & pSDataSingle );
	VAccess::ERet		deleteBrAppRight	(SListProj  & pSDataSingle );

	VAccess::ERet		delCorBrFromPrev	(SListProj  & pSDataSingle );
	VAccess::ERet		delCorBrFromAppRight(SListProj  & pSDataSingle );
	
//	ppRegion
	VAccess::ERet		insertReg(SListProj  & pSDataSingle );
	VAccess::ERet		deleteReg(SListProj  & pSDataSingle );
	VAccess::ERet		updateReg(SListProj  & pSDataSingle );
	VAccess::ERet		SelTestForUpdReg(SListProj  & pSDataSingle );

	VAccess::ERet		deleteRegPrevious	(SListProj  & pSDataSingle );
	VAccess::ERet		deleteRegCorBranch	(SListProj  & pSDataSingle );
	VAccess::ERet		deleteRegAppRight	(SListProj  & pSDataSingle );

	VAccess::ERet		delCorRegFromPrev	 (SListProj  & pSDataSingle );
	VAccess::ERet		delCorRegFromAppRight(SListProj  & pSDataSingle );
	

//  ppCorAppBranchReg
	VAccess::ERet		insertAppBranchReg(SListProj  & pSDataSingle );
	VAccess::ERet		deleteAppBranchReg(SListProj  & pSDataSingle );
	VAccess::ERet		deleteCORReg	  (SListProj  & pSDataSingle );
	
protected:
	virtual VAccess::ERet		fetch       (VData & pSDataProj);
	virtual VAccess::ERet		openCursor  (VData & pSDataCond);

private:
	bool m_bRegion;
	bool m_bBranch;
};


/*----------------------------------------------------------------------------*/

#endif

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
