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

#ifdef __linux__
#include <string.h>
#endif

#define WIN32_LEAN_AND_MEAN

#include <iostream>   


#include "server.hpp"

namespace {
    /**
        Server settings
    */
    static const char * options [] = { "listening_ports", "8080", "document_root", "websocket_html_root", NULL };
};

/**
    Default constructor
 */
SocketDwarf::Server::DwarfServer::DwarfServer()
{
    ctx = 0;
    memset(&callbacks, 0, sizeof(callbacks));
}

/**
    Destructor
    Stops the server
 */
SocketDwarf::Server::DwarfServer::~ DwarfServer()
{
    Stop ();
}

/**
    Starts the server
 */
void SocketDwarf::Server::DwarfServer::Start(int port)
{
    const char * options [] = { "listening_ports", "8080", "document_root", "websocket_html_root", NULL };
        
    callbacks.websocket_ready = OnClientConnectedS;
    callbacks.websocket_data = OnClientDataReceivedS;
    
    //Start server
    if ((ctx = mg_start(&callbacks, this, options)) == NULL) {
        throw std::runtime_error("Cannot start SocketDwarf server, fatal exit (already running?)");
    }
}

/**
    Stops the server
 */
void SocketDwarf::Server::DwarfServer::Stop()
{
    sessions.clear();
    if (ctx != 0)
    {
        mg_stop(ctx);
        ctx = 0;
    }
}

/**
    Fired when client sends data
 */
int SocketDwarf::Server::DwarfServer::OnClientDataReceivedS(struct mg_connection * conn, int flags, char *data, size_t data_len)
{
    try {
        const struct mg_request_info * requestInfo = mg_get_request_info(conn);
        DwarfServer * server = static_cast<DwarfServer*>(requestInfo->user_data);
        if (server == 0)
        {
            throw new std::runtime_error("internal error: invalid server");
        }
        std::string managedData;
        managedData.assign(data, data_len);
        return server->OnClientDataReceived(conn, managedData);
    }
    catch (std::exception & excp)
    {
        std::cerr << excp.what();
    }
    return WEBSOCKET_OPCODE_CONNECTION_CLOSE;
}

/**
    Fired when client sends data
 */
int SocketDwarf::Server::DwarfServer::OnClientDataReceived(struct mg_connection * conn, const std::string & data)
{
    const std::string & sessionId = Session::GetSessionId (conn);
    Session * session = GetSessionById (sessionId);
    if (session == 0)
        return WEBSOCKET_OPCODE_CONNECTION_CLOSE;
    std::lock_guard<std::mutex> lock(* session);    
    return session->OnClientDataReceived (data);
}

/**
    Fired when client is connected
 */
void SocketDwarf::Server::DwarfServer::OnClientConnectedS(struct mg_connection * conn)
{
    try {
        const struct mg_request_info * requestInfo = mg_get_request_info(conn);
        DwarfServer * server = static_cast<DwarfServer*>(requestInfo->user_data);
        if (server == 0)
        {
            throw new std::runtime_error("internal error: invalid server");
        }
        server->OnClientConnected(conn);
    }
    catch(std::exception & excp)
    {
        std::cerr << excp.what();
    }
}

/**
    Fired when client is connected
 */
void SocketDwarf::Server::DwarfServer::OnClientConnected(struct mg_connection * conn)
{
    Session * session = CreateSession(conn);        
}

/**
    Extracts key-vals from HTTP header
 */
std::map<std::string, std::string> SocketDwarf::Server::DwarfServer::ExtractHeaderValues (const struct mg_request_info * requestInfo)
{
    std::map<std::string, std::string> headerValues; 
    for (int i = 0; i < requestInfo->num_headers; i++)
    {
        headerValues.insert(headerValues.begin(), std::pair<std::string, std::string> (requestInfo->http_headers [i].name, requestInfo->http_headers [i].value));
    }
    return headerValues;
}

/**
    Creates a new session
 */
SocketDwarf::Server::Session * SocketDwarf::Server::DwarfServer::CreateSession (struct mg_connection * clientConnection)
{
    const struct mg_request_info * requestInfo = mg_get_request_info(clientConnection);
    std::map<std::string, std::string> headerValues (ExtractHeaderValues(requestInfo));
    std::map<std::string, std::string>::const_iterator citWebSocketProtocol = headerValues.find("Sec-WebSocket-Protocol");
    if (citWebSocketProtocol == headerValues.end())
    {
        throw std::runtime_error("Sec-WebSocket-Protocol header not defined");
    }    
    Session * session = new Session (clientConnection, citWebSocketProtocol->second);
    sessions.insert(sessions.begin(), std::make_pair(session->GetSessionId(), std::unique_ptr<Session>(session)));
    session->Start();
    return session;
}

/**
    Returns session by id
 */
SocketDwarf::Server::Session * SocketDwarf::Server::DwarfServer::GetSessionById (const std::string & sessionId)
{
    std::map<std::string, std::unique_ptr <Session>>::iterator it = sessions.find(sessionId);
    if (it != sessions.end())
    {
        return it->second.get();
    }
    return 0;
}
 
/**
    Returns session by id
 */
const SocketDwarf::Server::Session * SocketDwarf::Server::DwarfServer::GetSessionById (const std::string & sessionId) const
{
    std::map<std::string, std::unique_ptr <Session>>::const_iterator cit = sessions.find(sessionId);
    if (cit != sessions.end())
    {
        return cit->second.get();
    }
    return 0;
}
