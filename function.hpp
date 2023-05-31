#include "exp/expression.h"
#include <utility>
#include <vector>

class function
{
    Expression expr;

    public: 
        function() = default;
        void associate_expr(Expression &expr);
        void estimate(double a, double b, double max_error,
                std::vector<std::pair<double, double>>& result_iteration);
};
