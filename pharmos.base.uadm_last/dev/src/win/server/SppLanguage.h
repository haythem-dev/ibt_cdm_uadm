/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH			                                   */
/**   Date   : 18.06.2004                                                     */
/**                                                                           */
/******************************************************************************/
#ifndef ____SPPLANGUAGE_H___
#define ____SPPLANGUAGE_H___

#pragma once

/*----------------------------------------------------------------------------*/
class SppLanguage : public VAccVdooCurs
{
public:
	class SListProj : public VData
	{
	public:
		T_ListProjLanguage	stListProj;
		T_ListCondLanguage  stListCond;

					    SListProj   ();
		virtual void	clear		();
		virtual bool	operator == (const VData & r) const;
	};

	
	// -----------------------------------------------------------

							SppLanguage   ();
	virtual					~SppLanguage  ();

//	Correlation Language <-> Appl.
	VAccess::ERet		insertCorLang	(SListProj  & pSDataSingle );
	VAccess::ERet		deleteCorLang	(SListProj  & pSDataSingle );
	VAccess::ERet		deleteCorppText	(SListProj  & pSDataSingle );


//	LoginLanguage
	VAccess::ERet		insertSingle			(SListProj  & pSDataSingle );
	VAccess::ERet		updateSingle			(SListProj  & pSDataSingle );
	VAccess::ERet		deleteSingle			(SListProj  & pSDataSingle );
		
	VAccess::ERet		deletePrevious			(SListProj  & pSDataSingle );
	VAccess::ERet		deleteAppCorLang		(SListProj  & pSDataSingle );
	VAccess::ERet		deleteppText			(SListProj  & pSDataSingle );

	VAccess::ERet		SelTestForUpd			(SListProj  & pSDataSingle );
	
	
	
protected:
	virtual VAccess::ERet		fetch       (VData & pSDataProj);
	virtual VAccess::ERet		openCursor  (VData & pSDataCond);
};


/*----------------------------------------------------------------------------*/

#endif

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
