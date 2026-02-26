/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH			                                   */
/**   Date   : 06.10.2006                                                     */
/**                                                                           */
/******************************************************************************/
#ifndef ____SUSERANALYSIS_H___
#define ____SUSERANALYSIS_H___

#pragma once

/*----------------------------------------------------------------------------*/
class SUserAnalysis : public VAccVdooCurs
{
public:
	class SListProj : public VData
	{
	public:
		T_ListProjUserAnalysis	stListProj;
		T_ListCondUserAnalysis  stListCond;

					    SListProj   ();
		virtual void	clear		();
		virtual bool	operator == (const VData & r) const;
	};
	
	// -----------------------------------------------------------

							SUserAnalysis     ();
	virtual					~SUserAnalysis    ();

	
protected:
	virtual VAccess::ERet		fetch       (VData & pSDataProj);
	virtual VAccess::ERet		openCursor  (VData & pSDataCond);
};


/*----------------------------------------------------------------------------*/

#endif

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
