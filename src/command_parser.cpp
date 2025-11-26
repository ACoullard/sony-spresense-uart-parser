#include <Arduino.h> 
#include "command_parser.h"
#include "uart_command_router.h"
#include <string>
#include <vector>

CommandParser::CommandParser() 
  : bufferIndex1(0)
  , bufferIndex2(0)
{}

void CommandParser::update() {
  while (Serial.available()) {
    char c = Serial.read();
    processSerialChar(c, buffer1, bufferIndex1);
  }
  while (Serial2.available()) {
    char c = Serial2.read();
    processSerialChar(c, buffer2, bufferIndex2);
  }
}

// Helper function to process a single character from a serial port
void CommandParser::processSerialChar(char c, char* buffer, size_t& bufferIndex) {
  if (c == '\n' || c == '\r') {
    if (bufferIndex > 0) {
      buffer[bufferIndex] = '\0';
      parse_and_execute(buffer);
      bufferIndex = 0;
    }
  } else if (c == '\b') {
    if (bufferIndex > 0) {
      bufferIndex--;
    }
  } else if (bufferIndex < MAX_CMD_LENGTH - 1) {
    buffer[bufferIndex++] = c;
  }
}

void CommandParser::parse_and_execute(char* command_string) {
  std::vector<std::string> args;
  char* token = strtok(command_string, " ");
  while (token != NULL) {
      args.push_back(std::string(token));
      token = strtok(NULL, " ");
  }

  if (args.empty()) {
    return;
  }

  command_handler_t handler = get_command_handler(args[0]);

  if (handler != nullptr) {
    std::string response_str;
    command_result_t result = handler(args.data(), args.size(), &response_str);
    
    if (result == CMD_OK) {
      printfBothSerial("SUCCESS: %s\n", response_str.c_str());
    } else {
      printfBothSerial("ERROR %d: %s\n", result, response_str.c_str());
    }
  } else {
    printfBothSerial("ERROR: Unknown command '%s'\n", args[0].c_str());
  }
}

void CommandParser::printfBothSerial(const char* format, ...) {
  char buffer[256];
  va_list args;
  va_start(args, format);
  vsnprintf(buffer, sizeof(buffer), format, args);
  va_end(args);
  
  Serial.print(buffer);
  Serial2.print(buffer);
}