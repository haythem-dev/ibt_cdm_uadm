using log4net;
using log4net.Config;
using System.IO;

namespace UadmCommon.Classes
{
    public class UadmLogger
    {
        private static readonly ILog _Logger = LogManager.GetLogger(typeof(UadmLogger));

        static UadmLogger()
        {
            if (!LogManager.GetRepository().Configured)
            {
                string log4NetConfigFile = WebConfig.GetLog4NetConfigFile();

                if (!string.IsNullOrEmpty(log4NetConfigFile) && File.Exists(log4NetConfigFile))
                {
                    XmlConfigurator.Configure(new FileInfo(log4NetConfigFile));
                }
                else if (File.Exists("log4net.config"))
                {
                    XmlConfigurator.Configure(new FileInfo ("log4net.config"));
                }
                else if (File.Exists(@"c:\log\log4net.config"))
                {
                    XmlConfigurator.Configure(new FileInfo(@"c:\log\log4net.config"));
                }
            }
        }

        /// <summary>
        /// LogInfo
        /// </summary>
        /// <param name="message"></param>
        public static void LogInfo(string message)
        {
            _Logger.Info(message);
        }

        /// <summary>
        /// LogWarning
        /// </summary>
        /// <param name="message"></param>
        public static void LogWarning(string message)
        {
            _Logger.Warn(message);
        }

        /// <summary>
        /// LogError
        /// </summary>
        /// <param name="message"></param>
        public static void LogError(string message)
        {
            _Logger.Error(message);
        }

        /// <summary>
        /// LogDebug
        /// </summary>
        /// <param name="message"></param>
        public static void LogDebug(string message)
        {
            _Logger.Debug(message);
        }

        /// <summary>
        /// LogTrace
        /// </summary>
        /// <param name="message"></param>
        public static void LogTrace(string message)
        {
            _Logger.Debug(message); // Log4net does not have a Trace level, so use Debug.
        }
    }
}
