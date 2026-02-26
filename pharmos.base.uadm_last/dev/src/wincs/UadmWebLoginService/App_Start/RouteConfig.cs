using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Mvc;
using System.Web.Routing;

namespace UadmWebLoginService
{
    public class RouteConfig
    {
        public static void RegisterRoutes(RouteCollection routes)
        {
            routes.IgnoreRoute("{resource}.axd/{*pathInfo}");

            // Add this route for api/permissions
            routes.MapRoute(
                name: "ApiPermissionsPermissionInfo",
                url: "api/permissions/permissioninfo/{permissionRight}",
                defaults: new { controller = "Permissions", action = "GetUadmUserPermissionInfo", permissionRight = UrlParameter.Optional }
            );

            // Add this route for api/permissions
            routes.MapRoute(
                name: "ApiPermissionsVerifyPermission",
                url: "api/permissions/verifypermission/{permissionRight}",
                defaults: new { controller = "Permissions", action = "VerifyUserPermission", permissionRight = UrlParameter.Optional }
            );

            routes.MapRoute(
                name: "ApiPermissionsAll",
                url: "api/permissions/all",
                defaults: new { controller = "Permissions", action = "GetUadmAllUserPermissions" }
            );

            // Add this route for api/parameters
            routes.MapRoute(
                name: "ApiParametersAppParameterInfo",
                url: "api/parameters/parameterinfo/{para1}/{para2}/{parameterName}",
                defaults: new { controller = "Parameters", action = "GetUadmAppParametersInfo", para1 = UrlParameter.Optional, para2 = UrlParameter.Optional, parameterName = UrlParameter.Optional }
            );

            routes.MapRoute(
                name: "ApiParametersAll",
                url: "api/parameters/all",
                defaults: new { controller = "Parameters", action = "GetUadmAllAppParameters" }
            );

            routes.MapRoute(
                name: "Default",
                url: "{controller}/{action}/{id}",
                defaults: new { controller = "Home", action = "Index", id = UrlParameter.Optional }
            );
        }
    }
}
