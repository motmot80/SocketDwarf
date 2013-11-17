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
#include "../Dwarfs/include/dwarf.hpp"

/** 
    All loaded shared libraries
*/
SocketDwarf::Server::LibraryMap SocketDwarf::Server::Session::Libraries;
SocketDwarf::Server::LibraryProbingPathList SocketDwarf::Server::Session::LibraryProbingPaths;
SocketDwarf::Server::AdminDwarfList SocketDwarf::Server::Session::AdminDwarfs;

/** 
    Sets probing paths for libraries
*/
void SocketDwarf::Server::Session::SetProbingPaths (LibraryProbingPathList const & probingPaths)
{
    LibraryProbingPaths = probingPaths;
}

/** 
    Sets valid admin dwarfs
*/
void SocketDwarf::Server::Session::SetAdminDwarfs (LibraryProbingPathList const & adminDwarfs)
{
    AdminDwarfs = adminDwarfs;
}

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
    const Helper::Library * library = GetDwarfLibraryByName (Protocol);
    if (library == 0) {
        library = LoadDwarfLibraryByName(Protocol);
        if (library != 0) {
            OnInitializeDwarf (Protocol, *library);
        }
    }
    if (library != 0)
    {
        //TODO: Transfer callback func for duplex communication
        Helper::Function<void (const std::string &)> startFunc (* library, "Start");
        if (startFunc.IsValid()) {
            startFunc(GetSessionId());
        }
    }
    else {
        std::cerr << "Could not load dwarf library '" << Protocol << "'";
    }
}

/** 
    Stops the session
*/
void SocketDwarf::Server::Session::Stop ()
{
    //Cannot close connection directly
    const Helper::Library * library = GetDwarfLibraryByName (Protocol);
    if (library != 0)
    {
        Helper::Function<void ()> freeFunc (* library, "Free");
        if (freeFunc.IsValid()) {
            freeFunc();
        }
    }
    else {
        std::cerr << "Could not load dwarf library '" << Protocol << "'";
    }
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
    const Helper::Library * library = GetDwarfLibraryByName (Protocol);
    if (library != 0)
    {
        Helper::Function<const std::string (const std::string &)> processRequestFunc (* library, "ProcessRequest");
        if (processRequestFunc.IsValid()) {
            std::string responseData = processRequestFunc(data);
            if (!responseData.empty())
            {
                OnDwarfDataReceived (responseData);
            }
        }
    }
    else {
        std::cerr << "Could not load dwarf library '" << Protocol << "'";
    }
    return WEBSOCKET_OPCODE_TEXT;
}

/** 
    Returns a shared library by its name.
*/
const Helper::Library * SocketDwarf::Server::Session::GetDwarfLibraryByName (const std::string & name)
{
    Helper::Library * libraryPtr = 0;
    LibraryMap::const_iterator cit = Libraries.find(name);
    if (cit != Libraries.end())
    {
        return cit->second.get ();
    }
    return 0;
}

/** 
    Loads a shared library by its name. Runtime exception if loaded
*/
const Helper::Library * SocketDwarf::Server::Session::LoadDwarfLibraryByName (const std::string & name)
{
    const std::string libraryName = name + "Dwarf";
    if (GetDwarfLibraryByName (name) != 0) {
        throw std::runtime_error ("library '" + libraryName + "' already loaded"); 
    }
    Helper::Library * libraryPtr = 0;
    try {
        libraryPtr = new Helper::Library(libraryName, LibraryProbingPaths);
    }
    catch (...) {
        return 0;
    }
    Libraries.insert(Libraries.begin(), std::pair<std::string, std::unique_ptr<Helper::Library>> (name, std::unique_ptr<Helper::Library>(libraryPtr)));
    return libraryPtr;
}

/** 
    Initializes a dwarf and notifies all admin dwarfs
*/
void SocketDwarf::Server::Session::OnInitializeDwarf (std::string const & dwarfName, Helper::Library const & dwarfLibrary)
{
    Helper::Function<void ()> initFunc (dwarfLibrary, "Init");
    if (initFunc.IsValid()) {
        initFunc();
    }
    AdminNotifyNewDwarf(dwarfName, dwarfLibrary);    
}

/** 
    Notifies all admin dwarfs about the new dwarf
*/
void SocketDwarf::Server::Session::AdminNotifyNewDwarf (std::string const & dwarfName, Helper::Library const & dwarfLibrary)
{
    Helper::Function<void (::DwarfState &)> dwarfGetStateFunc (dwarfLibrary, "GetState");
    if (!dwarfGetStateFunc.IsValid())
    {
        std::cerr << "Warning: Dwarf'" + dwarfName + "' does not support GetState";
        return;
    }
    for (AdminDwarfList::const_iterator cit = AdminDwarfs.begin(); cit != AdminDwarfs.end(); cit++)
    {
        if (* cit != dwarfName) 
        {
            const Helper::Library * adminDwarfLibrary = GetDwarfLibraryByName (* cit);
            if (adminDwarfLibrary == 0) {
                adminDwarfLibrary = LoadDwarfLibraryByName(* cit);
            }
            if (adminDwarfLibrary != 0) {
                Helper::Function<void(std::string const &, Helper::Function<void (::DwarfState &)>)> notifyNewDwarfFunc (*adminDwarfLibrary, "NotifyNewDwarf");
                if (notifyNewDwarfFunc.IsValid()) {
                    notifyNewDwarfFunc(dwarfName, dwarfGetStateFunc);
                }
            }
            else {
                std::cerr << "Warning: Could not load admin dwarf '" + dwarfName + "'";
            }
        }
    }
}



