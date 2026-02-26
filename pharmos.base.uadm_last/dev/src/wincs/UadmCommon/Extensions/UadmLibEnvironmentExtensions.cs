using System.Collections.Generic;
using UadmCommon.Classes;
using UadmCommon.Models;
using UadmCommon.Models.Extensions;

namespace UadmCommon.Extensions
{
    public static class UadmLibEnvironmentExtensions
    {
        /// <summary>
        /// GetFromClaims
        /// </summary>
        /// <param name="parameters"></param>
        /// <param name="uadmAppSettings"></param>
        /// <returns></returns>
        public static UadmLibEnvironment GetFromClaims(Dictionary<string, string> parameters, UadmAppSettings uadmAppSettings)
        {
            string application = parameters.ContainsKey(UadmUserToken.CLAIM_APPLICATION) ? parameters[UadmUserToken.CLAIM_APPLICATION] : string.Empty;
            string environment = parameters.ContainsKey(UadmUserToken.CLAIM_ENVIRONMENT) ? parameters[UadmUserToken.CLAIM_ENVIRONMENT] : string.Empty;

            UadmAppSettingsApplication applicationSettings = uadmAppSettings.GetApplication(application);
            UadmAppSettingsEnvironment environmentSettings = uadmAppSettings.GetEnvironment(environment);

            UadmLibEnvironment uadmAppEnvironment = environmentSettings.ConvertToUadmLibEnvironment(applicationSettings);

            return uadmAppEnvironment;
        }
    }
}
