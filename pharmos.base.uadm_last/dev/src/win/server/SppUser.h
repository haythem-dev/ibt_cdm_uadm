/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH			                                   */
/**   Date   : 24.05.2004                                                     */
/**                                                                           */
/******************************************************************************/
#ifndef ____SPPUSER_H___
#define ____SPPUSER_H___

#pragma once

/*----------------------------------------------------------------------------*/
class SppUser : public VAccVdooCurs
{
public:
	class SComplSingle : public VData
	{
	public:
		T_ListProjppUser	stSingleCond;
		T_SingleProjppUser  stSingleProj;

					    SComplSingle   ();
		virtual void	clear		();
		virtual bool	operator == (const VData & r) const;
	};

	class SListProj : public VData
	{
	public:
		T_ListProjppUser	stListProj;
		T_ListCondppUser	stListCond;

						SListProj();
		virtual void	clear		();
		virtual bool	operator == (const VData & r) const;
	};

	// -----------------------------------------------------------

							SppUser     ();
	virtual					~SppUser     ();

	VAccess::ERet		selectSingle(SComplSingle  & pSDataSingle, bool bLock );
	VAccess::ERet		selectID	(CBString &strUsername, long & iUserid);
	VAccess::ERet		insertSingle(SComplSingle  & pSDataSingle );
	VAccess::ERet		updateSingle(SComplSingle  & pSDataSingle );
	VAccess::ERet		deleteSingle(SComplSingle  & pSDataSingle );
	VAccess::ERet		SelTestForUpd(SComplSingle  & pSDataSingle );
	
	VAccess::ERet		deletePrevious(SComplSingle  & pSDataSingle );
	VAccess::ERet		deleteFromGroup(SComplSingle  & pSDataSingle );
	VAccess::ERet		deleteAppRight(SComplSingle  & pSDataSingle );
	VAccess::ERet		deactivateUserAndSetInactive(SComplSingle& pSDataSingle);
	

protected:
	virtual VAccess::ERet		fetch       (VData & pSDataProj);
	virtual VAccess::ERet		openCursor  (VData & pSDataCond);


private:
	HDYNSQL m_UpdHandle;
	
};


/*----------------------------------------------------------------------------*/

#endif

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
