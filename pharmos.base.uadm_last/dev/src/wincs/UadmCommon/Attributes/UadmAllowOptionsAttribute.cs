using System.Web;
using System.Web.Mvc;
using UadmCommon.Classes;

namespace UadmCommon.Attributes
{
    public class UadmAllowOptionsAttribute : AuthorizeAttribute
    {
        /// <summary>
        /// OnAuthorization
        /// </summary>
        /// <param name="filterContext"></param>
        public override void OnAuthorization(AuthorizationContext filterContext)
        {
            // Allow OPTIONS requests to bypass authorization (for CORS preflight)
            if (filterContext.HttpContext.Request.HttpMethod == "OPTIONS")
            {
                filterContext.Result = new HttpStatusCodeResult(200); // Return HTTP 200 OK
                return;
            }

            // Custom authorization logic
            if (!IsUserAuthorized(filterContext.HttpContext))
            {
                // If unauthorized, set the result to HTTP 401 Unauthorized
                filterContext.Result = new HttpUnauthorizedResult();
            }
        }

        /// <summary>
        /// IsUserAuthorized
        /// </summary>
        /// <param name="httpContext"></param>
        /// <returns></returns>
        private bool IsUserAuthorized(HttpContextBase httpContext)
        {
            // Example: Check if a specific cookie exists
            string userToken = UadmUserToken.GetUserToken(httpContext, true, out _); 

            return !string.IsNullOrEmpty(userToken);
        }
    }
}