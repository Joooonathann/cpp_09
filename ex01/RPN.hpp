#pragma once

#include <stack>
#include <string>

class RPN
{
    private:
        std::stack<int> _stack;
    public:
        RPN(void);
        RPN(const RPN &copy);
        RPN &operator=(const RPN &copy);
        ~RPN(void);

        void calculate(const std::string &expression);
};