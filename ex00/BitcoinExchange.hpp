#pragma once

#include <map>
#include <string>
#include <fstream>
#include <iostream>

class BitcoinExchange
{
    private:
        std::map<std::string, float> _database;
        
        bool isValidDate(const std::string &date) const;
        bool isValidValue(const std::string &value) const;

    public:
        BitcoinExchange(void);
        BitcoinExchange(const BitcoinExchange &copy);
        BitcoinExchange &operator=(const BitcoinExchange &copy);
        ~BitcoinExchange(void);

        bool loadDatabase(const std::string &filename);
        void evaluatedFile(const std::string &filename) const;
};
