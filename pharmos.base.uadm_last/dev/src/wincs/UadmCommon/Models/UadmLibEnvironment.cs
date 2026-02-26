namespace UadmCommon.Models
{
    public class UadmLibEnvironment
    {
        public string Application { get; set; } = "";

        public string AppVersion { get; set; } = "";

        public string Environment { get; set; } = "";

        public string EnvironmentName { get; set; } = "";

        public string PrimaryServer { get; set; } = "";

        public string PrimaryDatabase { get; set; } = "";

        public string SecondaryServer { get; set; } = "";

        public string SecondaryDatabase { get; set; } = "";

        public string VoodooPipeName { get; set; } = "";

        public int VoodooPipePort { get; set; } = 0;

        public string VoodooPipePortText { get; set; } = "";
    }
}
