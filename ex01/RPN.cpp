#include "RPN.hpp"

#include <iostream>
#include <sstream>
#include <cstdlib>
#include <stdexcept>

RPN::RPN(void) { }

RPN::RPN(const RPN &copy) { *this = copy; }

RPN &RPN::operator=(const RPN &copy)
{
    if (this != &copy)
        this->_stack = copy._stack;
    return (*this);
}

RPN::~RPN(void) { }

void RPN::calculate(const std::string &expression)
{
    std::istringstream iss(expression);
    std::string token;

    while (iss >> token)
    {
        if (token.length() == 1 && std::isdigit(token[0]))
            this->_stack.push(token[0] - '0');
        else if (token.length() == 1 && std::string("+-*/").find(token[0]) != std::string::npos)
        {
            if (this->_stack.size() < 2)
                throw (std::runtime_error("Error: not enough operands."));
            int b = this->_stack.top(); this->_stack.pop();
            int a = this->_stack.top(); this->_stack.pop();
            int result = 0;
            switch (token[0])
            {
                case '+': result = a + b; break;
                case '-': result = a - b; break;
                case '*': result = a * b; break;
                case '/':
                    if (b == 0)
                        throw (std::runtime_error("Error: division by zero."));
                    result = a / b;
                    break;
            }
            this->_stack.push(result);
        }
        else
            throw (std::runtime_error("Error: invalid token: " + token));
    }
    if (this->_stack.size() != 1)
        throw (std::runtime_error("Error: invalid expression."));
    std::cout << this->_stack.top() << std::endl;
}