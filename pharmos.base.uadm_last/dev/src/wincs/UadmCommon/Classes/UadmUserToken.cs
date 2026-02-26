using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Net.Http;
using System.Security.Principal;
using System.Text;
using System.Web;
using UadmCommon.Models;

namespace UadmCommon.Classes
{
    public static class UadmUserToken
    {
        public enum UadmUserTokenError
        {             
            None,
            InvalidFormat,
            InvalidParameter,
            InvalidBase64Format,
            InvalidJsonFormat,
            ChecksumMismatch,
            TokenExpired,
            ClaimsInvalid,
            InvalidApplicationId,
            InvalidOriginUrlOrId,
            OriginUrlOrIdMissing,
        }

        public static readonly string TOKEN_SCHEMA = "UadmToken ";

        public static readonly string CLAIM_APPLICATION = "application";
        public static readonly string CLAIM_APPLICATIONID = "applicationid";
        public static readonly string CLAIM_BRANCH = "branch";
        public static readonly string CLAIM_COUNTRY = "country";
        public static readonly string CLAIM_CREATED = "created";
        public static readonly string CLAIM_DESCRIPTION = "description";
        public static readonly string CLAIM_DOMAINACCOUNT = "domainaccount";
        public static readonly string CLAIM_EMAIL = "email";
        public static readonly string CLAIM_ENVIRONMENT = "environment";
        public static readonly string CLAIM_EXPIRES = "expires";
        public static readonly string CLAIM_FIRSTNAME = "firstname";
        public static readonly string CLAIM_ISACTIVE = "isactive";
        public static readonly string CLAIM_ISAUTHENTICATED = "isauthenticated";
        public static readonly string CLAIM_ISPASSWORDCHANGEREQUIRED = "ispasswordchangerequired";
        public static readonly string CLAIM_LANGUAGE = "language";
        public static readonly string CLAIM_LASTLOGINDATE = "lastlogindate";
        public static readonly string CLAIM_LASTNAME = "lastname";
        public static readonly string CLAIM_LASTPASSWORDCHANGEDATE = "lastpasswordchangedate";
        public static readonly string CLAIM_MODIFIED = "modified";
        public static readonly string CLAIM_OSACCOUNT = "osaccount";
        public static readonly string CLAIM_USERID = "userid";
        public static readonly string CLAIM_USERNAME = "username";

        /// <summary>
        /// IsUserAuthenticated
        /// </summary>
        /// <param name="context"></param>
        /// <returns></returns>
        public static bool IsUserAuthenticated(HttpContextBase context)
        {
            Dictionary<string, string> claims = GetUadmUserTokenClaims(context);
            if (claims == null)
                return false;

            return true;
        }

        /// <summary>
        /// IsValidCheckApplicationId
        /// </summary>
        /// <param name="claims"></param>
        /// <param name="logWarnings"></param>
        /// <returns></returns>
        public static bool IsValidCheckApplicationId(Dictionary<string, string> claims, bool logWarnings = true)
        {
            if (claims == null)
                return false;

            string applicationId = WebConfig.GetUadmApplicationId();
            if (string.IsNullOrEmpty(applicationId))
                return true;

            string applicationIdClaim = claims.GetClaim(CLAIM_APPLICATIONID);
            if (string.IsNullOrEmpty(applicationIdClaim))
                return false;

            // check if application id matches
            if (!applicationIdClaim.Equals(applicationId, StringComparison.OrdinalIgnoreCase))
            {
                if (logWarnings)
                {
                    string osAccountClaim = claims.GetClaim(CLAIM_OSACCOUNT, claims.GetClaim(CLAIM_USERNAME), true);
                    UadmLogger.LogWarning($"UadmIsAuthenticatedAttribute: AplicationId '{applicationId.ToLower()}' does not match the claim in the user token '{applicationIdClaim.ToLower()}' for user: '{osAccountClaim}'.");
                }                            
                return false;
            }

            return true;
        }

        /// <summary>
        /// GetUadmUserTokenClaims
        /// </summary>
        /// <param name="context"></param>
        /// <param name="ignoreCookieForTokenSearch"></param>
        /// <returns></returns>
        public static Dictionary<string, string> GetUadmUserTokenClaims(HttpContextBase context, bool ignoreCookieForTokenSearch = false)
        {
            string userToken;
            Dictionary<string, string> claims;
            UadmUserTokenError uadmUserTokenError = UadmUserTokenError.None;
            bool cookieUsed;

            // first try to get from cookie
            if (ignoreCookieForTokenSearch == false)
            {
                // try to get from cookie first
                userToken = GetUserToken(context, true, out cookieUsed);
                if (string.IsNullOrEmpty(userToken))
                    return null;

                // now chck claims
                claims = GetUadmUserTokenClaims(context, userToken, cookieUsed, out uadmUserTokenError);
                if ((claims != null) && (claims.Count > 0))
                {
                    if (IsValidCheckApplicationId(claims, false))
                    {
                        return claims;
                    }

                    uadmUserTokenError = UadmUserTokenError.InvalidApplicationId;
                }
            }

            // if expired try to get from header or querystring
            if ((uadmUserTokenError == UadmUserTokenError.TokenExpired) || (uadmUserTokenError == UadmUserTokenError.InvalidApplicationId) || ignoreCookieForTokenSearch)
            {                 
                // try to get from header or querystring if expired
                userToken = GetUserToken(context, false, out cookieUsed);
                if (string.IsNullOrEmpty(userToken))
                    return null;

                claims = GetUadmUserTokenClaims(context, userToken, cookieUsed, out uadmUserTokenError);
                if ((claims != null) && (claims.Count > 0))
                {
                    if (IsValidCheckApplicationId(claims, true))
                    {
                        return claims;
                    }

                    uadmUserTokenError = UadmUserTokenError.InvalidApplicationId;
                }
            }

            UadmLogger.LogDebug("GetUadmUserTokenClaims: uadmUserTokenError=" + uadmUserTokenError.ToString());

            return null;
        }

        /// <summary>
        /// GetUadmUserToken
        /// </summary>
        /// <param name="context"></param>
        /// <param name="ignoreCookieForTokenSearch"></param>
        /// <returns></returns>
        public static string GetUadmUserToken(HttpContextBase context, bool ignoreCookieForTokenSearch = false)
        {
            string userToken = null;
            Dictionary<string, string> claims;
            UadmUserTokenError uadmUserTokenError = UadmUserTokenError.None;
            bool cookieUsed;

            // first try to get from cookie
            if (ignoreCookieForTokenSearch == false)
            {
                // try to get from cookie first
                userToken = GetUserToken(context, true, out cookieUsed);
                if (string.IsNullOrEmpty(userToken))
                    return null;

                // try to get from cookie first
                claims = GetUadmUserTokenClaims(context, userToken, cookieUsed, out uadmUserTokenError);
                if ((claims != null) && (claims.Count > 0))
                {
                    if (IsValidCheckApplicationId(claims, false))
                    {
                        return userToken;
                    }

                    uadmUserTokenError = UadmUserTokenError.InvalidApplicationId;
                }
            }

            // if expired try to get from header or querystring
            if ((uadmUserTokenError == UadmUserTokenError.TokenExpired) || (uadmUserTokenError == UadmUserTokenError.InvalidApplicationId) || ignoreCookieForTokenSearch)
            {
                // try to get from header or querystring if expired
                userToken = GetUserToken(context, false, out cookieUsed);
                if (string.IsNullOrEmpty(userToken))
                    return null;

                claims = GetUadmUserTokenClaims(context, userToken, cookieUsed, out uadmUserTokenError);
                if ((claims != null) && (claims.Count > 0))
                {
                    if (IsValidCheckApplicationId(claims, true))
                    {
                        return userToken;
                    }

                    uadmUserTokenError = UadmUserTokenError.InvalidApplicationId;
                }
            }

            UadmLogger.LogDebug("GetUadmUserToken: uadmUserTokenError=" + uadmUserTokenError.ToString());

            return null;
        }

        /// <summary>
        /// GetUadmUserTokenClaims
        /// </summary>
        /// <param name="context"></param>
        /// <param name="userToken"></param>
        /// <param name="cookieUsed"></param>
        /// <param name="uadmUserTokenError"></param>
        /// <returns></returns>
        public static Dictionary<string, string> GetUadmUserTokenClaims(HttpContextBase context, string userToken, bool cookieUsed, out UadmUserTokenError uadmUserTokenError)
        {
            string originUrlOrId = WebConfig.GetUadmOriginUrlOrId(context.Request.Url.ToString());

            uadmUserTokenError = UadmUserTokenError.InvalidParameter;

            if (string.IsNullOrEmpty(userToken))
                return null;

            string[] parts = userToken.Split('.');
            if (parts.Length != 2)
                return null;

            uadmUserTokenError = UadmUserTokenError.InvalidFormat;

            // create checksum from current data            
            string cryptedToken = Crypt.EncryptToBase64(GetWindowsUserId(context), GetHashValue(parts[0]));
            if (string.IsNullOrEmpty(cryptedToken))
                return null;

            uadmUserTokenError = UadmUserTokenError.ChecksumMismatch;

            // check if checksum valid
            if ((cryptedToken != parts[1]) && cookieUsed)
            {
                cryptedToken = Crypt.EncryptToBase64("", GetHashValue(parts[0]));
                if (string.IsNullOrEmpty(cryptedToken))
                    return null;

                if (cryptedToken != parts[1])
                    return null;
            }

            uadmUserTokenError = UadmUserTokenError.InvalidBase64Format;

            // decode data
            byte[] byteArray = UadmValidation.ConvertFromBase64String(parts[0]);
            if (byteArray == null || byteArray.Length == 0)
                return null;

            string data = Encoding.UTF8.GetString(byteArray);

            if (string.IsNullOrEmpty(data))
                return null;

            uadmUserTokenError = UadmUserTokenError.InvalidJsonFormat;

            Dictionary<string, string> claims;
            try
            {
                // check claims
                claims = JsonConvert.DeserializeObject<Dictionary<string, string>>(data);
                if (claims == null || claims.Count == 0)
                    return null;
            }
            catch (Exception ex)
            {
                UadmLogger.LogError("GetUadmUserTokenClaims: JsonConvert.DeserializeObject failed: " + ex.Message);
                return null;
            }

            uadmUserTokenError = UadmUserTokenError.TokenExpired;

            long currentTicks = DateTime.Now.Ticks;
            DateTime dateTime = new DateTime(long.Parse(claims[CLAIM_MODIFIED]));

            int delay = int.Parse(claims[CLAIM_EXPIRES]);

            // if delay is 0 set to max value (1 week)
            if (delay <= 0)
                delay = 7 * 24 * 60;

            dateTime = dateTime.AddMinutes(delay);

            // check if token expired
            if (dateTime.Ticks < currentTicks)
                return null;

            uadmUserTokenError = UadmUserTokenError.None;

            return claims;
        }

        /// <summary>
        /// GetUadmAuthorizationToken
        /// </summary>
        /// <param name="context"></param>
        /// <returns></returns>
        public static string GetUadmAuthorizationToken(HttpContextBase context)
        { 
            string userToken = GetUadmUserToken(context);
            if (string.IsNullOrEmpty(userToken))
                return null;

            return TOKEN_SCHEMA + userToken;
        }

        /// <summary>
        /// GetUserToken
        /// </summary>
        /// <param name="context"></param>
        /// <param name="useCookie"></param>
        /// <param name="cookieUsed"></param>
        /// <returns></returns>
        public static string GetUserToken (HttpContextBase context, bool useCookie, out bool cookieUsed)
        {
            cookieUsed = true;

            // Read the "CTM" cookie from the request
            string place = "cookie";
            string userToken = "";

            if (useCookie)
                userToken = UadmCookie.GetFromHttpContextRequestCookie(context, UadmCookie.GetApplicationUserTokenname(WebConfig.GetUadmApplicationKey()));
            
            if (string.IsNullOrEmpty(userToken))
            {
                place = "header/HEADER_UADMTOKEN";
                // try to get from header
                userToken = UadmRequestValues.GetValueFromHeader(context, UadmRequestValues.HEADER_UADMUSERTOKEN, "");
                if (!string.IsNullOrEmpty(userToken) && userToken.StartsWith(TOKEN_SCHEMA))
                {
                    userToken = userToken.Substring(TOKEN_SCHEMA.Length).Trim();
                    cookieUsed = false;
                }
                else
                {
                    place = "header/HEADER_AUTHORIZATION";
                    userToken = UadmRequestValues.GetValueFromHeader(context, UadmRequestValues.HEADER_AUTHORIZATION, "");
                    if (!string.IsNullOrEmpty(userToken) && userToken.StartsWith(TOKEN_SCHEMA))
                    {
                        userToken = userToken.Substring(TOKEN_SCHEMA.Length).Trim();
                        cookieUsed = false;
                    }
                    else
                    {
                        place = "querystring";
                        userToken = UadmRequestValues.GetValueFromQueryString (context, UadmRequestValues.REQUESTVALUES_PHX_UADM_USERTOKEN, "");

                        if (!string.IsNullOrEmpty(userToken))
                        {
                            cookieUsed = false;
                        }
                    }
                }
            }

            UadmLogger.LogDebug("GetUserToken: place=" + place + " userToken=" + (string.IsNullOrEmpty(userToken) ? "<null>" : userToken.Length > 10 ? userToken.Substring(0, 10) + "..." : userToken));

            return userToken ?? "";    
        }

        /// <summary>
        /// UpdateUadmUserToken
        /// </summary>
        /// <param name="context"></param>
        /// <returns></returns>
        public static string UpdateUadmUserToken(HttpContextBase context)
        {
            Dictionary<string, string> claims = GetUadmUserTokenClaims(context);
            if (claims == null)
                return null;

            return CreateUadmUserToken(context, claims);
        }

        /// <summary>
        /// CreateUadmUserToken
        /// </summary>
        /// <param name="context"></param>
        /// <param name="claims"></param>
        /// <param name="key"></param>
        /// <returns></returns>
        public static string CreateUadmUserToken(HttpContextBase context, Dictionary<string, string> claims, string key = null)
        {
            return CreateUadmUserToken(GetWindowsUserId(context), claims, key); 
        }

        /// <summary>
        /// CreateUadmUserToken
        /// </summary>
        /// <param name="userId"></param>
        /// <param name="claims"></param>
        /// <param name="key"></param>
        /// <returns></returns>
        public static string CreateUadmUserToken(string userId, Dictionary<string, string> claims, string key = null)
        {
            if (claims == null)
                return null;

            claims[CLAIM_MODIFIED] = DateTime.Now.Ticks.ToString();

            string userToken = JsonConvert.SerializeObject(claims);
            string userTokenBase64 = Convert.ToBase64String(System.Text.Encoding.UTF8.GetBytes(userToken));

            string cryptedToken = Crypt.EncryptToBase64(userId, GetHashValue(userTokenBase64), key);
            userTokenBase64 += "." + cryptedToken;

            return userTokenBase64;
        }

        /// <summary>
        /// GetHashValue
        /// </summary>
        /// <param name="text"></param>
        /// <returns></returns>
        public static string GetHashValue (string text)
        {
            if (string.IsNullOrEmpty(text))
                return null;
            return string.Format("0x{0:X8}", text.GetHashCode());
        }   

        /// <summary>
        /// GetClaim
        /// </summary>
        /// <param name="context"></param>
        /// <param name="name"></param>
        /// <param name="defValue"></param>
        /// <returns></returns>
        public static string GetClaim(HttpContextBase context, string name, string defValue = "")
        {
            Dictionary<string, string> claims = GetUadmUserTokenClaims(context);
            if (claims == null)
                return defValue;

            return GetClaim(claims, name, defValue);
        }

        /// <summary>
        /// GetClaim
        /// </summary>
        /// <param name="claims"></param>
        /// <param name="name"></param>
        /// <param name="defValue"></param>
        /// <param name="checkIfEmpty"></param>
        /// <returns></returns>
        public static string GetClaim(this Dictionary<string, string> claims, string name, string defValue = "", bool checkIfEmpty = false)
        {
            if (claims == null)
                return defValue;    

            if (!claims.ContainsKey(name))
                return defValue;

            if (checkIfEmpty && string.IsNullOrEmpty(claims[name]))
                return defValue;

            return claims[name];
        }

        /// <summary>
        /// GetClaimAsInt
        /// </summary>
        /// <param name="context"></param>
        /// <param name="name"></param>
        /// <param name="defValue"></param>
        /// <returns></returns>
        public static int GetClaimAsInt(HttpContextBase context, string name, int defValue = 0)
        {
            Dictionary<string, string> claims = GetUadmUserTokenClaims(context);
            if (claims == null)
                return defValue;

            return GetClaimAsInt(claims, name, defValue);
        }

        /// <summary>
        /// GetClaimAsInt
        /// </summary>
        /// <param name="claims"></param>
        /// <param name="name"></param>
        /// <param name="defValue"></param>
        /// <returns></returns>
        public static int GetClaimAsInt(this Dictionary<string, string> claims, string name, int defValue = 0)
        {
            string claim = GetClaim(claims, name);
            if (string.IsNullOrEmpty(claim))
                return defValue;

            int.TryParse(claim, out int value);               
            return value;
        }

        /// <summary>
        /// GetClaim
        /// </summary>
        /// <param name="context"></param>
        /// <param name="name"></param>
        /// <param name="defValue"></param>
        /// <returns></returns>
        public static long GetClaimAsLong(HttpContextBase context, string name, long defValue = 0)
        {
            Dictionary<string, string> claims = GetUadmUserTokenClaims(context);
            if (claims == null)
                return defValue;

            return GetClaimAsLong(claims, name, defValue);
        }

        /// <summary>
        /// GetClaimAsLong
        /// </summary>
        /// <param name="claims"></param>
        /// <param name="name"></param>
        /// <param name="defValue"></param>
        /// <returns></returns>
        public static long GetClaimAsLong(this Dictionary<string, string> claims, string name, long defValue = 0)
        {
            string claim = GetClaim(claims, name);
            if (string.IsNullOrEmpty(claim))
                return defValue;

            long.TryParse(claim, out long value);
            return value;
        }

        /// <summary>
        /// GetClaimAsBool
        /// </summary>
        /// <param name="context"></param>
        /// <param name="name"></param>
        /// <param name="defValue"></param>
        /// <returns></returns>
        public static bool GetClaimAsBool(HttpContextBase context, string name, bool defValue = false)
        {
            Dictionary<string, string> claims = GetUadmUserTokenClaims(context);
            if (claims == null)
                return defValue;

            return GetClaimAsBool(claims, name, defValue);
        }

        /// <summary>
        /// GetClaimAsBool
        /// </summary>
        /// <param name="claims"></param>
        /// <param name="name"></param>
        /// <param name="defValue"></param>
        /// <returns></returns>
        public static bool GetClaimAsBool(this Dictionary<string, string> claims, string name, bool defValue = false)
        {
            string claim = GetClaim(claims, name);
            if (string.IsNullOrEmpty(claim))
                return defValue;

            bool.TryParse(claim, out bool value);
            return value;
        }

        /// <summary>
        /// GetWindowsUser
        /// </summary>
        /// <param name="context"></param>
        /// <returns></returns>
        public static string GetWindowsUserId(HttpContextBase context)
        {
            if (context == null)
                return null;

            if (context.User == null && context.Request == null)    
                return null;

            // Get the Windows user name
            string windowsUser = context.User?.Identity?.Name ?? WindowsIdentity.GetCurrent().Name;

            if (string.IsNullOrEmpty(windowsUser))
            {
                windowsUser = context.Request.LogonUserIdentity.Name;
            }

            return windowsUser;
        }

        /// <summary>
        /// CheckUserTokenClaimsForUsernameAndApp
        /// </summary>
        /// <param name="claims"></param>
        /// <param name="functionName"></param>
        /// <returns></returns>
        public static bool CheckUserTokenClaimsForUsernameAndApp(Dictionary<string, string> claims, string functionName)
        {
            string applicationKey = claims.GetClaim(CLAIM_APPLICATION);
            string userName = claims.GetClaim(CLAIM_USERNAME);

            if (!string.IsNullOrEmpty(applicationKey) && !string.IsNullOrEmpty(userName))
            {
                UadmLogger.LogDebug($"CheckToken for {functionName} called for user '{userName}' and application '{applicationKey}'");
                return true;
            }

            UadmLogger.LogWarning($"CheckToken for {functionName} called for user '{userName}' and application '{applicationKey}' not valid");

            return false;
        }

        /// <summary>
        /// GetUadmLibEnvironment
        /// </summary>
        /// <param name="claims"></param>
        /// <returns></returns>
        public static UadmLibEnvironment GetUadmLibEnvironment(this Dictionary<string, string> claims)
        {
            // get UadmEnvironment from settings
            UadmLibEnvironment uadmLibEnvironment = new UadmLibEnvironment
            {
                Environment = claims.GetClaim(CLAIM_ENVIRONMENT),
                Application = claims.GetClaim(CLAIM_APPLICATION)
            };

            return uadmLibEnvironment;
        }

        /// <summary>
        /// GetOSAccount
        /// </summary>
        /// <returns></returns>
        public static string GetOSAccount(HttpContextBase context, string defValue = "JohnDoe")
        {
            string osAccount = GetWindowsUserId (context);
            if (string.IsNullOrEmpty(osAccount))
                return defValue;

            if (osAccount.Contains("@"))
                osAccount = osAccount.Split('@')[0];

            if (osAccount.Contains("\\"))
                osAccount = osAccount.Split('\\')[1];

            osAccount = osAccount.Replace("\\", "_").Replace("/", "_");

            return osAccount.Trim().ToLower();
        }
    }
}
