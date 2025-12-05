#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <string>

uint8_t ROW_MAX;
uint8_t COLUMN_MAX;

void read(const std::string &filename, std::vector<std::vector<bool>> &input)
{
    // 1. Read all lines into a vector
    std::ifstream infile(filename);
    std::string line;
    while (std::getline(infile, line))
    {
        input.push_back(std::vector<bool>());
        for (char roll : line)
            input.back().push_back(roll == '@' ? true : false);
    }
}

bool canRemove(std::vector<std::vector<bool>> &input, uint row, uint column)
{
    uint8_t rollsAroundroll = 0;

    const uint8_t startRow = std::max(0, (int)row - 1);
    const uint8_t endRow = std::min((int)input.size() - 1, (int)row + 1);
    const uint8_t startColumn = std::max(0, (int)column - 1);
    const uint8_t endColumn = std::min((int)input.size() - 1, (int)column + 1);

    for (uint i = startRow; i <= endRow && rollsAroundroll < 4; i++)
    {
        for (uint j = startColumn; j <= endColumn && rollsAroundroll < 4; j++)
        {
            if (i == row && j == column)
                continue;
            rollsAroundroll += input.at(i).at(j);
        }
    }
    return rollsAroundroll < 4;
}
uint solve1(std::vector<std::vector<bool>> &input)
{
    uint removedRolls = 0;
    for (uint i = 0; i < ROW_MAX; i++)
        for (uint j = 0; j < COLUMN_MAX; j++)
            if (input.at(i).at(j) && canRemove(input, i, j))
                removedRolls++;
    return removedRolls;
}

uint solve2(std::vector<std::vector<bool>> &input, uint row, uint column)
{
    if (row == ROW_MAX)
        return 0;
    if (column == COLUMN_MAX)
        return solve2(input, row + 1, 0);
    bool removedRoll = false;

    if (input.at(row).at(column))
        if (canRemove(input, row, column))
            removedRoll = true;
    int current = removedRoll + solve2(input, row, column + 1);
    if (removedRoll)
        input.at(row).at(column) = false;
    return current;
}

int main(int argc, char **argv)
{
    std::vector<std::vector<bool>> input;

    read(argv[1], input);

    ROW_MAX = input.size();
    COLUMN_MAX = input.front().size();
    std::cout << "First Solution :" << solve1(input) << std::endl;

    int currentRollsRemoved = 0;
    uint totalRollsRemoved = 0;
    do
    {
        currentRollsRemoved = solve2(input, 0, 0);
        totalRollsRemoved += currentRollsRemoved;
    } while (currentRollsRemoved != 0);
    std::cout << "Second Solution :" << totalRollsRemoved << std::endl;
}