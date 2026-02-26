using System.Collections.Generic;

namespace UadmCommon.Models
{
    public class UadmAppSettingsApplication
    {
        public string Key { get; set; }

        public string Name { get; set; }

        public string DefaultApplicationRedirectUrl { get; set; } = "/";

        public string DefaultLoginRedirectUrl { get; set; } = "/UadmLogin/Index";

        public string DefaultLoginAuthenticationValidUrl { get; set; } = "/UadmLogin/AuthenticationValid";

        public int DefaultUserTokenExpirationMinutes { get; set; } = 120;

        public bool HideCountries { get; set; } = false;

        public bool HideLanguages { get; set; } = false;

        public bool HideApplicationItems { get; set; } = false;

        public bool HideEnvironments { get; set; } = false;

        public bool HideApplications { get; set; } = false;

        public Dictionary<string, Dictionary<string, UadmAppSettingsApplicationWebParameter>> SupportedEnvironments { get; set; }

        public List<UadmAppSettingsLanguage> SupportedLanguages { get; set; }

        public List<UadmAppSettingsApplicationItem> Items { get; set; }

        public bool IsDefault { get; set; } = false;
    }
}
