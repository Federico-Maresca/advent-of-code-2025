#include <iostream>
#include <vector>
#include <fstream>
#include <numeric>

void solve(std::string fileName)
{
    std::ifstream infile(fileName);
    std::string line;

    std::getline(infile, line);
    std::vector<unsigned long> beamsPathCount(line.size());
    beamsPathCount[line.find('S')] = 1;

    unsigned int numberOfSplitsHit = 0; // number of beams split is the number of splitters hit by a beam
    while (std::getline(infile, line))
    {
        std::size_t pos = -1;
        while ((pos = line.find('^', pos + 1)) != std::string::npos)
        {
            if (beamsPathCount[pos] != 0)
            {
                ++numberOfSplitsHit;
                beamsPathCount[pos - 1] += beamsPathCount[pos]; // accumulate paths count into the final beams
                beamsPathCount[pos + 1] += beamsPathCount[pos];
                beamsPathCount[pos] = 0;
            }
        }
    }

    // number of paths is the sum of all the paths each beam represents
    unsigned long totalPaths = 0;
    totalPaths = std::accumulate(beamsPathCount.begin(), beamsPathCount.end(), 0UL);

    std::cout << "Number of beams created: " << numberOfSplitsHit << std::endl;
    std::cout << "Number of timelines: " << totalPaths << std::endl;
}
int main(int argc, char **argv)
{
    solve(argv[1]);
    return 0;
}