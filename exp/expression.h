#pragma once
#include <queue>
#include <list>
#include <map>
#include <string>
#include <utility>
#include "definitions.h"
#include "lexicon.h"

using ErrorCode = bool;

class Expression
{
    std::list<Token> tokenized_expr;
    Lexicon symbol_table;
    public:
        Expression();
        Expression(const Expression& expr);
        Expression(std::string &expression); //Tokenize the expression
        ErrorCode infixToPostfix(); //certify that expression is valid first
        ErrorCode evaluateAt(double x, double &f_of_x);
        void tokenizeExpression(std::string &expression);
        void getIteratorRange(std::list<Token>::iterator &start,
                              std::list<Token>::iterator &end); //comment later
        ErrorCode isValid();     //check for infix
    private:
        void removeFirstToken(); //move back to private
        void addToken(Token &new_token);
        float do_unary(double x, Token_name type);
        float do_binary(double x, double y, Token_name type);
};

/*Autenticate lexical correctness of expression before sending to the class*/
/*Also add whitespace to better identify lexemes*/
ErrorCode addSpaces(std::string &expression);
