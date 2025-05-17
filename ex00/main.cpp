#include "BitcoinExchange.hpp"

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cerr << "Error: could not open file.\n";
        return (1);
    }
    BitcoinExchange btc;
    if (!btc.loadDatabase("data.csv"))
        return (1);
    btc.evaluatedFile(argv[1]);
    return (0);
}