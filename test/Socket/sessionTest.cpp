//
//  SocketDwarf++ - A multiplatform HTML 5 peripherals adapter
//  
//  Copyright (C) 2013 Thomas Stollenwerk
//  
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//  


#include <cppunit/config/SourcePrefix.h>
#include <server.hpp>
#include "sessionTest.hpp"

#ifdef WIN32
#include <winsock.h>
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
void closesocket(int socket) { close(socket); }
#endif

CPPUNIT_TEST_SUITE_REGISTRATION(SessionTestFixture );

/*
WEBSOCKET DATAGRAM see http://tools.ietf.org/html/rfc6455#page-38
      0                   1                   2                   3
      0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
     +-+-+-+-+-------+-+-------------+-------------------------------+
     |F|R|R|R| opcode|M| Payload len |    Extended payload length    |
     |I|S|S|S|  (4)  |A|     (7)     |             (16/64)           |
     |N|V|V|V|       |S|             |   (if payload len==126/127)   |
     | |1|2|3|       |K|             |                               |
     +-+-+-+-+-------+-+-------------+ - - - - - - - - - - - - - - - +
     |     Extended payload length continued, if payload len == 127  |
     + - - - - - - - - - - - - - - - +-------------------------------+
     |                               |Masking-key, if MASK set to 1  |
     +-------------------------------+-------------------------------+
     | Masking-key (continued)       |          Payload Data         |
     +-------------------------------- - - - - - - - - - - - - - - - +
     :                     Payload Data continued ...                :
     + - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - +
     |                     Payload Data continued ...                |
     +---------------------------------------------------------------+
     */

void SessionTestFixture::ServerIsUp()
{
    std::unique_ptr<SocketDwarf::Server::DwarfServer> server (new SocketDwarf::Server::DwarfServer ());
    server->Start(serverPort);
    int thisSocket;
    struct sockaddr_in destination;
	destination.sin_family = AF_INET;
	thisSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 
    CPPUNIT_ASSERT(thisSocket != 0);
    try {
        destination.sin_port = htons(serverPort);
	    destination.sin_addr.s_addr = inet_addr("127.0.0.1");
        CPPUNIT_ASSERT(connect(thisSocket,(struct sockaddr *)&destination,sizeof(destination))==0);
        closesocket(thisSocket);
    }
    catch (...) {
        closesocket(thisSocket);
        throw;
    }
}

void SessionTestFixture::ServerIsRespondingWithWebSocket()
{
    std::unique_ptr<SocketDwarf::Server::DwarfServer> server (new SocketDwarf::Server::DwarfServer ());
    server->Start(serverPort);
    int thisSocket;
    struct sockaddr_in destination;
	destination.sin_family = AF_INET;
	thisSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 
    CPPUNIT_ASSERT(thisSocket != 0);
    try {
        destination.sin_port = htons(serverPort);
	    destination.sin_addr.s_addr = inet_addr("127.0.0.1");
        CPPUNIT_ASSERT(connect(thisSocket,(struct sockaddr *)&destination,sizeof(destination))==0);
        CPPUNIT_ASSERT(send(thisSocket, requestData.c_str(), requestData.length(), 0));
        const int BUFFERSIZE = 1000;
        char buffer [BUFFERSIZE];
        int recvDataLength = recv(thisSocket, buffer, BUFFERSIZE-1, 0);
        buffer [recvDataLength] = 0;
        std::string responseData (buffer);
        std::string const REFERENCERESPONSEDATA ("HTTP/1.1 101 Switching Protocols");
        CPPUNIT_ASSERT(responseData.find(REFERENCERESPONSEDATA) != std::string::npos);        
        closesocket(thisSocket);
    }
    catch (...) {
        closesocket(thisSocket);
        throw;
    }
}

void SessionTestFixture::SampleDwarfIsRespondingThroughWebSocket()
{
    std::unique_ptr<SocketDwarf::Server::DwarfServer> server (new SocketDwarf::Server::DwarfServer ());
    server->Start(serverPort);
    int thisSocket;
    struct sockaddr_in destination;
	destination.sin_family = AF_INET;
	thisSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 
    CPPUNIT_ASSERT(thisSocket != 0);
    try {
        destination.sin_port = htons(serverPort);
	    destination.sin_addr.s_addr = inet_addr("127.0.0.1");
        CPPUNIT_ASSERT(connect(thisSocket,(struct sockaddr *)&destination,sizeof(destination))==0);
        CPPUNIT_ASSERT(send(thisSocket, requestData.c_str(), requestData.length(), 0));
        const int BUFFERSIZE = 1000;
        char buffer [BUFFERSIZE];
        int recvDataLength = recv(thisSocket, buffer, BUFFERSIZE-1, 0);
        buffer [recvDataLength] = 0;
        std::string responseData (buffer);
        std::string const REFERENCERESPONSEDATA ("HTTP/1.1 101 Switching Protocols");
        CPPUNIT_ASSERT(responseData.find(REFERENCERESPONSEDATA) != std::string::npos);
        std::string const requestPayload = "\x81\x06\D\E\M\O\n\n"; //see websocket datagram flags+opcode+length+payload
        CPPUNIT_ASSERT(send(thisSocket, requestPayload.c_str(), requestPayload.length(), 0));
        recvDataLength = recv(thisSocket, buffer, BUFFERSIZE-1, 0);
        buffer [recvDataLength] = 0;
        std::string responsePayload (buffer);
        std::string const REFERENCERESPONSEPAYLOAD ("DEMO");
        CPPUNIT_ASSERT(responsePayload.find(REFERENCERESPONSEPAYLOAD) != std::string::npos);
        closesocket(thisSocket);
    }
    catch (...) {
        closesocket(thisSocket);
        throw;
    }
}


void SessionTestFixture::setUp()
{
#ifdef WIN32
    WSADATA wsaData;
    WSAStartup(0x0202, &wsaData);
#endif
    serverPort = 8080;
    requestData =   "GET / HTTP/1.1\n"
                    "Origin: null\n"
                    "Sec-WebSocket-Protocol: SampleDwarf\n"
                    "Sec-WebSocket-Key: CJgV+uoBFwiAadsp6JsxJw==\n"
                    "Connection: Upgrade\n"
                    "Upgrade: Websocket\n"
                    "Sec-WebSocket-Version: 13\n"
                    "User-Agent: Mozilla/5.0 (compatible; MSIE 10.0; Windows NT 6.2; WOW64; Trident/6.0)\n"
                    "Host: localhost:8080\n"
                    "DNT: 1\n"
                    "Cache-Control: no-cache\n"
                    "Cookie: Nop.customer=8e3cc00c-b021-4acb-884c-d1883c2d7a61\n"
                    "\n";
}

void SessionTestFixture::tearDown()
{
#ifdef WIN32
    WSACleanup();
#endif
}
