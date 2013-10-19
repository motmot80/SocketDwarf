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
#include "library.hpp"

namespace Helper {

    template <typename Signature> struct Function
    {
    public:
                                Function(const Library & library, const std::string & name)
                                {
#ifdef WIN32
                                    func = reinterpret_cast<Signature *>(::GetProcAddress(library.getHandle(), name.c_str()));
#else
                                    func = reinterpret_cast<Signature *>(::dlsym(library.getHandle(), name.c_str()));
#endif
                                }
        operator Signature      *() {  return func; }
    private:
        Signature *             func;
    };   

}
