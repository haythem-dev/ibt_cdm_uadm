using System.Collections.Generic;
using System.Threading.Tasks;
using System.Web;
using System.Web.Mvc;
using UadmCommon.Attributes;
using UadmCommon.Classes;
using UadmCommon.Models;

namespace UadmTestWebApplication.Controllers
{
    [UadmIsAuthenticated]
    public class HomeController : Controller
    {
        private LibAppUadmService _libAppUadmService = new LibAppUadmService(new HttpContextWrapper(System.Web.HttpContext.Current));

        /// <summary>
        /// Index
        /// </summary>
        /// <returns></returns>
        public async Task<ActionResult> Index(string rv = null)
        {
            Dictionary<string, string> claims = UadmUserToken.GetUadmUserTokenClaims(HttpContext);

            //if (!string.IsNullOrEmpty(rv))
            //{
            //    return RedirectToAction("Index", "Home");
            //}

            // Get the Windows user name
            string windowsUser = UadmUserToken.GetWindowsUserId (HttpContext);

            // Call through Web API
            _libAppUadmService.UseWebRequest = true;

            List<UadmLibPermission> result = await _libAppUadmService.GetUadmAllUserPermissionsAsync(HttpContext, false);

            //UadmLibPermission result2 = await _libAppUadmService.GetUadmUserPermissionInfoAsync(HttpContext, "UW_CHANGE_ACCESS");
            //bool result3 = await _libAppUadmService.VerifyUserPermissionAsync(HttpContext, "UW_CHANGE_ACCESS");
            //List<UadmLibParameter> result4 = await _libAppUadmService.GetUadmAppParametersInfoAsync(HttpContext, "22", null, "BASAR_LOCALE");
            //List<UadmLibParameter> result5 = await _libAppUadmService.GetUadmAllAppParametersAsync(HttpContext);
            //List<UadmLibParameter> result6 = await _libAppUadmService.GetUadmAppParametersInfoAsync(HttpContext, "5", null, null);
            //UadmLibPermission result7 = await _libAppUadmService.GetUadmUserPermissionInfoAsync(HttpContext, "UW_CHANGE_ACCESS");
            //bool result8 = await _libAppUadmService.VerifyUserPermissionAsync(HttpContext, "UW_CHANGE_ACCESS");
            //List<UadmLibPermission> result9 = await _libAppUadmService.GetUadmAllUserPermissionsAsync(HttpContext);

            ViewBag.WindowsUser = windowsUser + " Permissions: " + (result == null ? 0 : result?.Count).ToString();

            if (claims != null && claims.ContainsKey(UadmUserToken.CLAIM_EMAIL))
                ViewBag.LoggedInUser = claims[UadmUserToken.CLAIM_EMAIL];

            return View();
        }

        /// <summary>
        /// About
        /// </summary>
        /// <returns></returns>
        public ActionResult About()
        {
            return View();
        }

        /// <summary>
        /// Logout
        /// </summary>
        /// <returns></returns>
        public ActionResult Logout()
        {
            _libAppUadmService.Logout(HttpContext); 

            return View();
        }


        /// <summary>
        /// Contact
        /// </summary>
        /// <returns></returns>
        public ActionResult Contact()
        {
            ViewBag.Message = "Your contact page.";

            return View();
        }
    }
}