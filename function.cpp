//#define DEBUG
#include "function.hpp"
#include <ios>
#ifdef DEBUG
#include <iostream>
#endif
#include <iomanip>
#include <utility>

void function::associate_expr(Expression &expr)
{ this->expr = expr;  
}

void function::estimate(double a, double b, double max_error,
                        std::vector<std::pair<double, double>> &result_iteration)
{
    //std::cout << std::fixed;
    //std::cout << std::setprecision(2);

    double f_of_a, f_of_b, error;
    expr.evaluateAt(a, f_of_a);
    expr.evaluateAt(b, f_of_b);
    if((f_of_a < 0.0l && f_of_b > 0.0l) || (f_of_b < 0.0l && f_of_a > 0.0l))
    {
        double middle = (a + b) / 2.0l;
        double f_middle;
        result_iteration.clear();
        expr.evaluateAt(middle, f_middle);
        error = b - middle;
        result_iteration.push_back(std::pair<double, double>(error, middle));
        while (error > max_error)
        {
#ifdef DEBUG
            std::cout << "a: " << a << '\n';
            std::cout << "b: " << b << '\n';
            std::cout << "middle: " << middle << "\n\n";
#endif

            if((f_of_a > 0.0l) && (f_middle < 0.0l) ||
                (f_middle > 0.0l) && (f_of_a < 0.0l)) 
            {
                b = middle;
                middle = (a + b) / 2.0l;
                f_of_b = f_middle;
            }
            else
            {
                a = middle;
                middle = (a + b) / 2.0l;
                f_of_a = f_middle;
            }
            error = b - middle;

#ifdef DEBUG
            std::cout << "f_of_a: " << f_of_a << '\n';
            std::cout << "f_middle: " << f_middle<< '\n';
            std::cout << "f_of_b: " << f_of_b << "\n\n";
#endif

            expr.evaluateAt(middle, f_middle);
            result_iteration.push_back(std::pair<double, double>(error, middle));
        }
    }
}
