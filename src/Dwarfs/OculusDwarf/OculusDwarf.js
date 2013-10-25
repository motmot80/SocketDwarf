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
    this.isDeviceConnected = false;
    this.isContinuousOrientationUpdate = false;
    this.deviceInfo = null;
    this.orientation = null;
    this.onGetOrientation = null;
    this.onGetInfo = null;
    this.onDeviceConnected = null;
    this.onDeviceDisconnected = null;
}

OculusDwarf.prototype = new SocketDwarf("Oculus");

OculusDwarf.prototype.registerOrientation = function () {
    console.log("registerOrientation");
    this.isContinuousOrientationUpdate = true;
    this.getOrientation();
}

OculusDwarf.prototype.unregisterOrientation = function () {
    console.log("unregisterOrientation");
    this.isContinuousOrientationUpdate = false;
}

OculusDwarf.prototype.getOrientation = function () {
    console.log("getOrientation");
    var data = {
        "uid": this.generateUid(),
        "command": "GetOrientation"        
    };
    this.sendMessage(JSON.stringify(data));
}

OculusDwarf.prototype.getInfo = function () {
    console.log("getInfo");
    var data = {
        "uid": this.generateUid(),
        "command": "GetInfo"
    };
    this.sendMessage(JSON.stringify(data));
}

OculusDwarf.prototype.onError = function (event) {
    console.log("onError");
    if (onDeviceDisconnected != undefined) {
        onDeviceDisconnected();
    }
    isDeviceConnected = false;
}

OculusDwarf.prototype.onOpen = function (event) {
    if (this.isContinuousOrientationUpdate) {
        console.log("Continue getOrientation");
        getOrientation();
    }
}

OculusDwarf.prototype.onMessage = function (event) {
    var that = this;
    data = JSON.parse(event.data);
    if (data.devicestate != undefined && data.devicestate.connected != undefined) {
        if (this.isDeviceConnected && !data.devicestate.connected && this.onDeviceDisconnected != undefined) {
            console.log("Device disconnected");
            this.onDeviceDisconnected();
        }
        else if (!this.isDeviceConnected && data.devicestate.connected && this.onDeviceConnected != undefined) {
            console.log("Device connected");
            this.onDeviceConnected();
            if (this.isContinuousOrientationUpdate) {
                this.getOrientation();
            }
        }
        this.isDeviceConnected = data.devicestate.connected;
    }
    if (data.data != undefined) {
        if (data.command == "GetInfo") {
            this.deviceInfo = data;
            if (this.onGetInfo != undefined) {
                this.onGetInfo(data);
            }
        }
        else if (data.command == "GetOrientation") {
            this.orientation = data;
            if (this.onGetOrientation != undefined) {
                this.onGetOrientation(data);
            }
            if (this.isContinuousOrientationUpdate) {
                this.getOrientation();
            }
        }
    }
}

