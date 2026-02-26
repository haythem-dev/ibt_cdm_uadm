using Newtonsoft.Json;
using System.Collections.Generic;

namespace UadmCommon.Models
{
    public class UadmUserData
    {
        // public members
        [JsonIgnore]
        public long UserId { get; set; }

        [JsonIgnore]
        public string UserName { get; set; }

        [JsonIgnore]
        public string FirstName { get; set; }

        [JsonIgnore]
        public string LastName { get; set; }

        [JsonIgnore]
        public string Email { get; set; }

        [JsonIgnore]
        public string DomainAccount { get; set; }

        [JsonIgnore]
        public string OSAccount { get; set; }

        [JsonIgnore]
        public bool IsActive { get; set; }

        [JsonIgnore]
        public bool ForcePasswordChange { get; set; }

        [JsonIgnore]
        public long LastPasswordChangeDate { get; set; }

        [JsonIgnore]
        public long LastLoginDate { get; set; }
    }
}
