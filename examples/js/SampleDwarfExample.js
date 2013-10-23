var sampleControler;

function init(){

	sampleControler = new SampleDwarf();
	sampleControler.init();
	
	/* Clean up when the site gets unloaded */
	window.onbeforeunload = function (e) {
		sampleControler.destroy();
	};
}

window.onload = function() {
  init();
  animate();
}