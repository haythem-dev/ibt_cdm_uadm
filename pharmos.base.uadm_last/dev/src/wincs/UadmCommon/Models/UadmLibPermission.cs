namespace UadmCommon.Models
{
    public class UadmLibPermission
    {
        public int UserId { get; set; } = 0;
        
        public int GroupId { get; set; } = 0;
        
        public string Right { get; set; }
        
        public int ModuleId { get; set; } = 0;
        
        public int AppId { get; set; } = 0;
        
        public int RegionBranch { get; set; } = 0;
    }
}
