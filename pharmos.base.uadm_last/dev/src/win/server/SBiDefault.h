/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH			                                   */
/**   Date   : 06.06.2005                                                     */
/**                                                                           */
/******************************************************************************/
#ifndef __SBIDEFAULT_H__
#define __SBIDEFAULT_H__

#pragma once

/*----------------------------------------------------------------------------*/
class SBiDefault : public VAccOdbcCurs 
{
public:
	class SListProj : public VData
	{
	public:
		T_ListProjIbiUser	stListProj;
		T_ListCondIbiUser   stListCond;

					    SListProj   ();
		virtual void	clear		();
		virtual bool	operator == (const VData & r) const;
	};

	
	// -----------------------------------------------------------

							SBiDefault     ();
	virtual					~SBiDefault     ();


	VAccess::ERet		insertIBIUser			(SListProj  & pSDataSingle );
	VAccess::ERet		updateIBIUser			(SListProj  & pSDataSingle );
	VAccess::ERet		selcountIBIUser			(SListProj  & pSDataSingle, int * piCountIbiUsr );

	VAccess::ERet		selcountIBIUserCountryR			(SListProj  & pSDataSingle, int * piCountCountryR );
	VAccess::ERet		insertIBIUserCountryRights		(SListProj  & pSDataSingle );
	
			
protected:
	virtual VAccess::ERet		fetch       (VData & pSDataProj);
	virtual VAccess::ERet		openCursor  (VData & pSDataCond);

	
	//virtual void		DoBulkFieldExchange	(CFieldExchange * pFX);

};
/*----------------------------------------------------------------------------*/
#endif
/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
