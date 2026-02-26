
#pragma once

struct CsvRowPermission
{
    int area_region_branch_id;
    char area_region_branch[31];
    int groupid;
    char groupname[31];
    int user_id;
    char username[21];
    char forename[21];
    char surname[31];
	char usertype[151];
    bool activeUser;
    int applicationid;
    char application[31];
    char module[31];
    char permission[21];
    char permission_description[255];
    bool validUser;

    CsvRowPermission() : area_region_branch_id(0), groupid(0), user_id(0), activeUser(false), applicationid(0), validUser(false)
    {
        area_region_branch[0] = '\0';
        groupname[0] = '\0';
        username[0] = '\0';
        forename[0] = '\0';
        surname[0] = '\0';
		usertype[0] = '\0';
        application[0] = '\0';
        module[0] = '\0';
        permission[0] = '\0';
        permission_description[0] = '\0';
    }
};

struct CsvRowApplication
{
    int area_region_branch_id;
    char area_region_branch[31];
    int groupid;
    char groupname[31];
    int user_id;
    char username[21];
    char forename[21];
    char surname[31];
	char emailuser[51];
	char usertype[151];
    bool activeUser;
    int applicationid;
    char application[31];
    bool validUser;

    CsvRowApplication() : area_region_branch_id(0), groupid(0), user_id(0), activeUser(false), applicationid(0), validUser(false)
    {
        area_region_branch[0] = '\0';
        groupname[0] = '\0';
        username[0] = '\0';
        forename[0] = '\0';
        surname[0] = '\0';
		emailuser[0] = '\0';
        application[0] = '\0';
		usertype[0] = '\0';
    }
};

struct CompareCsvRowApplication
{
    bool operator()(const CsvRowApplication* lhs, const CsvRowApplication* rhs) const
    {
        int cmp = 0;
        // Compare based on the combination of fields
        if (lhs->user_id != rhs->user_id)
        {
            return lhs->user_id < rhs->user_id;
        }
        cmp = std::strcmp(lhs->username, rhs->username);
        if (cmp != 0)
        {
            return cmp < 0;
        }
        cmp = std::strcmp(lhs->groupname, rhs->groupname);
        if (cmp != 0)
        {
            return cmp < 0;
        }
        if (lhs->area_region_branch_id != rhs->area_region_branch_id)
        {
            return lhs->area_region_branch_id < rhs->area_region_branch_id;
        }
        cmp = std::strcmp(lhs->application, rhs->application);
        if (cmp != 0)
        {
            return cmp < 0;
        }
        // If all specified fields are equal, consider the elements as equal
        return false;
    }
};

struct PPUSER
{
    short userid;
    char employeeid[21];
    char username[21];
    char forename[21];
    char surname[31];
    char emailuser[51];
    char usertype[151];
    short active;

    PPUSER() : userid(0), active(0)
    {
        employeeid[0] = '\0';
        username[0] = '\0';
        forename[0] = '\0';
        surname[0] = '\0';
        emailuser[0] = '\0';
		usertype[0] = '\0';
    }
};

struct PPGROUP
{
    short groupid;
    char name[31];

    PPGROUP() : groupid(0)
    {
        name[0] = '\0';
    }
};

struct PPAPPLICATION
{
    short appid;
    char name[31];

    PPAPPLICATION() : appid(0)
    {
        name[0] = '\0';
    }
};

struct PPREGIONBRANCH
{
    short regionbranchid;
    char name[31];

    PPREGIONBRANCH() : regionbranchid(0)
    {
        name[0] = '\0';
    }
};

struct PPCORAPPUSERGROUP
{
    short groupid;
    short userid;
    short appid;

    PPCORAPPUSERGROUP() : groupid(0), userid(0), appid(0) {}
};

struct PPAPPRIGHT
{
    short appid;
    short userid;
    short groupid;
    short branchregionid;
    short moduleid;
    char rightName[21];

    PPAPPRIGHT() : groupid(0), userid(0), appid(0), branchregionid(0), moduleid(0)
    {
        rightName[0] = '\0';
    }
};

struct PPAPPMODULE
{
    short appid;
    short moduleid;
    char moduleName[31];

    PPAPPMODULE() : appid(0), moduleid(0)
    {
        moduleName[0] = '\0';
    }
};

struct PPAPPMODRIGHT
{
    short appid;
    short moduleid;
    char rightName[21];
    char rightDescription[255];

    PPAPPMODRIGHT() : appid(0), moduleid(0)
    {
        rightName[0] = '\0';
        rightDescription[0] = '\0';
    }
};