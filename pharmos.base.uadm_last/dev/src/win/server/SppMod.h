/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH			                                   */
/**   Date   : 14.06.2004                                                     */
/**                                                                           */
/******************************************************************************/
#ifndef ____SPPMOD_H___
#define ____SPPMOD_H___

#pragma once

/*----------------------------------------------------------------------------*/
class SppMod : public VAccVdooCurs
{
public:
	class SListProj : public VData
	{
	public:
		T_ListProjppAppMod	stListProj;
		T_ListCondppAppMod  stListCond;

					    SListProj   ();
		virtual void	clear		();
		virtual bool	operator == (const VData & r) const;
	};

	
	// -----------------------------------------------------------

							SppMod     ();
	virtual					~SppMod     ();

	VAccess::ERet		insertMod(SListProj  & pSDataSingle );
	VAccess::ERet		deleteMod(SListProj  & pSDataSingle );
	VAccess::ERet		updateMod(SListProj  & pSDataSingle );

	VAccess::ERet		deleteAppModRight	(SListProj  & pSDataSingle );
	VAccess::ERet		deleteppText		(SListProj  & pSDataSingle );
	VAccess::ERet		deleteAppRight		(SListProj  & pSDataSingle );
	
	VAccess::ERet		SelTestForUpd		(SListProj  & pSDataSingle );
	
	
	
	
protected:
	virtual VAccess::ERet		fetch       (VData & pSDataProj);
	virtual VAccess::ERet		openCursor  (VData & pSDataCond);
};


/*----------------------------------------------------------------------------*/

#endif

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
