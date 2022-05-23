var checkboxFunction;
var setRotationSpeed;
var statusElement = document.getElementById('status');
var progressElement = document.getElementById('progress');
var spinnerElement = document.getElementById('spinner');

var resizeWindow;
var handleKey;

var setAngleX;
var setAngleY;
var setAngleZ;

var Module = {
	onRuntimeInitialized: function() {
                resizeWindow = Module.cwrap('resizeWindow',null,['number','number']);
                handleKey = Module.cwrap('handleKey', null, ['string']);
	},
	preRun: [function() {ENV.TOKEN_HASH = tokenData['hash']}, function() {ENV.WIDTH = window.innerWidth.toString()}, function() {ENV.HEIGHT = window.innerHeight.toString()}],
	postRun: [],
	print: (function() {})(),
	printErr: function(text) {
		if (arguments.length > 1) text = Array.prototype.slice.call(arguments).join(' ');
		console.error(text);
	},
	canvas: (function() {
		var canvas = document.getElementById('canvas');
		canvas.addEventListener("webglcontextlost", function(e) { alert('WebGL context lost. You will need to reload the page.'); e.preventDefault(); }, false);
                window.addEventListener("resize", function (e) {
                        canvas.height = window.innerHeight;
                        canvas.style.height = window.innerHeight;
                        canvas.width = document.body.clientWidth;
                        resizeWindow(canvas.width, canvas.height);
                });

                window.addEventListener("keydown", function (e) {
                        if (e.code === "F5" || e.code === "F11" || e.code === "F12" || e.code === "KeyR" || (e.ctrlKey && e.code === "KeyI")) {
                                e.preventDefault = function() {};
                        }
                        e.isTrusted && handleKey(e.key);
                        // e.preventDefault();
                }, {capture: true});

		return canvas;
	})(),
	setStatus: function(text) {},
	totalDependencies: 0,
	monitorRunDependencies: function(left) {
		this.totalDependencies = Math.max(this.totalDependencies, left);
		Module.setStatus(left ? 'Preparing... (' + (this.totalDependencies-left) + '/' + this.totalDependencies + ')' : 'All downloads complete.');
	}
};
window.onerror = function() {};
