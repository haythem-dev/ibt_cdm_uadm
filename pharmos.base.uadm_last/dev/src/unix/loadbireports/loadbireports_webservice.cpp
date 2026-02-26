//-----------------------------------------------------------------------------------------------------//
//! \file  loadbireports_webservice
// \brief  application class (webservice) for download bi reports
// \author Thomas Arnold
// \date   05.10.2023
//-----------------------------------------------------------------------------------------------------//

//-----------------------------------------------------------------------------------------------------//
// includes
//-----------------------------------------------------------------------------------------------------//

#include <cstring> // memset
#include <sstream>

#include "loadbireports_webservice.h"

//-----------------------------------------------------------------------------------------------------//
// namespace
//-----------------------------------------------------------------------------------------------------//
namespace loadbireports
{
	//-------------------------------------------------------------------------------------------------//
	//! standard constructor of loadbireports webservice
	//-------------------------------------------------------------------------------------------------//
	WebService::WebService()
	{
		m_intSocket = 0;
		m_blnConnected = false;
		m_strHTTPRequest = "";
		m_strHTTPResponse = "";
		m_strHost = "";
		m_intPort = 0;
		m_strAuthSchema = "";
		m_strCredentials = "";
	}

	//-------------------------------------------------------------------------------------------------//
	//! standard destructor of loadbireports webservice
	//-------------------------------------------------------------------------------------------------//
	WebService::~WebService()
	{
		close();
	}

	//-------------------------------------------------------------------------------------------------//
	//! set authorization in case if needed
	//-------------------------------------------------------------------------------------------------//
	bool WebService::authorize(std::string authschema, std::string credentials)
	{
		bool ret = true;

		m_strAuthSchema = authschema;
		m_strCredentials = credentials;
		int pos = (int)credentials.find(":", 0);
		if ((authschema.compare("Basic") == 0) && (pos > 0))
		{
			//encode credentials with base64
			m_strCredentials = base64_encode(credentials);
		}
		return ret;
	}

	//-------------------------------------------------------------------------------------------------//
	//! connect to web service with host and port
	//-------------------------------------------------------------------------------------------------//
	bool WebService::connect(std::string host, int port)
	{
		bool ret = true;

		m_strHost = host;
		m_intPort = port;

#if defined WIN32
		WSADATA wsaData;
		int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (iResult != 0) 
		{
			printf("error at WSAStartup\n");
			return false;
		}

		m_intSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
		ret = (m_intSocket >= 0);
#else
		int socket_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
		m_intSocket = socket_fd;
		ret = (-1 != socket_fd);
#endif
		if (ret == false)
		{
			printf("socket creation failed.\n");
			// closesocket(m_intSocket);
			clearSocket();
			return false;
		}

		// Server address construction
		struct sockaddr_in sad;
		std::memset(&sad, 0, sizeof(sad));
		sad.sin_family = AF_INET;
		sad.sin_addr.s_addr = inet_addr(m_strHost.c_str()); // server IP
		sad.sin_port = htons(m_intPort); // Server port
		// Connection to the bi server
		if (::connect(m_intSocket, (struct sockaddr *) &sad, sizeof(sad)) < 0)
		{
			printf("Failed to connect.\n");
			closesocket(m_intSocket);
			clearSocket();
			return false;
		}

		// define timeout for responses
		struct timeval timeout;
#if defined WIN32
		timeout.tv_sec = SOCKET_READ_TIMEOUT_SEC * 1000; // windows expects timeout in milliseconds
#else
		timeout.tv_sec = SOCKET_READ_TIMEOUT_SEC; // linux/unix expects timeout in seconds
#endif
		timeout.tv_usec = 0;
		::setsockopt(m_intSocket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));

		m_blnConnected = true;
		return true;
	}

	//-------------------------------------------------------------------------------------------------//
	//! request to webservice with given type (GET, POST) and optionally authentication and body
	//-------------------------------------------------------------------------------------------------//
	bool WebService::request(std::string type, std::string request, std::string body, std::string bodytype)
	{
		bool ret = false;
		if ((m_blnConnected == false) || (m_intSocket == 0))
		{
			printf("request cannot be sent, no connection established.\n");
			return ret;
		}

		std::stringstream strm;
		strm << type << " " << request << " HTTP/1.1\r\n"
			<< "Host: " << m_strHost << "\r\n";

		if (body.empty() == false)
		{
			strm << "Content-Type: " << bodytype << "\r\n"
				<< "Content-Length: " << body.length() << "\r\n";
		}
		strm << "Accept: */*\r\n"
			<< "User-Agent: loadbireports/1.0\r\n";
		if (!m_strAuthSchema.empty() && !m_strCredentials.empty())
		{
			strm << "Authorization: " << m_strAuthSchema << " " << m_strCredentials << "\r\n";
		}
		strm << "Pragma: no-cache\r\n"
			<< "Connection: keep-alive\r\n"
			<< "Cache-Control: no-cache\r\n\r\n";

		m_strHTTPRequest = strm.str() + body;

		int len = (int)m_strHTTPRequest.length();

		// use plain request to send to server
		if (::send(m_intSocket, m_strHTTPRequest.c_str(), len, 0) != len)
		{
			printf("send() sent a different number of bytes than expected");
			close();
			return ret;
		}

		m_strHTTPResponse = readResponse();
		return true;
	}

	//-------------------------------------------------------------------------------------------------//
	//! read and return the response from server after a request
	//-------------------------------------------------------------------------------------------------//
	std::string WebService::readResponse()
	{
		std::string ret = "";
		int bytesRcvd = 0;
		int totalBytesRcvd = 0;
		m_strHTTPResponse = "";
		char buf[MAX_BUFFER]; // buffer for data from the server

		do
		{
			bytesRcvd = ::recv(m_intSocket, buf, MAX_BUFFER - 1, 0);
			if (bytesRcvd > 0)
			{
				totalBytesRcvd += bytesRcvd; // Keep tally of total bytes
				buf[bytesRcvd] = '\0'; // Add \0 so printf knows where to stop
				ret += buf;
			}
		} 
		while (bytesRcvd > 0);

		return ret;
	}

	//-------------------------------------------------------------------------------------------------//
	//! returns the last created http request
	//-------------------------------------------------------------------------------------------------//
	std::string WebService::getLastRequest()
	{
		return m_strHTTPRequest;
	}

	//-------------------------------------------------------------------------------------------------//
	//! returns the last received http response
	//-------------------------------------------------------------------------------------------------//
	std::string WebService::getLastResponse()
	{
		return m_strHTTPResponse;
	}

	//-------------------------------------------------------------------------------------------------//
	//! close the importmedreg webservice and resets the data
	//-------------------------------------------------------------------------------------------------//
	bool WebService::close()
	{
		bool ret = false;

		if (m_intSocket > 0)
		{
			closesocket(m_intSocket);
			clearSocket();
			m_blnConnected = false;
			ret = true;
		}
		return ret;
	}

	//-------------------------------------------------------------------------------------------------//
	//! internal function to clear importmedreg webservice and resets the socket data
	//-------------------------------------------------------------------------------------------------//
	void WebService::clearSocket()
	{
#if defined WIN32
		WSACleanup();
#endif
		m_intSocket = 0;
	}

	std::string WebService::base64_encode(std::string text)
	{
		std::string ret = "";
		int i = 0;
		int j = 0;
		BYTE char_array_3[3];
		BYTE char_array_4[4];

		int buflen = (int)text.length();
		const char* buf = text.c_str();
		while (buflen--)
		{
			char_array_3[i++] = *(buf++);
			if (i == 3)
			{
				char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
				char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
				char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
				char_array_4[3] = char_array_3[2] & 0x3f;

				for (i = 0; (i <4); i++)
				{
					ret += BASE64_CHARS[char_array_4[i]];
				}
				i = 0;
			}
		}

		if (i)
		{
			for (j = i; j < 3; j++)
			{
				char_array_3[j] = '\0';
			}
			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;

			for (j = 0; (j < i + 1); j++)
			{
				ret += BASE64_CHARS[char_array_4[j]];
			}
			while ((i++ < 3))
			{
				ret += '=';
			}
		}
		return ret;
	}

	std::string WebService::base64_decode(std::string text)
	{
		std::string ret = "";
		int in_len = (int)text.size();
		int i = 0;
		int j = 0;
		int in_ = 0;
		BYTE char_array_4[4], char_array_3[3];

		while ((in_len--) && (text[in_] != '=') && ((isalnum(text[in_])) || (text[in_] == '+') || (text[in_] == '/')))
		{
			char_array_4[i++] = text[in_]; in_++;
			if (i == 4)
			{
				for (i = 0; i < 4; i++)
				{
					char_array_4[i] = (BYTE)BASE64_CHARS.find(char_array_4[i]);
				}
				char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
				char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
				char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

				for (i = 0; (i < 3); i++)
				{
					ret += char_array_3[i];
				}
				i = 0;
			}
		}
		if (i)
		{
			for (j = i; j < 4; j++)
			{
				char_array_4[j] = 0;
			}
			for (j = 0; j < 4; j++)
			{
				char_array_4[j] = (BYTE)BASE64_CHARS.find(char_array_4[j]);
			}
			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

			for (j = 0; (j < i - 1); j++)
			{
				ret += char_array_3[j];
			}
		}
		return ret;
	}
}
