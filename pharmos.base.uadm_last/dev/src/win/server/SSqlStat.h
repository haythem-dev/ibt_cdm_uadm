/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH		                                     */
/**   Date   : 06.07.2004                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once

#ifndef __SSQLSTAT_H____
#define __SSQLSTAT_H____

class SSqlStat : public VSingleton<SSqlStat>
{
friend class sccmn::VSingleton<SSqlStat>;

	public:
		virtual ~SSqlStat();

		void setStat (CBString & strStat);
		const CBString getStat ();

		VAccess::ERet ExecuteSqlStat	();
	
	protected:
   	    SSqlStat(); 

	private:
		CBString m_strSqlStat;
};

#endif