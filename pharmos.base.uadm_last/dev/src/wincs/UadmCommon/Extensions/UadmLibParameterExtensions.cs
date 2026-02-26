using System.Collections.Generic;
using UadmCommon.Models;

namespace UadmCommon.Extensions
{
    public static class UadmLibParameterExtensions
    {
        /// <summary>
        /// ToUadmAppParameter
        /// </summary>
        /// <param name="uadmAppParameter"></param>
        /// <param name="parameters"></param>
        public static void ToUadmAppParameter(this UadmLibParameter uadmAppParameter, Dictionary<string, string> parameters)
        {
            uadmAppParameter.Initialize();
            
            if (parameters == null || parameters.Count == 0)
                return;
            
            uadmAppParameter.AppId = int.Parse(parameters.ContainsKey(nameof(uadmAppParameter.AppId)) ? parameters[nameof(uadmAppParameter.AppId)] : "0");
            uadmAppParameter.Para1 = parameters.ContainsKey(nameof(uadmAppParameter.Para1)) ? parameters[nameof(uadmAppParameter.Para1)] : string.Empty;
            uadmAppParameter.Para2 = parameters.ContainsKey(nameof(uadmAppParameter.Para2)) ? parameters[nameof(uadmAppParameter.Para2)] : string.Empty;
            uadmAppParameter.ParaName = parameters.ContainsKey(nameof(uadmAppParameter.ParaName)) ? parameters[nameof(uadmAppParameter.ParaName)] : string.Empty;
            uadmAppParameter.ParaValue = parameters.ContainsKey(nameof(uadmAppParameter.ParaValue)) ? parameters[nameof(uadmAppParameter.ParaValue)] : string.Empty;
        }

        /// <summary>
        /// ToDictionary
        /// </summary>
        /// <param name="uadmAppParameter"></param>
        public static Dictionary<string, string> ToDictionary(this UadmLibParameter uadmAppParameter)
        {
            Dictionary<string, string> parameters = new Dictionary<string, string>
            {
                [nameof(uadmAppParameter.AppId)] = uadmAppParameter.AppId.ToString(),
                [nameof(uadmAppParameter.Para1)] = uadmAppParameter.Para1 ?? string.Empty,
                [nameof(uadmAppParameter.Para2)] = uadmAppParameter.Para2 ?? string.Empty,
                [nameof(uadmAppParameter.ParaName)] = uadmAppParameter.ParaName ?? string.Empty,
                [nameof(uadmAppParameter.ParaValue)] = uadmAppParameter.ParaValue ?? string.Empty
            };

            return parameters;
        }

        /// <summary>
        /// Initialize
        /// </summary>
        /// <param name="uadmAppParameter"></param>
        public static void Initialize(this UadmLibParameter uadmAppParameter)
        {
            uadmAppParameter.AppId = 0;
            uadmAppParameter.Para1 = string.Empty;
            uadmAppParameter.Para2 = string.Empty;
            uadmAppParameter.ParaName = string.Empty;
            uadmAppParameter.ParaValue = string.Empty;
        }
    }
}
