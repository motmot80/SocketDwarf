function SocketDwarf(prot) {
	
	this.backendURI = "ws://localhost:8080";
	this.binaryType = "arraybuffer";
	this.websocket;
	this.protocol = prot;
	
	this.doSend = function (message) {
		this.websocket.send(message);	
	}
	
	this.destroy = function () {	
		this.websocket.close();
	}
	
	this.dataReceived = function (event) { 
		console.log("Websocket received with protokoll '"  + protocol + "'");
	}
	
}

SocketDwarf.prototype.init = function () {
	var that = this;
	console.log(that.backendURI);
	this.websocket = new WebSocket(that.backendURI);		
	this.websocket.binaryType = that.binaryType;
	this.websocket.onopen = function(event) { that.onOpen(event) };
	this.websocket.onmessage = function(event) { that.dataReceived( event ) };
	this.websocket.onerror = function(event) { that.onError(event) };
	this.websocket.onclose = function(event) { that.onClose(event) };
}

SocketDwarf.prototype.onError = function () {
	var that = this;
	console.log("Error on Websocket with protocol '" + that.protocol + "'");
}

SocketDwarf.prototype.onClose = function (event) {
	var that = this;
	console.log("Websocket for protocol'"  + that.protocol + "' was closed");
}

SocketDwarf.prototype.onOpen = function (event) {
	var that = this;
	console.log("Websocket opened with protokoll '"  + that.protocol + "'");
}
