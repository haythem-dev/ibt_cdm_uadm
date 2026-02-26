using System.Collections.Generic;
using UadmCommon.Models;

namespace UadmCommon.Extensions
{
    public static class UadmLibPermissionExtensions
    {
        /// <summary>
        /// ToUadmAppPermission
        /// </summary>
        /// <param name="uadmAppPermission"></param>
        /// <param name="permission"></param>
        public static void ToUadmAppPermission(this UadmLibPermission uadmAppPermission, Dictionary<string, string> permission)
        {
            uadmAppPermission.Initialize();

            if (permission == null || permission.Count == 0)
                return;

            uadmAppPermission.AppId = int.Parse(permission.ContainsKey(nameof(UadmLibPermission.AppId)) ? permission[nameof(UadmLibPermission.AppId)] : "0");
            uadmAppPermission.UserId = int.Parse(permission.ContainsKey(nameof(UadmLibPermission.UserId)) ? permission[nameof(UadmLibPermission.UserId)] : "0");
            uadmAppPermission.GroupId = int.Parse(permission.ContainsKey(nameof(UadmLibPermission.GroupId)) ? permission[nameof(UadmLibPermission.GroupId)] : "0");
            uadmAppPermission.ModuleId = int.Parse(permission.ContainsKey(nameof(UadmLibPermission.ModuleId)) ? permission[nameof(UadmLibPermission.ModuleId)] : "0");
            uadmAppPermission.RegionBranch = int.Parse(permission.ContainsKey(nameof(UadmLibPermission.RegionBranch)) ? permission[nameof(UadmLibPermission.RegionBranch)] : "0");

            uadmAppPermission.Right = permission.ContainsKey(nameof(UadmLibPermission.Right)) ? permission[nameof(UadmLibPermission.Right)] : string.Empty;
        }

        /// <summary>
        /// ToDictionary
        /// </summary>
        /// <param name="uadmAppPermission"></param>
        public static Dictionary<string, string> ToDictionary(this UadmLibPermission uadmAppPermission)
        {
            Dictionary<string, string> parameters = new Dictionary<string, string>
            {
                [nameof(UadmLibPermission.AppId)] = uadmAppPermission.AppId.ToString(),
                [nameof(UadmLibPermission.UserId)] = uadmAppPermission.UserId.ToString(),
                [nameof(UadmLibPermission.GroupId)] = uadmAppPermission.GroupId.ToString(),
                [nameof(UadmLibPermission.ModuleId)] = uadmAppPermission.ModuleId.ToString(),
                [nameof(UadmLibPermission.RegionBranch)] = uadmAppPermission.RegionBranch.ToString(),

                [nameof(uadmAppPermission.Right)] = uadmAppPermission.Right ?? string.Empty
            };

            return parameters;
        }

        /// <summary>
        /// Initialize
        /// </summary>
        /// <param name="uadmAppPermission"></param>
        public static void Initialize(this UadmLibPermission uadmAppPermission)
        {
            uadmAppPermission.AppId = 0;
            uadmAppPermission.UserId = 0;
            uadmAppPermission.GroupId = 0;
            uadmAppPermission.ModuleId = 0;
            uadmAppPermission.RegionBranch = 0;

            uadmAppPermission.Right = string.Empty; 

        }
    }
}
