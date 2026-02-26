/******************************************************************************/
/**                                                                           */
/**   Author : T. Hörath PRZ GmbH			                                   */
/**   Date   : 25.05.2004                                                     */
/**                                                                           */
/******************************************************************************/

#pragma once

#ifndef __BPPUSER_H____
#define __BPPUSER_H____

class SppUser;
/*----------------------------------------------------------------------------*/
class BppUser : public VBsnObject
{
	SCBMEB_DECLARE_BSN(BppUser)
public:
									BppUser             ();
	                                BppUser             (const BppUser & r);
	virtual                        ~BppUser             ();

	virtual BppUser               * clone		        () const;
	virtual IPerst::ERet		load                (bool bLock);
	virtual IPerst::ERet		update              ();
	virtual IPerst::ERet		insert              ();
	virtual IPerst::ERet		delObj              ();
	
	virtual IPerst::ERet		delppUser           ();
	virtual IPerst::ERet		delLoginPrevious    ();
	virtual IPerst::ERet		delAppRight         ();
	virtual IPerst::ERet		delCorAppUserGroup  ();

	virtual IPerst::ERet		selectID            ();

	virtual IPerst::ERet		SelForUpd           ();
	virtual IPerst::ERet		deactivateUserAndSetInactive();



	void                            setUserid           (long lUserid);
	void                            setUsername         (CBString strUsername);
	void                            setPHXPasswd        (CBString strPasswd);
	void                            setAESPasswd        (CBString strPasswd);
	void                            setSurname          (CBString strSurname);
	void                            setForename         (CBString strForename);
	void                            setDescription      (CBString strDescription);
	void                            setMailuser         (CBString strMailuser);
	void                            setEmailuser        (CBString strEmailuser);
	void                            setReference        (CBString strRef);
	void                            setExternal         (short sExternal);
	void                            setChangePW         (short sChangePW);
	void                            setActiv            (short sActiv);
	void                            setNtUser           (CBString strNtUser);
	void							setUserTypeID       (short sUserTypeID);
	void							setUserCountry      (CBString strCountry);
	void                            setUserCountryName  (CBString strCountry);
	void                            setUserTypeName     (CBString strUserTypeName);

	const long						getUserid           () const;
	const CBString					getUseridStr		() const;
	const CBString                 getUsername         () const;
	const CBString                  getPHXPasswd        () const;
	const CBString                  getAESPasswd        () const;
	const CBString                 getSurname          () const;
	const CBString                 getForename         () const;
	const CBString                 getDescription      () const;
	const CBString                 getMailuser         () const;
	const CBString                 getEmailuser        () const;
	const CBString                 getReference        () const;
	const short						getExternal			() const;
	const short						getChangepw			() const;
	const short						getActiv			() const;
	const long						getLastLogin		() const;
	const CBString					getLastLoginStr		() const;
	const CBString                 getNtUser            () const;
	const long						getLastChangePw		() const;
	const short						getUserTypeID       () const;
	const CBString					getUserCountry      () const;
	const CBString                  getListUserCountry  () const;
	const CBString                  getListUserType     () const;
	
protected:
	void                            cpyMember           (const BppUser & r);
	//bool							PWCrypt				(CBString& strPassword);
	//bool							PWDeCrypt			(CBString& strPassword);
	SppUser * m_objSppUser;
};

#endif

/*############################################################################*/
/* End of File                                                                */
/*############################################################################*/
