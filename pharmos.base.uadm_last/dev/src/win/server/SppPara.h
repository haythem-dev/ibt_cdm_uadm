/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH			                                   */
/**   Date   : 03.08.2004                                                     */
/**                                                                           */
/******************************************************************************/
#ifndef ____SPPPARA_H___
#define ____SPPPARA_H___

#pragma once

/*----------------------------------------------------------------------------*/
class SppPara : public VAccVdooCurs
{
public:
	class SListProj : public VData
	{
	public:
		T_ListProjppPara	stListProj;
		T_ListCondppPara    stListCond;

					    SListProj   ();
		virtual void	clear		();
		virtual bool	operator == (const VData & r) const;
	};

	
	// -----------------------------------------------------------

							SppPara     ();
	virtual					~SppPara     ();

	VAccess::ERet		insertPara(SListProj  & pSDataSingle );
	VAccess::ERet		deletePara(SListProj  & pSDataSingle );
	VAccess::ERet		updatePara(SListProj  & pSDataSingle );

	VAccess::ERet		SelTestForUpd(SListProj  & pSDataSingle );

		
protected:
	virtual VAccess::ERet		fetch       (VData & pSDataProj);
	virtual VAccess::ERet		openCursor  (VData & pSDataCond);
};


/*----------------------------------------------------------------------------*/

#endif

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/