using System.Web.Mvc;

public class OkResult : ActionResult
{
    private readonly string _content;

    public OkResult(string content = "OK")
    {
        _content = content;
    }

    public override void ExecuteResult(ControllerContext context)
    {
        var response = context.HttpContext.Response;
        response.StatusCode = 200;
        response.ContentType = "text/plain";
        response.Write(_content);
    }
}