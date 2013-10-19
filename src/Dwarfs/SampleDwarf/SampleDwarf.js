function SampleDwarf() {}

SampleDwarf.prototype = new SocketDwarf("sample");

SampleDwarf.prototype.dataReceived = function (event) {
	var that = this;
	message = JSON.parse(event.data);
	console.log(" X:" + message.X + " Y:" + message.Y + " Z:" + message.Y);
}