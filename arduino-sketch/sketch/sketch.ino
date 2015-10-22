#include <Servo.h>
String message = "";
boolean messageComplete = false;
String command = "";
String value = "";
int number = -1;
Servo myServo;


void setup() {
  Serial.begin(115200);
  message.reserve(128);
  command.reserve(4);
  value.reserve(5);
  pinMode(7, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  myServo.attach(9);
}

void loop() {
  serialEvent();
  if (messageComplete) {
    command = message.substring(0, 3);
    command.toUpperCase();
    
    value = message.substring(4);

    if(command == "BNX") {
      Serial.print("I understand BNX: ");
      Serial.println(value.toInt());
      number = value.toInt();
      if(number == 1) {
        digitalWrite(5, HIGH);
      } else {
        digitalWrite(5, LOW);
      }
    }
    else if(command == "BNY") {
      Serial.print("I understand BNY: ");
      Serial.println(value.toInt());
      number = value.toInt();
      if(number == 1) {
        digitalWrite(7, HIGH);
      } else {
        digitalWrite(7, LOW);
      }
    }
    else if(command == "LFX") {
            Serial.print("I understand LFX: ");
      Serial.println(value.toInt());
      number = value.toInt();
      if(number > 128){
        digitalWrite(10,HIGH);
      }
      
      if(number < 128)
      {
        digitalWrite(11,HIGH);
      }
      
      if(number == 128) {
        digitalWrite(10,LOW);
        digitalWrite(11,LOW);
      }
      int angle = map(number, 0, 255, 0, 179);
      myServo.write(angle);
    }
    else {
      Serial.print("I don't understand: ");
      Serial.println(command);
    }
    reset();
  }
}

void reset() {
  message = "";
  value = "";
  command = "";
  messageComplete = false;
  number = -1;
}

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar == '\n') {
      messageComplete = true;
    } 
    else {
      message += inChar;
    }
  }
}

