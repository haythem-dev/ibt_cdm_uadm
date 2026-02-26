using System;

namespace UadmCommon.Classes
{
    public class UadmValidation
    {
        /// <summary>
        /// IsValidUrl
        /// </summary>
        /// <param name="url"></param>
        /// <returns></returns>
        public static bool IsValidUrl(string url)
        {
            if (Uri.TryCreate(url, UriKind.Absolute, out Uri uriResult))
            {
                return uriResult.Scheme == Uri.UriSchemeHttp || uriResult.Scheme == Uri.UriSchemeHttps;
            }
            return false;
        }

        /// <summary>
        /// IsValidBase64String
        /// </summary>
        /// <param name="base64String"></param>
        /// <returns></returns>
        public static byte[] IsValidBase64String(string base64String)
        {
            if (string.IsNullOrEmpty(base64String) || base64String.Length % 4 != 0)
            {
                return null;
            }
            try
            {
                return Convert.FromBase64String(base64String);
            }
            catch (FormatException)
            {
                return null;
            }
        }

        /// <summary>
        /// ConvertFromBase64String
        /// </summary>
        /// <param name="input"></param>
        /// <returns></returns>
        public static byte[] ConvertFromBase64String(string input)
        {
            return Crypt.ConvertFromBase64String (input);  
        }
    }
}
