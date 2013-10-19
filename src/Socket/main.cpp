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


#include <csignal>
#include <ios>
#include <string>
#include <map>
#include <iostream>   
#include "server.hpp"

namespace
{
    typedef std::map<std::string, std::string> ArgumentMap;

    volatile sig_atomic_t quit;

    void signal_handler(int sig)
    {
        signal(sig, signal_handler);
        quit = 1;
    }
}

int main(int argc, char *argv[]) 
{
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    std::auto_ptr<SocketDwarf::Server::DwarfServer> server (new SocketDwarf::Server::DwarfServer ());
    try {
        std::cout << "SocketDwarf server starting..."  << std::endl;
        server->Start(8080);
        std::cout << "SocketDwarf server started"  << std::endl;
        std::cout << "Press enter to stop server"  << std::endl;
        std::string input;
        std::getline(std::cin, input);
        std::cout << "SocketDwarf server stopping..."  << std::endl;
        server->Stop();
        std::cout << "SocketDwarf server stopped..."  << std::endl;
    }
    catch (std::exception & excp)
    {
        std::cerr << excp.what();
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}