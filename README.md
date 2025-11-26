# Sony Spresense UART Command Parser

This parser is inteded to be an extensible framework for intepretting and executing commands over UART using a command line like syntax.

Handler functions may be mapped to command names by adding them to the map in the `uart_command_router.c` as seen below:

```cpp
static const std::map<std::string, command_handler_t> router = {
    {"TAKE_PHOTO", cmd_take_photo},
    {"BLINK_LED", cmd_blink_led},
    {"ECHO", cmd_echo}
};
```

Handler functions acept arguements in the same way that C main functions do, for example:

```cpp
command_result_t cmd_blink_led(
  std::string *args, 
  uint8_t arg_count,
  std::string *response) 
{}
```

Each command may communicate across the serial ports as needed and then return a code from the `command_result_t` enum in `uart_commands.h` as well as a string response by writing to the `response` pointer.