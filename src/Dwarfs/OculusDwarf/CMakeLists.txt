#
#  SocketDwarf++ - A multiplatform HTML 5 peripherals adapter
#  
#  Copyright (C) 2013 Thomas Stollenwerk
#  
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#  
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#  
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <http:#www.gnu.org/licenses/>.
#  

cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
# Above line indicates to Cmake that minimum version 2.8 is required.
# As you may have noted all lines beginning with hash symbol are treated as comments by Cmake.
SET(projectName OculusDwarf)
MESSAGE(STATUS "Creating " ${projectName} "...")

cmake_policy(SET CMP0015 NEW)

link_directories("../../../../jsoncpp/lib")
include_directories("../../../../jsoncpp/include")

link_directories("../../../../LibOVR/lib")
include_directories("../../../../LibOVR/include")


target_link_libraries(${projectName})

# The following line is very important.
add_library (${projectName} SHARED dwarf.cpp OculusDwarf.js)
set(LIBRARY_OUTPUT_PATH ../../../build/bin)

#static library declarations
IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	target_link_libraries(${projectName})
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux" json libovr)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	target_link_libraries(${projectName} json.lib libovr.lib winmm.lib)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

