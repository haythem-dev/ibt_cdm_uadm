using System.Collections.Generic;

namespace UadmCommon.Models
{
    public class UadmLibPermissions
    {
       public Dictionary<string, UadmLibPermission> Permissions { get; set; } = new Dictionary<string, UadmLibPermission>();
    }
}
