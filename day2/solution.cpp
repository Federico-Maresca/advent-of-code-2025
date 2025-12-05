#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
long long firstInvalidIdSum = 0;
long long secondInvalidIdSum = 0;

void checkForInvalidId(const long id)
{
    std::string idStr = std::to_string(id);
    size_t len = idStr.length();
    for (int dividend = 2; dividend <= (len / 2) + 1; ++dividend)
    {
        if (len % dividend == 0)
        {
            std::string part = idStr.substr(0, len / dividend);
            if(part[0] == '0')
            {
                continue;
            }
            std::string constructed;
            for (int i = 0; i < dividend; ++i)
            {
                constructed += part;
            }
            if (constructed == idStr)
            {
                if (dividend == 2)
                {
                    firstInvalidIdSum += id;
                }
                secondInvalidIdSum += id;
                return;
            }
        }
    }
    // Special check for single digit repeated len times
    if (len > 1 && idStr[0] != '0')
    {
        bool allSame = true;
        for (size_t i = 1; i < len; ++i)
        {
            if (idStr[i] != idStr[0])
            {
                allSame = false;
                break;
            }
        }
        if (allSame)
        {
            secondInvalidIdSum += id;
        }
    }
}
// Find invalid id in the ranges (including start and end) and add them up. Invalid id ranges are  any ID which is made only of some sequence of digits repeated twice.
// Leading zeros do not count
// ID is invalid if it is made only of some sequence of digits repeated at least twice. So, 12341234 (1234 two times),
// 123123123 (123 three times), 1212121212 (12 five times), and 1111111 (1 seven times) are all invalid IDs.
void solve(const std::vector<std::pair<long, long>> &idRanges)
{
    for (const auto &range : idRanges)
    {
        for (long id = range.first; id <= range.second; ++id)
        {
            checkForInvalidId(id);
        }
    }
}

void read(const std::string &filename, std::vector<std::pair<long, long>> &idRanges)
{
    // 1. Read all lines into a vector
    std::ifstream infile(filename);

    std::string line;
    // Read string from file, file has just one string made up of ranges, ranges are number-number, separated by commas
    const char delim = ',';
    while (std::getline(infile, line, delim))
    {
        size_t dashPos = line.find('-');
        if (dashPos != std::string::npos)
        {
            std::cout << "Processing line: " << line << "\n";
            long start = std::stol(line.substr(0, dashPos));
            long end = std::stol(line.substr(dashPos + 1));
            idRanges.emplace_back(start, end);
        }
    }
}

int main(int argc, char **argv)
{
    std::vector<std::pair<long, long>> idRanges;
    // Use a function call to cleanly execute the logic
    read(argv[1], idRanges);
    // Debugging print to output read ranges
    for (const auto &range : idRanges)
    {
        std::cout << "Read range: " << range.first << "-" << range.second << "\n";
    }
    solve(idRanges);
    std::cout << "Invalid ranges : " << firstInvalidIdSum << "\n";
    std::cout << "Invalid ranges : " << secondInvalidIdSum << "\n";
    return 0;
}