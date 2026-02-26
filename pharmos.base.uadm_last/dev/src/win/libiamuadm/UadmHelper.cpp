#include "stdafx.h"
#include "UadmHelper.h"

UadmHelper::UadmHelper(void)
{
}

UadmHelper::~UadmHelper(void)
{
}

bool UadmHelper::Login(PPCONFIG *config)
{
	ppBLoginData::ex().createObjCLoginData();
	// Sets default values to current environment values
	CConnect::HOST = config->MAIN_HOST;
	CConnect::HOST_BACKUP = config->BACKUP_HOST;
	CConnect::DATABASE = config->MAIN_DB;
	CConnect::DATABASE_BACKUP = config->BACKUP_DB;
	m_objppBLogIn.setConnectionConfig(CConnect::HOST, CConnect::HOST_BACKUP, CConnect::DATABASE, CConnect::DATABASE_BACKUP);
	return true;
}

void UadmHelper::MapUser(BppUser* user, PPUSER* apiUser)
{
	apiUser->USERID= user->getUserid();
	strcpy_s(apiUser->USERNAME, user->getUsername().c_str());
	strcpy_s(apiUser->PASSWORD, user->getPHXPasswd().c_str());
	strcpy_s(apiUser->SURNAME, user->getSurname().c_str());
	strcpy_s(apiUser->FORENAME, user->getForename().c_str());
	strcpy_s(apiUser->DESCRIPTION, user->getDescription().c_str());
	strcpy_s(apiUser->MAILUSER, user->getMailuser().c_str());
	strcpy_s(apiUser->EMAILUSER, user->getEmailuser().c_str());
	apiUser->CHANGEPW = user->getChangepw();
	apiUser->ACTIV = user->getActiv();
	apiUser->LASTLOGIN = user->getLastLogin();
	strcpy_s(apiUser->NTUSER, user->getNtUser().c_str());
	apiUser->LASTPWCHANGE = user->getLastChangePw();
	strcpy_s(apiUser->REFERENCE, user->getReference().c_str());
	apiUser->EXTERNAL = user->getExternal();
	strcpy_s(apiUser->PWD_ENC, user->getAESPasswd().c_str());
	apiUser->USERTYPEID = user->getUserTypeID();
	strcpy_s(apiUser->COUNTRY, user->getUserCountry().c_str());

}	
