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
#include "sampleDwarfTest.hpp"


CPPUNIT_TEST_SUITE_REGISTRATION(SampleDwarfTestFixture);

void SampleDwarfTestFixture::DwarfStartable()
{
	Helper::Library lib ("SampleDwarf");
    Helper::Function<const std::string (const std::string &)> echoFunction(lib, "ProcessRequest");
    const std::string responseData = echoFunction("{\"command\": \"echo\", \"data\": \"hello world\"}");
    CPPUNIT_ASSERT(responseData == "\n{\n\t\"data\" : \"hello world\"\n}\n");
}

void SampleDwarfTestFixture::setUp()
{

}

void SampleDwarfTestFixture::tearDown()
{

}
