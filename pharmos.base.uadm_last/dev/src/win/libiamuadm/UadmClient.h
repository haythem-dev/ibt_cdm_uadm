#pragma once

#ifndef __UADMCLIENT_H____
#define __UADMCLIENT_H____

#include "BppUser.h"
#include "libscmlogin/libscmlogin.h"
#include "CConnect.h"
#include "BConnect.h"
#include "tbl_ppuser.h"
#include <vector>
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#include "UadmHelper.h"
#ifdef _WINDLL
#define PROTOTYPE __declspec(dllexport)
#else
#define PROTOTYPE
#endif
#include "PPCONFIG.h"
#include "C_DELETE_IDENTITY_REQUEST.h"

	// Interface functions

	//user management
	PROTOTYPE int DeleteUser(C_DELETE_IDENTITY_REQUEST* request, PPCONFIG* config);//deactivete user set inactive only no deletion
	PROTOTYPE long GetUserIdbyName(char* username, PPCONFIG* config);

	// helper functions
	int Init(PPCONFIG *config);

	UadmHelper* m_UadmHelper;
	bool m_IsInitialized;
};

#endif // __UADMCLIENT_H____