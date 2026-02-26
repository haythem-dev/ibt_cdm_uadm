using System;
using System.Runtime.InteropServices;
using UadmCommon.Models;

namespace UadmCommon.Classes
{
    public enum UADM_CONNECTION_TYPE
    {
        UADM_CONNECTION_TYPE_NONE = 0,
        UADM_CONNECTION_TYPE_VOODOO_PRIMARY,
        UADM_CONNECTION_TYPE_VOODOO_SECONDARY,
        UADM_CONNECTION_TYPE_INFORMIX_PRIMARY,
        UADM_CONNECTION_TYPE_INFORMIX_SECONDARY
    };

    public class VoodooInterface
    {
        [DllImport("libscsvoodoo.dll")]
        private static extern long ViaEshuOpenEx(out int handle, string server, string serverProgram, int port);

        [DllImport("libscsvoodoo.dll")]
        private static extern long NmpCloseHandle(int handle);

        [DllImport("libscsvoodoo.dll")]
        private static extern long NmpPipeOpen(int handle, string pipeName);

        [DllImport("libscsvoodoo.dll")]
        private static extern long __SqlExecuteImm(string query);

        [DllImport("libscsvoodoo.dll", EntryPoint = "__SqlRead", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        private static extern long __SqlReadVerifyUser([MarshalAs(UnmanagedType.LPStr)] string query,
            IntPtr userId,
            IntPtr username,
            IntPtr password,
            IntPtr surname,
            IntPtr forename,
            IntPtr email,
            IntPtr ntuser,
            IntPtr active,
            IntPtr changePW,
            IntPtr lastLogin,
            IntPtr lastPWChange,
            IntPtr endPoint);

        // private members
        private int _pipeHandle = -1;
        private UadmLibEnvironment _uadmLibEnvironment = null;
        private UADM_CONNECTION_TYPE _uadmConnectionType = UADM_CONNECTION_TYPE.UADM_CONNECTION_TYPE_NONE;

        // public members

        public UadmUserData UadmUserData { get; set; } = new UadmUserData();  

        /// <summary>
        /// VoodooInterface
        /// </summary>
        public VoodooInterface()
        {
        }

        /// <summary>
        /// VoodooInterface
        /// </summary>
        ///<param name="uadmAppEnvironment"></param>
        public VoodooInterface(UadmLibEnvironment uadmAppEnvironment)
	    {
            Initialize(uadmAppEnvironment);
        }

        /// <summary>
        /// ~VoodooInterface
        /// </summary>
        ~VoodooInterface()
        {
            CloseConnection();
        }

        /// <summary>
        /// Reset
        /// </summary>
        public void Reset()
        {
            UadmUserData = new UadmUserData();
        }

        /// <summary>
        /// Initialize
        /// </summary>
        /// <param name="uadmAppEnvironment"></param>
        public void Initialize(UadmLibEnvironment uadmAppEnvironment)
        {
            _uadmLibEnvironment = uadmAppEnvironment;
        }

        /// <summary>
        /// OpenConnection
        /// </summary>
        /// <returns></returns>
        public bool OpenConnection()
        {
            if (_uadmLibEnvironment == null)
            {
                return false; // environment not initialized
            }
            if (_pipeHandle >= 0)
            {
                CloseConnection(); // already opened, close the existing connection first
            }

            if ((ViaEshuOpenEx(out _pipeHandle, _uadmLibEnvironment.PrimaryServer, _uadmLibEnvironment.VoodooPipeName, _uadmLibEnvironment.VoodooPipePort)) == 0)
            {
                _uadmConnectionType = UADM_CONNECTION_TYPE.UADM_CONNECTION_TYPE_VOODOO_PRIMARY; // connected to primary voodoo server
                return OpenDatabase(); // successfully opened
            }
            else if ((ViaEshuOpenEx(out _pipeHandle, _uadmLibEnvironment.SecondaryServer, _uadmLibEnvironment.VoodooPipeName, _uadmLibEnvironment.VoodooPipePort)) == 0)
            {
                _uadmConnectionType = UADM_CONNECTION_TYPE.UADM_CONNECTION_TYPE_VOODOO_SECONDARY; // connected to secondary voodoo server
                return OpenDatabase(); // successfully opened
            }
            else
            {
                // TODO: add informix connection types if needed
            }
            return false; // failed to open
        }

        /// <summary>
        /// CloseConnection
        /// </summary>
        /// <returns></returns>
        private bool CloseConnection()
        {
            bool ret = false;
            if ((_uadmConnectionType == UADM_CONNECTION_TYPE.UADM_CONNECTION_TYPE_VOODOO_PRIMARY) || (_uadmConnectionType == UADM_CONNECTION_TYPE.UADM_CONNECTION_TYPE_VOODOO_SECONDARY))
            {
                if (_pipeHandle >= 0)
                {
                    // close database first
                    CloseDatabase();
                    // have to call the NmpCloseHandle function to prevent memory leaks
                    long result = NmpCloseHandle(_pipeHandle);
                    if (result == 0)
                    {
                        ret = true;
                    }
                }
                else
                {
                    ret = true; // already closed or never opened
                }
            }
            else if ((_uadmConnectionType == UADM_CONNECTION_TYPE.UADM_CONNECTION_TYPE_INFORMIX_PRIMARY) || (_uadmConnectionType == UADM_CONNECTION_TYPE.UADM_CONNECTION_TYPE_INFORMIX_SECONDARY))
            {
                //TODO: add informix close handle logic if needed
            }

            _pipeHandle = -1;

            _uadmConnectionType = UADM_CONNECTION_TYPE.UADM_CONNECTION_TYPE_NONE; // reset connection type

            Reset(); // reset user data

            return ret;
        }

        /// <summary>
        /// OpenDatabase
        /// </summary>
        /// <returns></returns>
        private bool OpenDatabase()
        {
            if (((_uadmConnectionType != UADM_CONNECTION_TYPE.UADM_CONNECTION_TYPE_VOODOO_PRIMARY) && (_uadmConnectionType != UADM_CONNECTION_TYPE.UADM_CONNECTION_TYPE_VOODOO_SECONDARY)) || (_pipeHandle < 0) || (_uadmLibEnvironment == null))
            {
                // no voodoo connection established
                return false;
            }

            long result = 0;

            // use primary or secondary database
            string dbconnect = "database " + _uadmLibEnvironment.PrimaryDatabase;
            if (_uadmConnectionType == UADM_CONNECTION_TYPE.UADM_CONNECTION_TYPE_VOODOO_SECONDARY)
            {
                dbconnect = "database " + _uadmLibEnvironment.SecondaryDatabase;
            }

            // open database
            result = __SqlExecuteImm(dbconnect);
            if (result == 0)
            {
                // set isolation to prevent locks
                result = __SqlExecuteImm("set isolation to dirty read");
                if (result == 0)
                {
                    // in case of locks, set wait to 10 secs, before error will be raised
                    result = __SqlExecuteImm("SET LOCK MODE TO WAIT 10");
                    if (result == 0)
                    {
                        return true;
                    }
                }
            }
            return false;
        }

        /// <summary>
        /// CloseDatabase
        /// </summary>
        /// <returns></returns>
        private bool CloseDatabase()
        {
            return (__SqlExecuteImm("close database") == 0);
        }

        /// <summary>
        /// VerifyUser
        /// </summary>
        /// <param name="username"></param>
        /// <param name="password"></param>
        /// <returns></returns>
        public bool VerifyUser(string username, string password)
        {
            if ((_uadmConnectionType == UADM_CONNECTION_TYPE.UADM_CONNECTION_TYPE_NONE) || 
                (_pipeHandle < 0) || 
                (_uadmLibEnvironment == null) || 
                (string.IsNullOrEmpty(username) == true) || 
                (string.IsNullOrEmpty(password) == true))
            {
                return false; // no connection established or invalid parameters
            }

            bool ret = false;
            string strSQL;

            IntPtr userIdPtr = Marshal.AllocHGlobal(sizeof(long)); // for long
            IntPtr usernamePtr = Marshal.AllocHGlobal(21); // for char[21]
            IntPtr passwordPtr = Marshal.AllocHGlobal(9);  // for char[9]
            IntPtr surnamePtr = Marshal.AllocHGlobal(31); // for char[31]
            IntPtr forenamePtr = Marshal.AllocHGlobal(21); // for char[21]
            IntPtr emailPtr = Marshal.AllocHGlobal(51);   // for char[51]
            IntPtr ntuserPtr = Marshal.AllocHGlobal(51);  // for char[51]
            IntPtr activePtr = Marshal.AllocHGlobal(sizeof(short)); // for short
            IntPtr changePWPtr = Marshal.AllocHGlobal(sizeof(short)); // for short
            IntPtr lastLoginPtr = Marshal.AllocHGlobal(sizeof(long)); // for long
            IntPtr lastPWChangePtr = Marshal.AllocHGlobal(sizeof(long)); // for long

            strSQL = "SELECT userid, username, password, surname, forename, emailuser, ntuser, activ, changepw, lastlogin, lastpwchange FROM ppuser WHERE username='" + username + "'";

            // read mapping must be exact to the column sizes in the database
            long result = __SqlReadVerifyUser (strSQL, 
                userIdPtr,
                usernamePtr,
                passwordPtr,
                surnamePtr,
                forenamePtr,
                emailPtr,
                ntuserPtr,
                activePtr,
                changePWPtr,
                lastLoginPtr,
                lastPWChangePtr,
                IntPtr.Zero);
              
            if (result == 0)
            {
                string tmpPassword = Marshal.PtrToStringAnsi(passwordPtr)?.Trim();

                // user found, lets check the pwd
                if (Crypt.PasswordEncrypt(password)?.Equals(tmpPassword?.ToString()) == true)
                {
                    UadmUserData.UserName = Marshal.PtrToStringAnsi(usernamePtr)?.Trim();  
                    UadmUserData.FirstName = Marshal.PtrToStringAnsi(forenamePtr)?.Trim();
                    UadmUserData.LastName = Marshal.PtrToStringAnsi(surnamePtr)?.Trim();
                    UadmUserData.Email = Marshal.PtrToStringAnsi(emailPtr)?.Trim();
                    UadmUserData.DomainAccount = Marshal.PtrToStringAnsi(ntuserPtr)?.Trim();

                    ret = true;
                }
            }
            return ret;
        }

    }
}
