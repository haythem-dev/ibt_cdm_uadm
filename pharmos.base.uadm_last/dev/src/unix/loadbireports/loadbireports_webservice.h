//----------------------------------------------------------------------------
//! \file  loadbireports_webservice.h
// \brief  declarations of webservice for bi report download
// \author Thomas Arnold
// \date   05.10.2023
//----------------------------------------------------------------------------

#ifndef GUARD_LOADBIREPORTS_WEBSERVICE_H
#define GUARD_LOADBIREPORTS_WEBSERVICE_H

#if _AIX
#define closesocket(fd) ::close(fd)
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#else
#include <winsock.h>
#endif
#include <string>
#include <stdio.h>

#if !defined(UINT_PTR) || !defined(BYTE)
#include <stdint.h> // uint8_t, uintptr_t
#endif

#ifndef BYTE
#define BYTE     uint8_t
#endif

#ifndef UINT_PTR
#define UINT_PTR uintptr_t
#endif

//--------------------------------------------------------------------------------------------------//
// class declaration section
//--------------------------------------------------------------------------------------------------//
namespace loadbireports
{
	static const int HTTP_GET = 0;
	static const int HTTP_POST = 1;

	static const int MAX_BUFFER = 1024;

	static const std::string BASE64_CHARS = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	static const int SOCKET_READ_TIMEOUT_SEC = 5;

	class WebService
	{
	public:
		WebService();
		~WebService();

		bool connect(std::string host, int port);
		bool authorize(std::string authschema, std::string credentials);
		bool request(std::string type, std::string request, std::string body, std::string bodytype);
		bool close();

		std::string getLastRequest();
		std::string getLastResponse();

	private:
		void clearSocket();
		std::string readResponse();
		std::string base64_encode(std::string text);
		std::string base64_decode(std::string text);

		int								m_intPort;
		std::string						m_strHost;
		bool							m_blnConnected;
		UINT_PTR						m_intSocket;
		std::string						m_strHTTPRequest;
		std::string						m_strHTTPResponse;
		std::string						m_strAuthSchema;
		std::string						m_strCredentials;
	};
}

#endif // GUARD_LOADBIREPORTS_WEBSERVICE_H
