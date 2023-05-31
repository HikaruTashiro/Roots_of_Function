#include <cmath>
#include <cstdio>
#include <math.h>
#include <string>
#include <stack>
#include <iostream>
#include <cmath>
#include <utility>
#include "definitions.h"
#include "expression.h"

//
//using errorHandler = void (*)(std::string errorMesage);
//
//void error(std::string errorMesage)
//{
//    std::cout << errorMesage;
//}
//
//errorHandler addSpaces(std::string &expression)
//{
//    if(true)    
//        return error;
//    else
//        return nullptr;
//}
//

inline bool isAlphabetLexeme(std::string &lexeme)
{
   return lexeme == "x" || lexeme == "e" || lexeme == "pi" ||
          lexeme == "sin" || lexeme == "cos" || lexeme == "exp" ||
          lexeme == "ln" || lexeme == "lg" || lexeme == "abs" ||
          lexeme == "sqrt" || lexeme == "arctan";
}

ErrorCode addSpaces(std::string &expression)
{
    ErrorCode lexicalError = false;
    std::string spacedExpression, auxStr;
    std::string opLexemes = ";()+-*/^";
    auto iteratorString = expression.begin();

    while (iteratorString != expression.end() && !lexicalError)
    {
        if(*iteratorString == ' ')
            iteratorString++;
        else if(*iteratorString >= 'a' && *iteratorString <= 'z')
        {
            do
            {
                auxStr += *iteratorString;                
                iteratorString++;
            }while (*iteratorString >= 'a' && *iteratorString <= 'z');

            if(isAlphabetLexeme(auxStr))
                spacedExpression += auxStr + ' ';
            else 
                lexicalError = true;
            auxStr.clear();
        }
        else if(*iteratorString >= '1' && *iteratorString <= '9')
        {
            do
            {
                auxStr += *iteratorString;                
                iteratorString++;
            }while (*iteratorString >= '0' && *iteratorString <= '9');
            spacedExpression += auxStr + ' ';
            auxStr.clear();

            /* check for float expression */
            /* 3.8*x^3.35  */
        }
        else
        {
            auto auxIterator = opLexemes.begin();
            while (*auxIterator != *iteratorString && auxIterator != opLexemes.end())
                auxIterator++;
            if(auxIterator == opLexemes.end())
            {
                lexicalError = true;
                std::cout << "Error with operation\n";
                std::cout << *iteratorString << '\n';
            }
            else
            {
                spacedExpression += *iteratorString;
                if(*iteratorString == ';')
                    iteratorString = expression.end();
                else
                {
                    iteratorString++;
                    spacedExpression += ' ';
                }
            }
        }
    }

    expression.clear();
    expression = spacedExpression;

    return lexicalError;
}

Expression::Expression(std::string &expression)
{
    symbol_table.setStandardTokens();
    tokenizeExpression(expression);
}

Expression::Expression()
{
    symbol_table.setStandardTokens();
}

Expression::Expression(const Expression& expr)
{
    tokenized_expr = expr.tokenized_expr;
}

void Expression::tokenizeExpression(std::string &expression)
{
    auto iteratorExpr = expression.begin();
    bool leading = true;
    std::string auxStr;
    AtributeValue token_id;
    Token new_token;
    while(iteratorExpr != expression.end())
    {
        if(*iteratorExpr == ' ')
            iteratorExpr++;
        if(*iteratorExpr == '+' && leading)
            iteratorExpr += 2;
        else if(*iteratorExpr == '-' && leading)
            *iteratorExpr = '~';

        do
        {
            auxStr += *iteratorExpr;             
            iteratorExpr++;
        }while(*iteratorExpr != ' ' && iteratorExpr != expression.end());

        //std::cout << auxStr << '\n';
        //std::cout << auxStr[0] << '\n';
        //if(iteratorExpr == expression.end())
        //    std::cout << "Iterator got to the end\n";

        if(auxStr[0] >= 'a' && auxStr[0] <= 'z')
        {
            if(auxStr == "x")
                new_token.first = operand;
            else if (auxStr == "e" || auxStr == "pi")
                new_token.first = number;
            else
                new_token.first = unaryOp;
        }
        else if(auxStr[0] >= '0' && auxStr[0] <= '9')
            new_token.first = number;
        else 
        {
            if(auxStr == "(")
                new_token.first = leftParen;
            else if(auxStr == ")")
                new_token.first = rightParen;
            else if(auxStr == ";")
                new_token.first = endExpression;
            else if(auxStr == "~")
                new_token.first = unaryOp;
            else
                new_token.first = binaryOp;
        }

        if(new_token.first == leftParen || new_token.first == unaryOp ||
           new_token.first == binaryOp)
            leading = true;
        else
            leading = false;
        
        token_id = symbol_table.findAtribute(auxStr);
        if(token_id == NON_EXISTENT)
        {
            //std::cout << "float -> " << auxStr << '\n';
            float value = std::stof(auxStr);
            token_id = symbol_table.newToken(new Token_data(auxStr,value,0));
        }
        auxStr.clear();
        new_token.second = token_id;
        addToken(new_token);
    }
}

ErrorCode Expression::infixToPostfix()
{
    std::stack<Token> delay_ops;
    Token current, prior;
    auto iterInfix = tokenized_expr.begin();
    while(iterInfix->first != endExpression)
    {
        //std::cout << "somgoidfg\n";
        switch (iterInfix->first)
        {
            case operand:
            case number:
                addToken(*iterInfix);
                break;
            case leftParen:
                delay_ops.push(*iterInfix);
                break;
            case rightParen:
                prior = delay_ops.top();
                while (prior.first != leftParen)
                {
                    addToken(prior);
                    delay_ops.pop();
                    prior = delay_ops.top();
                }
                delay_ops.pop();
                break;
            case unaryOp:
            case binaryOp:
                bool end_right = false;
                do
                {
                    if(delay_ops.empty()) 
                        end_right = true;
                    else
                    {
                        prior = delay_ops.top();
                        if(prior.first == leftParen)
                            end_right = true;
                        else if(symbol_table.getTokenInfo(prior.second)->priority <
                                symbol_table.getTokenInfo(iterInfix->second)->priority)
                            end_right = true;
                        else if(symbol_table.getTokenInfo(iterInfix->second)->priority == 6)
                            end_right = true;
                        else
                            addToken(prior);
                        if(!end_right)
                            delay_ops.pop();
                    }
                }while (!end_right);
                delay_ops.push(*iterInfix);
                break;
        }

        iterInfix++;
        removeFirstToken();
        //for (auto iter : tokenized_expr)
        //    std::cout << iter.second << ' ';        
        //std::cout<<'\n';
    }

    while(!delay_ops.empty())
    {
        prior = delay_ops.top();
        addToken(prior);
        delay_ops.pop();
    }
    prior = tokenized_expr.front();
    removeFirstToken();
    addToken(prior);

    return SUCCESS;
}

ErrorCode Expression::evaluateAt(double x,double &f_of_x)
{
    std::string lexeme_x = "x";
    symbol_table.getTokenInfo(symbol_table.findAtribute(lexeme_x))->value = x;
    double first_elem, second_elem;
    std::stack<double> operands;
    auto iterExpr = tokenized_expr.begin();
    do
    {
        switch (iterExpr->first)
        {
            case unaryOp:
                if(operands.empty())
                    return FAILURE;
                first_elem = operands.top();
                operands.pop();
                operands.push(do_unary(first_elem, iterExpr->second));
                break;
            case binaryOp:
                if(operands.empty())
                    return FAILURE;
                second_elem = operands.top();
                operands.pop();
                if(operands.empty())
                    return FAILURE;
                first_elem = operands.top();
                operands.pop();
                operands.push(do_binary(first_elem,second_elem,iterExpr->second)); 
                break;
            case operand:
            case number:
                operands.push(symbol_table.getTokenInfo(iterExpr->second)->value);
                break;
            case endExpression:
                break;
        } 
        //std::cout << operands.top() << '\n';
        iterExpr++;
    }while (iterExpr->first != endExpression);

    if(operands.empty())
        return FAILURE;
    f_of_x = operands.top();
    operands.pop();
    if(!operands.empty())
        return FAILURE;
    return SUCCESS;
}

void Expression::addToken(Token &new_token)
{
    tokenized_expr.push_back(new_token);
}

void Expression::removeFirstToken()
{
    tokenized_expr.pop_front();
}

ErrorCode Expression::isValid()
{
    auto iterToken = tokenized_expr.begin();
    int parent_count = 0;
    bool leading = true;
    Token_name type;
    while (iterToken->first != endExpression)
    {
        type = iterToken->first;            
        //std::cout << "Type -> " << type << '\n';
        if(type == rightParen || type == binaryOp)
        {
            if(leading)
                return FAILURE;
        }
        else if(!leading)
            return FAILURE;

        if(type == leftParen)
            parent_count++;
        else if(type == rightParen)
        {
            parent_count--;
            if(parent_count < 0)
                return FAILURE;
        }
        if(type == binaryOp || type == unaryOp || type == leftParen)
            leading = true;
        else
            leading = false;

        iterToken++;
    }

    if(leading)
    {
        //std::cout << "\nLeading FAILURE\n";
        return FAILURE;
    }
    if(parent_count > 0)
    {
        //std::cout << "\nparent_count FAILURE\n";
        return FAILURE;
    }

    return SUCCESS;
}

void Expression::getIteratorRange(std::list<Token>::iterator &start, std::list<Token>::iterator &end) 
{
    start = tokenized_expr.begin();
    end = tokenized_expr.end();
}

float Expression::do_unary(double x, Token_name type)
{
    switch (type)
    {
        case 3:
            return -x;
        case 4:
            return std::abs(x);
        case 5:
            return std::sqrt(x);
        case 6:
            return std::exp(x);
        case 7:
            return std::log(x);
        case 8:
            return std::log2(x);
        case 9:
            return std::sin(x);
        case 10:
            return std::cos(x);
        case 11:
            return std::atan(x);
        default:
            break;
    }    
    return 0.0f;
}

float Expression::do_binary(double x, double y, Token_name type)
{
    switch (type)
    {
        case 12:
            return x + y;
        case 13:
            return x - y;
        case 14:
            return x * y;
        case 15:
            return x / y;
        case 16:
            return std::pow(x, y);
    }

    return 0.0f;
}
