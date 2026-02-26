#if !defined(UADM_LOGINMANAGERWRAPPER_H)
#define UADM_LOGINMANAGERWRAPPER_H

#if _MSC_VER > 1000
#pragma once
#endif

#include <string>
#include <vector>
#include <algorithm>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef LIBAPPUADM_SHARED
#define LIBAPPUADM_API __declspec(dllexport)
#else
#define LIBAPPUADM_API
#endif

namespace UADM
{
	// Add this to UADMLoginManager.h

	enum UADM_JSON_RESPONSE_ERRORCODES
	{
		UADM_JSON_RESPONSE_OK = 0,
		UADM_JSON_RESPONSE_ERROR = -1,
		UADM_JSON_RESPONSE_COULD_NOT_INIITIALIZE = -2,
		UADM_JSON_RESPONSE_COULD_NOT_VERIFY_USER = -3,
		UADM_JSON_RESPONSE_HISTORY_VERIFY_FAILED = -4,
		UADM_JSON_RESPONSE_LAST_PASSWORD_VERIFY_FAILED = -5,
		UADM_JSON_RESPONSE_LOGININTERFACE_IS_NULL = -6,
		UADM_JSON_RESPONSE_MANAGER_IS_NULL = -7,
		UADM_JSON_RESPONSE_NO_ENVIRONMENT_FOUND = -8,
		UADM_JSON_RESPONSE_NO_PREVIOUS_LOGIN_DATA_FOUND = -9,
		UADM_JSON_RESPONSE_PASSWORD_NOT_VALID = -10,
		UADM_JSON_RESPONSE_SAVE_PASSWORD_FAILED = -11,
		UADM_JSON_RESPONSE_USER_IS_NOT_ACTIVATED = -12,
		UADM_JSON_RESPONSE_USER_IS_NOT_AUTHENTICATED = -13,
	};

	LIBAPPUADM_API CUADMLoginManager* CreateCUADMLoginManager();
	LIBAPPUADM_API void DestroyCUADMLoginManager(CUADMLoginManager* mgr);

	LIBAPPUADM_API const char* CUADMLoginManager_Initialize(UADM::CUADMLoginManager* mgr, const char* strEnv, const char* strName, const char* strPrimaryServer, const char* strPrimaryDatabase, const char* strSecondaryServer, const char* strSecondaryDatabase, const char* strVoodooPipeName, const char* strVoodooPipePort, const char* strAppName, const char* strAppVersion, bool useVoodo);
	LIBAPPUADM_API const char* CUADMLoginManager_InitializeCompact(UADM::CUADMLoginManager* mgr, const char* strEnv, const char* strName, const char* strAppName, bool useVoodo);

	LIBAPPUADM_API bool CUADMLoginManager_InitializeApp(UADM::CUADMLoginManager* mgr, const char* strAppName, bool useVoodo);
	LIBAPPUADM_API const char* CUADMLoginManager_GetPreviousLoginData(UADM::CUADMLoginManager* mgr, const char* osAccount);
	LIBAPPUADM_API const char* CUADMLoginManager_SetPreviousLoginData(UADM::CUADMLoginManager* mgr, const char* osAccount, long lngUserID, int branchRegion, const char* country, const char* language);

	LIBAPPUADM_API const char* CUADMLoginManager_VerifyUser(UADM::CUADMLoginManager* mgr, const char* username, const char* password, const bool isPasswordEncrypted);
	LIBAPPUADM_API const char* CUADMLoginManager_ChangeUserPassword(UADM::CUADMLoginManager* mgr, const char* username, const char* password, const char* newPassword, const bool isPasswordEncrypted);
	LIBAPPUADM_API const char* CUADMLoginManager_UpdateLastLoginDate(UADM::CUADMLoginManager* mgr, const long lngUserID, const char* username);
	LIBAPPUADM_API const char* CUADMLoginManager_GetAppCountries(UADM::CUADMLoginManager* mgr);
	LIBAPPUADM_API const char* CUADMLoginManager_GetAppItems(UADM::CUADMLoginManager* mgr);
	LIBAPPUADM_API const char* CUADMLoginManager_GetAppItemsByCountry(UADM::CUADMLoginManager* mgr, const char* country);
	LIBAPPUADM_API const char* CUADMLoginManager_GetAppLanguages(UADM::CUADMLoginManager* mgr);

	LIBAPPUADM_API const char* CUADMLoginManager_GetAllUserPermissionsByUserID(UADM::CUADMLoginManager* mgr, const long lngUserID, const int branchRegion, const bool blnRegionBranchOnly);
	LIBAPPUADM_API const char* CUADMLoginManager_GetAppParameters(UADM::CUADMLoginManager* mgr, const char* para1, const char* para2, const char* parameterName);
	LIBAPPUADM_API const char* CUADMLoginManager_GetAllAppParameters(UADM::CUADMLoginManager* mgr);
	LIBAPPUADM_API const char* CUADMLoginManager_VerifyUserPermission(UADM::CUADMLoginManager* mgr, const long lngUserID, const int branchRegion, const char* permission);
	LIBAPPUADM_API const char* CUADMLoginManager_GetUserPermissionInfo(UADM::CUADMLoginManager* mgr, const long lngUserID, const int branchRegion, const char* permission);

	LIBAPPUADM_API const bool CUADMLoginManager_ReleaseJsonString(UADM::CUADMLoginManager* mgr, char* ptr);

}
#ifdef __cplusplus
};
#endif /* __cplusplus */

#endif // UADM_LOGINMANAGERWRAPPER_H