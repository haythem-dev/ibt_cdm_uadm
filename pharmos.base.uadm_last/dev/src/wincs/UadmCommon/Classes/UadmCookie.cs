using System;
using System.Web;

namespace UadmCommon.Classes
{
    public class UadmCookie
    {
        public static readonly string COOKIE_X_PHX_UADM_USERTOKEN = "X-PHX-UADMUSERTOKEN";
        public static readonly string COOKIE_X_PHX_UADM_APPLICATIONKEY = "X-PHX-UADM-APPLICATIONKEY";

        /// <summary>
        /// GetApplicationUserTokenname
        /// </summary>
        /// <param name="appName"></param>
        /// <returns></returns>
        public static string GetApplicationUserTokenname(string appName)
        {
            if (!string.IsNullOrEmpty(appName))
            {
                return COOKIE_X_PHX_UADM_USERTOKEN + "-" + appName.Trim().Replace(" ", "-").Replace("_", "-").Replace("+", "-").ToUpper();
            }

            return COOKIE_X_PHX_UADM_USERTOKEN;
        }           

        /// <summary>
        /// GetFromHttpContextRequestCookie
        /// </summary>
        /// <param name="context"></param>
        /// <param name="cookieName"></param>
        /// <returns></returns>
        public static string GetFromHttpContextRequestCookie(HttpContextBase context, string cookieName)
        {
            if (context == null || string.IsNullOrEmpty(cookieName))
                return null;

            if (context.Request == null || context.Request.Cookies == null)
                return null;

            // Read the "CTM" cookie from the request
            HttpCookie ctmCookie = context.Request.Cookies[cookieName];
            if (ctmCookie == null)
                return null;

            string ctmValue = ctmCookie.Value;

            if (string.IsNullOrEmpty(ctmValue))
                return null;

            if (ctmCookie.Expires > DateTime.Now)
                return null;

            return ctmValue;
        }

        /// <summary>
        /// CreateUadmUserCookie
        /// </summary>
        /// <param name="httpContext"></param>
        /// <param name="appName"></param>
        /// <param name="value"></param>
        /// <param name="expires"></param>
        /// <returns></returns>
        public static HttpCookie CreateUadmUserCookie(HttpContextBase httpContext, string appName, string value, DateTime? expires = null)
        {
            HttpCookie cookie = CreateCookie(GetApplicationUserTokenname(appName), value, expires);

            httpContext.Response.Cookies.Add(cookie);

            return cookie;
        }

        /// <summary>
        /// CreateCookie
        /// </summary>
        /// <param name="name"></param>
        /// <param name="value"></param>
        /// <param name="expires"></param>
        /// <returns></returns>
        public static HttpCookie CreateCookie(string name, string value, DateTime? expires = null)
        {
            HttpCookie cookie = new HttpCookie(name, value);

            if (expires.HasValue)
            {
                cookie.Expires = expires.Value;
            }

            cookie.Path = WebConfig.GetUadmIISAppPath("/"); // Optional: set path

            return cookie;
        }
    }
}
