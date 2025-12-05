#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <string>

void read(const std::string &filename, std::vector<std::vector<uint8_t>> &input)
{
    // 1. Read all lines into a vector
    std::ifstream infile(filename);
    std::string line;
    while (std::getline(infile, line))
    {
        input.push_back(std::vector<uint8_t>());
        for (char digit : line)
        {
            input.back().push_back(digit - '0');
        }
    }
}

unsigned long long int solve(std::vector<std::vector<uint8_t>> &input, uint8_t digits)
{
    unsigned long int sum = 0;
    for (auto& batteryBank : input)
    {
        if (digits > batteryBank.size())
            continue;
        uint64_t result = 0;
        auto currentMax = std::begin(batteryBank);
        for (int8_t count = digits - 1; count >= 0; count--)
        {
            currentMax = std::max_element(
                currentMax, batteryBank.end() - count);
            result = result * 10 + (*currentMax);
            currentMax = currentMax + 1;
        }
        sum += result;
    }
    return sum;
}

int main(int argc, char **argv)
{
    std::vector<std::vector<uint8_t>> input;
    //read(argv[1], input);
    read("input1", input);

    std::cout << "First Puzzle : " << solve(input, 2) << std::endl;
    std::cout << "Second Puzzle : " << solve(input, 12) << std::endl;
}