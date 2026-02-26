using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using UadmCommon.Classes;
using UadmCommon.Models;

namespace UadmCommon
{
    public enum UADM_JSON_RESPONSE_ERRORCODES
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

    public class LibAppUadmWrapper
    {
        [DllImport("libappuadm.dll", EntryPoint = "CreateCUADMLoginManager", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr CUADMLoginManager();

        [DllImport("libappuadm.dll", EntryPoint = "DestroyCUADMLoginManager", CallingConvention = CallingConvention.Cdecl)]
        public static extern void DestroyCUADMLoginManager(IntPtr mgr);

        [DllImport("libappuadm.dll", EntryPoint = "CUADMLoginManager_Initialize", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        private static extern IntPtr CUADMLoginManager_Initialize(
            IntPtr mgr,
            string strEnv,
            string strName,
            string strPrimaryServer,
            string strPrimaryDatabase,
            string strSecondaryServer,
            string strSecondaryDatabase,
            string strVoodooPipeName,
            string strVoodooPipePort,
            string strAppName,
            string strAppVersion,
            bool useVoodoo
        );

        [DllImport("libappuadm.dll", EntryPoint = "CUADMLoginManager_InitializeCompact", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        private static extern IntPtr CUADMLoginManager_InitializeCompact(IntPtr mgr, string strEnv, string strName, string strAppName, bool useVoodoo);

        [DllImport("libappuadm.dll", EntryPoint = "CUADMLoginManager_InitializeApp", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        private static extern bool CUADMLoginManager_InitializeApp(IntPtr mgr, string strAppName, bool useVoodoo);

        [DllImport("libappuadm.dll", EntryPoint = "CUADMLoginManager_VerifyUser", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        public static extern IntPtr CUADMLoginManager_VerifyUser(IntPtr mgr, string username, string password, bool isPasswordEncrypted = false);

        [DllImport("libappuadm.dll", EntryPoint = "CUADMLoginManager_ChangeUserPassword", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        public static extern IntPtr CUADMLoginManager_ChangeUserPassword(IntPtr mgr, string username, string oldPassword, string newPassword, bool isPasswordEncrypted = false);

        [DllImport("libappuadm.dll", EntryPoint = "CUADMLoginManager_GetPreviousLoginData", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        public static extern IntPtr CUADMLoginManager_GetPreviousLoginData(IntPtr mgr, string osAccount);

        [DllImport("libappuadm.dll", EntryPoint = "CUADMLoginManager_SetPreviousLoginData", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        public static extern IntPtr CUADMLoginManager_SetPreviousLoginData(IntPtr mgr, string osAccount, long userID, int branchRegionId, string country, string language);

        [DllImport("libappuadm.dll", EntryPoint = "CUADMLoginManager_UpdateLastLoginDate", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        public static extern IntPtr CUADMLoginManager_UpdateLastLoginDate(IntPtr mgr, long userID, string username);

        [DllImport("libappuadm.dll", EntryPoint = "CUADMLoginManager_GetAppCountries", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        public static extern IntPtr CUADMLoginManager_GetAppCountries(IntPtr mgr);

        [DllImport("libappuadm.dll", EntryPoint = "CUADMLoginManager_GetAppLanguages", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        public static extern IntPtr CUADMLoginManager_GetAppLanguages(IntPtr mgr);

        [DllImport("libappuadm.dll", EntryPoint = "CUADMLoginManager_GetAppItems", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        public static extern IntPtr CUADMLoginManager_GetAppItems(IntPtr mgr);

        [DllImport("libappuadm.dll", EntryPoint = "CUADMLoginManager_GetAppItemsByCountry", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        public static extern IntPtr CUADMLoginManager_GetAppItemsByCountry(IntPtr mgr, string country);

        [DllImport("libappuadm.dll", EntryPoint = "CUADMLoginManager_GetAllUserPermissionsByUserID", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        public static extern IntPtr CUADMLoginManager_GetAllUserPermissionsByUserID(IntPtr mgr, long userID, int branchRegionId, bool useRegionBranchOnly);

        [DllImport("libappuadm.dll", EntryPoint = "CUADMLoginManager_GetAllAppParameters", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        public static extern IntPtr CUADMLoginManager_GetAllAppParameters(IntPtr mgr);

        [DllImport("libappuadm.dll", EntryPoint = "CUADMLoginManager_GetAppParameters", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        public static extern IntPtr CUADMLoginManager_GetAppParameters(IntPtr mgr, string para1, string para2, string parameterName);

        [DllImport("libappuadm.dll", EntryPoint = "CUADMLoginManager_VerifyUserPermission", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        public static extern IntPtr CUADMLoginManager_VerifyUserPermission(IntPtr mgr, long userID, int branchRegionId, string right);

        [DllImport("libappuadm.dll", EntryPoint = "CUADMLoginManager_GetUserPermissionInfo", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        public static extern IntPtr CUADMLoginManager_GetUserPermissionInfo(IntPtr mgr, long userID, int branchRegionId, string right);

        [DllImport("libappuadm.dll", EntryPoint = "CUADMLoginManager_ReleaseJsonString", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        public static extern bool CUADMLoginManager_ReleaseJsonString(IntPtr mgr, IntPtr jsonString);

        // private members

        private static readonly Dictionary<string, object> _ConnectionLocks = new Dictionary<string, object>();
        private static readonly Dictionary<string, DateTime> _ConnectionLockExpirations = new Dictionary<string, DateTime>();

        private UadmLibEnvironment _uadmLibEnvironment = null;
        private bool _useVoodoo = false;
        private bool _useLock = false;
        private bool _useUserBasedLock = true;
        private int _lockExpirationInMinutes = 10;

        private IntPtr _loginManager;

        public UadmUserData UadmUserData = new UadmUserData();

        public string OsAccount = "AnyUser";
        public string Id = Guid.NewGuid().ToString().ToLower();

        /// <summary>
        /// LibAppUadmWrapper
        /// </summary>
        public LibAppUadmWrapper()
        {
            _useLock = WebConfig.GetUseLock(_useLock);
            _useUserBasedLock = WebConfig.GetUseUserBasedLock(_useUserBasedLock);   
            _lockExpirationInMinutes = WebConfig.GetLockExpiration();
        }

        /// <summary>
        /// ~LibAppUadmWrapper
        /// </summary>
        ~LibAppUadmWrapper()
        {
            CloseConnection();
        }

        /// <summary>
        /// Reset
        /// </summary>
        public void Reset()
        {
            UadmUserData = new UadmUserData();
        }

        #region LOCKS
        /// <summary>
        /// OpenConnection
        /// </summary>
        /// <returns></returns>
        public bool OpenConnection(UadmLibEnvironment uadmLibEnvironment, bool? useVoodoo = null)
        {
            if (_useLock == true)
            {
                lock (CheckConnectionLock(OsAccount))
                {
                    UpdateConnectionLockTimeout(OsAccount);

                    return OpenConnectionInternal(uadmLibEnvironment, useVoodoo);
                }
            }
            return OpenConnectionInternal(uadmLibEnvironment, useVoodoo);
        }

        /// <summary>
        /// CloseConnection
        /// </summary>
        /// <returns></returns>
        public bool CloseConnection()
        {
            if (_useLock == true)
            {
                lock (CheckConnectionLock(OsAccount))
                {
                    UpdateConnectionLockTimeout(OsAccount);
                    return CloseConnectionInternal();
                }
            }
            return CloseConnectionInternal();
        }

        /// <summary>
        /// VerifyUser
        /// </summary>
        /// <param name="username"></param>
        /// <param name="password"></param>
        /// <param name="isPasswordEncrypted"></param>
        /// <returns></returns>
        public UadmLibUser VerifyUser(string username, string password, bool isPasswordEncrypted = false)
        {
            if (_useLock == true)
            {
                lock (CheckConnectionLock(OsAccount))
                {
                    UpdateConnectionLockTimeout(OsAccount);
                    return VerifyUserInternal(username, password, isPasswordEncrypted);
                }
            }
            return VerifyUserInternal(username, password, isPasswordEncrypted);
        }

        /// <summary>
        /// ChangeUserPassword
        /// </summary>
        /// <param name="username"></param>
        /// <param name="oldPassword"></param>
        /// <param name="newPassword"></param>
        /// <param name="isPasswordEncrypted"></param>
        /// <param name="errorMessage"></param>
        /// <param name="errorCode"></param>
        /// <returns></returns>
        public UadmLibUser ChangeUserPassword(string username, string oldPassword, string newPassword, bool isPasswordEncrypted, out int errorCode, out string errorMessage)
        {
            if (_useLock == true)
            {
                lock (CheckConnectionLock(OsAccount))
                {
                    UpdateConnectionLockTimeout(OsAccount);
                    return ChangeUserPasswordInternal(username, oldPassword, newPassword, isPasswordEncrypted, out errorCode, out errorMessage);
                }
            }
            return ChangeUserPasswordInternal(username, oldPassword, newPassword, isPasswordEncrypted, out errorCode, out errorMessage);
        }

        /// <summary>
        /// GetPreviousLoginData
        /// </summary>
        /// <param name="osAccount"></param>
        /// <returns></returns>
        public UadmLibLoginData GetPreviousLoginData(string osAccount)
        {
            if (_useLock == true)
            {
                lock (CheckConnectionLock(OsAccount))
                {
                    UpdateConnectionLockTimeout(OsAccount);
                    return GetPreviousLoginDataInternal(osAccount);
                }
            }
            return GetPreviousLoginDataInternal(osAccount);
        }

        /// <summary>
        /// SetPreviousLoginData
        /// </summary>
        /// <param name="loginData"></param>
        /// <param name="errorCode"></param>
        /// <param name="errorMessage"></param>
        /// <returns></returns>
        public bool SetPreviousLoginData(UadmLibLoginData loginData, out int errorCode, out string errorMessage)
        {
            if (_useLock == true)
            {
                lock (CheckConnectionLock(OsAccount))
                {
                    UpdateConnectionLockTimeout(OsAccount);
                    return SetPreviousLoginDataInternal(loginData, out errorCode, out errorMessage);
                }
            }
            return SetPreviousLoginDataInternal(loginData, out errorCode, out errorMessage);
        }

        /// <summary>
        /// UpdateLastLoginDate
        /// </summary>
        /// <param name="userId"></param>
        /// <param name="username"></param>
        /// <returns></returns>
        public bool UpdateLastLoginDate(long userId, string username)
        {
            if (_useLock == true)
            {
                lock (CheckConnectionLock(OsAccount))
                {
                    UpdateConnectionLockTimeout(OsAccount);
                    return UpdateLastLoginDateInternal(userId, username);
                }
            }
            return UpdateLastLoginDateInternal(userId, username);
        }

        /// <summary>
        /// GetAppCountries
        /// </summary>
        /// <returns></returns>
        public Dictionary<string, UadmLibCountry> GetAppCountries()
        {
            if (_useLock == true)
            {
                lock (CheckConnectionLock(OsAccount))
                {
                    UpdateConnectionLockTimeout(OsAccount);
                    return GetAppCountriesInternal();
                }
            }
            return GetAppCountriesInternal();
        }

        /// <summary>
        /// GetAppLanguages
        /// </summary>
        /// <returns></returns>
        public Dictionary<string, UadmLibLanguage> GetAppLanguages()
        {
            if (_useLock == true)
            {
                lock (CheckConnectionLock(OsAccount))
                {
                    UpdateConnectionLockTimeout(OsAccount);
                    return GetAppLanguagesInternal();
                }
            }
            return GetAppLanguagesInternal();
        }

        /// <summary>
        /// GetAppItems
        /// </summary>
        /// <returns></returns>
        public Dictionary<string, UadmLibApplicationItem> GetAppItems()
        {
            if (_useLock == true)
            {
                lock (CheckConnectionLock(OsAccount))
                {
                    UpdateConnectionLockTimeout(OsAccount);
                    return GetAppItemsInternal();
                }
            }
            return GetAppItemsInternal();
        }

        /// <summary>
        /// GetAppItemsByCountry
        /// </summary>
        /// <param name="countryKey"></param>
        /// <returns></returns>
        public Dictionary<string, UadmLibApplicationItem> GetAppItemsByCountry(string countryKey)
        {
            if (_useLock == true)
            {
                lock (CheckConnectionLock(OsAccount))
                {
                    UpdateConnectionLockTimeout(OsAccount);
                    return GetAppItemsByCountryInternal(countryKey);
                }
            }
            return GetAppItemsByCountryInternal(countryKey);
        }

        /// <summary>
        /// GetAllUserPermissionsByUserId
        /// </summary>
        /// <param name="userId"></param>
        /// <param name="branchRegionId"></param>
        /// <param name="useRegionBranchOnly">  </param>
        /// <returns></returns>
        public List<UadmLibPermission> GetAllUserPermissionsByUserId(long userId, int branchRegionId = -1, bool useRegionBranchOnly = true)
        {
            if (_useLock == true)
            {
                lock (CheckConnectionLock(OsAccount))
                {
                    UpdateConnectionLockTimeout(OsAccount);
                    return GetAllUserPermissionsByUserIdInternal(userId, branchRegionId, useRegionBranchOnly);
                }
            }
            return GetAllUserPermissionsByUserIdInternal(userId, branchRegionId, useRegionBranchOnly);
        }

        /// <summary>
        /// GetAllApplicationParameters
        /// </summary>
        /// <returns></returns>
        public List<UadmLibParameter> GetAllApplicationParameters()
        {
            if (_useLock == true)
            {
                lock (CheckConnectionLock(OsAccount))
                {
                    UpdateConnectionLockTimeout(OsAccount);
                    return GetAllApplicationParametersInternal();
                }
            }
            return GetAllApplicationParametersInternal();
        }

        /// <summary>
        /// GetAppParameter
        /// </summary>
        /// <returns></returns>
        public List<UadmLibParameter> GetAppParameter(string para1 = null, string para2 = null, string parameterName = null)
        {
            if (_useLock == true)
            {
                lock (CheckConnectionLock(OsAccount))
                {
                    UpdateConnectionLockTimeout(OsAccount);
                    return GetAppParameterInternal(para1, para2, parameterName);
                }
            }
            return GetAppParameterInternal(para1, para2, parameterName);
        }

        /// <summary>
        /// VerifyUserPermission
        /// </summary>
        /// <param name="userId"></param>
        /// <param name="branchRegionId"></param>
        /// <param name="right"></param>
        /// <returns></returns>
        public bool VerifyUserPermission(long userId, int branchRegionId, string right)
        {
            if (_useLock == true)
            {
                lock (CheckConnectionLock(OsAccount))
                {
                    UpdateConnectionLockTimeout(OsAccount);
                    return VerifyUserPermissionInternal(userId, branchRegionId, right);
                }
            }
            return VerifyUserPermissionInternal(userId, branchRegionId, right);
        }

        /// <summary>
        /// GetPermissionInfo
        /// </summary>
        /// <param name="userId"></param>
        /// <param name="branchRegionId"></param>
        /// <param name="right"></param>
        /// <returns></returns>
        public UadmLibPermission GetPermissionInfo(long userId, int branchRegionId, string right)
        {
            if (_useLock == true)
            {
                lock (CheckConnectionLock(OsAccount))
                {
                    UpdateConnectionLockTimeout(OsAccount);
                    return GetPermissionInfoInternal(userId, branchRegionId, right);
                }
            }
            return GetPermissionInfoInternal(userId, branchRegionId, right);
        }

        /// <summary>
        /// ReleaseJsonString
        /// </summary>
        /// <param name="jsonPtr"></param>
        public void ReleaseJsonString(IntPtr jsonPtr)
        {
            if (_useLock == true)
            {
                lock (CheckConnectionLock(OsAccount))
                {
                    UpdateConnectionLockTimeout(OsAccount);
                    ReleaseJsonStringInternal(jsonPtr);
                }
            }
            ReleaseJsonStringInternal(jsonPtr);
        }

        #endregion


        #region INTERNALS

        /// <summary>
        /// CheckConnectionLock
        /// </summary>
        /// <param name="osAccount"></param>
        private object CheckConnectionLock(string osAccount)
        {
            if (_useUserBasedLock == false)
            {
                osAccount = "default";
            }

            if (!_ConnectionLocks.ContainsKey(osAccount))
            {
                _ConnectionLocks[osAccount] = new object();
                _ConnectionLockExpirations[osAccount] = DateTime.Now.AddMinutes(_lockExpirationInMinutes);
            }
            else
            {
                if (DateTime.Now > _ConnectionLockExpirations[osAccount])
                {
                    // remove the expired lock
                    _ConnectionLocks.Remove(osAccount);
                    _ConnectionLockExpirations.Remove(osAccount);

                    // create a new lock
                    _ConnectionLocks[osAccount] = new object();
                    _ConnectionLockExpirations[osAccount] = DateTime.Now.AddMinutes(_lockExpirationInMinutes);
                }
            }

            return _ConnectionLocks[osAccount];
        }

        /// <summary>
        /// UpdateConnectionLockTimeout
        /// </summary>
        /// <param name="osAccount"></param>
        private object UpdateConnectionLockTimeout(string osAccount)
        {
            if (_useUserBasedLock == false)
            {
                osAccount = "default";
            }

            if (!_ConnectionLocks.ContainsKey(osAccount))
            {
                _ConnectionLocks[osAccount] = new object();
            }

            _ConnectionLockExpirations[osAccount] = DateTime.Now.AddMinutes(_lockExpirationInMinutes);

            return _ConnectionLocks[osAccount];
        }

        /// <summary>
        /// OpenConnection
        /// </summary>
        /// <returns></returns>
        private bool OpenConnectionInternal(UadmLibEnvironment uadmLibEnvironment, bool? useVoodoo = null)
        {
            UadmLogger.LogDebug($"LibAppUadmWrapper.OpenConnection {Id}, {_loginManager}: starting connection initialization.");

            if (uadmLibEnvironment == null)
            {
                return false;
            }

            _uadmLibEnvironment = uadmLibEnvironment;
            _useVoodoo = useVoodoo == null ? WebConfig.GetUadmUseVoodoo() : useVoodoo.Value;

            // create the login manager object in the dll
            _loginManager = CUADMLoginManager();

            if (_loginManager == IntPtr.Zero)
            {
                UadmLogger.LogError($"LibAppUadmWrapper.OpenConnection {Id}, {_loginManager}: failed to create login manager.");
                return false; // failed to create
            }

            UadmLogger.LogDebug($"LibAppUadmWrapper.OpenConnection {Id}, {_loginManager}: created login manager.");

            // check if we have an environment to connect to
            if (!string.IsNullOrEmpty(_uadmLibEnvironment.Environment))
            {
                IntPtr jsonPtr = IntPtr.Zero;

                try
                {
                    if (string.IsNullOrEmpty(_uadmLibEnvironment.PrimaryServer))
                    {
                        jsonPtr = CUADMLoginManager_InitializeCompact(_loginManager, _uadmLibEnvironment.Environment, _uadmLibEnvironment.EnvironmentName, _uadmLibEnvironment.Application, _useVoodoo);
                    }
                    else
                    {
                        jsonPtr = CUADMLoginManager_Initialize(
                            _loginManager,
                            _uadmLibEnvironment.Environment,
                            _uadmLibEnvironment.EnvironmentName,
                            _uadmLibEnvironment.PrimaryServer,
                            _uadmLibEnvironment.PrimaryDatabase,
                            _uadmLibEnvironment.SecondaryServer,
                            _uadmLibEnvironment.SecondaryDatabase,
                            _uadmLibEnvironment.VoodooPipeName,
                            _uadmLibEnvironment.VoodooPipePort.ToString(),
                            _uadmLibEnvironment.Application,
                            _uadmLibEnvironment.AppVersion,
                            _useVoodoo
                        );
                    }

                    if (jsonPtr == IntPtr.Zero)
                        return false;

                    string jsonString = Marshal.PtrToStringAnsi(jsonPtr);
                    if (string.IsNullOrEmpty(jsonString))
                        return false;

                    UadmLibResponse<List<UadmLibEnvironment>> response = JsonConvert.DeserializeObject<UadmLibResponse<List<UadmLibEnvironment>>>(jsonString);

                    if ((response != null) && (response.ReturnCode != 0))
                    {
                        UadmLogger.LogError($"LibAppUadmWrapper.OpenConnection {Id}, {_loginManager}: " + response.ErrorMessage);
                    }

                    UadmLogger.LogDebug($"LibAppUadmWrapper.OpenConnection {Id}, {_loginManager}: end connection initialization.");

                    return response.ReturnCode == 0;
                }
                catch (Exception e)
                {
                    // log error
                    UadmLogger.LogError($"LibAppUadmWrapper.OpenConnection {Id}, {_loginManager}: " + e.Message.ToString());
                }
                finally
                {
                    if (jsonPtr != IntPtr.Zero)
                    {
                        try
                        {
                            CUADMLoginManager_ReleaseJsonString(_loginManager, jsonPtr);
                        }
                        catch (Exception e)
                        {
                            // log error
                            UadmLogger.LogError(e.Message.ToString());
                        }
                    }
                }
            }

            // initialize with just the app name
            return CUADMLoginManager_InitializeApp(_loginManager, _uadmLibEnvironment.Application, _useVoodoo);
        }

        /// <summary>
        /// CloseConnection
        /// </summary>
        /// <returns></returns>
        private bool CloseConnectionInternal()
        {
            if (_loginManager == IntPtr.Zero)
                return true;

            UadmLogger.LogDebug($"LibAppUadmWrapper.CloseConnection {Id}, {_loginManager}: starting connection close.");

            bool ret = false;
            try
            {
                if (_loginManager != IntPtr.Zero)
                {
                    DestroyCUADMLoginManager(_loginManager);
                    _loginManager = IntPtr.Zero;
                    ret = true;
                }
            }
            catch (Exception e)
            {
                // log error
                UadmLogger.LogError(e.Message.ToString());
            }

            UadmLogger.LogDebug($"LibAppUadmWrapper.CloseConnection {Id}, {_loginManager}: connection closed.");

            return ret;
        }

        /// <summary>
        /// VerifyUser
        /// </summary>
        /// <param name="username"></param>
        /// <param name="password"></param>
        /// <param name="isPasswordEncrypted"></param>
        /// <returns></returns>
        private UadmLibUser VerifyUserInternal(string username, string password, bool isPasswordEncrypted = false)
        {
            UadmLogger.LogDebug($"LibAppUadmWrapper.VerifyUser {Id}, {_loginManager}: starting user verification.");

            UadmLibUser returnValue = null;

            IntPtr jsonPtr = IntPtr.Zero;

            try
            {
                jsonPtr = CUADMLoginManager_VerifyUser(_loginManager, username, password, isPasswordEncrypted);
                if (jsonPtr == IntPtr.Zero)
                    return returnValue;

                string jsonString = Marshal.PtrToStringAnsi(jsonPtr);
                if (string.IsNullOrEmpty(jsonString))
                    return returnValue;

                UadmLibResponse<UadmLibUser> response = JsonConvert.DeserializeObject<UadmLibResponse<UadmLibUser>>(jsonString);
                returnValue = response.Data;
            }
            catch (Exception e)
            {
                // log error
                UadmLogger.LogError(e.Message.ToString());
            }
            finally
            {
                if (jsonPtr != IntPtr.Zero)
                {
                    try
                    {
                        CUADMLoginManager_ReleaseJsonString(_loginManager, jsonPtr);
                    }
                    catch (Exception e)
                    {
                        // log error
                        UadmLogger.LogError(e.Message.ToString());
                    }
                }
            }

            UadmLogger.LogDebug($"LibAppUadmWrapper.VerifyUser {Id}, {_loginManager}: user verification completed.");

            return returnValue;
        }

        /// <summary>
        /// ChangeUserPassword
        /// </summary>
        /// <param name="username"></param>
        /// <param name="oldPassword"></param>
        /// <param name="newPassword"></param>
        /// <param name="isPasswordEncrypted"></param>
        /// <param name="errorMessage"></param>
        /// <param name="errorCode"></param>
        /// <returns></returns>
        private UadmLibUser ChangeUserPasswordInternal(string username, string oldPassword, string newPassword, bool isPasswordEncrypted, out int errorCode, out string errorMessage)
        {
            UadmLibUser returnValue = null;

            IntPtr jsonPtr = IntPtr.Zero;

            errorMessage = "";
            errorCode = 0;

            try
            {
                jsonPtr = CUADMLoginManager_ChangeUserPassword(_loginManager, username, oldPassword, newPassword, isPasswordEncrypted);
                if (jsonPtr == IntPtr.Zero)
                    return returnValue;

                string jsonString = Marshal.PtrToStringAnsi(jsonPtr);
                if (string.IsNullOrEmpty(jsonString))
                    return returnValue;

                UadmLibResponse<UadmLibUser> response = JsonConvert.DeserializeObject<UadmLibResponse<UadmLibUser>>(jsonString);

                if (response == null)
                    return returnValue;

                if (response.ReturnCode != 0)
                {
                    errorMessage = response.ErrorMessage;
                    errorCode = response.ReturnCode;

                    UadmLogger.LogWarning($"ChangeUserPassword: failed to change password for user {username}, error message: {response.ErrorMessage}, error code: {errorCode}");
                }

                returnValue = response.Data;
            }
            catch (Exception e)
            {
                // log error
                UadmLogger.LogError(e.Message.ToString());
            }
            finally
            {
                if (jsonPtr != IntPtr.Zero)
                {
                    try
                    {
                        CUADMLoginManager_ReleaseJsonString(_loginManager, jsonPtr);
                    }
                    catch (Exception e)
                    {
                        // log error
                        UadmLogger.LogError(e.Message.ToString());
                    }
                }
            }
            return returnValue;
        }
        
        /// <summary>
        /// GetPreviousLoginData
        /// </summary>
        /// <param name="osAccount"></param>
        /// <returns></returns>
        private UadmLibLoginData GetPreviousLoginDataInternal(string osAccount)
        {

            UpdateConnectionLockTimeout(OsAccount);

            UadmLogger.LogDebug($"LibAppUadmWrapper.GetPreviousLoginData {Id}, {_loginManager}: starting to get previous login data.");

            UadmLibLoginData returnValue = null;

            IntPtr jsonPtr = IntPtr.Zero;

            try
            {
                jsonPtr = CUADMLoginManager_GetPreviousLoginData(_loginManager, osAccount);
                if (jsonPtr == IntPtr.Zero)
                    return returnValue;

                string jsonString = Marshal.PtrToStringAnsi(jsonPtr);
                if (string.IsNullOrEmpty(jsonString))
                    return returnValue;

                UadmLibResponse<UadmLibLoginData> response = JsonConvert.DeserializeObject<UadmLibResponse<UadmLibLoginData>>(jsonString);
                returnValue = response.Data;
            }
            catch (Exception e)
            {
                // log error
                UadmLogger.LogError(e.Message.ToString());
            }
            finally
            {
                if (jsonPtr != IntPtr.Zero)
                {
                    try
                    {
                        CUADMLoginManager_ReleaseJsonString(_loginManager, jsonPtr);
                    }
                    catch (Exception e)
                    {
                        // log error
                        UadmLogger.LogError(e.Message.ToString());
                    }
                }
            }

            UadmLogger.LogDebug($"LibAppUadmWrapper.GetPreviousLoginData {Id}, {_loginManager}: completed getting previous login data.");

            return returnValue;
        }

        /// <summary>
        /// SetPreviousLoginData
        /// </summary>
        /// <param name="loginData"></param>
        /// <param name="errorCode"></param>
        /// <param name="errorMessage"></param>
        /// <returns></returns>
        private bool SetPreviousLoginDataInternal(UadmLibLoginData loginData, out int errorCode, out string errorMessage)
        {
            bool returnValue = false;

            IntPtr jsonPtr = IntPtr.Zero;

            errorMessage = "";
            errorCode = 0;

            try
            {
                jsonPtr = CUADMLoginManager_SetPreviousLoginData(_loginManager, loginData.OsAccount, loginData.UserId, loginData.BranchRegionId, loginData.Country, loginData.Language);
                if (jsonPtr == IntPtr.Zero)
                    return returnValue;

                string jsonString = Marshal.PtrToStringAnsi(jsonPtr);
                if (string.IsNullOrEmpty(jsonString))
                    return returnValue;

                UadmLibResponse<bool> response = JsonConvert.DeserializeObject<UadmLibResponse<bool>>(jsonString);

                if (response == null)
                    return returnValue;

                if (response.ReturnCode != 0)
                {
                    errorMessage = response.ErrorMessage;
                    errorCode = response.ReturnCode;

                    UadmLogger.LogWarning($"SetPreviousLoginData: failed for user {loginData.OsAccount}, error message: {response.ErrorMessage}, error code: {errorCode}");
                }

                returnValue = response.Data;
            }
            catch (Exception e)
            {
                // log error
                UadmLogger.LogError(e.Message.ToString());
            }
            finally
            {
                if (jsonPtr != IntPtr.Zero)
                {
                    try
                    {
                        CUADMLoginManager_ReleaseJsonString(_loginManager, jsonPtr);
                    }
                    catch (Exception e)
                    {
                        // log error
                        UadmLogger.LogError(e.Message.ToString());
                    }
                }
            }
            
            return returnValue;
        }

        /// <summary>
        /// UpdateLastLoginDate
        /// </summary>
        /// <param name="userId"></param>
        /// <param name="username"></param>
        /// <returns></returns>
        private bool UpdateLastLoginDateInternal(long userId, string username)
        {
            bool returnValue = false;

            IntPtr jsonPtr = IntPtr.Zero;

            try
            {
                jsonPtr = CUADMLoginManager_UpdateLastLoginDate(_loginManager, userId, username);
                if (jsonPtr == IntPtr.Zero)
                    return returnValue;

                string jsonString = Marshal.PtrToStringAnsi(jsonPtr);
                if (string.IsNullOrEmpty(jsonString))
                    return returnValue;

                UadmLibResponse<bool> response = JsonConvert.DeserializeObject<UadmLibResponse<bool>>(jsonString);

                returnValue = response.Data;
            }
            catch (Exception e)
            {
                // log error
                UadmLogger.LogError(e.Message.ToString());
            }
            finally
            {
                if (jsonPtr != IntPtr.Zero)
                {
                    try
                    {
                        CUADMLoginManager_ReleaseJsonString(_loginManager, jsonPtr);
                    }
                    catch (Exception e)
                    {
                        // log error
                        UadmLogger.LogError(e.Message.ToString());
                    }
                }
            }

            return returnValue;
        }

        /// <summary>
        /// GetAppCountries
        /// </summary>
        /// <returns></returns>
        private Dictionary<string, UadmLibCountry> GetAppCountriesInternal()
        {

            UadmLogger.LogDebug($"LibAppUadmWrapper.GetAppCountries {Id}, {_loginManager}: starting to get application countries.");

            Dictionary<string, UadmLibCountry> returnValue = new Dictionary<string, UadmLibCountry>();

            IntPtr jsonPtr = IntPtr.Zero;

            try
            {
                jsonPtr = CUADMLoginManager_GetAppCountries(_loginManager);
                if (jsonPtr == IntPtr.Zero)
                    return returnValue;

                string jsonString = Marshal.PtrToStringAnsi(jsonPtr);
                if (string.IsNullOrEmpty(jsonString))
                    return returnValue;

                UadmLibResponse<List<UadmLibCountry>> response = JsonConvert.DeserializeObject<UadmLibResponse<List<UadmLibCountry>>>(jsonString);
                foreach (UadmLibCountry country in response.Data)
                {
                    returnValue[country.Key] = country;
                }

                if (returnValue.Count == 0)
                {
                    UadmLogger.LogWarning($"LibAppUadmWrapper.GetAppCountries {Id}, {_loginManager}: no countries found, error message:" + response.ErrorMessage);
                }
            }
            catch (Exception e)
            {
                // log error
                UadmLogger.LogError(e.Message.ToString());
            }
            finally
            {
                if (jsonPtr != IntPtr.Zero)
                {
                    try
                    {
                        CUADMLoginManager_ReleaseJsonString(_loginManager, jsonPtr);
                    }
                    catch (Exception e)
                    {
                        // log error
                        UadmLogger.LogError(e.Message.ToString());
                    }
                }
            }

            UadmLogger.LogDebug($"LibAppUadmWrapper.GetAppCountries {Id}, {_loginManager}: completed getting application countries.");

            return returnValue;
        }

        /// <summary>
        /// GetAppLanguages
        /// </summary>
        /// <returns></returns>
        private Dictionary<string, UadmLibLanguage> GetAppLanguagesInternal()
        {

            UadmLogger.LogDebug($"LibAppUadmWrapper.GetAppLanguages {Id}, {_loginManager}: starting to get application languages.");

            Dictionary<string, UadmLibLanguage> returnValue = new Dictionary<string, UadmLibLanguage>();

            IntPtr jsonPtr = IntPtr.Zero;

            try
            {
                jsonPtr = CUADMLoginManager_GetAppLanguages(_loginManager);
                if (jsonPtr == IntPtr.Zero)
                    return returnValue;

                string jsonString = Marshal.PtrToStringAnsi(jsonPtr);
                if (string.IsNullOrEmpty(jsonString))
                    return returnValue;

                UadmLibResponse<List<UadmLibLanguage>> response = JsonConvert.DeserializeObject<UadmLibResponse<List<UadmLibLanguage>>>(jsonString);
                foreach (UadmLibLanguage lang in response.Data)
                {
                    returnValue[lang.Key] = lang;
                }
            }
            catch (Exception e)
            {
                // log error
                UadmLogger.LogError(e.Message.ToString());
            }
            finally
            {
                if (jsonPtr != IntPtr.Zero)
                {
                    try
                    {
                        CUADMLoginManager_ReleaseJsonString(_loginManager, jsonPtr);
                    }
                    catch (Exception e)
                    {
                        // log error
                        UadmLogger.LogError(e.Message.ToString());
                    }
                }
            }

            UadmLogger.LogDebug($"LibAppUadmWrapper.GetAppLanguages {Id}, {_loginManager}: completed getting application languages.");

            return returnValue;
        }

        /// <summary>
        /// GetAppItems
        /// </summary>
        /// <returns></returns>
        private Dictionary<string, UadmLibApplicationItem> GetAppItemsInternal()
        {

            UadmLogger.LogDebug($"LibAppUadmWrapper.GetAppItems {Id}, {_loginManager}: starting to get application items.");

            Dictionary<string, UadmLibApplicationItem> returnValue = new Dictionary<string, UadmLibApplicationItem>();

            IntPtr jsonPtr = IntPtr.Zero;

            try
            {
                jsonPtr = CUADMLoginManager_GetAppItems(_loginManager);
                if (jsonPtr == IntPtr.Zero)
                    return returnValue;

                string jsonString = Marshal.PtrToStringAnsi(jsonPtr);
                if (string.IsNullOrEmpty(jsonString))
                    return returnValue;

                UadmLibResponse<List<UadmLibApplicationItem>> response = JsonConvert.DeserializeObject<UadmLibResponse<List<UadmLibApplicationItem>>>(jsonString);
                foreach (UadmLibApplicationItem item in response.Data)
                {
                    returnValue[item.Key] = item;
                }
            }
            catch (Exception e)
            {
                // log error
                UadmLogger.LogError(e.Message.ToString());
            }
            finally
            {
                if (jsonPtr != IntPtr.Zero)
                {
                    try
                    {
                        CUADMLoginManager_ReleaseJsonString(_loginManager, jsonPtr);
                    }
                    catch (Exception e)
                    {
                        // log error
                        UadmLogger.LogError(e.Message.ToString());
                    }
                }
            }

            UadmLogger.LogDebug($"LibAppUadmWrapper.GetAppItems {Id}, {_loginManager}: completed getting application items.");

            return returnValue;
        }

        /// <summary>
        /// GetAppItemsByCountry
        /// </summary>
        /// <param name="countryKey"></param>
        /// <returns></returns>
        private Dictionary<string, UadmLibApplicationItem> GetAppItemsByCountryInternal(string countryKey)
        {
            UadmLogger.LogDebug($"LibAppUadmWrapper.GetAppItemsByCountry {Id}, {_loginManager}: starting to get application items by country.");

            Dictionary<string, UadmLibApplicationItem> returnValue = new Dictionary<string, UadmLibApplicationItem>();

            IntPtr jsonPtr = IntPtr.Zero;

            try
            {
                jsonPtr = CUADMLoginManager_GetAppItemsByCountry(_loginManager, countryKey);
                if (jsonPtr == IntPtr.Zero)
                    return returnValue;

                string jsonString = Marshal.PtrToStringAnsi(jsonPtr);
                if (string.IsNullOrEmpty(jsonString))
                    return returnValue;

                UadmLibResponse<List<UadmLibApplicationItem>> response = JsonConvert.DeserializeObject<UadmLibResponse<List<UadmLibApplicationItem>>>(jsonString);
                foreach (UadmLibApplicationItem item in response.Data)
                {
                    returnValue[item.Key] = item;
                }
            }
            catch (Exception e)
            {
                // log error
                UadmLogger.LogError(e.Message.ToString());
            }
            finally
            {
                if (jsonPtr != IntPtr.Zero)
                {
                    try
                    {
                        CUADMLoginManager_ReleaseJsonString(_loginManager, jsonPtr);
                    }
                    catch (Exception e)
                    {
                        // log error
                        UadmLogger.LogError(e.Message.ToString());
                    }
                }
            }

            UadmLogger.LogDebug($"LibAppUadmWrapper.GetAppItemsByCountry {Id}, {_loginManager}: completed getting application items by country.");

            return returnValue;
        }

        /// <summary>
        /// GetAllUserPermissionsByUserId
        /// </summary>
        /// <param name="userId"></param>
        /// <param name="branchRegionId"></param>
        /// <param name="useRegionBranchOnly">  </param>
        /// <returns></returns>
        private List<UadmLibPermission> GetAllUserPermissionsByUserIdInternal(long userId, int branchRegionId = -1, bool useRegionBranchOnly = true)
        {
            List<UadmLibPermission> returnValue = new List<UadmLibPermission>();

            IntPtr jsonPtr = IntPtr.Zero;

            try
            {
                if (branchRegionId < 0)
                {
                    branchRegionId = 0;
                    useRegionBranchOnly = false;
                }

                jsonPtr = CUADMLoginManager_GetAllUserPermissionsByUserID(_loginManager, userId, branchRegionId, useRegionBranchOnly);
                if (jsonPtr == IntPtr.Zero)
                    return returnValue;

                string jsonString = Marshal.PtrToStringAnsi(jsonPtr);
                if (string.IsNullOrEmpty(jsonString))
                    return returnValue;

                UadmLibResponse<List<UadmLibPermission>> response = JsonConvert.DeserializeObject<UadmLibResponse<List<UadmLibPermission>>>(jsonString);
                returnValue = response.Data;
            }
            catch (Exception e)
            {
                // log error
                UadmLogger.LogError(e.Message.ToString());
            }
            finally
            {
                if (jsonPtr != IntPtr.Zero)
                {
                    try
                    {
                        CUADMLoginManager_ReleaseJsonString(_loginManager, jsonPtr);
                    }
                    catch (Exception e)
                    {
                        // log error
                        UadmLogger.LogError(e.Message.ToString());
                    }
                }
            }

            return returnValue;
        }

        /// <summary>
        /// GetAllApplicationParameters
        /// </summary>
        /// <returns></returns>
        private List<UadmLibParameter> GetAllApplicationParametersInternal()
        {
            UadmLogger.LogDebug("LibAppUadmWrapper.GetAllApplicationParameters: starting to get all application parameters.");

            List<UadmLibParameter> returnValue = new List<UadmLibParameter>();

            IntPtr jsonPtr = IntPtr.Zero;

            try
            {
                jsonPtr = CUADMLoginManager_GetAllAppParameters(_loginManager);
                if (jsonPtr == IntPtr.Zero)
                    return returnValue;

                string jsonString = Marshal.PtrToStringAnsi(jsonPtr);
                if (string.IsNullOrEmpty(jsonString))
                    return returnValue;

                UadmLibResponse<List<UadmLibParameter>> response = JsonConvert.DeserializeObject<UadmLibResponse<List<UadmLibParameter>>>(jsonString);

                returnValue = response.Data;
            }
            catch (Exception e)
            {
                // log error
                UadmLogger.LogError(e.Message.ToString());
            }
            finally
            {
                if (jsonPtr != IntPtr.Zero)
                {
                    try
                    {
                        CUADMLoginManager_ReleaseJsonString(_loginManager, jsonPtr);
                    }
                    catch (Exception e)
                    {
                        // log error
                        UadmLogger.LogError(e.Message.ToString());
                    }
                }
            }

            UadmLogger.LogDebug("LibAppUadmWrapper.GetAllApplicationParameters: completed getting all application parameters.");

            return returnValue;
        }

        /// <summary>
        /// GetAppParameter
        /// </summary>
        /// <returns></returns>
        private List<UadmLibParameter> GetAppParameterInternal(string para1 = null, string para2 = null, string parameterName = null)
        {
            List<UadmLibParameter> returnValue = null;

            IntPtr jsonPtr = IntPtr.Zero;

            try
            {
                jsonPtr = CUADMLoginManager_GetAppParameters(_loginManager, para1, para2, parameterName);
                if (jsonPtr == IntPtr.Zero)
                    return returnValue;

                string jsonString = Marshal.PtrToStringAnsi(jsonPtr);
                if (string.IsNullOrEmpty(jsonString))
                    return returnValue;

                UadmLibResponse<List<UadmLibParameter>> response = JsonConvert.DeserializeObject<UadmLibResponse<List<UadmLibParameter>>>(jsonString);

                returnValue = response.Data;
            }
            catch (Exception e)
            {
                // log error
                UadmLogger.LogError(e.Message.ToString());
            }
            finally
            {
                if (jsonPtr != IntPtr.Zero)
                {
                    try
                    {
                        CUADMLoginManager_ReleaseJsonString(_loginManager, jsonPtr);
                    }
                    catch (Exception e)
                    {
                        // log error
                        UadmLogger.LogError(e.Message.ToString());
                    }
                }
            }

            return returnValue;
        }

        /// <summary>
        /// VerifyUserPermission
        /// </summary>
        /// <param name="userId"></param>
        /// <param name="branchRegionId"></param>
        /// <param name="right"></param>
        /// <returns></returns>
        private bool VerifyUserPermissionInternal(long userId, int branchRegionId, string right)
        {
            bool returnValue = false;

            IntPtr jsonPtr = IntPtr.Zero;

            try
            {
                jsonPtr = CUADMLoginManager_VerifyUserPermission(_loginManager, userId, branchRegionId, right);
                if (jsonPtr == IntPtr.Zero)
                    return returnValue;

                string jsonString = Marshal.PtrToStringAnsi(jsonPtr);
                if (string.IsNullOrEmpty(jsonString))
                    return returnValue;

                UadmLibResponse<bool> response = JsonConvert.DeserializeObject<UadmLibResponse<bool>>(jsonString);

                returnValue = response.Data;
            }
            catch (Exception e)
            {
                // log error
                UadmLogger.LogError(e.Message.ToString());
            }
            finally
            {
                if (jsonPtr != IntPtr.Zero)
                {
                    try
                    {
                        CUADMLoginManager_ReleaseJsonString(_loginManager, jsonPtr);
                    }
                    catch (Exception e)
                    {
                        // log error
                        UadmLogger.LogError(e.Message.ToString());
                    }
                }
            }

            return returnValue;
        }

        /// <summary>
        /// GetPermissionInfo
        /// </summary>
        /// <param name="userId"></param>
        /// <param name="branchRegionId"></param>
        /// <param name="right"></param>
        /// <returns></returns>
        private UadmLibPermission GetPermissionInfoInternal(long userId, int branchRegionId, string right)
        {
            UadmLibPermission returnValue = null;

            IntPtr jsonPtr = IntPtr.Zero;

            try
            {
                jsonPtr = CUADMLoginManager_GetUserPermissionInfo(_loginManager, userId, branchRegionId, right);
                if (jsonPtr == IntPtr.Zero)
                    return null;

                string jsonString = Marshal.PtrToStringAnsi(jsonPtr);
                if (string.IsNullOrEmpty(jsonString))
                    return null;

                UadmLibResponse<UadmLibPermission> response = JsonConvert.DeserializeObject<UadmLibResponse<UadmLibPermission>>(jsonString);

                returnValue = response.Data;
            }
            catch (Exception e)
            {
                // log error
                UadmLogger.LogError(e.Message.ToString());
            }
            finally
            {
                if (jsonPtr != IntPtr.Zero)
                {
                    try
                    {
                        CUADMLoginManager_ReleaseJsonString(_loginManager, jsonPtr);
                    }
                    catch (Exception e)
                    {
                        // log error
                        UadmLogger.LogError(e.Message.ToString());
                    }
                }
            }

            return returnValue;
        }

        /// <summary>
        /// ReleaseJsonStringInternal
        /// </summary>
        /// <param name="jsonPtr"></param>
        private void ReleaseJsonStringInternal(IntPtr jsonPtr)
        {
            UadmLogger.LogDebug($"LibAppUadmWrapper.ReleaseJsonString {Id}, {_loginManager}: releasing JSON string.");

            try
            {
                CUADMLoginManager_ReleaseJsonString(_loginManager, jsonPtr);
            }
            catch (Exception e)
            {
                // log error
                UadmLogger.LogError(e.Message.ToString());
            }

            UadmLogger.LogDebug($"LibAppUadmWrapper.ReleaseJsonString {Id}, {_loginManager}: JSON string released.");
        }

        #endregion
    }
}
