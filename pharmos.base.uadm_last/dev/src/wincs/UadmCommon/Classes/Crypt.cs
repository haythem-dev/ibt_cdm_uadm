using System;
using System.IO;
using System.Security.Cryptography;
using System.Text;

namespace UadmCommon.Classes
{
    public class Crypt
    {
        private static readonly string _key = "fdsjlk43509f99ßjdsfkjkds3249AHG";

        /// <summary>
        /// Encrypt
        /// </summary>
        /// <param name="userId"></param>
        /// <param name="data"></param>
        /// <param name="key"></param>
        /// <param name="useRandomIV"></param>
        /// <returns></returns>
        public static byte[] Encrypt(string userId, string data, string key = null, bool useRandomIV = false)
        {
            if (key == null)
            {
                key = _key;
            }

            if (string.IsNullOrEmpty(data) || string.IsNullOrEmpty(key))
                return null;

            // Static IV (16 bytes for AES)
            byte[] iv = GetIV(userId); // Example static IV, must be 16 bytes

            // Ensure key is 32 bytes (256 bits) for AES-256
            byte[] keyBytes = new byte[32];
            byte[] inputKeyBytes = Encoding.UTF8.GetBytes(key);
            Array.Copy(inputKeyBytes, keyBytes, Math.Min(keyBytes.Length, inputKeyBytes.Length));

            using (Aes aes = Aes.Create())
            {
                aes.Key = keyBytes;
                aes.Mode = CipherMode.CBC;
                aes.Padding = PaddingMode.PKCS7;

                // Generate a random IV

                if (useRandomIV)
                {
                    aes.GenerateIV();
                    iv = aes.IV;
                }

                using (var encryptor = aes.CreateEncryptor(aes.Key, iv))
                using (var ms = new MemoryStream())
                {
                    // Prepend IV to the output
                    ms.Write(iv, 0, iv.Length);

                    using (var cs = new CryptoStream(ms, encryptor, CryptoStreamMode.Write))
                    {
                        byte[] dataBytes = Encoding.UTF8.GetBytes(data);
                        cs.Write(dataBytes, 0, dataBytes.Length);
                        cs.FlushFinalBlock();
                    }

                    return ms.ToArray();
                }
            }
        }

        /// <summary>
        /// GetIV
        /// </summary>
        /// <param name="userId"></param>
        /// <returns></returns>
        private static byte[] GetIV(string userId)
        {
            if (string.IsNullOrEmpty(userId))
            {
                userId = "EncryptToBase64F";
            }
            else if (userId.Length < 16)
            {
                for (int i = userId.Length; i < 16; i++)
                {
                    userId = "0" + userId;
                }
            }

            // TODO: ONLY FOR DEMO PURPOSES, DO NOT USE A STATIC IV IN PRODUCTION
            //userId = "EncryptToBase64F";

            // Example static IV, must be 16 bytes
            return Encoding.UTF8.GetBytes(userId.Substring(0, 16)); 
        }

        /// <summary>
        /// EncryptToBase64
        /// </summary>
        /// <param name="userId"></param>
        /// <param name="data"></param>
        /// <param name="key"></param>
        /// <param name="useRandomIV"></param>
        /// <returns></returns>
        public static string EncryptToBase64(string userId, string data, string key = null, bool useRandomIV = false)
        {
            if (key == null)
            {
                key = _key;
            }

            if (string.IsNullOrEmpty(data) || string.IsNullOrEmpty(key))
                return string.Empty;

            // Static IV (16 bytes for AES)
            byte[] result = Encrypt(userId, data, key, useRandomIV);

            if (result == null || result.Length == 0)
                return string.Empty;

            return Convert.ToBase64String(result);
        }

        /// <summary>
        /// DecryptFromBase64
        /// </summary>
        /// <param name="base64Data"></param>
        /// <param name="key"></param>
        /// <returns></returns>
        /// <exception cref="ArgumentException"></exception>
        public static string DecryptFromBase64(string base64Data, string key = null)
        {
            if (key == null)
            {
                key = _key;
            }

            if (string.IsNullOrEmpty(base64Data) || string.IsNullOrEmpty(key))
                return string.Empty;

            byte[] allBytes = ConvertFromBase64String(base64Data);

            // AES block size is 16 bytes (128 bits)
            int ivLength = 16;
            if (allBytes.Length < ivLength)
                throw new ArgumentException("Invalid encrypted data.");

            byte[] iv = new byte[ivLength];
            Array.Copy(allBytes, 0, iv, 0, ivLength);

            byte[] cipherBytes = new byte[allBytes.Length - ivLength];
            Array.Copy(allBytes, ivLength, cipherBytes, 0, cipherBytes.Length);

            byte[] keyBytes = new byte[32];
            byte[] inputKeyBytes = Encoding.UTF8.GetBytes(key);
            Array.Copy(inputKeyBytes, keyBytes, Math.Min(keyBytes.Length, inputKeyBytes.Length));

            using (Aes aes = Aes.Create())
            {
                aes.Key = keyBytes;
                aes.IV = iv;
                aes.Mode = CipherMode.CBC;
                aes.Padding = PaddingMode.PKCS7;

                using (var decryptor = aes.CreateDecryptor(aes.Key, aes.IV))
                using (var ms = new MemoryStream(cipherBytes))
                using (var cs = new CryptoStream(ms, decryptor, CryptoStreamMode.Read))
                using (var reader = new StreamReader(cs, Encoding.UTF8))
                {
                    return reader.ReadToEnd();
                }
            }
        }

        /// <summary>
        /// ConvertFromBase64String
        /// </summary>
        /// <param name="input"></param>
        /// <returns></returns>
        public static byte[] ConvertFromBase64String(string input)
        {
            string correctedInput = input.Replace('-', '+').Replace('_', '/').Replace(' ', '+');
   
            // .Net core 2.1 bug
            // https://github.com/dotnet/corefx/issues/30793
            try
            {
                return Convert.FromBase64String(correctedInput);
            }
            catch (Exception) { }
            try
            {
                return Convert.FromBase64String(correctedInput + "=");
            }
            catch (Exception) { }
            try
            {
                return Convert.FromBase64String(correctedInput + "==");
            }
            catch (Exception) { }
            try
            {
                return Convert.FromBase64String(correctedInput + "===");
            }
            catch (Exception) { }

            return null;            
        }

        /// <summary>
        /// PasswordEncrypt
        /// </summary>
        /// <param name="data"></param>
        /// <returns></returns>
        public static string PasswordEncrypt(string data)
        {
            int clng = 8;
            int NOCHAR_1 = 127;
            int NOCHAR_2 = '"';
            int NOCHAR_3 = '\'';
            int[] perm = { 2, 7, 1, 5, 3, 0, 4, 6 };
            int PERM_ANZ = perm.Length;
            byte[] offs = { 0x1C, 0x08, 0x1B, 0x05, 0x17, 0x0D, 0x13, 0x09 };
            int OFFS_ANZ = offs.Length;
            char[] crypt = new char[clng];

            // Pad data to length 8 with spaces
            if (data == null) data = "";
            while (data.Length < clng)
                data += " ";
            string orig = data;

            for (int i = 0; i < clng; i++)
            {
                int j = perm[i % PERM_ANZ];
                char origChar = orig[j];
                if (origChar == NOCHAR_1 || origChar == NOCHAR_2 || origChar == NOCHAR_3)
                {
                    crypt[i] = origChar;
                }
                else
                {
                    char c = (char)((origChar ^ offs[i % OFFS_ANZ]) & 0x7F);
                    if (c == NOCHAR_1 || c == NOCHAR_2 || c == NOCHAR_3)
                    {
                        crypt[i] = origChar;
                    }
                    else
                    {
                        crypt[i] = c;
                    }
                }
            }

            for (int i = 0; i < clng; i++)
            {
                char c = crypt[i];
                if (char.IsUpper(c) && ((c & 0x01) != 0))
                {
                    crypt[i] = char.ToLower(c);
                }
                else if (char.IsLower(c) && ((c & 0x01) != 0))
                {
                    crypt[i] = char.ToUpper(c);
                }
            }

            return new string(crypt);
        }
    }
}
