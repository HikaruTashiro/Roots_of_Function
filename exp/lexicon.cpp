#include "lexicon.h"
#include "definitions.h"
#include <cmath>
#include <math.h>

void Lexicon::setStandardTokens()
{
   lexeme_map =
   {
      {";",0},
      {"(",1},
      {")",2},
      {"~",3},
      {"abs",4},
      {"sqrt",5},
      {"exp",6},
      {"ln",7},
      {"lg",8},
      {"sin",9},
      {"cos",10},
      {"arctan",11},
      {"+",12},
      {"-",13},
      {"*",14},
      {"/",15},
      {"^",16},
      {"x",17},
      {"pi",18},
      {"e",19}
   };

   symbol_table = 
   {
      new Token_data(";",0.0,6),
      new Token_data("(",0.0,6),
      new Token_data(")",0.0,6),
      new Token_data("~",0.0,6),
      new Token_data("abs",0.0,6),
      new Token_data("sqrt",0.0,6),
      new Token_data("exp",0.0,6),
      new Token_data("ln",0.0,6),
      new Token_data("lg",0.0,6),
      new Token_data("sin",0.0,6),
      new Token_data("cos",0.0,6),
      new Token_data("arctan",0.0,6),
      new Token_data("+",0.0,4),
      new Token_data("-",0.0,4),
      new Token_data("*",0.0,5),
      new Token_data("/",0.0,5),
      new Token_data("^",0.0,6),
      new Token_data("x",0.0,0),
      new Token_data("pi",M_PI,0),
      new Token_data("e",M_E,0),
   };
}

AtributeValue Lexicon::findAtribute(std::string &lexeme)
{
   if (lexeme_map.find(lexeme) != lexeme_map.end())
      return lexeme_map.find(lexeme)->second;
   else
      return NON_EXISTENT;
}

AtributeValue Lexicon::newToken(Token_data *new_token)
{
   symbol_table.push_back(new_token);
   return symbol_table.size() - 1;
}

Token_data* Lexicon::getTokenInfo(AtributeValue token_id)
{
   if(token_id < symbol_table.size())
      return symbol_table[token_id];
   else
      return nullptr;
}
