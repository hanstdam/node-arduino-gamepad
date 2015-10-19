String message = "";
boolean messageComplete = false;
String command = "";
String value = "";

void setup() {
  Serial.begin(115200);
  message.reserve(128);
  command.reserve(4);
  value.reserve(5);
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
    }
    else if(command == "BNY") {
      Serial.print("I understand BNY: ");
      Serial.println(value.toInt());
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

