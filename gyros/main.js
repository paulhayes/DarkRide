'use strict';

var bluetoothSerialPort = require('bluetooth-serial-port');
var simpleOvr = require('SimpleLibOVR');

var bsp = new bluetoothSerialPort.BluetoothSerialPort();

var Comms = function(bsp){
	var _blueSerial = bsp;
	var _address;
	var _channel=1;
	var _connected=false;
	var _name = 'GyroServo';
	var _isReady = true;

	this.init = function(){
		_blueSerial = bsp;
		_blueSerial.on('found',this.onFound);
		_blueSerial.on('data',this.onData);
		_blueSerial.on('failure',this.onError);
		_blueSerial.on('finished',this.onInquireComplete)
		_blueSerial.inquire();
	}.bind(this);

	this.onFound = function(address,name){
		console.log('DEVICE FOUND ',name);
		if(name==_name){
			console.log('CONNECTING ',name);
			console.log( this.onChannelFound );
			_address=address;
		}
	}.bind(this);

	this.onChannelFound = function(channel){
		console.log("CHANNEL FOUND");
		_channel = channel;
		_blueSerial.connect(_address,_channel,this.onConnect);
	}.bind(this)

	this.onConnect = function(){
		console.log("ON CONNECTED");
		//_blueSerial.write(new Buffer('my data', 'utf-8'),this.onWrite);
		_connected = true;
	}.bind(this);

	this.onData = function(buffer){
		//console.log('ON DATA',buffer);
		_isReady = true;
	}.bind(this);

	this.onError = function(e){
		console.log('ERROR',e);
	}.bind(this);

	this.onWrite = function(err,bytes){		
		if(err){
			this.onError(err);
		}
	}

	this.onInquireComplete = function(){
		console.log('INQUIRE COMPLETE');
		//if(_address)_blueSerial.findSerialPortChannel(_address,this.onChannelFound,this.onError);
		if(_address){
			_blueSerial.connect(_address,_channel,this.onConnect);
		}
		else throw new Error("Device not found");
	}.bind(this);

	this.move = function(pos){
		if( _connected && (typeof pos == 'number') && _isReady ){
			_blueSerial.write(new Buffer(String.fromCharCode(pos),'utf-8'),this.onWrite);
			_isReady = false;
		}
	}
	
}

var comms = new Comms(bsp);

comms.init();

Math.radToDeg = 180 / Math.PI ;

simpleOvr.orientationY = function(){
	var q = this.orientation();
	return Math.atan2(2*q.y*q.w-2*q.w*q.z, 1-2*q.y*q.y - 2*q.z*q.z);
}.bind(simpleOvr);

simpleOvr.setup();

Math.clamp = function(min,max,value){
	if( value <= min ) value = min;
	if( value >= max ) value = max;
	return value;
}

var lastY;
var i=0;

setInterval(function(){ 
	var y = Math.radToDeg * simpleOvr.orientationY() + 90;
	y = Math.round( y );
	if( y < -90 ) y+=360;
	if( y > 270 ) y-=360;
	y = Math.clamp( 0, 179, y );
	if( true || y != lastY ){
		comms.move( y );
		lastY = y;
		i++;
	}
},20);
