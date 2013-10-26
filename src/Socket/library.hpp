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

#include <string>
#include <map>
#include <stdexcept>

#ifdef WIN32
#include <Windows.h>
#define LIBRARYHANDLE HINSTANCE  
#define DllExport __declspec( dllexport )
#else 
#include <dlfcn.h>
#define LIBRARYHANDLE void *  
#define DllExport 
#endif

namespace Helper {    

    DllExport struct Library {
    public:
                        Library (std::string const & name)
                        {
                            libraryHandle = NULL;
#ifdef WIN32
                            libraryHandle = ::LoadLibrary((name + std::string(".dll")).c_str());
#else   
                            libraryHandle = ::dlopen((std::string("./lib") + name + std::string(".so")).c_str(), RTLD_LAZY);
#endif
                            if (libraryHandle == NULL)
                            {
                                throw std::runtime_error(name);
                            }
                        }

                      ~ Library ()
                        {
#ifdef WIN32
                            ::FreeLibrary(libraryHandle);
#else 
                            ::dlclose(libraryHandle);
#endif
                        }

        LIBRARYHANDLE   getHandle () const
                        {
                            return libraryHandle;
                        }                    
    private:
        LIBRARYHANDLE   libraryHandle;                       
    };         
}
