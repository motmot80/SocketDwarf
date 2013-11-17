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


#include <cppunit/config/SourcePrefix.h>
#include "../../../src/Socket/library.hpp"
#include "../../../src/Socket/function.hpp"
#include "sixenseDwarfTest.hpp"


CPPUNIT_TEST_SUITE_REGISTRATION(SixenseDwarfTestFixture);

void SixenseDwarfTestFixture::GetAllNewestData()
{
	Helper::Library lib ("SixenseDwarf");
    Helper::Function<void ()> initFunction(lib, "Init");
    Helper::Function<void ()> freeFunction(lib, "Free");
    initFunction();
    try {
        Helper::Function<const std::string (const std::string &)> getAllNewestDataFunction(lib, "ProcessRequest");
        const std::string responseData = getAllNewestDataFunction("{\"uid\": 1, \"command\": \"GetAllNewestData\" }");
        //first response is "always" empty
        CPPUNIT_ASSERT(responseData == "\n{\n\t\"command\" : \"GetAllNewestData\",\n\t\"uid\" : 1\n}\n");        
    }
    catch (...)
    {
        freeFunction();
        throw;
    }
    freeFunction();
}

void SixenseDwarfTestFixture::setUp()
{
    
}

void SixenseDwarfTestFixture::tearDown()
{

}
