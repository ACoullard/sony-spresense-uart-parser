#ifndef TAKE_PHOTO_H
#define TAKE_PHOTO_H

#include "uart_commands.h"

command_result_t cmd_take_photo(
  std::string *args, 
  uint8_t arg_count,
  std::string *response);

#endif
