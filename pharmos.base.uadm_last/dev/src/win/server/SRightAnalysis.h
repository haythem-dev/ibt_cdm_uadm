/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH			                                   */
/**   Date   : 30.08.2004                                                     */
/**                                                                           */
/******************************************************************************/
#ifndef ____SRIGHTANALYSIS_H___
#define ____SRIGHTANALYSIS_H___

#pragma once

/*----------------------------------------------------------------------------*/
class SRightAnalysis : public VAccVdooCurs
{
public:
	class SListProj : public VData
	{
	public:
		T_ListProjAnalysis	stListProj;
		T_ListCondAnalysis  stListCond;

					    SListProj   ();
		virtual void	clear		();
		virtual bool	operator == (const VData & r) const;
	};

	
	// -----------------------------------------------------------

							SRightAnalysis     ();
	virtual					~SRightAnalysis     ();

	
protected:
	virtual VAccess::ERet		fetch       (VData & pSDataProj);
	virtual VAccess::ERet		openCursor  (VData & pSDataCond);
};


/*----------------------------------------------------------------------------*/

#endif

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
