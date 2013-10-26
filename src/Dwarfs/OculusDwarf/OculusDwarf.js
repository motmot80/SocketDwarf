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

OculusDwarf.prototype = new SocketDwarf("Oculus");

function OculusDwarf() {
    var that = this;
}

OculusDwarf.prototype.onDwarfConnected = null;

OculusDwarf.prototype.deviceInfo = null;

OculusDwarf.prototype.orientation = null;

OculusDwarf.prototype.onGetOrientation = null;

OculusDwarf.prototype.onGetInfo = null;

OculusDwarf.prototype.onDeviceConnected = null;

OculusDwarf.prototype.onDeviceDisconnected = null;

OculusDwarf.prototype.getOrientation = function () {
    console.log("OculusDwarf >> request: GetInfo");
    var data = {
        "uid": this.generateUid(),
        "command": "GetOrientation"        
    };
    this.sendMessage(JSON.stringify(data));
}

OculusDwarf.prototype.getInfo = function () {
    console.log("OculusDwarf >> request: GetInfo");
    var data = {
        "uid": this.generateUid(),
        "command": "GetInfo"
    };
    this.sendMessage(JSON.stringify(data));
}

OculusDwarf.prototype.onError = function (event) {
    this.isDwarfConnected = false;
}

OculusDwarf.prototype.onOpen = function (event) {
    if (this.onDwarfConnected != null) {
        this.onDwarfConnected();
    }
}

OculusDwarf.prototype.onClose = function (event) {
    if (this.onDwarfDisconnected != null) {
        this.onDwarfDisconnected();
    }
}

OculusDwarf.prototype.onMessage = function (event) {
    var that = this;
    data = JSON.parse(event.data);
    if (data.devicestate != null && data.devicestate.connected != null) {
        if (this.isDwarfConnected && !data.devicestate.connected && this.onDeviceDisconnected != null) {
            console.log("OculusDwarf >> Device disconnected");
            this.onDeviceDisconnected();
        }
        else if (!this.isDwarfConnected && data.devicestate.connected && this.onDeviceConnected != null) {
            console.log("OculusDwarf >> Device connected");
            this.onDeviceConnected();
            if (this.isContinuousOrientationUpdate) {
                this.getOrientation();
            }
        }
        this.isDwarfConnected = data.devicestate.connected;
    }
    if (data.command != null) {
        console.log("OculusDwarf >> response: " + data.command);
        if (data.data != null) {
            if (data.command == "GetInfo") {
                this.deviceInfo = data;
                if (this.onGetInfo != null) {
                    this.onGetInfo(data.data);
                }
            }
            else if (data.command == "GetOrientation") {
                this.orientation = data;
                if (this.onGetOrientation != null) {
                    this.onGetOrientation(data.data);
                }
            }
        }
    }
}

