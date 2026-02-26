namespace UadmCommon.Models
{
    public class UadmAppSettingsApplicationItem
    {
        public string Key { get; set; } = "";

        public string Name { get; set; } = "";

        public string EnvironmentKey { get; set; } = "";

        public string CountryKey { get; set; } = "";

        public int BranchNo { get; set; } = 0;

        public string RegionBranch { get; set; } = "";

        public string RegionBranchName { get; set; } = "";

        public string Server { get; set; } = "";

        public string Database { get; set; } = "";

        public int Port { get; set; } = 0;

        public bool IsDefault { get; set; } = false;    
    }
}
