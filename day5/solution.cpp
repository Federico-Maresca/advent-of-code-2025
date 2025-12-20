#include <iostream>
#include <fstream>
#include <string>
#include <ranges>
#include <numeric>
#include <vector>
#include <map>
#include <algorithm>

uint totalFreshAvailable = 0;

struct RangeEvent
{
    unsigned long value;
    bool isStart;
    
    bool operator<(const RangeEvent& other) const
    {
        if (value != other.value)
            return value < other.value;
        return isStart; // place range starts before equal range ends
    }
};

bool isIngredientFresh(const std::vector<RangeEvent>& freshRanges, long ingredient)
{
    for (size_t i = 0; i < freshRanges.size(); i += 2)
    {
        if (ingredient >= freshRanges[i].value && ingredient <= freshRanges[i + 1].value)
            return true;
    }
    return false;
}

void readInput(const std::string& filename, std::vector<RangeEvent>& freshRanges)
{
    std::ifstream file(filename);
    std::string line;
    bool readingIngredients = false;
    
    while (std::getline(file, line))
    {
        if (line.empty())
        {
            readingIngredients = true;
            continue;
        }

        if (readingIngredients)
        {
            totalFreshAvailable += isIngredientFresh(freshRanges, std::stoll(line));
        }
        else
        {
            size_t dashPos = line.find('-');
            if (dashPos != std::string::npos)
            {
                unsigned long start = std::stoul(line.substr(0, dashPos));
                unsigned long end = std::stoul(line.substr(dashPos + 1));
                freshRanges.push_back({start, true});
                freshRanges.push_back({end, false});
            }
        }
    }
}

unsigned long calculateFreshTotal(const std::vector<RangeEvent>& freshRanges)
{
    unsigned long totalFresh = 0;
    int activeRanges = 0;
    unsigned long rangeStart = 0;

    for (size_t i = 0; i < freshRanges.size(); i++)
    {
        if (freshRanges[i].isStart)
        {
            if (activeRanges == 0)
                rangeStart = freshRanges[i].value;
            activeRanges++;
        }
        else
        {
            activeRanges--;
        }
        
        if (activeRanges == 0)
            totalFresh += freshRanges[i].value - rangeStart + 1;
    }
    
    return totalFresh;
}

int main(int argc, char** argv)
{
    std::vector<RangeEvent> freshRanges;
    readInput(argv[1], freshRanges);
    
    std::cout << "First answer: " << totalFreshAvailable << "\n";

    std::sort(freshRanges.begin(), freshRanges.end());
    
    std::cout << "Second answer: " << calculateFreshTotal(freshRanges) << "\n";
    
    return 0;
}
