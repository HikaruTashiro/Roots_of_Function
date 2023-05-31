#include <ios>
#include <ostream>
#include <string>
#include <iostream>
#include <utility>
#include <vector>
#include <limits>
#include <iomanip>
#include "exp/expression.h"
#include "exp/token.h"
#include "function.hpp"


int main() {
    bool quit = false;
    bool cannot_be_conv = false;
    std::string user_expr;
    std::string user_info1, user_info2, user_info3;
    double start, end, error;
    std::vector<std::pair<double, double>> iteration;

    Expression *expression;
    do
    {
        std::cout << "\nEscreva uma expressao:\n";
        std::cout << "f(x) = ";
        std::getline(std::cin, user_expr);
        user_expr += ';';
        std::endl(std::cout);

        if(!addSpaces(user_expr))
        {
            expression = new Expression(user_expr);
            if(expression->isValid())
            {
                expression->infixToPostfix();
                do{
                    std::cout << "\nEscreva o ponto extremo da esquerda:\n";
                    std::cin >> user_info1; 
                    std::cout << "Escreva o ponto extremo da direita:\n";
                    std::cin >> user_info2; 
                    std::cout << "Valor maximo do erro:\n";
                    std::cin >> user_info3; 
                    try{
                        start = std::stold(user_info1.data()); 
                        end = std::stold(user_info2.data()); 
                        error = std::stold(user_info3.data()); 
                        cannot_be_conv = false;
                        user_info1.clear(), user_info2.clear(); user_info3.clear();
                    }catch (std::invalid_argument){
                        std::cout << "Uma variavel foi escrita de forma indevida";
                        cannot_be_conv = true;                    
                    }
                }while (cannot_be_conv);

                std::cout << std::fixed;
                std::cout << std::setprecision(5);
                function fn;
                fn.associate_expr(*expression);
                fn.estimate(start, end, error, iteration);

                std::cout << "\n\nSaida:\n\n";
                std::cout << "\nTabela de Valores:\n";
                std::cout << "erro"; 
                for (int i = -1; i <= 2; i++)
                    std::cout << ' ';
                std::cout << "| " << "x_i" << '\n';
                for (auto f : iteration)
                    std::cout << f.first << " | " << f.second << '\n';
                delete expression;
            }
            else
                std::cout << "\nA expressao fornecida contem erro\n";
        }
        else
            std::cout << "\nErro Lexico detectado na expressao fornecida\n";

        std::cout << "Deseja sair do programa?\n";
        std::cout << "Sim (Escreva q)\t Nao (Escreva qualquer coisa)\n";
        std::cin >> user_info1;
        if(user_info1 == "q")
            quit = true;

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
        user_expr.clear();
        user_info1.clear(); 
        std::cout << "\033[2J\033[1;1H";

    }while (!quit); 
    return 0;
}
