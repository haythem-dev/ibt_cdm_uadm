#pragma once

#include <string>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#define ERR_INPUT_CSV_OPEN 4

static inline void trim(std::string& s);

class CValidUsers
{
public:
    CValidUsers(); 

    int ReadCSV(const std::string& filename);

    // Find a user by username
    bool FindUserByUsername(std::string username, std::pair<std::string, std::string>& fullname);

    // check if user is valid given his fullname (lastname, firstname)
    bool IsUserFullnameValid(std::string lastname, std::string firstname);

private:
    // map username -> fullname pair(lastname, firstname)
    std::unordered_map<std::string, std::pair<std::string, std::string>>     m_mapIdUser;

    // set for all user fullnames
    std::unordered_set<std::string>                                          m_setUserFullnames;
};