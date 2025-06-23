#include "./BitcoinExchange.hpp"
#include <sstream>
#include <cstdlib>
#include <cerrno>
#include <climits>
#include <fstream>
#include <iostream>
#include <climits>

BitcoinExchange::BitcoinExchange(void) {}

BitcoinExchange::BitcoinExchange(const BitcoinExchange &copy)
{
    *this = copy;
}

BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &copy)
{
    if (this != &copy)
        this->_database = copy._database;
    return (*this);
}

BitcoinExchange::~BitcoinExchange(void) {}

bool    BitcoinExchange::isValidDate(const std::string &date) const
{
    int y, m, d;
    const int   daysInMonth[12] = {
        31, 28, 31, 30, 31, 30,
        31, 31, 30, 31, 30, 31
    };

    if ((date.length() != 10) || (date[4] != '-') || (date[7] != '-'))
        return (false);

    std::istringstream yearStream(date.substr(0, 4));
    std::istringstream monthStream(date.substr(5, 2));
    std::istringstream dayStream(date.substr(8, 2));

    if (!(yearStream >> y) || !(monthStream >> m) || !(dayStream >> d))
        return (false);

    if ((y < 0) || (m < 1) || (m > 12) || (d < 1) || (d > 31))
        return (false);
    if (d > daysInMonth[m - 1])
        return (false);
    return (true);
}

bool    BitcoinExchange::isValidValue(const std::string &value) const
{
    errno = 0;
    char    *end = NULL;
    double  val = std::strtod(value.c_str(), &end);

    if ((errno == ERANGE) || (*end) || (val < 0) || (val > static_cast<double>(INT_MAX)))
        return (false);
    return (true);
}

bool    BitcoinExchange::loadDatabase(const std::string &filename)
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
        std::istringstream  ss(line);
        std::string         date, value;

        if ((!getline(ss, date, ',')) || (!getline(ss, value)))
            continue;
        if (!isValidDate(date))
            continue;
        if (!isValidValue(value))
            continue;
        errno = 0;
        float val = static_cast<float>(std::strtod(value.c_str(), NULL));
        if (errno == ERANGE)
            continue;
        this->_database[date] = val;
    }
    file.close();
    return (true);
}

void    BitcoinExchange::evaluatedFile(const std::string &filename) const
{
    std::ifstream file(filename.c_str());
    if (!file.is_open())
    {
        std::cerr << "Error: could not open file.\n";
        return ;
    }

    std::string line;
    getline(file, line);
    while (getline(file, line))
    {
        std::istringstream  ss(line);
        std::string         date, value;

        if ((!getline(ss, date, '|')) || (!getline(ss, value)))
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

        errno = 0;
        char    *end = NULL;
        double  amount = std::strtod(value.c_str(), &end);

        if ((errno == ERANGE) || (*end) || (amount < 0) || (amount > static_cast<double>(INT_MAX)))
        {
            if (amount < 0)
                std::cerr << "Error: not a positive number.\n";
            else
                std::cerr << "Error: too large a number.\n";
            continue;
        }
        std::map<std::string, float>::const_iterator it = _database.lower_bound(date);
        if ((it == _database.end()) || (it->first != date))
        {
            if (it == _database.begin())
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