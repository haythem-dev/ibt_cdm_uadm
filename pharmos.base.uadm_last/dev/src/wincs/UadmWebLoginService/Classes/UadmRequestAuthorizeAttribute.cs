using System.Web.Mvc;
using UadmCommon.Classes;

namespace UadmWebLoginService.Classes
{
    public class UadmRequestAuthorizeAttribute : ActionFilterAttribute
    {
        public override void OnActionExecuting(ActionExecutingContext filterContext)
        {
            UadmLogger.LogDebug("UadmRequestAuthorizeAttribute: " + filterContext.HttpContext.Request.Url.ToString());

            if (!UadmUserToken.IsUserAuthenticated(filterContext.HttpContext))
            {
                filterContext.Result = new HttpStatusCodeResult(401, "Not Authorized");

                UadmLogger.LogWarning("UadmRequestAuthorizeAttribute: Unauthorized access attempt to " + filterContext.HttpContext.Request.Url.ToString());
                return;
            }

            base.OnActionExecuting(filterContext);
        }
    }
}
