#pragma once

#define SQL_SETISOLA_ERROR   (-554)
#include <string>
#include <unordered_set>
#include <set>
#include <vector>
#include "validUsers.h"
#include "structs.h"

/// Main class CExportRights

class CExportRights
{
public:
	CExportRights();
	virtual ~CExportRights();

	void Usage();
	void Reset();
	bool Init(int argc, char* argv[]);
	int  Run();

protected:
	bool OpenDatabase();
	bool CloseDatabase();
	bool WriteLogFile(std::string log);

	long ReadCommonData();
	long ReadPermissionsData();
	long ReadApplicationsData();
	long ReadRolesData();

	long CreateCSVFilePermissions();
	long CreateCSVFileApplications();
	long CreateCSVFileRoles();
	void MergeReport(std::string& filename, int nFirstLine, std::string applicationName);
	void MergeMainframeReport();
	void MergeOrderEntryReport();

private:
	HNMP						m_pipConnection;
	std::string					m_strAppName;
	std::string					m_strServer;
	std::string					m_strDBName;
	std::string					m_strServerName;
	std::string					m_strServiceName;
	int							m_intPort;
	int							m_intExportMode;

	std::string					m_strAreaID;
	std::string					m_strCountry;		
	std::string					m_strApplicationID;
	std::string					m_strGroupID;
	std::string					m_strUserID;
	bool						m_bGroupsOnly;

	std::string					m_strCSVFile;
	std::string					m_strLogFile;

	std::string					m_strValidUserFile;
	CValidUsers					m_validUsers;

	std::vector<CsvRowPermission*>							m_vecPermissions;
	std::set<CsvRowApplication*, CompareCsvRowApplication>	m_setApplications;

	std::vector<PPUSER*>		m_vecUsers;
	std::vector<PPGROUP*>		m_vecUsergroups;
	std::vector<PPAPPLICATION*> m_vecApplications;
	std::vector<PPREGIONBRANCH*>	m_vecRegionBranches;
	std::vector<PPCORAPPUSERGROUP*> m_vecGroupUserAppRelations;
	std::vector<PPAPPRIGHT*>	m_vecAppRightRelations;
	std::vector<PPAPPMODULE*>	m_vecModules;
	std::vector<PPAPPMODRIGHT*>	m_vecModRights;

	std::string					m_strMainframePermissionReport;
	std::string					m_strOrderEntryPermissionReport;

	char* getRegionBranchName(short regionbranchid);
	char* getUsergroupName(short groupid);
	char* getUsername(short userid);
	char* getForename(short userid);
	char* getSurname(short userid);
	char* getEMail(short userid);
	char* getUserType(short userid);
	bool  isUserActive(short userid);
	char* getApplicationName(short appid);
	char* getModuleName(short appid, short moduleid);
	char* getRightDescription(short appid, short moduleid, char* rightName);

	void savecpy(char* dest, char* source, int len);

	std::string trim(std::string data);
	std::string trim(char* data);
};
