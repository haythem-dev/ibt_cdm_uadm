using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Net;
using System.Net.Http;
using System.Threading.Tasks;
using System.Web;
using UadmCommon.Classes;
using UadmCommon.Extensions;
using UadmCommon.Models;

namespace UadmCommon.HttpClients
{
    public class UadmLoginServiceHttpClient
    {
        private readonly string jsonMediaType = "application/json";

        public Dictionary<string, string> ExcludeHeaderList { get; set; } = new Dictionary<string, string>()
        {
            { "content-type", "" },
            { "content-length", "" },
            { "Host", "" },
            { "Accept", "" },
            { "Accept-Encoding", "" },
            { "Accept-Language", "" }
        };

        /// <summary>
        /// GetUadmAllUserPermissionsAsync
        /// </summary>
        /// <param name="httpContext"></param>
        /// <returns></returns>
        public async Task<List<UadmLibPermission>> GetUadmAllUserPermissionsAsync(HttpContextBase httpContext)
        {
            string apiUrl = WebConfig.GetUadmLoginWebServer();
            string applicationKey = WebConfig.GetUadmApplicationKey();

            apiUrl += "/api/permissions/all";

            string jsonResult = string.Empty;

            using (HttpClient client = new HttpClient(CreateClientHandler(httpContext, apiUrl)))
            {
                try
                {
                    HttpResponseMessage response = await UadmSendAsync(client, CreateHttpRequestMessage(client, httpContext, apiUrl));

                    jsonResult = GetJsonResult(response, apiUrl);
                }
                catch (Exception ex)
                {
                    UadmLogger.LogError("GetUadmAllUserPermissionsAsync:" + ex.Message);
                    return null;
                }
            }

            if (CheckJsonResult(jsonResult) == false)
            {
                return null;
            }

            return JsonConvert.DeserializeObject<List<UadmLibPermission>>(jsonResult);
        }

        /// <summary>
        /// GetUadmAllUserPermissionsByBranchAsync
        /// </summary>
        /// <param name="httpContext"></param>
        /// <returns></returns>
        public async Task<List<UadmLibPermission>> GetUadmAllUserPermissionsByBranchAsync(HttpContextBase httpContext)
        {
            string apiUrl = WebConfig.GetUadmLoginWebServer();
            string applicationKey = WebConfig.GetUadmApplicationKey();

            apiUrl += "/api/permissions/branch";

            string jsonResult = string.Empty;

            using (HttpClient client = new HttpClient(CreateClientHandler(httpContext, apiUrl)))
            {
                try
                {
                    HttpResponseMessage response = await UadmSendAsync(client, CreateHttpRequestMessage(client, httpContext, apiUrl));

                    jsonResult = GetJsonResult(response, apiUrl);
                }
                catch (Exception ex)
                {
                    UadmLogger.LogError("GetUadmAllUserPermissionsAsync:" + ex.Message);
                    return null;
                }
            }

            if (CheckJsonResult(jsonResult) == false)
            {
                return null;
            }

            return JsonConvert.DeserializeObject<List<UadmLibPermission>>(jsonResult);
        }


        /// <summary>
        /// GetUadmUserPermissionInfoAsync
        /// </summary>
        /// <param name="httpContext"></param>
        /// <param name="permissionRight"></param>
        /// <returns></returns>
        public async Task<UadmLibPermission> GetUadmUserPermissionInfoAsync(HttpContextBase httpContext, string permissionRight)
        {
            string apiUrl = WebConfig.GetUadmLoginWebServer();
            string applicationKey = WebConfig.GetUadmApplicationKey();

            apiUrl += @"/api/permissions/permissioninfo?permissionRight=" + permissionRight;

            string jsonResult = string.Empty;

            using (HttpClient client = new HttpClient(CreateClientHandler(httpContext, apiUrl)))
            {
                HttpResponseMessage response = await UadmSendAsync(client, CreateHttpRequestMessage(client, httpContext, apiUrl));

                jsonResult = GetJsonResult(response, apiUrl);
            }

            if (CheckJsonResult(jsonResult) == false)
            {
                return null;
            }

            return JsonConvert.DeserializeObject<UadmLibPermission>(jsonResult);
        }

        /// <summary>
        /// VerifyUserPermissionAsync
        /// </summary>
        /// <param name="httpContext"></param>
        /// <param name="permissionRight"></param>
        /// <returns></returns>
        public async Task<bool> VerifyUserPermissionAsync(HttpContextBase httpContext, string permissionRight)
        {
            string apiUrl = WebConfig.GetUadmLoginWebServer();
            string applicationKey = WebConfig.GetUadmApplicationKey();

            apiUrl += "/api/permissions/verifypermission?permissionRight=" + permissionRight;

            string jsonResult = string.Empty;

            using (HttpClient client = new HttpClient(CreateClientHandler(httpContext, apiUrl)))
            {
                HttpResponseMessage response = await UadmSendAsync(client, CreateHttpRequestMessage(client, httpContext, apiUrl));

                jsonResult = GetJsonResult(response, apiUrl);
            }

            if (CheckJsonResult(jsonResult) == false)
            {
                return false;
            }

            return JsonConvert.DeserializeObject<bool>(jsonResult);
        }

        /// <summary>
        /// GetUadmAllAppParametersAsync
        /// </summary>
        /// <param name="httpContext"></param>
        /// <returns></returns>
        public async Task<List<UadmLibParameter>> GetUadmAllAppParametersAsync(HttpContextBase httpContext)
        {
            string apiUrl = WebConfig.GetUadmLoginWebServer();
            string applicationKey = WebConfig.GetUadmApplicationKey();

            apiUrl += "/api/parameters/all";

            string jsonResult = string.Empty;

            using (HttpClient client = new HttpClient(CreateClientHandler(httpContext, apiUrl)))
            {
                HttpResponseMessage response = await UadmSendAsync(client, CreateHttpRequestMessage(client, httpContext, apiUrl));

                jsonResult = GetJsonResult(response, apiUrl);
            }

            if (CheckJsonResult(jsonResult) == false)
            {
                return null;
            }

            return JsonConvert.DeserializeObject<List<UadmLibParameter>>(jsonResult);
        }

        /// <summary>
        /// GetUadmAppParametersInfoAsync
        /// </summary>
        /// <param name="httpContext"></param>
        /// <param name="para1"></param>
        /// <param name="para2"></param>
        /// <param name="parameterName"></param>
        /// <returns></returns>
        public async Task<List<UadmLibParameter>> GetUadmAppParametersInfoAsync(HttpContextBase httpContext, string para1, string para2, string parameterName)
        {
            string apiUrl = WebConfig.GetUadmLoginWebServer();
            string applicationKey = WebConfig.GetUadmApplicationKey();

            apiUrl += "/api/parameters/parameterinfo";
            char delimiter = '?';

            if (!string.IsNullOrEmpty(para1))
            {
                apiUrl += delimiter + "para1=" + para1;
                delimiter = '&';
            }

            if (!string.IsNullOrEmpty(para2))
            {
                apiUrl += delimiter + "para2=" + para2;
                delimiter = '&';
            }

            if (!string.IsNullOrEmpty(parameterName))
            {
                apiUrl += delimiter + "parameterName=" + parameterName;
                delimiter = '&';
            }

            string jsonResult = string.Empty;

            HttpClientHandler handler = new HttpClientHandler
            {
                CookieContainer = new CookieContainer()
            };

            HttpCookie ctmCookie = httpContext.Request.Cookies[UadmCookie.COOKIE_X_PHX_UADM_USERTOKEN];
            if (ctmCookie == null)
                return null;

            string ctmValue = ctmCookie.Value;

            // Example: Add a cookie
            Uri uri = new Uri(apiUrl);
            handler.CookieContainer.Add(uri, new Cookie(UadmCookie.COOKIE_X_PHX_UADM_USERTOKEN, ctmValue));

            using (HttpClient client = new HttpClient(handler))
            {
                HttpResponseMessage response = await UadmSendAsync(client, CreateHttpRequestMessage(client, httpContext, apiUrl));

                jsonResult = GetJsonResult(response, apiUrl);
            }

            if (CheckJsonResult(jsonResult) == false)
            {
                return null;
            }

            return JsonConvert.DeserializeObject<List<UadmLibParameter>>(jsonResult);
        }

        /// <summary>
        /// AuthenticationValidAsync
        /// </summary>
        /// <param name="httpContext"></param>
        /// <param name="url"></param>
        /// <param name="osAccount"></param>
        /// <param name="values"></param>
        /// <returns></returns>
        public async Task<bool> AuthenticationValidAsync(HttpContextBase httpContext, string url, string osAccount, Dictionary<string, string> values)
        {
            string apiUrl = url ?? "";
            string applicationKey = WebConfig.GetUadmApplicationKey();

            string queryString = "?" + UadmRequestValues.REQUESTVALUES_PARAMETER + "=" + HttpUtility.UrlEncode(Crypt.EncryptToBase64(osAccount, JsonConvert.SerializeObject(values)));

            apiUrl += queryString;

            string jsonResult = string.Empty;

            HttpClientHandler handler = new HttpClientHandler
            {
            };

            using (HttpClient client = new HttpClient(handler))
            {
                HttpResponseMessage response = await UadmSendAsync(client, CreateHttpRequestMessage(client, httpContext, apiUrl));

                jsonResult = GetJsonResult(response, apiUrl);
            }

            return CheckJsonResult(jsonResult);
        }

        /// <summary>
        /// CheckJsonResult
        /// </summary>
        /// <param name="jsonResult"></param>
        /// <returns></returns>
        private bool CheckJsonResult(string jsonResult)
        {
            if (string.IsNullOrEmpty(jsonResult))
            {
                return false;
            }
            if (jsonResult == "{}")
            {
                return false;
            }
            return true;
        }

        /// <summary>
        /// CreateClientHandler
        /// </summary>
        /// <param name="httpContext"></param>
        /// <param name="apiUrl"></param>
        /// <returns></returns>
        private HttpClientHandler CreateClientHandler(HttpContextBase httpContext, string apiUrl)
        {
            HttpClientHandler handler = new HttpClientHandler
            {
                //CookieContainer = new CookieContainer(),
                ServerCertificateCustomValidationCallback = (sender, cert, chain, sslPolicyErrors) => true,
                UseDefaultCredentials = true
            };

            // TODO: RBV : remove comments after testing
            //string userToken = UadmUserToken.GetUserToken(httpContext, out bool cookieUsed);
            //if (!string.IsNullOrEmpty(userToken))
            //{
            //    // Example: Add a cookie
            //    Uri uri = new Uri(apiUrl);
            //    handler.CookieContainer.Add(uri, new Cookie(UadmCookie.COOKIE_X_PHX_UADM_USERTOKEN, userToken));
            //    handler.UseCookies = true;
            //}
            //else
            //{
            //    handler.UseCookies = false;
            //}

            return handler;
        }

        /// <summary>
        /// GetJsonResult
        /// </summary>
        /// <param name="response"></param>
        /// <param name="apiUrl"></param>
        /// <returns></returns>
        private string GetJsonResult(HttpResponseMessage response, string apiUrl)
        {
            if (response.IsSuccessStatusCode)
            {
                return response.Content.ReadAsStringAsync().Result;
            }
            else
            {
                UadmLogger.LogError("GetJsonResult: Response status code: " + response.StatusCode.ToString() + " API URL: " + apiUrl);

                return string.Empty;
            }
        }

        /// <summary>
        /// CreateHttpRequestMessage
        /// </summary>
        /// <param name="client"></param>
        /// <param name="httpContext"></param>
        /// <param name="apiUrl"></param>
        private HttpRequestMessage CreateHttpRequestMessage(HttpClient client, HttpContextBase httpContext, string apiUrl)
        {
            HttpRequestMessage httpRequestMessage = new HttpRequestMessage(HttpMethod.Get, apiUrl);

            httpRequestMessage.Headers.Accept.Clear();
            httpRequestMessage.Headers.Accept.Add(new System.Net.Http.Headers.MediaTypeWithQualityHeaderValue(jsonMediaType));

            string headerValue;
            foreach (string headerKey in httpContext.Request.Headers.AllKeys)
            {
                if (ExcludeHeaderList.ContainsKey(headerKey))
                {
                    continue;
                }
                headerValue = httpContext.Request.Headers[headerKey];
                if (string.IsNullOrEmpty(headerValue))
                {
                    continue;
                }
                if (httpRequestMessage.Headers.TryAddWithoutValidation(headerKey, headerValue) == false)
                {
                    UadmLogger.LogError("CreateHttpRequestMessage: Failed to add header " + headerKey + " for url: " + apiUrl);
                }
            }             

            headerValue = httpContext.Request.Headers[UadmRequestValues.HEADER_AUTHORIZATION];
            if (string.IsNullOrEmpty(headerValue))
            {
                if (httpRequestMessage.Headers.TryAddWithoutValidation(UadmRequestValues.HEADER_AUTHORIZATION, UadmUserToken.GetUadmAuthorizationToken(httpContext)) == false)
                {
                    UadmLogger.LogError("CreateHttpRequestMessage: Failed to add header " + UadmRequestValues.HEADER_AUTHORIZATION + " for url: " + apiUrl);
                }
            }

            if (httpRequestMessage.Headers.TryAddWithoutValidation(UadmRequestValues.HEADER_UADMUSERTOKEN, UadmUserToken.GetUadmAuthorizationToken(httpContext)) == false)
            {
                UadmLogger.LogError("CreateHttpRequestMessage: Failed to add header " + UadmRequestValues.HEADER_UADMUSERTOKEN + " for url: " + apiUrl);
            }

            return httpRequestMessage;
        }

        /// <summary>
        /// UadmSendAsync
        /// </summary>
        /// <param name="client"></param>
        /// <param name="requestMessage"></param>
        /// <returns></returns>
        private async Task<HttpResponseMessage> UadmSendAsync(HttpClient client, HttpRequestMessage requestMessage)
        {
            HttpResponseMessage response;

            bool useAsyncTimeOut = WebConfig.GetUseAsyncTimeOut();

            if (useAsyncTimeOut == false)
            {
                response = await client.SendAsync(requestMessage);
                //response = await client.SendAsync(requestMessage).WithTimeout(10000);
            }
            else
            {
                response = await UadmTaskExtensions.WithTimeoutAndRetry(
                    async () => { return await client.SendAsync(requestMessage); },
                    5000,
                    3
                ); // Timeout: 5000ms, Max Retries: 3
            }
            return response;
        }
    }
}