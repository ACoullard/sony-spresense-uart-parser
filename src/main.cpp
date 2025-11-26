#include <Arduino.h>
#include "command_parser.h"

CommandParser parser;

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);
  delay(1000);
  Serial.println("Command Parser Initialized.");
}
void loop() {
  parser.update();
}

