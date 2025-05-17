#pragma once

#include <vector>
#include <deque>
#include <string>

class PmergeMe
{
    private:
        std::vector<int> _vector;
        std::deque<int> _deque;
        void fordJohnsonSort(std::vector<int> &sequence);
        void fordJohnsonSort(std::deque<int> &sequence);
    public:
        PmergeMe(void);
        PmergeMe(const PmergeMe &copy);
        PmergeMe &operator=(const PmergeMe &copy);
        ~PmergeMe(void);

        void parseInput(char **argv);
        void sortAndMeasure();
        void printSequence(const std::string &label, const std::vector<int> &sequence) const;
};
