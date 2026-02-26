/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH			                                   */
/**   Date   : 01.06.2004                                                     */
/**                                                                           */
/******************************************************************************/
#ifndef ____SPPAPP_H___
#define ____SPPAPP_H___

#pragma once

/*----------------------------------------------------------------------------*/
class SppApp : public VAccVdooCurs
{
public:
	class SListAppProj : public VData
	{
	public:
		T_ListProjApp	stListAppProj;

						SListAppProj();
		virtual void	clear		();
		virtual bool	operator == (const VData & r) const;
	};

	// -----------------------------------------------------------

									SppApp     ();
	virtual						   ~SppApp     ();

	VAccess::ERet	insertSingle			(SListAppProj  & pSDataSingle );
	VAccess::ERet	updateSingle			(SListAppProj  & pSDataSingle );
	VAccess::ERet	deleteSingle			(SListAppProj  & pSDataSingle );
	VAccess::ERet	selectID		(CBString &strAppname, long & lAppid);
	VAccess::ERet	SelTestForUpd			(SListAppProj  & pSDataSingle );
	
	
	VAccess::ERet	deletePrevious			(SListAppProj  & pSDataSingle );
	VAccess::ERet	deleteCorAppUserGroup	(SListAppProj  & pSDataSingle );
	VAccess::ERet	deleteCorAppLang		(SListAppProj  & pSDataSingle );
	VAccess::ERet	deleteppText			(SListAppProj  & pSDataSingle );
	VAccess::ERet	deleteAppModule			(SListAppProj  & pSDataSingle );
	VAccess::ERet	deleteCorAppBranchReg	(SListAppProj  & pSDataSingle );
	VAccess::ERet	deleteAppModRight		(SListAppProj  & pSDataSingle );
	VAccess::ERet	deleteAppRight			(SListAppProj  & pSDataSingle );
	
	
protected:
	virtual VAccess::ERet		fetch       (VData & pSDataProj);
	virtual VAccess::ERet		openCursor  (VData & pSDataCond);
};


/*----------------------------------------------------------------------------*/

#endif

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
