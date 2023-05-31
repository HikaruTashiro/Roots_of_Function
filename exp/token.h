#pragma once
#include "definitions.h"
#include <string>

struct Token_data
{
    std::string name;
    double value;
    Priority priority;
    Token_data(std::string token_name, double value, Priority priority);
};
