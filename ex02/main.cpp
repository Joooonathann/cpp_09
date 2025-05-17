#include "PmergeMe.hpp"
#include <iostream>

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cerr << "Usage: ./PmergeMe <sequence of positive integers>" << std::endl;
        return (1);
    }

    try
    {
        PmergeMe sorter;
        sorter.parseInput(argv);
        sorter.sortAndMeasure();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return (1);
    }
    return (0);
}