/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH			                                   */
/**   Date   : 06.10.2006                                                     */
/**                                                                           */
/******************************************************************************/
#ifndef ____SUSERRIGHTANALYSIS_H___
#define ____SUSERRIGHTANALYSIS_H___

#pragma once

/*----------------------------------------------------------------------------*/
class SUserRightAnalysis : public VAccVdooCurs
{
public:
	class SListProj : public VData
	{
	public:
		T_ListProjUserRightAnalysis  stListProj;
		T_ListCondUserRightAnalysis  stListCond;

					    SListProj   ();
		virtual void	clear		();
		virtual bool	operator == (const VData & r) const;
	};
	
	// -----------------------------------------------------------

							SUserRightAnalysis     ();
	virtual					~SUserRightAnalysis    ();

	
protected:
	virtual VAccess::ERet		fetch       (VData & pSDataProj);
	virtual VAccess::ERet		openCursor  (VData & pSDataCond);
};


/*----------------------------------------------------------------------------*/

#endif

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
