#ifndef AUXILIAR_HPP
#define AUXILIAR_HPP

#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <vector>

/// Given a CSV file, takes all the strings between commas (",") and stores them in a vector<vector<string>>
int parserInputCSV(std::string fileName, std::vector<std::vector<std::string>> *result)
{
    std::ifstream ifile(fileName);
    if(ifile.is_open())
    {
        std::string line;
        size_t beg, end;
        size_t row = 0;

        while(std::getline(ifile, line))
        {
            beg = end = 0;
            result->push_back(std::vector<std::string>(0));

            while(end != std::string::npos)
            {
                end = line.find(",", beg);
                result->operator[](row).push_back(line.substr(beg, end-beg));
                beg = end + 1;
            }

            ++row;
        }

        ifile.close();
    }
    else
    {
        std::cout << "Cannot open file (" << fileName << ")" << std::endl;
        return -1;
    }

    return 0;
}

/// Given a CSV file, takes all the element between quotation marks ("") and stores them in a vector<vector<string>>
int parserInputCSV2(std::string fileName, std::vector<std::vector<std::string>> *result)
{
    std::ifstream ifile(fileName);
    if(ifile.is_open())
    {
        std::string line;
        size_t beg, end;
        size_t row = 0;

        while(std::getline(ifile, line))
        {
            beg = line.find("\"", 0);
            result->push_back(std::vector<std::string>(0));

            while(beg != std::string::npos)
            {
                end = line.find("\"", beg+1);
                //std::cout << beg << ", " << end << std::endl;
                result->operator[](row).push_back(line.substr(beg+1, end-beg-1));
                //std::cout << result->operator[](row)[result->operator[](row).size()-1] << std::endl;
                beg = line.find("\"", end+1);
            }

            ++row;
        }

        ifile.close();
    }
    else
    {
        std::cout << "Cannot open file (" << fileName << ")" << std::endl;
        return -1;
    }

    return 0;
}

/// Add another apostrophe (') next to any existing apostrophe in the string (SQL scape sequence for ' is '')
void add_extra_apostrophes(std::string &str)
{
    size_t siz = str.size();

    for(size_t i = 0; i < siz; i++)
        if(str[i] == '\'')
        {
            str.insert(i, "\'");
            siz++;
            i++;
        }
}

/// [Overloaded] Given a string and a character, get the same string but without that character
std::string erase_all(std::string str, char character)
{
    std::string output;
    output.reserve(str.size());     // set string capacity

    for(size_t i = 0; i < str.size(); ++i)
        if(str[i] != character) output += str[i];

    return output;
}

/// [Overloaded] Given a first string and a second string, get the same first string but without that second string
std::string erase_all(std::string str, std::string word)
{
    std::string output;
    output.reserve(str.size());     // set string capacity
    size_t len = word.size();

    for(size_t i = 0; i < str.size(); ++i)
        if(str.substr(i, len) != word) output += str[i];
        else
            i += (len-1);

    return output;
}

#endif
