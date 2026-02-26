using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Runtime.Remoting.Messaging;
using System.Web;
using System.Web.Mvc;
using UadmCommon;
using UadmCommon.Classes;

public class UadmRedirectResult : ActionResult
{
    public bool Permanent { get; private set; }

    public string Url { get; private set; }

    public string OsAccount { get; private set; }

    public bool AddHeader { get; private set; }

    public bool AddQueryString { get; private set; }

    public bool UseEncryptedQueryString { get; private set; } = true;

    Dictionary<string, string> Values = new Dictionary<string, string>();

    /// <summary>
    /// UadmRedirectResult
    /// </summary>
    /// <param name="url"></param>
    /// <param name="osAccount"></param>
    /// <param name="values"></param>
    /// <param name="addHeader"></param>
    /// <param name="addQueryString"></param>
    /// <param name="useEncryptedQueryString"></param>
    public UadmRedirectResult(string url, string osAccount = null, Dictionary<string, string> values = null, bool addHeader = false, bool addQueryString = true, bool useEncryptedQueryString = true)
    {
        Url = url;
        OsAccount = osAccount;
        Values = values;
        AddHeader = addHeader;
        AddQueryString = addQueryString;
        UseEncryptedQueryString = useEncryptedQueryString;
    }

    /// <summary>
    /// UadmRedirectResult
    /// </summary>
    /// <param name="url"></param>
    /// <param name="osAccount"></param>
    /// <param name="header"></param>
    /// <param name="value"></param>
    /// <param name="addHeader"></param>
    /// <param name="addQueryString"></param>
    /// <param name="useEncryptedQueryString"></param>
    public UadmRedirectResult(string url, string osAccount = null, string header = null, string value = null, bool addHeader = false, bool addQueryString = true, bool useEncryptedQueryString = true):
        this (
            url,
            osAccount, 
            new Dictionary<string, string>()
            {
                { header ?? "x-uadm-undefined", value ?? "not-set"}
            }, 
            addHeader, 
            addQueryString, 
            useEncryptedQueryString
        )
    {        
    }

    /// <summary>
    /// ExecuteResult
    /// </summary>
    /// <param name="context"></param>
    /// <exception cref="ArgumentNullException"></exception>
    /// <exception cref="InvalidOperationException"></exception>
    public override void ExecuteResult(ControllerContext context)
    {
        if (context == null)
        {
            throw new ArgumentNullException("context");
        }

        if (context.IsChildAction)
        {
            throw new InvalidOperationException("UadmRedirectResult: CannotRedirectInChildAction");
        }

        string url = UrlHelper.GenerateContentUrl(Url, context.HttpContext);
        context.Controller.TempData.Keep();

        string queryString = "";
        if ((Values != null) && (Values.Count > 0))
        {
            // Add custom headers if required
            if (AddHeader)
            {
                // Add custom headers
                foreach (KeyValuePair<string, string> header in Values)
                {
                    if (!string.IsNullOrEmpty(header.Key) && !string.IsNullOrEmpty(header.Value))
                    {
                        context.HttpContext.Response.AppendHeader(header.Key, header.Value);
                        context.HttpContext.Request.Headers[header.Key] = header.Value; 
                    }
                }
            }

            // Add encrypted query string            
            if (AddQueryString)
            {
                if (UseEncryptedQueryString)
                    queryString = "?" + UadmRequestValues.REQUESTVALUES_PARAMETER + "=" + HttpUtility.UrlEncode(Crypt.EncryptToBase64(OsAccount, JsonConvert.SerializeObject(Values)));
                else
                    queryString = "?" + UadmRequestValues.REQUESTVALUES_PARAMETER + "=" + HttpUtility.UrlEncode(JsonConvert.SerializeObject(Values));
            }
        }

        if (context.HttpContext.Request.Headers["x-requested-with"] == "XMLHttpRequest")
        {
            context.HttpContext.Response.AppendHeader("x-redirect", url?.TrimEnd('/') + queryString);
            context.HttpContext.Response.End();
        }
        else
        {
            context.HttpContext.Response.Redirect(url?.TrimEnd('/') + queryString, endResponse: true);
        }
    }
}