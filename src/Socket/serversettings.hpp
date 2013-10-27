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

#ifdef WIN32
#define Export __declspec( dllexport )
#else 
#define Export 
#endif

namespace SocketDwarf {    

    namespace Server {
        
        Export struct DwarfServerSettings {
            public:
                        std::string                 ServerPort;
                        std::vector<std::string>    LibraryProbingPaths;
                        std::string                 DocumentRoot;
        };
    }
}