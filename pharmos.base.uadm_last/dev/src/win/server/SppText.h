/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH			                                   */
/**   Date   : 06.07.2004                                                     */
/**                                                                           */
/******************************************************************************/
#ifndef ____SPPTEXT_H___
#define ____SPPTEXT_H___

#pragma once

/*----------------------------------------------------------------------------*/
class SppText : public VAccVdooCurs
{
public:
	class SListProj : public VData
	{
	public:
		T_ListProjppText	stListProj;
		T_ListCondppText    stListCond;

					    SListProj   ();
		virtual void	clear		();
		virtual bool	operator == (const VData & r) const;
	};

	
	// -----------------------------------------------------------

							SppText   ();
	virtual					~SppText  ();

	VAccess::ERet		insertText				(SListProj  & pSDataSingle );
	VAccess::ERet		updateText				(SListProj  & pSDataSingle );
	VAccess::ERet		deleteLangText			(SListProj  & pSDataSingle );
	VAccess::ERet		deleteRefText			(SListProj  & pSDataSingle );

	VAccess::ERet		selectTextEN			(SListProj  & pSDataSingle, CBString & strTextEN );
		
	VAccess::ERet	SelTestForUpd			(SListProj  & pSDataSingle );

protected:
	virtual VAccess::ERet		fetch       (VData & pSDataProj);
	virtual VAccess::ERet		openCursor  (VData & pSDataCond);
};

#endif

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
