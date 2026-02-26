//------------------------------------------------------------------------------------------------------------------//
//! \file  loadbireports
// \brief  application class (main) for loading bi reports from bi portal
// \author Thomas Arnold
// \date   05.10.2023
//------------------------------------------------------------------------------------------------------------------//
#ifndef GUARD_LOADBIREPORTS_H
#define GUARD_LOADBIREPORTS_H

#include "loadbireports_webservice.h"
#include <vector>

static const int RETURN_RESULT_NO_ACTION = -1;
static const int RETURN_RESULT_INIT_OK = 1;
static const int RETURN_RESULT_WEBSERVICE_OK = 0;
static const int RETURN_RESULT_UNKNOWN_ERROR = -100;
static const int RETURN_RESULT_WRONG_PARAMETERS_ERROR = -101;
static const int RETURN_RESULT_WEBSERVICE_CONNECT_ERROR = -110;
static const int RETURN_RESULT_WEBSERVICE_REQUEST_ERROR = -111;
static const int RETURN_RESULT_WEBSERVICE_RESPONSE_ERROR = -112;
static const int RETURN_RESULT_WEBSERVICE_WRITEFILE_ERROR = -113;
static const int RETURN_RESULT_EMPTY_HTML_FILE_ERROR = -114;
static const int RETURN_RESULT_UNABLE_OPEN_HTML_FILE = -115;
static const int RETURN_RESULt_NO_CSV_FILE_GENERATED = -116;

namespace loadbireports
{
	class LoadBIReports
	{
	public:
		LoadBIReports();
		~LoadBIReports();

		int     init(std::string path, int argc, char *argv[]);
		int     runWebService(std::string nBranch);

	private:
		void printUsage();
		bool parseCommandLine(int argc, char* argv[]);
		std::string replace_all(std::string text, std::string from, std::string to);
		void writeFile(std::string file, std::string text);
		bool doLoadMainframeReport(std::string nBranch);
		bool doLoadOrderEntryReport(std::string nBranch);
		int doExtractDatafromHtmlFile(std::string file);
		std::string parseResponse(std::string response);
		bool     convertToCSV(std::string csvFilePath);

		std::string m_strBasePath;
		std::string m_strBIReport;
		loadbireports::WebService m_objWebAPI;
		std::string m_strWebServer;
		std::string m_strWebPort;
		std::string m_strWebAccept;
		std::string m_strWebAuth;
		std::string m_strWebCred;
		std::string m_strWebType;
		std::string m_strWebRequest;
		std::string m_strWebBodyType;
		std::string m_strWebBody;
		std::string m_strWebReportFile;
		std::string m_strWebCSVFile;
		std::vector<std::vector<std::string>> m_extractedData;
	};
}
#endif //end GUARD_LOADBIREPORTS_H