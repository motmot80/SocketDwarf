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


SixenseDwarf.prototype = new SocketDwarf("Sixense");

function SixenseDwarf() {
    var that = this;
}

SixenseDwarf.prototype.isDebug = false;

SixenseDwarf.prototype.isDwarfConnected = false;

SixenseDwarf.prototype.allNewestData = null;

SixenseDwarf.prototype.onDwarfConnected = null;

SixenseDwarf.prototype.onDwarfDisconnected = null;

SixenseDwarf.prototype.onDeviceConnected = null;

SixenseDwarf.prototype.onDeviceDisconnected = null;

SixenseDwarf.prototype.onGetAllNewestData = null;

SixenseDwarf.prototype.getAllNewestData = function () {
    if (this.isDebug) {
        console.log("SixenseDwarf >> request: GetAllNewestData");
    }
    var data = {
        "uid": this.generateUid(),
        "command": "GetAllNewestData"
    };
    this.sendMessage(JSON.stringify(data));
}

SixenseDwarf.prototype.onError = function (event) {
    this.isDwarfConnected = false;
}

SixenseDwarf.prototype.onOpen = function (event) {
    if (this.onDwarfConnected != null) {
        this.isDwarfConnected = true;
        this.onDwarfConnected();
    }
}

SixenseDwarf.prototype.onClose = function (event) {
    if (this.onDwarfDisconnected != null) {
        this.isDwarfConnected = false;
        this.onDwarfDisconnected();
    }
}

SixenseDwarf.prototype.onMessage = function (event) {
    var that = this;
    data = JSON.parse(event.data);
    if (data.command != null) {
        if (this.isDebug) {
            console.log("SixenseDwarf >> response: " + data.command);
        }
        if (data.data != null) {
            if (data.command == "GetAllNewestData") {
                this.allNewestData = data;
                if (this.onGetAllNewestData != null) {
                    this.onGetAllNewestData(data.data);
                }
            }
        }
    }
}

