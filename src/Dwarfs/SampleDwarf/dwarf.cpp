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

#include <string>
#include "../include/dwarf.hpp"

Export const std::string ProcessRequest (const std::string & data)
{
    Json::Value requestRoot;   
    Json::Reader requestReader;
    if (requestReader.parse(data, requestRoot)) {
        std::string command = requestRoot.get("command", std::string()).asString();
        if (command == "echo")
        {
            std::string data = requestRoot.get("data", std::string()).asString();
            Json::Value responseRoot;
            responseRoot["data"] = data;
            std::ostringstream output;
            output << responseRoot;
            return output.str();
        }        
    }
    return std::string();
}

