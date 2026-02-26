namespace UadmCommon.Models
{
    public class UadmAppSettingsUrlEnvironment
    {
        public string Url { get; set; } = "";
        
        public string ApplicationKey { get; set; } = "";

        public string Environment{ get; set; } = "";

        public bool IsDefault { get; set; } = false;
    }
}
