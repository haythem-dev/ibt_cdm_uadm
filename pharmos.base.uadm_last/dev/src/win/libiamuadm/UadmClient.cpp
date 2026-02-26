// Date: 2021-01-19

// UadmClient.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include "stdafx.h"
#include "UadmClient.h"




/// <summary>
///  Internal function used for getting user id by username
/// </summary>
/// <param name="user"></param>
/// <param name="config"></param>
/// <returns>User Id </returns>
long GetUserIdbyName(char* user, PPCONFIG* config)
{
	if (Init(config) == -1)
	{
		return -1;
	}

	CBString error = "";
	BppUser *userb = new BppUser();
	userb->setUsername(user);
	IPerst::ERet eRet;

	if (BConnect::ex().connect(error))
	{
		eRet = userb->selectID();

		if (eRet == IPerst::RET_OK)
		{
			return userb->getUserid();
		}
		else
		{
			return -1;
		}
	}
	else
	{
		return -1;
	}
}
int DeleteUser(C_DELETE_IDENTITY_REQUEST* request, PPCONFIG* config)
{
	if (Init(config) == -1)
	{
		return -1;
	}
	//important to load the user because we only get the username it has to be unique
	long userId = GetUserIdbyName(request->username, config);

	if (userId == 0)
	{
		DbgMsg(__FUNCTION__ ": User not found!");
		return -1;
	}

	CBString error = "";
	BppUser* userb = new BppUser();
	userb->setUserid(userId);
	IPerst::ERet eRet;

	if (BConnect::ex().connect(error))
	{
		eRet = userb->deactivateUserAndSetInactive();

		if (eRet == IPerst::RET_OK)
		{
			return 0;
		}
		else
		{
			return -1;
		}
	}
	else
	{
		return -1;
	}
}


int Init(PPCONFIG* config)
{
	if (m_IsInitialized)
	{
		return 0;
	}

	m_UadmHelper = new UadmHelper();
	if (m_UadmHelper->Login(config))
	{
		m_IsInitialized = true;
		return 0;
	}
	else
	{
		return -1;
	}
	
}
