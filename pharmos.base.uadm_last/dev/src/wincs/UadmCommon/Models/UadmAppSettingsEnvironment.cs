namespace UadmCommon.Models
{
    public class UadmAppSettingsEnvironment
    {
        public string Key { get; set; } = "";

        public string Name { get; set; } = "";

        public bool UseNewPasswordRules { get; set; } = true;

        public string PrimaryServer { get; set; } = "";

        public string PrimaryDatabase { get; set; } = "";

        public string SecondaryServer { get; set; } = "";

        public string SecondaryDatabase { get; set; } = "";

        public string VoodooPipeName { get; set; } = "";

        public int VoodooPipePort { get; set; } = 0;

        public bool IsDefault { get; set; } = false;    
    }
}
