/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH			                                   */
/**   Date   : 09.06.2004                                                     */
/**                                                                           */
/******************************************************************************/
#ifndef ____SPPGROUP_H___
#define ____SPPGROUP_H___

#pragma once

/*----------------------------------------------------------------------------*/
class SppGroup : public VAccVdooCurs
{
public:
	class SListProj : public VData
	{
	public:
		T_ListProjppGroup	stListProj;
		T_ListCondppGroup   stListCond;

					    SListProj   ();
		virtual void	clear		();
		virtual bool	operator == (const VData & r) const;
	};

	
	// -----------------------------------------------------------

							SppGroup     ();
	virtual					~SppGroup     ();

//	Correlation Group <-> User
	VAccess::ERet		insertCorUser(SListProj  & pSDataSingle );
	VAccess::ERet		deleteCorUser(SListProj  & pSDataSingle );
	VAccess::ERet		deleteAllCorUser(SListProj  & pSDataSingle );

//	Group
	VAccess::ERet		insertSingle			(SListProj  & pSDataSingle );
	VAccess::ERet		updateSingle			(SListProj  & pSDataSingle );
	VAccess::ERet		deleteSingle			(SListProj  & pSDataSingle );
	VAccess::ERet		selectID	(CBString &strGroupname, long & lGroupid);
	VAccess::ERet		SelTestForUpd			(SListProj  & pSDataSingle );
		
	VAccess::ERet		deleteAppRight			(SListProj  & pSDataSingle );
	VAccess::ERet		deleteAppCorUserGroup	(SListProj  & pSDataSingle );
	
		
protected:
	virtual VAccess::ERet		fetch       (VData & pSDataProj);
	virtual VAccess::ERet		openCursor  (VData & pSDataCond);
};


/*----------------------------------------------------------------------------*/

#endif

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
