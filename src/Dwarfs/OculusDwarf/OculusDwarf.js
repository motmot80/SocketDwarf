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


//EARLY DRAFT

function OculusDwarf() {
    
}

OculusDwarf.prototype.getOrientation = function (callback) {
    //TODO implement callback
    var data = {
        "command": "GetOrientation"
    };
    this.sendMessage(JSON.stringify(data));
}

OculusDwarf.prototype.getInfo = function (callback) {
    //TODO implement callback
    var data = {
        "command": "GetInfo"
    };
    this.sendMessage(JSON.stringify(data));
}

OculusDwarf.prototype.onMessage = function (event) {
    var that = this;
    data = JSON.parse(event.data);
    var message = data;
    console.log("echo received '" + message);
}

OculusDwarf.prototype = new OculusDwarf("Oculus");
