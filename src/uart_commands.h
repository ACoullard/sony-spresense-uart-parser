#pragma once

#include <stdint.h> 
#include <functional>
#include <string>

typedef enum {
    CMD_OK = 0,
    CMD_ERROR = 1,
    CMD_INVALID_ARGS = 2,
    CMD_TIMEOUT = 3,
    CMD_NOT_READY = 4
} command_result_t;

using command_handler_t = std::function<
command_result_t(
    std::string*,
    uint8_t,
    std::string*)>;