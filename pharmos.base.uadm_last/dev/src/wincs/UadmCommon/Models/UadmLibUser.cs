namespace UadmCommon.Models
{
    public class UadmLibUser
    {
        public long UserId { get; set; } = 0;

        public string Username { get; set; } = "";

        public string Email { get; set; } = "";

        public string Firstname { get; set; } = "";

        public string Lastname { get; set; } = "";

        public string DomainAccount { get; set; } = "";

        public string Description { get; set; } = "";

        public string OSAccount { get; set; } = "";

        public bool IsAuthenticated{ get; set; } = false;

        public bool IsActive { get; set; } = false;

        public bool IsPasswordChangeRequired { get; set; } = false;

        public long LastPasswordChangeDate { get; set; } = 0;

        public long LastLoginDate { get; set; } = 0;
    }
}
