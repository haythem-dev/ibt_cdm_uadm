using System.Web.Mvc;
using UadmWebLoginService.Classes;

namespace UadmWebLoginService.Controllers
{
    [RoutePrefix("api/permissions")]
    [UadmRequestAuthorize]
    public class BaseController : Controller
    {
    }
}
