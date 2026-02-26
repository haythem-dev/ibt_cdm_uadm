/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH			                                   */
/**   Date   : 06.10.2006                                                     */
/**                                                                           */
/******************************************************************************/
#ifndef ____SUSERGROUPANALYSIS_H___
#define ____SUSERGROUPANALYSIS_H___

#pragma once

/*----------------------------------------------------------------------------*/
class SUserGroupAnalysis : public VAccVdooCurs
{
public:
	class SListProj : public VData
	{
	public:
		T_ListProjUserGroupAnalysis	stListProj;
		T_ListCondUserGroupAnalysis  stListCond;

					    SListProj   ();
		virtual void	clear		();
		virtual bool	operator == (const VData & r) const;
	};
	
	// -----------------------------------------------------------

							SUserGroupAnalysis     ();
	virtual					~SUserGroupAnalysis    ();

	
protected:
	virtual VAccess::ERet		fetch       (VData & pSDataProj);
	virtual VAccess::ERet		openCursor  (VData & pSDataCond);
};


/*----------------------------------------------------------------------------*/

#endif

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
