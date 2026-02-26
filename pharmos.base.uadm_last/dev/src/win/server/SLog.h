/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH                                             */
/**   Date   : 24.08.2004                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once

class VSCSVdooConn;
/*----------------------------------------------------------------------------*/
class SLog : public VAccess, public VSingleton<SLog>
{
friend class sccmn::VSingleton<SLog>;

public:
    virtual                        ~SLog          ();

	VAccess::ERet				    write			(const char * const spFunction, 
		                                             VAccess::ERet           eError, 
													 const char * const spMsg, ...);

	void							setVZ			(short sVZ);
	void							setUID			(long  lUID);
	void							setActivConn	(CVdooConn* objVdooActivConn);


protected:
                                    SLog            ();

	short				m_sVZ;
	long				m_lUID;
	const char * const  m_spFuncWrite;
	CVdooConn *		m_objVdooActivConn;
};

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
