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

function SocketDwarf(protocol) {
    var that = this;
    var autoReconnect = true;
	var backendURI = "ws://localhost:8080";
	var websocket;
	var protocolType = protocol;

	this.generateUid = function (message) {
	    return Math.floor(Math.random() * 1000000001);
	}

	this.sendMessage = function (message) {
	    websocket.send(message);
	}
	
	this.destroy = function () {
	    autoReconnect = false;
	    websocket.close();
	}
    
	this.init = function () {
	    console.log("Connecting to '" + backendURI + "'...");
	    autoReconnect = true;
	    websocket = new WebSocket(backendURI, protocolType);
	    websocket.onopen = that.onOpen;
	    websocket.onmessage = that.onMessage;
	    websocket.onerror = that.onError;
	    websocket.onclose = that.onClose;
	}
	
	this.onError = function () {
	    console.log("Error on '" + backendURI + "'");
	}
    
	this.onOpen = function () {
	    console.log("Connected to '" + backendURI + "'");
	}

	this.onClose = function () {
	    console.log("Disconnected from '" + backendURI + "'");
	    if (autoReconnect) {
	        this.init();
	    }
	}

	this.onMessage = function (event) {
	    console.log("Data received from '" + protocolType + "'");
	}
}
