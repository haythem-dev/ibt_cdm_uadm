// loadbireports.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstdio>
#include "loadbireports.h"

namespace loadbireports
{
	//-------------------------------------------------------------------------------------------------//
	//! standard constructor of loadbireports
	//-------------------------------------------------------------------------------------------------//
	LoadBIReports::LoadBIReports()
	{
		m_strBasePath = "";
		m_strBIReport = "";
		m_strWebServer = "";
		m_strWebPort = "";
		m_strWebAccept = "";
		m_strWebAuth = "";
		m_strWebCred = "";
		m_strWebType = "";
		m_strWebRequest = "";
		m_strWebBodyType = "";
		m_strWebBody = "";
		m_strWebReportFile = "";
		m_strWebCSVFile = "";
	}

	//-------------------------------------------------------------------------------------------------//
	//! standard destructor of loadbireports
	//-------------------------------------------------------------------------------------------------//
	LoadBIReports::~LoadBIReports()
	{
	}

	//-------------------------------------------------------------------------------------------------//
	//! initialization with handed over arguments
	//-------------------------------------------------------------------------------------------------//
	int LoadBIReports::init(std::string path, int argc, char* argv[])
	{
		// take over the base path
		m_strBasePath = path;
		// parse and transfer the command line params into system
		bool ret = this->parseCommandLine(argc, argv);

		if (ret == false)
		{
			this->printUsage();
			return RETURN_RESULT_WRONG_PARAMETERS_ERROR;
		}

		return RETURN_RESULT_INIT_OK;
	}

	//-------------------------------------------------------------------------------------------------//
	// webservice handler
	//-------------------------------------------------------------------------------------------------//
	int LoadBIReports::runWebService(std::string nBranch)
	{
		int ret = RETURN_RESULT_NO_ACTION;
		// connect the webservice to obtain bi portal data
		if ((this->doLoadMainframeReport(nBranch)) || (this->doLoadOrderEntryReport(nBranch)))
		{
			if (m_objWebAPI.connect(m_strWebServer, atoi(m_strWebPort.c_str())) == true)
			{
				if (m_objWebAPI.request(m_strWebType, m_strWebRequest, m_strWebBody, m_strWebBodyType) == true)
				{
					std::string req = m_objWebAPI.getLastRequest();
					std::string resp = m_objWebAPI.getLastResponse();

					// transform response into file format
					std::string data = parseResponse(resp);
					// in case html response will be needed as file
					if (m_strWebReportFile.empty() == false)
					{
						writeFile(m_strWebReportFile, data);
					}

					//Export to CSV File when flag is set
					if (m_strWebCSVFile.empty() == false)
					{
						int resultOfExtraction = this->doExtractDatafromHtmlFile(m_strWebReportFile);
						//remove html file because it's not needed any more
						std::string htmlFilePath = m_strBasePath + m_strWebReportFile;
						std::remove(htmlFilePath.c_str());

						if (resultOfExtraction == 0) 
						{
							ret = RETURN_RESULT_EMPTY_HTML_FILE_ERROR;
							std::remove(htmlFilePath.c_str());

						}
						else if (resultOfExtraction == -1) 
						{
							ret = RETURN_RESULT_UNABLE_OPEN_HTML_FILE;
						}
						else 
						{
							// convert the data into CSV and save it into file
							std::string csvFilePath = m_strBasePath + m_strWebCSVFile;
							bool resultValue = convertToCSV(csvFilePath);
							if (resultValue)
							{
								ret = RETURN_RESULT_WEBSERVICE_OK;
							}
							else
							{
								ret = RETURN_RESULT_WEBSERVICE_WRITEFILE_ERROR;
							}
						}
					}
					else
					{
						// do nothing with result
						ret = RETURN_RESULT_WEBSERVICE_OK;
					}
				}
				else
				{
					ret = RETURN_RESULT_WEBSERVICE_REQUEST_ERROR;
				}
				// close the web service connection
				m_objWebAPI.close();
			}
			else
			{
				ret = RETURN_RESULT_WEBSERVICE_CONNECT_ERROR;
			}
		}
		return ret;
	}

	// command line parameter management
	bool LoadBIReports::parseCommandLine(int argc, char* argv[])
	{
		bool answer = false;
		size_t delimPos;
		std::string optKey;
		std::string optValue;

		for (int i = 1; i < argc; i++)
		{
			std::string option(argv[i]);

			delimPos = option.find_first_of("=:");
			if (delimPos == std::string::npos)
			{
				/* simple option / flag */
				optKey = option;
				optValue = "";
			}
			else
			{
				/* option: key=value */
				optKey = option.substr(0, delimPos);
				optValue = option.substr(delimPos + 1);
			}
			// set the properties accordingly
			if (optKey.compare("report") == 0)
			{
				m_strBIReport = optValue;
				if ((m_strBIReport.compare("BSC001") == 0) || (m_strBIReport.compare("BSA003") == 0))
				{
					answer = true;
				}
			}
		}
		return answer;
	}

	void LoadBIReports::printUsage()
	{
		std::cout << "USAGE:" << std::endl << "loadbireports " <<
		"report=[BSC001|BSA003]" << std::endl <<
		"report parameter is mandatory!" << std::endl;
	}

	bool LoadBIReports::doLoadMainframeReport(std::string nBranch)
	{
		// only if mainframe report is requested
		if (m_strBIReport.compare("BSC001") == 0)
		{
			// set all parameters for this report
			m_strWebServer = "10.156.63.154"; // "denu00ms0866.phoenix.loc"; // "bi-std-web-p-cs.phoenix.loc";
			m_strWebPort = "80";
			m_strWebAccept = "text/html,application/xhtml+xml,application/xml";
			m_strWebAuth = "";
			m_strWebCred = "";
			m_strWebType = "POST";
			m_strWebRequest = "/ibi_apps/WFServlet";
			m_strWebBodyType = "application/x-www-form-urlencoded";
			m_strWebBody = "parCBranchAll="
				"&parCBranchHead=" + nBranch +
				"&parCBranchWhere=%27" + nBranch + "%27"
				"&parNQtySelBranches=1"
				"&IBIF_ex=BSC001fex"
				"&IBIAPP_app="
				"&RUNTIMEID=12394180999"
				"&FEXNAME=BSC001"
				"&EVENTID=1"
				"&parCReportOutputTarget=bi_report"
				"&LNGID=D"
				"&DOMUSER=phoenix%5Cantje.zoschke"
				"&RCRepNamLong=BSC001"
				"&RCLanguage=D"
				"&RCUserID=phoenix%5Cantje.zoschke"
				"&RCUserEmail=%27%27"
				"&RCReportName=BSC001fex"
				"&RCDateType=6"
				"&RCESchTime=06%3A00"
				"&RCMaxBG=5"
				"&parCNameOfClickedButton=start"
				"&parCChkParameterValues=%23parCBranchAll%5B%5D%23parCBranchHead%5B12%5D%23parCBranchWhere%5B%2212%22%5D%23parNQtySelBranches%5B1%5D%23IBIF_ex%5BBSC001fex%5D%23IBIAPP_app%5B%5D%23FEXNAME%5BBSC001%5D%23EVENTID%5B1%5D%23parCReportOutputTarget%5Bbi_report%5D%23LNGID%5BD%5D%23DOMUSER%5Bphoenix%5Cantje.zoschke%5D%23RCRepNamLong%5BBSC001%5D%23RCLanguage%5BD%5D%23RCUserID%5Bphoenix%5Cantje.zoschke%5D%23RCUserEmail%5B%22%22%5D%23RCReportName%5BBSC001fex%5D%23RCDateType%5B6%5D%23RCESchTime%5B06%3A00%5D%23RCMaxBG%5B5%5D%23bgbutton%5BHintergrund%5D%23abobutton%5BAbonnement%5D%23ovbutton%5B%C3%9Cbersicht%5D%23startbutton%5BStart%5D%23LBShowMissingData%5B%5D%23parCStyle%5BUNFHOCH.sty%5D%23WFFMT%5BHTML%5D%23parNBranch%5B12%5D%23parCAllVz%5BY%5Dfalse%23parNUserID%5B%5D%23parNRightID%5B%5D%23parCUserName%5B%5D%23parCCICSUserShort%5B%5D%23parCRightCode%5B%5D%23parCQCountry%5BY%5Dfalse%23parCSortColumn%5BCICSUser%5D%23parCSortType%5BLOWEST%5Dtrue%23parCSortType%5BHIGHEST%5Dfalse&startbutton=Start"
				"&parCStyle=UNFHOCH.sty"
				"&WFFMT=HTML"
				"&parNBranch=" + nBranch +
				"&parNUserID="
				"&parNRightID="
				"&parCUserName="
				"&parCCICSUserShort="
				"&parCRightCode="
				"&parCSortColumn=CICSUser"
				"&parCSortType=LOWEST";
			m_strWebReportFile = "BSC001_" + nBranch + ".html";
			m_strWebCSVFile = "BSC001.csv";
			return true;
		}
		return false;
	}

	bool LoadBIReports::doLoadOrderEntryReport(std::string nBranch)
	{
		// only if order entry report is requested
		if (m_strBIReport.compare("BSA003") == 0)
		{
			// set all parameters for this report
			m_strWebServer = "10.156.63.154"; // "denu00ms0866.phoenix.loc"; // "bi-std-web-p-cs.phoenix.loc";
			m_strWebPort = "80";
			m_strWebAccept = "text/html,application/xhtml+xml,application/xml";
			m_strWebAuth = "";
			m_strWebCred = "";
			m_strWebType = "POST";
			m_strWebRequest = "/ibi_apps/WFServlet";
			m_strWebBodyType = "application/x-www-form-urlencoded";
			m_strWebBody = "IBIF_ex=BSA003fex"
				"&IBIAPP_app="
				"&RUNTIMEID=12394180999"
				"&FEXNAME=BSA003"
				"&EVENTID=1"
				"&parCReportOutputTarget=bi_report"
				"&LNGID=D"
				"&DOMUSER=phoenix%5Cantje.zoschke"
				"&RCRepNamLong=BSA003"
				"&RCLanguage=D"
				"&RCUserID=phoenix%5Cantje.zoschke"
				"&RCUserEmail=%27%27"
				"&RCReportName=BSA003fex"
				"&RCDateType=6"
				"&RCESchTime=06%3A00"
				"&RCMaxBG=5"
				"&parCNameOfClickedButton=start"
				"&parCChkParameterValues=%23IBIF_ex%5BBSA003fex%5D%23IBIAPP_app%5B%5D%23FEXNAME%5BBSA003%5D%23EVENTID%5B1%5D%23parCReportOutputTarget%5Bbi_report%5D%23LNGID%5BD%5D%23DOMUSER%5Bphoenix%5Cantje.zoschke%5D%23RCRepNamLong%5BBSA003%5D%23RCLanguage%5BD%5D%23RCUserID%5Bphoenix%5Cantje.zoschke%5D%23RCUserEmail%5B%22%22%5D%23RCReportName%5BBSA003fex%5D%23RCDateType%5B6%5D%23RCESchTime%5B06%3A00%5D%23RCMaxBG%5B5%5D%23bgbutton%5BHintergrund%5D%23abobutton%5BAbonnement%5D%23ovbutton%5B%C3%9Cbersicht%5D%23startbutton%5BStart%5D%23LBShowMissingData%5B%5D%23parCStyle%5BUNFHOCH.sty%5D%23WFFMT%5BHTML%5D%23parNBranchHead%5B12%5D%23parNBranchWhere%5B12%5D%23parNBranchSelected%5B%5D%23parNNorthBrDisabled%5B1%5D%23parNSouthBrDisabled%5B1%5D%23parNBranch%5B12%5D%23parCAllNORTHvz%5BY%5Dfalse%23parCAllSOUTHvz%5BY%5Dfalse%23parNUserID%5B%5D%23parNRightID%5B%5D%23parCQCountry%5BY%5Dfalse%23parCSortColumn%5BUserID%5D%23parCSortType%5BLOWEST%5Dtrue%23parCSortType%5BHIGHEST%5Dfalse"
				"&startbutton=Start"
				"&parCStyle=UNFHOCH.sty"
				"&WFFMT=HTML"
				"&parNBranchHead=" + nBranch +
				"&parNBranchWhere=" + nBranch +
				"&parNBranchSelected="
				"&parNNorthBrDisabled=1"
				"&parNSouthBrDisabled=1"
				"&parNBranch=" + nBranch + 
				"&parNUserID="
				"&parNRightID="
				"&parCSortColumn=UserID"
				"&parCSortType=LOWEST";
			m_strWebReportFile = "BSA003_" + nBranch + ".html";
			m_strWebCSVFile = "BSA003.csv";
			return true;
		}
		return false;
	}

	//-------------------------------------------------------------------------------------------------//
	//! parses the raw http(s) response and split the data from the header
	//-------------------------------------------------------------------------------------------------//
	std::string LoadBIReports::parseResponse(std::string response)
	{
		std::string ret = "";

		// split response into http header and body
		int pos = (int)response.find("\r\n\r\n");
		if (pos > 0)
		{
			std::string head = response.substr(0, pos);
			std::string body = response.substr(pos + 4);

			bool chunked = false;
			if (head.find("Transfer-Encoding: chunked") != std::string::npos)
			{
				chunked = true;
			}
			if (chunked == true)
			{
				std::string newbody = "";
				do
				{
					pos = (int)body.find("\r\n");
					if (pos > 0)
					{
						std::string chunk = body.substr(0, pos);
						long chunksize = strtol(chunk.c_str(), NULL, 16);
						std::string chunkdata = body.substr(pos + 2, chunksize);
						newbody += chunkdata;
						body = body.substr(pos + 2 + chunksize + 2);
					}
				} while (pos > 0);
				// done, so transfer the new body back to body after transfer-decoding
				body = newbody;
			}
			ret = body;
		}
		return ret;
	}

	//-------------------------------------------------------------------------------------------------//
	//! writes a file e.g. for the received data from web
	//-------------------------------------------------------------------------------------------------//
	void LoadBIReports::writeFile(std::string file, std::string text)
	{
		std::ofstream protocolFile;
		std::string pfile = m_strBasePath + file;
		protocolFile.open(pfile.c_str(), std::ios_base::app);
		protocolFile << text << std::endl;
		protocolFile.close();
	}

	//-------------------------------------------------------------------------------------------------//
	//! replaces characters in a text, as this method is not available in the std library
	//-------------------------------------------------------------------------------------------------//
	std::string LoadBIReports::replace_all(std::string text, std::string from, std::string to)
	{
		std::string ret = text;

		if (from.empty() == true)
		{
			return ret;
		}

		size_t start_pos = 0;
		while ((start_pos = ret.find(from, start_pos)) != std::string::npos)
		{
			ret.replace(start_pos, from.length(), to);
			start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
		}
		return ret;
	}

	//--------------------------------------------------------------------------------------------------------------//
	//! Giving the file , this method will extract the data needed from the html file and add it to  m_extractedData
	//--------------------------------------------------------------------------------------------------------------//
	int LoadBIReports::doExtractDatafromHtmlFile(std::string file)
	{
		std::string pfile = m_strBasePath + file;

		std::ifstream htmlFile(pfile);
		if (!htmlFile.is_open())
		{
			std::cerr << "Failed to open" << pfile << "." << std::endl;
			return -1;
		}

		std::stringstream htmlBuffer;
		htmlBuffer << htmlFile.rdbuf();
		htmlFile.close();

		std::string htmlContent = htmlBuffer.str();

		// Remove any newline and whitespace characters
		htmlContent.erase(std::remove_if(htmlContent.begin(), htmlContent.end(), ::isspace), htmlContent.end());
		if (htmlContent.size() == 0) {
			std::cout << "File " << pfile << "is empty." << std::endl;
			return 0;
		}
		size_t startPos = 0;
		startPos = htmlContent.find("class='x2_0'>");
		startPos = startPos + 13;
		size_t endPos = startPos;
		while (htmlContent.substr(endPos, 1) != "<")
		{
			endPos++;
		}
		std::string contentColumn1 = htmlContent.substr(startPos, endPos - startPos);

		startPos = endPos;

		while (htmlContent.substr(startPos, 4) != "x2_0")
		{
			startPos++;
		}
		startPos = startPos + 6;
		endPos = startPos;
		while (htmlContent.substr(endPos, 1) != "<")
		{
			endPos++;
		}
		std::string contentColumn2 = htmlContent.substr(startPos, endPos - startPos);
		if (contentColumn2 != "&nbsp;")
		{
			std::vector<std::string> rowData;
			rowData.push_back(contentColumn1);
			rowData.push_back(contentColumn2);
			m_extractedData.push_back(rowData);
		}
		startPos = endPos;
		while (startPos < htmlContent.size())
		{
			if ((htmlContent.substr(startPos, 4) == "<tr>") && ((htmlContent.substr(startPos + 31, 4) == "x2_0") || (htmlContent.substr(startPos + 31, 4) == "x2_1")))
			{
				std::vector<std::string> rowData;
				rowData.push_back(contentColumn1);
				startPos = startPos + 64;
				if ((htmlContent.substr(startPos - 4, 4) == "x2_0") || (htmlContent.substr(startPos - 4, 4) == "x2_1"))
				{
					startPos = startPos + 2;
				}
				endPos = startPos;
				while (htmlContent.substr(endPos, 1) != "<")
				{
					endPos++;
				}
				std::string contentColumn2 = htmlContent.substr(startPos, endPos - startPos);
				rowData.push_back(contentColumn2);
				m_extractedData.push_back(rowData);
				startPos = endPos;
			}
			startPos++;
		}
		return 1;
	}

	//-------------------------------------------------------------------------------------------------//
	//! Giving csvFilePath, this method will add the data that is stored in m_extractedData to the file
	//-------------------------------------------------------------------------------------------------//
	bool LoadBIReports::convertToCSV(std::string csvFilePath) 
	{
		std::ofstream csvFile(csvFilePath, std::ios_base::app);
		if (!csvFile.is_open())
		{
			std::cerr << "Failed to open CSV file for writing." << std::endl;
			return false;
		}

		for (const auto& row : this->m_extractedData)
		{
			if (row.size() >= 2) // Ensure there are at least 2 columns
			{ 
				csvFile << row[0] << ";" << row[1] << "\n";
			}
		}
		this->m_extractedData.clear();
		csvFile.close();
		return true;
	}
}

int main(int argc, char* argv[])
{
	int result = RETURN_RESULT_NO_ACTION;

	try
	{
		std::string path = std::string(argv[1]);
		if (path.find_last_of("\\") != -1)
		{
			// windows version
			path = path.substr(0, path.find_last_of("\\"));
		}
		else if (path.find_last_of("//") != -1)
		{
			// unix version
			path = path.substr(0, path.find_last_of("//"));
		}
		else
		{
			// no path given, so take current directory
			char spath[1024];
#if defined WIN32
			GetCurrentDirectory(1024, (LPSTR)spath);
#else
			getcwd(spath, sizeof(spath));
#endif
			path = std::string(spath);
		}
#if defined WIN32
		path = path + "\\";
#else
		path = path + "//";
#endif

		loadbireports::LoadBIReports app;
		result = app.init(path, argc, argv);
		if (result == RETURN_RESULT_INIT_OK)
		{
			bool isReturnCodeZero = false;
			for (int nBranch = 12; nBranch < 17; nBranch++)
			{
				if (nBranch == 14)
				{
					nBranch = 16;
				}
				result = app.runWebService(std::to_string(nBranch));
				if (result == RETURN_RESULT_WEBSERVICE_CONNECT_ERROR) 
				{
					std::cout << "Error occured when trying to get the report for the branch: " << nBranch << ": Cannot connect to the web server to get the report." << std::endl;
				}
				else if (result == RETURN_RESULT_WEBSERVICE_REQUEST_ERROR)
				{
					std::cout << "Error occured when trying to get the report for the branch: " << nBranch << ": Something wrong happened when sending request to the server." << std::endl;
				}
				else if (result == RETURN_RESULT_UNABLE_OPEN_HTML_FILE)
				{
					std::cout << "Error occured when trying to get the report for the branch: " << nBranch << ": Unable to open HTML file." << std::endl;
				}
				else if (result == RETURN_RESULT_EMPTY_HTML_FILE_ERROR)
				{
					std::cout << "Error occured when trying to get the report for the branch: " << nBranch << ": HTML file returned from the server is empty." << std::endl;
				}
				else if (result == RETURN_RESULT_WEBSERVICE_WRITEFILE_ERROR)
				{
					std::cout << "Error occured when trying to get the report for the branch: " << nBranch << ": Cannot Add data to the CSV file." << std::endl;
				}
				else if (result == RETURN_RESULT_WEBSERVICE_OK)
				{
					isReturnCodeZero = true;
				}
			}
			if (!isReturnCodeZero) 
			{
				std::cout << "Fatal: There is no CSV file generated!!! " << std::endl;
				result = RETURN_RESULt_NO_CSV_FILE_GENERATED;
			}
			else 
			{
				result = RETURN_RESULT_WEBSERVICE_OK;
			}
		}
	}
	catch (...)
	{
		// default error, when something happend unexpected
		result = RETURN_RESULT_UNKNOWN_ERROR;
	}
	return result;
}
