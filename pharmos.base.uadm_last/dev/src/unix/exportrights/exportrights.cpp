// exportrights.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <libscsvoodoo/libscsvoodoo.h>
#include <chrono>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <utility>
#include "exportrights.h"

#define SEPARATOR ';'
#define UADM_MODE_EXPORT_APPLICATIONS 0
#define UADM_MODE_EXPORT_PERMISSIONS  1
#define UADM_MODE_EXPORT_APPLICATION_ROLES 2

#define ERR_DB_CONNECTION   1
#define ERR_SQL             2
#define ERR_OUTPUT_CSV_OPEN 3
#define ERR_APP_ARGUMENTS   9

CExportRights::CExportRights()
{
    // constructor
    m_pipConnection = NULL;
    m_strAppName = "";
    m_strServer = "";
    m_strDBName = "";
    m_strServerName = "";
    m_strServiceName = "";
    m_intPort = 0;
    m_intExportMode = UADM_MODE_EXPORT_APPLICATIONS; // default
    m_strAreaID = "";
    m_strCountry = "";
    m_strApplicationID = "";
    m_strGroupID = "";
    m_strUserID = "";
    m_strCSVFile = "";
    m_strLogFile = "";
    m_strValidUserFile = "";
    m_bGroupsOnly = false;
    m_strMainframePermissionReport = "";
    m_strOrderEntryPermissionReport = "";
}

CExportRights::~CExportRights()
{
    Reset();
}

void CExportRights::Reset()
{
    // Delete all elements in vector m_vecPermissions
    for (CsvRowPermission* ptr1 : m_vecPermissions)
    {
        delete ptr1;
    }
    m_vecPermissions.clear();

    // Delete all elements in set m_setApplications
    for (CsvRowApplication* ptr2 : m_setApplications)
    {
        delete ptr2;
    }
    m_setApplications.clear();

    // Delete all elements in user, group and application and its relations
    for (PPUSER* pUser : m_vecUsers)
    {
        delete pUser;
    }
    m_vecUsers.clear();
    for (PPGROUP* pGroup : m_vecUsergroups)
    {
        delete pGroup;
    }
    m_vecUsergroups.clear();
    for (PPAPPLICATION* pApp : m_vecApplications)
    {
        delete pApp;
    }
    m_vecApplications.clear();
    for (PPREGIONBRANCH* pReg : m_vecRegionBranches)
    {
        delete pReg;
    }
    m_vecRegionBranches.clear();
    for (PPCORAPPUSERGROUP* pRel : m_vecGroupUserAppRelations)
    {
        delete pRel;
    }
    m_vecGroupUserAppRelations.clear();
    for (PPAPPRIGHT* pAppRel : m_vecAppRightRelations)
    {
        delete pAppRel;
    }
    m_vecAppRightRelations.clear();

    for (PPAPPMODULE* pMod : m_vecModules)
    {
        delete pMod;
    }
    m_vecModules.clear();
    for (PPAPPMODRIGHT* pModRight : m_vecModRights)
    {
        delete pModRight;
    }
    m_vecModRights.clear();
}

bool CExportRights::Init(int argc, char* argv[])
{
    // cnt = 0 is the application name only
    int cnt = 1;
    std::string param = "";
    std::string value = "";
    // get the app name 
    m_strAppName = argv[0];
    // windows
    size_t pos = m_strAppName.find_last_of('\\');
    if (pos != std::string::npos)
    {
        m_strAppName = m_strAppName.substr(pos + 1);
    }
    //unix
    pos = m_strAppName.find_last_of('/');
    if (pos != std::string::npos)
    {
        m_strAppName = m_strAppName.substr(pos + 1);
    }
    // check other params
    while (cnt < argc)
    {
        param = argv[cnt];
        cnt++;
        value = "";
        if (cnt < argc)
        {
            value = argv[cnt];
            cnt++;
        }
        if (param.compare("-server") == 0)
        {
            m_strServer = value;
        }
        if (param.compare("-db") == 0)
        {
            m_strDBName = value;
        }
        else if (param.compare("-csvfile") == 0)
        {
            m_strCSVFile = value;
        }
        else if (param.compare("-mode") == 0)
        {
            if (value.compare("1") == 0)
            {
                m_intExportMode = UADM_MODE_EXPORT_PERMISSIONS;
            }
            else if (value.compare("2") == 0)
            {
                m_intExportMode = UADM_MODE_EXPORT_APPLICATION_ROLES;
			}
            else
            {
                m_intExportMode = UADM_MODE_EXPORT_APPLICATIONS;
            }
        }
        else if (param.compare("-area") == 0)
        {
            m_strAreaID = value;
        }
        else if (param.compare("-country") == 0)
        {
            m_strCountry = value;
		}   
        else if (param.compare("-app") == 0)
        {
            m_strApplicationID = value;
        }
        else if (param.compare("-group") == 0)
        {
            m_strGroupID = value;
        }
        else if (param.compare("-go") == 0)
        {
            m_bGroupsOnly = true; // ignore value
		}
        else if (param.compare("-user") == 0)
        {
            m_strUserID = value;
        }
        else if (param.compare("-vu") == 0)
        {
            m_strValidUserFile = value;
        }
        else if (param.compare("-mpr") == 0)
        {
            m_strMainframePermissionReport = value;
        }
        else if (param.compare("-oepr") == 0)
        {
            m_strOrderEntryPermissionReport = value;
        }
    }
    if ((m_strDBName.empty() == true) || (m_strCSVFile.empty() == true))
    {
        // mandatory parameters missing
        return false;
    }

    // Default DATA folder. If no path is specified, use the DATA_PATH environment variable as the path of the file
    char* dataPath = std::getenv("DATA_PATH");

    // Check if m_strCSVFile contains both path and filename
    if ((m_strCSVFile.rfind('/') == std::string::npos) && (m_strCSVFile.rfind('\\') == std::string::npos) && (dataPath != NULL))
    {
        m_strCSVFile = std::string(dataPath) + '/' + m_strCSVFile; // Append the filename to the DATA_PATH
    }
    // Check if m_strValidUserIDFile contains both path and filename
    if (!m_strValidUserFile.empty() && (m_strValidUserFile.rfind('/') == std::string::npos) && (m_strValidUserFile.rfind('\\') == std::string::npos) && (dataPath != NULL))
    {
        m_strValidUserFile = std::string(dataPath) + '/' + m_strValidUserFile;  // Append the filename to the DATA_PATH
    }
    // Check if m_strMainframePermissionReport contains both path and filename
    if (!m_strMainframePermissionReport.empty() && (m_strMainframePermissionReport.rfind('/') == std::string::npos) && (m_strMainframePermissionReport.rfind('\\') == std::string::npos) && (dataPath != NULL))
    {
        m_strMainframePermissionReport = std::string(dataPath) + '/' + m_strMainframePermissionReport;  // Append the filename to the DATA_PATH
    }
    // Check if m_strOrderEntryPermissionReport contains both path and filename
    if (!m_strOrderEntryPermissionReport.empty() && (m_strOrderEntryPermissionReport.rfind('/') == std::string::npos) && (m_strOrderEntryPermissionReport.rfind('\\') == std::string::npos) && (dataPath != NULL))
    {
        m_strOrderEntryPermissionReport = std::string(dataPath) + '/' + m_strOrderEntryPermissionReport;  // Append the filename to the DATA_PATH
    }
    // set logfile settings
    char* logPath = std::getenv("LOG_PATH");
    if (logPath != NULL)
    {
        // absolute path
        m_strLogFile = std::string(logPath) + "/" + m_strAppName + ".log";
    }
    else
    {
        // relative path
        m_strLogFile = m_strAppName + ".log";
    }

    return true;
}

void CExportRights::Usage()
{
    // print out the usage parameters for the app
    printf("Usage: %s -db <db>   -csvfile <file> [-mode <mode>] [-area <areaid>] [-app <appid>] [-group <groupid>] [-user <userid>] [-vu <file>] [-mpr <file>] [-oepr <file>] \n", m_strAppName.c_str());
    printf("  -server <server>   -> the database name to connect to\n");
    printf("  -db <db>           -> the database name to connect to\n");
    printf("  -csvfile <file>    -> the export file, if provided without a path, the DATA_PATH variable will be used\n");
    printf("  -mode <mode>       -> export mode (0=applications, 1=permissions, 2=roles)\n");
    printf("  -area <areaid>     -> optional parameter to filter to a specific area (e.g. branch)\n");
	printf("  -country <country> -> optional parameter to filter to a specific country\n");
    printf("  -app <appid>       -> optional parameter to filter to a specific application\n");
    printf("  -group <groupid>   -> optional parameter to filter to a specific usergroup\n");
    printf("  -user <userid>     -> optional parameter to filter to a specific user\n");
    printf("  -vu <file>         -> optional parameter to show user validity (Yes/No) based on input CSV file\n");
    printf("  -mpr <file>        -> optional parameter. Mainframe Permission Report CSV file to merge (only for mode 0)\n");
    printf("  -oepr <file>       -> optional parameter. CSC Order Entry Permission Report CSV file to merge (only for mode 0)\n");
}

bool CExportRights::WriteLogFile(std::string log)
{
    FILE* pFile = fopen(m_strLogFile.c_str(), "a");

    if (pFile == NULL)
        return false;

    fputs(log.c_str(), pFile);
    fputc('\n', pFile);

    fclose(pFile);

    printf("%s\n", log.c_str());

    return true;
}

bool CExportRights::OpenDatabase()
{
    long lngStatus = 0;
    std::string strError = "";

#if defined(WIN32)
    //for test connect to voodoo server pipe via server, service and port
    m_strServerName = "zdev21";
    if (m_strServer.empty() == false)
    {
        m_strServerName = m_strServer;
	}
    m_strServiceName = "usradmuiserv";
    m_intPort = 58002;
    lngStatus = ViaEshuOpenEx(&m_pipConnection, m_strServerName.c_str(), m_strServiceName.c_str(), m_intPort);
    if (lngStatus != 0)
    {
        strError = "pipe " + m_strServiceName+ "@" + m_strServerName + ": " + std::string(DynErrmld());
        WriteLogFile(strError);
        return false;
    }
#else
#endif

    // connect to defined database
    std::string strConn = "database " + m_strDBName;

    lngStatus = SqlExecuteImm(strConn.c_str());
    if (lngStatus != 0)
    {
        strError = strConn + ": " + std::string(DynErrmld());
        WriteLogFile(strError);
        return false;
    }

    lngStatus = SqlExecuteImm("set isolation to dirty read");
    if (lngStatus == SQL_SETISOLA_ERROR)
        lngStatus = 0; // ignore set isola error
    if (lngStatus != 0)
    {
        strError = "set isolation: " + std::string(DynErrmld());
        WriteLogFile(strError);
        CloseDatabase();
        return false;
    }

    lngStatus = SqlExecuteImm("SET LOCK MODE TO WAIT 10");
    if (lngStatus == SQL_SETISOLA_ERROR)
        lngStatus = 0; // ignore set isola error
    if (lngStatus != 0)
    {
        strError = "set lock mode: " + std::string(DynErrmld());
        WriteLogFile(strError);
        CloseDatabase();
        return false;
    }
    return true;
}

bool CExportRights::CloseDatabase()
{
    SqlExecuteImm("close database");
#if defined(WIN32)
    // for test, close pipe
    NmpCloseHandle(m_pipConnection);
#else
#endif
    return true;
}

int CExportRights::Run()
{
    int          ret = 0;
    long         sqlStatus = 0;
    char         sqlErr[1000];

    // Open DB connection
    if (OpenDatabase() == false)
    {
        WriteLogFile("No DB Connection established, abort program.");
        return ERR_DB_CONNECTION;
    }
    WriteLogFile("DB Connection to " + m_strDBName + "@" + m_strServerName + " successful");

    #ifdef _DEBUG
    auto startTime = std::chrono::high_resolution_clock::now();
    #endif

    // Read valid users csv file
    if (!m_strValidUserFile.empty())
    {
        if (m_validUsers.ReadCSV(m_strValidUserFile) == ERR_INPUT_CSV_OPEN)
        {
            WriteLogFile("Error opening file:" + std::string(m_strValidUserFile));
        }
    }

    // Export CSV file for selected mode
    if (m_intExportMode == UADM_MODE_EXPORT_PERMISSIONS)
    {
        // Read Permissions mode data (from database to vectors) and fill m_vecPermissions
        sqlStatus = ReadPermissionsData();
        if (sqlStatus < SQL_SUCCESS)
        {
            std::strcpy(sqlErr, DynErrmld());
            WriteLogFile("SQL Query Error: " + std::string(sqlErr));
            ret = ERR_SQL;
        }
        else
        {
            // Create CSV file with rows from m_vecPermissions
            ret = CreateCSVFilePermissions();
            if (ret == 0)
            {
                WriteLogFile("export " + std::to_string(m_vecPermissions.size()) + " user permission rows from UADM into csvfile " + m_strCSVFile);
            }
        }
    }
    else if (m_intExportMode == UADM_MODE_EXPORT_APPLICATION_ROLES)
    {
        if ((m_strAreaID.empty() && m_strCountry.empty()) || m_strApplicationID.empty())
        {
            WriteLogFile("Error: Missing mandatory parameters for export application roles mode. Please provide -area and -app parameters.");
            return ERR_APP_ARGUMENTS;
		}
        sqlStatus = ReadRolesData();
        if (sqlStatus < SQL_SUCCESS)
        {
            std::strcpy(sqlErr, DynErrmld());
            WriteLogFile("SQL Query Error: " + std::string(sqlErr));
            ret = ERR_SQL;
        }
        else
        {
            // Create CSV file with rows from m_vecPermissions
            ret = CreateCSVFileRoles();
            if (ret == 0)
            {
                WriteLogFile("export " + std::to_string(m_vecPermissions.size()) + " user role rows from UADM into csvfile " + m_strCSVFile);
            }
		}
    }
    else //UADM_MODE_EXPORT_APPLICATIONS
    {
        // Read Apllications mode data (from database to vectors) and fill m_setApplications
        sqlStatus = ReadApplicationsData();
        if (sqlStatus < SQL_SUCCESS)
        {
            std::strcpy(sqlErr, DynErrmld());
            WriteLogFile("SQL Query Error: " + std::string(sqlErr));
            ret = ERR_SQL;
        }
        else
        {
            // Create the CSV file with rows from m_setApplications
            ret = CreateCSVFileApplications();
            if (ret == 0)
            {
                WriteLogFile("export " + std::to_string(m_setApplications.size()) + " user application rows from UADM into csvfile " + m_strCSVFile);
            }
        }
    }

    // Close the db connection
    CloseDatabase();

    #ifdef _DEBUG
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
    WriteLogFile("CSV generation duration: " + std::to_string(duration / 1000 ) + " s");
    #endif

    return ret;
}

long CExportRights::ReadCommonData()
{
    long         sqlStatus = 0;
    int          sqlHandle;
    std::string  sqlQuery;

    if ((m_bGroupsOnly == false) && (m_intExportMode != UADM_MODE_EXPORT_APPLICATION_ROLES))
    {
        // read all users
        sqlQuery = "SELECT CAST(ppu.userid as SMALLINT) as userid, ppu.username, ppu.forename, ppu.surname, ppu.emailuser, ppu.activ, ut.name FROM ppuser ppu INNER JOIN ppusertype ut on ut.usertypeid = ppu.usertypeid ";
        if (m_strUserID.empty() == false) { sqlQuery += "WHERE userid in (" + m_strUserID + ")"; }
        sqlStatus = SqlOpenCs(&sqlHandle, sqlQuery.c_str());
        if (sqlStatus == SQL_SUCCESS)
        {
            do  // loop result
            {
                PPUSER* pUser = new PPUSER();
                sqlStatus = SqlFetch(sqlHandle, 1, &pUser->userid, &pUser->username, &pUser->forename, &pUser->surname, &pUser->emailuser, &pUser->active, &pUser->usertype, NULL);
                if (sqlStatus == SQL_SUCCESS) { m_vecUsers.push_back(pUser); }
                else { delete pUser; }
            } while (sqlStatus == SQL_SUCCESS);
        }
        else { return sqlStatus; }
        SqlCloseCs(sqlHandle);
        WriteLogFile("Found " + std::to_string(m_vecUsers.size()) + " users");
    }

    // read all groups
    sqlQuery = "SELECT CAST(groupid as SMALLINT) as groupid, name FROM ppgroup ";
    if (m_strGroupID.empty() == false) { sqlQuery += "WHERE groupid in (" + m_strGroupID + ")"; }
    sqlStatus = SqlOpenCs(&sqlHandle, sqlQuery.c_str());
    if (sqlStatus == SQL_SUCCESS)
    {
        do  // loop result
        {
            PPGROUP* pGroup = new PPGROUP();
            sqlStatus = SqlFetch(sqlHandle, 1, &pGroup->groupid, &pGroup->name, NULL);
            if (sqlStatus == SQL_SUCCESS) { m_vecUsergroups.push_back(pGroup); }
            else { delete pGroup; }
        } while (sqlStatus == SQL_SUCCESS);
    }
    else { return sqlStatus; }
    SqlCloseCs(sqlHandle);
    WriteLogFile("Found " + std::to_string(m_vecUsergroups.size()) + " usergroups");

    // read all appliations
    sqlQuery = "SELECT CAST(applicationid as SMALLINT) as applicationid, name FROM ppapplication ";
    if (m_strApplicationID.empty() == false) { sqlQuery += "WHERE applicationid in (" + m_strApplicationID + ")"; }
    sqlStatus = SqlOpenCs(&sqlHandle, sqlQuery.c_str());
    if (sqlStatus == SQL_SUCCESS)
    {
        do  // loop result
        {
            PPAPPLICATION* pApp = new PPAPPLICATION();
            sqlStatus = SqlFetch(sqlHandle, 1, &pApp->appid, &pApp->name, NULL);
            if (sqlStatus == SQL_SUCCESS) { m_vecApplications.push_back(pApp); }
            else { delete pApp; }
        } while (sqlStatus == SQL_SUCCESS);
    }
    else { return sqlStatus; }
    SqlCloseCs(sqlHandle);
    WriteLogFile("Found " + std::to_string(m_vecApplications.size()) + " applications");

    if (m_strCountry.empty() == false)
    {
        // read all regionbranches
        sqlQuery = "SELECT CAST(regionbranchid as SMALLINT), CAST(name as CHAR(30)) FROM ppregionbranch WHERE country ='" + m_strCountry + "'";
        sqlStatus = SqlOpenCs(&sqlHandle, sqlQuery.c_str());
        if (sqlStatus == SQL_SUCCESS)
        {
            do  // loop result
            {
                PPREGIONBRANCH* pReg = new PPREGIONBRANCH();
                sqlStatus = SqlFetch(sqlHandle, 1, &pReg->regionbranchid, &pReg->name, NULL);
                if (sqlStatus == SQL_SUCCESS) { m_vecRegionBranches.push_back(pReg); }
                else { delete pReg; }
            } while (sqlStatus == SQL_SUCCESS);
        }
        else { return sqlStatus; }
        SqlCloseCs(sqlHandle);
        WriteLogFile("Found " + std::to_string(m_vecRegionBranches.size()) + " regions and branches for country " + m_strCountry);
    }
    else
    {
        // read all regionbranches
        sqlQuery = "SELECT regionid, name FROM ppregion ";
        if (m_strAreaID.empty() == false) { sqlQuery += "WHERE regionid in (" + m_strAreaID + ")"; }
        sqlQuery += " UNION SELECT branchid, name FROM ppbranch ";
        if (m_strAreaID.empty() == false) { sqlQuery += "WHERE branchid in (" + m_strAreaID + ")"; }
        sqlStatus = SqlOpenCs(&sqlHandle, sqlQuery.c_str());
        if (sqlStatus == SQL_SUCCESS)
        {
            do  // loop result
            {
                PPREGIONBRANCH* pReg = new PPREGIONBRANCH();
                sqlStatus = SqlFetch(sqlHandle, 1, &pReg->regionbranchid, &pReg->name, NULL);
                if (sqlStatus == SQL_SUCCESS) { m_vecRegionBranches.push_back(pReg); }
                else { delete pReg; }
            } while (sqlStatus == SQL_SUCCESS);
        }
        else { return sqlStatus; }
        SqlCloseCs(sqlHandle);
        WriteLogFile("Found " + std::to_string(m_vecRegionBranches.size()) + " regions and branches");
    }
    if (m_intExportMode != UADM_MODE_EXPORT_APPLICATION_ROLES)
    {
        if (m_bGroupsOnly == false)
        {
            // read all group2user2app
            sqlQuery = "SELECT groupid, userid, applicationid FROM ppcorappusergroup WHERE 1 = 1";
            if (m_strApplicationID.empty() == false) { sqlQuery += " AND applicationid in (" + m_strApplicationID + ")"; }
            if (m_strUserID.empty() == false) { sqlQuery += " AND userid in (" + m_strUserID + ")"; }
            if (m_strGroupID.empty() == false) { sqlQuery += " AND groupid in (" + m_strGroupID + ")"; }
            sqlStatus = SqlOpenCs(&sqlHandle, sqlQuery.c_str());
            if (sqlStatus == SQL_SUCCESS)
            {
                do  // loop result
                {
                    PPCORAPPUSERGROUP* pRel = new PPCORAPPUSERGROUP();
                    sqlStatus = SqlFetch(sqlHandle, 1, &pRel->groupid, &pRel->userid, &pRel->appid, NULL);
                    if (sqlStatus == SQL_SUCCESS) { m_vecGroupUserAppRelations.push_back(pRel); }
                    else { delete pRel; }
                } while (sqlStatus == SQL_SUCCESS);
            }
            else { return sqlStatus; }
            SqlCloseCs(sqlHandle);
            WriteLogFile("Found " + std::to_string(m_vecGroupUserAppRelations.size()) + " group2user2app relations");
        }
        else
        {
            // read all group2user2app
            sqlQuery = "SELECT DISTINCT groupid, 0, applicationid FROM ppcorappusergroup WHERE 1 = 1";
            if (m_strApplicationID.empty() == false) { sqlQuery += " AND applicationid in (" + m_strApplicationID + ")"; }
            if (m_strUserID.empty() == false) { sqlQuery += " AND userid in (" + m_strUserID + ")"; }
            if (m_strGroupID.empty() == false) { sqlQuery += " AND groupid in (" + m_strGroupID + ")"; }
            sqlStatus = SqlOpenCs(&sqlHandle, sqlQuery.c_str());
            if (sqlStatus == SQL_SUCCESS)
            {
                do  // loop result
                {
                    PPCORAPPUSERGROUP* pRel = new PPCORAPPUSERGROUP();
                    sqlStatus = SqlFetch(sqlHandle, 1, &pRel->groupid, &pRel->userid, &pRel->appid, NULL);
                    if (sqlStatus == SQL_SUCCESS) { m_vecGroupUserAppRelations.push_back(pRel); }
                    else { delete pRel; }
                } while (sqlStatus == SQL_SUCCESS);
            }
            else { return sqlStatus; }
            SqlCloseCs(sqlHandle);
            WriteLogFile("Found " + std::to_string(m_vecGroupUserAppRelations.size()) + " group2user2app relations");
        }
    }
    return sqlStatus;
}

long CExportRights::ReadPermissionsData()
{
    long         sqlStatus = 0;
    int          sqlHandle;
    std::string  sqlQuery;

    // Read common data (from database to vectors) for both modes
    Reset();
    WriteLogFile("Reading all UADM data...");
    sqlStatus = ReadCommonData();
    if(sqlStatus < SQL_SUCCESS) { return sqlStatus; }

    // read all modules
    sqlQuery = "SELECT applicationid, moduleid, name FROM ppappmodule ";
    if (m_strApplicationID.empty() == false) { sqlQuery += "WHERE applicationid in (" + m_strApplicationID + ")"; }
    sqlStatus = SqlOpenCs(&sqlHandle, sqlQuery.c_str());
    if (sqlStatus == SQL_SUCCESS)
    {
        do  // loop result
        {
            PPAPPMODULE* pMod = new PPAPPMODULE();
            sqlStatus = SqlFetch(sqlHandle, 1, &pMod->appid, &pMod->moduleid, &pMod->moduleName, NULL);
            if (sqlStatus == SQL_SUCCESS) { m_vecModules.push_back(pMod); }
            else { delete pMod; }
        } while (sqlStatus == SQL_SUCCESS);
    }
    else { return sqlStatus; }
    SqlCloseCs(sqlHandle);
    WriteLogFile("Found " + std::to_string(m_vecModules.size()) + " modules");

    // read all modules-rights
    sqlQuery = "SELECT applicationid, moduleid, rightname, description FROM ppappmodright ";
    if (m_strApplicationID.empty() == false) { sqlQuery += "WHERE applicationid in (" + m_strApplicationID + ")"; }
    sqlStatus = SqlOpenCs(&sqlHandle, sqlQuery.c_str());
    if (sqlStatus == SQL_SUCCESS)
    {
        do  // loop result
        {
            PPAPPMODRIGHT* pModRight = new PPAPPMODRIGHT();
            sqlStatus = SqlFetch(sqlHandle, 1, &pModRight->appid, &pModRight->moduleid, &pModRight->rightName, &pModRight->rightDescription, NULL);
            if (sqlStatus == SQL_SUCCESS) { m_vecModRights.push_back(pModRight); }
            else { delete pModRight; }
        } while (sqlStatus == SQL_SUCCESS);
    }
    else { return sqlStatus; }
    SqlCloseCs(sqlHandle);
    WriteLogFile("Found " + std::to_string(m_vecModules.size()) + " modules rights");

    // read specific area/region/branch/module/rightname based permissions
    sqlQuery = "SELECT applicationid, userid, groupid, CAST(branchregionid as SMALLINT) as branchregionid, moduleid, rightname FROM ppappright WHERE 1 = 1";
    if (m_strApplicationID.empty() == false) { sqlQuery += " AND applicationid in (" + m_strApplicationID + ")"; }
    if (m_strUserID.empty() == false) { sqlQuery += " AND userid in (" + m_strUserID + ")"; }
    if (m_strGroupID.empty() == false) { sqlQuery += " AND groupid in (" + m_strGroupID + ")"; }
	if (m_bGroupsOnly == true) { sqlQuery += " AND groupid > 0"; } // only group rights
    if (m_strAreaID.empty() == false) { sqlQuery += " AND branchregionid in (" + m_strAreaID + ")"; }
    sqlStatus = SqlOpenCs(&sqlHandle, sqlQuery.c_str());
    if (sqlStatus == SQL_SUCCESS)
    {
        do  // loop result
        {
            PPAPPRIGHT* pAppRel = new PPAPPRIGHT();
            sqlStatus = SqlFetch(sqlHandle, 1, &pAppRel->appid, &pAppRel->userid, &pAppRel->groupid, &pAppRel->branchregionid, &pAppRel->moduleid, &pAppRel->rightName, NULL);
            if (sqlStatus == SQL_SUCCESS) { m_vecAppRightRelations.push_back(pAppRel); }
            else { delete pAppRel; }
        } while (sqlStatus == SQL_SUCCESS);
    }
    else { return sqlStatus; }
    SqlCloseCs(sqlHandle);
    WriteLogFile("Found " + std::to_string(m_vecAppRightRelations.size()) + " application right relations");
	char dummy[40] = "                                       ";
    // now fill m_vecPermissions for CSV
    for (PPAPPRIGHT* pAppRight : m_vecAppRightRelations)
    {
        if (pAppRight->userid > 0)
        {
            if (getUsername(pAppRight->userid) != NULL)
            {
                // dedicated user right for the app
                CsvRowPermission* pRow = new CsvRowPermission();
                pRow->area_region_branch_id = pAppRight->branchregionid;
                savecpy(pRow->area_region_branch, getRegionBranchName(pAppRight->branchregionid), 31);
                pRow->groupid = pAppRight->groupid;
                savecpy(pRow->groupname, getUsergroupName(pAppRight->groupid), 31);
                pRow->user_id = pAppRight->userid;
                savecpy(pRow->username, getUsername(pAppRight->userid), 21);
                savecpy(pRow->forename, getForename(pAppRight->userid), 21);
                savecpy(pRow->surname, getSurname(pAppRight->userid), 31);
				savecpy(pRow->usertype, getUserType(pAppRight->userid), 151);
                pRow->activeUser = isUserActive(pAppRight->userid);
                pRow->applicationid = pAppRight->appid;
                savecpy(pRow->application, getApplicationName(pAppRight->appid), 31);
                savecpy(pRow->module, getModuleName(pAppRight->appid, pAppRight->moduleid), 31);
                savecpy(pRow->permission, pAppRight->rightName, 21);
                savecpy(pRow->permission_description, getRightDescription(pAppRight->appid, pAppRight->moduleid, pAppRight->rightName), 255);
                if (!m_strValidUserFile.empty())
                {
                    std::pair<std::string, std::string> fullname;
                    pRow->validUser = (m_validUsers.FindUserByUsername(pRow->username, fullname) || m_validUsers.IsUserFullnameValid(pRow->surname, pRow->forename));
                }
                m_vecPermissions.push_back(pRow);
            }
        }
        else
        {
            // group right for the app -> needs to be flatten into the user list
            for (PPCORAPPUSERGROUP* pRel : m_vecGroupUserAppRelations)
            {
                if ((pRel->groupid == pAppRight->groupid) && (pRel->appid == pAppRight->appid))
                {
                    if ((pRel->userid > 0) && (getUsername(pRel->userid) != NULL))
                    {
                        // dedicated group right for the app
                        CsvRowPermission* pRow = new CsvRowPermission();
                        pRow->area_region_branch_id = pAppRight->branchregionid;
                        savecpy(pRow->area_region_branch, getRegionBranchName(pAppRight->branchregionid), 31);
                        pRow->groupid = pAppRight->groupid;
                        savecpy(pRow->groupname, getUsergroupName(pAppRight->groupid), 31);
                        pRow->user_id = pRel->userid; // take it from the group relation
                        savecpy(pRow->username, getUsername(pRel->userid), 21); // take it from the group relation
                        savecpy(pRow->forename, getForename(pRel->userid), 21); // take it from the group relation
                        savecpy(pRow->surname, getSurname(pRel->userid), 31); // take it from the group relation
						savecpy(pRow->usertype, getUserType(pRel->userid), 151); // take it from the group relation
                        pRow->activeUser = isUserActive(pRel->userid); // take it from the group relation
                        pRow->applicationid = pAppRight->appid;
                        savecpy(pRow->application, getApplicationName(pAppRight->appid), 31);
                        savecpy(pRow->module, getModuleName(pAppRight->appid, pAppRight->moduleid), 31);
                        savecpy(pRow->permission, pAppRight->rightName, 21);
                        savecpy(pRow->permission_description, getRightDescription(pAppRight->appid, pAppRight->moduleid, pAppRight->rightName), 255);
                        if (!m_strValidUserFile.empty())
                        {
                            std::pair<std::string, std::string> fullname;
                            pRow->validUser = (m_validUsers.FindUserByUsername(pRow->username, fullname) || m_validUsers.IsUserFullnameValid(pRow->surname, pRow->forename));
                        }
                        m_vecPermissions.push_back(pRow);
                    }
                    else if ((pRel->userid == 0) && (m_bGroupsOnly))
                    {
                        // only group right, no user assigned -> create a dummy user with userid = 0
                        CsvRowPermission* pRow = new CsvRowPermission();
                        pRow->area_region_branch_id = pAppRight->branchregionid;
                        savecpy(pRow->area_region_branch, getRegionBranchName(pAppRight->branchregionid), 31);
                        pRow->groupid = pAppRight->groupid;
                        savecpy(pRow->groupname, getUsergroupName(pAppRight->groupid), 31);
                        pRow->user_id = 0; // dummy user
                        savecpy(pRow->username, dummy, 21); // no username
                        savecpy(pRow->forename, dummy, 21); // no forename
                        savecpy(pRow->surname, dummy, 31); // no surname
						savecpy(pRow->usertype, dummy, 151); // no usertype
                        pRow->activeUser = false; // no active user
                        pRow->applicationid = pAppRight->appid;
                        savecpy(pRow->application, getApplicationName(pAppRight->appid), 31);
                        savecpy(pRow->module, getModuleName(pAppRight->appid, pAppRight->moduleid), 31);
                        savecpy(pRow->permission, pAppRight->rightName, 21);
                        savecpy(pRow->permission_description, getRightDescription(pAppRight->appid, pAppRight->moduleid, pAppRight->rightName), 255);
                        //pRow->validUser = false; // no valid user
						m_vecPermissions.push_back(pRow);
                    }
                }
            }
        }
    }
    return sqlStatus;
}

long CExportRights::ReadApplicationsData()
{
    long         sqlStatus = 0;
    int          sqlHandle;
    std::string  sqlQuery;

    // Read common data (from database to vectors) for both modes
    Reset();
    WriteLogFile("Reading all UADM data...");
    sqlStatus = ReadCommonData();
    if (sqlStatus < SQL_SUCCESS) { return sqlStatus; }

    // read specific area/region/branch based permissions
    sqlQuery = "SELECT DISTINCT applicationid, userid, groupid, CAST(branchregionid as SMALLINT) as branchregionid FROM ppappright  WHERE 1 = 1";
    if (m_strApplicationID.empty() == false) { sqlQuery += " AND applicationid in (" + m_strApplicationID + ")"; }
    if (m_strUserID.empty() == false) { sqlQuery += " AND userid in (" + m_strUserID + ")"; }
    if (m_strGroupID.empty() == false) { sqlQuery += " AND groupid in (" + m_strGroupID + ")"; }
    if (m_strAreaID.empty() == false) { sqlQuery += " AND branchregionid in (" + m_strAreaID + ")"; }
    sqlStatus = SqlOpenCs(&sqlHandle, sqlQuery.c_str());
    if (sqlStatus == SQL_SUCCESS)
    {
        do  // loop result
        {
            PPAPPRIGHT* pAppRel = new PPAPPRIGHT();
            sqlStatus = SqlFetch(sqlHandle, 1, &pAppRel->appid, &pAppRel->userid, &pAppRel->groupid, &pAppRel->branchregionid, NULL);
            if (sqlStatus == SQL_SUCCESS) { m_vecAppRightRelations.push_back(pAppRel); }
            else { delete pAppRel; }
        } while (sqlStatus == SQL_SUCCESS);
    }
    else { return sqlStatus; }
    SqlCloseCs(sqlHandle);
    WriteLogFile("Found " + std::to_string(m_vecAppRightRelations.size()) + " application right relations");

    // now fill m_setApplications for CSV
    for (PPAPPRIGHT* pAppRight : m_vecAppRightRelations)
    {
        if (pAppRight->userid > 0)
        {
            if (getUsername(pAppRight->userid) != NULL)
            {
                // dedicated user right for the app
                CsvRowApplication* pRow = new CsvRowApplication();
                pRow->area_region_branch_id = pAppRight->branchregionid;
                savecpy(pRow->area_region_branch, getRegionBranchName(pAppRight->branchregionid), 31);
                pRow->groupid = pAppRight->groupid;
                savecpy(pRow->groupname, getUsergroupName(pAppRight->groupid), 31);
                pRow->user_id = pAppRight->userid;
                savecpy(pRow->username, getUsername(pAppRight->userid), 21);
                savecpy(pRow->forename, getForename(pAppRight->userid), 21);
                savecpy(pRow->surname, getSurname(pAppRight->userid), 31);
				savecpy(pRow->emailuser, getEMail(pAppRight->userid), 51);
				savecpy(pRow->usertype, getUserType(pAppRight->userid), 151);
                pRow->activeUser = isUserActive(pAppRight->userid);
                pRow->applicationid = pAppRight->appid;
                savecpy(pRow->application, getApplicationName(pAppRight->appid), 31);
                if (!m_strValidUserFile.empty())
                {
                    std::pair<std::string, std::string> fullname;
                    pRow->validUser = (m_validUsers.FindUserByUsername(pRow->username, fullname) || m_validUsers.IsUserFullnameValid(pRow->surname, pRow->forename));
                }
                m_setApplications.insert(pRow);
            }
        }
        else
        {
            // group right for the app -> needs to be flatten into the user list
            for (PPCORAPPUSERGROUP* pRel : m_vecGroupUserAppRelations)
            {
                if ((pRel->groupid == pAppRight->groupid) && (pRel->appid == pAppRight->appid))
                {
                    if ((pRel->userid > 0) && (getUsername(pRel->userid) != NULL))
                    {
                        // multiply the data row for each user
                        CsvRowApplication* pRow = new CsvRowApplication();
                        pRow->area_region_branch_id = pAppRight->branchregionid;
                        savecpy(pRow->area_region_branch, getRegionBranchName(pAppRight->branchregionid), 31);
                        pRow->groupid = pAppRight->groupid;
                        savecpy(pRow->groupname, getUsergroupName(pAppRight->groupid), 31);
                        pRow->user_id = pRel->userid; // take it from the group relation
                        savecpy(pRow->username, getUsername(pRel->userid), 21); // take it from the group relation
                        savecpy(pRow->forename, getForename(pRel->userid), 21); // take it from the group relation
                        savecpy(pRow->surname, getSurname(pRel->userid), 31); // take it from the group relation
						savecpy(pRow->emailuser, getEMail(pRel->userid), 51); // take it from the group relation
						savecpy(pRow->usertype, getUserType(pRel->userid), 151); // take it from the group relation
                        pRow->activeUser = isUserActive(pRel->userid); // take it from the group relation
                        pRow->applicationid = pAppRight->appid;
                        savecpy(pRow->application, getApplicationName(pAppRight->appid), 31);
                        if (!m_strValidUserFile.empty())
                        {
                            std::pair<std::string, std::string> fullname;
                            pRow->validUser = (m_validUsers.FindUserByUsername(pRow->username, fullname) || m_validUsers.IsUserFullnameValid(pRow->surname, pRow->forename));
                        }
                        m_setApplications.insert(pRow);
                    }
                }
            }
        }
    }
    return sqlStatus;
}

long CExportRights::ReadRolesData()
{
    long         sqlStatus = 0;
    int          sqlHandle;
    std::string  sqlQuery;

    // Read common data (from database to vectors) for all modes
    Reset();
    WriteLogFile("Reading all UADM data...");
    sqlStatus = ReadCommonData();
    if (sqlStatus < SQL_SUCCESS) { return sqlStatus; }

    // read all modules
    sqlQuery = "SELECT applicationid, moduleid, name FROM ppappmodule ";
    if (m_strApplicationID.empty() == false) { sqlQuery += "WHERE applicationid in (" + m_strApplicationID + ")"; }
    sqlStatus = SqlOpenCs(&sqlHandle, sqlQuery.c_str());
    if (sqlStatus == SQL_SUCCESS)
    {
        do  // loop result
        {
            PPAPPMODULE* pMod = new PPAPPMODULE();
            sqlStatus = SqlFetch(sqlHandle, 1, &pMod->appid, &pMod->moduleid, &pMod->moduleName, NULL);
            if (sqlStatus == SQL_SUCCESS) { m_vecModules.push_back(pMod); }
            else { delete pMod; }
        } while (sqlStatus == SQL_SUCCESS);
    }
    else { return sqlStatus; }
    SqlCloseCs(sqlHandle);
    WriteLogFile("Found " + std::to_string(m_vecModules.size()) + " modules");

    // read all modules-rights
    sqlQuery = "SELECT applicationid, moduleid, rightname, description FROM ppappmodright ";
    if (m_strApplicationID.empty() == false) { sqlQuery += "WHERE applicationid in (" + m_strApplicationID + ")"; }
    sqlStatus = SqlOpenCs(&sqlHandle, sqlQuery.c_str());
    if (sqlStatus == SQL_SUCCESS)
    {
        do  // loop result
        {
            PPAPPMODRIGHT* pModRight = new PPAPPMODRIGHT();
            sqlStatus = SqlFetch(sqlHandle, 1, &pModRight->appid, &pModRight->moduleid, &pModRight->rightName, &pModRight->rightDescription, NULL);
            if (sqlStatus == SQL_SUCCESS) { m_vecModRights.push_back(pModRight); }
            else { delete pModRight; }
        } while (sqlStatus == SQL_SUCCESS);
    }
    else { return sqlStatus; }
    SqlCloseCs(sqlHandle);
    WriteLogFile("Found " + std::to_string(m_vecModRights.size()) + " modules rights");

    // read specific application based roles for given country/area/region/branch
    sqlQuery = "SELECT applicationid, groupid, CAST(branchregionid as SMALLINT) as branchregionid, moduleid, rightname FROM ppappright  WHERE 1 = 1";
    if (m_strApplicationID.empty() == false) { sqlQuery += " AND applicationid in (" + m_strApplicationID + ")"; }
    if (m_strGroupID.empty() == false) { sqlQuery += " AND groupid in (" + m_strGroupID + ")"; }
    if (m_strAreaID.empty() == false) { sqlQuery += " AND branchregionid in (" + m_strAreaID + ")"; }
    if (m_strCountry.empty() == false)
    {
        sqlQuery += " AND branchregionid in (";
        std::vector<PPREGIONBRANCH*>::iterator it;
        for (it = m_vecRegionBranches.begin(); it != m_vecRegionBranches.end(); ++it)
        {
            sqlQuery += std::to_string((*it)->regionbranchid) + ",";
        }
        if (sqlQuery.back() == ',')
        {
			sqlQuery = sqlQuery.substr(0, sqlQuery.size() - 1);
        }
        sqlQuery += ")";
    }
    sqlQuery += " ORDER BY groupid, branchregionid, moduleid, rightname";
    sqlStatus = SqlOpenCs(&sqlHandle, sqlQuery.c_str());
    if (sqlStatus == SQL_SUCCESS)
    {
        do  // loop result
        {
            PPAPPRIGHT* pAppRel = new PPAPPRIGHT();
            sqlStatus = SqlFetch(sqlHandle, 1, &pAppRel->appid, &pAppRel->groupid, &pAppRel->branchregionid, &pAppRel->moduleid, &pAppRel->rightName, NULL);
            if (sqlStatus == SQL_SUCCESS) { m_vecAppRightRelations.push_back(pAppRel); }
            else { delete pAppRel; }
        } while (sqlStatus == SQL_SUCCESS);
    }
    else { return sqlStatus; }
    SqlCloseCs(sqlHandle);
    WriteLogFile("Found " + std::to_string(m_vecAppRightRelations.size()) + " application right relations");

    return sqlStatus;
}

char* CExportRights::getRegionBranchName(short regionbranchid)
{
    char* ret = NULL;
    if (regionbranchid > 0)
    {
        for (PPREGIONBRANCH* pReg : m_vecRegionBranches)
        {
            if (pReg->regionbranchid == regionbranchid)
            {
                return pReg->name;
            }
        }
    }
    return ret;
}

char* CExportRights::getUsergroupName(short groupid)
{
    char* ret = NULL;
    if (groupid > 0)
    {
        for (PPGROUP* pGroup : m_vecUsergroups)
        {
            if (pGroup->groupid == groupid)
            {
                return pGroup->name;
            }
        }
    }
    return ret;
}

char* CExportRights::getUsername(short userid)
{
    char* ret = NULL;
    if (userid > 0)
    {
        for (PPUSER* pUser : m_vecUsers)
        {
            if (pUser->userid == userid)
            {
                return pUser->username;
            }
        }
    }
    return ret;
}

char* CExportRights::getForename(short userid)
{
    char* ret = NULL;
    if (userid > 0)
    {
        for (PPUSER* pUser : m_vecUsers)
        {
            if (pUser->userid == userid)
            {
                return pUser->forename;
            }
        }
    }
    return ret;
}

char* CExportRights::getSurname(short userid)
{
    char* ret = NULL;
    if (userid > 0)
    {
        for (PPUSER* pUser : m_vecUsers)
        {
            if (pUser->userid == userid)
            {
                return pUser->surname;
            }
        }
    }
    return ret;
}

char* CExportRights::getEMail(short userid)
{
    char* ret = NULL;
    if (userid > 0)
    {
        for (PPUSER* pUser : m_vecUsers)
        {
            if (pUser->userid == userid)
            {
                return pUser->emailuser;
            }
        }
    }
    return ret;
}

char* CExportRights::getUserType(short userid)
{
    char* ret = NULL;
    if (userid > 0)
    {
        for (PPUSER* pUser : m_vecUsers)
        {
            if (pUser->userid == userid)
            {
                return pUser->usertype;
            }
        }
    }
    return ret;
}

bool  CExportRights::isUserActive(short userid)
{
    bool ret = false;
    if (userid > 0)
    {
        for (PPUSER* pUser : m_vecUsers)
        {
            if (pUser->userid == userid)
            {
                return (pUser->active == 1);
            }
        }
    }
    return ret;
}

char* CExportRights::getApplicationName(short appid)
{
    char* ret = NULL;
    if (appid > 0)
    {
        for (PPAPPLICATION* pApp : m_vecApplications)
        {
            if (pApp->appid == appid)
            {
                return pApp->name;
            }
        }
    }
    return ret;
}

char* CExportRights::getModuleName(short appid, short moduleid)
{
    char* ret = NULL;
    if (appid > 0 && moduleid > 0)
    {
        for (PPAPPMODULE* pMod : m_vecModules)
        {
            if (pMod->appid == appid && pMod->moduleid == moduleid)
            {
                return pMod->moduleName;
            }
        }
    }
    return ret;
}

char* CExportRights::getRightDescription(short appid, short moduleid, char* rightName)
{
    char* ret = NULL;
    if (appid > 0 && moduleid > 0 && rightName != NULL)
    {
        for (PPAPPMODRIGHT* pModRight : m_vecModRights)
        {
            if (pModRight->appid == appid && pModRight->moduleid == moduleid && strcmp(pModRight->rightName, rightName) == 0)
            {
                return pModRight->rightDescription;
            }
        }
    }
    return ret;
}


void CExportRights::savecpy(char* dest, char* source, int len)
{
    if ((dest != NULL) && (source != NULL))
    {
        strncpy(dest, source, len);
        for (int l = len; l > 0; l--)
        {
            if (dest[l] == ' ')
            {
                dest[l] = '\0';
            }
            else if (dest[l] != '\0')
            {
                break;
            }
        }
    }
}

long CExportRights::CreateCSVFilePermissions()
{
    // Create CSV file and write header row
    std::ofstream outputFile(m_strCSVFile.c_str(), std::ofstream::out);
    if (outputFile.is_open() == false)
    {
        return ERR_OUTPUT_CSV_OPEN; // Handle file open error
    }

    // Define a custom comparison function (lambda function)
    auto customCompare = [](const CsvRowPermission* ptra, const CsvRowPermission* ptrb)
    {
        // Compare based on surname, then forename, and finally username
        int surnameComparison = std::strcmp(ptra->surname, ptrb->surname);
        if (std::strcmp(ptra->surname, ptrb->surname) != 0)
        {
            return surnameComparison < 0;
        }
        int forenameComparison = std::strcmp(ptra->forename, ptrb->forename);
        if (forenameComparison != 0)
        {
            return forenameComparison < 0;
        }
        return std::strcmp(ptra->username, ptrb->username) < 0;
    };

    // Sort the vector
    std::sort(m_vecPermissions.begin(), m_vecPermissions.end(), customCompare);

    // write header row
    outputFile
        << "area/region/branch id" << SEPARATOR
        << "area/region/branch" << SEPARATOR
        << "groupid" << SEPARATOR
        << "groupname" << SEPARATOR
        << "user id" << SEPARATOR
        << "username" << SEPARATOR
        << "surname" << SEPARATOR
        << "forename" << SEPARATOR
		<< "usertype" << SEPARATOR
        << "application id" << SEPARATOR
        << "application" << SEPARATOR
        << "module" << SEPARATOR
        << "permission" << SEPARATOR
        << "permission description";

    if (!m_strValidUserFile.empty())
    {
        outputFile << SEPARATOR << "Valid user" << std::endl;
    }
    else
    {
        outputFile << std::endl;
    }

    // loop over the sorted list m_vecPermissions
    for (const CsvRowPermission* pRow : m_vecPermissions)
    {
        // Write the row data to the CSV file
        outputFile
            << pRow->area_region_branch_id << SEPARATOR
            << pRow->area_region_branch << SEPARATOR
            << pRow->groupid << SEPARATOR
            << pRow->groupname << SEPARATOR
            << pRow->user_id << SEPARATOR
            << pRow->username << SEPARATOR
            << pRow->surname << SEPARATOR
            << pRow->forename << SEPARATOR
			<< pRow->usertype << SEPARATOR
            << pRow->applicationid << SEPARATOR
            << pRow->application << SEPARATOR
            << pRow->module << SEPARATOR
            << pRow->permission << SEPARATOR
            << pRow->permission_description;

        if (!m_strValidUserFile.empty())
        {
            outputFile << SEPARATOR << (pRow->validUser ? "Yes" : "No") << std::endl;
        }
        else
        {
            outputFile << std::endl;
        }
    }

    // close File
    outputFile.close();

    return 0;
}

long CExportRights::CreateCSVFileRoles()
{
    long ret = 0;
    // Create CSV file and write header row
    std::ofstream outputFile(m_strCSVFile.c_str(), std::ofstream::out);
    if (outputFile.is_open() == false)
    {
        return ERR_OUTPUT_CSV_OPEN; // Handle file open error
    }

    // write header row
    outputFile
        << "application id" << SEPARATOR
        << "application" << SEPARATOR
        << "groupid" << SEPARATOR
        << "groupname" << SEPARATOR
        << "area/region/branch id" << SEPARATOR
        << "area/region/branch" << SEPARATOR;
    // generate for the app the dynamic list of module and rights as columns
	std::vector<PPAPPMODRIGHT*>::iterator itModRight;
    for (itModRight = m_vecModRights.begin(); itModRight != m_vecModRights.end(); ++itModRight)
    {
		std::string modName = getModuleName((*itModRight)->appid, (*itModRight)->moduleid);
        outputFile << (*itModRight)->moduleid << "-" << trim(modName) << " - " << trim((*itModRight)->rightName) << SEPARATOR;
	}
    outputFile << std::endl;

	// loop over the list m_vecAppRightRelations
	std::vector<PPAPPRIGHT*>::iterator itAppRight = m_vecAppRightRelations.begin();
    while (itAppRight != m_vecAppRightRelations.end())
    {
        if ((*itAppRight)->groupid == 0)
        {
            // skip user specific rights as we want only group based roles
            itAppRight = m_vecAppRightRelations.erase(itAppRight);
            continue;
		}
        // Write the row data to the CSV file
        outputFile
            << (*itAppRight)->appid << SEPARATOR
            << trim(getApplicationName((*itAppRight)->appid)) << SEPARATOR
            << (*itAppRight)->groupid << SEPARATOR
            << trim(getUsergroupName((*itAppRight)->groupid)) << SEPARATOR
            << (*itAppRight)->branchregionid << SEPARATOR
            << trim(getRegionBranchName((*itAppRight)->branchregionid)) << SEPARATOR;

        // for each module right column, fill it with "x" if the right is assigned to the group in the app, otherwise " "
        for (itModRight = m_vecModRights.begin(); itModRight != m_vecModRights.end(); ++itModRight)
        {
            bool hasRight = false;
            // check if the right is assigned to the group in the app
            std::vector<PPAPPRIGHT*>::iterator itAppRightCheck = m_vecAppRightRelations.begin();
            while ((hasRight == false) && (itAppRightCheck != m_vecAppRightRelations.end()))
            {
                if ((*itAppRightCheck)->appid == (*itAppRight)->appid &&
                    (*itAppRightCheck)->groupid == (*itAppRight)->groupid &&
                    (*itAppRightCheck)->branchregionid == (*itAppRight)->branchregionid &&
                    (*itAppRightCheck)->moduleid == (*itModRight)->moduleid &&
                    strcmp((*itAppRightCheck)->rightName, (*itModRight)->rightName) == 0)
                {
                    hasRight = true;
                    if (itAppRightCheck != itAppRight) // only erase if it's not the same pointer to avoid invalidating the main loop iterator
                    {
                        m_vecAppRightRelations.erase(itAppRightCheck); // to optimize the next search, remove the right once found
                    }
                    break;
                }
                ++itAppRightCheck;
            }
            outputFile << (hasRight ? "x" : " ") << SEPARATOR;
        }
        outputFile << std::endl;
        ret++;
        m_vecAppRightRelations.erase(itAppRight);
        itAppRight = m_vecAppRightRelations.begin();
    }

    // close File
    outputFile.close();

    return ret;
}

long CExportRights::CreateCSVFileApplications()
{
    // Create CSV file and write header row
    std::ofstream outputFile(m_strCSVFile.c_str(), std::ofstream::out);
    if (outputFile.is_open() == false)
    {
        WriteLogFile("ERROR: Fail to create CSV file " + m_strCSVFile);
        return ERR_OUTPUT_CSV_OPEN; // Handle file open error
    }

    // Merge reports
    MergeMainframeReport();
    MergeOrderEntryReport();

   // Define a custom comparison function (lambda function)
    auto customCompare = [](const CsvRowApplication* ptra, const CsvRowApplication* ptrb)
    {
        // Compare based on surname, then forename, and finally username
        int surnameComparison = std::strcmp(ptra->surname, ptrb->surname);
        if (surnameComparison != 0)
        {
            return surnameComparison < 0;
        }
        int forenameComparison = std::strcmp(ptra->forename, ptrb->forename);
        if (forenameComparison != 0)
        {
            return forenameComparison < 0;
        }

        return std::strcmp(ptra->username, ptrb->username) < 0;
    };

    // fill vector from set and sort it
    std::vector<CsvRowApplication*>	vecApplications(m_setApplications.begin(), m_setApplications.end());
    std::sort(vecApplications.begin(), vecApplications.end(), customCompare);

    // write header row
    outputFile
        << "area/region/branch id" << SEPARATOR
        << "area/region/branch" << SEPARATOR
        << "groupname" << SEPARATOR
        << "username" << SEPARATOR
        << "surname" << SEPARATOR
        << "forename" << SEPARATOR
        << "email" << SEPARATOR
		<< "usertype" << SEPARATOR
        << "application";

    if (!m_strValidUserFile.empty())
    {
        outputFile << SEPARATOR << "Valid user" << std::endl;
    }
    else
    {
        outputFile << std::endl;
    }

    // loop over the sorted list CsvRowApplication
    for (const CsvRowApplication* pRow : vecApplications)
    {
        // fill column "area/region/branch id" only if the id > 0
        std::string strAreaId = "";
        if(pRow->area_region_branch_id > 0)
            strAreaId = std::to_string(pRow->area_region_branch_id);

        // Write the row data to the CSV file
        outputFile
            << strAreaId << SEPARATOR
            << pRow->area_region_branch << SEPARATOR
            << pRow->groupname << SEPARATOR
            << pRow->username << SEPARATOR
            << pRow->surname << SEPARATOR
            << pRow->forename << SEPARATOR
			<< pRow->emailuser << SEPARATOR
            << pRow->usertype << SEPARATOR
            << pRow->application; 

        if (!m_strValidUserFile.empty())
        {
            outputFile << SEPARATOR << (pRow->validUser ? "Yes" : "No") << std::endl;
        }
        else
        {
            outputFile << std::endl;
        }
    }

    // close File
    outputFile.close();

    return 0;
}

void CExportRights::MergeReport(std::string& filename, int nFirstLine, std::string applicationName)
{
    if (filename.empty())
        return;

    // Open the input CSV file
    std::ifstream inputFile(filename);
    if (!inputFile.is_open())
    {
        WriteLogFile("Error opening file:" + std::string(filename));
        return;
    }

    // Read and process each line of the CSV file
    std::string line;
    int nCountLine = 1;
    while (std::getline(inputFile, line))
    {
        if (nCountLine < nFirstLine)
        {
            nCountLine++;
            continue;
        }
        std::istringstream   tokenStream(line);
        int                  nCountColumn = 1;
        std::string          token;
        CsvRowApplication   *pRow = new CsvRowApplication();

        while (std::getline(tokenStream, token, ';'))
        {
            switch (nCountColumn)
            {
            case 1:
                try
                {
                    pRow->area_region_branch_id = std::stol(token);
                }
                catch (const std::exception&)
                {
                    pRow->area_region_branch_id = 0;
                }
                break;
            case 2:
                std::strcpy(pRow->username, token.c_str());
                break;
            default:
                continue;
            }
            nCountColumn++;
        }
        if (std::strlen(pRow->username) != 0)
        {
            std::strcpy(pRow->application, applicationName.c_str());

            std::pair<std::string, std::string> fullname;
            pRow->validUser = m_validUsers.FindUserByUsername(pRow->username, fullname);
            if (pRow->validUser)
            {

                std::strcpy(pRow->surname, fullname.first.c_str());
                std::strcpy(pRow->forename, fullname.second.c_str());
            }
            m_setApplications.insert(pRow);
        }
        else
        {
            delete pRow;
        }
        nCountLine++;
    }
    inputFile.close();
}

void CExportRights::MergeMainframeReport()
{
    MergeReport(m_strMainframePermissionReport, 1, "PHARMOS");
}

void CExportRights::MergeOrderEntryReport()
{
    MergeReport(m_strOrderEntryPermissionReport, 1, "Auftragserfassung");
}

std::string CExportRights::trim(char* data)
{
    if (data != NULL)
    {
        return trim(std::string(data));
    }
    return "";
}

std::string CExportRights::trim(std::string data)
{
    std::string ret = data;
    bool blnSpace = true;
    // remove leading and trailing spaces
    while ((ret.length() > 0) && (blnSpace == true))
    {
        blnSpace = false;
        // leading
        if (ret.at(0) == ' ')
        {
            blnSpace = true;
            ret = ret.substr(1);
        }
        // trailing
        else if (ret.at(ret.length() - 1) == ' ')
        {
            blnSpace = true;
            ret = ret.substr(0, ret.length() - 1);
        }
    }
    return ret;
}

///////////////////////////////////////// Main /////////////////////////////////////////////////
int main(int argc, char* argv[])
{
    CExportRights theApp;

    if (theApp.Init(argc, argv) == false)
    {
        theApp.Usage();
        return ERR_APP_ARGUMENTS; // exit with return code of missing/wrong parameters
    }
    return theApp.Run();
}
