using System;
using System.Collections.Generic;
using System.Configuration;
using System.Threading.Tasks;
using System.Web;
using System.Web.Mvc;
using UadmCommon.HttpClients;
using UadmCommon.Models;

namespace UadmCommon.Classes
{
    public class LibAppUadmService
    {
        private LibAppUadmWrapper _libAppUadmWrapper = new LibAppUadmWrapper();
        private UadmLoginServiceHttpClient _uadmLoginServiceHttpClient = new UadmLoginServiceHttpClient();

        public bool UseWebRequest { get; set; } = false;

        public string Id { get; } = Guid.NewGuid().ToString().ToLower();

        public LibAppUadmService(HttpContextBase context)
        {
            UseWebRequest = (ConfigurationManager.AppSettings[WebConfig.UADM_USEWEBREQUESTS] ?? "false").ToLower() == "true";

            _libAppUadmWrapper.OsAccount = UadmUserToken.GetWindowsUserId(context);
        }

        public LibAppUadmService(bool useWebRequest)
        {
            UseWebRequest = useWebRequest;
        }

        /// <summary>
        /// GetUadmAllUserPermissionsAsync
        /// </summary>
        /// <param name="context"></param>
        /// <param name="useRegionBranchOnly"></param>
        /// <returns></returns>
        public async Task<List<UadmLibPermission>> GetUadmAllUserPermissionsAsync(HttpContextBase context, bool useRegionBranchOnly = true)
        {           
            UadmLogger.LogDebug($"GetUadmAllUserPermissionsAsync {Id}: UseWebRequest={UseWebRequest.ToString()}, useRegionBranchOnly={useRegionBranchOnly.ToString()}");

            if (UseWebRequest == true)
            {
                // Call through Web API
                if (useRegionBranchOnly == true)
                    return await _uadmLoginServiceHttpClient.GetUadmAllUserPermissionsByBranchAsync(context);
                else
                    return await _uadmLoginServiceHttpClient.GetUadmAllUserPermissionsAsync(context);
            }

            Dictionary<string, string> claims = UadmUserToken.GetUadmUserTokenClaims(context);

            UadmLogger.LogDebug($"GetUadmAllUserPermissionsAsync {Id}: Start");    

            if (UadmUserToken.CheckUserTokenClaimsForUsernameAndApp(claims, "GetUadmAllUserPermissions") == false)
            {
                UadmLogger.LogError($"GetUadmAllUserPermissionsAsync {Id}: CheckUserTokenClaimsForUsernameAndApp return null");
                return null;
            }

            List<UadmLibPermission> permissionList = null;
            try
            {
                // get UadmEnvironment from settings
                UadmLibEnvironment uadmLibEnvironment = claims.GetUadmLibEnvironment();

                UadmLogger.LogDebug($"GetUadmAllUserPermissionsAsync {Id}: UadmLibEnvironment: Application=" + uadmLibEnvironment.Application);

                // open connection to UADML DLL
                if (_libAppUadmWrapper.OpenConnection(uadmLibEnvironment) == false)
                    return null;

                UadmLogger.LogDebug($"GetUadmAllUserPermissionsAsync {Id}: GetAllUserPermissionsByUserId: UserId=" + claims.GetClaimAsLong(UadmUserToken.CLAIM_USERID, -1).ToString());

                permissionList = _libAppUadmWrapper.GetAllUserPermissionsByUserId(claims.GetClaimAsLong(UadmUserToken.CLAIM_USERID, -1), claims.GetClaimAsInt(UadmUserToken.CLAIM_BRANCH), useRegionBranchOnly);
            }
            catch (Exception ex)
            {
                // log error
                UadmLogger.LogError($"GetUadmAllUserPermissionsAsync {Id}: Error in GetUadmAllUserPermissions: " + ex.Message);
            }
            finally
            {
                UadmLogger.LogDebug($"GetUadmAllUserPermissionsAsync {Id}: CloseConnection");
                _libAppUadmWrapper.CloseConnection();
            }

            if (permissionList == null)
            {
                UadmLogger.LogError($"GetUadmAllUserPermissionsAsync {Id}: permissionList is null");
            }
            else
            {
                UadmLogger.LogDebug($"GetUadmAllUserPermissionsAsync {Id}: permissionList count: " + permissionList.Count.ToString());
            }

            return permissionList;
        }

        /// <summary>
        /// GetUadmUserPermissionInfoAsync
        /// </summary>
        /// <param name="context"></param>
        /// <param name="permissionRight"></param>
        /// <returns></returns>
        public async Task<UadmLibPermission> GetUadmUserPermissionInfoAsync(HttpContextBase context, string permissionRight)
        {
            UadmLogger.LogDebug($"GetUadmUserPermissionInfoAsync {Id}: UseWebRequest={UseWebRequest.ToString()}, permissionRight={permissionRight}");

            if (UseWebRequest == true)
            {
                // Call through Web API
                return await _uadmLoginServiceHttpClient.GetUadmUserPermissionInfoAsync(context, permissionRight);
            }

            Dictionary<string, string> claims = UadmUserToken.GetUadmUserTokenClaims(context);

            if (UadmUserToken.CheckUserTokenClaimsForUsernameAndApp(claims, "GetUadmUserPermissionInfo") == false)
            {
                return null;
            }

            if (string.IsNullOrEmpty(permissionRight))
            {
                return null;
            }

            UadmLibPermission permission = null;

            try
            {
                // get UadmEnvironment from settings
                UadmLibEnvironment uadmLibEnvironment = claims.GetUadmLibEnvironment();

                // open connection to UADML DLL
                if (_libAppUadmWrapper.OpenConnection(uadmLibEnvironment) == false)
                    return null;

                permission = _libAppUadmWrapper.GetPermissionInfo(claims.GetClaimAsLong(UadmUserToken.CLAIM_USERID, -1), claims.GetClaimAsInt(UadmUserToken.CLAIM_BRANCH), permissionRight);
            }
            catch (Exception ex)
            {
                // log error
                UadmLogger.LogError("Error in GetUadmUserPermissionInfo: " + ex.Message);
            }
            finally
            {
                _libAppUadmWrapper.CloseConnection();
            }

            return permission;
        }

        /// <summary>
        /// VerifyUserPermissionAsync
        /// </summary>
        /// <param name="context"></param>
        /// <param name="permissionRight"></param>
        /// <returns></returns>
        public async Task<bool> VerifyUserPermissionAsync(HttpContextBase context, string permissionRight)
        {
            UadmLogger.LogDebug($"VerifyUserPermissionAsync {Id}: UseWebRequest={UseWebRequest.ToString()}, permissionRight={permissionRight}");

            if (UseWebRequest == true)
            {
                // Call through Web API
                return await _uadmLoginServiceHttpClient.VerifyUserPermissionAsync(context, permissionRight);
            }

            Dictionary<string, string> claims = UadmUserToken.GetUadmUserTokenClaims(context);

            if (UadmUserToken.CheckUserTokenClaimsForUsernameAndApp(claims, "VerifyUserPermission") == false)
            {
                return false;
            }

            bool verifyOk = false;

            try
            {
                // get UadmEnvironment from settings
                UadmLibEnvironment uadmLibEnvironment = claims.GetUadmLibEnvironment();

                // open connection to UADML DLL
                if (_libAppUadmWrapper.OpenConnection(uadmLibEnvironment) == true)
                {
                    verifyOk = _libAppUadmWrapper.VerifyUserPermission(claims.GetClaimAsLong(UadmUserToken.CLAIM_USERID, -1), claims.GetClaimAsInt(UadmUserToken.CLAIM_BRANCH), permissionRight);
                }
            }
            catch (Exception ex)
            {
                // log error
                UadmLogger.LogError("Error in VerifyUserPermission: " + ex.Message);
            }
            finally
            {
                _libAppUadmWrapper.CloseConnection();
            }

            return verifyOk;
        }

        /// <summary>
        /// GetUadmAllAppParametersAsync
        /// </summary>
        /// <param name="context"></param>
        /// <returns></returns>
        public async Task<List<UadmLibParameter>> GetUadmAllAppParametersAsync(HttpContextBase context)
        {
            UadmLogger.LogDebug($"GetUadmAllAppParametersAsync {Id}: UseWebRequest={UseWebRequest.ToString()}");

            if (UseWebRequest == true)
            {
                // Call through Web API
                return await _uadmLoginServiceHttpClient.GetUadmAllAppParametersAsync(context);
            }

            Dictionary<string, string> claims = UadmUserToken.GetUadmUserTokenClaims(context);

            if (UadmUserToken.CheckUserTokenClaimsForUsernameAndApp(claims, "GetUadmAllParameters") == false)
            {
                return null;
            }

            List<UadmLibParameter> parameters = null;

            try
            {
                // get UadmEnvironment from settings
                UadmLibEnvironment uadmLibEnvironment = claims.GetUadmLibEnvironment();

                // open connection to UADML DLL
                if (_libAppUadmWrapper.OpenConnection(uadmLibEnvironment) == false)
                    return null;

                parameters = _libAppUadmWrapper.GetAllApplicationParameters();                
            }
            catch (Exception ex)
            {
                // log error
                UadmLogger.LogError("Error in GetUadmAllParameters: " + ex.Message);
            }
            finally
            {
                _libAppUadmWrapper.CloseConnection();
            }

            return parameters;
        }

        /// <summary>
        /// GetUadmAppParametersInfoAsync
        /// </summary>
        /// <param name="context"></param>
        /// <param name="para1"></param>
        /// <param name="para2"></param>
        /// <param name="parameterName"></param>
        /// <returns></returns>
        public async Task<List<UadmLibParameter>> GetUadmAppParametersInfoAsync(HttpContextBase context, string para1, string para2, string parameterName)
        {
            UadmLogger.LogDebug($"GetUadmAppParametersInfoAsync {Id}: UseWebRequest={UseWebRequest.ToString()}, para1={para1}, para2={para2}, parameterName={parameterName}");

            if (UseWebRequest == true)
            {
                // Call through Web API
                return await _uadmLoginServiceHttpClient.GetUadmAppParametersInfoAsync(context, para1, para2, parameterName);
            }   

            Dictionary<string, string> claims = UadmUserToken.GetUadmUserTokenClaims(context);

            if (UadmUserToken.CheckUserTokenClaimsForUsernameAndApp(claims, "GetUadmAppParametersInfo") == false)
            {
                return null;
            }

            List<UadmLibParameter> parameters = null;

            try
            {
                // get UadmEnvironment from settings
                UadmLibEnvironment uadmLibEnvironment = claims.GetUadmLibEnvironment();
                
                // open connection to UADML DLL
                if (_libAppUadmWrapper.OpenConnection(uadmLibEnvironment) == false)
                    return null;

                parameters = _libAppUadmWrapper.GetAppParameter(para1, para2, parameterName);
            }
            catch (Exception ex)
            {
                // log error
                UadmLogger.LogError("Error in GetParameters: " + ex.Message);
            }
            finally
            {
                _libAppUadmWrapper.CloseConnection();
            }

            return parameters;
        }

        /// <summary>
        /// Login
        /// </summary>
        /// <param name="context"></param>
        public ActionResult Login(HttpContextBase context)
        {
            UadmLogger.LogDebug($"Login {Id}: Starting login process");

            string loginUrl = ConfigurationManager.AppSettings["UadmLoginWebServer"] ?? "";
            string originUrl = ConfigurationManager.AppSettings["UadmOriginUrl"] ?? context.Request.Url.ToString();
            string applicationKey = ConfigurationManager.AppSettings["UadmApplicationKey"] ?? "";

            // User is not authenticated, redirect to login page
            if (!string.IsNullOrEmpty(originUrl) && UadmValidation.IsValidUrl(originUrl))
            {
                // Add a custom header
                Uri uriResult = new Uri(originUrl);

                HttpCookie cookie = new HttpCookie(UadmCookie.COOKIE_X_PHX_UADM_APPLICATIONKEY, originUrl)
                {
                    Expires = DateTime.Now.AddMinutes(5), // Optional: set expiration
                    Path = uriResult.LocalPath, // Optional: set path
                    Domain = uriResult.Host // Optional: set domain
                };

                context.Response.Cookies.Add(cookie);

                return new RedirectResult(loginUrl + "/login");
            }
            else if (!string.IsNullOrEmpty(applicationKey))
            {
                // Redirect to login with application key
                return new RedirectResult(loginUrl + "/login?app=" + applicationKey);
            }
            else
            {
                // Redirect to login without parameters
                return new RedirectResult(loginUrl + "/login");
            }
        }

        /// <summary>
        /// Logout
        /// </summary>
        /// <param name="context"></param>
        public void Logout(HttpContextBase context)
        {
            UadmLogger.LogDebug($"Logout {Id}: Starting logout process");    

            string appName = WebConfig.GetUadmApplicationKey();

            string appCookie = UadmCookie.GetApplicationUserTokenname(appName);

            HttpCookie cookie = UadmCookie.CreateCookie(appCookie, "", DateTime.Now.AddDays(-1));

            //// Add the cookie to the response
            context.Response.Cookies.Remove(appCookie);
            context.Request.Cookies.Remove(appCookie);

            context.Response.Cookies.Add(cookie);
        }
    }
}
