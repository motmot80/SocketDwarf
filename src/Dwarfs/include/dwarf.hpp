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

#include <iostream>     
#include <sstream>      
#include <json/json.h>

#ifdef WIN32
#include <Windows.h>
#endif
#ifdef WIN32
#define Export extern "C" __declspec( dllexport )
#else 
#define Export extern "C" 
#endif

namespace {

enum EDwarfState {
    Error = -1,
    Unknown = 0,
    Loading = 1,
    NoDevice = 2,
    DeviceIdle = 3,
    DeviceReady = 4,
    Unloading = 6
};

struct DwarfState {
public:
    EDwarfState State;
    std::string StateDescription;
    std::string ConnectedDeviceName;
};

#define DWARF_INIT() Export void Init ()
#define DWARF_FREE() Export void Free ()
#define DWARF_GETSTATE(STATE) Export void GetState (DwarfState & STATE)
#define DWARF_PROCESSREQUEST(DATA) Export const std::string ProcessRequest (const std::string & DATA)

};