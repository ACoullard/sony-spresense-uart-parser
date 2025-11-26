#pragma once
#include <Arduino.h>
#include "uart_commands.h"

#define MAX_CMD_LENGTH 128
#define MAX_ARGS 10

class CommandParser {
private:
  char buffer1[MAX_CMD_LENGTH];
  char buffer2[MAX_CMD_LENGTH];
  size_t bufferIndex1 = 0;
  size_t bufferIndex2 = 0;
  void parse_and_execute(char* command_string);

public:
  CommandParser();
  void update();
  void printfBothSerial(const char* format, ...);
  void processSerialChar(char c, char* buffer, size_t& bufferIndex);
};