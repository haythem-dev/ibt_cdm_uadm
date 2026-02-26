/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH			                                   */
/**   Date   : 14.06.2004                                                     */
/**                                                                           */
/******************************************************************************/
#ifndef ____SPPAPPRIGHT_H___
#define ____SPPAPPRIGHT_H___

#pragma once

/*----------------------------------------------------------------------------*/
class SppAppRight : public VAccVdooCurs
{
public:
	class SListProj : public VData
	{
	public:
		T_ListProjppAppRight	stListProj;
		T_ListCondppAppRight	stListCond;

					    SListProj     ();
		virtual void	clear		();
		virtual bool	operator == (const VData & r) const;
	};
	
	// -----------------------------------------------------------

							SppAppRight     ();
	virtual					~SppAppRight     ();

//  ppAppModRight
	VAccess::ERet		insertAppModRight(SListProj  & pSDataSingle );
	VAccess::ERet		deleteAppModRight(SListProj  & pSDataSingle );
	VAccess::ERet		updateAppModRight(SListProj  & pSDataSingle );
	VAccess::ERet		updateApplModRight(SListProj  & pSDataSingle, CBString& strPrevRightname );
	VAccess::ERet		SelTestForUpd	 (SListProj  & pSDataSingle );

	VAccess::ERet		deleteFromAppRight(SListProj  & pSDataSingle );
	

//  ppAppRight
	VAccess::ERet		insertAppRight(SListProj  & pSDataSingle );
	VAccess::ERet		deleteAppRight(SListProj  & pSDataSingle );
	VAccess::ERet		updateApplRight(SListProj  & pSDataSingle, CBString& strPrevRightname  );
	
	
protected:
	virtual VAccess::ERet		fetch       (VData & pSDataProj);
	virtual VAccess::ERet		openCursor  (VData & pSDataCond);
};

#endif

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
