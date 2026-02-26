using Newtonsoft.Json;
using System.Collections.Generic;
using System.Runtime.Remoting.Contexts;
using System.Web;
using UadmCommon.Classes;

namespace UadmCommon
{
    public class UadmRequestValues
    {
        // query string
        public const string REQUESTVALUES_PARAMETER = "rv";

        // headers
        public const string HEADER_AUTHORIZATION = "Authorization";
        public const string HEADER_UADMUSERTOKEN = "X-PHX-UADM-USERTOKEN";

        public const string REQUESTVALUES_PHX_UADM_ORIGINURL = "X-PHX-UADM-ORIGINURL";
        public const string REQUESTVALUES_PHX_UADM_USERTOKEN = "X-PHX-UADM-USERTOKEN";
        public const string REQUESTVALUES_PHX_UADM_LOGINREDIRECTURL = "X-PHX-UADM-LOGINREDIRECTURL";
        public const string REQUESTVALUES_PHX_UADM_APPLICATIONVIEW = "X-PHX-UADM-APPLICATIONVIEW";
        public const string REQUESTVALUES_PHX_UADM_APPLICATIONACTION = "X-PHX-UADM-APPLICATIONACTION";
        public const string REQUESTVALUES_PHX_UADM_APPLICATIONCONTROLLER = "X-PHX-UADM-APPLICATIONCONTROLLER";

        public const string REQUESTVALUES_PHX_UADM_USERNAME = "X-PHX-UADM-USERNAME";
        public const string REQUESTVALUES_PHX_UADM_APPLICATIONKEY = "X-PHX-UADM-APPLICATIONKEY";
        public const string REQUESTVALUES_PHX_UADM_ENVIRONMENTKEY = "X-PHX-UADM-ENVIRONMENTKEY";
        public const string REQUESTVALUES_PHX_UADM_COUNTRY = "X-PHX-UADM-COUNTRY";
        public const string REQUESTVALUES_PHX_UADM_LANGUAGE = "X-PHX-UADM-LANGUAGE";
        public const string REQUESTVALUES_PHX_UADM_BRANCHREGION = "X-PHX-UADM-BRANCHREGION";

        /// <summary>
        /// GetValueFromRequestValues
        /// </summary>
        /// <param name="httpContext"></param>
        /// <param name="headerName"></param>
        /// <param name="defValue"></param>
        /// <returns></returns>
        public static string GetValueFromHeader(HttpContextBase httpContext, string headerName, string defValue)
        {
            if (httpContext == null || httpContext.Request == null || httpContext.Request.Headers == null)
                return defValue;

            if (httpContext.Request.Headers[headerName] != null)
            {
                return httpContext.Request.Headers[headerName];
            }

            return defValue;
        }


        /// <summary>
        /// GetValueFromQueryString
        /// </summary>
        /// <param name="httpContext"></param>
        /// <param name="parameter"></param>
        /// <param name="defValue"></param>
        /// <returns></returns>
        public static string GetValueFromQueryString(HttpContextBase httpContext, string parameter, string defValue)
        {
            // try to get from query string

            if (httpContext == null || httpContext.Request == null || httpContext.Request.QueryString == null)
                return defValue;

            string queryStringValue = httpContext.Request.QueryString[REQUESTVALUES_PARAMETER];

            if (string.IsNullOrEmpty(queryStringValue))
                return defValue;    
            
            Dictionary<string, string> requestValues = GetRequestValuesFromQueryString(queryStringValue);
            if (requestValues.ContainsKey(parameter))
            {
                return requestValues[parameter];
            }

            return defValue;
        }

        /// <summary>
        /// GetValueFromRequestValues
        /// </summary>
        /// <param name="requestValues"></param>
        /// <param name="headerName"></param>
        /// <param name="defValue"></param>
        /// <returns></returns>
        public static string GetValueFromRequestValues(Dictionary<string, string> requestValues, string headerName, string defValue)
        {
            if (requestValues.ContainsKey(headerName))
            {
                return requestValues[headerName];
            }

            return defValue;
        }

        /// <summary>
        /// GetRequestValuesFromQueryString
        /// </summary>
        /// <param name="rv"></param>
        /// <param name="useCrypt"></param>
        /// <returns></returns>
        public static Dictionary<string, string> GetRequestValuesFromQueryString(string rv, bool useCrypt = true)
        {
            Dictionary<string, string> requestValues = new Dictionary<string, string>();

            if (!string.IsNullOrEmpty(rv))
            {
                rv = GetUrlDecodedString(rv);

                if (useCrypt)
                {
                    try
                    {
                        string decryptedValue = Crypt.DecryptFromBase64(rv);

                        requestValues = JsonConvert.DeserializeObject<Dictionary<string, string>>(TrimRequestValuesString(decryptedValue));
                    }
                    catch
                    {
                        try
                        {
                            requestValues = JsonConvert.DeserializeObject<Dictionary<string, string>>(TrimRequestValuesString(rv));
                        }
                        catch
                        {
                            UadmLogger.LogError("GetRequestValuesFromQueryString: Unable to decrypt or parse request values.");
                        }
                    }
                }
                else
                {
                    try
                    {
                        requestValues = JsonConvert.DeserializeObject<Dictionary<string, string>>(TrimRequestValuesString(rv));
                    }
                    catch
                    {
                        UadmLogger.LogError("GetRequestValuesFromQueryString: Unable to parse request values.");
                    }
                }            
            }

            return requestValues;
        }

        /// <summary>
        /// GetUrlDecodedString
        /// </summary>
        /// <param name="text"></param>
        /// <returns></returns>
        private static string GetUrlDecodedString(string text)
        {   
            if (text.StartsWith("\""))
                text = text.Trim('\"');

            if (text.Contains("%2b"))
                text = text.Replace("%2b", "+");

            if (text.Contains("%2d"))
                text = text.Replace("%2d", "-");

            if (text.Contains("%20"))
                text = text.Replace("%2f", "/");

            if (text.Contains("%20"))
                text = text.Replace("%20", " ");

            return text;
        }

        /// <summary>
        /// TrimRequestValuesString
        /// </summary>
        /// <param name="text"></param>
        /// <returns></returns>
        private static string TrimRequestValuesString(string text)
        {
            if (text.StartsWith("\""))
                text = text.Trim('\"');

            text = text.Replace("\\\"", "\"");

            return text;
        }   
    }
}
