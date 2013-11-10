//
//  SocketDwarf++ - A multiplatform HTML 5 peripherals adapter
//  
//  Copyright (C) 2013 Nico Thomaier
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

var echoTick = 0;

SampleDwarf.prototype = new SocketDwarf("Sample");

function SampleDwarf() {
    var that = this;
}

SampleDwarf.prototype.echo = function (message) {
    echoTick = (new Date()).getTime();
    console.log("message send '" + message + "'");
    var data = {
        "command": "echo",
        "data": message
    };
    this.sendMessage(JSON.stringify(data));
}

SampleDwarf.prototype.onMessage = function (event) {
    var that = this;
    data = JSON.parse(event.data);
    var message = data.data;
    console.log("echo received '" + this.message + "' (" + ((new Date()).getTime() - echoTick) + " msec)");
    this.echo(message);
}
