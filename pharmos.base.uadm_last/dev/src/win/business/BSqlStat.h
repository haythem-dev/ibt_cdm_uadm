/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                     */
/**   Date   : 06.07.2004                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once

#ifndef __BSQLSTAT_H____
#define __BSQLSTAT_H____


class BSqlStat : public VSingleton<BSqlStat>
{
friend class sccmn::VSingleton<BSqlStat>;

	public:
		virtual ~BSqlStat();

		virtual IPerst::ERet UpdateBackupMachine();
		
	protected:
		BSqlStat();
   	    
};

#endif