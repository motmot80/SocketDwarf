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

#define WIN32_LEAN_AND_MEAN

#include <exception>
#include <iostream>   

#include "session.hpp"

/** 
    All loaded shared libraries
*/
SocketDwarf::Server::LibraryMap SocketDwarf::Server::Session::Libraries;

/** 
    Default constructor
*/
SocketDwarf::Server::Session::Session (mg_connection * clientConnection, const std::string & protocol) 
    : ClientConnection(clientConnection),
      Protocol(protocol)
{
    
}

/** 
    Destructor
    Stops the session
*/
SocketDwarf::Server::Session::~ Session ()
{
    try 
    {
        Stop ();
    }
    catch (std::exception & excp)
    {
        std::cerr << "error:" << excp.what() << std::endl;
    }
}

/** 
    Starts the session
*/
void SocketDwarf::Server::Session::Start ()
{

}

/** 
    Stops the session
*/
void SocketDwarf::Server::Session::Stop ()
{
    //Cannot close connection directly
    std::unique_ptr<std::lock_guard<std::mutex>> lock_guard (new std::lock_guard<std::mutex> (* this));
    lock_guard.reset(0);
}

/** 
    Returns the session id    
*/
const std::string SocketDwarf::Server::Session::GetSessionId () const
{
    return Session::GetSessionId(ClientConnection);
}

/** 
    Returns the session id by connection
*/
const std::string SocketDwarf::Server::Session::GetSessionId (mg_connection * conn)
{
    const struct mg_request_info * requestInfo = mg_get_request_info(conn);
    return std::to_string (requestInfo->remote_ip) + std::to_string (requestInfo->remote_port);
}

/** 
    Fired when dwarf sends data
*/
void SocketDwarf::Server::Session::OnDwarfDataReceived (const std::string & data)
{
    mg_websocket_write(ClientConnection, WEBSOCKET_OPCODE_TEXT, data.c_str(), data.length());
}

/** 
    Fired when client sends data
*/
int SocketDwarf::Server::Session::OnClientDataReceived (const std::string & data)
{
    const Helper::Library * library = GetLibraryByName (Protocol);
    if (library != 0)
    {
        Helper::Function<const std::string (const std::string &)> processRequestFunc (* library, "ProcessRequest");
        std::string responseData = processRequestFunc(data);
        if (!data.empty())
        {
            OnDwarfDataReceived (data);
        }
    }
    else {
        std::cerr << "Could not load dwarf library '" << Protocol << "'";
    }
    return WEBSOCKET_OPCODE_CONTINUATION;
}

/** 
    Returns a shared library by its name.
    Shared library is loaded if unknown.
*/
const Helper::Library * SocketDwarf::Server::Session::GetLibraryByName (const std::string & name)
{
    Helper::Library * libraryPtr = 0;
    LibraryMap::const_iterator cit = Libraries.find(name);
    if (cit != Libraries.end())
    {
        return cit->second.get ();
    }
    try {
        libraryPtr = new Helper::Library(name);
    }
    catch (...) {
        return 0;
    }
    Libraries.insert(Libraries.begin(), std::pair<std::string, std::unique_ptr<Helper::Library>> (name, std::unique_ptr<Helper::Library>(libraryPtr)));
    return libraryPtr;
}



