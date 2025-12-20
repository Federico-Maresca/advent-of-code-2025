#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <iterator>
#include <numeric> // Required for std::reverse
#include <sstream>

constexpr char ADD = '+';
constexpr char MUL = '*';
constexpr char DELIM = ' ';

void readFiles(const std::string &fileName, std::string &operations, std::vector<std::vector<long>> &problems)
{
    std::ifstream infile(fileName, std::ios::ate); // open file and seek end
    infile.seekg(-1, std::ios::end);
    char ch = infile.get();
    std::streampos pos = infile.tellg(); // position at end
    if (ch == '\n')
    {
        infile.seekg(-1, std::ios::cur);
        pos = infile.tellg(); // update current pos
    }
    bool first = true;
    int count = 3;
    while (pos > 0)
    {
        infile.seekg(-2, std::ios::cur);
        ch = infile.get();
        pos = infile.tellg(); // update current pos
        if (ch == '\n')       // We are at end of previous line
        {
            break;
        }
        if (ch == '*' || ch == '+')
        {
            operations.insert(operations.begin(), ch);
            problems.insert(problems.begin(), std::vector<long>(count-1));
            count = 0;
        }
        count++;
    }
}

void solve1(const std::string &filename, const std::string &operations)
{

    std::ifstream infile(filename);
    std::string line;
    bool firstLine = true;
    std::vector<long> results(operations.size());

    while (std::getline(infile, line))
    {
        if (line.at(0) != ADD && line.at(0) != MUL)
        {
            std::istringstream stringStream(line);
            int number;
            int count = 0;
            while (stringStream >> number)
            {
                if (operations.at(count) == '*')
                {
                    if (results.at(count) == 0)
                        results.at(count) = 1;
                    auto save = results.at(count);
                    results.at(count) *= number;
                }
                else
                {
                    auto save = results.at(count);
                    results.at(count) += number;
                }
                count++;
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::accumulate(results.begin(), results.end(), 0ULL) << std::endl;
}

void solve2(const std::string &filename, std::vector<std::vector<long>> &problems, const std::string &operations)
{
    std::ifstream infile(filename);
    std::string line;
    while (std::getline(infile, line))
    {
        int count = 0;
        auto charIter = line.begin();
        if (line.at(0) == ADD || line.at(0) == MUL)
        {
            break;
        }
        do{
            auto &currentProblem = problems.at(count);
            for (int i = 0; i < currentProblem.size(); i++)
            {
                if(*charIter != ' ')
                    currentProblem.at(i) = currentProblem.at(i) * 10 + (*charIter - '0');
                charIter++;
            }
            if(*charIter == '\0')
                break;
            charIter++;
            count++;
        }
        while (*charIter != '\0');
    }
    long sum = 0;
    for (int i = 0; i < operations.size(); i++)
    {
        if (operations.at(i) == '*')
        {
            sum +=std::accumulate(problems.at(i).begin(), problems.at(i).end(), 1ULL, std::multiplies<long long>());
        }
        else
        {
            sum +=std::accumulate(problems.at(i).begin(), problems.at(i).end(), 0ULL);
        }
    }
    std::cout << "Second solution: " << sum << std::endl;
}

int main(int argc, char **argv)
{
    std::vector<long> results1;
    std::string operations;
    std::vector<uint8_t> numberSizes;
    std::vector<std::vector<long>> results2;
    readFiles("input", operations, results2);
    solve1("input", operations);
    solve2("input", results2, operations);
}
