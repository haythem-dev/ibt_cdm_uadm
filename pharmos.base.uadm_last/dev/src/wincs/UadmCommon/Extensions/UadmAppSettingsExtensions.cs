using System;
using System.Collections.Generic;
using System.Linq;

namespace UadmCommon.Models.Extensions
{
    public static class UadmAppSettingsExtensions
    {
        private static UadmAppSettings _instance = null;

        /// <summary>
        /// Instance
        /// </summary>
        /// <param name="fileName"></param>
        /// <returns></returns>
        public static UadmAppSettings Instance(string fileName = "UadmAppSettings.json")
        {
            if (_instance == null)
                _instance = new UadmAppSettings(fileName);

            return _instance;
        }

        /// <summary>
        /// LoadUadmAppSettings
        /// </summary>
        /// <param name="uadmAppSettings"></param>
        /// <param name="fileName"></param>
        /// <returns></returns>
        /// <exception cref="ArgumentException"></exception>
        public static UadmAppSettings LoadUadmAppSettings(this UadmAppSettings uadmAppSettings, string fileName) 
        {
            UadmAppSettings appSettings = uadmAppSettings.LoadFromFile<UadmAppSettings>(fileName);
            if (appSettings == null)
                throw new ArgumentException("AppSettings could not be loaded from file.");

            uadmAppSettings.Languages = appSettings.Languages;
            uadmAppSettings.Environments = appSettings.Environments;
            uadmAppSettings.Applications = appSettings.Applications;

            return uadmAppSettings;
        }

        #region COUNTRY

        /// <summary>
        /// GetSupportedApplicationCountries
        /// </summary>
        /// <returns></returns>
        public static List<string> GetSupportedApplicationCountries(this UadmAppSettings uadmAppSettings)
        {
            List<string> supportedCountries = new List<string>();

            foreach (UadmAppSettingsCountry country in uadmAppSettings.Countries)
            {
                supportedCountries.Add(country.Name);
            }

            return supportedCountries;
        }

        /// <summary>
        /// GetSupportedApplicationCountriesAsDictionary
        /// </summary>
        /// <returns></returns>
        public static Dictionary<string, string> GetSupportedApplicationCountriesAsDictionary(this UadmAppSettings uadmAppSettings)
        {
            Dictionary<string, string> supportedCountries = new Dictionary<string, string>();

            if (uadmAppSettings.Countries == null)
                return supportedCountries;  

            foreach (UadmAppSettingsCountry country in uadmAppSettings.Countries)
            {
                supportedCountries[country.Key] = country.Name;
            }

            return supportedCountries;
        }

        /// <summary>
        /// GetDefaultCountry
        /// </summary>
        /// <returns></returns>
        public static string GetDefaultCountry(this UadmAppSettings uadmAppSettings)
        {
            string defaultCountry = "";

            if (uadmAppSettings.Countries == null)
                return defaultCountry;

            foreach (UadmAppSettingsCountry country in uadmAppSettings.Countries)
            {
                if (string.IsNullOrEmpty(defaultCountry))
                {
                    defaultCountry = country.Key;
                }

                if (country.IsDefault)
                {
                    return country.Key;
                }
            }

            return defaultCountry;
        }

        /// <summary>
        /// GetCountry
        /// </summary>
        /// <returns></returns>
        public static UadmAppSettingsCountry GetCountry(this UadmAppSettings uadmAppSettings, string nameOrKey)
        {     
            if (uadmAppSettings.Countries == null)
                return null;

            foreach (UadmAppSettingsCountry country in uadmAppSettings.Countries)
            {
                if (country.Key.Equals(nameOrKey, StringComparison.OrdinalIgnoreCase))
                {
                    return country;
                }
                if (country.Name.Equals(nameOrKey, StringComparison.OrdinalIgnoreCase))
                {
                    return country;
                }
            }

            return null;
        }

        /// <summary>
        /// GetUadmAppUrlEnvironment
        /// </summary>
        /// <param name="uadmAppSettings"></param>
        /// <param name="url"></param>
        /// <returns></returns>
        public static UadmAppSettingsUrlEnvironment GetUadmAppUrlEnvironment(this UadmAppSettings uadmAppSettings, string url)
        {
            if (string.IsNullOrEmpty(url))
                return null;

            // Normalize the URL (remove trailing slash and convert to lowercase)
            url = url.Trim().Trim('/').ToLower();

            if (uadmAppSettings.ApplicationUrlEnvironments == null)
                return null;

            foreach (string key in uadmAppSettings.ApplicationUrlEnvironments.Keys)
            {
                if (string.IsNullOrEmpty(key))
                    continue;

                UadmAppSettingsUrlEnvironment uadmAppSettingsUrlEnvironment = uadmAppSettings.ApplicationUrlEnvironments[key];

                // Normalize the key (remove trailing slash and convert to lowercase)   
                string normalizedKey = uadmAppSettingsUrlEnvironment.Url.Trim().Trim('/').ToLower();

                if (url.UrlStartsWith(normalizedKey))
                    return uadmAppSettingsUrlEnvironment;
            }

            if (!uadmAppSettings.ApplicationUrlEnvironments.ContainsKey(url))
                return null;

            return uadmAppSettings.ApplicationUrlEnvironments[url];
        }

        /// <summary>
        /// GetUadmAppSettingsApplicationWebParameter
        /// </summary>
        /// <param name="uadmAppSettings"></param>
        /// <param name="applicationKey"></param>
        /// <param name="environmentKey"></param>
        /// <param name="originUrl"></param>
        /// <returns></returns>
        public static UadmAppSettingsApplicationWebParameter GetUadmAppSettingsApplicationWebParameter(this UadmAppSettings uadmAppSettings, string applicationKey, string environmentKey, string originUrl = null)
        {
            if (string.IsNullOrEmpty(applicationKey))
                return null;

            if (string.IsNullOrEmpty(environmentKey))
                return null;

            UadmAppSettingsApplication app = uadmAppSettings.GetApplication(applicationKey);
            if (app == null)
                return null;

            if (!app.SupportedEnvironments.ContainsKey(environmentKey))
                return null;

            // If originUrl is null or empty, return the default or first available
            if (string.IsNullOrEmpty(originUrl))
            {
                // Look for a default key or wildcard
                foreach (string key in app.SupportedEnvironments[environmentKey].Keys)
                {
                    if (string.IsNullOrEmpty(key))
                        continue;

                    // Check for wildcard or default
                    if (key == "*")
                        return app.SupportedEnvironments[environmentKey][key];

                    if (key == "default")
                        return app.SupportedEnvironments[environmentKey][key];

                    // Check for IsDefault
                    if (app.SupportedEnvironments[environmentKey][key].IsDefault)
                        return app.SupportedEnvironments[environmentKey][key];
                }

                // If no default found, return the first available
                return app.SupportedEnvironments[environmentKey].Values.FirstOrDefault();
            }

            // Normalize the URL (remove trailing slash and convert to lowercase and ignore querystring)   
            originUrl = originUrl?.Trim().Trim('/').ToLower().Split('?')[0];

            foreach (string key in app.SupportedEnvironments[environmentKey].Keys)
            {
                if (string.IsNullOrEmpty(key))
                    continue;
                
                // Normalize the key (remove trailing slash and convert to lowercase)   
                string normalizedKey = key.Trim().Trim('/').ToLower();

                if (originUrl.UrlStartsWith(normalizedKey))
                    return app.SupportedEnvironments[environmentKey][key];
            }

            if (app.SupportedEnvironments[environmentKey].ContainsKey(originUrl))
                return app.SupportedEnvironments[environmentKey][originUrl];

            if (app.SupportedEnvironments[environmentKey].ContainsKey("*"))
                return app.SupportedEnvironments[environmentKey]["*"];

            if (app.SupportedEnvironments[environmentKey].ContainsKey("default"))
                return app.SupportedEnvironments[environmentKey]["default"];

            return null;
        }

        /// <summary>
        /// UrlStartsWith
        /// </summary>
        /// <param name="url"></param>
        /// <param name="part"></param>
        /// <returns></returns>
        private static bool UrlStartsWith(this string url, string part)
        {
            // Normalize the URL (remove trailing slash and convert to lowercase)
            if (!url.EndsWith("/"))
                url += "/";

            // Normalize the key (remove trailing slash and convert to lowercase)
            if (!part.EndsWith("/"))
                part += "/";

            return url.StartsWith(part);
        }
        #endregion

        #region LANGUAGE

        /// <summary>
        /// GetSupportedLanguages
        /// </summary>
        /// <returns></returns>
        public static List<string> GetSupportedLanguages (this UadmAppSettings uadmAppSettings)
        {
            List<string> supportedLanguages = new List<string>();

            foreach (UadmAppSettingsLanguage language in uadmAppSettings.Languages)
            {
                supportedLanguages.Add(language.Name);
            }

            return supportedLanguages;
        }

        /// <summary>
        /// GetSupportedLanguagesAsDictionary
        /// </summary>
        /// <returns></returns>
        public static Dictionary<string, string> GetSupportedLanguagesAsDictionary(this UadmAppSettings uadmAppSettings, string applicationKey)
        {
            Dictionary<string, string> supportedLanguages = new Dictionary<string, string>();

            List<UadmAppSettingsLanguage> appLanguages = null;
            
            UadmAppSettingsApplication app = uadmAppSettings.GetApplication(applicationKey);

            // Use application specific languages if defined, otherwise use global languages
            if ((app != null) && (app.SupportedLanguages != null) && (app.SupportedLanguages.Count > 0))
            {
                appLanguages = app.SupportedLanguages;
            }
            else 
            { 
                appLanguages = uadmAppSettings.Languages;
            }

            // If no languages defined, return empty dictionary
            if ((appLanguages == null) || (appLanguages.Count == 0))
                return supportedLanguages;


            foreach (UadmAppSettingsLanguage language in appLanguages)
            {
                supportedLanguages[language.Key] = language.Name;
            }

            return supportedLanguages;
        }

        /// <summary>
        /// GetDefaultLanguage
        /// </summary>
        /// <returns></returns>
        public static string GetDefaultLanguage(this UadmAppSettings uadmAppSettings, string applicationKey)
        {
            string defaultLanguage = "";

            List<UadmAppSettingsLanguage> appLanguages = null;

            UadmAppSettingsApplication app = uadmAppSettings.GetApplication(applicationKey);

            // Use application specific languages if defined, otherwise use global languages
            if ((app != null) && (app.SupportedLanguages != null) && (app.SupportedLanguages.Count > 0))
            {
                appLanguages = app.SupportedLanguages;
            }
            else
            {
                appLanguages = uadmAppSettings.Languages;
            }

            // If no languages defined, return empty dictionary
            if ((appLanguages == null) || (appLanguages.Count == 0))
                return defaultLanguage;

            foreach (UadmAppSettingsLanguage language in appLanguages)
            {
                if (string.IsNullOrEmpty (defaultLanguage))
                {
                    defaultLanguage = language.Key;
                }

                if (language.IsDefault)
                {
                    return language.Key;
                }
            }

            return defaultLanguage;
        }

        /// <summary>
        /// GetLanguage
        /// </summary>
        /// <returns></returns>
        public static UadmAppSettingsLanguage GetLanguage(this UadmAppSettings uadmAppSettings, string nameOrKey)
        {
            foreach (UadmAppSettingsLanguage language in uadmAppSettings.Languages)
            {
                if (language.Key.Equals(nameOrKey, StringComparison.OrdinalIgnoreCase))
                {
                    return language;
                }
                if (language.Name.Equals(nameOrKey, StringComparison.OrdinalIgnoreCase))
                {
                    return language;
                }
            }

            return null;
        }

        #endregion

        #region ENVIRONMENTS

        /// <summary>
        /// GetSupportedEnvironments
        /// </summary>
        /// <returns></returns>
        public static List<string> GetSupportedEnvironments(this UadmAppSettings uadmAppSettings)
        {
            List<string> supportedEnvironments = new List<string>();

            foreach (UadmAppSettingsEnvironment environment in uadmAppSettings.Environments)
            {
                supportedEnvironments.Add(environment.Name);
            }

            return supportedEnvironments;
        }

        /// <summary>
        /// GetSupportedEnvironmentsAsDictionary
        /// </summary>
        /// <returns></returns>
        public static Dictionary<string, string> GetSupportedEnvironmentsAsDictionary(this UadmAppSettings uadmAppSettings)
        {
            Dictionary<string, string> supportedEnvironments = new Dictionary<string, string>();

            foreach (UadmAppSettingsEnvironment  app in uadmAppSettings.Environments)
            {
                supportedEnvironments[app.Key] = app.Name;
            }

            return supportedEnvironments;
        }

        /// <summary>
        /// GetDefaultEnvironment
        /// </summary>
        /// <returns></returns>
        public static string GetDefaultEnvironment(this UadmAppSettings uadmAppSettings)
        {
            string defaultEnvironment = "";

            foreach (UadmAppSettingsEnvironment environment in uadmAppSettings.Environments)
            {
                if (string.IsNullOrEmpty(defaultEnvironment))
                {
                    defaultEnvironment = environment.Key;
                }

                if (environment.IsDefault)
                {
                    return environment.Key;
                }
            }

            return defaultEnvironment;
        }


        /// <summary>
        /// GetEnvironment
        /// </summary>
        /// <returns></returns>
        public static UadmAppSettingsEnvironment GetEnvironment(this UadmAppSettings uadmAppSettings, string name)
        {
            foreach (UadmAppSettingsEnvironment environment in uadmAppSettings.Environments)
            {
                if (environment.Name.Equals(name, StringComparison.OrdinalIgnoreCase))
                {
                    return environment;
                }

                if (environment.Key.Equals(name, StringComparison.OrdinalIgnoreCase))
                {
                    return environment;
                }
            }

            return null;
        }

        #endregion

        #region APPLICATIONS

        /// <summary>
        /// GetSupportedApplications
        /// </summary>
        /// <returns></returns>
        public static List<string> GetSupportedApplications(this UadmAppSettings uadmAppSettings)
        {
            List<string> supportedApplications = new List<string>();

            foreach (UadmAppSettingsApplication app in uadmAppSettings.Applications)
            {
                supportedApplications.Add(app.Name);
            }

            return supportedApplications;
        }

        /// <summary>
        /// GetSupportedApplicationsAsDictionary
        /// </summary>
        /// <returns></returns>
        public static Dictionary<string, string> GetSupportedApplicationsAsDictionary(this UadmAppSettings uadmAppSettings)
        {
            Dictionary<string, string> supportedApplications = new Dictionary<string, string>();

            foreach (UadmAppSettingsApplication app in uadmAppSettings.Applications)
            {
                supportedApplications[app.Key] = app.Name;
            }

            return supportedApplications;
        }

        /// <summary>
        /// GetDefaultApplication
        /// </summary>
        /// <returns></returns>
        public static string GetDefaultApplication(this UadmAppSettings uadmAppSettings)
        {
            string defaultApplication = "";

            foreach (UadmAppSettingsApplication application in uadmAppSettings.Applications)
            {
                if (string.IsNullOrEmpty(defaultApplication))
                {
                    defaultApplication = application.Key;
                }

                if (application.IsDefault)
                {
                    return application.Key;
                }
            }

            return defaultApplication;
        }

        /// <summary>
        /// GetApplication
        /// </summary>
        /// <returns></returns>
        public static UadmAppSettingsApplication GetApplication(this UadmAppSettings uadmAppSettings, string nameOrKey)
        {
            if (string.IsNullOrEmpty(nameOrKey)) 
                return null;

            foreach (UadmAppSettingsApplication application in uadmAppSettings.Applications)
            {
                if (application.Name.Equals(nameOrKey, StringComparison.OrdinalIgnoreCase))
                {
                    return application;
                }
                if (application.Key.Equals(nameOrKey, StringComparison.OrdinalIgnoreCase))
                {
                    return application;
                }
            }

            return null;
        }

        /// <summary>
        /// GetApplicationItems
        /// </summary>
        /// <returns></returns>
        public static List<UadmAppSettingsApplicationItem> GetApplicationItems(this UadmAppSettings uadmAppSettings, string applicationNameOrKey, string environmentKey = null, string countryKey = null)
        {
            List<UadmAppSettingsApplicationItem> seenItems = new List <UadmAppSettingsApplicationItem> ();
            
            UadmAppSettingsApplication uadmAppSettingsApplication = uadmAppSettings.GetApplication(applicationNameOrKey);
            if (uadmAppSettingsApplication == null) 
                return seenItems;

            foreach (UadmAppSettingsApplicationItem item in uadmAppSettingsApplication.Items)
            {
                if ((string.IsNullOrEmpty(environmentKey) || (environmentKey == item.EnvironmentKey)) &&
                    (string.IsNullOrEmpty(countryKey) || (countryKey == item.CountryKey)))
                {
                    seenItems.Add(item);
                }
            }

            return seenItems;
        }

        /// <summary>
        /// GetApplicationItemsAsDictionary
        /// </summary>
        /// <returns></returns>
        public static Dictionary<string, string> GetApplicationItemsAsDictionary(this UadmAppSettings uadmAppSettings, string applicationNameOrKey, string environmentKey = null, string countryKey = null)
        {
            Dictionary<string, string> seenItems = new Dictionary<string, string>();

            UadmAppSettingsApplication uadmAppSettingsApplication = uadmAppSettings.GetApplication(applicationNameOrKey);
            if (uadmAppSettingsApplication == null)
                return seenItems;

            foreach (UadmAppSettingsApplicationItem item in uadmAppSettingsApplication.Items)
            {
                if ((string.IsNullOrEmpty(environmentKey) || (environmentKey == item.EnvironmentKey)) && 
                    (string.IsNullOrEmpty(countryKey) || (countryKey == item.CountryKey)))
                {
                    seenItems.Add(string.IsNullOrEmpty(item.Key) ? item.RegionBranch : item.Key, item.Name);
                }
            }

            return seenItems;
        }

        /// <summary>
        /// GetApplicationEnvironments
        /// </summary>
        /// <returns></returns>
        public static List<UadmAppSettingsEnvironment> GetApplicationEnvironments(this UadmAppSettings uadmAppSettings, string applicationNameOrKey)
        {
            Dictionary<string, UadmAppSettingsEnvironment> seenEnvironments = new Dictionary<string, UadmAppSettingsEnvironment>();

            UadmAppSettingsApplication uadmAppSettingsApplication = uadmAppSettings.GetApplication(applicationNameOrKey);
            if (uadmAppSettingsApplication == null)
                return seenEnvironments.Values.ToList();

            foreach (UadmAppSettingsApplicationItem item in uadmAppSettingsApplication.Items)
            {
                if (!seenEnvironments.ContainsKey(item.EnvironmentKey))
                {
                    UadmAppSettingsEnvironment environment = uadmAppSettings.GetEnvironment(item.EnvironmentKey);
                    seenEnvironments.Add(item.EnvironmentKey, environment);
                }
            }

            return seenEnvironments.Values.ToList();
        }

        /// <summary>
        /// GetApplicationEnvironments
        /// </summary>
        /// <returns></returns>
        public static Dictionary<string, string> GetApplicationEnvironmentsAsDictionary(this UadmAppSettings uadmAppSettings, string applicationNameOrKey)
        {
            Dictionary<string, string> seenEnvironments = new Dictionary<string, string>();

            UadmAppSettingsApplication uadmAppSettingsApplication = uadmAppSettings.GetApplication(applicationNameOrKey);
            if (uadmAppSettingsApplication == null)
                return seenEnvironments;

            foreach (string key in uadmAppSettingsApplication.SupportedEnvironments.Keys)
            {
                if (!seenEnvironments.ContainsKey(key))
                {
                    UadmAppSettingsEnvironment environment = uadmAppSettings.GetEnvironment(key);

                    seenEnvironments.Add(key, environment.Name);
                }
            }

            return seenEnvironments;
        }

        /// <summary>
        /// GetApplicationCountries
        /// </summary>
        /// <returns></returns>
        public static List<UadmAppSettingsCountry> GetApplicationCountries(this UadmAppSettings uadmAppSettings, string applicationNameOrKey, string environmentNameOrKey)
        {
            Dictionary<string, UadmAppSettingsCountry> seenCountries = new Dictionary<string, UadmAppSettingsCountry>();

            UadmAppSettingsApplication uadmAppSettingsApplication = uadmAppSettings.GetApplication(applicationNameOrKey);
            if (uadmAppSettingsApplication == null)
                return seenCountries.Values.ToList();

            foreach (UadmAppSettingsApplicationItem item in uadmAppSettingsApplication.Items)
            {
                if (string.IsNullOrEmpty(environmentNameOrKey) || (environmentNameOrKey == item.EnvironmentKey))
                {
                    if (!seenCountries.ContainsKey(item.CountryKey))
                    {
                        UadmAppSettingsCountry country = uadmAppSettings.GetCountry(item.CountryKey);

                        if (country != null)
                            seenCountries.Add(item.CountryKey, country);
                        else
                            seenCountries.Add(item.CountryKey, new UadmAppSettingsCountry() { Key = item.CountryKey, Name = item.CountryKey + " (not found)" });
                    }
                }
            }

            return seenCountries.Values.ToList();
        }

        #endregion

        #region APPLICATIONITEM

        /// <summary>
        /// GetSupportedApplicationItems
        /// </summary>
        /// <returns></returns>
        public static List<string> GetSupportedApplicationItems(this UadmAppSettings uadmAppSettings, string applicationNameOrKey)
        {
            UadmAppSettingsApplication uadmAppSettingsApplication = uadmAppSettings.GetApplication(applicationNameOrKey);
            if (uadmAppSettingsApplication == null)
                return new List<string>();  

            List<string> supportedApplicationItems = new List<string>();

            foreach (UadmAppSettingsApplicationItem appItem in uadmAppSettingsApplication.Items)
            {
                supportedApplicationItems.Add(appItem.Name);
            }

            return supportedApplicationItems;
        }

        /// <summary>
        /// GetSupportedApplicationItemsAsDictionary
        /// </summary>
        /// <returns></returns>
        public static Dictionary<string, string> GetSupportedApplicationItemsAsDictionary(this UadmAppSettings uadmAppSettings, string applicationNameOrKey)
        {
            UadmAppSettingsApplication uadmAppSettingsApplication = uadmAppSettings.GetApplication(applicationNameOrKey);
            if (uadmAppSettingsApplication == null)
                return new Dictionary<string, string>();

            Dictionary<string, string> supportedApplicationItems = new Dictionary<string, string>();

            foreach (UadmAppSettingsApplicationItem appItem in uadmAppSettingsApplication.Items)
            {
                supportedApplicationItems[appItem.RegionBranch] = appItem.Name;
            }

            return supportedApplicationItems;
        }

        /// <summary>
        /// GetDefaultApplicationItem
        /// </summary>
        /// <returns></returns>
        public static string GetDefaultApplicationItem(this UadmAppSettings uadmAppSettings, string applicationNameOrKey)
        {
            UadmAppSettingsApplication uadmAppSettingsApplication = uadmAppSettings.GetApplication(applicationNameOrKey);
            if (uadmAppSettingsApplication == null)
                return "";

            string defaultApplicationItem = "";

            foreach (UadmAppSettingsApplicationItem appItem in uadmAppSettingsApplication.Items)
            {
                if (string.IsNullOrEmpty(defaultApplicationItem))
                {
                    defaultApplicationItem = appItem.Name;
                }

                if (appItem.IsDefault)
                {
                    return appItem.Name;
                }
            }

            return defaultApplicationItem;
        }

        /// <summary>
        /// GetApplicationItem
        /// </summary>
        /// <returns></returns>
        public static UadmAppSettingsApplicationItem GetApplicationItem(this UadmAppSettings uadmAppSettings, string applicationNameOrKey, string name)
        {
            UadmAppSettingsApplication uadmAppSettingsApplication = uadmAppSettings.GetApplication(applicationNameOrKey);
            if (uadmAppSettingsApplication == null)
                return null;

            foreach (UadmAppSettingsApplicationItem appItem in uadmAppSettingsApplication.Items)
            {
                if (appItem.Name.Equals(name, StringComparison.OrdinalIgnoreCase))
                {
                    return appItem;
                }
                if (appItem.Key.Equals(name, StringComparison.OrdinalIgnoreCase))
                {
                    return appItem;
                }
            }

            return null;
        }

        /// <summary>
        /// GetApplicationItemEnvironment
        /// </summary>
        /// <returns></returns>
        public static UadmAppSettingsEnvironment GetApplicationItemEnvironment(this UadmAppSettings uadmAppSettings, string applicationNameOrKey, string name)
        {
            UadmAppSettingsApplicationItem appItem = uadmAppSettings.GetApplicationItem(applicationNameOrKey, name);

            if (appItem == null)
                return null;
    
            return uadmAppSettings.GetEnvironment(appItem.EnvironmentKey);
        }

        #endregion
    }
}
