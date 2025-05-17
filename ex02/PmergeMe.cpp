#include "PmergeMe.hpp"

#include <iostream>
#include <sstream>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <stdexcept>
#include <iomanip>
#include <climits>

PmergeMe::PmergeMe(void) { }

PmergeMe::PmergeMe(const PmergeMe &copy) { *this = copy; }

PmergeMe &PmergeMe::operator=(const PmergeMe &copy)
{
    if (this != &copy)
    {
        this->_vector = copy._vector;
        this->_deque = copy._deque;
    }
    return (*this);
}

PmergeMe::~PmergeMe(void) { }

void PmergeMe::parseInput(char **argv)
{
    for (int i = 1; argv[i]; ++i)
    {
        std::string arg(argv[i]);
        for (size_t j = 0; j < arg.length(); ++j)
        {
            if (!isdigit(arg[j]))
                throw (std::runtime_error("Error"));
        }
        long val = std::atol(arg.c_str());
        if (val < 0 || val > INT_MAX)
            throw (std::runtime_error("Error"));
        this->_vector.push_back(static_cast<int>(val));
        this->_deque.push_back(static_cast<int>(val));
    }
    if (this->_vector.empty())
        throw (std::runtime_error("Error"));
}

void PmergeMe::printSequence(const std::string &label, const std::vector<int> &sequence) const
{
    std::cout << label;
    for (size_t i = 0; i < sequence.size(); ++i)
        std::cout << " " << sequence[i];
    std::cout << std::endl;
}

void PmergeMe::fordJohnsonSort(std::vector<int>& sequence) {
    if (sequence.size() <= 1)
        return;
    std::vector<int> left, right;
    for (size_t i = 0; i < sequence.size(); ++i) {
        if (i % 2 == 0)
            left.push_back(sequence[i]);
        else
            right.push_back(sequence[i]);
    }
    fordJohnsonSort(left);
    fordJohnsonSort(right);
    std::vector<int>::iterator it = left.begin();
    for (size_t i = 0; i < right.size(); ++i) {
        int val = right[i];
        it = std::upper_bound(left.begin(), left.end(), val);
        left.insert(it, val);
    }
    sequence = left;
}

void PmergeMe::fordJohnsonSort(std::deque<int>& sequence) {
    if (sequence.size() <= 1)
        return;
    std::deque<int> left, right;
    for (size_t i = 0; i < sequence.size(); ++i) {
        if (i % 2 == 0)
            left.push_back(sequence[i]);
        else
            right.push_back(sequence[i]);
    }
    fordJohnsonSort(left);
    fordJohnsonSort(right);
    std::deque<int>::iterator it = left.begin();
    for (size_t i = 0; i < right.size(); ++i) {
        int val = right[i];
        it = std::upper_bound(left.begin(), left.end(), val);
        left.insert(it, val);
    }
    sequence = left;
}

void PmergeMe::sortAndMeasure(void)
{
    std::vector<int> vCopy = this->_vector;
    std::deque<int> dCopy = this->_deque;

    printSequence("Before:", this->_vector);

    clock_t start = clock();
    fordJohnsonSort(vCopy);
    clock_t end = clock();
    double vecTime = static_cast<double>(end - start) / CLOCKS_PER_SEC * 1e6;

    start = clock();
    fordJohnsonSort(dCopy);
    end = clock();
    double deqTime = static_cast<double>(end - start) / CLOCKS_PER_SEC * 1e6;

    printSequence("After:", vCopy);
    std::cout << std::fixed << std::setprecision(5);
    std::cout << "Time to process a range of " << _vector.size()
              << " elements with std::vector : " << vecTime << " us" << std::endl;
    std::cout << "Time to process a range of " << _deque.size()
              << " elements with std::deque : " << deqTime << " us" << std::endl;
}