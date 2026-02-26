using UadmCommon.Models;

namespace UadmCommon.Extensions
{
    public static class UadmAppSettingsEnvironmentExtensions
    {
        /// <summary>
        /// ConvertToUadmLibEnvironment
        /// </summary>
        /// <param name="uadmAppSettingsEnvironment"></param>
        /// <param name="applicationSettings"></param>
        /// <returns></returns>
        public static UadmLibEnvironment ConvertToUadmLibEnvironment(this UadmAppSettingsEnvironment uadmAppSettingsEnvironment, UadmAppSettingsApplication applicationSettings)
        {
            if (uadmAppSettingsEnvironment == null)
                return null;

            UadmLibEnvironment uadmAppEnvironment = new UadmLibEnvironment()
            {
                Application = applicationSettings.Key,
                AppVersion = "",
                Environment = uadmAppSettingsEnvironment.Key,
                EnvironmentName = uadmAppSettingsEnvironment.Name,
                PrimaryServer = uadmAppSettingsEnvironment.PrimaryServer,
                PrimaryDatabase = uadmAppSettingsEnvironment.PrimaryDatabase,
                SecondaryServer = uadmAppSettingsEnvironment.SecondaryServer,
                SecondaryDatabase = uadmAppSettingsEnvironment.SecondaryDatabase,
                VoodooPipeName = uadmAppSettingsEnvironment.VoodooPipeName,
                VoodooPipePort = uadmAppSettingsEnvironment.VoodooPipePort,
                VoodooPipePortText = uadmAppSettingsEnvironment.VoodooPipePort.ToString()
            };

            return uadmAppEnvironment;
        }

        /// <summary>
        /// ConvertToUadmLibEnvironment
        /// </summary>
        /// <param name="uadmAppSettingsEnvironment"></param>
        /// <param name="applicationKey"></param>
        /// <returns></returns>
        public static UadmLibEnvironment ConvertToUadmLibEnvironment(this UadmAppSettingsEnvironment uadmAppSettingsEnvironment, string applicationKey)
        {
            if (uadmAppSettingsEnvironment == null)
                return null;

            UadmLibEnvironment uadmAppEnvironment = new UadmLibEnvironment()
            {
                Application = applicationKey,
                AppVersion = "",
                Environment = uadmAppSettingsEnvironment.Key,
                EnvironmentName = uadmAppSettingsEnvironment.Name,
                PrimaryServer = uadmAppSettingsEnvironment.PrimaryServer,
                PrimaryDatabase = uadmAppSettingsEnvironment.PrimaryDatabase,
                SecondaryServer = uadmAppSettingsEnvironment.SecondaryServer,
                SecondaryDatabase = uadmAppSettingsEnvironment.SecondaryDatabase,
                VoodooPipeName = uadmAppSettingsEnvironment.VoodooPipeName,
                VoodooPipePort = uadmAppSettingsEnvironment.VoodooPipePort,
                VoodooPipePortText = uadmAppSettingsEnvironment.VoodooPipePort.ToString()

            };

            return uadmAppEnvironment;
        }
    }
}
