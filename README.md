# GamePad communition to Arduino via Node.js

This repository contains the arduino sketch and node.js code necessary for a controller like Logitech RumblePad 2 (and other controllers supported by [node-gamepad](https://github.com/carldanley/node-gamepad)) to communicate with the Arduino.

The communication happens over serial connection.

The following will go through the installation process of each necessary parts and how to use and manipulate the existing sample.

## Install

The installation process is only described from a Ubuntu point of view, but most is applicable to other operating systems.

### Node.js

The code is tested with Node.js v0.10 stable. It can be installed via directly downloading it from here: https://nodejs.org/en/blog/release/v0.10.36/ or using [nvm](https://github.com/creationix/nvm), like so:

    $ curl -o- https://raw.githubusercontent.com/creationix/nvm/v0.29.0/install.sh | bash
    $ nvm install 0.10
    $ nvm use 0.10

Beware that you might need to run `node` as `sudo`, in order for the joystick to be recognized. This can be little challenging, but the problem has been fixed: https://github.com/creationix/nvm#problems

### Arduino

The arduino drivers and IDE can be downloaded from the [arduino.org website](http://www.arduino.org/software#ide).
Open the arduino sketch in the IDE, select the correct Board (fx. Uno) and serial port (fx. `COM3` or `/dev/ttyACM0`) and upload the sketch.

## Usage

- Upload the arduino sketch to the arduino board.
- Navigate to the `node-client` folder in your terminal.
- Edit [line 5 in `index.js`](https://github.com/hanstdam/node-arduino-gamepad/blob/master/node-client/index.js#L5) to have the correct serial port name.
- Run `npm install` to install the dependencies
- Run `node .` to start the program. If it outputs `connected`, everything is ready. Maybe you need ask linux kindly by running `sudo node .`.

## Extending the sample

### The protocol

The node program sends commands to the arduino with a simple protocol. The messages look something like this:

    BNY:1

Where the first 3 letters are the "command" followed by a colon and a value.
This makes it easy for the arduino to understand the command and the value.
The command could be longer, but then the arduino sketch would have to be changed as well. Further, the less you send to the arduino, the faster the communication will happen.

### Adding events

#### Node.js program

Adding events, such as button presses and joystick movements, can easily be done by adding additional blocks to `index.js` of the folloing form:

```JavaScript
controller.on('dpadUp:press', function () {
  addMessage('BNU:1')
})
```

The events are composed of a button/joystick/status name and a state (fx. `press`). In the above example the `dpadUp` is the `name` and the `press` is the state.

All names of the Logitech RumblePad 2 button, joystick and status names are listed in [the configuration json file of the node-gamepad project](https://github.com/carldanley/node-gamepad/blob/master/controllers/logitech/rumblepad2.json).
The [`node-gamepad` documentation](https://github.com/carldanley/node-gamepad#supported-events) describes how the events are composed, based on whether it's a button press/release, status change or a joystick move.

The `message` is the raw message send to the arduino. The command must be 3 characters and it will make it easier on yourself if it makes sense, like this: `BN = Button`, `U = Up` => `BNU = Button Up`. The value is `1` which describes a pressed state. `0` could describe a release state.

#### Arduino sketch

The arduino sketch needs to recognize the events we are sending to it. Further we need to define what the arduino should do when an event is received.

In the [arduino sketch](https://github.com/hanstdam/node-arduino-gamepad/blob/master/arduino-sketch/sketch/sketch.ino#L25-L28) each command needs to have a special case. A case for our example could look like this:

```Arduino
const int redLEDPin = 8;
void setup() {
    ...
    pinMode(redLEDPin, OUTPUT);
}

void loop() {
...
  else if(command == "BNU") {
    if (value.toInt() === 1) {
      digitalWrite(redLEDPin, HIGH);
    } else {
      digitalWrite(redLEDPin, LOW);
    }
  }
...
}
```

The `command` is always the first 3 letters in upper case. The `value` is always a string with everything after the colon (`:`) in the message.

### Reduce the bandwidth

It has not been tested with all commands from the controller, but the serial connection between the arduino and node is slow. This means that you could run into troubles like delayed or dropped messages.
In general keep the commands and values small, and reduce the number of events to a minimum to avoid these problems.

## Troubleshooting

 - Do not have the arduino IDE serial monitor open while the node program is running.
 - Close the node program (Ctrl+C) before uploading a new sketch to the arduino.
 - Make sure the correct serial port is used in your node program. Change [line 5 in `index.js`](https://github.com/hanstdam/node-arduino-gamepad/blob/master/node-client/index.js#L5) to have the correct serial port.

## License

MIT