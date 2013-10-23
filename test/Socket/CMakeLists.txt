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
SET(projectName DwarfSocket)
MESSAGE(STATUS "Creating " ${projectName} "...")

# The following line is very important.

# include dirs
include_directories("../../src/Socket")
include_directories("../../../cppunit/include")

# static library directories
cmake_policy(SET CMP0015 NEW)
link_directories("../../../boost/stage/lib")
link_directories("../../../cppunit/lib")

# preprocessor definitions
add_definitions(-DUSE_WEBSOCKET)
add_definitions(-DBOOST_PROCESS_WINDOWS_USE_NAMED_PIPE)

add_executable(${projectName}Test main.cpp sessionTest.cpp sessionTest.hpp)
set(EXECUTABLE_OUTPUT_PATH ../../build/bin)

#static library declarations
IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	target_link_libraries(${projectName}Test ${projectName} pthread dl boost_system cppunit)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

IF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	target_link_libraries(${projectName}Test ${projectName} Rpcrt4 cppunitd)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
