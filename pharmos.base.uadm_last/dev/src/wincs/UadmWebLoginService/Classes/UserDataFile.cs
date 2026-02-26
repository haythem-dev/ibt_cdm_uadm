using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.IO;
using UadmWebLoginService.Models;

namespace UadmWebLoginService.Classes
{
    public class UserDataFile
    {
        /// <summary>
        /// Save
        /// </summary>
        /// <param name="fileName"></param>
        /// <param name="model"></param>
        /// <returns></returns>
        public static bool Save(string fileName, LoginViewModel model)
        {
            CreateFolderIfNotExists(fileName);

            Dictionary<string, LoginViewModel> uadmUserData = Load(fileName);

            uadmUserData[model.SelectedApplication] = model;

            string json = JsonConvert.SerializeObject(uadmUserData);

            // Write to file
            System.IO.File.WriteAllText(fileName, json);

            return true;
        }

        /// <summary>
        /// Load
        /// </summary>
        /// <param name="fileName"></param>
        /// <returns></returns>
        public static Dictionary<string, LoginViewModel> Load(string fileName)
        {
            CreateFolderIfNotExists(fileName);

            if (!System.IO.File.Exists(fileName))
            {
                return new Dictionary<string, LoginViewModel>();
            }

            // Read from file
            string json = System.IO.File.ReadAllText(fileName);

            return JsonConvert.DeserializeObject<Dictionary<string, LoginViewModel>>(json) ?? new Dictionary<string, LoginViewModel>();
        }

        /// <summary>
        /// CreateFolderIfNotExists
        /// </summary>
        /// <param name="fileName"></param>
        /// <returns></returns>
        public static void CreateFolderIfNotExists(string fileName)
        {
            Path.GetDirectoryName(fileName);

            if (!Directory.Exists(Path.GetDirectoryName(fileName)))
            {
                Directory.CreateDirectory(Path.GetDirectoryName(fileName) ?? throw new InvalidOperationException());
            }
        }
    }
}