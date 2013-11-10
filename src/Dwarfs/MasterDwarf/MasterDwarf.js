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

MasterDwarf.prototype = new SocketDwarf("Master");

function MasterDwarf() {
    var that = this;
}

MasterDwarf.prototype.getDwarfStates = function (message) {
    var data = {
        "command": "GetDwarfStates"
    };
    if (this.isDebug) {
        console.log("MasterDwarf >> request: " + data.command);
    }
    this.sendMessage(JSON.stringify(data));
}

MasterDwarf.prototype.isDebug = false;

MasterDwarf.prototype.dwarfStates = null;

MasterDwarf.prototype.isDwarfConnected = false;

MasterDwarf.prototype.onDwarfConnected = null;

MasterDwarf.prototype.onDwarfDisconnected = null;

MasterDwarf.prototype.onGetDwarfStates = null;


MasterDwarf.prototype.onError = function (event) {
    this.isDwarfConnected = false;
}

MasterDwarf.prototype.onOpen = function (event) {
    if (this.onDwarfConnected != null) {
        this.onDwarfConnected();
    }
}

MasterDwarf.prototype.onClose = function (event) {
    if (this.onDwarfDisconnected != null) {
        this.onDwarfDisconnected();
    }
}

MasterDwarf.prototype.onMessage = function (event) {
    var that = this;
    data = JSON.parse(event.data);
    if (data.command != null) {
        if (this.isDebug) {
            console.log("MasterDwarf >> response: " + data.command);
        }
        if (data.data != null) {
            this.dwarfStates = data;
            if (data.command == "GetDwarfStates") {
                if (this.onGetDwarfStates != null) {
                    this.onGetDwarfStates(data);
                }
            }            
        }
    }
}
