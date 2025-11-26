#include "cmd_take_photo.h"

command_result_t cmd_take_photo(
  std::string *args, 
  uint8_t arg_count,
  std::string *response) {

    // TODO: make cmd to take photo
    *response = "photo taken";
    return CMD_OK;
}