#include "uart_command_router.h"

#include "commands/cmd_take_photo.h"
#include "commands/cmd_blink_led.h"
#include "commands/cmd_echo.h"

#include <map>
#include <string>

static const std::map<std::string, command_handler_t> router = {
    {"TAKE_PHOTO", cmd_take_photo},
    {"BLINK_LED", cmd_blink_led},
    {"ECHO", cmd_echo}
};

command_handler_t get_command_handler(const std::string& cmd_name) {
    auto it = router.find(cmd_name);
    if (it != router.end()) {
        return it->second;
    }
    return nullptr;
}
