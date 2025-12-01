#include <iostream>
#include <fstream>
#include <string>
#include <ranges>
#include <numeric>
#include <vector>

namespace ranges = std::ranges;
namespace
{
    constexpr uint8_t DIAL_SIZE = 100;
    constexpr uint8_t START_POS = 50;
    // Helper function to process a single line and calculate zero crossings
    // This function takes the current position by reference and updates it.
    int firstPW = 0;
    int secondPW = 0;
    long long process_rotation(uint8_t &current_pos, const int &rotation_amount)
    {
        const int steps = std::abs(rotation_amount);
        const uint8_t start_position = current_pos;
        uint32_t zero_crossings = 0;

        const uint16_t quotient = steps / DIAL_SIZE;
        const uint8_t remainder = steps % DIAL_SIZE;

        // Add a 0 cross for each full dial rotation
        zero_crossings += quotient;

        // Calculate possible extra zero crossing for the partial rotation
        if (rotation_amount < 0)
        {
            if (start_position != 0 && start_position <= remainder)
            {
                zero_crossings++;
            }
            current_pos = ((start_position - remainder) % DIAL_SIZE + DIAL_SIZE) % DIAL_SIZE;
        }
        else
        { // 'R'
            if (start_position != 0 && start_position + remainder >= DIAL_SIZE)
            {
                zero_crossings++;
            }
            // Update current position
            current_pos = (start_position + remainder) % DIAL_SIZE;
        }
        if (current_pos == 0)
        {
            firstPW++;
        }
        return zero_crossings;
    }

    void solve(const std::string &filename)
    {
        // 1. Read all lines into a vector
        std::ifstream infile(filename);
        std::vector<int> rotation_lines;
        std::string line;
        while (std::getline(infile, line))
        {
            if (!line.empty())
            {
                rotation_lines.push_back(
                    [&line]()
                    { return line.front() == 'L' ? -(std::stoi(line.substr(1))) : std::stoi(line.substr(1)); }());
            }
        }

        // 2. Initialize state for the reduction
        uint8_t current_pos = START_POS;

        // 3. Use std::accumulate (or a similar algorithm) for the range reduction
        // This accumulates the zero crossings while modifying the current_pos state.
        secondPW = std::accumulate(
            rotation_lines.begin(),
            rotation_lines.end(),
            0LL, // Initial value of total zero crossings
            [&current_pos](long long total, const int &rotation_amount)
            {
                // Binary operation: add the result of processing the rotation line
                return total + process_rotation(current_pos, rotation_amount);
            });
    }
}
int main(int argc, char **argv)
{
    // Use a function call to cleanly execute the logic
    solve(argv[1]);

    std::cout << "First Password : " << firstPW << "\n";
    std::cout << "Second Password : " << secondPW << "\n";
    return 0;
}