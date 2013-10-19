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
#include <memory>

#include "mongoose.h"
#include "library.hpp"
#include "function.hpp"
#include "session.hpp"

#ifdef WIN32
#define Export __declspec( dllexport )
#else 
#define Export 
#endif

namespace SocketDwarf {    

    namespace Server {

        typedef std::map<std::string, std::unique_ptr <Session>> SessionMap;

        Export class DwarfServer {

        public:
                                                          DwarfServer           ();
                                                        ~ DwarfServer           ();
                    void				                  Start   				(int port);
                    void				                  Stop                  ();
                    void                                  ProcessDwarfs         ();

        protected:
            static  int                                   OnClientDataReceivedS (mg_connection * conn, int flags, char *data, size_t data_len);
                    int                                   OnClientDataReceived  (mg_connection * conn, const std::string & data);
            static  void                                  OnClientConnectedS    (mg_connection * conn);
                    void                                  OnClientConnected     (mg_connection * conn);                    

        private:
            static  std::map<std::string, std::string>    ExtractHeaderValues   (const mg_request_info * requestInfo);
                    Session *                             CreateSession         (mg_connection * conn);            
                    Session *                             GetSessionById        (const std::string & sessionId);
                    const Session *                       GetSessionById        (const std::string & sessionId) const;
                    SessionMap                            sessions;                   
                    struct mg_callbacks		              callbacks;
                    struct mg_context *		              ctx;
        };
    }
}