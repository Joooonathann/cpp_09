#include "BitcoinExchange.hpp"
#include <sstream>
#include <cstdlib>

BitcoinExchange::BitcoinExchange(void) { }

BitcoinExchange::BitcoinExchange(const BitcoinExchange &copy) { *this = copy; }

BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &copy)
{
    if (this != &copy)
        this->_database = copy._database;
    return (*this);
}

BitcoinExchange::~BitcoinExchange(void) { }

bool BitcoinExchange::isValidDate(const std::string &date) const
{
    if (date.length() != 10 || date[4] != '-' || date[7] != '-')
        return (false);
    int y, m, d;
    char dash1, dash2;
    std::istringstream ss(date);
    if (!(ss >> y >> dash1 >> m >> dash2 >> d))
        return (false);
    if (dash1 != '-' || dash2 != '-' || y < 0 || m < 1 || m > 12 || d < 1 || d > 31)
        return (false);
    return (true);
}

bool BitcoinExchange::isValidValue(const std::string &value) const
{
    char *end;
    double val = std::strtod(value.c_str(), &end);
    if (*end || val < 0 || val > 1000)
        return (false);
    return (true);
}

bool BitcoinExchange::loadDatabase(const std::string &filename)
{
    std::ifstream file(filename.c_str());
    if (!file.is_open())
    {
        std::cerr << "Error: could not open database.\n";
        return (false);
    }
    std::string line;
    getline(file, line);
    while (getline(file, line))
    {
        std::istringstream ss(line);
        std::string date, value;
        if (!getline(ss, date, ',') || !getline(ss, value))
            continue;
        if (!isValidDate(date))
            continue;
        _database[date] = std::atof(value.c_str());
    }
    file.close();
    return (true);
}

void BitcoinExchange::evaluatedFile(const std::string &filename) const
{
    std::ifstream file(filename.c_str());
    if (!file.is_open())
    {
        std::cerr << "Error: could not open file.\n";
        return;
    }
    std::string line;
    getline(file, line);
    while (getline(file, line))
    {
        std::istringstream ss(line);
        std::string date, value;
        if (!getline(ss, date, '|') || !getline(ss, value))
        {
            std::cerr << "Error: bad input => " << line << '\n';
            continue;
        }
        date = date.substr(0, date.find_last_not_of(" \t") + 1);
        value = value.substr(value.find_first_not_of(" \t"));
        if (!isValidDate(date))
        {
            std::cerr << "Error: bad input => " << date << '\n';
            continue;
        }
        if (!isValidValue(value))
        {
            double d = std::strtod(value.c_str(), NULL);
            if (d < 0)
                std::cerr << "Error: not a positive number.\n";
            else
                std::cerr << "Error: too large a number.\n";
            continue;
        }
        double amount = std::strtod(value.c_str(), NULL);
        std::map<std::string, float>::const_iterator it = this->_database.lower_bound(date);
        if (it == this->_database.end() || it->first != date)
        {
            if (it == this->_database.begin())
            {
                std::cerr << "Error: no earlier date in database.\n";
                continue;
            }
            --it;
        }
        std::cout << date << " => " << value << " = " << amount * it->second << std::endl;
    }
    file.close();
}