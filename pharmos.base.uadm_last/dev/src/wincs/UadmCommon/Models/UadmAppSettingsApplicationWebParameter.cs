namespace UadmCommon.Models
{
    public class UadmAppSettingsApplicationWebParameter
    {
        public string ApplicationId { get; set; }    

        public bool IsDefault { get; set; } = false;

        public int? UserTokenExpirationMinutes { get; set; }

        public string LoginAuthenticationValidUrl { get; set; }

        public string LoginRedirectUrl { get; set; }

        public string ApplicationRedirectView { get; set; }

        public string ApplicationRedirectController { get; set; }

        public string ApplicationRedirectAction { get; set; }

        public bool HideCountries { get; set; } = false;

        public bool HideLanguages { get; set; } = false;

        public bool HideApplicationItems { get; set; } = false;

        public bool HideEnvironments { get; set; } = false;

        public bool HideApplications { get; set; } = false;
    }
}
