/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH			                                   */
/**   Date   : 16.08.2004                                                     */
/**                                                                           */
/******************************************************************************/
#ifndef ____SPPPREV_H___
#define ____SPPPREV_H___

#pragma once

/*----------------------------------------------------------------------------*/
class SppPrev : public VAccVdooCurs
{
public:
	class SListProj : public VData
	{
	public:
		T_ListProjppPrev	stListProj;
		T_ListCondppPrev    stListCond;

					    SListProj   ();
		virtual void	clear		();
		virtual bool	operator == (const VData & r) const;
	};

	
	// -----------------------------------------------------------

							SppPrev     ();
	virtual					~SppPrev     ();

	VAccess::ERet		deleteSingle			(SListProj  & pSDataSingle );
		
protected:
	virtual VAccess::ERet		fetch       (VData & pSDataProj);
	virtual VAccess::ERet		openCursor  (VData & pSDataCond);
};
/*----------------------------------------------------------------------------*/
#endif
/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
