using System;
using System.IO;
using System.Text.Json;
using UadmCommon.Classes;

namespace UadmCommon
{
    public class AppSettings
    {
        private string _fileName = "";
        private StreamReader _streamReader = null;

        public AppSettings()
        {
        }

        public AppSettings(string fileName)
        {
            _fileName = fileName;
        }

        /// <summary>
        /// LoadFromFile
        /// </summary>
        /// <returns></returns>
        public T LoadFromFile<T> (string fileName = null) where T : class
        {
            try
            {
                if (fileName == null)
                    fileName = _fileName;

                if (string.IsNullOrEmpty(fileName))
                    throw new ArgumentException("AppSettings file name must be provided.");

                if (File.Exists(fileName) == false)
                {
                    fileName = Path.Combine(GetApplicationPath(), fileName);
                    if (File.Exists(fileName) == false)
                        throw new FileNotFoundException("AppSettings file not found.", fileName);
                }

                string jsonString = File.ReadAllText(fileName); 

                if (string.IsNullOrEmpty(jsonString))
                    throw new ArgumentException("AppSettings file is empty.");

                return JsonSerializer.Deserialize<T>(jsonString);
            }
            catch (TypeInitializationException ex)
            {
                UadmLogger.LogError(ex.InnerException?.Message);
            }
            catch (Exception ex)
            {
                UadmLogger.LogError(ex.InnerException?.Message);
            }

            return null;
        }

        /// <summary>
        /// Dispose
        /// </summary>
        public void Dispose()
        {
            CloseFile();
        }

        /// <summary>
        /// OpenFile
        /// </summary>
        /// <param name="fileName"></param>
        /// <returns></returns>
        public bool OpenFile(string fileName = null)
        {
            if (fileName == null)
                fileName = _fileName;

            _streamReader = new StreamReader(fileName);
            if (_streamReader == null)
                return false;

            return true;
        }

        /// <summary>
        /// CloseFile
        /// </summary>
        public void CloseFile()
        {
            if (_streamReader != null)
                _streamReader.Close();
        }

        /// <summary>
        /// ReadNextLineFields
        /// </summary>
        /// <returns></returns>
        public string[] ReadNextLineFields(char delimiter = ';')
        {
            string[] result = new string[] { };

            if (_streamReader == null)
                return result;

            if (IsEndOfFile() == true)
                return result;

            string line = _streamReader.ReadLine();
            if (string.IsNullOrEmpty(line))
                return result;

            result = line.Split(delimiter);

            return result;
        }

        /// <summary>
        /// IsEndOfFile
        /// </summary>
        /// <returns></returns>
        public bool IsEndOfFile()
        {
            return _streamReader.EndOfStream;
        }

        /// <summary>
        /// GetApplicationPath
        /// </summary>
        /// <returns></returns>
        public string GetApplicationPath()
        {
            return AppDomain.CurrentDomain.BaseDirectory;
        }
    }
}
