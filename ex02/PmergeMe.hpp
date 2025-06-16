#pragma once

#include <vector>
#include <deque>
#include <string>

class PmergeMe {
public:
    PmergeMe(void);
    PmergeMe(const PmergeMe &copy);
    PmergeMe &operator=(const PmergeMe &copy);
    ~PmergeMe(void);

    void parseInput(char **argv);
    void sortAndMeasure(void);

private:
    std::vector<int> _vector;
    std::deque<int>  _deque;

    void printSequence(const std::string &label, const std::vector<int> &sequence) const;
    void printSequence(const std::string &label, const std::deque<int> &sequence) const;

    void fordJohnsonSort(std::vector<int> &sequence);
    void fordJohnsonSort(std::deque<int> &sequence);

    std::vector<size_t> generateJacobsthalOrder(size_t n);
};

