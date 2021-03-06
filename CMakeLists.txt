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
cmake_policy(SET CMP0015 NEW)

MESSAGE(STATUS "SocketDwarf++ - CMake")
MESSAGE(STATUS "Platform: " ${CMAKE_SYSTEM_NAME} )
MESSAGE(STATUS "Dependency: JSONCPP-0.6.0")
MESSAGE(STATUS "Dependency: cppunit-1.12")
MESSAGE(STATUS "Dependency: Oculus SDK")
MESSAGE(STATUS "Dependency: Sixense SDK")

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	MESSAGE(STATUS "Adding C++11 support")
	add_definitions(-D__GXX_EXPERMINETAL_CXX0X__=1)
	list( APPEND CMAKE_CXX_FLAGS " -std=c++0x -g -ftest-coverage -fprofile-arcs")
	MESSAGE(STATUS "CMAKE_CXX_FLAGS:           " ${CMAKE_CXX_FLAGS})
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")


add_subdirectory (src/Socket) 
add_subdirectory (test/Socket) 
add_subdirectory (src/Dwarfs/MasterDwarf) 
add_subdirectory (test/Dwarfs/MasterDwarf)
add_subdirectory (src/Dwarfs/SampleDwarf) 
add_subdirectory (test/Dwarfs/SampleDwarf) 
add_subdirectory (src/Dwarfs/OculusDwarf) 
add_subdirectory (test/Dwarfs/OculusDwarf)
add_subdirectory (src/Dwarfs/SixenseDwarf) 
add_subdirectory (test/Dwarfs/SixenseDwarf)
