#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

uint totalFreshAvailable = 0;

// Represents a range boundary event (start or end of a fresh ingredient range)
struct RangeEvent
{
    unsigned long value;
    bool isStart;
    
    // Custom comparator: sort by value, then place starts before ends
    bool operator<(const RangeEvent& other) const
    {
        if (value != other.value)
            return value < other.value;
        return isStart; // place range starts before equal range ends
    }
};

// Check if an ingredient value falls within any of the fresh ranges
bool isIngredientFresh(const std::vector<RangeEvent>& freshRanges, long ingredient)
{
    for (size_t i = 0; i < freshRanges.size(); i += 2)
    {
        if (ingredient >= freshRanges[i].value && ingredient <= freshRanges[i + 1].value)
            return true;
    }
    return false;
}

// Read input file: parse fresh ranges and count fresh ingredients
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
            // Check each ingredient and count if fresh
            totalFreshAvailable += isIngredientFresh(freshRanges, std::stoll(line));
        }
        else
        {
            // Parse range as "start-end" format
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

// Calculate total count of values covered by overlapping ranges
unsigned long calculateFreshTotal(const std::vector<RangeEvent>& freshRanges)
{
    unsigned long totalFresh = 0;
    uint activeRanges = 0;
    unsigned long rangeStart = 0;

    // Use sweep line algorithm to merge overlapping ranges
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
        
        // When no ranges are active, add the merged range size
        if (activeRanges == 0)
            totalFresh += freshRanges[i].value - rangeStart + 1;
    }
    
    return totalFresh;
}

int main(int argc, char** argv)
{
    std::vector<RangeEvent> freshRanges;
    readInput(argv[1], freshRanges);
    
    // Part 1: Count fresh ingredients
    std::cout << "First answer: " << totalFreshAvailable << "\n";

    // Sort range events before calculating total coverage
    std::sort(freshRanges.begin(), freshRanges.end());
    
    // Part 2: Calculate total fresh value range
    std::cout << "Second answer: " << calculateFreshTotal(freshRanges) << "\n";
    
    return 0;
}
