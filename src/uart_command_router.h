#pragma once

#include "uart_commands.h"
#include <string>

command_handler_t get_command_handler(const std::string& cmd_name);
