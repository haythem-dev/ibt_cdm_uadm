using System.Collections.Generic;

namespace UadmCommon.Models
{
    public class UadmLibParameters
    {
        public Dictionary<string, UadmLibParameter> Parameters { get; set; } = new Dictionary<string, UadmLibParameter>();
    }
}
