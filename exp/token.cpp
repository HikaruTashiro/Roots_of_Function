#include "token.h"
#include <string>

Token_data::Token_data(std::string token_name, double value, Priority priority)
{
   this->name = token_name;   
   this->value = value;
   this->priority = priority;
}
