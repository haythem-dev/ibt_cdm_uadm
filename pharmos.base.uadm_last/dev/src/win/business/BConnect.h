/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH			                                   */
/**   Date   : 25.05.2004                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once

#ifndef ___BCONNECT_H________
#define ___BCONNECT_H________

typedef int  HNMP;         /* Connection-Handle */

class CVdooConn;
/*----------------------------------------------------------------------------*/
class BConnect : public VObject, public VSingleton<BConnect>
{
friend class sccmn::VSingleton<BConnect>;

public:

    virtual                        ~BConnect          ();

	bool                            connect           (CBString& strErrorMsg);
	bool                            disconnect        (CBString& strErrorMsg);

	const CBString                 getCurDatabase    () const;
    const CBString					getServer         () const;

	const int						getSockMain		  () const;
	const int						getSockBackup	  () const;

	bool							beginWork		  (CBString& strErrorMsg);
	bool							commitWork		  (CBString& strErrorMsg);
	bool							rollbackWork	  (CBString& strErrorMsg);

	bool							changeConnection  (CBString& strErrorMsg);
	bool							activateMain	  (CBString& strErrorMsg);
	bool							activateBackup	  (CBString& strErrorMsg);

	void							setNotAct	(CBString strMsg);
	void							setNoActiv	(CBString strMsg);
	void							setNotDis	(CBString strMsg);

	void							writeToLog  (CBString& strMsg);

	void							setVZ		(short sVz);
	void							setUID		(long  lUid);

	void							setNoLoginData ( bool bNoLog );

	//bool                            setCurDatabase    (const CBString  & strDatabase);
	//bool                            setServer         (const CBString  & strHost  );

protected:
                                    BConnect          ();

	bool                            m_bOpen;
	bool                            m_bChange;
	bool							m_bNoLoginData;

private:
	bool isUadm4Test() const;

	HNMP							m_hSockMain;	
	HNMP							m_hSockBackup;	

	CVdooConn *					m_objVdooConnMain;
	CVdooConn *					m_objVdooConnBackup;

	CBString						m_strNotActivated;
	CBString						m_strNoActivConnect;
	CBString						m_strNotAllDisconn;
};


#endif
/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/


