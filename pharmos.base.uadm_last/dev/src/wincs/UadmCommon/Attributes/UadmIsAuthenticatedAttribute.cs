using System;
using System.Collections.Generic;
using System.Web.Mvc;
using UadmCommon.Classes;

namespace UadmCommon.Attributes
{
    public class UadmIsAuthenticatedAttribute : ActionFilterAttribute
    {
        /// <summary>
        /// OnActionExecuting
        /// </summary>
        /// <param name="filterContext"></param>
        public override void OnActionExecuting(ActionExecutingContext filterContext)
        {
            string applicationKey = WebConfig.GetUadmApplicationKey();
            string loginUrl = WebConfig.GetUadmLoginWebServer();
            string originUrlOrId = WebConfig.GetUadmOriginUrlOrId(filterContext.RequestContext.HttpContext.Request.Url.ToString());

            string newUadmUserToken = UadmUserToken.UpdateUadmUserToken(filterContext.HttpContext);

            if (string.IsNullOrEmpty(newUadmUserToken))
            {
                UadmLogger.LogDebug($@"UadmIsAuthenticatedAttribute: User '{UadmUserToken.GetOSAccount(filterContext.HttpContext)}' is not authenticated, redirecting to login page.");

                // User is not authenticated, redirect to login page
                if (!string.IsNullOrEmpty(originUrlOrId))
                {                    
                    // Add custom headers
                    filterContext.Result = new UadmRedirectResult(
                        loginUrl + "/login",
                        UadmUserToken.GetOSAccount(filterContext.HttpContext),
                        new Dictionary<string, string>
                        {
                            { UadmRequestValues.REQUESTVALUES_PHX_UADM_ORIGINURL, originUrlOrId },
                            { UadmRequestValues.REQUESTVALUES_PHX_UADM_USERNAME, UadmUserToken.GetOSAccount(filterContext.HttpContext) },
                        },
                        addQueryString: true
                    );
                }
                else if (!string.IsNullOrEmpty(applicationKey))
                {
                    // Redirect to login with application key
                    filterContext.Result = new RedirectResult(loginUrl + "/login?app=" + applicationKey);
                }
                else
                {
                    // Redirect to login without parameters
                    filterContext.Result = new RedirectResult(loginUrl + "/login");
                }

                return;
            }

            // User is authenticated, proceed with the action
            Dictionary<string, string> claims = UadmUserToken.GetUadmUserTokenClaims(filterContext.HttpContext, newUadmUserToken, false, out _);

            UadmLogger.LogDebug($@"UadmIsAuthenticatedAttribute: User '{UadmUserToken.GetOSAccount(filterContext.HttpContext)}' is authenticated, updating user token.");

            int delay = int.Parse(claims[UadmUserToken.CLAIM_EXPIRES]);

            if (delay <= 0)
                delay = WebConfig.GetDefaultBrowserCookieExpiration();

            UadmCookie.CreateUadmUserCookie(filterContext.HttpContext, applicationKey, newUadmUserToken, DateTime.Now.AddMinutes(delay));

            base.OnActionExecuting(filterContext);
        }
    }
}
