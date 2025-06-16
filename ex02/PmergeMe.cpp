#include "PmergeMe.hpp"

#include <iostream>
#include <sstream>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <stdexcept>
#include <iomanip>
#include <climits>

// Constructeurs / opérateurs / destructeur
PmergeMe::PmergeMe(void) { }
PmergeMe::PmergeMe(const PmergeMe &copy) { *this = copy; }
PmergeMe &PmergeMe::operator=(const PmergeMe &copy) {
    if (this != &copy) {
        _vector = copy._vector;
        _deque = copy._deque;
    }
    return *this;
}
PmergeMe::~PmergeMe(void) { }

// Parse input arguments
void PmergeMe::parseInput(char **argv) {
    for (int i = 1; argv[i]; ++i) {
        std::string arg(argv[i]);
        for (size_t j = 0; j < arg.length(); ++j)
            if (!isdigit(arg[j]))
                throw std::runtime_error("Error");
        long val = std::atol(arg.c_str());
        if (val < 0 || val > INT_MAX)
            throw std::runtime_error("Error");
        _vector.push_back(static_cast<int>(val));
        _deque.push_back(static_cast<int>(val));
    }
    if (_vector.empty())
        throw std::runtime_error("Error");
}

// Affichage
void PmergeMe::printSequence(const std::string &label, const std::vector<int> &sequence) const {
    std::cout << label;
    for (size_t i = 0; i < sequence.size(); ++i)
        std::cout << " " << sequence[i];
    std::cout << std::endl;
}

void PmergeMe::printSequence(const std::string &label, const std::deque<int> &sequence) const {
    std::cout << label;
    for (size_t i = 0; i < sequence.size(); ++i)
        std::cout << " " << sequence[i];
    std::cout << std::endl;
}

// Génération de l'ordre Jacobsthal
std::vector<size_t> PmergeMe::generateJacobsthalOrder(size_t n) {
    std::vector<size_t> order;
    size_t j1 = 1, j2 = 1;
    while (j1 < n) {
        order.push_back(j1);
        size_t tmp = j1;
        j1 = j1 + 2 * j2;
        j2 = tmp;
    }
    for (size_t i = 0; i < n; ++i) {
        bool found = false;
        for (size_t j = 0; j < order.size(); ++j) {
            if (order[j] == i) {
                found = true;
                break;
            }
        }
        if (!found)
            order.push_back(i);
    }
    return order;
}

// Tri Ford‑Johnson pour std::vector
void PmergeMe::fordJohnsonSort(std::vector<int> &seq) {
    size_t n = seq.size();
    if (n <= 1) return;

    std::vector<int> bigs, smalls;
    for (size_t i = 0; i + 1 < n; i += 2) {
        int a = seq[i], b = seq[i+1];
        if (a < b) std::swap(a, b);
        bigs.push_back(a);
        smalls.push_back(b);
    }
    if (n % 2 != 0)
        bigs.push_back(seq.back());

    fordJohnsonSort(bigs);

    std::vector<size_t> order = generateJacobsthalOrder(smalls.size());
    for (size_t i = 0; i < order.size(); ++i) {
        size_t idx = order[i];
        if (idx < smalls.size()) {
            int v = smalls[idx];
            std::vector<int>::iterator it = std::upper_bound(bigs.begin(), bigs.end(), v);
            bigs.insert(it, v);
        }
    }
    seq = bigs;
}

// Tri Ford‑Johnson pour std::deque
void PmergeMe::fordJohnsonSort(std::deque<int> &seq) {
    size_t n = seq.size();
    if (n <= 1) return;

    std::deque<int> bigs, smalls;
    for (size_t i = 0; i + 1 < n; i += 2) {
        int a = seq[i], b = seq[i+1];
        if (a < b) std::swap(a, b);
        bigs.push_back(a);
        smalls.push_back(b);
    }
    if (n % 2 != 0)
        bigs.push_back(seq.back());

    fordJohnsonSort(bigs);

    std::vector<size_t> order = generateJacobsthalOrder(smalls.size());
    for (size_t i = 0; i < order.size(); ++i) {
        size_t idx = order[i];
        if (idx < smalls.size()) {
            int v = smalls[idx];
            std::deque<int>::iterator it = std::upper_bound(bigs.begin(), bigs.end(), v);
            bigs.insert(it, v);
        }
    }
    seq = bigs;
}

// Mesure du temps
void PmergeMe::sortAndMeasure(void) {
    std::vector<int> vCopy = _vector;
    std::deque<int> dCopy = _deque;

    printSequence("Before:", _vector);

    clock_t s = clock();
    fordJohnsonSort(vCopy);
    clock_t e = clock();
    double tv = static_cast<double>(e - s) / CLOCKS_PER_SEC * 1e6;

    s = clock();
    fordJohnsonSort(dCopy);
    e = clock();
    double td = static_cast<double>(e - s) / CLOCKS_PER_SEC * 1e6;

    printSequence("After:", vCopy);
    std::cout << std::fixed << std::setprecision(5);
    std::cout << "Time to process a range of " << _vector.size()
              << " elements with std::vector : " << tv << " us" << std::endl;
    std::cout << "Time to process a range of " << _deque.size()
              << " elements with std::deque : " << td << " us" << std::endl;
}
