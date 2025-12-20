#include <algorithm>
#include <bit>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <filesystem>
#include <iostream>
#include <limits>
#include <sstream>
#include <fstream>
#include <string>
#include <string_view>
#include <vector>

// ---------------------------------------------------------------------
// Parsing helpers
// ---------------------------------------------------------------------

struct Button
{
    std::vector<int> toggles;
};

struct Machine
{
    std::vector<int> lights;
    std::vector<Button> buttons;
    std::vector<int> joltReqs;
};

std::vector<Machine> extract_info(const std::vector<std::string> &lines)
{
    std::vector<Machine> machines;
    for (const auto &line : lines)
    {
        machines.push_back(Machine());
        bool insideLight = false;
        bool insideButton = false;
        bool insideTarget = false;
        for (char c : line)
        {
            if (c == '[')
            {
                insideLight = true;
                continue;
            }
            if (c == ']')
            {
                insideLight = false;
                continue;
            }
            if (insideLight)
            {
                machines.back().lights.push_back(c == '#' ? 1 : 0);
                continue;
            }
            if (c == '(')
            {
                insideButton = true;
                machines.back().buttons.push_back(Button());
                continue;
            }
            if (c == ')')
            {
                insideButton = false;
                continue;
            }
            if (insideButton && isdigit(c))
            {
                machines.back().buttons.back().toggles.push_back(c - '0');
                continue;
            }
            if (c == '{')
            {
                insideTarget = true;
                continue;
            }
            if (insideTarget && isdigit(c))
            {
                machines.back().joltReqs.push_back(c - '0');
                continue;
            }
        }
    }
    return machines;
}

// Function to read and parse the line according to the rules
void parse_line(const std::string &line) {
    std::string s_vec1, s_vec2, s_vec3;

    // Find the three parts using character delimiters
    size_t pos1 = line.find(']');
    size_t pos2 = line.find('{');

    if (pos1 == std::string::npos || pos2 == std::string::npos || pos2 < pos1) return;

    s_vec1 = line.substr(1, pos1 - 1);
    s_vec2 = line.substr(pos1 + 2, pos2 - (pos1 + 3));
    s_vec3 = line.substr(pos2 + 1, line.length() - pos2 - 2);

    // Vector 1: [ . # . . ] -> vector<int> (0,1,0,0)
    std::vector<int> vec1;
    for (char c : s_vec1) {
        if (c == '#') vec1.push_back(1);
        else if (c == '.') vec1.push_back(0);
    }

    // Vector 2: ( ) ( ) -> vector<vector<int>>
    std::vector<std::vector<int>> vec2;
    std::string segment;
    std::stringstream ss2(s_vec2);
    while (getline(ss2, segment, ')')) {
        if (segment.empty() || segment.find('(') == std::string::npos) continue;
        
        std::vector<int> sub_vec;
        std::stringstream ss_sub(segment.substr(segment.find('(') + 1));
        std::string number_str;
        
        while (getline(ss_sub, number_str, ',')) {
            if (!number_str.empty()) {
                sub_vec.push_back(std::stoi(number_str));
            }
        }
        vec2.push_back(sub_vec);
    }

    // Vector 3: { 3, 5, 4 } -> vector<int>
    std::vector<int> vec3;
    std::stringstream ss3(s_vec3);
    std::string number_str;
    while (getline(ss3, number_str, ',')) {
        if (!number_str.empty()) {
            vec3.push_back(stoi(number_str));
        }
    }

    // Output for verification (Optional, can be removed to reduce lines)
    std::cout << "Vec1 size: " << vec1.size() << " | Vec2 size: " << vec2.size() << " | Vec3 size: " << vec3.size() << std::endl;
}

std::vector<int> parse_number_list(const std::string &s)
{
    std::vector<int> nums;
    std::stringstream ss(s);
    std::string tok;

    while (std::getline(ss, tok, ','))
    {
        if (!tok.empty())
            nums.push_back(std::stoi(tok));
    }
    return nums;
}

// ---------------------------------------------------------------------
// PART 1: Brute-force bitmask search
// ---------------------------------------------------------------------

int solve_machine_part1(const Machine& machine)
{
    const int nLights =machine.lights.size();

    const int nButtons = machine.buttons.size();
    const uint64_t limit = 1ULL << nButtons;

    int best = std::numeric_limits<int>::max();

    for (uint64_t mask = 0; mask < limit; ++mask)
    {
        int presses = std::popcount(mask);
        if (presses >= best)
            continue;

        std::vector<int> state(nLights, 0);

        for (int b = 0; b < nButtons; ++b)
        {
            if ((mask >> b) & 1)
            {
                for (int index : machine.buttons[b].toggles)
                    if (index < nLights)
                        state[index] ^= 1;
            }
        }

        if (state == machine.lights)
            best = presses;
    }

    return best == std::numeric_limits<int>::max() ? 0 : best;
}

uint64_t solve_part1(const std::vector<Machine> &machines)
{

    uint64_t total = 0;
    for (size_t i = 0; i < machines.size(); ++i)
        total += solve_machine_part1(machines[i]);

    return total;
}

// ---------------------------------------------------------------------
// PART 2: Linear system + Gaussian elimination + free var search
// ---------------------------------------------------------------------

long long solve_machine_part2(const Machine& machine)
{
    const int R = machine.joltReqs.size();
    const int C = machine.buttons.size();

    std::vector<std::vector<double>> A(R, std::vector<double>(C + 1, 0.0));

    for (int c = 0; c < C; ++c)
        for (int r : machine.buttons[c].toggles)
            if (r < R)
                A[r][c] = 1.0;

    for (int r = 0; r < R; ++r)
        A[r][C] = machine.joltReqs[r];

    int rank = 0;
    std::vector<int> pivotRow(C, -1);
    std::vector<int> pivotCols;
    std::vector<int> freeCols;

    // Gaussian elimination
    for (int c = 0; c < C && rank < R; ++c)
    {
        int sel = -1;
        for (int r = rank; r < R; ++r)
        {
            if (std::abs(A[r][c]) > 1e-9)
            {
                sel = r;
                break;
            }
        }
        if (sel == -1)
            continue;

        std::swap(A[rank], A[sel]);

        double div = A[rank][c];
        for (int j = c; j <= C; ++j)
            A[rank][j] /= div;

        for (int r = 0; r < R; ++r)
        {
            if (r != rank && std::abs(A[r][c]) > 1e-9)
            {
                double f = A[r][c];
                for (int j = c; j <= C; ++j)
                    A[r][j] -= f * A[rank][j];
            }
        }

        pivotRow[c] = rank;
        pivotCols.push_back(c);
        rank++;
    }

    // Check inconsistency
    for (int r = rank; r < R; ++r)
        if (std::abs(A[r][C]) > 1e-6)
            return 0;

    // Identify free vars
    for (int c = 0; c < C; ++c)
        if (pivotRow[c] == -1)
            freeCols.push_back(c);

    long long best = -1;

    std::vector<int> freeVals(freeCols.size());

    // DFS search over free vars (bound ~300)
    std::function<void(size_t)> dfs = [&](size_t idx)
    {
        if (idx == freeCols.size())
        {
            long long presses = 0;
            for (int v : freeVals)
                presses += v;

            bool ok = true;

            // compute pivot vars
            for (int pc : pivotCols)
            {
                int pr = pivotRow[pc];
                double val = A[pr][C];

                for (size_t i = 0; i < freeCols.size(); ++i)
                    val -= A[pr][freeCols[i]] * freeVals[i];

                if (val < -1e-9)
                {
                    ok = false;
                    break;
                }

                long long iv = llround(val);
                if (std::abs(val - iv) > 1e-4)
                {
                    ok = false;
                    break;
                }

                presses += iv;

                if (best != -1 && presses >= best)
                {
                    ok = false;
                    break;
                }
            }

            if (ok && (best == -1 || presses < best))
                best = presses;

            return;
        }

        for (int v = 0; v <= 300; ++v)
        {
            freeVals[idx] = v;

            long long partial = 0;
            for (size_t k = 0; k <= idx; ++k)
                partial += freeVals[k];
            if (best != -1 && partial >= best)
                break;

            dfs(idx + 1);
        }
    };

    dfs(0);
    return (best == -1 ? 0 : best);
}

uint64_t solve_part2(const std::vector<Machine> &machines)
{
    uint64_t total = 0;

    size_t index = 0;

    for(auto& machine : machines)
    {
        long long presses = solve_machine_part2(machine);
        total+=presses;
    }
    return total;
}

std::vector<std::string> readFile(const std::string &fileName)
{
    std::ifstream fin(fileName);
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(fin, line))
        if (!line.empty())
            lines.push_back(line);
    return lines;
}

int main(int argc, char *argv[])
{

    auto lines = readFile("input");
    std::vector<Machine> machines(std::move(extract_info(lines)));
    std::cout << "Part 1 : " << solve_part1(machines) << " | Part 2 : " << solve_part2(machines) << std::endl;

    return 0;
}
