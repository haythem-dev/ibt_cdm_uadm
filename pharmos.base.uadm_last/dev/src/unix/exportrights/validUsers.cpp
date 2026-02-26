#include <string>
#include <cctype>
#include "validUsers.h"

#define SEPARATOR ';'


// Trim from start
static inline void ltrim(std::string& s)
{
	size_t start = 0;
	while (start < s.length() && std::isspace(static_cast<unsigned char>(s[start])))
	{
		++start;
	}
	s.erase(s.begin(), s.begin() + start);
}

// Trim from end
static inline void rtrim(std::string& s)
{
	size_t end = s.length();
	while (end > 0 && std::isspace(static_cast<unsigned char>(s[end - 1])))
	{
		--end;
	}
	s.erase(s.begin() + end, s.end());
}

// Trim from both ends
static inline void trim(std::string& s)
{
	ltrim(s);
	rtrim(s);
}


CValidUsers::CValidUsers()
{
}

int CValidUsers::ReadCSV(const std::string& filename)
{
	// Open the input CSV file
	std::ifstream inputFile(filename);
	if (!inputFile.is_open())
	{
		return ERR_INPUT_CSV_OPEN;
	}
	std::string line;
	// Read and skip the first line (header)
	std::getline(inputFile, line);

	// Read CSV rows and insert users in the map
	while (std::getline(inputFile, line))
	{
		std::istringstream iss(line);
		std::string lastname, firstname, username;
		if (std::getline(iss, lastname, SEPARATOR))
		{
			if (std::getline(iss, firstname, SEPARATOR))
			{
				if (!std::getline(iss, username, SEPARATOR))
				{
					username = "";
				}
			}
			else
			{
				firstname = "";
				username = "";
			}

			// Trim the strings
			trim(lastname);
			trim(firstname);
			trim(username);

			// ignore empty full name
			if (lastname.empty() && firstname.empty())
			{
				continue;
			}	
			//trim username then compare it to N/A
			if (username == "n/a" || username == "N/A" || username == "n/A" || username == "N/a")
			{
				username = "";
			}
			// insert a pair (lastname, firstname) in the map
			if (!username.empty())
			{
				m_mapIdUser[username] = std::make_pair(lastname, firstname);
			}
			//concantenate lastname and firstname, then insert into the hashed set
			m_setUserFullnames.insert(lastname + " " + firstname);
		}
	}
	inputFile.close();
	return 0;
}

bool CValidUsers::FindUserByUsername(std::string username, std::pair<std::string, std::string>& fullname)
{
	trim(username);
	auto it = m_mapIdUser.find(username);
	if (it != m_mapIdUser.end())
	{
		fullname = it->second; // User found
		return true;
	}
	else
	{
		return false; // User not found
	}
}


bool CValidUsers::IsUserFullnameValid(std::string lastname, std::string firstname)
{
	trim(lastname);
	trim(firstname);
	std::string fullName = lastname + " " + firstname;
	return m_setUserFullnames.find(fullName) != m_setUserFullnames.end();
}