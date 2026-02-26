/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                     */
/**   Date   : 14.06.2004                                                     */
/**                                                                           */
/******************************************************************************/

/*----------------------------------------------------------------------------*/
// Includes
/*----------------------------------------------------------------------------*/
#include "bsn.h"		// precompiled header

#include <tbl_ppcorappbranchreg.h>
#include <tbl_ppbranch.h>
#include <tbl_ppregion.h>
#include <struct_branchreg.h>
#include <SppBranchReg.h>

#include "BppBranchReg.h"

//---------------------------------------------------------------------------
#define GETBRANCHREGDATA		((SppBranchReg::SListProj &)giveData())

SCBMEB_DEFINE_BSN(BppBranchReg, SppBranchReg::SListProj)

//---------------------------------------------------------------------------
BppBranchReg::BppBranchReg()
{
}

//---------------------------------------------------------------------------
BppBranchReg::BppBranchReg(const BppBranchReg& r) : VBsnObject(r)
{
	cpyMember(r);
}

//---------------------------------------------------------------------------
BppBranchReg::~BppBranchReg()
{
}

/**---------------------------------------------------------------------------*/
BppBranchReg * BppBranchReg::clone() const
{
	return new BppBranchReg(*this);
}

/**---------------------------------------------------------------------------*/
void BppBranchReg::cpyMember(const BppBranchReg & r)
{
    setData((r.giveData()));
}

/**---------------------------------------------------------------------------*/
IPerst::ERet BppBranchReg::deleteRegion()
{
	if (GETBRANCHREGDATA.stListRegion.REGIONID == 0) 
	{
		DbgMsg(__FUNCTION__ ": Regionid 0");
		clearData();
		return IPerst::RET_ERROR;
	}
	else
	{
		SppBranchReg objSppBranchReg;

		VAccess::ERet eRet = objSppBranchReg.deleteReg(GETBRANCHREGDATA);

		switch (eRet)
		{
		IPerst::ERet Ret;
		case VAccess::RET_OK:
			Ret = BSqlStat::ex().UpdateBackupMachine();
			if (Ret == IPerst::RET_ERROR)
			{
				DbgMsg(__FUNCTION__ ": Error on updating backup-machine!");
			}
			break;

		case VAccess::RET_NOTFOUND:
			clearData();

			return IPerst::RET_WARNING;
	
		default:
			assert(0);
			return IPerst::RET_ERROR;
		}
	}

	return IPerst::RET_OK;
}

/**---------------------------------------------------------------------------*/
IPerst::ERet BppBranchReg::SelForUpdBr()
{
	if (GETBRANCHREGDATA.stListBranch.BRANCHID == 0) 
	{
		DbgMsg(__FUNCTION__ ": Branchid 0");
		clearData();
		return IPerst::RET_ERROR;
	}
	else
	{
		SppBranchReg objSppBranch;
		
		VAccess::ERet eRet = objSppBranch.SelTestForUpdBr(GETBRANCHREGDATA);

		switch (eRet)
		{
		case VAccess::RET_OK:
			break;

		case VAccess::RET_NOTFOUND:
			DbgMsg(__FUNCTION__ ": Branchid %d not found in PPBRANCH!", 
				GETBRANCHREGDATA.stListBranch.BRANCHID);
			clearData();

			return IPerst::RET_WARNING;
	
		default:
			assert(0);
			return IPerst::RET_ERROR;
		}
	}

	return IPerst::RET_OK;
}

/**---------------------------------------------------------------------------*/
IPerst::ERet BppBranchReg::SelForUpdReg()
{
	if (GETBRANCHREGDATA.stListRegion.REGIONID == 0) 
	{
		DbgMsg(__FUNCTION__ ": Regionid 0");
		clearData();
		return IPerst::RET_ERROR;
	}
	else
	{
		SppBranchReg objSppReg;
		
		VAccess::ERet eRet = objSppReg.SelTestForUpdReg(GETBRANCHREGDATA);

		switch (eRet)
		{
		case VAccess::RET_OK:
			break;

		case VAccess::RET_NOTFOUND:
			DbgMsg(__FUNCTION__ ": Regionid %d not found in PPREGION!", 
				GETBRANCHREGDATA.stListRegion.REGIONID);
			clearData();

			return IPerst::RET_WARNING;
	
		default:
			assert(0);
			return IPerst::RET_ERROR;
		}
	}

	return IPerst::RET_OK;
}

/**---------------------------------------------------------------------------*/
IPerst::ERet BppBranchReg::deleteRegPrevious	    ()
{
 if (GETBRANCHREGDATA.stListRegion.REGIONID == 0) 
	{
		DbgMsg(__FUNCTION__ ": Regionid 0");
		clearData();
		return IPerst::RET_ERROR;
	}
	else
	{
		SppBranchReg objSppBranchReg;

		VAccess::ERet eRet = objSppBranchReg.deleteRegPrevious(GETBRANCHREGDATA);

		switch (eRet)
		{
		IPerst::ERet Ret;
		case VAccess::RET_OK:
			Ret = BSqlStat::ex().UpdateBackupMachine();
			if (Ret == IPerst::RET_ERROR)
			{
				DbgMsg(__FUNCTION__ ": Error on updating backup-machine!");
			}
			break;

		case VAccess::RET_NOTFOUND:
			clearData();

			return IPerst::RET_WARNING;
	
		default:
			assert(0);
			return IPerst::RET_ERROR;
		}
	}

	return IPerst::RET_OK;
}

/**---------------------------------------------------------------------------*/
IPerst::ERet BppBranchReg::deleteRegCorBranch	    ()
{
	if (GETBRANCHREGDATA.stListRegion.REGIONID == 0) 
	{
		DbgMsg(__FUNCTION__ ": Regionid 0");
		clearData();
		return IPerst::RET_ERROR;
	}
	else
	{
		SppBranchReg objSppBranchReg;

		VAccess::ERet eRet = objSppBranchReg.deleteRegCorBranch(GETBRANCHREGDATA);

		switch (eRet)
		{
		IPerst::ERet Ret;
		case VAccess::RET_OK:
			Ret = BSqlStat::ex().UpdateBackupMachine();
			if (Ret == IPerst::RET_ERROR)
			{
				DbgMsg(__FUNCTION__ ": Error on updating backup-machine!");
			}
			break;

		case VAccess::RET_NOTFOUND:
			clearData();

			return IPerst::RET_WARNING;
	
		default:
			assert(0);
			return IPerst::RET_ERROR;
		}
	}

	return IPerst::RET_OK;
}

/**---------------------------------------------------------------------------*/
IPerst::ERet BppBranchReg::deleteRegAppRight	    ()
{
	if (GETBRANCHREGDATA.stListRegion.REGIONID == 0) 
	{
		DbgMsg(__FUNCTION__ ": Regionid 0");
		clearData();
		return IPerst::RET_ERROR;
	}
	else
	{
		SppBranchReg objSppBranchReg;

		VAccess::ERet eRet = objSppBranchReg.deleteRegAppRight(GETBRANCHREGDATA);

		switch (eRet)
		{
		IPerst::ERet Ret;
		case VAccess::RET_OK:
			Ret = BSqlStat::ex().UpdateBackupMachine();
			if (Ret == IPerst::RET_ERROR)
			{
				DbgMsg(__FUNCTION__ ": Error on updating backup-machine!");
			}
			break;

		case VAccess::RET_NOTFOUND:
			clearData();

			return IPerst::RET_WARNING;
	
		default:
			assert(0);
			return IPerst::RET_ERROR;
		}
	}

	return IPerst::RET_OK;
}

/**---------------------------------------------------------------------------*/
IPerst::ERet BppBranchReg::delRegion()
{
	if (GETBRANCHREGDATA.stListRegion.REGIONID == 0) 
	{
		DbgMsg(__FUNCTION__ ": Regionid 0");
		clearData();
		return IPerst::RET_ERROR;
	}
	else
	{
		IPerst::ERet eRet = deleteRegion();
		if (eRet == IPerst::RET_OK)
		{
			//ppLoginPrevious
			eRet = deleteRegPrevious();
			if (eRet == IPerst::RET_ERROR)
			{
				return eRet;
			}

			//ppCorAppBranchReg
			eRet = deleteRegCorBranch();
			if (eRet == IPerst::RET_ERROR)
			{
				return eRet;
			}

			//ppAppRight
			eRet = deleteRegAppRight();
			if (eRet == IPerst::RET_ERROR)
			{
				return eRet;
			}

			return IPerst::RET_OK;
		}
		
		return eRet;
	}
}

/**---------------------------------------------------------------------------*/
IPerst::ERet BppBranchReg::deleteBrPrevious()
{
	if (GETBRANCHREGDATA.stListBranch.BRANCHID == 0) 
	{
		DbgMsg(__FUNCTION__ ": Branchid 0");
		clearData();
		return IPerst::RET_ERROR;
	}
	else
	{
		SppBranchReg objSppBranchReg;

		VAccess::ERet eRet = objSppBranchReg.deleteBrPrevious(GETBRANCHREGDATA);

		switch (eRet)
		{
		IPerst::ERet Ret;
		case VAccess::RET_OK:
			Ret = BSqlStat::ex().UpdateBackupMachine();
			if (Ret == IPerst::RET_ERROR)
			{
				DbgMsg(__FUNCTION__ ": Error on updating backup-machine!");
			}
			break;

		case VAccess::RET_NOTFOUND:
			clearData();

			return IPerst::RET_WARNING;
	
		default:
			assert(0);
			return IPerst::RET_ERROR;
		}
	}

	return IPerst::RET_OK;
}

/**---------------------------------------------------------------------------*/
IPerst::ERet BppBranchReg::deleteBrCorReg	()
{
	if (GETBRANCHREGDATA.stListBranch.BRANCHID == 0) 
	{
		DbgMsg(__FUNCTION__ ": Branchid 0");
		clearData();
		return IPerst::RET_ERROR;
	}
	else
	{
		SppBranchReg objSppBranchReg;

		VAccess::ERet eRet = objSppBranchReg.deleteBrCorReg(GETBRANCHREGDATA);

		switch (eRet)
		{
		IPerst::ERet Ret;
		case VAccess::RET_OK:
			Ret = BSqlStat::ex().UpdateBackupMachine();
			if (Ret == IPerst::RET_ERROR)
			{
				DbgMsg(__FUNCTION__ ": Error on updating backup-machine!");
			}
			break;

		case VAccess::RET_NOTFOUND:
			clearData();

			return IPerst::RET_WARNING;
	
		default:
			assert(0);
			return IPerst::RET_ERROR;
		}
	}

	return IPerst::RET_OK;
}

/**---------------------------------------------------------------------------*/
IPerst::ERet BppBranchReg::deleteBrAppRight()
{
	if (GETBRANCHREGDATA.stListBranch.BRANCHID == 0) 
	{
		DbgMsg(__FUNCTION__ ": Branchid 0");
		clearData();
		return IPerst::RET_ERROR;
	}
	else
	{
		SppBranchReg objSppBranchReg;

		VAccess::ERet eRet = objSppBranchReg.deleteBrAppRight(GETBRANCHREGDATA);

		switch (eRet)
		{
		IPerst::ERet Ret;
		case VAccess::RET_OK:
			Ret = BSqlStat::ex().UpdateBackupMachine();
			if (Ret == IPerst::RET_ERROR)
			{
				DbgMsg(__FUNCTION__ ": Error on updating backup-machine!");
			}
			break;

		case VAccess::RET_NOTFOUND:
			clearData();

			return IPerst::RET_WARNING;
	
		default:
			assert(0);
			return IPerst::RET_ERROR;
		}
	}

	return IPerst::RET_OK;
}

/**---------------------------------------------------------------------------*/
IPerst::ERet BppBranchReg::delBranch()
{
	if (GETBRANCHREGDATA.stListBranch.BRANCHID == 0) 
	{
		DbgMsg(__FUNCTION__ ": Branchid 0");
		clearData();
		return IPerst::RET_ERROR;
	}
	else
	{
		IPerst::ERet eRet = deleteBranch();
		if (eRet == IPerst::RET_OK)
		{
			//ppLoginPrevious
			eRet = deleteBrPrevious();
			if (eRet == IPerst::RET_ERROR)
			{
				return eRet;
			}

			//ppCorAppBranchReg
			eRet = deleteBrCorReg();
			if (eRet == IPerst::RET_ERROR)
			{
				return eRet;
			}

			//ppAppRight
			eRet = deleteBrAppRight();
			if (eRet == IPerst::RET_ERROR)
			{
				return eRet;
			}

			return IPerst::RET_OK;
		}
		
		return eRet;
	}
}

/**---------------------------------------------------------------------------*/
IPerst::ERet BppBranchReg::deleteBranch()
{
	if (GETBRANCHREGDATA.stListBranch.BRANCHID == 0) 
	{
		DbgMsg(__FUNCTION__ ": Branchid 0");
		clearData();
		return IPerst::RET_ERROR;
	}
	else
	{
		SppBranchReg objSppBranchReg;

		VAccess::ERet eRet = objSppBranchReg.deleteBranch(GETBRANCHREGDATA);

		switch (eRet)
		{
		IPerst::ERet Ret;
		case VAccess::RET_OK:
			Ret = BSqlStat::ex().UpdateBackupMachine();
			if (Ret == IPerst::RET_ERROR)
			{
				DbgMsg(__FUNCTION__ ": Error on updating backup-machine!");
			}
			break;

		case VAccess::RET_NOTFOUND:
			clearData();

			return IPerst::RET_WARNING;
	
		default:
			assert(0);
			return IPerst::RET_ERROR;
		}
	}

	return IPerst::RET_OK;
}

/**---------------------------------------------------------------------------*/
IPerst::ERet BppBranchReg::insertRegion  ()
{
	if (GETBRANCHREGDATA.stListRegion.REGIONID == 0) 
	{
		DbgMsg(__FUNCTION__ ": Regionid 0");
		clearData();
		return IPerst::RET_ERROR;
	}
	else
	{
		SppBranchReg objSppBranchReg;

		VAccess::ERet eRet = objSppBranchReg.insertReg(GETBRANCHREGDATA);

		switch (eRet)
		{
		IPerst::ERet Ret;
		case VAccess::RET_OK:
			Ret = BSqlStat::ex().UpdateBackupMachine();
			if (Ret == IPerst::RET_ERROR)
			{
				DbgMsg(__FUNCTION__ ": Error on updating backup-machine!");
			}
			break;

		case VAccess::RET_NOTFOUND:
			clearData();

			return IPerst::RET_WARNING;
	
		default:
			assert(0);
			return IPerst::RET_ERROR;
		}
	}

	return IPerst::RET_OK;
}



/**---------------------------------------------------------------------------*/
IPerst::ERet BppBranchReg::insertBranch  ()
{
	if (GETBRANCHREGDATA.stListBranch.BRANCHID == 0) 
	{
		DbgMsg(__FUNCTION__ ": Branchid 0");
		clearData();
		return IPerst::RET_ERROR;
	}
	else
	{
		SppBranchReg objSppBranchReg;

		VAccess::ERet eRet = objSppBranchReg.insertBranch(GETBRANCHREGDATA);

		switch (eRet)
		{
		IPerst::ERet Ret;
		case VAccess::RET_OK:
			Ret = BSqlStat::ex().UpdateBackupMachine();
			if (Ret == IPerst::RET_ERROR)
			{
				DbgMsg(__FUNCTION__ ": Error on updating backup-machine!");
			}
			break;

		case VAccess::RET_NOTFOUND:
			clearData();

			return IPerst::RET_WARNING;
	
		default:
			assert(0);
			return IPerst::RET_ERROR;
		}
	}

	return IPerst::RET_OK;
}


/**---------------------------------------------------------------------------*/
IPerst::ERet BppBranchReg::insertAppBranchReg  ()
{
	if (GETBRANCHREGDATA.stListCond.APPLICATIONID == 0) 
	{
		DbgMsg(__FUNCTION__ ": Appid 0");
		clearData();
		return IPerst::RET_ERROR;
	}
	else
	{
		SppBranchReg objSppBranchReg;

		VAccess::ERet eRet = objSppBranchReg.insertAppBranchReg(GETBRANCHREGDATA);

		switch (eRet)
		{
		IPerst::ERet Ret;
		case VAccess::RET_OK:
			Ret = BSqlStat::ex().UpdateBackupMachine();
			if (Ret == IPerst::RET_ERROR)
			{
				DbgMsg(__FUNCTION__ ": Error on updating backup-machine!");
			}
			break;

		case VAccess::RET_NOTFOUND:
			clearData();
			return IPerst::RET_WARNING;

		case VAccess::RET_WARNING:
			clearData();
			return IPerst::RET_WARNING;
	
		default:
			assert(0);
			return IPerst::RET_ERROR;
		}
	}

	return IPerst::RET_OK;
}

/**---------------------------------------------------------------------------*/
IPerst::ERet BppBranchReg::deleteCORReg  ()
{
	if (GETBRANCHREGDATA.stListCond.APPLICATIONID == 0) 
	{
		DbgMsg(__FUNCTION__ ": Appid 0");
		clearData();
		return IPerst::RET_ERROR;
	}
	else
	{
		SppBranchReg objSppBranchReg;

		VAccess::ERet eRet = objSppBranchReg.deleteCORReg(GETBRANCHREGDATA);

		switch (eRet)
		{
		IPerst::ERet Ret;
		case VAccess::RET_OK:
			Ret = BSqlStat::ex().UpdateBackupMachine();
			if (Ret == IPerst::RET_ERROR)
			{
				DbgMsg(__FUNCTION__ ": Error on updating backup-machine!");
			}
			else
			{
				//ppLoginPrevious
				Ret = delCorRegFromPrev();
				if (Ret == IPerst::RET_ERROR)
				{
					return Ret;
				}

				//ppAppRight
				Ret = delCorRegFromAppRight();
				if (Ret == IPerst::RET_ERROR)
				{
					return Ret;
				}
			}
			break;

		case VAccess::RET_NOTFOUND:
			clearData();

			return IPerst::RET_WARNING;
	
		default:
			assert(0);
			return IPerst::RET_ERROR;
		}
	}

	return IPerst::RET_OK;
}
	
/**---------------------------------------------------------------------------*/
IPerst::ERet BppBranchReg::deleteAppBranchReg()
{
	if (GETBRANCHREGDATA.stListCond.APPLICATIONID == 0) 
	{
		DbgMsg(__FUNCTION__ ": Appid 0");
		clearData();
		return IPerst::RET_ERROR;
	}
	else
	{
		SppBranchReg objSppBranchReg;

		VAccess::ERet eRet = objSppBranchReg.deleteAppBranchReg(GETBRANCHREGDATA);

		switch (eRet)
		{
		IPerst::ERet Ret;
		case VAccess::RET_OK:
			Ret = BSqlStat::ex().UpdateBackupMachine();
			if (Ret == IPerst::RET_ERROR)
			{
				DbgMsg(__FUNCTION__ ": Error on updating backup-machine!");
			}
			else
			{
				//ppLoginPrevious
				Ret = delCorBrFromPrev();
				if (Ret == IPerst::RET_ERROR)
				{
					return Ret;
				}

				//ppAppRight
				Ret = delCorBrFromAppRight();
				if (Ret == IPerst::RET_ERROR)
				{
					return Ret;
				}
			}
			break;

		case VAccess::RET_NOTFOUND:
			clearData();

			return IPerst::RET_WARNING;
	
		default:
			assert(0);
			return IPerst::RET_ERROR;
		}
	}

	return IPerst::RET_OK;
}

/**---------------------------------------------------------------------------*/
IPerst::ERet BppBranchReg::updateRegion()
{
	if (GETBRANCHREGDATA.stListRegion.REGIONID == 0) 
	{
		DbgMsg(__FUNCTION__ ": Regionid 0");
		clearData();
		return IPerst::RET_ERROR;
	}
	else
	{
		SppBranchReg objSppBranchReg;

		VAccess::ERet eRet = objSppBranchReg.updateReg(GETBRANCHREGDATA);

		switch (eRet)
		{
		IPerst::ERet Ret;
		case VAccess::RET_OK:
			Ret = BSqlStat::ex().UpdateBackupMachine();
			if (Ret == IPerst::RET_ERROR)
			{
				DbgMsg(__FUNCTION__ ": Error on updating backup-machine!");
			}
			break;

		case VAccess::RET_NOTFOUND:
			clearData();

			return IPerst::RET_WARNING;
	
		default:
			assert(0);
			return IPerst::RET_ERROR;
		}
	}

	return IPerst::RET_OK;
}

/**---------------------------------------------------------------------------*/
IPerst::ERet BppBranchReg::updateBranch()
{
	if (GETBRANCHREGDATA.stListBranch.BRANCHID == 0) 
	{
		DbgMsg(__FUNCTION__ ": Branchid 0");
		clearData();
		return IPerst::RET_ERROR;
	}
	else
	{
		SppBranchReg objSppBranchReg;

		VAccess::ERet eRet = objSppBranchReg.updateBranch(GETBRANCHREGDATA);

		switch (eRet)
		{
		IPerst::ERet Ret;
		case VAccess::RET_OK:
			Ret = BSqlStat::ex().UpdateBackupMachine();
			if (Ret == IPerst::RET_ERROR)
			{
				DbgMsg(__FUNCTION__ ": Error on updating backup-machine!");
			}
			break;

		case VAccess::RET_NOTFOUND:
			clearData();

			return IPerst::RET_WARNING;
	
		default:
			assert(0);
			return IPerst::RET_ERROR;
		}
	}

	return IPerst::RET_OK;
}

/**---------------------------------------------------------------------------*/
IPerst::ERet BppBranchReg::delCorBrFromPrev	()
{
	if (GETBRANCHREGDATA.stListCond.APPLICATIONID == 0) 
	{
		DbgMsg(__FUNCTION__ ": Appid 0");
		clearData();
		return IPerst::RET_ERROR;
	}
	else
	{
		SppBranchReg objSppBranchReg;

		VAccess::ERet eRet = objSppBranchReg.delCorBrFromPrev(GETBRANCHREGDATA);

		switch (eRet)
		{
		IPerst::ERet Ret;
		case VAccess::RET_OK:
			Ret = BSqlStat::ex().UpdateBackupMachine();
			if (Ret == IPerst::RET_ERROR)
			{
				DbgMsg(__FUNCTION__ ": Error on updating backup-machine!");
			}
			break;

		case VAccess::RET_NOTFOUND:
			clearData();

			return IPerst::RET_WARNING;
	
		default:
			assert(0);
			return IPerst::RET_ERROR;
		}
	}

	return IPerst::RET_OK;
}

/**---------------------------------------------------------------------------*/
IPerst::ERet BppBranchReg::delCorBrFromAppRight()
{
	if (GETBRANCHREGDATA.stListCond.APPLICATIONID == 0) 
	{
		DbgMsg(__FUNCTION__ ": Appid 0");
		clearData();
		return IPerst::RET_ERROR;
	}
	else
	{
		SppBranchReg objSppBranchReg;

		VAccess::ERet eRet = objSppBranchReg.delCorBrFromAppRight(GETBRANCHREGDATA);

		switch (eRet)
		{
		IPerst::ERet Ret;
		case VAccess::RET_OK:
			Ret = BSqlStat::ex().UpdateBackupMachine();
			if (Ret == IPerst::RET_ERROR)
			{
				DbgMsg(__FUNCTION__ ": Error on updating backup-machine!");
			}
			break;

		case VAccess::RET_NOTFOUND:
			clearData();

			return IPerst::RET_WARNING;
	
		default:
			assert(0);
			return IPerst::RET_ERROR;
		}
	}

	return IPerst::RET_OK;
}

/**---------------------------------------------------------------------------*/
IPerst::ERet BppBranchReg::delCorRegFromPrev	 ()
{
	if (GETBRANCHREGDATA.stListCond.APPLICATIONID == 0) 
	{
		DbgMsg(__FUNCTION__ ": Appid 0");
		clearData();
		return IPerst::RET_ERROR;
	}
	else
	{
		SppBranchReg objSppBranchReg;

		VAccess::ERet eRet = objSppBranchReg.delCorRegFromPrev(GETBRANCHREGDATA);

		switch (eRet)
		{
		IPerst::ERet Ret;
		case VAccess::RET_OK:
			Ret = BSqlStat::ex().UpdateBackupMachine();
			if (Ret == IPerst::RET_ERROR)
			{
				DbgMsg(__FUNCTION__ ": Error on updating backup-machine!");
			}
			break;

		case VAccess::RET_NOTFOUND:
			clearData();

			return IPerst::RET_WARNING;
	
		default:
			assert(0);
			return IPerst::RET_ERROR;
		}
	}

	return IPerst::RET_OK;
}

/**---------------------------------------------------------------------------*/
IPerst::ERet BppBranchReg::delCorRegFromAppRight()
{
	if (GETBRANCHREGDATA.stListCond.APPLICATIONID == 0) 
	{
		DbgMsg(__FUNCTION__ ": Appid 0");
		clearData();
		return IPerst::RET_ERROR;
	}
	else
	{
		SppBranchReg objSppBranchReg;

		VAccess::ERet eRet = objSppBranchReg.delCorRegFromAppRight(GETBRANCHREGDATA);

		switch (eRet)
		{
		IPerst::ERet Ret;
		case VAccess::RET_OK:
			Ret = BSqlStat::ex().UpdateBackupMachine();
			if (Ret == IPerst::RET_ERROR)
			{
				DbgMsg(__FUNCTION__ ": Error on updating backup-machine!");
			}
			break;

		case VAccess::RET_NOTFOUND:
			clearData();

			return IPerst::RET_WARNING;
	
		default:
			assert(0);
			return IPerst::RET_ERROR;
		}
	}

	return IPerst::RET_OK;
}

/**---------------------------------------------------------------------------*/
void BppBranchReg::setCorBranchid(short sCorBranchid)
{
	GETBRANCHREGDATA.stListProj.BRANCHID = sCorBranchid;
}

/**---------------------------------------------------------------------------*/
void BppBranchReg::setCorRegid(short sCorRegid)
{
	GETBRANCHREGDATA.stListProj.REGIONID = sCorRegid;
}

/**---------------------------------------------------------------------------*/
void BppBranchReg::setCorAppid(short sCorAppid)
{
	GETBRANCHREGDATA.stListCond.APPLICATIONID = sCorAppid;
}

/**---------------------------------------------------------------------------*/
const short	BppBranchReg::getCorBranchid() const
{
	return GETBRANCHREGDATA.stListProj.BRANCHID;
}

/**---------------------------------------------------------------------------*/
const CBString	BppBranchReg::getCorBranchidStr	() const
{
	long2scStr(m_strBufExch, GETBRANCHREGDATA.stListProj.BRANCHID);
	return m_strBufExch;
}

/**---------------------------------------------------------------------------*/
const CBString	BppBranchReg::getCorBranchname() const
{
	m_strBufExch = GETBRANCHREGDATA.stListProj.BRANCHNAME;
	return m_strBufExch;
}

/**---------------------------------------------------------------------------*/
const short	BppBranchReg::getCorRegid() const
{
	return GETBRANCHREGDATA.stListProj.REGIONID;
}

/**---------------------------------------------------------------------------*/
const CBString	BppBranchReg::getCorRegidStr() const
{
	long2scStr(m_strBufExch, GETBRANCHREGDATA.stListProj.REGIONID);
	return m_strBufExch;
}

/**---------------------------------------------------------------------------*/
const CBString	BppBranchReg::getCorRegname() const
{
	m_strBufExch = GETBRANCHREGDATA.stListProj.REGNAME;
	return m_strBufExch;
}

/**---------------------------------------------------------------------------*/
void BppBranchReg::setBranchid(short sBranchid)
{
	GETBRANCHREGDATA.stListBranch.BRANCHID = sBranchid;
}

/**---------------------------------------------------------------------------*/
void BppBranchReg::setRegid(short sRegid)
{
	GETBRANCHREGDATA.stListRegion.REGIONID = sRegid;
}

/**---------------------------------------------------------------------------*/
void BppBranchReg::setAppid	(short sAppid)
{
	GETBRANCHREGDATA.stListCond.APPLICATIONID = sAppid;
}
/**---------------------------------------------------------------------------*/
const short	BppBranchReg::getAppid() const
{
	return GETBRANCHREGDATA.stListCond.APPLICATIONID;
}

/**---------------------------------------------------------------------------*/
void BppBranchReg::setBranchname(CBString strBranchname)
{
	strcpy(GETBRANCHREGDATA.stListBranch.BRANCHNAME, strBranchname);
}

/**---------------------------------------------------------------------------*/
void BppBranchReg::setRegname(CBString strRegname)
{
	strcpy(GETBRANCHREGDATA.stListRegion.REGNAME, strRegname);
}

/**---------------------------------------------------------------------------*/
const short	BppBranchReg::getBranchid() const
{
	return GETBRANCHREGDATA.stListBranch.BRANCHID;
}

/**---------------------------------------------------------------------------*/
const short	BppBranchReg::getRegid() const
{
	return GETBRANCHREGDATA.stListRegion.REGIONID;
}

/**---------------------------------------------------------------------------*/
const CBString	BppBranchReg::getBranchidStr() const
{
	long2scStr(m_strBufExch, GETBRANCHREGDATA.stListBranch.BRANCHID);
	return m_strBufExch;
}

/**---------------------------------------------------------------------------*/
const CBString	BppBranchReg::getRegidStr() const
{
	long2scStr(m_strBufExch, GETBRANCHREGDATA.stListRegion.REGIONID);
	return m_strBufExch;
}

/**---------------------------------------------------------------------------*/
const CBString  BppBranchReg::getBranchname() const
{
	m_strBufExch = GETBRANCHREGDATA.stListBranch.BRANCHNAME;
	return m_strBufExch;
}

/**---------------------------------------------------------------------------*/
const CBString  BppBranchReg::getRegname() const
{
	m_strBufExch = GETBRANCHREGDATA.stListRegion.REGNAME;
	return m_strBufExch;
}

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/	
