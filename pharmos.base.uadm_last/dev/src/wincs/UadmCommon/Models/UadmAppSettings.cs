using System;
using System.Collections.Generic;
using UadmCommon.Classes;

namespace UadmCommon.Models
{
    public class UadmAppSettings : AppSettings
    {
        public Guid CorrelationId { get; set; } = Guid.NewGuid();

        public bool UsePasswordChange { get; set; } = true;    

        public List<UadmAppSettingsCountry> Countries { get; set; }

        public List<UadmAppSettingsLanguage> Languages { get; set; }

        public List<UadmAppSettingsEnvironment> Environments { get; set; }

        public List<UadmAppSettingsApplication> Applications { get; set; }

        public Dictionary<string, UadmAppSettingsUrlEnvironment> ApplicationUrlEnvironments { get; set; }

        /// <summary>
        /// UadmAppSettings
        /// </summary>
        public UadmAppSettings()
        {
        }

        /// <summary>
        /// UadmAppSettings
        /// </summary>
        /// <param name="fileName"></param>
        public UadmAppSettings(string fileName) : base(fileName)
        {
            UadmAppSettings appSettings = LoadFromFile<UadmAppSettings>(fileName);
            if (appSettings == null)
                throw new ArgumentException("AppSettings could not be loaded from file.");

            Countries = appSettings.Countries;
            Languages = appSettings.Languages;
            Environments = appSettings.Environments;
            Applications = appSettings.Applications;

            UpdateUadmAppSettingsApplicationWebParameter();
            CreateUadmAppUrlEnvironments();
        }

        /// <summary>
        /// UpdateUadmAppSettingsApplicationWebParameter
        /// </summary>
        public void UpdateUadmAppSettingsApplicationWebParameter()
        {
            // Build the ApplicationUrlEnvironments dictionary
            foreach (UadmAppSettingsApplication app in Applications)
            {
                if (app.SupportedEnvironments == null)
                {
                    UadmLogger.LogWarning("Supported environments are not defined for application: " + app.Key);
                    continue;
                }

                foreach (string environmentKey in app.SupportedEnvironments.Keys)
                {
                    Dictionary<string, UadmAppSettingsApplicationWebParameter> uadmAppSettingsApplicationWebParameters = app.SupportedEnvironments[environmentKey];

                    foreach (string url in uadmAppSettingsApplicationWebParameters.Keys)
                    {
                        if (string.IsNullOrEmpty(url))
                            continue;
                        
                        UadmAppSettingsApplicationWebParameter uadmAppSettingsApplicationWebParameter = uadmAppSettingsApplicationWebParameters[url];

                        if (string.IsNullOrEmpty(uadmAppSettingsApplicationWebParameter.LoginAuthenticationValidUrl))
                        {
                            uadmAppSettingsApplicationWebParameter.LoginAuthenticationValidUrl = (url + app.DefaultLoginAuthenticationValidUrl)?.ToLower();
                        }
                        else if (uadmAppSettingsApplicationWebParameter.LoginAuthenticationValidUrl.StartsWith("/") == true)
                        {
                            uadmAppSettingsApplicationWebParameter.LoginAuthenticationValidUrl = (url + uadmAppSettingsApplicationWebParameter.LoginAuthenticationValidUrl)?.ToLower();
                        }

                        if (string.IsNullOrEmpty(uadmAppSettingsApplicationWebParameter.LoginRedirectUrl))
                        {
                            uadmAppSettingsApplicationWebParameter.LoginRedirectUrl = (url + app.DefaultLoginRedirectUrl)?.ToLower();
                        }
                        else if (uadmAppSettingsApplicationWebParameter.LoginRedirectUrl.StartsWith("/") == true)
                        {
                            uadmAppSettingsApplicationWebParameter.LoginRedirectUrl = (url + uadmAppSettingsApplicationWebParameter.LoginRedirectUrl)?.ToLower();
                        }

                        if (uadmAppSettingsApplicationWebParameter.UserTokenExpirationMinutes == null)
                        {
                            uadmAppSettingsApplicationWebParameter.UserTokenExpirationMinutes = app.DefaultUserTokenExpirationMinutes;
                        }
                    }
                }
            }
        }

        /// <summary>
        /// CreateUadmAppUrlEnvironments
        /// </summary>
        public void CreateUadmAppUrlEnvironments()
        {
            ApplicationUrlEnvironments = new Dictionary<string, UadmAppSettingsUrlEnvironment>();

            // Build the ApplicationUrlEnvironments dictionary
            foreach (UadmAppSettingsApplication app in Applications)
            {
                foreach (string environmentKey in app.SupportedEnvironments.Keys)
                {
                    Dictionary<string, UadmAppSettingsApplicationWebParameter> uadmAppSettingsApplicationWebParameters = app.SupportedEnvironments[environmentKey];

                    if (uadmAppSettingsApplicationWebParameters == null)
                        continue;

                    foreach (string url in uadmAppSettingsApplicationWebParameters.Keys)
                    {
                        if (string.IsNullOrEmpty(url))
                            continue;

                        string appUrl = url.Trim().ToLower();

                        UadmAppSettingsApplicationWebParameter uadmAppSettingsApplicationWebParameter = uadmAppSettingsApplicationWebParameters[appUrl];

                        if (ApplicationUrlEnvironments.ContainsKey(appUrl))
                            continue;

                        UadmAppSettingsUrlEnvironment appUrlEnv = new UadmAppSettingsUrlEnvironment
                        {
                            Url = appUrl,
                            ApplicationKey = app.Key,
                            Environment = environmentKey
                        };

                        ApplicationUrlEnvironments[appUrl] = appUrlEnv;
                    }
                }
            }
        }
    }
}
