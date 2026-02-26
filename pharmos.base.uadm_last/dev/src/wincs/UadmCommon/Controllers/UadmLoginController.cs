using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Threading.Tasks;
using System.Web.Mvc;
using System.Web.Routing;
using UadmCommon.Classes;

//namespace UadmTestWebApplication.Controllers
namespace UadmCommon.Controllers
{
    public class UadmLoginController : Controller
    {
        /// <summary>
        /// Index
        /// </summary>
        /// <returns></returns>
        public ActionResult Index(string rv)
        {
            if (!string.IsNullOrEmpty(rv))
            {
                string applicationKey = WebConfig.GetUadmApplicationKey();

                Dictionary<string, string> requestValues = UadmRequestValues.GetRequestValuesFromQueryString(rv, true);

                string userToken = UadmRequestValues.GetValueFromRequestValues(requestValues, UadmRequestValues.REQUESTVALUES_PHX_UADM_USERTOKEN, "");

                UadmCookie.CreateUadmUserCookie(HttpContext, applicationKey, userToken, DateTime.Now.AddDays(1));

                // Redirect to the specified return URL with the Authorization header
                return new UadmRedirectResult(
                    UadmRequestValues.GetValueFromRequestValues(requestValues, UadmRequestValues.REQUESTVALUES_PHX_UADM_LOGINREDIRECTURL, ""),
                    UadmUserToken.GetOSAccount(HttpContext),
                    UadmRequestValues.HEADER_UADMUSERTOKEN,
                    userToken,
                    true,
                    true,
                    true
                );
            }

            return RedirectToAction("Index", "Home");
        }

        /// <summary>
        /// AuthenticationValid
        /// </summary>
        /// <param name="rv">request values</param>
        /// <returns></returns>
        [AllowAnonymous]
        [HttpGet]
        public ActionResult AuthenticationValid(string rv)
        {
            return Index(rv);   
        }

        /// <summary>
        /// AuthenticationValidWithApplicationViewRedirect
        /// </summary>
        /// <returns></returns>
        public ActionResult AuthenticationValidWithApplicationViewRedirect(string rv)
        {
            if (!string.IsNullOrEmpty(rv))
            {
                string applicationKey = WebConfig.GetUadmApplicationKey();

                Dictionary<string, string> requestValues = UadmRequestValues.GetRequestValuesFromQueryString(rv, true);

                string userToken = UadmRequestValues.GetValueFromRequestValues(requestValues, UadmRequestValues.REQUESTVALUES_PHX_UADM_USERTOKEN, "");

                UadmCookie.CreateUadmUserCookie(HttpContext, applicationKey, userToken, DateTime.Now.AddDays(1));

                // Get the view name
                string viewName = UadmRequestValues.GetValueFromRequestValues(requestValues, UadmRequestValues.REQUESTVALUES_PHX_UADM_APPLICATIONVIEW, "");
                if (!string.IsNullOrEmpty(viewName))
                {
                    // Return the specified view
                    ViewBag.RedirectUrl = UadmRequestValues.GetValueFromRequestValues(requestValues, UadmRequestValues.REQUESTVALUES_PHX_UADM_LOGINREDIRECTURL, "");
                    return View(viewName);
                }
            }

            return RedirectToAction("Index", "Home");
        }

        /// <summary>
        /// AuthenticationValidWithSendControllerAction
        /// </summary>
        /// <param name="rv">request values</param>
        /// <returns></returns>
        [AllowAnonymous]
        [HttpGet]
        public async Task<ActionResult> AuthenticationValidWithSendControllerAction(string rv)
        {
            // Do something with the body
            if (!string.IsNullOrEmpty(rv))
            {
                string applicationKey = WebConfig.GetUadmApplicationKey();

                Dictionary<string, string> requestValues = UadmRequestValues.GetRequestValuesFromQueryString(rv, true);

                string userToken = UadmRequestValues.GetValueFromRequestValues(requestValues, UadmRequestValues.REQUESTVALUES_PHX_UADM_USERTOKEN, "");

                UadmCookie.CreateUadmUserCookie(HttpContext, applicationKey, userToken, DateTime.Now.AddDays(1));

                string action = UadmRequestValues.GetValueFromRequestValues(requestValues, UadmRequestValues.REQUESTVALUES_PHX_UADM_APPLICATIONACTION, "");
                string controller = UadmRequestValues.GetValueFromRequestValues(requestValues, UadmRequestValues.REQUESTVALUES_PHX_UADM_APPLICATIONCONTROLLER, "");

                if (!string.IsNullOrEmpty(action) && !string.IsNullOrEmpty(controller))
                {
                    return await InvokeControllerAction(controller + "Controller", action);
                }

                if (!string.IsNullOrEmpty(controller))
                {
                    return await InvokeControllerAction(controller + "Controller", "Index");
                }

                if (!string.IsNullOrEmpty(action))
                {
                    return await InvokeControllerAction("HomeController", action);
                }
            }

            return new OkResult();
        }

        /// <summary>
        /// InvokeControllerAction
        /// </summary>
        /// <param name="controllerName"></param>
        /// <param name="actionName"></param>
        /// <returns></returns>
        /// <exception cref="InvalidOperationException"></exception>
        public async Task<ActionResult> InvokeControllerAction(string controllerName, string actionName)
        {
            // Find the controller type by scanning all loaded assemblies
            Type controllerType = AppDomain.CurrentDomain.GetAssemblies()
                .SelectMany(assembly => assembly.GetTypes())
                .FirstOrDefault(type => type.Name.Equals(controllerName, StringComparison.OrdinalIgnoreCase) && typeof(Controller).IsAssignableFrom(type));

            if (controllerType == null)
            {
                throw new InvalidOperationException($"Controller '{controllerName}' not found.");
            }

            // Create an instance of the controller
            Controller controllerInstance = Activator.CreateInstance(controllerType) as Controller;

            if (controllerInstance == null)
            {
                throw new InvalidOperationException($"Unable to create an instance of '{controllerName}'.");
            }

            // Set the ControllerContext with proper RouteData
            RouteData routeData = new RouteData();
            routeData.Values["controller"] = controllerName.Replace("Controller", ""); // Remove "Controller" suffix
            routeData.Values["action"] = actionName;

            // Set up RequestContext
            RequestContext requestContext = new RequestContext(this.HttpContext, routeData);

            // Set the ControllerContext
            controllerInstance.ControllerContext = new ControllerContext(requestContext, controllerInstance);

            // Find the action method
            MethodInfo method = controllerType.GetMethod(actionName);
            if (method == null)
            {
                throw new InvalidOperationException($"Action '{actionName}' not found in '{controllerName}'.");
            }

            // Check if the method is asynchronous
            bool isAsync = typeof(Task).IsAssignableFrom(method.ReturnType);

            if (isAsync)
            {
                // If the method returns Task<ActionResult>
                if (method.ReturnType.IsGenericType && method.ReturnType.GetGenericTypeDefinition() == typeof(Task<>))
                {
                    Task task = (Task)method.Invoke(controllerInstance, null);
                    await task; // Await the task to ensure it completes
                    return (ActionResult)((dynamic)task).Result; // Extract the result
                }
                else
                {
                    // If the method returns Task (non-generic)
                    await (Task)method.Invoke(controllerInstance, null);
                    return null; // No result to return
                }
            }
            else
            {
                // Invoke the synchronous method
                return method.Invoke(controllerInstance, null) as ActionResult;
            }
        }
    }
}
