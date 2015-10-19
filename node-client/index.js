var GamePad = require('node-gamepad')
var Queue = require('queue')
var serialPortLib = require('serialport')
var SerialPort = serialPortLib.SerialPort
var serialPort = new SerialPort('/dev/ttyACM0', {
  baudrate: 115200,
  parser: serialPortLib.parsers.readline('\n')
})

var controller = new GamePad('logitech/rumblepad2', {
  vendorID: 1133, // = 0x046d
  productID: 49688 // = 0xc218
})

var queue = Queue({
  concurrency: 1
})

var writeToSerial

function dummySerialWriter (message, callback) {
  console.warn('Serialport not connected yet. Message "' + message + '" not send.')
  callback()
}

writeToSerial = dummySerialWriter

function writeAndDrain (data, callback) {
  serialPort.write(data + '\n', function () {
    serialPort.drain(callback)
  })
}

serialPort.on('open', function () {
  writeToSerial = writeAndDrain

  serialPort.on('data', function (data) {
    console.log('Data received:', data)
  })
})

controller.connect()
queue.start()
console.log('connected')

function addMessage (message) {
  queue.push(function (callback) {
    writeToSerial(message, callback)
  })
  queue.start()
}

controller.on('dpadUp:press', function () {
  addMessage('BNX:1')
})
controller.on('dpadUp:release', function () {
  addMessage('BNX:0')
})

controller.on('dpadDown:press', function () {
  addMessage('BNY:1')
})

controller.on('dpadDown:release', function () {
  addMessage('BNY:0')
})
