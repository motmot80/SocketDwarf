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

# static library directories
cmake_policy(SET CMP0015 NEW)

link_directories("../../../jsoncpp/lib")
include_directories("../../../jsoncpp/include")

# preprocessor definitions
add_definitions(-DUSE_WEBSOCKET)
add_definitions(-DBOOST_PROCESS_WINDOWS_USE_NAMED_PIPE)

add_library (${projectName} server.cpp server.hpp serversettings.cpp serversettings.hpp session.cpp session.hpp mongoose.c mongoose.h library.cpp library.hpp function.cpp function.hpp)
set(LIBRARY_OUTPUT_PATH ../../build/lib)

add_executable(${projectName}Server main.cpp settings.json)
set(EXECUTABLE_OUTPUT_PATH ../../build/bin)

add_custom_command( TARGET ${projectName}Server
                    POST_BUILD 
                    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/deploy
                    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/deploy/docroot
                    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${projectName}Server>/docroot
                    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/settings.json $<TARGET_FILE_DIR:${projectName}Server>
                    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/settings.json ${CMAKE_SOURCE_DIR}/deploy
                    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/SocketDwarf.js $<TARGET_FILE_DIR:${projectName}Server>/docroot
                    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/SocketDwarf.js ${CMAKE_SOURCE_DIR}/deploy/docroot
                    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${projectName}Server> ${CMAKE_SOURCE_DIR}/deploy)

#static library declarations
IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	target_link_libraries(${projectName})
	target_link_libraries(${projectName}Server ${projectName} json pthread dl)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

IF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	target_link_libraries(${projectName} Rpcrt4)
	target_link_libraries(${projectName}Server ${projectName} json.lib Kernel32 Rpcrt4)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
