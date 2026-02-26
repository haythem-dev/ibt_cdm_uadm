/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH			                                   */
/**   Date   : 02.06.2004                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once

#ifndef __BTEST_H____
#define __BTEST_H____

class STest;

/*----------------------------------------------------------------------------*/
class BTest
{
public:
									BTest             ();
	virtual                        ~BTest             ();

	virtual bool					SelUNameExists      (bool & bExists);
	virtual bool					SelAppNameExists	(bool & bExists);
	virtual bool					SelModIDExists		(bool & bExists);
	virtual bool					SelRiNameExists		(bool & bExists);
	virtual bool					SelBrIDExists		(bool & bExists);
	virtual bool					SelRegIDExists		(bool & bExists);
	virtual bool					SelGrpNameExists	(bool & bExists);
	virtual bool					SelLangidExists		(bool & bExists);
	virtual bool					SelReftextExists	(bool & bExists);
	virtual bool					SelParaExists		(bool & bExists);
	virtual bool					SelUsertypeNameExists(bool& bExists);
	virtual bool					SelUserParaNameExists(bool& bExists);

    void							setAppid			(short sAppid);
	void							setModid			(short sModid);
	void							setBrRegid			(short sBrRegid);
	void                            setUsername         (CBString strUsername);
	const CBString                 getUsername         () const;
	const short						getCounter			() const;
	void							setAppname			(CBString strAppname);	
	void							setRiname			(CBString strRiName);
	void							setGrpname			(CBString strGrpName);
	void							setLangid			(CBString strLangid);
	void							setReftext			(CBString strReftext);
	void							setPara1			(CBString strPara1);
	void							setPara2			(CBString strPara2);
	void							setParaBez			(CBString strParaBez);
	void                            setUsertypeName     (CBString strUsertypeName);
	void                            setUserParaName     (CBString strUserParaName);


private:
	CBString	m_strUsername;
	CBString   m_strAppname;
	short		m_sAppid;
	short		m_sModid;
	CBString   m_strRiName;
	short		m_sCounter;
	STest *		m_pSTest;
	short		m_sBrRegid;
	CBString   m_strGrpName;
	CBString   m_strLangid;
	CBString	m_strReftext;
	CBString   m_strPara1;
	CBString   m_strPara2;
	CBString   m_strParaBez;
	CBString   m_strUsertypeName;
	CBString   m_strUserParaName;
};

#endif

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
