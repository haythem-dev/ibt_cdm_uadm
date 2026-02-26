using Newtonsoft.Json;

namespace UadmCommon.Models
{
    public class UadmLibResponse<T>
    {
        [JsonProperty("returnCode")]
        public int ReturnCode { get; set; }

        [JsonProperty("errorMessage")]
        public string ErrorMessage { get; set; }    

        [JsonProperty("data")]
        public T Data { get; set; }
    }
}
