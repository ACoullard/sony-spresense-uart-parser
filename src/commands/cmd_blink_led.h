#pragma once

#include <Arduino.h>
#include "uart_commands.h"

command_result_t cmd_blink_led(
  std::string *args, 
  uint8_t arg_count,
  std::string *response) 
{
  if (arg_count < 2) {
    *response = "Error: Missing argument for LED blink count.";
    return CMD_INVALID_ARGS;
  }

  int blink_count = std::stoi(args[1]);
  for (int i = 0; i < blink_count; i++) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
  }

  *response = "LED blinked " + std::to_string(blink_count) + " times.";
  return CMD_OK;
}