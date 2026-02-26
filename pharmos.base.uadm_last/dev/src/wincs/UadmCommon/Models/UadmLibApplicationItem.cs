using Newtonsoft.Json;

namespace UadmCommon.Models
{
    public class UadmLibApplicationItem
    {
        [JsonProperty("intEntryNo")]
        public int EntryId { get; set; } = 0;

        [JsonProperty("intKey")]
        public string Key { get; set; } = "";

        [JsonProperty("strEntryName")]
        public string Name { get; set; } = "";

        [JsonProperty("strEnvironment")]
        public string EnvironmentKey { get; set; } = "";

        [JsonProperty("strCountry")]
        public string CountryKey { get; set; } = "";

        [JsonProperty("intRegionBranchNo")]
        public int BranchRegionId { get; set; } = 0;

        [JsonProperty("strRegionBranchNo")]
        public string RegionBranch { get; set; } = "";

        [JsonProperty("strRegionBranchName")]
        public string RegionBranchName { get; set; } = "";

        [JsonProperty("strServer")]
        public string Server { get; set; } = "";

        [JsonProperty("strDatabase")]
        public string Database { get; set; } = "";

        [JsonProperty("intPortNo")]
        public int Port { get; set; } = 0;

        [JsonProperty("strLanguage")]
        public bool IsDefault { get; set; } = false;    
    }
}
