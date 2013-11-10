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

#pragma once

#include <map>
#include <vector>
#include <string>
#include <sstream> 
#include <mutex>
#include <thread>
#include <memory>

#include "mongoose.h"
#include "library.hpp"
#include "function.hpp"

#ifdef WIN32
#define Export __declspec( dllexport )
#else 
#define Export 
#endif

namespace SocketDwarf {    

    namespace Server {

        typedef std::map<std::string, std::unique_ptr <Helper::Library>> LibraryMap;
        typedef std::vector<std::string> LibraryProbingPathList;
        typedef std::vector<std::string> AdminDwarfList;

        Export class Session : public std::mutex {
        public:
                                                           Session                 (mg_connection * clientConnection, const std::string & protocol);
                                                         ~ Session                 ();
                    void                                   Start                   ();
                    void                                   Stop                    ();
                    const std::string                      GetSessionId            () const; 
         static     const std::string                      GetSessionId            (mg_connection * conn);
                    const mg_connection *                  GetClientConnection     () const { return ClientConnection; }
                    mg_connection *                        GetClientConnection     () { return ClientConnection; }
         static     void                                   SetProbingPaths         (LibraryProbingPathList const & probingPaths);
         static     void                                   SetAdminDwarfs          (AdminDwarfList const & adminDwarfs);
         virtual    void                                   OnDwarfDataReceived     (const std::string & data);
         virtual    int                                    OnClientDataReceived    (const std::string & data);
         virtual    void                                   OnInitializeDwarf       (std::string const & name, Helper::Library const & dwarfLibrary);
         
         
        private:
         static     const Helper::Library *                GetDwarfLibraryByName   (const std::string & name);
         static     const Helper::Library *                LoadDwarfLibraryByName  (const std::string & name);         
         static     void                                   AdminNotifyNewDwarf     (std::string const & name, Helper::Library const & dwarfLibrary);
         static     LibraryMap                             Libraries;
         static     LibraryProbingPathList                 LibraryProbingPaths;
         static     AdminDwarfList                         AdminDwarfs;         
                    const std::string                      Protocol;
                    mg_connection *                        ClientConnection;    
        };        
    }
}