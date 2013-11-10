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
#include "../include/adminDwarf.hpp"

namespace {
    std::map<std::string, GETSTATEFUNC> dwarfGetStateMap;
};

ADMIN_NOTIFY_NEW_DWARF(dwarfName, dwarfGetStateFunc)
{
    dwarfGetStateMap.insert(dwarfGetStateMap.begin(), std::pair<std::string, GETSTATEFUNC>(dwarfName, dwarfGetStateFunc));
}

DWARF_PROCESSREQUEST(data)
{
    Json::Value requestRoot;   
    Json::Reader requestReader;
    if (requestReader.parse(data, requestRoot)) {
        std::string command = requestRoot.get("command", std::string()).asString();
        if (command == "GetDwarfStates")
        {
            Json::Value responseRoot;
            responseRoot["command"] = command;
            responseRoot["data"] = Json::Value(Json::arrayValue);
            for (std::map<std::string, GETSTATEFUNC>::iterator it = dwarfGetStateMap.begin (); it != dwarfGetStateMap.end(); it++)
            {
                DwarfState currentState;
                it->second(currentState);
                Json::Value dwarfRoot;
                dwarfRoot["dwarf"] = it->first;
                dwarfRoot["state"] = currentState.State;
                dwarfRoot["connectedDeviceName"] = currentState.ConnectedDeviceName;
                dwarfRoot["stateDescription"] = currentState.StateDescription;
                responseRoot["data"].append(dwarfRoot);
            }
            std::ostringstream output;
            output << responseRoot;
            return output.str();
        }
    }
    return std::string();
}

DWARF_GETSTATE(currentState)
{
    currentState.State = EDwarfState::DeviceReady;
    currentState.ConnectedDeviceName = std::string();
    currentState.StateDescription = "ready";
}