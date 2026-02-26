/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH			                                   */
/**   Date   : 31.05.2005                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once

#ifndef __BCONNECTODBC_H__
#define __BCONNECTODBC_H__


/*----------------------------------------------------------------------------*/
class BConnectOdbc : public VObject, public VSingleton<BConnectOdbc>
{
friend class sccmn::VSingleton<BConnectOdbc>;

public:

    virtual                        ~BConnectOdbc			();

	bool                            connect				(CBString& strErrorMsg);
	bool                            disconnect			(CBString& strErrorMsg);

	bool                            setConnect			(const CBString & strConnect,
		                                                 const CBString & strUID    ,
														 const CBString & strPwd    );

	bool							IsOdbcOpen			();

protected:
                                    BConnectOdbc			();

	bool                            m_bOpen;
};


#endif
/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/


