using System.Configuration;
using UadmCommon.Classes;

namespace UadmCommon
{
    public class WebConfig
    {
        public const string UADM_LOGINWEBSERVER = "UadmLoginWebServer";
        public const string UADM_ORIGINURL = "UadmOriginUrl";
        public const string UADM_APPLICATIONID = "UadmApplicationId";
        public const string UADM_CLIENTID = "UadmClientId";
        public const string UADM_APPLICATIONKEY = "UadmApplicationKey";
        public const string UADM_USEWEBREQUESTS = "UadmUseWebRequests";
        public const string UADM_USEVOODOO = "UadmUseVoodoo";
        public const string UADM_USEDETAILEDMESSAGE = "UadmUseDetailedMessage";
        public const string UADM_USEASYNCREQUESTS = "UadmUseAsyncRequests";
        public const string UADM_SHOWPASSWORDCHANGEREQUIRED = "ShowPasswordChangeRequired";
        public const string UADM_SHOWPASSWORDCHANGEBUTTON = "ShowPasswordChangeButton";
        public const string UADM_IISAPPPATH = "UadmIISAppPath";
        public const string UADM_LOG4NETCONFIG = "UadmLog4NetConfig";
        public const string UADM_EXCLUDEDOSACCOUNTS = "UadmExcludedOsAccounts";
        public const string UADM_ALLOWEMPTYOSACCOUNT = "UadmAllowEmptyOsAccount";
        public const string UADM_DEFAULTBROWSERCOOKIEEXPIRATION = "DefaultBrowserCookieExpiration";
        public const string UADM_WRAPPERLOCKEXPIRATION = "UadmWrapperLockExpiration";
        public const string UADM_WRAPPERUSELOCK = "UadmWrapperUseLock";
        public const string UADM_WRAPPERUSEUSERBASEDLOCK = "UadmWrapperUseUserBasedLock";

        /// <summary>
        /// GetUadmIISAppPath
        /// </summary>
        /// <param name="defValue"></param>
        /// <returns></returns>
        public static string GetUadmIISAppPath(string defValue = "/")
        {
            string iisAppPath = ConfigurationManager.AppSettings[UADM_IISAPPPATH] ?? "";

            if (iisAppPath == null)
            {
                iisAppPath = defValue;
            }
            else if (!iisAppPath.StartsWith("/"))
            {
                iisAppPath = "/" + iisAppPath;
            }

            return iisAppPath;
        }

        /// <summary>
        /// GetUadmLoginWebServer
        /// </summary>
        /// <returns></returns>
        public static string GetUadmLoginWebServer(string defValue = "")
        {
            return GetWebConfigValue(UADM_LOGINWEBSERVER, defValue);
        }

        /// <summary>
        /// GetUadmClientId
        /// </summary>
        /// <returns></returns>
        public static string GetUadmClientId(string defValue = "")
        {
            return GetWebConfigValue(UADM_CLIENTID, defValue);
        }

        /// <summary>
        /// GetUadmApplicationId
        /// </summary>
        /// <returns></returns>
        public static string GetUadmApplicationId(string defValue = "")
        {
            return GetWebConfigValue(UADM_APPLICATIONID, defValue);
        }

        /// <summary>
        /// GetUadmOriginUrl
        /// </summary>
        /// <returns></returns>
        public static string GetUadmOriginUrlOrId(string defValue = "")
        {
            string clientId = GetUadmClientId();
            string originUrl = GetWebConfigValue(UADM_ORIGINURL, defValue);

            if (string.IsNullOrEmpty(originUrl))
                originUrl = defValue;

            // User is not authenticated, redirect to login page
            if ((!string.IsNullOrEmpty(originUrl) && UadmValidation.IsValidUrl(originUrl)) || !string.IsNullOrEmpty(clientId))
            {
                // Use clientId as originUrl if provided
                if (!string.IsNullOrEmpty(clientId))
                {
                    originUrl = clientId;
                }
            }

            return originUrl?.Split('?')[0];
        }

        /// <summary>
        /// GetUadmApplicationKey
        /// </summary>
        /// <returns></returns>
        public static string GetUadmApplicationKey(string defValue = "")
        {
            return GetWebConfigValue(UADM_APPLICATIONKEY, defValue);
        }

        /// <summary>
        /// GetWebConfigValue
        /// </summary>
        /// <returns></returns>
        public static string GetWebConfigValue(string key, string defValue = "")
        {
            return ConfigurationManager.AppSettings[key] ?? defValue;
        }

        /// <summary>
        /// GetUadmUseWebRequests
        /// </summary>
        /// <returns></returns>
        public static bool GetUadmUseWebRequests(bool defValue = false)
        {
            string value = GetWebConfigValue(UADM_USEWEBREQUESTS, defValue.ToString());

            return value.ToLower() == "true";
        }

        /// <summary>
        /// GetUadmUseVoodoo
        /// </summary>
        /// <returns></returns>
        public static bool GetUadmUseVoodoo(bool defValue = true)
        {
            string value = GetWebConfigValue(UADM_USEVOODOO, defValue.ToString());
            return value.ToLower() == "true";
        }

        /// <summary>
        /// GetUseDetailedMessage
        /// </summary>
        /// <returns></returns>
        public static bool GetUseDetailedMessage(bool defValue = false)
        {
            string value = GetWebConfigValue(UADM_USEDETAILEDMESSAGE, defValue.ToString());
            return value.ToLower() == "true";
        }

        /// <summary>
        /// GetUseAsyncTimeOut
        /// </summary>
        /// <returns></returns>
        public static bool GetUseAsyncTimeOut(bool defValue = false)
        {
            string value = GetWebConfigValue(UADM_USEASYNCREQUESTS, defValue.ToString());

            return value.ToLower() == "true";
        }

        /// <summary>
        /// GetShowPasswordChangeRequired
        /// </summary>
        /// <returns></returns>
        public static bool GetShowPasswordChangeRequired(bool defValue = false)
        {
            string value = GetWebConfigValue(UADM_SHOWPASSWORDCHANGEREQUIRED, defValue.ToString());

            return value.ToLower() == "true";
        }

        /// <summary>
        /// ShowPasswordChangeButton
        /// </summary>
        /// <returns></returns>
        public static bool GetShowPasswordChangeButton(bool defValue = false)
        {
            string value = GetWebConfigValue(UADM_SHOWPASSWORDCHANGEBUTTON, defValue.ToString());

            return value.ToLower() == "true";
        }

        /// <summary>
        /// GetUseLock
        /// </summary>
        /// <returns></returns>
        public static bool GetUseLock(bool defValue = false)
        {
            string value = GetWebConfigValue(UADM_WRAPPERUSELOCK, defValue.ToString());

            return value.ToLower() == "true";
        }


        /// <summary>
        /// GetUseUserBasedLock
        /// </summary>
        /// <returns></returns>
        public static bool GetUseUserBasedLock(bool defValue = false)
        {
            string value = GetWebConfigValue(UADM_WRAPPERUSEUSERBASEDLOCK, defValue.ToString());

            return value.ToLower() == "true";
        }

        /// <summary>
        /// GetDefaultBrowserCookieExpiration
        /// </summary>
        /// <returns></returns>
        public static int GetDefaultBrowserCookieExpiration(string defValue = "1440")
        {
            string value = GetWebConfigValue(UADM_DEFAULTBROWSERCOOKIEEXPIRATION, defValue);

            return int.TryParse(value, out int expiration) ? expiration : 1440;
        }

        /// <summary>
        /// GetLockExpiration
        /// </summary>
        /// <returns></returns>
        public static int GetLockExpiration(string defValue = "10")
        {
            string value = GetWebConfigValue(UADM_WRAPPERLOCKEXPIRATION, defValue);

            return int.TryParse(value, out int expiration) ? expiration : 10;
        }

        /// <summary>
        /// GetLog4NetConfigFile
        /// </summary>
        /// <returns></returns>
        public static string GetLog4NetConfigFile(string defValue = "")
        {
            return ConfigurationManager.AppSettings[UADM_LOG4NETCONFIG] ?? defValue;
        }

        /// <summary>
        /// GetUadmExcludedOsAccounts
        /// </summary>
        /// <returns></returns>
        public static string GetUadmExcludedOsAccounts(string defValue = "")
        {
            return ConfigurationManager.AppSettings[UADM_EXCLUDEDOSACCOUNTS] ?? defValue;
        }

        /// <summary>
        /// GetUadmAllowEmptyOsAccount
        /// </summary>
        /// <param name="defValue"></param>
        /// <returns></returns>
        public static bool GetUadmAllowEmptyOsAccount(bool defValue = false)
        {
            string allowEmptyOsAccount = GetWebConfigValue(UADM_ALLOWEMPTYOSACCOUNT, defValue.ToString());
            return allowEmptyOsAccount.ToLower() == "true";
        }
    }
}
