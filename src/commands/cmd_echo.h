#pragma once

#include <Arduino.h>
#include "uart_commands.h"

command_result_t cmd_echo(
  std::string *args, 
  uint8_t arg_count,
  std::string *response) 
{
 for (uint8_t i = 0; i < arg_count; i++) {
    *response += args[i];
    if (i < arg_count - 1) {
      *response += " ";
    }
  }
  return CMD_OK;
}