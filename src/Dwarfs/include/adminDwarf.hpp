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

#include "../../Socket/function.hpp"
#include "dwarf.hpp"

namespace {

#define GETSTATEFUNC Helper::Function<void (::DwarfState &)>
#define ADMIN_NOTIFY_NEW_DWARF(DWARFNAME,STATEFUNC) Export void NotifyNewDwarf (std::string const & DWARFNAME, Helper::Function<void (::DwarfState &)> STATEFUNC)

};
