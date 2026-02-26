#pragma once
#ifndef __UADMHELPER_H____
#define __UADMHELPER_H____


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#include "BppUser.h"
#include "libscmlogin/libscmlogin.h"
#include "CConnect.h"
#include "BConnect.h"
#include "tbl_ppuser.h"
#include "PPCONFIG.h"

};

#include <vector>
class UadmHelper
{
	public:
	UadmHelper(void);
	~UadmHelper(void);

	bool Login(PPCONFIG* config);
	void MapUser(BppUser* user, PPUSER* apiUser);	
private:
	ppBLogIn m_objppBLogIn;
};

#endif // __UADMHELPER_H____

