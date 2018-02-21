#pragma once

#include <string>
#include <vector>

class Controller {
    std::string name;
    std::vector<bool> buttons;
    std::vector<int16_t> axis;
};
